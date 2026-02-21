//10-27-55 Change R of CT
//2013-8-6 : Add bit check flying start befor slip comp. function
//2013-8-7 : move initial "Freq_rev" to SttsCtrl.c
//2013-8-7 : Revise Slip compensation function


#include "F28x_Project.h"
#include "include\ND2.h"

/****************Description************************
Function Success

F501 Control mode
	0 = Linear V/F
	1 = Energy saving V/F
	2 = Quadratic V/F
	3 = Programming V/F

F502 Torque boost
F510 Slip compensation
****************************************************/


//-------------Select_Star_Delta------------------ 
#define Star			1
#define Delta			2
//-------------Select_Star_Delta------------------   

#define TWO_PI      6.283185307f
#define INV_2POW30  (1.0f / 1073741824.0f)

void V_PER_F(void);

extern int Vsd_ref;
extern int Vsq_ref;
extern int Theta_Q15;
extern float SinTheta_fp;
extern float CosTheta_fp;
extern int Vs_alpha;
extern int Vs_beta;

extern int omega;
extern int va;
extern int vb;
extern int vc;

extern int CountFreqIn;

struct data_grp{
	Uint16 value[100];
	Uint16 factor[100];
};

//struct data_grp grp_num[8];  
extern struct data_grp grp_num[8] ;  


extern int Vbase;

int Vrate_Keypad = 0;
int Frate_Keypad = 0;

int Vrate_Solfware  = 0;
int Frate_Solfware  = 0;

int BoostVoltage_Percent = 0;
int BoostVoltage         = 0;

int Voltage_VperF_ref   = 0;
int Voltage_VperF_input = 0;

extern int F_Command;


static float theta_pu = 0.0f;
static float omega_pu = 0.0f;
int Slope = 0;

int Status_PWM = 0;


/*+++++++++++++++++++++++++++++++++++++++++++++

Descript : V/F


	V = Slope*F + Vboost

	Slope = (Vmax - Vboost) / Fmax

+++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------------------
============================= Auto Type Torque Boost ================================

                                 ___________________________________________________
                                /                2
                               / | EmR.omega_e  |   |        2                  2 |
   Us = (Is.Rs.cos_Theta) +   /  |--------------| - | (Is.Rs) - (Is.Rs.cos_Theta) |   
                            \/   |   omega_eR   |   |                             |

when;

   is_alpha = sqrt(2/3) * { is_u - is_v>>1 - is_w>>1 }; 
   is_beta  = sqrt(2/3) * { ( sqrt(3)*is_v>>1 ) - ( sqrt(3)*is_v>>1 ) };  

     __   
   | is | =  sqrt(is_alpha^2 + is_beta^2)
               __
     Is   =  | is | * sqrt(2)/3      


                     |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
    is_alpha   o---->|  cos_omega_e     sin_omega_e    |----> is_d
    is_beta    o---->| -sin_omega_e     cos_omega_e    |----> is_q
                     |_________________________________|

   Is.cos_Theta = is_q * sqrt(2)/3      

============================= Slip Frequency Compensetion ================================

          |-           0            ; Is <= I_noload
omega_S = |- (omega_SR * Is / IsR)      ; I_noload < Is <= IsR
          |-        omega_SR         ; IsR < Is
*/
 


_iq17 Vrate = 0;
_iq17 Frate = 0;
long Vboost = 0;
long slope  = 0;
long Voltage_VperF = 0;
long Voltage_VperF_Quad = 0;

_iq17 F_Quad = 0,F_3_Quad = 0,F_sqrt3_Quad = 0;
_iq17 F1=0,F2=0,F3=0,V1=0,V2=0,V3=0,m1=0,m2=0,m3=0,m4=0,Fcheck=0;

_iq17 isu = 0,isv = 0,isw = 0;

_iq17 Free1 = 0,Free2 = 0,Free3 = 0,Free4 = 0,Free5 = 0,Free6 = 0;

long Is = 0;
long Iscos_Theta = 0;
long EmR = 0;
long omega_e  = 0;
long omega_eR = 0;

 
extern int isd,isq; 

