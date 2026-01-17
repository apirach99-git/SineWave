#include "F28x_Project.h"
#include "ND2.h"

//2011-09-01 DEC DCbrake and Terminal DCbrake

void DCBreak(void);

long TimeDCBreak    = 0;
long VDCbreak       = 0;
long FDCbreak       = 0;
long Time_DCBreak   = 0;
long Freq_DCbreak   = 0;

extern int F_Command,Vsq_ref;
extern _iq17 Vrate,Frate;
extern int Theta_Q15,omega; 

extern long Volt_Vq;

struct data_grp{
   Uint16 value[100];
   Uint16 factor[100];
};
extern struct data_grp grp_num[8] ;   


int Flag_DCbreak    = 0;
int Flag_Run        = 0;
int Count_1s        = 0;
int Count_200us     = 0;

void DCBreak(void)
{
	VDCbreak = _IQ17mpy(Vrate,ND2System_Reg.DCBreak_Level);//DC brake level

	if(grp_num[5].value[27] == 0)//normal Decel stopping
	{
		if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)
		{
			FDCbreak = _IQ17div(ND2System_Reg.DCBreak_Start_Frq,MainIQ_Variable.FRef);//DC brake start freq.
			Flag_Run = 0;
		}  
		if( ND2System_Reg.DCBreak_Time != 0 )//DC Break Time
		{
			if(ND2System_Reg.STATE_FLAG.bit.State == state_start)
			{
				Count_1s        = 0;
				Count_200us     = 0;
				Flag_DCbreak    = 0;
				Flag_Run        = 0;
			}

			Freq_DCbreak = MainIQ_Variable.FCommand;//((long)F_Command<<5);//Q17
			if(Freq_DCbreak<0)
				Freq_DCbreak = - Freq_DCbreak;


			if(ND2System_Reg.STATE_FLAG.bit.State == state_stopping && Flag_Run == 0)
			{ 
				if( FDCbreak >= Freq_DCbreak )
				{
					Flag_DCbreak    = 1;
					Flag_Run        = 1;
				}
			}

			if(Flag_DCbreak == 1)
			{
				Volt_Vq = VDCbreak;//Q17
				omega   = 0;
				if(Count_200us>=5000)
				{
					Count_200us = 0;
					Count_1s++;

					Time_DCBreak = _IQ17(Count_1s);

					if(ND2System_Reg.DCBreak_Time <= Time_DCBreak)
					{
						Flag_DCbreak = 0;
					}
				}
				Count_200us++;
			}
			else
			{
				Count_1s    = 0;
				Count_200us = 0;
			}
		}
	}

//=================DCbreak_with_Terminal=============================
	if(DgtlPrt_Reg.DataLDIN.bit.DC_Brk == 1)
	{
		Volt_Vq = VDCbreak;//Q17
		omega   = 0;
		Flag_DCbreak = 1;
	} 
	else
	{
		if(Flag_Run == 0)
			Flag_DCbreak = 0;
	}	
} 






