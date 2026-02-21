//2012-7-23 : completed acc %Vout
//2012-7-24 : completed fly stage2
//2012-7-24 : got decel fsearch
//2012-7-24 : completed fly stage3
//2012-7-24 : revised replace Fsearch by Fcommand
//2012-8-10 : revised 5stages and reverse dir.
//2012-8-11 : revised stage3 
//2012-8-11 : revised stage5
//2012-8-14 : revised checking Irms increase/decrease
//2012-8-15 : just detect current but need to check gain
//2012-8-15 : completed Flying start
//2012-8-15 : revised no divide F004 for  check i
//2012-8-30 : use bit lock fly for disable flying func.
//2012-10-8 : no use Fk-1 as Fcommand
//2012-10-13 : debugged Vaccel after search
//2012-10-15 : revised equation Vtarget after search
//2012-11-22 : add check Vsrch lower or higher than vtarget
//2012-11-22 : revised process accel/decel V after search
//2012-11-23 : revised process accel/decel V after search->Tsmap=1ms
//2012-11-26 : revised variable V to Vtarget
//2012-11-27 : revised F after search FcommandA=Fk-1
//2012-11-27 : revised delta cutI inc=0.005,cutI dec=0.001
//2013-4-24 : use IV_Read_reg.BuffIp instead of Irms(no LPF)
//2013-4-25 : debug clear "FlyStrt_Reg.FFly_K_1 = 0" when decel FcommA=0
//2013-4-25 : revise algorithm with no LPF I
//2013-5-21 : revise use Ipeak, V after search refered by V_V_PER_F, backup Ik-1 when decreasing
//2013-7-1 : fslip variable
//2013-7-17 : revise stage3

#include "F28x_Project.h"
#include "include\ND2.h"


struct FLYINGSTART FlyStrt_Reg={0,0,0,0,0,
								0,0,0,0};							  
//-----------------------------------------------------------------
//---External variable import--------------------------------------
struct data_grp{
	Uint16 value[100];
	Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber];

