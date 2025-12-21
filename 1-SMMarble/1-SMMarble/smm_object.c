//
//  smm_node.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODENR      100
#define MAX_NODETYPE    7



#define GRADE_AP        0
#define GRADE_A0        1
#define GRADE_AM        2
#define GRADE_BP        3
#define GRADE_B0        4
#define GRADE_BM        5
#define GRADE_CP        6
#define GRADE_C0        7
#define GRADE_CM        8


static char smmObj_nodeName[MAX_NODETYPE][MAX_CHARNAME] = {
    "lecture",
    "restaurant",
    "laboratory",
    "home",
    "gotolab",
    "foodchance",
    "festival"
};

static char smmObj_gradeName[SMMNODE_MAX_GRADE][MAX_CHARNAME] = {
    "A+",
    "A0",
    "A-",
    "B+",
    "B0",
    "B-",
    "C+",
    "C0",
    "C-"
};
//structure type definition
typedef struct{
    char name [MAX_CHARNAME];
    int objType;
    int type;
    int credit;
    int energy;
    int grade;
} smmObj_object_t;


//object generation
void* smmObj_genObject(char* name, int objType, int type, int credit, int energy, int grade)
{
    smmObj_object_t* ptr;
    
    ptr = (smmObj_object_t*)malloc(sizeof(smmObj_object_t));
    
    strcpy(ptr->name,name);
    ptr->type = type;
    ptr->objType = objType;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
    
    return ((void*)(ptr));
}



//member retrieving
char* smmObj_getObjectName(void *ptr)
{
    smmObj_object_t* objPtr = (smmObj_object_t*)ptr;
    return (objPtr->name);
}

int smmObj_getNodeType(void *ptr)
{
    if(ptr == NULL) // 유효하지 않은 데이터일 때 포인터 접근 방지
        return -1;
    
    smmObj_object_t* objPtr = (smmObj_object_t*)ptr;
    return (objPtr->type);
}

int smmObj_getNodeCredit(void *ptr)
{
    if(ptr == NULL)
        return -1;
    
    smmObj_object_t* objPtr = (smmObj_object_t*)ptr;
    return (objPtr->credit);
}

int smmObj_getObjectEnergy(void *ptr)
{
    if(ptr == NULL)
        return -1;
    
    smmObj_object_t* objPtr = (smmObj_object_t*)ptr;
    return (objPtr->energy);
}

char* smmObj_getTypeName(int node_type)
{
    return (smmObj_nodeName[node_type]);
}

int smmObj_getObjectGrade(void *ptr)
{
    smmObj_object_t* objPtr = (smmObj_object_t*)ptr;
    return (objPtr->grade);
}


//element to string
#if 0
char* smmObj_getNodeName(smmNode_e type)
{
    return smmObj_nodeName[type];
}
#endif
char* smmObj_getGradeName(smmGrade_e grade)
{
    return smmObj_gradeName[grade];
}

