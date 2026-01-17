//completed : t sampling PID 1ms
//completed : select chanel of Fb
//completed : check limit %Fb
//completed : multiply by gain of Fb
//completed : selection of setpoint exept multistep F.
//completed : multiplu by gain of SP
//completed : check limit of SP
//completed : check mode trim/setpoint
//use : conversed IQ from keypad.c
//completed : P of d-on error
//calculated : PID on error
//seperated : I,D term between D-on error & D-on feedback
//calculated : PI D-on feedback
//calculated : PID D-on feedback
//completed : increasing limit by time F619 with IQ24&IQ17
//revised : limit time with Ts of increasing 30ms and all data = IQ17
//calculated : limit lower&upper
//switched between IQ15/IQ17 for Td/Ti
//completed : PI IQ15/IQ17 d-on feedback
//completed : PI IQ15/IQ17 d-on feedback
//completed : PID IQ15/IQ17 d-on error
//revised : if Ti & Td = 0, the controller will implement i_temp & d_temp =0
//revised : avoid maxD d-on error
//revised : avoid maxD d-on error
//revised : Kc*SatEr with saturation
//revised : Adding with saturation
//revised : avoid sat. of I in d-on feedback
//revised : avoid sat. of I,D in d-on feedback
//completed : PID calculation with against the overflow IQ17
//completed : selector of multiF source
//revised : selector of MultF as array
//completed : Sp source from multF(F604=4)
//completed : Sp source from multiF
//revised : limit time:no division if time limit = 0 
//completed : accel/decel Sp
//completed : accel Sp if PUSP=0
//debugged : accel/decel
//completed : run PID with on/off
//revised : function Accel_Decel with argument
//added : PID_Reg.FLAG_PID.bit.Chk_AccDec = 1
//completed : run "sp_cond" for display SP when stop status
//revised : acc/dec to approach the target
//completed : compensation SP
//revised : accel operation with counter Ts outside loop for avoid the IQ converse effect
//completed : tune max/min Setpoint
//2012-6-2 : revised : move clear dynamax/dynamin into after stopping(stop state)
//2012-6-11 : revised selector of FSet to single module"SelectorFSet"
//2012-6-11 : revised accel setpoint 
//2012-6-11 : completed revising the acc/dec of PU SP
//2012-6-11 : completed adding Feedback/CalPID
//2012-6-12 : completed adding acc/dec output
//2012-6-12 : removed clearing dynamax/dynamin 
//2012-6-14 : cheak whether PID disable or not?
//2012-6-14 : revise Finput because Sptemp need to to show in normal mode
//2012-6-14 : remove SpTemp=0 in stopping
//2012-6-14 : added part intersection with normal mode when up/down
//2012-6-15 : remove acc/dec op in calPID
//2012-6-16 : revise calPID 
//2012-6-16 : remove acc/dec output into system control
//2012-6-18 : remove skip function
//2012-6-19 : revised acc/dec Sp PID without S-Curve
//2012-6-19 : global acc/dec tsampling ts_accdecT
//2012-6-21 : added refular multi F in switch
//2012-7-2 : replace deltaF to IQ(1)in acc/dec
//2012-7-2 : add inverse index of SelectorFSet
//2012-7-2 : use buffer of KeyPad_reg.GENERAL_FLAG.bit.FKey_active
//2012-7-2 : revised Selector SP
//2012-7-2 : completed trim mode
//2012-7-9 : added function terminal FWD/REV
//2012-7-10 : added function DIN Current input signal in selector SP
//2012-7-11 : added bit check update output for FCommMngr
//2012-7-20 : move check feedback out of start/run and no clear if the system is stopping
//2012-7-23 : add refference in acc() function
//2012-7-24 : add refference in dec() function
//2012-7-27 : revised function "ChkRngF" without IQtoF in argument
//2012-7-27 : debugged add Iterm if iterm=0 in d-on error
//2012-7-30 : revised Iterm multoply with check minimum resolution
//2012-9-4 : added function select source with model F015
//2012-9-5 : move clearing data stopping/stop to sttsctrl.c14 
//2012-11-14 : revised trim mode after check limit PID and sat data
//2012-11-14 : debugged while system = stop,load timer sampling
//2012-11-17 : revised D,I of D on Feedback
//2012-11-19 : revised adding IQ17,I Don error
//2012-11-23 : revised acc/dec of sp PID by adding Tsmp
//2013-1-10 : revised "SelectorFSet" with real time up/down key
//2013-1-21 : revised "SelectorFSet" DIN(X1-X4) != 0,sp follow mulF
//2013-1-21 : debugged SelectorFSet, src_mulF=0
//2013-5-21 : debugged accel/decel with minimum iq mpy
//2013-7-2 : revise addIQ17 function with iq format
//2013-7-10 : revise selector source multiF
//2013-8-16 : revise trim/sp mode after limit PID
//2013-9-2 : revise disable PID
    

