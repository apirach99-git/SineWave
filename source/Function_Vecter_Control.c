#include "F28x_Project.h"
#include "IQmathLib.h"

void Vecter_Control(void);
void PI_Controller(void);
void Decoupling_Control(void);

extern int error_PI;
extern int output_PI;

extern int isd_ref;
extern int isq_ref; 

extern int wc_decoup_statordynamic1; 
extern int wc_decoup_statordynamic2; 
extern int isd_est; 
extern int isq_est; 
extern int wc_RotorFlux1; 
extern int wc_RotorFlux2; 
extern int imr; 
extern int omega_slip; 
extern int omega_o; 
extern int Theta ;
extern int Theta_o;

extern int SigmaLsOmegabaseIbasePerVbase;
extern int RsIbasePerVbase;
extern int LsOmegabaseIbasePerVbase;
extern int Vsd_ref;
extern int Vsq_ref;

extern int Vs_alpha;
extern int Vs_beta;
extern int va;
extern int vb;
extern int vc;


extern int Speed_Com_S_Curve;
extern int Speed_Feedback;
extern int kp_gain_PI;
extern int ki_gain_PI;
extern int kc_gain_PI;
extern int limit_min_PI ;
extern int limit_max_PI ;

extern int Speed_vector_Com;

struct data_grp{
   uint16_t value[100];
   uint16_t factor[100];
};
extern struct data_grp grp_num[8] ;   

extern int speed_skp_cnt;


void Decoupling_Control(void)
{
/*; Description   : Parameter of motor
;					Lr = 0.12
;					Ls = 0.131
;					M  = 0.12
;					Rr = 0.885
;					Rs = 1.84  //1.95
;					p  = 4
;					sigma = 1  -   M^2/(Lr*Ls) = 0.084
;					J  = 0.021 
;----------------------------------------------------------------------------------*/

//-----------------------------------------------------
//			 	 LowPass Filter
//----------------------------------------------------- 
//
//			wc*T					   wc*T - 2
//	y(k) = ------ * {x(k)+x(k-1)}  -  ---------- * y(k-1) 
//		  wc*T + 2					   wc*T + 2		
//	Example Determine 1/wc = 5.98 ms 
//	omega_LPF1 = 272;//Q15
//	omega_LPF2 = -4028;//Q12
//	vo = (((long)(vi+vi_old)*omega_LPF1) >> 15) - (((long)vo*omega_LPF2)>>12);
//	vi_old = vi;
//=====================================================
//-----------------------------------------------------
//			  Decoupled Stator Dynamic
//-----------------------------------------------------  
//  wc = sigma*Ls / Rs = (0.084 * 0.131) / 1.84
//	isq_ref = vi;
	wc_decoup_statordynamic1 = 571;//Q15
	wc_decoup_statordynamic2 = -3953;//Q12

//	isd_est = (((long)(isd_ref + isd_ref_old)*wc_decoup_statordynamic1) >> 15) - (((long)isd_est*wc_decoup_statordynamic2)>>12);
//	isd_ref_old = isd_ref;
	isd_est = isd_ref;

//	isq_est = (((long)(isq_ref + isq_ref_old)*wc_decoup_statordynamic1) >> 15) - (((long)isq_est*wc_decoup_statordynamic2)>>12);
//	isq_ref_old = isq_ref; 
	isq_est = isq_ref;
//-----------------------------------------------------

//-----------------------------------------------------
//			  Rotor Flux
//-----------------------------------------------------   
//  wc = Lr / Rr = 0.12 / 0.885
	wc_RotorFlux1 = 12;//Q15
	wc_RotorFlux2 = -4092;//Q12
//	imr = (((long)(isd_est + imr_old)*wc_RotorFlux1) >> 15) - (((long)imr*wc_RotorFlux2)>>12);
//	isd_est_old = isd_est; 
	imr = isd_est;

//	omega_slip = ((long)isq_est*193)/imr;//Rr/Lr = ((0.885/0.12) / omega_base) * 2^13 = 193; ,omega_base = 314    ===> Q12
	omega_slip = _IQ1div(_IQ1mpy(isq_est,193),imr);//100%

//	omega_o = (Speed_vector_Com<<2) + omega_slip;//Q13

	omega_o = (Speed_Feedback<<2) + omega_slip;//Q13


//(Q15/T_rate)*T_sampling = 164(Q15),T_rate = 20ms ,T_sampling = 100us		<-- 10kHz
//(Q15/T_rate)*T_sampling = 328(Q15),T_rate = 20ms ,T_sampling = 200us		<--  5kHz

	Theta = ((long)328*omega_o)>>13;
	Theta_o = (Theta_o + Theta) & 0x7FFF;//Maintain positive value of angle command



//-----------------------------------------------------
//			  Decoupling Controller
//-----------------------------------------------------   
//Vsd_ref = Rs*isd - w_mr*sigma*Ls*isq_est
   Vsd_ref = _IQ12mpy(RsIbasePerVbase,isd_ref) - _IQ12mpy(_IQ13mpy(omega_o,isq_est),SigmaLsOmegabaseIbasePerVbase) ;//+ _IQ12mpy(isd_est - imr,73);//Q12
//         (Rs*10/500)*4096 = 162                    (sigma*Ls*10*314/500)*4096 = 253

//Vsq_ref = Rs*isq + w_mr*Ls*isd_est  
   Vsq_ref = _IQ12mpy(RsIbasePerVbase,isq_ref) + _IQ12mpy(_IQ13mpy(omega_o,isd_est),LsOmegabaseIbasePerVbase) ;//+ _IQ12mpy(_IQ13mpy(omega_o,imr),3087);//Q12
//         (Rs*10/500)*4096 = 162                 (w_mr*Ls*314*10/500)*4096 = 3009

//-----------End Decoupling Controller----------------- 
	return;	

} 
 


void Vecter_Control(void)
{
//========================================================
//				PI - Controller
//========================================================
//    kp_gain_PI = 25;//25;//150 ;//Q5
//    ki_gain_PI = 1000;//1000;//2500;//Q16

//	kp_gain_PI = grp_num[7].value[1];//138		Q6		F702
//	ki_gain_PI = grp_num[7].value[2];//2000		Q16		F703
	kc_gain_PI = 0;

    limit_min_PI = -8192;
    limit_max_PI = 8192;   

	if(speed_skp_cnt == 0)//5ms
	{
		error_PI =  Speed_vector_Com - Speed_Feedback;//Q12 rpm_base = 3000
		PI_Controller();
		isq_ref = output_PI;
	}
//======================================================== 

//	isd_ref = 1229;	//(3A/10A) * 4096  = 1229
//	isd_ref = 1638;//4A 


//	isd_ref = 2458;//6A

	isd_ref = 473;//6A / sqrt(3) if Star type



//========================================================
//				Decoupling_Control
//========================================================
	Decoupling_Control();
//========================================================  





 



	return;
} 





