#include "F28x_Project.h"
#include "include\ND2.h"
#include <math.h>

struct LIFT_REG Lift_Reg	={0,0,0,0}; 

//---External variable import--------------------------------------
struct data_grp{
	Uint16 value[100];
	Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber];  
////////////////////////////////////////////////////////////////////////////////


