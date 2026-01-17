//2012-8-17 : got PI
//2012-8-18 : got limit 2Hz
//2012-8-18 : completed stop between stall
//2012-8-20 : used bit check saturate
//2012-8-21 : Completed stall I
//2012-11-20 : add clear data stallV when inactive
//2012-12-1 : got enter PI stall V
//2012-12-1 : completed stallV
//2013-1-11 : only stall(stall I or stall V) can be active by disable stall V if stall is operating
//2013-1-11 : reset bit completed ACDC when first stop in stall I
//2013-6-17 : Disable StallI,V in vector mode
//2013-7-2 : remove multiply kp at I term
//2013-7-2 : revise add iq17 function with iq format

#include "F28x_Project.h"
#include "ND2.h"

struct data_grp{
	Uint16 value[100];
	Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber];  

long b          = 0;
long Stall_V    = 0;
long c          = 0;
char Flag_stall_V_Active = 0; 

/////////////////////////////////////////////////////////////////////////////////
void CurrentStall(void)
{		
_iq buffF    = MainIQ_Variable.FCommandA;
_iq output   = 0;
_iq PIOutput = 0;
/*struct {
	Uint16 savedata:1;
	Uint16 disablestalli:1;
}flagstallI={0};*/

	if(Flag_stall_V_Active == 0)//Stall V not Active or flying is working
	{

		if(ND2System_Reg.STATE_FLAG.bit.State == state_start ||
		   ND2System_Reg.STATE_FLAG.bit.State == state_run)
		{
			ND2System_Reg.FLAG_STALL.bit.StallFStop = 1;

			if(grp_num[4].value[0] == 0||
			   grp_num[5].value[0] == 4)//Vector control mode
			{
				//flagstallI.disablestalli = 1;
				ND2System_Reg.FLAG_STALL.bit.DisableStallI = 1;
			}
			//if(flagstallI.disablestalli == 0)
			if(ND2System_Reg.FLAG_STALL.bit.DisableStallI == 0)
			{
				if(ChkFault_Reg.FLAG_FAULT.bit.W_OC==1)
				{
					ND2System_Reg.FLAG_STALL.bit.StallIActive = 1;
				}
				if(ND2System_Reg.FLAG_STALL.bit.StallIActive==1)
				{
					//flagstallI.savedata = 1;
				    ND2System_Reg.FLAG_STALL.bit.SaveData_StallI = 1;
					PIOutput = PI_StallCurrent(_IQ17abs(buffF)); //PI for stall current

					if(PIOutput>_IQ17abs(buffF))
					{ 
						ND2System_Reg.FLAG_STALL.bit.StallIActive = 0;
						output = _IQ17abs(buffF);											
					}
					else
					{
						output = PIOutput;
					}
					buffF = (buffF<0)?_IQ17mpy(output,MinusIQ):output;
				}
			}
		}else
		{
			if(ND2System_Reg.FLAG_STALL.bit.StallFStop==1)//case of first stopping
			{
				MainIQ_Variable.FCommandA = MainIQ_Variable.FCommand;//for backing to normal ACDC

				buffF = MainIQ_Variable.FCommand;

				ND2System_Reg.FLAG_STALL.bit.StallFStop     = 0;
				ND2System_Reg.FLAG_SYST_CTRL.bit.CmpltdACDC = 0;
			}		
		}		
	}
	//if(flagstallI.savedata == 0)
	if(ND2System_Reg.FLAG_STALL.bit.SaveData_StallI == 0)
	{
		ClrDataStallI();
	}	
	MainIQ_Variable.FCommandB = buffF;
}	
///////////////////////////////////////////////////////////////////////////////// 
_iq PI_StallCurrent(_iq spstalli)
{
_iq ts_ti = 0;
_iq preoutput = 0;
_iq	Er_K = 0,tempP = 0,tempI = 0;

	ts_ti = (ND2System_Reg.StallI_Ti==0)?0:_IQ17div(TsPIDMain,ND2System_Reg.StallI_Ti);

	Er_K = ChkFault_Reg.I_Fault - ND2System_Reg.StallI_Sp; 	
	Er_K = _IQ17mpy(Er_K,MinusIQ);
	
	//cal P. : P = Kp*Er(k)
	tempP = IQmpyMin(ND2System_Reg.StallI_KP,Er_K);					//P_temp = Kp*Er(k)	

	//I term D-on error	
	if(ND2System_Reg.StallI_Ti == 0) 		
	{
		tempI = 0;
	}else
	{
		//***cal I. : I = Kp*[I(k-1) + Er(k)*Ts/Ti]***//
		//tempI = Er(k)*Ts/Ti	
		tempI = IQmpyMin(Er_K,ts_ti);

		//tempI = I(k-1)+Er(k)*Ts/Ti
		tempI = Add_IQ17(tempI,ND2System_Reg.StallI_IK_1);
		
		//Check saturation
		tempI = _IQsat(tempI, _IQ17(2),_IQ17(-2));//I_tempA;
	}		
	//Total Output before saturate = P+I																				
	preoutput = Add_IQ17(tempP,tempI);	//P+I	
	preoutput = spstalli+preoutput;
	
	if(preoutput<MainIQ_Variable.FSlip)
	{
		preoutput = MainIQ_Variable.FSlip;
	}
	ND2System_Reg.StallI_IK_1 = tempI;
	return preoutput;
}
/////////////////////////////////////////////////////////////////////////////////
void ClrDataStallI (void)
{
	ND2System_Reg.StallI_IK_1 = 0;
	ND2System_Reg.FLAG_STALL.bit.StallIActive = 0;
} 
/////////////////////////////////////////////////////////////////////////////////
//================================= STALL V ====================================
void VoltageStall(void)
{
_iq buffF       = MainIQ_Variable.FCommandB;
_iq buffOutput  = 0;
_iq Ts_Ti       = 0;
struct {
	Uint16 savedata:1;
	Uint16 disablestallv:1;
}flagstallv={0}; 
	
	if(grp_num[4].value[5] == 0 ||
	   ND2System_Reg.STATE_FLAG.bit.State == state_stop ||
	   ND2System_Reg.FLAG_STALL.bit.StallIActive==1||
	   grp_num[5].value[0] == 4)
	{
		flagstallv.disablestallv = 1;
	}
	if(flagstallv.disablestallv==0)//Over-Voltage stall enable
	{		
		if((VDC_Read_reg.VdcBus>=ND2System_Reg.Over_Volt_Stall) || Flag_stall_V_Active == 1)
		{			 		
			Ts_Ti = (ND2System_Reg.StallV_Ti==0)?0:_IQ17div(TsPIDMain,ND2System_Reg.StallV_Ti);
			Flag_stall_V_Active = 1;
			//Cal.P=Kp*Er(K)
			Parameter_stall_V.PI_Er = VDC_Read_reg.VdcBus - ND2System_Reg.Over_Volt_Stall;//Q17	
				
			//Cal.I=Kp*[Output(K-1)+Er(K)*(Ts/Ti)]
			Parameter_stall_V.PI_Output = Parameter_stall_V.PI_Output + _IQ17mpy(Parameter_stall_V.PI_Er,Ts_Ti);
			Parameter_stall_V.PI_Output = _IQ17mpy(Parameter_stall_V.PI_Output,ND2System_Reg.StallV_KP);
				 		
			if(UpperStallV <= Parameter_stall_V.PI_Output)
				Parameter_stall_V.PI_Output = UpperStallV;
			if(LowerStallV >= Parameter_stall_V.PI_Output)
				Parameter_stall_V.PI_Output = LowerStallV;  

//===================== I - Controller =============================
			buffOutput = _IQ17abs(buffF) + Parameter_stall_V.PI_Output;

			if(buffOutput>=MainIQ_Variable.PU_FMax)
			{
				buffOutput	= MainIQ_Variable.PU_FMax;
			}else if(buffOutput<=_IQ17abs(buffF))
			{
				buffOutput	= _IQ17abs(buffF);
				Flag_stall_V_Active = 0;

			}				  
			buffF = (MainIQ_Variable.FCommandB<0)?_IQ17mpy(buffOutput,MinusIQ):buffOutput;		
			flagstallv.savedata = 1;				
		}					
	}
	if(flagstallv.savedata == 0)
	{
		ClrDataStallV();
	}					
	MainIQ_Variable.FCommand = buffF;
}
//================================= STALL V ====================================
void ClrDataStallV(void)
{
    Flag_stall_V_Active         = 0;
	Parameter_stall_V.PI_Er     = 0;
	Parameter_stall_V.PI_Output = 0;
}
//---------------------------------------------------------------------------



