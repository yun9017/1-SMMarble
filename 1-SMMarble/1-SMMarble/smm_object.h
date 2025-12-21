//
//  smm_object.h
//  SMMarble object
//
//  Created by Juyeop Kim on 2023/11/05.
//

#ifndef smm_object_h
#define smm_object_h

/* node type :
    lecture,
    restaurant,
    laboratory,
    home,
    experiment,
    foodChance,
    festival
*/
#define SMMNODE_TYPE_LECTURE            0
#define SMMNODE_TYPE_RESTAURANT         1
#define SMMNODE_TYPE_LABORATORY         2
#define SMMNODE_TYPE_HOME               3
#define SMMNODE_TYPE_GOTOLAB            4
#define SMMNODE_TYPE_FOODCHANCE         5
#define SMMNODE_TYPE_FESTIVAL           6

#define SMMNODE_OBJTYPE_BOARD   0
#define SMMNODE_OBJTYPE_GRADE   1
#define SMMNODE_OBJTYPE_FOOD    2
#define SMMNODE_OBJTYPE_FEST    3

/* grade :
    A+,
    A0,
    A-,
    B+,
    B0,
    B-,
    C+,
    C0,
    C-
*/

typedef enum // 열거형
{
    GRADE_AP,
    GRADE_A0,
    GRADE_AM,
    GRADE_BP,
    GRADE_B0,
    GRADE_BM,
    GRADE_CP,
    GRADE_C0,
    GRADE_CM
} smmGrade_e;


#define SMMNODE_MAX_GRADE       13


//object generation
void* smmObj_genObject(char* name, int objType, int type, int credit, int energy, int grade);
char* smmObj_getObjectName(void *ptr);
int smmObj_getNodeType(void *ptr);
int smmObj_getObjectEnergy(void *ptr);
char* smmObj_getTypeName(int node_type);
int smmObj_getNodeCredit(void *ptr);
int smmObj_getObjectGrade(void *ptr);

char* smmObj_getGradeName(smmGrade_e grade);
//member retrieving


//element to string



#endif /* smm_object_h */