long I_noload   = 0;//I_noload = 14418;//Q17	2.2A
long Slip_rate  = 0;//Slip_rate = 16;//		100% = 50Hz = 328,   5% = 2.5Hz = 16.4
long I_fullload = 0;//I_fullload = 24969;//Q17 3.81A = I_noload *sqrt(3) = 3.81A 		Ibase = 20A
long Slip_Compensate = 0;
extern int count_200us;
int Slip_Compensate_LPF = 0; 


long Volt_VperFProfile = 0; 
long Volt_Vq = 0;
long Volt_Vd = 0;

long Freq_rev = 0;

////////////////////////////
long ActivePower = 0;
long VsLN, Isrms;
float theta_autotune,cos_theta_autotune, sin_theta_autotune;
long LsLr, RsRr,Z;

long  vSalpha, vSbeta, iSalpha, iSbeta, pS, qS, pS_LPF, qS_LPF, Isrms_LPF;
long vMu, vMv, vMw, iMu, iMv, iMw, RSu, RSv, RSw;
long vSd,vSq,iSd,iSq;
///////////////////

//Gus
float Po_total;
float Qo_total;
float So_total;
float PF;
float Vline_rms;

long theta_accumulator_Q30=0;
const long omega_Q30 = 5369783;//5368709;
long omega_=0;
_iq     Buff_P;
_iq     Buff_Q;


/*
         case Star :// Parameter of motor if connected with star Type
               Rr = 0.885*3; Lr = 0.12*3; M  = 0.12*3; Rs = 1.98*3; Ls = 0.117*3;   p  = 4;   Sigma = 0.084; J  = 0.021 ;  

         case Delta :// Parameter of motor if connected with delta Type
                Rr = 0.885;   Lr = 0.12;   M  = 0.12;   Rs = 1.98;   Ls = 0.117; p  = 4; Sigma = 0.084;   J  = 0.021 ; 
*/

