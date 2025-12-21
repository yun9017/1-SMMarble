//
//  main.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "/Users/hanseoyun/Desktop/1-SMMarble/marbleBoardConfig.txt"
#define FOODFILEPATH "/Users/hanseoyun/Desktop/1-SMMarble/marbleFoodConfig.txt"
#define FESTFILEPATH "/Users/hanseoyun/Desktop/1-SMMarble/marbleFestivalConfig.txt"


//board configuration parameters
static int smm_board_nr;
static int smm_food_nr;
static int smm_festival_nr;
static int smm_player_nr;

typedef struct{
    char name[MAX_CHARNAME];
    int pos;
    int credit;
    int energy;
    int flag_graduated;
    int flag_experiment; // 실험 중인지 확인
    int experiment_target; // 실험 성공 여부 판단 기준값
} smm_player_t;

smm_player_t *smm_players;

void generatePlayers(int n, int initEnergy); //generate a new player
void printPlayerStatus(void); //print all player status at the beginning of each turn
void graduationResult(void);


//function prototypes
void printGrades(int player) //print grade history of the player
{
    int size = smmdb_len(LISTNO_OFFSET_GRADE + player);
    int i;
    
    for (i=0; i<size; i++)
    {
        void* ptr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
        char* name = smmObj_getObjectName(ptr);
        int grade = smmObj_getObjectGrade(ptr);
        
        printf("%s : %d\n", name, grade);
    }
}


float calcAverageGrade(int player) //calculate average grade of the player
{
    int i;
    int size = smmdb_len(LISTNO_OFFSET_GRADE + player);
    int total = 0;
    
    for (i=0; i < size; i++)
    {
        void* ptr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
        total += smmObj_getObjectGrade(ptr); // 성적 모두 더하기
    }
    return (float)total / size; // (총점 / 과목 수) 계산해 실수형으로 변환
}

smmGrade_e takeLecture(int player, char *lectureName, int credit) //take the lecture (insert a grade of the player)
{
    smmGrade_e grade;
    void* gradePtr;
    
    grade = rand() % SMMNODE_MAX_GRADE; // 성적 랜덤 생성
    
    gradePtr = smmObj_genObject(lectureName, SMMNODE_OBJTYPE_BOARD, SMMNODE_TYPE_LECTURE, credit, 0, grade); // 성적 객체 생성
    smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr); // 플레이어 성적 리스트에 추가
    
    return grade;
}

void printGrades(int player); //print all the grade history of the player


void* findGrade(int player, char *lectureName) //find the grade from the player's grade history
{
    int size = smmdb_len(LISTNO_OFFSET_GRADE+player);
    int i;
    
    for(i=0;i<size;i++)
    {
        void *ptr = smmdb_getData(LISTNO_OFFSET_GRADE+player,i);
        if (strcmp(smmObj_getObjectName(ptr),lectureName) == 0)
        {
            return ptr;
        }
    }
    
    return NULL;
}

int isGraduated(void) //check if any player is graduated
{
    int i;
    for (i=0;i<smm_player_nr;i++)
    {
        if (smm_players[i].flag_graduated == 1)
            return 1;
    }
    
    return 0;
}

void goForward(int player, int step) //make player go "step" steps on the board (check if player is graduated)
{
    int i;
    void *ptr;
    
    //player_pos[player] = player_pos[player] + step;
    ptr = smmdb_getData(LISTNO_NODE, smm_players[player].pos);
    printf("start from %i(%s) (%i)\n", smm_players[player].pos, smmObj_getObjectName(ptr), step);
    
    for (i=0;i<step;i++)
    {
        smm_players[player].pos = (smm_players[player].pos + 1)%smm_board_nr;
        printf(" => moved to %i(%s)\n", smm_players[player].pos, smmObj_getObjectName(ptr));
    }
}

void printPlayerStatus(void)
{
    int i;
    void *ptr;
    
    for(i=0;i<smm_player_nr;i++)
    {
        ptr = smmdb_getData(LISTNO_NODE, smm_players[i].pos); // 현재 플레이어의 위치에 있는 노드 가져오기
        
        printf("%s - position:%i(%s), credit:%i, energy:%i\n",
               smm_players[i].name, smm_players[i].pos, smmObj_getObjectName(ptr),smm_players[i].credit, smm_players[i].energy);
    }
}

