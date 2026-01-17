//2013-6-24 : add state start/stop auto tune
//2013-7-27 : debug reset autotune(clearing Vq) 

#include "F28x_Project.h"
#include "ND2.h"  
#include <math.h>

//2013-6-24 : add state start/stop auto tune
//2013-7-27 : debug reset autotune(clearing Vq)
//2014-09-13 : Auto Tune parameter of IM find Rs SigmaLs Rr' Lm'.
//2015-02-14 : Auto Tune parameter Rs, Lm, Rr and Ls = Lr
//2023-01-xx : AutoTune is done


//#include "DSP280x_Device.h"     // DSP280x Headerfile Include File
//#include "DSP280x_Examples.h"   // DSP280x Examples Include File


char AutoTuneStep = 0;
struct data_grp{
   Uint16 value[100];
   Uint16 factor[100];
};
extern struct data_grp grp_num[8] ;
extern long Volt_Vq,Volt_Vd;
extern int Theta_Q15,omega,sin_Theta_o,cos_Theta_o,Vsq_ref,Vsd_ref,Vs_alpha,Vs_beta,va,vb,vc;
struct AUTOTUNE_CTRL_REGS AutoTuneCtrlRegs = {0,0,0,0,0,
                                              0,0,0,0,0,
                                              0,0,0,0,0,
                                              0,0,0,0,0,
                                              0,0,0,0,0,
                                              0,0,0,0,0,
                                              0,0,0,0,0,
                                              0,0,0,0,0,
                                              0,0};

extern long Kp,Ki,Ti,Max,Min,Err,PI_out,intregrate,out1;

int IntregrateSigmaLsCount = 0;

#define SQRT_2_3 0.8164965809


/*--ND2System_Reg.STATE_FLAG.bit.State--
#define state_stop          1
#define state_start         2
#define state_startAT       3//state start autotune
#define state_run           4
#define state_stopping      8
#define state_stoppingAT    9//state stopping autotune
---ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd--
#define SystemReady     3
#define SystemAccel     0
#define SystemDecel     1
#define SystemSteady    2
*/

///////////////////////////////////////////////////////////////////
extern _iq17 Frate,Vrate;
long rads = 0, degree = 0, Req = 0, Xeq = 0, Leq = 0;

float Ls = 0;
float Rr = 0;
float Rs = 0;
float Lm = 0;
float SigmaLs = 0;
int AutoTuneFinish = 0;

/*
struct RLS_REGS{
   long F11;
   long F12;
   long F21;
   long F22;

   long FT11;
   long FT12;
   long FT21;
   long FT22;

   long FTF11;
   long FTF12;
   long FTF21;
   long FTF22;

   long Det_FTF;

   long Inv_FTF11;
   long Inv_FTF12;
   long Inv_FTF21;
   long Inv_FTF22;

   long Y11;
   long Y21;

   long Temp11;
   long Temp21;

   long Zeta11;
   long Zeta21;

   long Fnew11;
   long Fnew21;

   long FnewT11;
   long FnewT12;

   long FnewT_P_Fnew;

   long Lnew11;
   long Lnew21;

   long Ynew;

   long Zetanew11;
   long Zetanew21;



};
extern struct RLS_REGS RLS_Regs ;

struct RLS_REGS RLS_Regs = {0,0,0,0,
                            0,0,0,0,
                            0,0,0,0,0,
                            0,0,0,0,
                            0,0,
                            0,0,
                            0,0,
                            0,0,
                            0,0,0,
                            0,0,
                            0,0,0};
*/
/*===========================================================
 *            Least-Squares Curve Fitting
 * ==========================================================
 *
 * 1    Sampling Input (U) and Output (Y)
 *
 *              | u(0)|         | y(0) |
 *      U =     | ... | , Y =   | ...  |
 *              | u(n)|         | y(n) |
 *
 * ----------------------------------------------------------
 *
 * 2    Find Function F
 *
 *              | y(0)  u(0) |      | f11   f12 |
 *      F =     |            |  =   |           |
 *              | y(1)  u(1) |      | f21   f22 |
 *
 * ----------------------------------------------------------
 *
 * 3    Find Transpose of Function F, FT
 *
 *              | f11   f21 |       | FT11  FT12 |
 *      FT =    |           |   =   |            |
 *              | f12   f22 |       | FT21  FT22 |
 *
 * ----------------------------------------------------------
 *
 * 4    Find Function Y
 *
 *              | y(1) |        | Y11 |
        Y =     |      |   =    |     |
 *              | y(2) |        | Y21 |
 *
 * ----------------------------------------------------------
 *
 * 5    Find Transpose of Function F multiplies with F, FT*F
 *
 *              | f11   f21 |   | f11   f12 |
 *      FT*F =  |           | * |           |
 *              | f12   f22 |   | f21   f22 |
 *
 *
 *              | FTF11 FTF12 |
 *           =  |             |
 *              | FTF21 FTF22 |
 *
 *      when
 *
 *      FTF11 = (f11*f11) + (f21*f21)
 *      FTF12 = (f11*f12) + (f21*f22)
 *      FTF21 = (f12*f11) + (f22*f21)
 *      FTF22 = (f12*f12) + (f22*f22)
 *
 *  ----------------------------------------------------------
 *
 * 6    Find inverse of FT*F
 *
 *                      1       | FTF22     -FTF12 |
 *      Inv(FT*F) =   ------  * |                  |
 *                   Det(FT*F)  | -FTF21    FTF11  |
 *
 *
 *                  | InvFTF11  InvFTF12 |
 *                = |                    |
 *                  | InvFTF21  InvFTF22 |
 *
 *      When
 *
 *      Det(FT*F) = (FTF11*FTF22) - (FTF12*FTF21)
 *
 *  ----------------------------------------------------------
 *
 * 7    Find the least square of this, Zeta
 *
 *      Zeta_LS = Inv(FT*F) * FT * Y
 *
 *      | Zeta11 |   | InvFTF11  InvFTF12 |   | FT11  FT12 |   | Y11 |
 *      |        | = |                    | * |            | * |     |
 *      | Zeta21 |   | InvFTF21  InvFTF22 |   | FT21  FT22 |   | Y21 |
 *
 *
 *      | Zeta11 |   | InvFTF11  InvFTF12 |   | FTY11 |
 *      |        | = |                    | * |       |
 *      | Zeta21 |   | InvFTF21  InvFTF22 |   | FTY21 |
 *
 *      When
 *
 *      FTY11 = (FT11*Y11) + (FT12*Y21)
 *      FTY21 = (FT21*Y11) + (FT22*Y21)
 *
 *      So,
 *
 *      Zeta11 = (InvFTF11*FTY11) + (InvFTF12*FTY21)
 *      Zeta21 = (InvFTF21*FTY11) + (InvFTF22*FTY21)
 *
 *
 *===========================================================
 *             Recursive Least-Squares (RLS)
 * ==========================================================
 *
 * From above example, we derive the sampling N = 2.
 *
 * 1    Sampling both the new input and the new output
 *
 *                    | Y(N+1) |   | F11(N+1) |
 *          F(N+1)  = |        | = |          |
 *                    | U(N+1) |   | F21(N+1) |
 *
 *          FT(N+1) = | Y(N+1)  U(N+1) | = | FT11(N+1)  FT21(N+1) |
 *
 *  ----------------------------------------------------------
 *
 * 2    Find function P(N)
 *
 *          P(N) = Inv(FT*F) = Inv(FT(N)*F(N))
 *
 *  ----------------------------------------------------------
 *
 * 3    Find function FT(N+1)*P(N)*F(N+1)
 *
 *
 *                                                           | InvFTF11 InvFTF12 |   | FT11(N+1) |
 *          FT(N+1)*P(N)*F(N+1) = | FT11(N+1)  FT21(N+1) | * |                   | * |           |
 *                                                           | InvFTF21 InvFTF22 |   | FT21(N+1) |
 *
 *
 *                              =    {InvFTF11*FT11(N+1) + InvFTF12*FT21(N+1)} * FT11(N+1)  +
 *                                   {InvFTF21*FT11(N+1) + InvFTF22*FT21(N+1)} * FT21(N+1)
 *
 *  ----------------------------------------------------------
 *
 *
 * 4    Find function L(N+1)
 *
 *                        P(N)*F(N+1)
 *          L(N+1) = ------------------------
 *                   (1 + FT(N+1)*P(N)*F(N+1))
 *
 *  ----------------------------------------------------------
 *
 * 5    Now, we can test parameter at this sampling.
 *
 *
 *      Zeta_LS(N+1) = Zeta_LS(N) + L(N+1)*[Y(N+1) - (FT(N+1)*Zeta_LS(N))]
 *
 *  ----------------------------------------------------------
 *
 */

