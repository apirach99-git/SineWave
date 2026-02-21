//2012-6-20 : Tsampling 1ms
//2012-6-20 : added initial variable
//2012-6-20 : completed Jog function
//2012-6-21 : completed JOG/regular source F353=0
//2012-6-21 : completed arrange data of acc stage 1-7
//2012-6-21 : completed arrange data of acc/dec stage 1-7
//2012-6-21 : completed pattern mode
//2012-6-21 : add check range
//2012-6-21 : completed regular source F(grp3)
//2012-7-2 : moved F308 into SelectorFSet
//2012-7-7 : completed added function DIN rev and fwd_rev
//2012-7-9 : added function terminal FWD/REV
//2012-7-9 : revised bit check update for grp3 before load sp of ACDC
//2012-9-11 : revised pattern mode no converse IQ increase time pattern
//2012-11-08 : debugged if F201=0 DIN no effect of sign
//2013-6-13 : debugged use decel of joging if stopping of jog
//2013-6-24 : debugged REV and 3wire stop
//2013-7-11 : use bit auto stop of monocycle
//2013-9-2 : revise disable freq module
//2013-9-4 : use bit "CmdType" check mode
//2013-11-12 : add check bit flying start will not operate

#include "F28x_Project.h"
#include "include\ND2.h"
#include "f28004x_sw_prioritized_isr_levels.h"

struct Freq_REG Freq_Reg={0,0,0,0,0,
						  0,0,0,0,0,
						  0,0,0,0,0,
						  0,0,0,0,0,
						  0,0,0,0,0,
						  0,0,0,0,0}; 

_iq arrFreq_MulF[]={0,0,0,0,0,
				    0,0,0,0,0,
				    0,0,0,0,0};//MultiF of freq. function

_iq arr_AccStg[]={0,0,0,0,0,
				  0,0};//array accel stage

_iq arr_DecStg[]={0,0,0,0,0,
				  0,0};//array decel stage

//---External variable import--------------------------------------
struct data_grp{
	Uint16 value[100];
	Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber]; 
/////////////////////////////////////////////////////////////////////////////////
void Freq_function(void)						
{
Uint16 buff_data=0;
_iq buff=0;
struct  {
	Uint16 disablefreq:1;
}flagfreq={0};

	if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0||
	   grp_num[7].value[0]==1||
	   ND2System_Reg.MAIN_FLAG2.bit.CmdType==1||
	   FlyStrt_Reg.Flag_FlyStrt.bit.FlyActive == 1)
	{
		flagfreq.disablefreq = 1;
	}
