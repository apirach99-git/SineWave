#include "F28x_Project.h"
#include "IQmathLib.h"
#include "Module_CheckVDC.h"
#include "Module_SttsCtrl.h"
#include "Module_PID.h" 

void PI_Controller(void);

extern int output_PI	;
extern int error_PI		;
extern int error_PI_old	;
extern int kp_gain_PI	;
extern int ki_gain_PI	;
extern int kc_gain_PI	;
extern int limit_min_PI	;
extern int limit_max_PI	;
extern int integral_PI	;
extern long X_n			;
extern long integral_PI_long;

void PI_Controller(void)
{
   	if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)//OFF PWM setting
	{
		error_PI = 0;
	} 
	
	X_n = _IQ6mpy(error_PI,kp_gain_PI) + integral_PI;//kp_gain_PI(Q5)   integral_PI(Q12)
//	X_n = _IQ12mpy(error_PI,kp_gain_PI) + integral_PI;//kp_gain_PI(Q12)

	if(X_n > limit_max_PI)
		output_PI = limit_max_PI;
	else if(X_n < limit_min_PI)
		output_PI = limit_min_PI;
	else	
		output_PI = X_n;

//	X_n = X_n - output_PI;

		integral_PI_long =  ((long)error_PI*ki_gain_PI) + integral_PI_long;//Q28	ki_gain_PI(Q16)

//Limit Intregral +Q28 to -Q28
		if(integral_PI_long > 268435456)//268435456
			integral_PI_long = 268435456;//Q28
		if(integral_PI_long < -268435456)
			integral_PI_long = -268435456;//-Q28 

		integral_PI = (int)(integral_PI_long>>16);//>>16;//(Q12) So, integral_PI has value between +Q12 to -Q12


   	if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)//OFF PWM setting
	{
		integral_PI_long = 0; 
		output_PI = 0;
	}

// 	U_dc_n = Kp_U_dc*U_dc_Error + Integral_U_dc
// 	if U_dc_n > U_dc_max ; U_dc_out = U_dc_max
//	if U_dc_n < U_dc_min ; U_dc_out = U_dc_min
//	if U_dc_max < U_dc_n < U_dc_min  ; U_dc_out = U_dc_n
//	Integral_U_dc = ki_U_dc*U_dc_Error + kc_U_dc*(U_dc_out-U_dc_n) + Integral_U_dc
/*

   if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)//OFF PWM setting
		integral_PI = 0; 


//1----U_dc_n = Kp_U_dc*U_dc_Error + Integral_U_dc
//	X_n = (((long)kp_gain_PI*error_PI)>>12) + integral_PI ;
	X1 = ((long)kp_gain_PI*error_PI)>>12;
	X_n = X1 + integral_PI ; 

//2----if U_dc_n > U_dc_max ; U_dc_out = U_dc_max,if U_dc_n < U_dc_min ; U_dc_out = U_dc_min,if U_dc_max < U_dc_n < U_dc_min  ; U_dc_out = U_dc_n 
	if(X_n > limit_max_PI)
		output_PI = limit_max_PI;
	else if(X_n < limit_min_PI)
		output_PI = limit_min_PI;
	else
		output_PI = X_n ;


//3----Integral_U_dc = ki_U_dc*U_dc_Error + kc_U_dc*(U_dc_out-U_dc_n) + Integral_U_dc
	X_n = output_PI - X_n  ; 
//	integral_PI = (((long)ki_gain_PI*error_PI)>>12) + (((long)kc_gain_PI*X_n)>>12) + integral_PI ;

	X1 = ((long)ki_gain_PI*error_PI)>>12 ; 
	X2 = ((long)kc_gain_PI*X_n)>>12 ; 

	integral_PI = X1 + X2 + integral_PI;*/
} 