//============== New AutoTune 2023/02/11 ===============
long IratePU,Irate;
long Vsearch = 0;
long RS = 0;
int Flag_VSearch = 0;

long RR, LR, LS;

extern long vSalpha, vSbeta, iSalpha, iSbeta, pS, qS, pS_LPF, qS_LPF, Isrms_LPF;
extern long vSd,vSq,iSd,iSq;
extern long VsLN, Isrms;
extern long vMu, vMv, vMw, iMu, iMv, iMw, RSu, RSv, RSw;

extern float theta_autotune,cos_theta_autotune, sin_theta_autotune;
extern long LsLr, RsRr,Z;

void Function_AutoTune(void)
{

}
//////////////////////////////////////////////////////////////////////////////////////////
void Find_Rr()
{
    if(AutoTuneCtrlRegs.FINDRR.bit.INITAIL == 0)
    {
        AutoTuneCtrlRegs.CountDelayAutoTune = 0;
        AutoTuneCtrlRegs.FINDRR.bit.INITAIL = 1;
    }

    if(AutoTuneCtrlRegs.CountDelayAutoTune < 20000 )
    {
        Volt_Vq = 0;
    }
    else
    {
        if(AutoTuneCtrlRegs.CountDelayAutoTune >= 20000 && AutoTuneCtrlRegs.CountDelayAutoTune < 70000)
        {
            Volt_Vq = _IQ17div(_IQ17(24),MainIQ_Variable.V_base);
            omega = 327;

            AbsoluteTransCurrent();

            //=========== Star Type ===============

//          AutoTuneCtrlRegs.TEMP1 = _IQ17div((AutoTuneCtrlRegs.Is - AutoTuneCtrlRegs.I_old),_IQ17(628.318));//di/dt  = cosu du/dt when u = 2wt
            AutoTuneCtrlRegs.TEMP1 = (AutoTuneCtrlRegs.Is - AutoTuneCtrlRegs.I_old);//di/dt
            if(AutoTuneCtrlRegs.TEMP1 < 0)
                AutoTuneCtrlRegs.TEMP1 = -AutoTuneCtrlRegs.TEMP1;//=====> Absolute Is
            AutoTuneCtrlRegs.TEMP1 = _IQ18mpy(_IQ17(AutoTuneCtrlRegs.SigmaLs),AutoTuneCtrlRegs.TEMP1);  //di/dt * 2 * SimaLs
            AutoTuneCtrlRegs.TEMP1 = _IQ17mpy(AutoTuneCtrlRegs.TEMP1,MainIQ_Variable.I_base);           //di/dt * 2 * SimaLs * Ibase

            AutoTuneCtrlRegs.TEMP2 = _IQ17mpy(AutoTuneCtrlRegs.Is,MainIQ_Variable.I_base);              //Is*Ibase
            AutoTuneCtrlRegs.TEMP2 = _IQ17mpy(_IQ18(AutoTuneCtrlRegs.Rs),AutoTuneCtrlRegs.TEMP2);       //2*Rs*Is*Ibase if star

            AutoTuneCtrlRegs.TEMP3 = (_IQ17mpy(Volt_Vq,MainIQ_Variable.V_base) - AutoTuneCtrlRegs.TEMP1 - AutoTuneCtrlRegs.TEMP2);      //Vs - SigmaLs - RsIs
            AutoTuneCtrlRegs.TEMP2 =  _IQ17mpy(AutoTuneCtrlRegs.Is,MainIQ_Variable.I_base);             //Is
            AutoTuneCtrlRegs.TEMP1 = _IQ17div(AutoTuneCtrlRegs.TEMP3,AutoTuneCtrlRegs.TEMP2);           //(Vs - 2SigmaLs - 2RsIs)/Is

            AutoTuneCtrlRegs.Rr = AutoTuneCtrlRegs.Rr + _IQ17toF(AutoTuneCtrlRegs.TEMP1);

            AutoTuneCtrlRegs.I_old = AutoTuneCtrlRegs.Is;

            //=========== Star Type ===============

        }
        else
        {
            if(AutoTuneCtrlRegs.CountDelayAutoTune >= 70000)
            {
                AutoTuneCtrlRegs.FINDRR.bit.FindRrStep = 1;
                AutoTuneCtrlRegs.Rr = AutoTuneCtrlRegs.Rr / 50000;
                AutoTuneCtrlRegs.Rr = AutoTuneCtrlRegs.Rr / 3;
            }
        }
    }

    AutoTuneCtrlRegs.CountDelayAutoTune++;

}

