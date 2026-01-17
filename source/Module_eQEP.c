//2013-2-19 : move GPIO QEP into GPIO manager
//2013-2-20 : got read speed by using interrupt UTO
//2013-2-20 : clear cntr check direction when RPM=0
//2013-2-20 : revise solve problem for mis-cal
//2013-2-20 : revise minus RPM
//2013-2-21 : got time latch capture
//2013-2-21 : got time/rev
//2013-2-21 : no clear QPOS when change direction
//2013-2-22 : revise algorithm after change direction
//2013-2-23 : got RPM and no overshoot
//2013-2-23 : Revise Check toggle direction with my algorithm
//2013-2-23 : include read speed
//2013-2-23 : revise clear CAPLAT and add LPF
//2013-2-26 : revise QPOSMAX by user
//2013-2-26 : complete read speed by user key(F710)
//2013-2-26 : revise QCLM=0
//2013-2-27 : Completed All
//2013-3-2 : revise QCLM=1,no clear cntr direction when stop
//2013-3-11 : revise after change direction and clearing capture
//2013-3-11 : change time scale for POSCNT 500us and time count check direction=200
//2013-3-13 : revise:if direction not stable the system will hold last RPM
//2013-3-13 : revise algorithm by using interrupt change direction
//2013-3-19 : move check direction to loop 5k
//2013-3-22 : revise UTO Period to 2ms and LPF in loop 1ms
//2013-4-23 : revise no LPF and algorithm
//2013-5-2 : use LPF for reading speed 
//2013-5-2 : check speed must less than F104
//2013-5-6 : clear QEP_Reg.ABSRPM_UTO when stop
//2013-6-7 : revise with current motor->pulse encoder
//2013-8-7 : debug remove limit RPM
//2013-10-18 : change UTO to 2ms and limit capture<=1
//2013-10-18 : change algorithm read speed with switching UTO/CAP
//2013-10-19 : UTO = 1ms

#include "F28x_Project.h"
#include "ND2.h"
#include "f28004x_sw_prioritized_isr_levels.h"

struct QEP_REG QEP_Reg = {0,0,0,0,0,
						  0,0,0,0,0};
struct data_grp{
	Uint16 value[100];
	Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber];  

Uint32 RamTestRPMOS = 0;

//void Read_Speed(void);
//void  eQEP_Init(void);

extern int direction	;
extern int theta_new	;
extern int theta_raw	;
extern int theta_mech	;
extern int theta_elec	;
extern int shaft_angle	;
extern int speed_skp_cnt;
extern int s_angle_old	;
extern int s_angle_cur	;
extern int speed_frq 	; 


