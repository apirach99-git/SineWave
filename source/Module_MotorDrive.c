//2013-3-19 : move check direction into this function
//2013-4-1 : move R charge C to loop 1ms
//2013-4-23 : remove function check direction speed to qQEP.c
//2013-6-18 : replace Scalling_Parameter() by ReadIV()	
//2013-8-7 : Add bit check "FLAG_SYST_CTRL[Syst_ready]"

#include "F28x_Project.h"
#include "ND2.h"
#include "f28004x_sw_prioritized_isr_levels.h"

extern int Vsd_ref;
extern int Vsq_ref;
extern int Theta_Q15;
extern int Vs_alpha;
extern int Vs_beta;

extern int omega;
extern int va;
extern int vb;
extern int vc;

struct  DAC_SPI{
   long Data_high;
   long Data_low;
   long DAC_a;
   long DAC_b;
   long DAC_c;
   long DAC_d; 
   long Indep_long1;
   int  C_DAC;

}; 
extern struct  DAC_SPI DAC;  

struct  PWM_COMMAN{
   long va_com;
   long vb_com;
   long vc_com;
}; 
extern struct  PWM_COMMAN PWM_COM;   


struct data_grp{
   Uint16 value[100];
   Uint16 factor[100];
};
extern struct data_grp grp_num[8] ;  


extern int Vbase;

extern int F_Command;

//======Vector=======
extern int Speed_Feedback,speed_frq;
extern int Theta_o ; 
extern int Speed_vector_Com;
extern int SigmaLsOmegabaseIbasePerVbase;
extern int RsIbasePerVbase;
extern int LsOmegabaseIbasePerVbase; 
int Ibase = 0,Omegabase = 0; 
extern int isa_ADC,isb_ADC,isc_ADC;
extern int isa;
extern int isb;
extern int isc;  
extern int isalpha;
extern int isbeta;   
extern int isd;
extern int isq;   

extern long sin_theta,cos_theta;
extern int isd_LPF ,isq_LPF ;

int softstart = 0;


extern _iq17 Free1;
extern int Vsq_ref;

extern int Carrier_Frq;

extern int First_Test_ND2;

extern long Imr,Id,Theta_out,Isq_ref;
extern long Kp,Ti,Ki,Max,Min,intregrate,PI_out,Err,integral_PI_long,Volt_VperFProfile;
extern int Speed_vector_Com,isq_ref; 

extern int flag_PWMEnable;

long direction_detec = 0;
extern int direction;

#define BUFFSIZE 128
int gBuf[BUFFSIZE]; 
int hBuf[BUFFSIZE];   
int ii = 0;
int jj = 0;
extern int Period_PWM,Vdcbus;
extern long Random_reg;

long Vin = 0,I_s = 0;
float R_s = 0;

extern long Volt_Vq;


int Const1 = 0, Const2 = 0, Const3 = 0, Const4 = 0;

int Thetatest = 0;
long Ampli = 2500;
//int va_com = 0, vb_com = 0,vc_com = 0;
//int Vmax = 0,Vmin = 0;
extern int va_com, vb_com, vc_com;
extern int Vmax,Vmin;

void Motor_Drive(void)
{
//GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;
  	if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)
	{ 
		softstart = 0;//For Vector
	}  

    	ReadIV();

    if(grp_num[5].value[0] <= 3 && ND2System_Reg.FLAG_SYST_CTRL.bit.Syst_ready==1)//F501   
	{
		V_PER_F();
	}
/*	if(grp_num[7].value[0] == 1)//F701
	{ 
		RsIbasePerVbase = 1460;
	   	SigmaLsOmegabaseIbasePerVbase = 2261;
	   	LsOmegabaseIbasePerVbase = 27073;

		Speed_vector_Com = F_Command>>1;
 		Vecter_Control_Decoupling(); 
	}*/


	Function_BtStrp();
 	Run_ePWM();

//=======Clear Ini ADC============
	//AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1 to CONV00
	//AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1); //Clear ADC interrupt
//=======Clear Ini ADC============ 
}