//////////////////////////////////////////////////////////////////////////////////////////
void Find_RotorTimeConstant(void)
{
    switch(AutoTuneCtrlRegs.FINDROTORTIMECONSTANT.bit.FINDROTORTIMECONSTANTStep)
    {
        case 0:
            if(AutoTuneCtrlRegs.CountDelayAutoTune >= 50000 )//t = 10 s
            {
                AutoTuneCtrlRegs.CountDelayAutoTune                 = 0;
                AutoTuneCtrlRegs.FINDROTORTIMECONSTANT.bit.INITAIL  = 1;

                AutoTuneCtrlRegs.FINDROTORTIMECONSTANT.bit.FINDROTORTIMECONSTANTStep = 1;
            }
            AutoTuneCtrlRegs.CountDelayAutoTune++;

            AutoTuneCtrlRegs.IntregratePI_Is = 0;
            AutoTuneCtrlRegs.Isref = _IQ17(0);
            break;
        case 1:
            AutoTuneCtrlRegs.Isref = _IQ17(0.013);

            if(AutoTuneCtrlRegs.CountDelayAutoTune >= 50000 )//t = 10 s
            {
                AutoTuneCtrlRegs.CountDelayAutoTune = 0;

                AutoTuneCtrlRegs.FINDROTORTIMECONSTANT.bit.FINDROTORTIMECONSTANTStep = 2;
            }
            AutoTuneCtrlRegs.CountDelayAutoTune++;

            break;
        case 2://Change phase
//          AutoTuneCtrlRegs.Isref = _IQ17(-0.02);
            if(AutoTuneCtrlRegs.CountDelayAutoTune >= 50000 )//t = 10 s
            {
                AutoTuneCtrlRegs.CountDelayAutoTune = 0;

                AutoTuneCtrlRegs.FINDROTORTIMECONSTANT.bit.FINDROTORTIMECONSTANTStep = 3;
            }
            AutoTuneCtrlRegs.CountDelayAutoTune++;
            break;
    }

    if(AutoTuneCtrlRegs.FINDROTORTIMECONSTANT.bit.FINDROTORTIMECONSTANTStep <= 2)
    {
        //------- Current Control ------------
        Kp = _IQ17(0.1);
        Ti = _IQ17(0.1);
        Ki = _IQ17div(6711,Ti);//_IQ17(0.2);//Ki = Ts/Ti

        Max = _IQ17div(_IQ17(40),MainIQ_Variable.V_base);
        Min = _IQ17div(_IQ17(-40),MainIQ_Variable.V_base);


        Err = AutoTuneCtrlRegs.Isref - IV_Read_reg.I_rms;//Q17
        intregrate = AutoTuneCtrlRegs.IntregratePI_Is;
        PI_Antiwindup();
        AutoTuneCtrlRegs.IntregratePI_Is = intregrate;

        Volt_Vq = PI_out ;
        //------- Current Control ------------
    }
    else
        Volt_Vq = 0 ;


}
//////////////////////////////////////////////////////////////////////////////////////////


void Find_Ls(void)
{
    if(AutoTuneCtrlRegs.FINDLS.bit.FindLsStep == 0)
    {
        if(AutoTuneCtrlRegs.CountDelayAutoTune >= 5000)//t = 1 s
        {
            AutoTuneCtrlRegs.CountDelayAutoTune     = 0;
            AutoTuneCtrlRegs.FINDLS.bit.FindLsStep  = 1;
        }
        AutoTuneCtrlRegs.CountDelayAutoTune++;
    }

    switch(AutoTuneCtrlRegs.FINDLS.bit.FindLsStep)
    {
        case 1:
            if(AutoTuneCtrlRegs.FINDLS.bit.INITAILFLUX1 == 0)
            {
                AutoTuneCtrlRegs.IntregrateFlux     = 0;
                AutoTuneCtrlRegs.FLUX1              = 0;
                AutoTuneCtrlRegs.CountDelayAutoTune = 0;

                AutoTuneCtrlRegs.FINDLS.bit.INITAILFLUX1 = 1;
            }
            Volt_Vq = _IQ17div(_IQ17(12),MainIQ_Variable.V_base);

            AbsoluteTransCurrent();

            AutoTuneCtrlRegs.TEMP1 = _IQ17mpy(_IQ17(AutoTuneCtrlRegs.Rs),AutoTuneCtrlRegs.Is);
            if(grp_num[5].value[32] == 1)//F533 = 1 is Y Type
                AutoTuneCtrlRegs.TEMP1 = AutoTuneCtrlRegs.TEMP1<<1;//2Rs*Is

            AutoTuneCtrlRegs.TEMP2 = _IQ17mpy(AutoTuneCtrlRegs.TEMP1,MainIQ_Variable.I_base);
            AutoTuneCtrlRegs.TEMP3 = _IQ17div(AutoTuneCtrlRegs.TEMP2,MainIQ_Variable.V_base);

            AutoTuneCtrlRegs.TEMP1 = _IQ17mpy(Volt_Vq - AutoTuneCtrlRegs.TEMP3,MainIQ_Variable.V_base) - _IQ17(2.5);

            if(AutoTuneCtrlRegs.FINDLS.bit.Count100ms >= 5)
            {
                AutoTuneCtrlRegs.IntregrateFlux = _IQ17mpy(AutoTuneCtrlRegs.TEMP1,_IQ17(0.001)) + AutoTuneCtrlRegs.IntregrateFlux;
                AutoTuneCtrlRegs.FINDLS.bit.Count100ms = 0;
            }
            AutoTuneCtrlRegs.FINDLS.bit.Count100ms++;

            if(AutoTuneCtrlRegs.CountDelayAutoTune >= 10000)//t = 2 s
            {
                AutoTuneCtrlRegs.CountDelayAutoTune     = 0;
                AutoTuneCtrlRegs.FINDLS.bit.FindLsStep  = 2;
            }
            AutoTuneCtrlRegs.CountDelayAutoTune++;

            break;
        case 2:
            Volt_Vq = 0;

            AutoTuneCtrlRegs.TEMP1 = _IQ17mpy(AutoTuneCtrlRegs.Is,MainIQ_Variable.I_base);
            AutoTuneCtrlRegs.TEMP2 = _IQ17div(AutoTuneCtrlRegs.IntregrateFlux,AutoTuneCtrlRegs.TEMP1);
            AutoTuneCtrlRegs.Ls    = _IQ17toF(AutoTuneCtrlRegs.TEMP2);

            AutoTuneCtrlRegs.FINDLS.bit.FindLsStep = 3;
            break;
    }




}

//////////////////////////////////////////////////////////////////////////////////////////