extern long Volt_VperFProfile;
extern _iq17 Vrate;
extern _iq17 Frate;
//-----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////
void FlyingStart_Function(_iq target_output1)						
{
//_iq FSlip = _IQ17div(_IQ17(2),MainIQ_Variable.FRef);//slip 2hz
_iq buff = 0,buffVtarget=0,buffvref=0;

	if(grp_num[4].value[18]==0 ||
	   FlyStrt_Reg.Flag_FlyStrt.bit.LockFly != 0)
	{//case of Flying-Start is inactive
		FlyStrt_Reg.Flag_FlyStrt.bit.FlyActive = 0;		
	}else
	{
		FlyStrt_Reg.Flag_FlyStrt.bit.FlyActive = 1;				//System is flying indicator
		buff = MainIQ_Variable.PU_FMax+MainIQ_Variable.FSlip;
		buff = (target_output1<0)?_IQ17mpy(buff,MinusIQ):buff;	//check direction of target output
		
		buffVtarget = _IQ17div(Volt_VperFProfile,Vrate);
		//----------------------------------------------------
		if(FlyStrt_Reg.Flag_FlyStrt.bit.FlyStage==0)
		{
			MainIQ_Variable.FCommandA = buff;
			if(FlyStrt_Reg.TDelayVout>=TdelayAccVout)
			{
				FlyStrt_Reg.FlyStrt_Timer.bit.T_Smp_AccVop = TsCntr30;
				FlyStrt_Reg.Flag_FlyStrt.bit.FlyStage = 1;
				FlyStrt_Reg.TDelayVout = 0;						//clear timer for support next stage
			}else
			{
				FlyStrt_Reg.TDelayVout++;
			}
		//----------------------------------------------------
		}else if(FlyStrt_Reg.Flag_FlyStrt.bit.FlyStage==1)		
		{
			if(FlyStrt_Reg.VOutputSearch >= FlyStrt_Reg.LevelVSearch)//check Voutput to up stage		
			{
				FlyStrt_Reg.VOutputSearch = FlyStrt_Reg.LevelVSearch;//Level stage is %value
				MainIQ_Variable.FCommandA = buff;
				FlyStrt_Reg.Flag_FlyStrt.bit.FlyStage = 2;
			}else
			{					
				if(FlyStrt_Reg.FlyStrt_Timer.bit.T_Smp_AccVop >= TsCntr30)//tsampling = 30ms			
				{
					FlyStrt_Reg.FlyStrt_Timer.bit.T_Smp_AccVop = 0;
					FlyStrt_Reg.VOutputSearch = _IQ17(Accel(_IQ17toF(FlyStrt_Reg.VOutputSearch),
						     		  						_IQ17toF(FlyStrt_Reg.LevelVSearch),TaccelV,
						     		  						_IQ17toF(FlyStrt_Reg.LevelVSearch),0.03));
				}else
				{
					FlyStrt_Reg.FlyStrt_Timer.bit.T_Smp_AccVop++;
				}
			}
		//----------------------------------------------------
		}else if(FlyStrt_Reg.Flag_FlyStrt.bit.FlyStage==2)
		{
			if(FlyStrt_Reg.TDelayVout >= TdelayVout)
			{				
				FlyStrt_Reg.IFly_K_1 = IV_Read_reg.I_Peak;
				FlyStrt_Reg.FFly_K_1 = MainIQ_Variable.FCommandA;
				MainIQ_Variable.FCommandA = _IQ17(Decel(_IQ17toF(MainIQ_Variable.FCommandA),0,
												  _IQ17toF(FlyStrt_Reg.TDecelSearch),
												  _IQ17toF(_IQ17abs(buff)),0.03));
				FlyStrt_Reg.FlyStrt_Timer.bit.T_Smp_DecFop = 0;				
				FlyStrt_Reg.Flag_FlyStrt.bit.FlyStage = 3;
			}else
			{
				FlyStrt_Reg.TDelayVout++;
			}
		//----------------------------------------------------
		}else if(FlyStrt_Reg.Flag_FlyStrt.bit.FlyStage==3)
		{
			if(MainIQ_Variable.FCommandA==0)
			{
				FlyStrt_Reg.IFly_K_1 = IV_Read_reg.I_Peak;
				FlyStrt_Reg.FFly_K_1 = MainIQ_Variable.FCommandA;
				goto upstg4;			
			}else if(FlyStrt_Reg.Flag_FlyStrt.bit.IDecrease == 0)			
			{				
				if(IV_Read_reg.I_Peak>FlyStrt_Reg.IFly_K_1)
				{
					if(IV_Read_reg.I_Peak-FlyStrt_Reg.IFly_K_1>=cutI)
					{
						FlyStrt_Reg.IFly_K_1=IV_Read_reg.I_Peak;	
					}					
					DecFFly(_IQ17abs(buff));					
				}else if((FlyStrt_Reg.IFly_K_1-IV_Read_reg.I_Peak)>=cutI)
				{						
					FlyStrt_Reg.Flag_FlyStrt.bit.IDecrease = 1;								
					FlyStrt_Reg.IFly_K_1 = IV_Read_reg.I_Peak;
					FlyStrt_Reg.FFly_K_1 = MainIQ_Variable.FCommandA;
				}else
				{			
					DecFFly(_IQ17abs(buff));		
				}
			}else
			{
				if(IV_Read_reg.I_Peak<FlyStrt_Reg.IFly_K_1)
				{
					DecFFly(_IQ17abs(buff));
				}else if(IV_Read_reg.I_Peak-FlyStrt_Reg.IFly_K_1>=cutI1)
				{	
upstg4:				FlyStrt_Reg.Flag_FlyStrt.bit.FlyStage = 4;
					buffvref = buffVtarget-FlyStrt_Reg.VOutputSearch;						
					FlyStrt_Reg.VReffSrch = _IQ17abs(buffvref);
					FlyStrt_Reg.FlyStrt_Timer.bit.T_Smp_AccVop = TsCntr30;	
					MainIQ_Variable.FCommandA = FlyStrt_Reg.FFly_K_1;		
				}else
				{
					DecFFly(_IQ17abs(buff));					
				}												
			}
		//----------------------------------------------------
		}else if(FlyStrt_Reg.Flag_FlyStrt.bit.FlyStage == 4)
		{	
			if(FlyStrt_Reg.VOutputSearch == buffVtarget)
			{
				FlyStrt_Reg.Flag_FlyStrt.bit.FlyStage = 5;
				FlyStrt_Reg.Flag_FlyStrt.bit.LockFly  = FlyStrt_Reg.Flag_FlyStrt.bit.LockFly | 0x2;
			}else
			{
				if(FlyStrt_Reg.VOutputSearch<buffVtarget)
				{
					FlyStrt_Reg.VOutputSearch = _IQ17(Accel(_IQ17toF(FlyStrt_Reg.VOutputSearch),
					     		  				  _IQ17toF(buffVtarget),TaccelV1,
					     		  				  _IQ17toF(FlyStrt_Reg.VReffSrch),0.001));	
				}else
				{
					FlyStrt_Reg.VOutputSearch = _IQ17(Decel(_IQ17toF(FlyStrt_Reg.VOutputSearch),
					     		  				  _IQ17toF(buffVtarget),TaccelV1,
					     		  				  _IQ17toF(FlyStrt_Reg.VReffSrch),0.001));
				}					
			}
			
		}
	}	
}
/////////////////////////////////////////////////////////////////////////////////
void DecFFly(_iq reff)
{
	FlyStrt_Reg.FlyStrt_Timer.bit.T_Smp_DecFop++;	
	if(FlyStrt_Reg.FlyStrt_Timer.bit.T_Smp_DecFop >= TsCntr30)//tsampling = 30ms			
	{
		FlyStrt_Reg.FlyStrt_Timer.bit.T_Smp_DecFop = 0;					
		FlyStrt_Reg.FFly_K_1 = MainIQ_Variable.FCommandA;
		MainIQ_Variable.FCommandA = _IQ17(Decel(_IQ17toF(MainIQ_Variable.FCommandA),0,
									  _IQ17toF(FlyStrt_Reg.TDecelSearch),
									  _IQ17toF(reff),0.03));
	}
}
/////////////////////////////////////////////////////////////////////////////////
