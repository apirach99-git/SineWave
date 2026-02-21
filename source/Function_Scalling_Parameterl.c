//2012-10-29 : cal I
//2012-11-07 : debugged port A2D and I sensor
//2012-11-12 : add offset of each I phase while stop
//2013-1-11 : remove check minimum i when stop
//2013-4-18 : remove unnecessary scalling I and add cheking bit"OffsetA2DI" before cal. I phase

#include "F28x_Project.h"
#include "include\ND2.h"



void Scalling_Parameter(void);



extern int isd_LPF ,isq_LPF ;
//---External variable import--------------------------------------
extern Uint16 arA2dBuff[];
struct data_grp{
	Uint16 value[100];
	Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber]; 

//-----------------------------------------------------------------
void Scalling_Parameter(void)
{
} 