void AbsoluteTransCurrent(void)
{
    //---------------Absolute Transformation------------------
    AutoTuneCtrlRegs.Ix_alpha = _IQ17mpy(IV_Read_reg.PU_IU,IQ17_1)  +
                                 _IQ17mpy(IV_Read_reg.PU_IV,_IQ17(-0.5))        +
                                    _IQ17mpy(IV_Read_reg.PU_IW,_IQ17(-0.5))     ;

    AutoTuneCtrlRegs.Ix_beta =    _IQ17mpy(IV_Read_reg.PU_IU,_IQ17(0))    +
                                   _IQ17mpy(IV_Read_reg.PU_IV,_IQ17(0.8660254038))  +
                                   _IQ17mpy(IV_Read_reg.PU_IW,_IQ17(-0.8660254038)) ;

    AutoTuneCtrlRegs.Ix_alpha = _IQ17mpy(_IQ17(0.66666666667),AutoTuneCtrlRegs.Ix_alpha);// 2/3 = 0.66666666667
    AutoTuneCtrlRegs.Ix_beta  = _IQ17mpy(_IQ17(0.66666666667),AutoTuneCtrlRegs.Ix_beta);// 2/3 = 0.66666666667
    //---------------Absolute Transformation------------------

    AutoTuneCtrlRegs.Is = _IQ17mag(AutoTuneCtrlRegs.Ix_alpha,AutoTuneCtrlRegs.Ix_beta);
}

void Find_SigmaLs(void)
{
    //----------------Delay 10 sec ----------------------
    if(AutoTuneCtrlRegs.FINDSIGMALS.bit.INITAIL == 0)
    {
        if(AutoTuneCtrlRegs.CountDelayAutoTune >= 50000 )//t = 10 s
        {
            AutoTuneCtrlRegs.CountDelayAutoTune = 0;
            AutoTuneCtrlRegs.FINDSIGMALS.bit.INITAIL = 1;
        }
        AutoTuneCtrlRegs.CountDelayAutoTune++;
    }
    //----------------Delay 10 sec ----------------------
    else
    {
        omega = 0;

        switch(AutoTuneCtrlRegs.FINDSIGMALS.bit.FINDSIGMALSStep)
        {
            case 0:
                Volt_Vq = AutoTuneCtrlRegs.FindSigmaLsInitVol;
                if(AutoTuneCtrlRegs.CountDelayAutoTune >= (20000) )//t = 1 s
                {
                    AutoTuneCtrlRegs.CountDelayAutoTune = 0;
                    AutoTuneCtrlRegs.FINDSIGMALS.bit.FINDSIGMALSStep = 1;
                }

                AutoTuneCtrlRegs.CountDelayAutoTune++;

                //                   _
                //              1   |
                //  sigmaLs =  ---  | (us - Rs*is)  dt
                //              is _|

                //---------------Absolute Transformation------------------
                AutoTuneCtrlRegs.Ix_alpha = _IQ17mpy(IV_Read_reg.PU_IU,IQ17_1)+
                                       _IQ17mpy(IV_Read_reg.PU_IV,_IQ17(-0.5))+
                                       _IQ17mpy(IV_Read_reg.PU_IW,_IQ17(-0.5));

                AutoTuneCtrlRegs.Ix_beta =    _IQ17mpy(IV_Read_reg.PU_IU,_IQ17(0))+
                                       _IQ17mpy(IV_Read_reg.PU_IV,_IQ17(0.8660254038))+
                                       _IQ17mpy(IV_Read_reg.PU_IW,_IQ17(-0.8660254038));

                AutoTuneCtrlRegs.Ix_alpha = _IQ17mpy(_IQ17(0.66666666667),AutoTuneCtrlRegs.Ix_alpha);// 2/3 = 0.66666666667
                AutoTuneCtrlRegs.Ix_beta = _IQ17mpy(_IQ17(0.66666666667),AutoTuneCtrlRegs.Ix_beta);// 2/3 = 0.66666666667
                  //---------------Absolute Transformation------------------

                AutoTuneCtrlRegs.Delta_I = AutoTuneCtrlRegs.Ix_alpha + AutoTuneCtrlRegs.Ix_beta;

                if(AutoTuneCtrlRegs.Delta_I<0)
                    AutoTuneCtrlRegs.Delta_I = -AutoTuneCtrlRegs.Delta_I;

//              AutoTuneCtrlRegs.TEMP4 =    _IQ17toF(Volt_Vq -
//                                              _IQ17div(
//                                                  _IQ17mpy(
//                                                      _IQ17mpy(_IQ17(AutoTuneCtrlRegs.Rs)
//                                                          ,AutoTuneCtrlRegs.Delta_I)
//                                                              ,MainIQ_Variable.I_base)
//                                                                  ,MainIQ_Variable.V_base)
//                                                                      );

                AutoTuneCtrlRegs.TEMP4 =    _IQ17toF(_IQ17div(_IQ17mpy(_IQ17mpy(_IQ17(AutoTuneCtrlRegs.Rs)
                                                                                ,AutoTuneCtrlRegs.Delta_I)
                                                                       ,MainIQ_Variable.I_base)
                                                              ,MainIQ_Variable.V_base));

                AutoTuneCtrlRegs.TEMP1 = _IQ17mpy(_IQ17(AutoTuneCtrlRegs.TEMP4),IQ17_1);//1.224744871 = sqrt(3/2)

                if(grp_num[5].value[32] == 1)//F533 = 1 is Y Type
                    AutoTuneCtrlRegs.TEMP2 = _IQ17mpy(AutoTuneCtrlRegs.TEMP1,_IQ17(2));
                else
                    AutoTuneCtrlRegs.TEMP2 = AutoTuneCtrlRegs.TEMP1;

                if(Volt_Vq>=AutoTuneCtrlRegs.TEMP2)
                    AutoTuneCtrlRegs.TEMP5 = _IQ17toF(Volt_Vq - AutoTuneCtrlRegs.TEMP2);
                else
                    AutoTuneCtrlRegs.TEMP5 = 0 ;

                if(IntregrateSigmaLsCount>=5)//Intregrate 100 ms
                {
                IntregrateSigmaLsCount=0;
                AutoTuneCtrlRegs.IntregrateSigmaLs = AutoTuneCtrlRegs.IntregrateSigmaLs +
                        _IQ17toF(_IQ17mpy(_IQ17(0.001),_IQ17(AutoTuneCtrlRegs.TEMP5)));//Q17

                AutoTuneCtrlRegs.SigmaLs = _IQ17toF(_IQ17mpy(_IQ17div(MainIQ_Variable.V_base,MainIQ_Variable.I_base)
                                                             ,_IQ17(AutoTuneCtrlRegs.IntregrateSigmaLs)));//Q17

                }//Intregrate 100 ms
                IntregrateSigmaLsCount++;





                break;
            case 1:
                    Volt_Vq = 0;
                    AutoTuneCtrlRegs.FINDSIGMALS.bit.FINDSIGMALSStep = 2;
                break;


        }
    }


}
//////////////////////////////////////////////////////////////////////////////////////////


