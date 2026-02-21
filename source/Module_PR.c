#include "F28x_Project.h"
#include "include\ND2.h"
#include "f28004x_sw_prioritized_isr_levels.h"

#define Ts 0.001/10

#define PI 3.14159265358979

#define F 50

float32_t K_HC = 2.0*PI*F;

struct PR_REG_IQ PR_Reg_IQ={0,0,0,0,0,
                      0,0,0,0,0,
                      0,0,0,0,0,
                      0,0,0,0,0,
                      0,0,0,0,0,
                      0,0,0,0,0};

//---External variable import--------------------------------------
struct data_grp{
	Uint16 value[100];
	Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber]; 
/////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////// 
void CalPRControl(void)
{
       // Compute the error

        PR_Reg_IQ.Err = PR_Reg_IQ.Ref - PR_Reg_IQ.Fdb;
    //    PR_Reg_IQ.Err = PR_Reg_IQ.Err; //edit
    //    PR_Reg_IQ.Err =0;
        PR_Reg_IQ.yk = PR_Reg_IQ.yk_old + _IQ17mpy(PR_Reg_IQ.Ki,PR_Reg_IQ.err_old) + _IQ17mpy(PR_Reg_IQ.wts,PR_Reg_IQ.vk_old);
        PR_Reg_IQ.vk = PR_Reg_IQ.vk_old - _IQ17mpy(PR_Reg_IQ.wts,PR_Reg_IQ.yk);

        PR_Reg_IQ.Up  = _IQ17mpy(PR_Reg_IQ.Kp,PR_Reg_IQ.err_old);
        PR_Reg_IQ.OutPreSat = PR_Reg_IQ.Up + PR_Reg_IQ.yk;// + PR_Reg_IQ.yk3 + PR_Reg_IQ.yk5;
    //  PR_Reg_IQ.Out = PR_Reg_IQ.Up + PR_Reg_IQ.yk + PR_Reg_IQ.yk3 + PR_Reg_IQ.yk5;
        PR_Reg_IQ.OutPreSat = PR_Reg_IQ.OutPreSat;//edit

        // Saturate the output
        if (PR_Reg_IQ.OutPreSat > PR_Reg_IQ.OutMax)
          PR_Reg_IQ.Out =  PR_Reg_IQ.OutMax;
        else if (PR_Reg_IQ.OutPreSat < PR_Reg_IQ.OutMin)
          PR_Reg_IQ.Out =  PR_Reg_IQ.OutMin;
        else
          PR_Reg_IQ.Out = PR_Reg_IQ.OutPreSat;

        // Compute the saturate difference
        PR_Reg_IQ.SatErr = PR_Reg_IQ.Out - PR_Reg_IQ.OutPreSat;

        PR_Reg_IQ.err_old = PR_Reg_IQ.Err;
        PR_Reg_IQ.vk_old = PR_Reg_IQ.vk;
        PR_Reg_IQ.yk_old = PR_Reg_IQ.yk;

} 
////////////////////////////////////////////////////////////////// 
void SetupPRControl(void)
{
        PR_Reg_IQ.Kp = _IQ17(0.3) ;        //0.3
        PR_Reg_IQ.Ki = _IQ17(Ts/0.0215);
        PR_Reg_IQ.Ki3 = _IQ17(Ts/0.7);
        PR_Reg_IQ.Ki5 = _IQ17(Ts/0.999);
        PR_Reg_IQ.wts = _IQ17(Ts*K_HC);   //Ts*w              0.02453125
        PR_Reg_IQ.wts3 = _IQ17(Ts*3.0*K_HC);  //Ts*w      0.07359375
        PR_Reg_IQ.wts5 = _IQ17(Ts*5.0*K_HC);  //Ts*w  0.12265625
        PR_Reg_IQ.OutMax = _IQ17(0.975);
        PR_Reg_IQ.OutMin = _IQ17(-0.975);
        PR_Reg_IQ.Kc = _IQ17(0.9999);
       // Kipr_a = _IQ17(Ts/0.0005);
       // Kipr_b = _IQ17(Ts/0.008);

        PR_Reg_IQ.err_old = 0;
        PR_Reg_IQ.vk_old = 0;
        PR_Reg_IQ.yk_old = 0;
        PR_Reg_IQ.vk_old3 = 0;
        PR_Reg_IQ.yk_old3 = 0;
        PR_Reg_IQ.vk_old5 = 0;
        PR_Reg_IQ.yk_old5 = 0;


}
