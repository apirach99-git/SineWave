#include "F28x_Project.h"
#include "include\ND2.h" 

void PI_Antiwindup(void);

long Kp = 0;
long Ki = 0;
long Ti = 0;
long Max = 0;
long Min = 0;
long Err = 0;
long PI_out = 0;
long out1 = 0;
long intregrate = 0;

extern _iq17 Free1,Free2,Free3,Free4,Free5,Free6;

void PI_Antiwindup(void)
{
//------------Input PI------------------
/*	
	Kp = _IQ17(0.01);//Kp
	Ti = _IQ25(0.001);//Ti = 0.001ms

	Err = _IQ17(1);


	Ki = _IQ17div(6711,Ti);//_IQ17(0.2);//Ki = Ts/Ti

	

	Max = _IQ17(2);
	Min = _IQ17(-2);
*/
//------------Input PI------------------

	Free1 = _IQ17mpy(Err,Kp);
	Free2 = _IQ17mpy(Free1,Ki);

   	if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)//OFF PWM setting
	{
		intregrate = 0;
	}  

	out1 = Free1 + Free2 + intregrate;
	
	if(Max > out1 && Min < out1)
		PI_out = out1;
	else
	{
		if(out1 >= Max)
			PI_out = Max;
		if(out1 <= Min)
			PI_out = Min;
	}

	Free3 = _IQ17mpy(Ki,PI_out) - _IQ17mpy(Ki,out1);

	intregrate = PI_out - Free1 + Free3;
	 
} 