void Find_Rs(void)
{
    long temp_RS1,temp_RS2;

   if(AutoTuneCtrlRegs.FINDRS.bit.INITAIL == 0)
   {
      ResetAutoTune();

      AutoTuneCtrlRegs.FINDRS.bit.INITAIL = 1;
   }

//=========== New Finding Rs   2023/02/11 ================
   //++++++++++++++++++++++++ Description ++++++++++++++++++++++++
   //AutoTuneCtrlRegs.CountDelayAutoTune  --> CountDelayAutoTune
   //AutoTuneCtrlRegs.FLAGALLPARA.bit.AllParaStep = FindAutoTuneEnd;  ---> Used when it is the end
   //           Vsq_ref = Volt_Vq>>5;// Q17 / Q5 = Q12
   //           Vsd_ref = Volt_Vd>>5;
   //           Theta_Q15 = (Theta_Q15 + omega) & 0x7FFF;
   //Vsd_ref,Vsq_ref,Theta_Q15 For controlling the voltages and frequency of motor.
   //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

   //*******Step 1: Read Irate, ISrms, VSrms**********
       //CurrentMotor->I_Motor is Irate motor in F103 (Q17 and not PU)

//    IratePU = _IQ17div(CurrentMotor->I_Motor,MainIQ_Variable.I_base);
    Irate = _IQ17mpy(CurrentMotor->I_Motor,43691);//  2/3 Q17 = 87381

    temp_RS1  = _IQ17mpy(IV_Read_reg.PU_IU,131072) - _IQ17mpy(IV_Read_reg.PU_IV,65536) - _IQ17mpy(IV_Read_reg.PU_IW,65536);//1* 2^17 = 131072    0.5* 2^17 = 65536
    iSalpha   = _IQ17mpy(temp_RS1,107020);//sqrt(2/3)* 2^17 = 107020
    temp_RS1  = _IQ17mpy(IV_Read_reg.PU_IV,113512) - _IQ17mpy(IV_Read_reg.PU_IW,113512) ;//sqrt(3)/2 * 2^17 = 113512
    iSbeta    = _IQ17mpy(temp_RS1,107020);//sqrt(2/3)* 2^17 = 107020
    iSd       = _IQ15mpy(iSalpha,cos_Theta_o) + _IQ15mpy(iSbeta,sin_Theta_o);
    iSq       = -_IQ15mpy(iSalpha,sin_Theta_o) + _IQ15mpy(iSbeta,cos_Theta_o);
    temp_RS1  = _IQ17mag(iSd, iSq);
    temp_RS2  = _IQ17mpy(MainIQ_Variable.I_base,temp_RS1);
    Isrms     = _IQ17mpy(temp_RS2,75674);//1/sqrt(3) Q17 = 75674
    Isrms_LPF = _IQ17mpy(Isrms,2000) + _IQ17mpy(Isrms_LPF,131072 - 2000);

    temp_RS1 = _IQ12mag(Vsd_ref, Vsq_ref);
    temp_RS2 = _IQ12mpy(MainIQ_Variable.V_base,temp_RS1);
    VsLN     = _IQ17mpy(temp_RS2,75674);//1/sqrt(3) Q17 = 75674

   //********Step 2: Find voltage at Irate********
    if(Flag_VSearch == 0)
     {
         if(Isrms_LPF < Irate)
             Vsearch = Vsearch + 200;
         else
             Flag_VSearch = 1;

         if(Vsearch > _IQ17(30))
             Vsearch = _IQ17(30);

//         Volt_Vq = _IQ17div(_IQ17(Vsearch),MainIQ_Variable.V_base);
         Volt_Vq = _IQ17div(Vsearch,MainIQ_Variable.V_base);
         Vsq_ref = Volt_Vq>>5;// Q17 / Q5 = Q12
         Vsd_ref = 0;

         Theta_Q15 = 0;
     }


   //******** Step 3: Start searching Rs at 0, 120, 240 degrees at Irate ********
     if(Flag_VSearch == 1)
     {
         if(AutoTuneCtrlRegs.CountDelayAutoTune >= 0 && AutoTuneCtrlRegs.CountDelayAutoTune < 10000)
         {
             Volt_Vq = _IQ17div(Vsearch,MainIQ_Variable.V_base);//Volt_Vq = _IQ17div(_IQ17(Vsearch),MainIQ_Variable.V_base);
             Vsq_ref = Volt_Vq>>5;// Q17 / Q5 = Q12
             Vsd_ref = 0;

             Theta_Q15 = 0;// Find Rs @ 0 degree

             RSu = _IQ16div(VsLN,Isrms_LPF);// VsLN/Isrms_LPF/2
         }
         else if(AutoTuneCtrlRegs.CountDelayAutoTune >= 10000 && AutoTuneCtrlRegs.CountDelayAutoTune < 15000)
         {
             Volt_Vq = 0;
             Vsq_ref = Volt_Vq>>5;// Q17 / Q5 = Q12
             Vsd_ref = 0;

             Theta_Q15 = 10923;// Find Rs @ 0 degree
         }
         else if(AutoTuneCtrlRegs.CountDelayAutoTune >= 15000 && AutoTuneCtrlRegs.CountDelayAutoTune < 25000)
         {
             Volt_Vq = _IQ17div(Vsearch,MainIQ_Variable.V_base);//Volt_Vq = _IQ17div(_IQ17(Vsearch),MainIQ_Variable.V_base);
             Vsq_ref = Volt_Vq>>5;// Q17 / Q5 = Q12
             Vsd_ref = 0;

             Theta_Q15 = 10923;// Find Rs @ 120 degree

             RSv = _IQ16div(VsLN,Isrms_LPF);// VsLN/Isrms_LPF/2
         }
         else if(AutoTuneCtrlRegs.CountDelayAutoTune >= 25000 && AutoTuneCtrlRegs.CountDelayAutoTune < 30000)
         {
             Volt_Vq = 0;
             Vsq_ref = Volt_Vq>>5;// Q17 / Q5 = Q12
             Vsd_ref = 0;

             Theta_Q15 = 21845;// Find Rs @ 120 degree
         }
         else if(AutoTuneCtrlRegs.CountDelayAutoTune >= 30000 && AutoTuneCtrlRegs.CountDelayAutoTune < 40000)
         {
             Volt_Vq = _IQ17div(Vsearch,MainIQ_Variable.V_base);//Volt_Vq = _IQ17div(_IQ17(Vsearch),MainIQ_Variable.V_base);
             Vsq_ref = Volt_Vq>>5;// Q17 / Q5 = Q12
             Vsd_ref = 0;

             Theta_Q15 = 21845;// Find Rs @ 240 degree

             RSw = _IQ16div(VsLN,Isrms_LPF);// VsLN/Isrms_LPF/2
         }
         else if(AutoTuneCtrlRegs.CountDelayAutoTune >= 40000)   //Step 4: End
         {
             Vsq_ref   = 0;// Q17 / Q5 = Q12
             Vsd_ref   = 0;
             Theta_Q15 = 0;

             RS = _IQ17mpy((RSu+RSv+RSw), 43691);

             //CurrentMotor->Rs = RS;
             //AutoTuneCtrlRegs.FLAGALLPARA.bit.AllParaStep = FindAutoTuneEnd;
             AutoTuneCtrlRegs.FLAGALLPARA.bit.AllParaStep = FindRsRr_LsLr;
         }

         AutoTuneCtrlRegs.CountDelayAutoTune++;
     }
//========= End New Finding Rs   2023/02/11 ==============




/******************************########################################
   //+++++++++++++++++++++++++++++++++++++++++
    //+++++++++ Find Ls+Lr and Rs+Rr +++++
    Free1 = _IQ12mpy(va,131072) - _IQ12mpy(vb,65536) - _IQ12mpy(vc,65536);//1* 2^17 = 131072    0.5* 2^17 = 65536
    vSalpha = _IQ17mpy(Free1,107020);//sqrt(2/3)* 2^17 = 107020
    Free1 = _IQ12mpy(vb,113512) - _IQ12mpy(vc,113512) ;//sqrt(3)/2 * 2^17 = 113512
    vSbeta = _IQ17mpy(Free1,107020);//sqrt(2/3)* 2^17 = 107020

    vSd = _IQ15mpy(vSalpha,cos_Theta_o) + _IQ15mpy(vSbeta,sin_Theta_o);
    vSq = - _IQ15mpy(vSalpha,sin_Theta_o) + _IQ15mpy(vSbeta,cos_Theta_o);


    Free1 = _IQ17mpy(IV_Read_reg.PU_IU,131072) - _IQ17mpy(IV_Read_reg.PU_IV,65536) - _IQ17mpy(IV_Read_reg.PU_IW,65536);//1* 2^17 = 131072    0.5* 2^17 = 65536
    iSalpha = _IQ17mpy(Free1,107020);//sqrt(2/3)* 2^17 = 107020
    Free1 = _IQ17mpy(IV_Read_reg.PU_IV,113512) - _IQ17mpy(IV_Read_reg.PU_IW,113512) ;//sqrt(3)/2 * 2^17 = 113512
    iSbeta = _IQ17mpy(Free1,107020);//sqrt(2/3)* 2^17 = 107020

    iSd = _IQ15mpy(iSalpha,cos_Theta_o) + _IQ15mpy(iSbeta,sin_Theta_o);
    iSq = - _IQ15mpy(iSalpha,sin_Theta_o) + _IQ15mpy(iSbeta,cos_Theta_o);

//    Free1 = _IQ17mpy(vSalpha,iSalpha) + _IQ17mpy(vSbeta,iSbeta);
    Free1 = _IQ17mpy(vSd,iSd) + _IQ17mpy(vSq,iSq);
    Free2 = _IQ17mpy(Free1,MainIQ_Variable.I_base);
    pS = _IQ17mpy(Free2,MainIQ_Variable.V_base);

//    Free1 = - _IQ17mpy(vSbeta,iSalpha) + _IQ17mpy(vSalpha,iSbeta);
    Free1 = - _IQ17mpy(vSq,iSd) + _IQ17mpy(vSd,iSq);
    Free2 = _IQ17mpy(Free1,MainIQ_Variable.I_base);
    qS = _IQ17mpy(Free2,MainIQ_Variable.V_base);

    pS_LPF = _IQ17mpy(pS,600) + _IQ17mpy(pS_LPF,131072 - 600);
    qS_LPF = _IQ17mpy(qS,600) + _IQ17mpy(qS_LPF,131072 - 600);


   Free1 = _IQ12mpy(va,IV_Read_reg.PU_IU) + _IQ12mpy(vb,IV_Read_reg.PU_IV) + _IQ12mpy(vc,IV_Read_reg.PU_IW);
   Free2 = _IQ17mpy(Free1,MainIQ_Variable.I_base);
   ActivePower = _IQ17mpy(Free2,MainIQ_Variable.V_base);

//----  Vsd_ref and Vsq_ref are RMS Line-to-Line.
//----  IV_Read_reg.Isd and IV_Read_reg.Isq are RMS Line-to-Nuetrol.
//----  Power = Vector VS*IS; VS and IS have to be RMS Line-to-Line.
//----  So,  IV_Read_reg.Isd and IV_Read_reg.Isq have to be multiplied with sqrt(3).


   Free1 = _IQ12mag(Vsd_ref, Vsq_ref);
   Free2 = _IQ12mpy(MainIQ_Variable.V_base,Free1);
   VsLN  = _IQ17mpy(Free2,75674);//1/sqrt(3) Q17 = 75674

//   Free1 = _IQ17mag(iSalpha, iSbeta);
   Free1 = _IQ17mag(iSd, iSq);
   Free2 = _IQ17mpy(MainIQ_Variable.I_base,Free1);
   Isrms = _IQ17mpy(Free2,75674);//1/sqrt(3) Q17 = 75674

   Isrms_LPF = _IQ17mpy(Isrms,2000) + _IQ17mpy(Isrms_LPF,131072 - 2000);

   //------ Find cos_theta_autotune --------
   Free1 = _IQ17mpy(VsLN,Isrms_LPF);    //VsLN*Isrms * 2^17
   Free2 = _IQ17mpy(Free1,393216);  //3*VsLN*Isrms * 2^17
   Free3 = _IQ17div(pS_LPF,Free2);  //pS_LPF/(3*VsLN*Isrms) * 2^17
   cos_theta_autotune = ((float)Free3/131072);//2^17 = 131072,   3*2^17 = 393216

   //------ Find sin_theta_autotune --------
   theta_autotune = acos(cos_theta_autotune);
   sin_theta_autotune = sin(theta_autotune);

   //------ Find Impedance ---------
   Z = _IQ17div(VsLN,Isrms_LPF);

   //------ Find Lr, Rr ---------
   Free1 = _IQ18mpy(Z,_IQ17(sin_theta_autotune));
   LsLr_autotune =_IQ17mpy(Free1,417);//Q17/(2*pi*50) = 417
   RsRr_autotune = _IQ18mpy(Z,_IQ17(cos_theta_autotune));    //z cos_theta  /2
   //+++++++++ Find Ls+Lr and Rs+Rr +++++
   //+++++++++++++++++++++++++++++++++++++++++

 ###############################***************************************/
}

