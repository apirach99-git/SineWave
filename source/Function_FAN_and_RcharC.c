//2012-11-20 : add function enable 5V I/O after completed RCharge C counter
//2012-11-21 : only real test waiting for RC delay for enabling 5V OP
//2013-1-10 : Fan with min
//2013-4-1 : Revise RchargeC relative with LU(VDC),scan loop 1ms
//2013-4-24 : debug off RchargeC after LU and state stop
//2013-5-6 : replace bit test run with "MAIN_FLAG2[TestNoFault]"
//2013-7-19 : add hysteresis for RchargeC

#include "F28x_Project.h"
#include "include\ND2.h"
#include "f28004x_sw_prioritized_isr_levels.h"

//void Funtion_FAN_and_RchargeC(void);

int FAN = 0;
int Fan_Time_sec = 0,Fan_Time_min=0,Fan_Time=0;
extern int First_Test_ND2;
//---External variable import--------------------------------------
struct data_grp{
	Uint16 value[100];
	Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber]; 
Uint16 RamTestRC=0;
_iq17	RamTestRC1=0;
void Funtion_FAN_and_RchargeC(void)
{
_iq17 buffdata=0;
struct{
	Uint16 rcon_off:1;//flag R/C on off
}flagfanrc={0};		
//-----------------------------------------------------------------------		
	if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
	{	//Fan Control			
		if(ND2System_Reg.STATE_FLAG.bit.State != state_stop)
   		{
			FAN = 1;
   			Fan_Time_sec = 0;
   			Fan_Time_min = 0;
   			Fan_Time = 0;
   		}else if(ND2System_Reg.STATE_FLAG.bit.State == state_stop && FAN == 1)
   		{
   			if(grp_num[0].value[13]!=0)
   			{
   				Fan_Time_sec++;
   				if(Fan_Time_sec>=1000)
   				{
   					Fan_Time_sec = 0;
   					Fan_Time_min++;

   					if(Fan_Time_min>=60)
   					{
   						Fan_Time_min = 0;
   						Fan_Time++;
   						if(Fan_Time>=grp_num[0].value[13])
   						{
   							goto offfan;
   						}
   					}
   				}
   			}else
   			{
offfan:
   				FAN = 0;
   				Fan_Time_sec = 0;
   				Fan_Time_min = 0;
   				Fan_Time = 0;
   			}		 
   		}				
/////////////////////////////////////////////////////////////////////////////////////////
		//	Delay time RchargeC		
		//------------------------------------------------		
		if(First_Test_ND2 == 0&&ND2System_Reg.MAIN_FLAG2.bit.TestNoFault==0)
		{//case of real test
			if(VDC_Read_reg.VdcBus<ChkFault_Reg.Level_LU)
			{
				if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)
				{
					ND2System_Reg.CntrTimeDelayRC = 0;
					ND2System_Reg.STATE_FLAG.bit.CmpltdRC = 0;		//completed R charge C
				}							
			}else if(ND2System_Reg.STATE_FLAG.bit.CmpltdRC==0)
			{
				if(ChkFault_Reg.DATA_FAULT.bit.Fault_LU==0&&
				   (VDC_Read_reg.VdcBus>ChkFault_Reg.Level_LU+VOutDecLU))
				{										
					buffdata = _IQ17div(_IQ17(ND2System_Reg.CntrTimeDelayRC),_IQ17(1000));
					if(buffdata>=MainIQ_Variable.Time_RelayRC)
					{
						ND2System_Reg.STATE_FLAG.bit.CmpltdRC = 1;		//completed R charge C
					}else
					{
						ND2System_Reg.CntrTimeDelayRC++;
					}					
				}else
				{
					ND2System_Reg.CntrTimeDelayRC=0;
				}
			}
			flagfanrc.rcon_off = ND2System_Reg.STATE_FLAG.bit.CmpltdRC;
		}else
		{//case of first test
			ND2System_Reg.STATE_FLAG.bit.CmpltdRC = 1;						
			flagfanrc.rcon_off = FAN;
		}		
	}
	if(FAN==0)
	{
        //GpioDataRegs.GPACLEAR.bit.GPIO18 = 1;
		//GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;
		GPIO_writePin(22, 0);
	}else
	{
        //GpioDataRegs.GPASET.bit.GPIO18 = 1;
		//GpioDataRegs.GPASET.bit.GPIO22 = 1;
        GPIO_writePin(22, 1);
	}	
	//------------------------------------------------
	if(flagfanrc.rcon_off==0)
	{
        //GpioDataRegs.GPBCLEAR.bit.GPIO34=1;
		//GpioDataRegs.GPACLEAR.bit.GPIO15=1;
        GPIO_writePin(15, 0);
	}else
	{
        //GpioDataRegs.GPBSET.bit.GPIO34 = 1;
		//GpioDataRegs.GPASET.bit.GPIO15 = 1;
		GPIO_writePin(15, 1);
	}					
} 
/////////////////////////////////////////////////////////////////////////////////////////
