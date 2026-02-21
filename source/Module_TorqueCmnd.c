//2013-9-3 : completed ACDC torque command
//2013-9-3 : revised ACDC torque command
//2013-9-4 : use bit "CmdType" check mode
//2013-11-12 : debug->the system restart with ACC by move set UpdateCmd wothout state check

#include "F28x_Project.h"
#include "include\ND2.h"

struct TRQCMND_REG TrqCmnd_Reg={0,0,0,0,0,
								0}; 

//---External variable import--------------------------------------
struct data_grp{
	Uint16 value[100];
	Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber]; 
extern int flag_PWMEnable;
/////////////////////////////////////////////////////////////////////////////////
void TCommMngr(void)
{ 
struct {
	Uint16 disableTCommMngr:1;	//disable this module
	Uint16 analogsource:1;			//analog source
}flagTCommMngr={0}; 
_iq	target_output=0;

	if(ND2System_Reg.MAIN_FLAG2.bit.CmdType==0)		//non/vector control
	{
		flagTCommMngr.disableTCommMngr=1;
	}
//------------------------------------------------------------------
	if(flagTCommMngr.disableTCommMngr==0)
	{
		//---Select source of command
		switch(grp_num[5].value[37])	
		{
			case 1://Sp from A12
				target_output = Analog_reg.PU_B0;
				flagTCommMngr.analogsource = 1;
				break;
			case 2://Sp from A2
				target_output = (grp_num[0].value[14]==1)?Analog_reg.PU_B1:Analog_reg.VRKeypad;
				flagTCommMngr.analogsource = 1;
				break;
			case 3://Sp from A3
				target_output = Analog_reg.PU_B2;
				flagTCommMngr.analogsource = 1;
				break;
			default:
				target_output = TrqCmnd_Reg.TrqKey;
		}
		KeyPad_reg.GENERAL_FLAG.bit.Analog_Src =  flagTCommMngr.analogsource;
		TrqCmnd_Reg.TrqSp = target_output;
		//---ACDC command
		if(ND2System_Reg.STATE_FLAG.bit.ShtdwnOP==0)
		{
			if(ND2System_Reg.STATE_FLAG.bit.State == state_stopping)
			{
				if(grp_num[5].value[27]==1)
				{
					ND2System_Reg.STATE_FLAG.bit.ShtdwnOP = 1;
					goto chk_offpwm;
				}else
				{
					target_output = 0;		//target torque
				}
			}
			if(ND2System_Reg.STATE_FLAG.bit.State!=state_stop)
			{				
				if(BtStrp_reg.BTSTRP_FLAG1.bit.CmpltBtStrp==1)
				{
					TrqCmnd_Reg.TrqCommand = ACDCSCrv(target_output,TrqCmnd_Reg.TrqdAcc,TrqCmnd_Reg.TrqDec,1
												 		,TrqCmnd_Reg.TrqCommand,IQ17_1);//without s-curve
				}
			}
		}else
		{
		chk_offpwm:
			if(flag_PWMEnable == 2)//check off PWM(=2)
			{
				TrqCmnd_Reg.TrqCommand = 0;	//off PWM before T=0				
			}
		}
		ND2System_Reg.STATE_FLAG.bit.UpdateCmd = 1;			
	}
}
/////////////////////////////////////////////////////////////////////////////////