void Find_RsRr_LsLr(void)
{
    long temp_RsRr_LsLr1,temp_RsRr_LsLr2, Ksetpoint;

    //if(AutoTuneCtrlRegs.FINDRS.bit.INITAIL == 0)
    if(AutoTuneCtrlRegs.FINDRR.bit.INITAIL == 0)
    {
//        ResetAutoTune();

        AutoTuneCtrlRegs.CountDelayAutoTune = 0;

        AutoTuneCtrlRegs.FINDRR.bit.INITAIL = 1;
    }
    AutoTuneCtrlRegs.CountDelayAutoTune++;


//=========== Final ==============
 //       omega = 326;
 //       Vsd_ref = 0.380 Q17
 //       Vsq_ref = 0;

    Irate = _IQ17mpy(CurrentMotor->I_Motor,43691);//  2/3 Q17 = 87381    ,  43691 = 1/3 Q17

        Ksetpoint = _IQ14mpy(16384, AutoTuneCtrlRegs.CountDelayAutoTune);
        Ksetpoint = Ksetpoint<<3;//Q17
        if(Ksetpoint >= 131072)
            Ksetpoint = 131072;


//        temp_RsRr_LsLr1 = _IQ17div(CurrentMotor->V_Motor,MainIQ_Variable.V_base);
//        Vsearch = _IQ17mpy(temp_RsRr_LsLr1,13107);//1/10 = 13107, 1/6 = 21845, 1/5 Q17 = 26214

       if(AutoTuneCtrlRegs.CountDelayAutoTune <= 16384)
        {
            temp_RsRr_LsLr1 = _IQ17div(CurrentMotor->V_Motor,MainIQ_Variable.V_base);
            Vsearch         = _IQ17mpy(temp_RsRr_LsLr1,13107);//1/10 = 13107, 1/6 = 21845, 1/5 Q17 = 26214
        }
        else//adjust Vsearch for making ISN nearly Irate
        {

            if(Isrms_LPF <= Irate)
            {
                Vsearch = Vsearch + 1;


                temp_RsRr_LsLr1 = _IQ17div(CurrentMotor->V_Motor,MainIQ_Variable.V_base);
                temp_RsRr_LsLr2 = _IQ17mpy(temp_RsRr_LsLr1,13107) + _IQ17(0.01);//Vsearch > Vrate/10 + 10 V

                if(Vsearch > temp_RsRr_LsLr2)//Vsearch > Vrate/10 + 10 V
                {
                    Vsearch = temp_RsRr_LsLr2;//Vsearch = Vrate/10 + 10 V
                }
            }
        }

        Volt_Vq = _IQ17mpy(Vsearch,Ksetpoint);
        Volt_Vd = 0;

        omega = _IQ17mpy(326,Ksetpoint);

    //Step 1: Find Active Power
    temp_RsRr_LsLr1 = _IQ12mpy(va,131072) - _IQ12mpy(vb,65536) - _IQ12mpy(vc,65536);//1* 2^17 = 131072    0.5* 2^17 = 65536
    vSalpha         = _IQ17mpy(temp_RsRr_LsLr1,107020);//sqrt(2/3)* 2^17 = 107020
    temp_RsRr_LsLr1 = _IQ12mpy(vb,113512) - _IQ12mpy(vc,113512) ;//sqrt(3)/2 * 2^17 = 113512
    vSbeta          = _IQ17mpy(temp_RsRr_LsLr1,107020);//sqrt(2/3)* 2^17 = 107020

    vSd = _IQ15mpy(vSalpha,cos_Theta_o) + _IQ15mpy(vSbeta,sin_Theta_o);
    vSq = - _IQ15mpy(vSalpha,sin_Theta_o) + _IQ15mpy(vSbeta,cos_Theta_o);

    temp_RsRr_LsLr1 = _IQ17mpy(IV_Read_reg.PU_IU,131072) - _IQ17mpy(IV_Read_reg.PU_IV,65536) - _IQ17mpy(IV_Read_reg.PU_IW,65536);//1* 2^17 = 131072    0.5* 2^17 = 65536
    iSalpha         = _IQ17mpy(temp_RsRr_LsLr1,107020);//sqrt(2/3)* 2^17 = 107020
    temp_RsRr_LsLr1 = _IQ17mpy(IV_Read_reg.PU_IV,113512) - _IQ17mpy(IV_Read_reg.PU_IW,113512) ;//sqrt(3)/2 * 2^17 = 113512
    iSbeta          = _IQ17mpy(temp_RsRr_LsLr1,107020);//sqrt(2/3)* 2^17 = 107020

    iSd = _IQ15mpy(iSalpha,cos_Theta_o) + _IQ15mpy(iSbeta,sin_Theta_o);
    iSq = - _IQ15mpy(iSalpha,sin_Theta_o) + _IQ15mpy(iSbeta,cos_Theta_o);

    temp_RsRr_LsLr1 = _IQ17mpy(vSd,iSd) + _IQ17mpy(vSq,iSq);
    temp_RsRr_LsLr2 = _IQ17mpy(temp_RsRr_LsLr1,MainIQ_Variable.I_base);
    pS              = _IQ17mpy(temp_RsRr_LsLr2,MainIQ_Variable.V_base);
    if(pS<0)
        pS = -pS;

    temp_RsRr_LsLr1 = - _IQ17mpy(vSq,iSd) + _IQ17mpy(vSd,iSq);
    temp_RsRr_LsLr2 = _IQ17mpy(temp_RsRr_LsLr1,MainIQ_Variable.I_base);
    qS              = _IQ17mpy(temp_RsRr_LsLr2,MainIQ_Variable.V_base);
    if(qS<0)
        qS = -qS;

    pS_LPF = _IQ17mpy(pS,600) + _IQ17mpy(pS_LPF,131072 - 600);
    qS_LPF = _IQ17mpy(qS,600) + _IQ17mpy(qS_LPF,131072 - 600);


    //Step 2: Find VS
    temp_RsRr_LsLr1 = _IQ12mag(Vsd_ref, Vsq_ref);
    temp_RsRr_LsLr2 = _IQ12mpy(MainIQ_Variable.V_base,temp_RsRr_LsLr1);
    VsLN            = _IQ17mpy(temp_RsRr_LsLr2,75674);//1/sqrt(3) Q17 = 75674

    //Step 3: Find IS
    temp_RsRr_LsLr1 = _IQ17mag(iSd, iSq);
    temp_RsRr_LsLr2 = _IQ17mpy(MainIQ_Variable.I_base,temp_RsRr_LsLr1);
    Isrms           = _IQ17mpy(temp_RsRr_LsLr2,75674);//1/sqrt(3) Q17 = 75674

    Isrms_LPF = _IQ17mpy(Isrms,2000) + _IQ17mpy(Isrms_LPF,131072 - 2000);

    //Step 4: Find cos(theta) and theta and    sin(theta)
    temp_RsRr_LsLr1    = _IQ17mpy(VsLN,Isrms_LPF);    //VsLN*Isrms * 2^17
    temp_RsRr_LsLr2    = _IQ17mpy(temp_RsRr_LsLr1,393216);  //3*VsLN*Isrms * 2^17
    temp_RsRr_LsLr1    = _IQ17div(pS_LPF,temp_RsRr_LsLr2);  //pS_LPF/(3*VsLN*Isrms) * 2^17
    cos_theta_autotune = ((float)temp_RsRr_LsLr1/131072);//2^17 = 131072,   3*2^17 = 393216

    theta_autotune = acos(cos_theta_autotune);

    sin_theta_autotune = sin(theta_autotune);


    //Step 5: Find Z
    Z = _IQ17div(VsLN,Isrms_LPF);

    //Step 6: Find Rs+Rr
    RsRr = _IQ18mpy(Z,_IQ17(cos_theta_autotune));    //z cos_theta  /2

    //Step 7: Find Ls+Lr
    temp_RsRr_LsLr1 = _IQ18mpy(Z,_IQ17(sin_theta_autotune));  // Z*sin_theta / 2
    LsLr            =_IQ17mpy(temp_RsRr_LsLr1,417);//Q17/(2*pi*50) = 417



    if(AutoTuneCtrlRegs.CountDelayAutoTune >= 40000)
    {
        //Step 8: Find Rr = (Rs+Rr) - Rs
        RR = RsRr - RS;

        //Step 9: Find Ls = Lr = (Ls+Lr)/2
        LR = LsLr>>1;
        LS = LR;
    }


    if(AutoTuneCtrlRegs.CountDelayAutoTune >= 80000)
        AutoTuneCtrlRegs.FLAGALLPARA.bit.AllParaStep = FindAutoTuneEnd;
}