#include "F28x_Project.h"
#include "ND2.h"
#include "f28004x_sw_prioritized_isr_levels.h"


struct PID_REG PID_Reg={0,0,0,0,0,
						0,0,0,0,0,
						0,0,0,0,0,
						0,0,0,0,0,
						0,0,0,0,0,
						0,0,0,0,0}; 

_iq arrPID_MulF[]={0,0,0,0,0,
				   0,0,0,0,0,
				   0,0,0,0,0};//Multi-setpoint of PID

//---External variable import--------------------------------------
struct data_grp{
	Uint16 value[100];
	Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber]; 
/////////////////////////////////////////////////////////////////////////////////
void interrupt_timerPID(void)						
{
struct {
	Uint16 disablePID:1;	//disable this module
}flagpid={0}; 

	if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0||
	   grp_num[7].value[0]==0||
	   grp_num[5].value[0]==4)
	{
		flagpid.disablePID = 1;
	}	
//----------------------------------------------------------------------
	if(flagpid.disablePID==0)
	{
		
		PID_Reg.TIMER_PID.bit.T_Smp++;	
		if(PID_Reg.TIMER_PID.bit.T_Smp == TsCntr1)			
		{
			PID_Reg.TIMER_PID.bit.T_Smp = 0;
			ND2System_Reg.STATE_FLAG.bit.UpdateCmd = 1;//for updating FCommMngr
			SpCond();//Signal conditioning of Setpoint
			FbCond();//Signal conditioning of FeedBack
			if(ND2System_Reg.STATE_FLAG.bit.State == state_start ||
			   ND2System_Reg.STATE_FLAG.bit.State == state_run)
			{									
				CalPID();//calculation of PID controller D-on Er/D-on Fb				
			}else
			{
				PID_Reg.TIMER_PID.bit.T_Smp_AccDecTSP = TsCntr30;
				PID_Reg.TIMER_PID.bit.T_Smp_LimT      = TsCntr30;
			}				
		}
	}							
} 
////////////////////////////////////////////////////////////////// 
void CalPID(void)
{
_iq ts_ti=0,td_ts=0,td_ti=0;
_iq	buffcal = 0;
_iq	I_tempA = 0,I_temp = 0;//,I_tempSat = 0
_iq	D_temp = 0;

//the below are common variables D-on feedback/D-on error 
	if(PID_Reg.Ti==0)
	{
		ts_ti = 0; 
		td_ti = 0;
	}else
	{
		ts_ti = _IQ17div(TsPIDMain,PID_Reg.Ti);
		td_ti = _IQ17div(PID_Reg.Td,PID_Reg.Ti);
		PID_Reg.FLAG_PID.bit.Td_TiIQ15 = 0;
		if(td_ti >= _IQ17(16383.0))		//check over range of IQ17
		{
			td_ti = _IQ15div(PID_Reg.Td,PID_Reg.Ti);	//change to IQ15 for expanding range
			PID_Reg.FLAG_PID.bit.Td_TiIQ15 = 1;
		}
	}	
	td_ts = _IQ17div(PID_Reg.Td,TsPIDMain);
	PID_Reg.FLAG_PID.bit.Td_TsIQ15 = 0;
	if(td_ts >= _IQ17(16383.0))		//check over range of IQ17
	{
		td_ts = _IQ15div(PID_Reg.Td,TsPIDMain);	//change to IQ15 for expanding range
		PID_Reg.FLAG_PID.bit.Td_TsIQ15 = 1;
	}

	PID_Reg.PU_ErrorK = PID_Reg.PU_SetpointK - PID_Reg.PU_FeedbackK; 	//Er(k) = Sp(k)-Fb(k)
//-----------------------------------------------------------------
	//cal P. : P = Kp*Er(k)
	PID_Reg.P_TermK = _IQ17mpy(PID_Reg.Kp,PID_Reg.PU_ErrorK);					//P_temp = Kp*Er(k)	
//-----------------------------------------------------------------
	//cal I-saturate term = Kc*ErSat(k)
//	I_tempSat = _IQ17rsmpy(PID_Reg.Kc,PID_Reg.PU_SatErK);				//I_tempSat = Kc*SaturateEr(k)

	if(grp_num[7].value[2]==1)//check D-on error(1) or D-on feedback(0)	
	{
//-----------------------------------------------------------------
		//I term D-on error	
		if(PID_Reg.Ti == 0) 		
		{
			PID_Reg.I_TermK = 0;
		}else
		{
			//***cal I. : I = {Kp*[I(k-1) + Er(k)*Ts/Ti]} + Kc*ErSat(k)***//
			//I_temp = Er(k)*Ts/Ti	
			I_temp = IQmpyMin(PID_Reg.PU_ErrorK,ts_ti);			
			
			//I_temp = Kp*I_temp			
			I_temp = _IQ17rsmpy(PID_Reg.Kp,I_temp);

			//I_temp = I_temp+I(k-1)					
			I_temp = Add_IQ17(I_temp,PID_Reg.I_termK_1);																												
									
			//Check limit of output PID
			if(I_temp >= PID_Reg.PID_Dyna_Max)
			{
				PID_Reg.I_TermK = PID_Reg.PID_Dyna_Max;
			}else if(I_temp <= PID_Reg.PID_Dyna_Min)
			{
				PID_Reg.I_TermK = PID_Reg.PID_Dyna_Min;
			}else
			{
				PID_Reg.I_TermK = I_temp;
			}			
		}
//-----------------------------------------------------------------				
		//D term D-on error
		if(PID_Reg.Td == 0)
		{
			PID_Reg.D_TermK = 0;
		}else
		{
			//***cal D. : D = [Er(k)-Er(k-1)]*Kp*Td/Ts***//
			//D_temp = Er(k)-Er(k-1)
			D_temp = PID_Reg.PU_ErrorK-PID_Reg.PU_ErrorK_1;					
				
			//D_temp = D_temp*Kp
			D_temp = _IQ17mpy(D_temp,PID_Reg.Kp);							
			
			//D_temp = D_temp*(Td/Ts), max pu = 80 ans gain = 60000				
			if(PID_Reg.FLAG_PID.bit.Td_TsIQ15 == 1)
			{				

				PID_Reg.D_TermK = _IQ17(Mul_IQ17_IQ8(_IQ17toF(D_temp),_IQ15toF(td_ts)));//case of td/ts is iq15
			}else
			{
				PID_Reg.D_TermK = _IQ17rsmpy(D_temp,td_ts);
			}
		}	
//-----------------------------------------------------------------					
	}else//D-on feedback
	{
//-----------------------------------------------------------------
		//I term D-on feedback
		if(PID_Reg.Ti == 0) 		
		{
			PID_Reg.I_TermK = 0;
		}else
		{
			//***cal I. : I = Kp*{Er(k)*[Ts/Ti] - [(Fb(k) - Fb(k-1))*(Td/Ti)] + I(k-1)}+ Kc*ErSat(k)***//
			//I_temp = Er(k)*(Ts/Ti)
			I_temp = IQmpyMin(PID_Reg.PU_ErrorK,ts_ti);						
					
			//I_tempA = Fb(k) - Fb(k-1)		
			I_tempA = PID_Reg.PU_FeedbackK - PID_Reg.PU_FeedbackK_1;			

			//I_tempA = I_tempA*(Td/Ti)
			if(PID_Reg.FLAG_PID.bit.Td_TiIQ15 == 1)
			{
				I_tempA = _IQ17(Mul_IQ17_IQ8(_IQ17toF(I_tempA),_IQ15toF(td_ti)));																	
			}else
			{
				I_tempA = _IQ17rsmpy(I_tempA,td_ti);							
			}		
			I_temp = I_temp - I_tempA;										

			//I_temp = I_temp*Kp
			I_temp = _IQ17rsmpy(I_temp,PID_Reg.Kp);	
			
			//I_temp = I_temp+I(k-1)
			I_temp = Add_IQ17(I_temp,PID_Reg.I_termK_1);							
									
			//Check limit of output PID
			if(I_temp >= PID_Reg.PID_Dyna_Max)
			{
				PID_Reg.I_TermK = PID_Reg.PID_Dyna_Max;
			}else if(I_temp <= PID_Reg.PID_Dyna_Min)
			{
				PID_Reg.I_TermK = PID_Reg.PID_Dyna_Min;
			}else
			{
				PID_Reg.I_TermK = I_temp;
			}														
		}
//-----------------------------------------------------------------
		//D term D-on feedback
		if(PID_Reg.Td == 0)
		{
			PID_Reg.D_TermK = 0;
		}else
		{
			//***cal D. : D = Kp*{[Fb(k-1) - Fb(k)]*(Td/Ts)}***//
			//D_temp = Fb(k-1) - Fb(k)
			D_temp = PID_Reg.PU_FeedbackK_1-PID_Reg.PU_FeedbackK;			

			//D_temp = D_temp*(Td/Ts)
			if(PID_Reg.FLAG_PID.bit.Td_TsIQ15 == 1)
			{
				D_temp = _IQ17(Mul_IQ17_IQ8(_IQ17toF(D_temp),_IQ15toF(td_ts)));
			}else
			{
				D_temp = _IQ17rsmpy(D_temp,td_ts);							
			}
			
			//D_temp = D_temp*Kp
			PID_Reg.D_TermK = _IQ17rsmpy(D_temp,PID_Reg.Kp);							
		}		
	}
//-----------------------------------------------------------------
	//Total Output before saturate = P+I+D
	buffcal = Add_IQ17(PID_Reg.P_TermK,PID_Reg.I_TermK);						//P+I
	buffcal = Add_IQ17(buffcal,PID_Reg.D_TermK);	//P+I+D

	PID_Reg.PU_OutputPreBuff = _IQsat(buffcal, _IQ17(16380.0),_IQ17(-16380.0));		
//-----------------------------------------------------------------		
	//limit upper/lower PID & limit time
	PID_Reg.PU_OutputPreBuff = LimitPID();	//max<=PID_Reg.PU_OutputK<=min,and check trim mode	
	//Mode trim/setpoint
	if(grp_num[7].value[1]==1)				
	{
		buffcal = _IQ17(SelectorFSet(3,0,grp_num[3].value[8]));//select sp trim F308
		PID_Reg.PU_OutputPreBuff = PID_Reg.PU_OutputPreBuff+buffcal;//add o/p PID with sp system
	}														
	PID_Reg.PU_OutputK = PID_Reg.PU_OutputPreBuff;
			
	PID_Reg.I_termK_1      = PID_Reg.I_TermK;
	PID_Reg.PU_ErrorK_1    = PID_Reg.PU_ErrorK;
	PID_Reg.PU_FeedbackK_1 = PID_Reg.PU_FeedbackK;		
} 
////////////////////////////////////////////////////////////////// 
_iq LimitPID(void)
{
_iq ts_limT = _IQ17(0.03);	//T sampling of increasing dynamic range
_iq step_inc = 0;  	//buffer step increasing
_iq buffer=0;

	if(PID_Reg.PID_Lim_T != 0)
	{
		step_inc = _IQ17div(ts_limT,PID_Reg.PID_Lim_T);
	}	

	//increasing limit & check Tsampling		
	if(PID_Reg.TIMER_PID.bit.T_Smp_LimT >= TsCntr30)//tsampling = 30ms			
	{
		PID_Reg.TIMER_PID.bit.T_Smp_LimT = 0;
		//check upper
		PID_Reg.PID_Dyna_Max = _IQ17abs(PID_Reg.PID_Dyna_Max) + step_inc;
		if((_IQ17abs(PID_Reg.PID_Dyna_Max)>=_IQ17abs(PID_Reg.PID_max)) || (PID_Reg.PID_max == 0) || step_inc == 0)//check complete increasing
		{
			PID_Reg.PID_Dyna_Max = PID_Reg.PID_max;			//load dynamic range as user setting
		}else
		{			
			if(PID_Reg.PID_max <= 0)
			{
				PID_Reg.PID_Dyna_Max = _IQ17mpy(PID_Reg.PID_Dyna_Max,MinusIQ);
			}
		}
		//check lower
		PID_Reg.PID_Dyna_Min = _IQ17abs(PID_Reg.PID_Dyna_Min) + step_inc;
		if((_IQ17abs(PID_Reg.PID_Dyna_Min)>=_IQ17abs(PID_Reg.PID_min)) || (PID_Reg.PID_min == 0) || step_inc == 0)//check complete increasing
		{
			PID_Reg.PID_Dyna_Min = PID_Reg.PID_min;			//load dynamic range as user setting
		}else
		{			
			if(PID_Reg.PID_min <= 0)
			{
				PID_Reg.PID_Dyna_Min = _IQ17mpy(PID_Reg.PID_Dyna_Min,MinusIQ);
			}
		}		
	}else
	{
		PID_Reg.TIMER_PID.bit.T_Smp_LimT++;
	}	

	//Check limit of output PID
	if(PID_Reg.PU_OutputPreBuff >= PID_Reg.PID_Dyna_Max)
	{
		buffer = PID_Reg.PID_Dyna_Max;
	}else if(PID_Reg.PU_OutputPreBuff <= PID_Reg.PID_Dyna_Min)
	{
		buffer = PID_Reg.PID_Dyna_Min;
	}else
	{
		buffer = PID_Reg.PU_OutputPreBuff;
	}
	return buffer;			
} 
////////////////////////////////////////////////////////////////// 
void SpCond(void)
{
_iq	Sp_temp=0;		
	
//Select Source of Setpoint
	Sp_temp = _IQ17(SelectorFSet(7,grp_num[7].value[3],0));//load PID parameter

//multiply by Sp gain 
	Sp_temp = _IQ17mpy(Sp_temp,PID_Reg.Sp_gain);

//check limit of Sp
	Sp_temp = ChkRngF(Sp_temp,MainIQ_Variable.PU_FMax,MainIQ_Variable.PU_FMin);//check range of sp						

	if(DgtlPrt_Reg.DataLDIN.bit.Rev_Com!=DgtlPrt_Reg.DataLDIN.bit.Rev_Fwd)
	{
		Sp_temp = _IQ17mpy(Sp_temp,MinusIQ); 
	}
	PID_Reg.PID_SpTemp = Sp_temp;//immediate SP from Signal SP
		
	if(PID_Reg.TIMER_PID.bit.T_Smp_AccDecTSP >= TsCntr30)//tsampling = 30ms			
	{
		PID_Reg.TIMER_PID.bit.T_Smp_AccDecTSP = 0;
		
		if(ND2System_Reg.STATE_FLAG.bit.State == state_run ||
		   ND2System_Reg.STATE_FLAG.bit.State == state_start)	
		{						
			PID_Reg.PU_SetpointK = _IQ17(Accel_Decel(_IQ17toF(PID_Reg.PU_SetpointK),_IQ17toF(PID_Reg.PID_SpTemp),_IQ17toF(PID_Reg.PID_TDecelSp),_IQ17toF(PID_Reg.PID_TAccelSp)));//accel/decel without SCurve			
		}else if(PID_Reg.PU_SetpointK != 0)
		{
			PID_Reg.PU_SetpointK = _IQ17(Accel_Decel(_IQ17toF(PID_Reg.PU_SetpointK),0,_IQ17toF(PID_Reg.PID_TDecelSp),_IQ17toF(PID_Reg.PID_TAccelSp)));//accel/decel without SCurve			
		}
	}else
	{
		PID_Reg.TIMER_PID.bit.T_Smp_AccDecTSP++;
	}
}
//////////////////////////////////////////////////////////////////  
float32 SelectorFSet(Uint16 Grp,Uint16 Selector,Uint16 InvDir)
{
Uint16 src_mulF=0,src_mulFdata=0;//ram for multifreq. source selector (trml bit0-3)
_iq MultF=0;//final freq. from selecting digital input
_iq bufferF=0,bufferFkey=0;
_iq sourceA2=0;	
struct {
	Uint16 analogsource:1;	
	Uint16 MultFactive:1;
}flagfset={0};
		
	//---Check Multi F.	
	src_mulF = 0;//clear source of multiF selector
	src_mulF = DgtlPrt_Reg.DataLDIN.bit.Mul_FrqB0*1;
	src_mulF = src_mulF+(DgtlPrt_Reg.DataLDIN.bit.Mul_FrqB1*2);
	src_mulF = src_mulF+(DgtlPrt_Reg.DataLDIN.bit.Mul_FrqB2*4);
	src_mulF = src_mulF+(DgtlPrt_Reg.DataLDIN.bit.Mul_FrqB3*8);			
	
	if(Grp==7)//PID
	{
		if(KeyPad_reg.MODE_FLAG.bit.norm_state==1)
		{
			bufferFkey = _IQ17div(KeyPad_reg.Sp_Norm,IQ17_100); 	
		}else
		{
			bufferFkey = PID_Reg.PID_Freq;	//Loaded data in converse all IQ
		}		
		if(src_mulF!=0)
		{
			src_mulFdata = src_mulF-1;
			MultF = arrPID_MulF[src_mulFdata];	
		}			
	}else
	{
		if(KeyPad_reg.MODE_FLAG.bit.norm_state==1)
		{
			bufferFkey = KeyPad_reg.Sp_Norm;
		    //-------------------Gus-------------------------
			/*if (grp_num[3].value[0]==1)
			{
			    bufferFkey = Freq_Reg.F_Key;
			}
			else
			{
			    bufferFkey = KeyPad_reg.Sp_Norm;
			}*/
			//-------------------Gus-------------------------
		}else
		{
			bufferFkey = Freq_Reg.F_Key;	//Loaded data in converse all IQ
		}

		bufferFkey = _IQ17div(bufferFkey,MainIQ_Variable.FRef);
		
		if(src_mulF!=0)
		{
			src_mulFdata = src_mulF-1;
			MultF = arrFreq_MulF[src_mulFdata];
			MultF = _IQ17div(MultF,MainIQ_Variable.FRef);				
		}			
	}
	//---Check Model
	//Fset from A2 or VRKeypad
	sourceA2 = (grp_num[0].value[14]==1)?Analog_reg.PU_B1:Analog_reg.VRKeypad;
	//---Check option of source
	if(DgtlPrt_Reg.DataHDIN.bit.Curr_IP != 0)//check DIN Current Input
	{
		Selector = 3;
	}else if(src_mulF!=0)//MultiF active
	{
		if(Selector!=8&&Selector!=11&&Selector!=13&&Selector!=14)
		{
			Selector=4;
		}
	}
						
	switch(Selector)//Select Source of Setpoint
	{
		case 0://Sp from keypad					
			bufferF = bufferFkey;//freq. from keypad (freq. function/PID)							
			break;
		case 1://Sp from A12
			bufferF = Analog_reg.PU_B0;
			flagfset.analogsource = 1;
			break;
		case 2://Sp from A2/VRKeypad
			bufferF = sourceA2;
			flagfset.analogsource = 1;
			break;
		case 3://Sp from A3
			bufferF = Analog_reg.PU_B2;
			flagfset.analogsource = 1;
			break;
		case 4://Sp multiF
			bufferF = MultF;
			break;			
		case 5://Sp from keypad+A12		
			bufferF = bufferFkey+Analog_reg.PU_B0;
			flagfset.analogsource = 1;
			break;
		case 6://Sp from keypad+A2
			bufferF = bufferFkey+sourceA2;
			flagfset.analogsource = 1;				 
			break;
		case 7://Sp from keypad+A3
			bufferF = bufferFkey+Analog_reg.PU_B2;	
			flagfset.analogsource = 1;		
			break;
		case 8://Sp from keypad+MultF
			bufferF = bufferFkey+MultF;			
			break;		
		case 9://Sp from A12+A2
			bufferF = Analog_reg.PU_B0+sourceA2;
			flagfset.analogsource = 1;
			break;
		case 10://Sp from A12+A3
			bufferF = Analog_reg.PU_B0+Analog_reg.PU_B2;	
			flagfset.analogsource = 1;		
			break;		
		case 11://Sp from A12+MultF
			bufferF = Analog_reg.PU_B0+MultF;	
			flagfset.analogsource = 1;		
			break;	
		case 12://Sp from A2+A3
			bufferF = sourceA2+Analog_reg.PU_B2;
			flagfset.analogsource = 1;			
			break;
		case 13://Sp from A2+MultF
			bufferF = sourceA2+MultF;	
			flagfset.analogsource = 1;		
			break;	
		case 14://Sp from A3+MultF
			bufferF = Analog_reg.PU_B2+MultF;	
			flagfset.analogsource = 1;		
			break;
		case 15:
		    bufferF = Analog_reg.VRKeypad;
		    flagfset.analogsource = 1;
		    break;
		default:
			bufferF = _IQ17(1.0);	
	}
	
	bufferF = (InvDir==1)?_IQ17mpy(bufferF,MinusIQ):bufferF;//check keypad freq.
	KeyPad_reg.GENERAL_FLAG.bit.Analog_Src =  flagfset.analogsource;
	return _IQ17toF(bufferF);
}
////////////////////////////////////////////////////////////////// 
float32 Accel_Decel(float32 InputAccDec,float32 MainTarget,float32 Tdecel,float32 Taccel)//routine 
{
/*
	Freq./step = (0.03*1)/Tacc(Tdec)

	note : meanining of PID acc-> 0 to 100% of freq. use time = Tacc
		 : meanining of PID dec-> from 100% to 0 use time = Tdec
		 : 0.03 is 30ms time of sampling acc/dec		  
*/
_iq MainTarget1 = _IQ17(MainTarget);
_iq InputAccDec1 = _IQ17(InputAccDec);
float32 OutputAccDec=0;
	
	//Accel/Decel normal type
	if((MainTarget1 & 0x80000000)!=(InputAccDec1 & 0x80000000))//check sign
	{//different sign
		if(InputAccDec1==0)	
		{
			OutputAccDec = Accel(InputAccDec,MainTarget,Taccel,1,0.03);
		}else
		{
			OutputAccDec = Decel(InputAccDec,0,Tdecel,1,0.03);//decel to 0Hz first
		}
	}else
	{
		if(_IQ17abs(MainTarget1)<_IQ17abs(InputAccDec1))
		{
			OutputAccDec = Decel(InputAccDec,MainTarget,Tdecel,1,0.03);
		}else if(_IQ17abs(MainTarget1)>_IQ17abs(InputAccDec1))
		{
			OutputAccDec = Accel(InputAccDec,MainTarget,Taccel,1,0.03);
		}else
		{
			OutputAccDec = MainTarget;
		}
	}
		
	return OutputAccDec;
}  
////////////////////////////////////////////////////////////////// 
float32 Accel(float32 CurrentF,float32 Target,float32 Tacc,float32 Reff,float32 Tsmp)//routine accel
{
_iq	Target1 = _IQ17(Target);		
_iq	CurrentF1 = _IQ17(CurrentF);
_iq Tacc1 = _IQ17(Tacc);
_iq step_accdec = 0,step_accdec1=0;

	if(Tacc1 != 0)
	{
		step_accdec = _IQ17div(_IQ17(Tsmp),Tacc1);//cal. step accel with Fpu 
		step_accdec1 = _IQ17mpy(step_accdec,_IQ17(Reff));
		
		if(step_accdec1==0)
		{
			if(step_accdec!=0 && _IQ17(Reff)!=0)
			{
				step_accdec1 = MinResIQ17;
			}
		}
		CurrentF1 = _IQ17abs(CurrentF1) + step_accdec1;

		if(CurrentF1 >= _IQ17abs(Target1))//check over range
		{	
			goto load_targetAcc;
		}else
		{			
			CurrentF1 = (Target1 < 0)?_IQ17mpy(CurrentF1,MinusIQ):CurrentF1;						
		}				
	}else
	{
load_targetAcc:		CurrentF1 = Target1;//if accel time Sp PID = 0, load Sp_temp immediately
	}

	return _IQ17toF(CurrentF1);
} 
//////////////////////////////////////////////////////////////////  
float32 Decel(float32 CurrentF,float32 Target,float32 Tdec,float32 Reff,float32 Tsmp)//routine decel
{
_iq	Target1 = _IQ17(Target);		
_iq	CurrentF1 = _IQ17(CurrentF);
_iq Tdec1 = _IQ17(Tdec);
_iq step_accdec = 0,step_accdec1=0;

	if(Tdec1 != 0)
	{
		step_accdec = _IQ17div(_IQ17(Tsmp),Tdec1);//cal. step accel
		step_accdec1 = _IQ17mpy(step_accdec,_IQ17(Reff));
		if(step_accdec1==0)
		{
			if(step_accdec!=0 && _IQ17(Reff)!=0)
			{
				step_accdec1 = MinResIQ17;
			}
		}

		CurrentF1 = _IQ17abs(CurrentF1) - step_accdec1;

		if(CurrentF1 <= _IQ17abs(Target1))
		{			
			goto load_targetDec;					
		}else
		{			
			CurrentF1 = (_IQ17(CurrentF) < 0)?_IQ17mpy(CurrentF1,MinusIQ):CurrentF1;//change to same with target														
		}										
	}else
	{
load_targetDec:		CurrentF1 = Target1;//if accel time Sp PID = 0, load Sp_temp immediately					
	}

	return _IQ17toF(CurrentF1);
}  
//////////////////////////////////////////////////////////////////
void FbCond(void)
{
_iq	Fb_temp=0;

//Select Source of Fb
	if(grp_num[7].value[7]==0)
	{
		Fb_temp = Analog_reg.PU_B0;//Fb as A12
	}else
	{
		Fb_temp = Analog_reg.PU_B2;//Fb as A3
	}

//check limit of Fb
	if(Fb_temp>=PID_Reg.Fb_max)
	{
		Fb_temp=PID_Reg.Fb_max;

	}else if(Fb_temp<=PID_Reg.Fb_min)
	{
		Fb_temp=PID_Reg.Fb_min;
	}

//multiply by Fb gain 
	Fb_temp = _IQ17mpy(Fb_temp,PID_Reg.Fb_gain);

//check type of transducer
	if(grp_num[7].value[11]==1)
	{
		Fb_temp = _IQ17mpy(Fb_temp,MinusIQ);
	}
	PID_Reg.PU_FeedbackK = Fb_temp;//last variable of feedback for calculation
}
////////////////////////////////////////////////////////////////// 
float32 Mul_IQ17_IQ8(float32 A17, float32 B15)
{
_iq8 IQ8_temp = _IQ8(0);
_iq buff_result = 0;
float32 C = 0.0;

	IQ8_temp = _IQ8mpyIQX(_IQ17(A17),17,_IQ15(B15),15);					//test A*B overflow or not?
	if(_IQ8abs(IQ8_temp)>=_IQ8(16383.0))
	{
		IQ8_temp = _IQsat(IQ8_temp, _IQ8(16383.0),_IQ8(-16383.0));		
		buff_result = _IQ8toIQ(IQ8_temp); 
	}else
	{
		buff_result = _IQ17mpyIQX(_IQ17(A17),17,_IQ15(B15),15);
	}	
	
	C = _IQ17toF(buff_result);

	return C;			
}				
////////////////////////////////////////////////////////////////// 				 
_iq Add_IQ17(_iq A, _iq B)
{
float32 C = 0;
_iq	output = 0;
	
	C = _IQ17toF(A)+_IQ17toF(B);
	if(C>16383)
	{
		C = 16000;
	}else if(C< -16383)
	{
		C = -16000;
	} 
	output = _IQ17(C);
	return output;
}
//////////////////////////////////////////////////////////////////  




