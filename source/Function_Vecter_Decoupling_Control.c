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
   Uint16 value[100];
   Uint16 factor[100];
};
extern struct data_grp grp_num[8] ;   

extern int speed_skp_cnt;

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

long Vsd = 0;
long Vsq = 0;
long isdd = 0;
long isqq = 0;
extern _iq17 Free1,Free2,Free3,Free4,Free5,Free6;

void VDecoupling_Control(void)
{
	isd_est = isd_ref;

	isq_est = isq_ref;
//-----------------------------------------------------

//-----------------------------------------------------
//			  Rotor Flux
//-----------------------------------------------------   
	imr = isd_est;

//	omega_slip = ((long)isq_est*193)/imr;//Rr/Lr = ((0.885/0.12) / omega_base) * 2^13 = 193; ,omega_base = 314    ===> Q12
	omega_slip = _IQ1div(_IQ1mpy(isq_est,193),imr);//100%


	omega_o = (Speed_Feedback<<2) + omega_slip;//Q13

//(Q15/T_rate)*T_sampling = 328(Q15),T_rate = 20ms ,T_sampling = 200us		<--  5kHz

	Theta = ((long)328*omega_o)>>13;
	Theta_o = (Theta_o + Theta) & 0x7FFF;//Maintain positive value of angle command



//-----------------------------------------------------
//			  Decoupling Controller
//-----------------------------------------------------   
//Vsd_ref = Rs*isd - w_mr*sigma*Ls*isq_est
 //  Vsd_ref = _IQ12mpy(RsIbasePerVbase,isd_ref) - _IQ12mpy(_IQ13mpy(omega_o,isq_est),SigmaLsOmegabaseIbasePerVbase) ;//+ _IQ12mpy(isd_est - imr,73);//Q12
//         (Rs*10/500)*4096 = 162                    (sigma*Ls*10*314/500)*4096 = 253

//Vsq_ref = Rs*isq + w_mr*Ls*isd_est  
//   Vsq_ref = _IQ12mpy(RsIbasePerVbase,isq_ref) + _IQ12mpy(_IQ13mpy(omega_o,isd_est),LsOmegabaseIbasePerVbase) ;//+ _IQ12mpy(_IQ13mpy(omega_o,imr),3087);//Q12
//         (Rs*10/500)*4096 = 162                 (w_mr*Ls*314*10/500)*4096 = 3009

//RsIbasePerVbase    5.4*20/500      isdd  = 5A	isqq = 1A		omega_o = 314.1592654 rad	Ibase = 20  
//LsOmegabaseIbasePerVbase		Ls = 0.393   Omegabase = 628.3185307 rad   Vbase = 500
	Free1 = _IQ17mpy(28312,32768);  //RsIbasePerVbase        5.4ohms* 5A 
	Free2 = _IQ17mpy(_IQ17(0.5),6554);//omega_o,isq_est		314.1592654rad*1A
	Free3 = _IQ17mpy(Free2,_IQ17(9.87216));//,LsOmegabaseIbasePerVbase*2^17 = 9.87216			Ls=0.393H				
	//5.4ohms* 5A  -  0.393H*314.1592654rad*1A
	Vsd = Free1 - Free3;

//-----------End Decoupling Controller----------------- 
	return;	

} 
 


void Vecter_Control_Decoupling(void)
{
//========================================================
//				PI - Speed Controller
//========================================================

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

	isd_ref = 473;//6A / sqrt(3) if Star type
	isq_ref = 473;


//========================================================
//				Decoupling_Control
//========================================================
	VDecoupling_Control();
//========================================================  




 



	return;
} 