void Save_and_End_Autotune(void)
{
    Ls = _IQ17toF(LS);//AutoTuneCtrlRegs.Ls;
    Rr = _IQ17toF(RR);//AutoTuneCtrlRegs.Rr;
    Rs = _IQ17toF(RS);//AutoTuneCtrlRegs.Rs;
    Lm = 1.234;//AutoTuneCtrlRegs.Lm;
    SigmaLs = 0.888;//AutoTuneCtrlRegs.SigmaLs;

    AutoTuneFinish = 1;//Bring to Load Default in "KeyPad.c"

    ND2System_Reg.STATE_FLAG.bit.State = state_stoppingAT;//Reset Function
}

//////////////////////////////////////////////////////////////////////////////////////////
void ResetAutoTune(void)
{
//  AutoTuneFinish = 0;//Bring to Keypad.c

    AutoTuneCtrlRegs.FLAGALLPARA.bit.AllParaStep = 0;

    AutoTuneCtrlRegs.FINDRS.bit.FindRsStep = 0;
    AutoTuneCtrlRegs.CountDelayAutoTune    = 0;
    AutoTuneCtrlRegs.FINDRS.all            = 0;
    AutoTuneCtrlRegs.TEMP1                 = 0;

    AutoTuneCtrlRegs.Delta_V     = 0;
    AutoTuneCtrlRegs.Delta_I     = 0;
    AutoTuneCtrlRegs.Count_Delta = 0;

    AutoTuneCtrlRegs.sum_Rs = 0;
//  AutoTuneCtrlRegs.Rs = 0;

    AutoTuneCtrlRegs.FindRsInitVol = _IQ17div(_IQ17(12),MainIQ_Variable.V_base);
    AutoTuneCtrlRegs.FindRsVolStep = _IQ17div(IQ17_1,MainIQ_Variable.V_base);

    Volt_Vd = 0;
    Volt_Vq = 0;

    //--------------Sigma Ls-------------------
    AutoTuneCtrlRegs.FINDSIGMALS.bit.INITAIL         = 0;
    AutoTuneCtrlRegs.FINDSIGMALS.bit.FINDSIGMALSStep = 0;
    AutoTuneCtrlRegs.FindSigmaLsInitVol              = _IQ17div(_IQ17(30),MainIQ_Variable.V_base); //60
    AutoTuneCtrlRegs.IntregrateSigmaLs               = 0;
    IntregrateSigmaLsCount                           = 0;

    //----------------Ls-----------------------
    AutoTuneCtrlRegs.FINDLS.bit.FindLsStep   = 0;
    AutoTuneCtrlRegs.IntregrateFlux          = 0;
    AutoTuneCtrlRegs.FINDLS.bit.INITAILFLUX1 = 0;
    AutoTuneCtrlRegs.FLUX1                   = 0;



    //-------- Rotor Time Constant ------------
    AutoTuneCtrlRegs.FINDROTORTIMECONSTANT.bit.INITAIL = 0;
    AutoTuneCtrlRegs.IntregratePI_Is                   = 0;

    //----------------Rr-----------------------
    AutoTuneCtrlRegs.FINDRR.bit.FindRrStep = 0;
    AutoTuneCtrlRegs.FINDRR.bit.INITAIL    = 0;

    //----------------Lm-----------------------
    AutoTuneCtrlRegs.FINDLM.bit.FINDLmStep = 0;
    AutoTuneCtrlRegs.FINDLM.bit.INITAIL    = 0;

//============================
    AutoTuneCtrlRegs.Y_RLS    = 0;
    AutoTuneCtrlRegs.Xnew_RLS = 0;
    AutoTuneCtrlRegs.Er_RLS   = 0;

//=======================================
//======== New AutoTune 2023/02/11 ======
    Isrms_LPF    = 0;
    Vsearch      = 0;
    Flag_VSearch = 0;

}
//////////////////////////////////////////////////////////////////////////////////////////
void ResetAutoTune1(void)
{
    if(grp_num[5].value[31]==1)
    {
        grp_num[5].value[31] = 0;
        AutoTuneCtrlRegs.FLAGALLPARA.bit.EnableAT = 0;
        AutoTuneCtrlRegs.FLAGALLPARA.bit.ActiveAT = 0;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