void  eQEP_Init(void)
{
EALLOW;
    //Interrupt_register(INT_EQEP1, &EQEP1_ISR);
	//-----Initial Setting Register
	EQep1Regs.QUPRD                = 100000;//time out period(s) = 10ns*QUPRD
	EQep1Regs.QDECCTL.bit.QSRC     = 0;		// QEP quadrature count mode
	EQep1Regs.QEPCTL.bit.FREE_SOFT = 0;
	EQep1Regs.QEPCTL.bit.PCRM      = 2;		// PCRM=00 mode - QPOSCNT reset on index event
	EQep1Regs.QEPCTL.bit.UTE       = 1; 		// Unit Timeout Enable
	EQep1Regs.QEPCTL.bit.QCLM      = 1; 		// 0:Latch on CPU reading,1:Latch on UTO
	EQep1Regs.QEINT.bit.UTO        = 1;
	EQep1Regs.QEINT.bit.QDC        = 1;
	EQep1Regs.QEPCTL.bit.QPEN      = 1; 		// QEP enable

	EQep1Regs.QCAPCTL.bit.UPPS     = 0;		//1event/qclk
	EQep1Regs.QCAPCTL.bit.CCPS     = 7;		//qclk=sysclk/128=1.28us
	EQep1Regs.QCAPCTL.bit.CEN      = 1;		//enable qcap
EDIS;
}
/////////////////////////////////////////////////////////////////////////////
void ReadSpeed(void)
{
_iq buff=0;	
float32 buff2=0,buffcap=0;	
Uint32 buff1=0;

	if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)//check finish of setup keypad   
	{
		//-------------------------------------------
		//Initial parameter
		buff1 = (_IQ17int(CurrentMotor->PulseEncoder)*4)-1;
		if(EQep1Regs.QPOSMAX!=buff1)
		{
			EQep1Regs.QPOSMAX=buff1;//4095;
			//Unit timeout			
			buff = _IQ17div(_IQ17(1000),_IQ17(EQep1Regs.QPOSMAX));//_IQ17(500)=>ตัวตั้ง=1/(QUPRD*10ns))
			QEP_Reg.TimeScaleUTO = _IQ17mpy(buff,_IQ17(60));	//Factor for Unit timeout calculation
			//Capture			
			buff2 = 1/((float32)EQep1Regs.QPOSMAX*0.00000128);//buff2 = 1/((float32)EQep1Regs.QPOSMAX*0.00000128);
			QEP_Reg.TimeScaleCap = buff2*60;	//Factor for Capture calculation
		}	
		if(QEP_Reg.GENERAL_QEPFLAG.bit.FrstChkDirt==0)
		{			
			ChkDir();
			QEP_Reg.GENERAL_QEPFLAG.bit.FrstChkDirt = 1;
		}else
		{
			QEP_Reg.New_Cnt = EQep1Regs.QPOSLAT;
			//---UTO Section------------------------------------------------		
			if(QEP_Reg.New_Cnt>=QEP_Reg.Old_Cnt)
			{
				QEP_Reg.DeltaCnt = QEP_Reg.New_Cnt-QEP_Reg.Old_Cnt;
			}else
			{
				QEP_Reg.DeltaCnt = EQep1Regs.QPOSMAX-QEP_Reg.Old_Cnt;
				QEP_Reg.DeltaCnt = QEP_Reg.DeltaCnt+QEP_Reg.New_Cnt;
			}									
			
			if((QEP_Reg.GENERAL_QEPFLAG.bit.data_qdf==0)&&(QEP_Reg.DeltaCnt!=0))//case reverse
			{
				QEP_Reg.DeltaCnt = EQep1Regs.QPOSMAX-QEP_Reg.DeltaCnt;
			} 		
			QEP_Reg.Old_Cnt = QEP_Reg.New_Cnt;	
			QEP_Reg.RPM_UTO = _IQ17mpy(_IQ17(QEP_Reg.DeltaCnt),QEP_Reg.TimeScaleUTO);
			//---CAP Section------------------------------------------------
			if(EQep1Regs.QEPSTS.bit.UPEVNT==1)
			{
				if(EQep1Regs.QEPSTS.bit.COEF==0)
				{						
					buffcap = (float32)EQep1Regs.QCPRDLAT;//EQep1Regs.QCPRD;
					
					if(buffcap<=1)//2ms/1.28us
					{
						buffcap = QEP_Reg.ABSRPM;
						ClrCptr();							
					}else
					{
						buffcap = 1/buffcap;	//1/(time*4*1024*clkperiod)->got rev/s
						buffcap = buffcap*QEP_Reg.TimeScaleCap;	
						buffcap = _IQ17(buffcap);	
					}						
				}else
				{
					buffcap = 0;
					ClrCptr();
					EQep1Regs.QEPSTS.bit.COEF = 1;
				}
				EQep1Regs.QEPSTS.bit.UPEVNT = 1;					
			}else
			{//No event occur
				if(EQep1Regs.QEPSTS.bit.COEF==1)
				{
					buffcap = 0;
					QEP_Reg.ABSRPM = 0;
					ClrCptr();
					EQep1Regs.QEPSTS.bit.COEF = 1;					
				}else
				{
					buffcap = QEP_Reg.ABSRPM;
				}				
			}
			QEP_Reg.RPM_CAP = buffcap;			
			//---Switch UTO/CAP Section------------------------------------------------
			if(QEP_Reg.DeltaCnt>=5)
			{
				if(QEP_Reg.GENERAL_QEPFLAG.bit.UTOActive==1)
				{
					LdUTOSpeed();
				}else
				{
					if(_IQ17abs(QEP_Reg.RPM_UTO-QEP_Reg.RPM_CAP)<=GapRPM)
					{
						LdUTOSpeed();
					}else
					{
						LdCAPSpeed();
					}
				}
			}else if(QEP_Reg.DeltaCnt>=8)
			{
				LdUTOSpeed();
			}else
			{
				if(QEP_Reg.GENERAL_QEPFLAG.bit.UTOActive==0)
				{
					LdCAPSpeed();
				}else
				{
					if(_IQ17abs(QEP_Reg.RPM_UTO-QEP_Reg.RPM_CAP)<=GapRPM)
					{
						LdCAPSpeed();
					}else
					{
						LdUTOSpeed();
					}
				}
			}

			buff = _IQ17div(QEP_Reg.ABSRPM,CurrentMotor->Speed_Motor);
			QEP_Reg.PU_RPM = (QEP_Reg.GENERAL_QEPFLAG.bit.data_qdf==0)?_IQ17mpy(buff,MinusIQ):buff;				
		}
	}
	EQep1Regs.QCLR.bit.UTO=1;
} 
//////////////////////////////////////////////////////////////////////
void LdCAPSpeed(void)
{
	QEP_Reg.ABSRPM = QEP_Reg.RPM_CAP;
	QEP_Reg.GENERAL_QEPFLAG.bit.UTOActive = 0;
} 
//////////////////////////////////////////////////////////////////////
void LdUTOSpeed(void)
{
	QEP_Reg.ABSRPM = QEP_Reg.RPM_UTO;
	QEP_Reg.GENERAL_QEPFLAG.bit.UTOActive = 1;
}
//////////////////////////////////////////////////////////////////////
void ClrCntr(void)
{
	 QEP_Reg.New_Cnt = EQep1Regs.QPOSLAT;
	 QEP_Reg.Old_Cnt = EQep1Regs.QPOSLAT;
}
//////////////////////////////////////////////////////////////////////
void ClrCptr(void)
{
	EQep1Regs.QCPRD     = 0;
	EQep1Regs.QCPRDLAT  = 0;
	EQep1Regs.QCTMR     = 0;
	EQep1Regs.QCTMRLAT  = 0;
}
//////////////////////////////////////////////////////////////////////
void ChkDir(void)
{	
  	ClrCntr();
  	ClrCptr();			
  	QEP_Reg.GENERAL_QEPFLAG.bit.data_qdf = EQep1Regs.QEPSTS.bit.QDF;
	EQep1Regs.QCLR.bit.QDC = 1;
}
//////////////////////////////////////////////////////////////////////