void generatePlayers(int n, int initEnergy)
{
    int i;
    
    smm_players = (smm_player_t*)malloc(n*sizeof(smm_player_t));
    
    for(i=0;i<n;i++)
    {
        smm_players[i].pos = 0;
        smm_players[i].credit = 0;
        smm_players[i].energy = initEnergy;
        smm_players[i].flag_graduated = 0;
        
        printf("Input %i-th player name: ", i);
        scanf("%s", &smm_players[i].name[0]);
        fflush(stdin);
    }
}


int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    
    if (c == 'g')
        printGrades(player);
    
    return (rand() % MAX_DIE + 1);
}

void graduationResult(void)
{
    int i, j;
    
    for (i=0; i<smm_player_nr; i++)
    {
        if (smm_players[i].flag_graduated == 1)
        {
            int size = smmdb_len(LISTNO_OFFSET_GRADE + i);
            
            printf("Player %d Graduation \n", i);
            
            for (j=0; j<size; j++)
            {
                void *ptr = smmdb_getData(LISTNO_OFFSET_GRADE + i, j);
                
                if(ptr == NULL)
                    break;
                
                char *lectureName = smmObj_getObjectName(ptr);
                int credit = smmObj_getNodeCredit(ptr);
                int grade = smmObj_getObjectGrade(ptr);
                
                printf("Lecture : %s, Credit: %d, Grade: %d\n", lectureName, credit, grade);
            }
        }
    }
}

//action code when a player stays at a node
void actionNode(int player)
{
    void *ptr = smmdb_getData(LISTNO_NODE,smm_players[player].pos);
    
    int type = smmObj_getNodeType(ptr);
    int credit = smmObj_getNodeCredit(ptr);
    int energy = smmObj_getObjectEnergy(ptr);
    int grade;
    void *gradePtr;
    
    printf("--> player%i pos : %i, type : %s, credit : %i, energy : %i\n",
           player, smm_players[player].pos, smmObj_getTypeName(type), credit, energy);
    
    switch(type)
    {
        case SMMNODE_TYPE_LECTURE:
            // 플레이어가 이미 강의를 수강했는지 확인
            if (findGrade(player , smmObj_getObjectName(ptr)) == NULL)
        {
            smm_players[player].credit += credit;
            smm_players[player].energy -= energy;
            
            grade = rand()%SMMNODE_MAX_GRADE;
            
            gradePtr = smmObj_genObject(smmObj_getObjectName(ptr), SMMNODE_OBJTYPE_BOARD, type, credit, energy, grade);
            smmdb_addTail(LISTNO_OFFSET_GRADE+player, gradePtr);
        }
            break;
            
        case SMMNODE_TYPE_RESTAURANT:
            smm_players[player].energy += energy;
            break;
            
        case SMMNODE_TYPE_LABORATORY:
            if (smm_players[player].flag_experiment == 1) // 실험 중 상태일 때만 주사위 굴려 이동 시도
            {
                smm_players[player].energy -= energy; // 실험 시도할 때마다 에너지 소모, 음수 가능
                
                int dice = (rand() % MAX_DIE) + 1;
                if (dice >= smm_players[player].experiment_target)
                {
                    smm_players[player].flag_experiment = 0; // 성공 했을 때 실험 종료 상태로 전환
                }
            }  // 실패 시 이동 못하고 머무름
            break;
            
        case SMMNODE_TYPE_HOME:
            smm_players[player].energy += energy;
            if (smm_players[player].credit <= GRADUATE_CREDIT)
            {
                smm_players[player].flag_graduated = 1;
            }
            break;
            
        case SMMNODE_TYPE_GOTOLAB:
            smm_players[player].flag_experiment = 1; // 실험 중 상태로 전환
            
            smm_players[player].experiment_target = (rand() % MAX_DIE) + 1; // 실험 성공 기준값은 주사위 범위 내에서 랜덤 지정됨
            
            for (int i = 0; i < smm_board_nr; i++)
            {
                void *nodePtr = smmdb_getData(LISTNO_NODE, i); // 실험실 위치로 이동하도록
                if (smmObj_getNodeType(nodePtr) == SMMNODE_TYPE_LABORATORY)
                {
                    smm_players[player].pos = i;
                    break;
                }
            }
            break;
            
        case SMMNODE_TYPE_FOODCHANCE:
            int food_count = smmdb_len(LISTNO_FOODCARD); // 음식카드 총 개수 파악
            
            if (food_count > 0)
            {
                int rand_index = rand() % food_count;
                void *foodObj = smmdb_getData(LISTNO_FOODCARD, rand_index); // 랜덤 인덱스 선택과 해당 음식 선택
                
                smm_players[player].energy += smmObj_getObjectEnergy(foodObj); // 명시된 보충 에너지를 현재 에너지에 더함
            }
            break;
            
        case SMMNODE_TYPE_FESTIVAL:
            int fest_count = smmdb_len(LISTNO_FESTCARD);
            
            if (fest_count > 0)
            {
                int rand_index = rand() % fest_count;
                void *festObj = smmdb_getData(LISTNO_FESTCARD, rand_index);
            }
            break;
        //case lecture:
        default:
            break;
    }
}