//-----------------------------------------------------------------------
	if(flagfreq.disablefreq==0)		
	{
		Freq_Reg.TIMER_FREQ.bit.T_Smp++;	
		if(Freq_Reg.TIMER_FREQ.bit.T_Smp >= TsCntr1)			
		{
			Freq_Reg.TIMER_FREQ.bit.T_Smp = 0;

			buff_data = DgtlPrt_Reg.DataLDIN.all & 0x0030;//check jog R/L
			if(buff_data == 0x0030 || buff_data == 0x0000)
			{//---No jogging/normal--------------
				if(ND2System_Reg.STATE_FLAG.bit.BckUpSTT ==1||
					Freq_Reg.FLAG_FREQ.bit.Jogging == 0)
				{
					Freq_Reg.FLAG_FREQ.bit.Jogging = 0;
					if(grp_num[3].value[52]==0)//check pattern mode
					{
						//this case for pattern mode disable
						//---check FWD/REV by DIN						
						if(grp_num[2].value[0]==0)
						{
							buff = _IQ17(SelectorFSet(3,grp_num[3].value[0],grp_num[3].value[8]));//load PID parameter
						}else
						{
							buff = _IQ17(SelectorFSet(3,grp_num[3].value[0],0));
							if(DgtlPrt_Reg.DataLDIN.bit.Rev_Com!=DgtlPrt_Reg.DataLDIN.bit.Rev_Fwd)
							{
								buff = _IQ17mpy(buff,MinusIQ); 
							}else if(DgtlPrt_Reg.DataHDIN.bit.Wire_stp==Active)
							{
								if(MainIQ_Variable.FCommandA<0)
								{
									buff = _IQ17mpy(buff,MinusIQ); 
								}
							}
						}						
						Freq_Reg.MainAcc = Freq_Reg.AccT1;
						Freq_Reg.MainDec = Freq_Reg.DecT1;
					}else
					{
						//---Pattern-----------------
						//this case for pattern mode enable
						buff = FPattern();//function pattern
					}
				}else
				{
					Freq_Reg.FLAG_FREQ.bit.Jogging = 1;
				}																		
			}else
			{
		//---jogging-----------------
				Freq_Reg.FLAG_FREQ.bit.Jogging = 2;
				buff = _IQ17(FJogging());//output function : FSET,ACC,DEC
			}														
			Freq_Reg.Freq_SpPU = buff;
			ND2System_Reg.STATE_FLAG.bit.UpdateCmd = 1;
		}
	}									
} 
/////////////////////////////////////////////////////////////////////////////////
_iq FPattern(void)
{
_iq buff=0;
_iq *pntracc = &Freq_Reg.AccT1;
_iq *pntrdec = &Freq_Reg.DecT1;
_iq *pntrtstg = &Freq_Reg.StageT1;
Uint16 i=0,y=0;		

	buff = arrFreq_MulF[Freq_Reg.FLAG_FREQ.bit.CtrlPttnStg];
	buff = _IQ17div(buff,MainIQ_Variable.FRef);
	if(ND2System_Reg.STATE_FLAG.bit.State == state_start ||
	   ND2System_Reg.STATE_FLAG.bit.State == state_run)
	{
		Freq_Reg.TIMER_FREQ.bit.T_Smpstg++;			
		for(i=0;i<7;i++)
		{
			y = (grp_num[3].value[i+61]-1)*2;
			arr_AccStg[i] = *(pntracc+y);
			arr_DecStg[i] = *(pntrdec+y);
		}
		Freq_Reg.MainAcc = arr_AccStg[Freq_Reg.FLAG_FREQ.bit.CtrlPttnStg];
		Freq_Reg.MainDec = arr_DecStg[Freq_Reg.FLAG_FREQ.bit.CtrlPttnStg];

		if(Freq_Reg.TIMER_FREQ.bit.T_Smpstg>=100)	
		{
			Freq_Reg.TIMER_FREQ.bit.T_Smpstg = 0;
			if(_IQ17(Freq_Reg.TCtrlPttn) >= *(pntrtstg+Freq_Reg.FLAG_FREQ.bit.CtrlPttnStg))
			{
				Freq_Reg.TCtrlPttn = 0;
				if(Freq_Reg.FLAG_FREQ.bit.CtrlPttnStg>=6)
				{
					if(grp_num[3].value[52]==3)
					{
						Freq_Reg.FLAG_FREQ.bit.FinishMonoCyc = 1;
					}else
					{
						Freq_Reg.FLAG_FREQ.bit.CtrlPttnStg = 0;
						if(grp_num[3].value[52]==1)
						{
							Freq_Reg.FLAG_FREQ.bit.AutoStopPttn = 1;//Stop mono cycle
						}
					}										
				}else
				{
					Freq_Reg.FLAG_FREQ.bit.CtrlPttnStg++;
				}
			}else
			{
				Freq_Reg.TCtrlPttn+=0.1;//up 0.1s a step
			}
		}
	}
	return buff;
}
/////////////////////////////////////////////////////////////////////////////////
float32 FJogging(void)						
{
_iq buff=0;

	if(DgtlPrt_Reg.DataLDIN.bit.Jog_Rev==Active)		
	{
		buff = _IQ17mpy(Freq_Reg.FJogRev,MinusIQ);
	}else if(DgtlPrt_Reg.DataLDIN.bit.Jog_Fwd==Active)
	{
		buff = Freq_Reg.FJogFwd;
	}

	Freq_Reg.MainAcc = Freq_Reg.AccTJog;
	Freq_Reg.MainDec = Freq_Reg.DecTJog;
	buff = _IQ17div(buff,MainIQ_Variable.FRef);

	return _IQ17toF(buff);
} 
///////////////////////////////////////////////////////////////////////////////// 