void V_PER_F(void)
{
	//Is = _IQ17mpy(IV_Read_reg.I_rms,SQRT2);
	if(grp_num[5].value[1] == 0)//F502 = 0 Manual torque boost
	{  
		//============= 4. Programming =======================
	   	if(grp_num[5].value[0] == 3)//Programmable control mode F501=3
	   	{

		//============= 4. Programming =======================					   
		   	
	   }
	   else////Linear,Energy,Quadratic control mode  F501<3
	   {
	//============= 1. Linear V/F =======================

			Vboost = _IQ17mpy(Vrate,ND2System_Reg.Vboost); //Q17 * Q17 / Q17

			slope = Vrate;
			Voltage_VperF = _IQ17mpy(MainIQ_Variable.FCommand,slope);//Q17 * Q17 / Q17

			if(Voltage_VperF<0)
				Voltage_VperF = -Voltage_VperF;

			Volt_VperFProfile = (Voltage_VperF + Vboost);//Q17

/*
			//--------------------------
			// 1) Convert input Q17 -> float
			//--------------------------
			float Vrate_f     = (float)Vrate     / IQ17_SCALE_F;
			float VboostIn_f  = (float)ND2System_Reg.Vboost / IQ17_SCALE_F;
			float Fcmd_f      = (float)MainIQ_Variable.FCommand / IQ17_SCALE_F;
			//--------------------------
			// 2) Compute float values
			//--------------------------
			float Vboost_f        = Vrate_f * VboostIn_f;       // float multiply
			float Voltage_VperF_f = Fcmd_f * Vrate_f;
			// abs()
			if (Voltage_VperF_f < 0.0f)
			    Voltage_VperF_f = -Voltage_VperF_f;
			// profile
			float Volt_VperFProfile_f = Voltage_VperF_f + Vboost_f;

			Volt_VperFProfile   = (_iq)(Volt_VperFProfile_f   * IQ17_SCALE_F);
*/






	//============= 1. Linear V/F =======================

	//============= 2. Energy =======================
			if(grp_num[5].value[0] == 1)//Energy saving control mode
			{ 

			}
	//============= 2. Energy =======================

	//============= 3. Quadratic =======================
			if(grp_num[5].value[0] == 2)//Quadratic control mode
			{

			}
	//============= 3. Quadratic =======================
		}
	 
		
	}
	else //F502 = 1 Auto torque boost
	{

	}
 

//(Q15/T_rate)*T_sampling = 328(Q15),T_rate = 20ms ,T_sampling = 200us		<--  5kHz  
//	(Frate * F_Command) / 328 = (131072 * 4096) / 328 = 1636801		at 50Hz 

//	omega = ((long)_IQ17(1) * F_Command) / 1636801;//409200;//++++++++++++++++++++++++++++++++
//	omega = _IQ1div(_IQ11(MainIQ_Variable.FCommand),1636801);//Q29	

//	omega = ((long)328 * MainIQ_Variable.FCommand) / Freq_rev ;
//	omega = ((long)16 * _IQ17(1.0)) / Freq_rev;
//	omega = ((long)164 * (MainIQ_Variable.FCommand)+ _IQ17(0.1)) / Freq_rev ;

	//omega_ = ((long)5368709 * _IQ17(1.0)) / Freq_rev;

	omega_ = _IQ17mpy(5368709,(MainIQ_Variable.FCommand+CountFreqIn));
//	omega_ = _IQ17mpy(5368709,(MainIQ_Variable.FCommand));
//=========================== Slip Compensate ===========================

//=========================== Slip Compensate ===========================
	Vsd_ref = 0;

	Volt_Vq = Volt_VperFProfile;//Q17

//Limit Vout by Vrate(Voltage motor)
	if(Volt_Vq >= Vrate)Volt_Vq = Vrate;
//End Limit Vout by Vrate(Voltage motor)

//	DCBreak();  //Has Volt_Vq Q17

	if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)//OFF PWM setting
	{
		Volt_Vd = 0;
		Volt_Vq = 0;
	}
	else//Run Motor
	{  
		Vsq_ref = Volt_Vq>>5;//	Q17 / Q5 = Q12
		Volt_Vd = 0;
	}


//==========================>>>	Volt_Vd and Volt_Vq Bring to Display Motor Voltage;
/*
 *              |Valpha|        | cos (Theta)   -sin(Theta) | | Vd |
                |      |   =    |                           | |    |
                |Vbeta |        | sin (Theta)    cos(Theta) | | Vq |

                |Va|               |  1       0        | | Valpha |
                |Vb|   = sqrt(2/3) | -1/2    sqrt(3)/2 | |        |
                |Vc|               | -1/2   -sqrt(3)/2 | | Vbeta  |
*/
//=========================================================================================

    omega_pu = (float)omega_ * INV_2POW30;   // ≈ 0.005 ที่ 50Hz/10kHz

    // 2) accumulate angle 0..1
    theta_pu += omega_pu;
    if (theta_pu >= 1.0f)      theta_pu -= 1.0f;
    else if (theta_pu < 0.0f)  theta_pu += 1.0f;

    // 3) per-unit -> rad
    float theta_rad = theta_pu * TWO_PI;

    // 4) sin/cos float + limit 0.99
    SinTheta_fp = sinf(theta_rad) * 0.99f;
    CosTheta_fp = cosf(theta_rad) * 0.99f;

    // 5) limit float safety
    SinTheta_fp = fmaxf(-0.99f, fminf(SinTheta_fp, 0.99f));
    CosTheta_fp= fmaxf(-0.99f, fminf(CosTheta_fp, 0.99f));

    float VsdRef_fp = (float)Vsd_ref / 4096.0f;     // 2^15
    float VsqRef_fp = (float)Vsq_ref / 4096.0f;

    float Vs_alpha_fp =  VsdRef_fp * CosTheta_fp - VsqRef_fp * SinTheta_fp;
    float Vs_beta_fp  =  VsdRef_fp* SinTheta_fp + VsqRef_fp* CosTheta_fp;

    // 4) limit แล้วแปลงกลับเป็น Q12
    Vs_alpha_fp = fmaxf(-0.99f, fminf(Vs_alpha_fp, 0.99f));
    Vs_beta_fp= fmaxf(-0.99f, fminf(Vs_beta_fp, 0.99f));


        SinTheta_fp = fmaxf(-0.99f, fminf(SinTheta_fp, 0.99f));
        CosTheta_fp= fmaxf(-0.99f, fminf(CosTheta_fp, 0.99f));

        Vs_alpha = (int16_t)(Vs_alpha_fp* 4096.0f);  // Q12
        Vs_beta  = (int16_t)(Vs_beta_fp  * 4096.0f);  // Q12

   // PR Controller — Floating Point
   // Ref: Vs_alpha normalised to ±1.0  (Q12 → float: /4096)
   PR_Reg.Ref = Vs_alpha_fp;               // sine reference  ±1.0 p.u.
   PR_Reg.Fdb = _IQ17toF(IV_Read_reg.PU_VO) * 0.35f;  // output voltage feedback (float)

   if(ND2System_Reg.STATE_FLAG.bit.State != state_stop) // Run only when not stopped
   {
       PR_Update(&PR_Reg);                 // execute one PR step (float)
   }

   // PR_Reg.Out is modulation index ±0.975 → feed to SPWM
   // For 3-phase mode: map to va/vc (Q12 range ≈ ±4096)
   va = (int16_t)(PR_Reg.Out * 4096.0f);
   vc = -va;