int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int turn;
    
    smm_board_nr = 0;
    smm_food_nr = 0;
    smm_festival_nr = 0;
    
    srand(time(NULL));
    

    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n");
    while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4) //read a node parameter set
    {
        //store the parameter set
        void* ptr;
        printf("%s %i %i %i\n", name, type, credit, energy);
        ptr = smmObj_genObject(name,SMMNODE_OBJTYPE_BOARD, type, credit, energy, 0);
        smm_board_nr = smmdb_addTail(LISTNO_NODE, ptr);
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", smm_board_nr);
    
    
    //2. food card config
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n");
    while (fscanf(fp, "%s %i", name, &energy) == 2) //read a food parameter set (이름, 에너지 수치)
    {
        //store the parameter set
        void* ptr;
        printf("%s %i\n", name, energy);
        ptr = smmObj_genObject(name, SMMNODE_OBJTYPE_FOOD, 0, 0, energy, 0);
        smm_food_nr = smmdb_addTail(LISTNO_FOODCARD, ptr);
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", smm_food_nr);

    
    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n");
    while (fscanf(fp, "%s", name) == 1) //read a festival card string (미션 내용 문자열)
    {
        //store the parameter set
        void* ptr;
        printf("%s\n", name);
        ptr = smmObj_genObject(name, SMMNODE_OBJTYPE_FEST, 0, 0, 0, 0);
        smm_festival_nr = smmdb_addTail(LISTNO_FESTCARD, ptr);
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", smm_festival_nr);
    
    
    //2. Player configuration ---------------------------------------------------------------------------------
    
    do
    {
        //input player number to player_nr
        printf("Input player number:");
        scanf("%i", &smm_player_nr);
        fflush(stdin);
        
        if (smm_player_nr <= 0 || smm_player_nr > MAX_PLAYER)
            printf("Invalid player number!\n");
    }
    while (smm_player_nr <= 0 || smm_player_nr > MAX_PLAYER);
    
    generatePlayers(smm_player_nr,smmObj_getObjectEnergy(smmdb_getData(SMMNODE_OBJTYPE_BOARD, 0)));

    
    turn = 0;
    
    //3. SM marble game starts ---------------------------------------------------------------------------------
    while (isGraduated() == 0) //is anybody graduated?
    {
        int die_result;
        
        //4-1. initial printing
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)
        die_result = rolldie(turn);
        
        //4-3. go forward
        goForward(turn, die_result);
        //pos = pos+2;
        
		//4-4. take action at the destination node of the board
        actionNode(turn);
        
        //4-5. next turn
        turn = (turn + 1)%smm_player_nr;
    }
    
    graduationResult();
    free(smm_players);
    
    system("PAUSE");
    return 0;
}
