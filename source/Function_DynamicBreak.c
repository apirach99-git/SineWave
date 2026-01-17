//2013-5-14 : remove hysteresys
#include "F28x_Project.h"
#include "ND2.h"

void DynamicBreak(void);
void OFF_R_Break(void);
struct data_grp{
   Uint16 value[100];
   Uint16 factor[100];
};
extern struct data_grp grp_num[8] ;   

int DynamicBrk = 0;

extern int First_Test_ND2 ;

void DynamicBreak(void)
{
    //Start Dynamic Break
    if(grp_num[4].value[9] == 1)//Dynamic Break Enable
    {
        grp_num[0].value[17] = 0;

        if(VDC_Read_reg.VdcBus>=ND2System_Reg.DynamicBrkLevel)//650V * Q17;VDC_Read_reg.VdcBus
        {
            DynamicBrk = 1;//DynamicBrk ON
        }else
        {
            DynamicBrk = 0;//DynamicBrk OFF
        }

        if(DynamicBrk == 1)
        {
            GpioDataRegs.GPATOGGLE.bit.GPIO14 = 1;//Enable Toggle Dynamic Break  *******Real test
        }
        else
        {
            OFF_R_Break();
        }
    }

    else if (grp_num[0].value[17]==1)
    {
        GpioDataRegs.GPATOGGLE.bit.GPIO14 = 1;//Enable Toggle Dynamic Break  *******Real test
    }

    else//Disable Dynamic Break
    {
        OFF_R_Break();
    }
//End Dynamic Break 
} 
/////////////////////////////////////////////////////////////////////
void OFF_R_Break(void)
{

    //GpioDataRegs.GPATOGGLE.bit.GPIO14 = 0;//UnToggle R Break
    if(grp_num[0].value[14] == 1)//Model inverter LCD
    {//GpioDataRegs.GPASET.bit.GPIO27 = 1;//R Break OFF
        //GpioDataRegs.GPASET.bit.GPIO14 = 1;//R Break OFF
        GpioDataRegs.GPATOGGLE.bit.GPIO14 = 0;//UnToggle R Break
        GPIO_writePin(14, 1);
    }
    else if (grp_num[0].value[14] == 0)//7-Seg(Small Size)  Lower 5 HP
    {//GpioDataRegs.GPACLEAR.bit.GPIO27 = 1;//R Break OFF
        //GpioDataRegs.GPACLEAR.bit.GPIO14 = 1;//R Break OFF
        GpioDataRegs.GPATOGGLE.bit.GPIO14 = 0;//UnToggle R Break
        GPIO_writePin(14, 0);
    }
}  
/////////////////////////////////////////////////////////////////////

//VDC_Read_reg.VdcBus