/*Average  power calculation by using Alpha-Beta transformation method
   Buff_P = _IQmpy( _IQ12toIQ(Vs_alpha), IV_Read_reg.I_Alpha)+_IQmpy(_IQ12toIQ(Vs_beta), IV_Read_reg.I_Beta);
   Buff_Q = _IQmpy(_IQ12toIQ(Vs_alpha), IV_Read_reg.I_Beta)-_IQmpy(_IQ12toIQ(Vs_beta), IV_Read_reg.I_Alpha);
*/
/*
   Buff_P = _IQmpy( IV_Read_reg.Vout_Rms, IV_Read_reg.Isd);
   Buff_Q = _IQmpy(IV_Read_reg.Vout_Rms, IV_Read_reg.Isq);

   IV_Read_reg.Filtered_P = LowPassFilter(Buff_P, IV_Read_reg.Filtered_P, _IQ17(12), _IQ17(2));
   IV_Read_reg.Filtered_Q = LowPassFilter(Buff_Q, IV_Read_reg.Filtered_Q, _IQ17(12), _IQ17(2));

   IV_Read_reg.S  = _IQmag(IV_Read_reg.Filtered_P, IV_Read_reg.Filtered_Q);
   IV_Read_reg.pf = _IQdiv(IV_Read_reg.Filtered_P, IV_Read_reg.S);
*/
 /*
   Po_total = _IQtoF(_IQmpy(_IQmpy(IV_Read_reg.Isd,
                    MainIQ_Variable.I_base), IV_Read_reg.Vout_Rms))*1000;
   Qo_total = _IQtoF(_IQmpy(_IQmpy(IV_Read_reg.Isq,
                    MainIQ_Variable.I_base), IV_Read_reg.Vout_Rms))*1000;
*/
/*
  Po_total  = _IQtoF(_IQmpy(IV_Read_reg.Filtered_P, MainIQ_Variable.I_base))*1000;
  Qo_total  = _IQtoF(_IQmpy(IV_Read_reg.Filtered_Q, MainIQ_Variable.I_base))*1000;
  So_total  = _IQtoF(_IQmpy(IV_Read_reg.S, MainIQ_Variable.I_base))*1000;
  PF = Po_total/So_total;

   Vline_rms = _IQtoF(IV_Read_reg.Vout_Rms)*1000;
*/
   //Po_Filtered = LowPassFilter(Po_total, Po_Filtered, _IQ17(5), _IQ17(1));

//####################################################### 

}
