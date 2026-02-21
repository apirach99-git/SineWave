//
//
//###########################################################################
//
// FILE:   VariableDSP2808.h
//
// TITLE:  Variable of system
//
//###########################################################################
#ifndef VARIABLEDSP2808_AUI_H
#define VARIABLEDSP2808_AUI_H 


#define PI 3.14156 


//#pragma DATA_SECTION(PWM_COM,"PWM");
struct  PWM_COMMAN{
   long va_com;
   long vb_com;
   long vc_com;
}; 
struct  PWM_COMMAN PWM_COM;  
/*
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
struct  DAC_SPI DAC; */

//===========V/F==============
int Vsd_ref = 0;
int Vsq_ref= 0;
int Theta_Q15= 0;
int sin_Theta_o= 0;
int cos_Theta_o= 0;
int Vs_alpha= 0;
int Vs_beta= 0;


int Vbase = 500;
int omega = 0;
int F_Command = 0;

int va = 0;
int vb = 0;
int vc = 0;  

float SinTheta_fp=0;
float CosTheta_fp=0;
//===========V/F==============



//=======SpeedFeedBack========

int Speed_Feedback = 0;

//=======SpeedFeedBack========

//========PWM==========
int Vdcbus = 0;
//========PWM==========

//=======Decoupling Control===========

int Speed_com= 0;

int error_PI= 0;
int output_PI= 0;

int isd_ref= 0;
int isq_ref= 0; 

int wc_decoup_statordynamic1= 0; 
int wc_decoup_statordynamic2= 0; 
int isd_est= 0; 
int isq_est= 0; 
int wc_RotorFlux1= 0; 
int wc_RotorFlux2= 0; 
int imr= 0; 
int omega_slip= 0; 
int omega_o= 0; 
int Theta= 0 ;
int Theta_o= 0;

int SigmaLsOmegabaseIbasePerVbase= 0;
int RsIbasePerVbase= 0;
int LsOmegabaseIbasePerVbase= 0;

int kp_gain_PI= 0;
int ki_gain_PI= 0;
int kc_gain_PI = 0;
int limit_min_PI= 0 ;
int limit_max_PI = 0; 

int integral_PI	= 0;
long X_n= 0			;
long integral_PI_long= 0;  

int Speed_vector_Com;

//=======Decoupling Control===========


//=========Speed==========

int direction	=0;
int theta_new	=0;
int theta_raw	=0;
int theta_mech	=0;
int theta_elec	=0;
int shaft_angle	=0;
int speed_skp_cnt=0;
int s_angle_old	=0;
int s_angle_cur	=0;
int speed_frq 	=0;  

//=========Speed==========

//========ADC==============
//int isa_ADC = 0,isb_ADC = 0,isc_ADC = 0;
//int isa = 0;
//int isb = 0;
//int isc = 0;
int isd_LPF = 0,isq_LPF = 0;
//========ADC==============

//=====Current Control=====
//int isalpha = 0;
//int isbeta = 0;   
//int isd = 0;
//int isq = 0;   
//=====Current Control=====

//======FOC=======
int isd_est_old = 0; 
//======FOC=======



#endif  // end of DSP280x_DEVICE_H definition
