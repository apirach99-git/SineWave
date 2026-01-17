#include "F28x_Project.h"
#include "ND2.h"
#include "pid_reg3.h"

/*----------------Description---------------------------
10-20-55 : Vecter FOC Full Option

2013-03-16: Add SpeedFeedback from Read Encoder solfware new version 
2013-08-24: Add Speed and Torque control mode.
2013-12-07: Improve Theta of Rotor Flux
            Id_ref and Iq_ref command substitule Id and Iq feedback.
------------------Description---------------------------*/

struct VECTOR_REG Vecter_Reg = {0,0,0,0,0,
                                0,0,0,0,0,
                                0,0,0};

void Vecter_Control_FOC(void);
void PI_Controller_FOC(void);
void Rotor_Flux(void);


int omega_LPF1 = 0;
int omega_LPF2 = 0;
int i_d_old = 0;
int i_d_LPF = 0;
int i_q_LPF = 0;
int i_q_old = 0; 

extern int omega_slip; 
extern int omega_o; 
extern int Theta ;
extern int Theta_o;


extern int Vsd_ref;
extern int Vsq_ref;

extern int Vs_alpha;
extern int Vs_beta;
extern int va;
extern int vb;
extern int vc;

extern int kp_gain_PI;
extern int ki_gain_PI;
extern int error_PI;
extern int output_PI; 
extern int limit_min_PI ;
extern int limit_max_PI ;
extern int integral_PI;
extern long integral_PI_long;

extern int omega_LPF1;
extern int omega_LPF2;

extern int i_d_LPF;
extern int i_d_old;
extern int i_q_LPF;
extern int i_q_old; 

extern int isd;
extern int isq; 
extern int wc_RotorFlux1;
extern int wc_RotorFlux2;
extern int isd_est_old;
extern int imr ; 
extern int Speed_vector_Com;
extern int Speed_Feedback;
extern int isd_ref;
extern int isq_ref; 

long integral_PI_long_Speed = 0;
int integral_PI_Speed = 0;
long integral_PI_long_d = 0;
int integral_PI_d = 0;
long integral_PI_long_q = 0;
int integral_PI_q = 0; 
long integral_PI_long_speed = 0;
int integral_PI_speed = 0; 


struct data_grp{
   Uint16 value[100];
   Uint16 factor[100];
};
extern struct data_grp grp_num[8] ;    


extern int isd_LPF;
int count_200us = 0;

extern int softstart;


//extern int isa,isb,isc,isalpha,isbeta,speed_skp_cnt;
extern int isd_LPF ,isq_LPF ;
int count_100ms = 0;

long Rotor_Time_Cons = 0;
extern _iq17 Free1,Free2,Free3,Free4,Free5,Free6;
long Time_Cons1 = 0;
long Time_Cons2 = 0;
long Krotor = 0;

long Imr = 0;
long Id = 0;
long Iq = 0;

int count = 0; 
long w_slip = 0;
long w_m = 0;
long w_out = 0;

long Theta_out = 0;
long Theta_sum = 0;
int  M_Intr_w_out = 0;

extern long Kp,Ti,Ki,Max,Min,intregrate,PI_out,Err;
extern int F_Command;
//float Kpp = 0.05,Tii = 0.00009;
float Kpp = 0.008,Tii = 0.004;

long Isq_ref = 0;
long Isd_ref = 0;
long Max_w_out = 0;
long Min_w_out = 0; 


//=========== Parameters for Field weakening ===============
long Isd_ref_weaken = 0;
long Vmax_weaken;
long KpFW = 33100;//Q17 0.253
long KiFW_Ts = 262;//Q17  kiFW*Ts = 10*200e-6*2^17 = 262
long integral_Isd_ref_weaken = 0;
long Verror_FW = 0;
long Isd_weaken_Limit = 0;

long InstActivePower = 0;
//=================================================


/*; Description   : Parameter of motor
;                   Lr = 0.12
;                   Ls = 0.131
;                   M  = 0.12
;                   Rr = 0.885
;                   Rs = 1.84  //1.95
;                   p  = 4
;                   sigma = 1  -   M^2/(Lr*Ls) = 0.084
;                   J  = 0.021
;----------------------------------------------------------------------------------*/ 

//-----------------------------------------------------
//               LowPass Filter
//----------------------------------------------------- 
//           Ts                  Tc
//  y(k) = ------ * x(k)  +  ---------- * y(k-1)
//         Ts + Tc            Ts + Tc
//
//                  Ts
//  Time_Cons1 = --------   ,   Ts = 200us
//               Ts +  Tc
//
//                  Tc
//  Time_Cons1 = --------   ,   Ts = 200us  ,   Tc = Lr/Rr
//               Ts +  Tc
//-----------------------------------------------------  

int CountLPFImr = 0;

int increte = 2;//5

int count_1ms = 0;

Uint16 *v;
PIDREG3 pid1, pid2, speedloop, currentloop_d, currentloop_q;
PIDREG3 speedloop=PIDREG3_DEFAULTS;
PIDREG3 currentloop_d=PIDREG3_DEFAULTS;
PIDREG3 currentloop_q=PIDREG3_DEFAULTS;
PIDREG3 pid1=PIDREG3_DEFAULTS;
PIDREG3 pid2=PIDREG3_DEFAULTS;

void pid_reg3_calc(PIDREG3 *v)
{
//============================================
//>>>>>> Description of PID_reg3 type <<<<<<<<
//============================================
//  We can find the parameters of this PID_reg3 as follow:
//      Kp = Kp,
//      Ki = (Kp*Ts)/Ti,
//  and Kd = (Td*Kp)/Ts.
//============================================

    if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)//OFF PWM setting
    {
        v->Err = 0;
        v->Up = 0;
        v->Ui = 0;
        v->Ud = 0;
        v->OutPreSat = 0;
        v->SatErr = 0;
        v->OutPreSat = 0;
        v->Up1 = 0;
        v->Out = 0;
    }
    else
    {
        // Compute the error
        v->Err = v->Ref - v->Fdb;

        // Compute the proportional output
        v->Up = _IQmpy(v->Kp,v->Err);

        // Compute the integral output
        v->Ui = v->Ui + _IQmpy(v->Ki,v->Up) + _IQmpy(v->Kc,v->SatErr);

        // Compute the derivative output
        v->Ud = _IQmpy(v->Kd,(v->Up - v->Up1));

        // Compute the pre-saturated output
        v->OutPreSat = v->Up + v->Ui + v->Ud;

        // Saturate the output
        if (v->OutPreSat > v->OutMax)
          v->Out =  v->OutMax;
        else if (v->OutPreSat < v->OutMin)
          v->Out =  v->OutMin;
        else
          v->Out = v->OutPreSat;

        // Compute the saturate difference
        v->SatErr = v->Out - v->OutPreSat;

        // Update the previous proportional output
        v->Up1 = v->Up;
    }
}


//Torque parameter
long Isqrate = 0;
long Flimit = 0;
long Isq_com = 0;
//Torque parameter
void Rotor_Flux(void)
{
//=======================================================
//=================== Find Parameter ====================
//=======================================================
    if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)
    {
        Rotor_Time_Cons = _IQ17div(CurrentMotor->Lr,CurrentMotor->Rr);
        Krotor = _IQ17div(_IQ17(1),Rotor_Time_Cons);
        
        //0.001 = 1ms = Ts         //0.0002 = 200us = Ts
        Time_Cons1 = _IQ17div(_IQ25(0.0002),_IQ25(0.0002)+_IQ8(Rotor_Time_Cons));           //(Q25/(Q25+Q25))*Q17 = Q17
        Time_Cons2 = _IQ17div(_IQ8(Rotor_Time_Cons),_IQ25(0.0002)+_IQ8(Rotor_Time_Cons));   //(Q25/(Q25+Q25))*Q17 = Q17

        //(Q15/T_rate)*T_sampling = 328(Q15),T_rate = 20ms ,T_sampling = 200us      <--  5kHz
        M_Intr_w_out = _IQ17div(328,CurrentMotor->Poles_Motor);

        //===============Change Base===================
        Vecter_Reg.OmageToCurrent = _IQ17div(MainIQ_Variable.Omega_base,MainIQ_Variable.I_base);    //Q17
        Vecter_Reg.CurrentToVoltage = _IQ17div(MainIQ_Variable.I_base,MainIQ_Variable.V_base);      //Q17
        //===============Change Base===================

//          Isqrate = _IQ17div(_IQ17mpy(CurrentMotor->PF_Motor,CurrentMotor->I_Motor),MainIQ_Variable.I_base);
//          Isqrate = CurrentMotor->PF_Motor;
        Isqrate = _IQ17div(Vecter_Reg.Limit_Max_SpeedLoop,MainIQ_Variable.I_base);
    }
//=======================================================
//================= End Find Parameter ==================
//=======================================================


//=======================================================
//==================== Find Imr =========================
//=======================================================
//------- Id Q17 ---> Lowpass Filter (Tc = Lr/Rr) ----->  Imr
   
    Imr = _IQ17mpy(Time_Cons1,Id) + _IQ17mpy(Time_Cons2,Imr);//Q17 * Q22 / Q17 = Q17 //<<<<<<<<<<<<<_IQ12mpy(Time_Cons1,Isd_ref)

//  if(Imr <= _IQ17div(_IQ17(0.05),MainIQ_Variable.I_base) && grp_num[5].value[33] == 1)//Limit  if Imr < 1; Imr = 1A  and Torque Control
    if(Imr <= _IQ17div(_IQ17(0.05),MainIQ_Variable.I_base) && grp_num[5].value[33] >= 1)
       Imr = _IQ17div(_IQ17(0.05),MainIQ_Variable.I_base);

//-------------------Low pass Filter------------------->>>>>   Imr    Q17 
//=======================================================
//=================== End Find Imr ======================
//=======================================================

//=======================================================
//==================== Find wSlip =======================
//=======================================================
//-----------------------------------------------------
//  (iq*Rr/Lr) / (Imr * w_base * polepair)      ,   w_base = 2pi*50
//-----------------------------------------------------
    //Free1 = _IQ17mpy(Krotor,Isq_ref);//<<<<<<<<<<<<<_IQ17mpy(Krotor,Iq);
    Free1 = _IQ17mpy(Krotor,Iq);//<<<<<<<<<<<<<_IQ17mpy(Krotor,Iq);
//  Free1 = _IQ17mpy(Krotor,Isq_com);//<<<<<<<<<<<<<_IQ17mpy(Krotor,Iq);
    w_slip = _IQ17div(Free1,_IQ17mpy(Imr,MainIQ_Variable.Omega_base));//100pi = 314.1592654 **********************************          157.0796327

    Free1 = CurrentMotor->Poles_Motor;
    w_slip = _IQ17div(w_slip,Free1);//CurrentMotor->Poles_Motor / 2


/*
//   if(grp_num[5].value[33] == 1)//F534 = 1 is Torque Control
    if(grp_num[5].value[33] >= 1)
    {
   //========== Limit Omega Slip ============= 
        if(w_slip>=_IQ17(0.05))//0.05   //0.55
            w_slip = _IQ17(0.05);
        else if(w_slip<=_IQ17(-0.05))
            w_slip = _IQ17(-0.05);

    //=========================================
    }
*/
//----------------------------------------------------->>>>>    w_slip  Q17
//=======================================================
//================== End Find wSlip =====================
//=======================================================


//=======================================================
//==================== Find wout ========================
//=======================================================
    w_m = QEP_Reg.PU_RPM;

    w_out = _IQ17mpy(w_m,CurrentMotor->Poles_Motor) + w_slip;//-->>>>>  w_out   Q17 //CurrentMotor->Poles_Motor / 2

    Max_w_out = _IQ17mpy(_IQ17div(Vecter_Reg.Max_Freq_out + _IQ17(5),MainIQ_Variable.F_base),CurrentMotor->Poles_Motor);//compenaste 5 Hz
    Min_w_out = -Max_w_out;//_IQ17mpy(_IQ17div(Vecter_Reg.Min_Freq_out,MainIQ_Variable.F_base),CurrentMotor->Poles_Motor);

    //-------------- Limit w_ou -----------------
    if(w_out>=0)
    {
        if(w_out > Max_w_out)
            w_out = Max_w_out;
    }
    else//(w_out<0)
    {
        if(w_out < Min_w_out)
            w_out = Min_w_out;
    }
    //-------------- Limit w_ou -----------------

//=======================================================
//================ End Find wout ========================
//=======================================================

//=======================================================
//=============== Find Theta_out ========================
//=======================================================
//  w_out -------> intregrate ---------> Theta_out
    Theta_sum = _IQ17mpy(M_Intr_w_out,w_out);//((long)328/P*w_out)>>17;     M_Intr_w_out    410
    Theta_out = (Theta_out + Theta_sum) & 0x7FFF;//Maintain positive value of angle command
//----------------------------------------------------->>>>>    Theta_out Max Q15
//=======================================================
//============= End Find Theta_out ======================
//=======================================================

//=======================================================
//================= Find sin cos ========================
//=======================================================

//=======================================================
//=============== End Find sin cos ======================
//=======================================================

} 




void Vecter_Control_FOC(void)
{

    Id = IV_Read_reg.Isd;//_IQ17mpy(IV_Read_reg.Isd,_IQ17(1.732));
    Iq = IV_Read_reg.Isq;//_IQ17mpy(IV_Read_reg.Isq,_IQ17(1.732));

    Rotor_Flux();

//========================================================
//==================== Speed Loop ========================
//======================================================== 
/*  speedloop.Kp =  Vecter_Reg.Kn_SpeedLoop;
    speedloop.Ki =  Vecter_Reg.Tn_SpeedLoop;
    speedloop.Kd = _IQ(0.000);
    speedloop.Kc = _IQ17div(Vecter_Reg.Tn_SpeedLoop,Vecter_Reg.Kn_SpeedLoop);//_IQ(0.8);            Kc = Ki/Kp
    speedloop.OutMax = _IQ17div(Vecter_Reg.Limit_Max_SpeedLoop,MainIQ_Variable.I_base);
    speedloop.OutMin = _IQ17div(Vecter_Reg.Limit_Min_SpeedLoop,MainIQ_Variable.I_base);

    speedloop.Ref = MainIQ_Variable.FCommand;
    speedloop.Fdb = QEP_Reg.PU_RPM;
    speedloop.calc(&speedloop);

    PI_out = speedloop.Out;*/
//========================================================
//================== End Speed Loop ======================
//========================================================

/////////////Reverse direction command///////////////////
//  Free1 = TrqCmnd_Reg.TrqCommand;
//  TrqCmnd_Reg.TrqCommand = -Free1;


//------Select Speed Control or Torque Control------------
    if(grp_num[5].value[33] == 0)//F534 = 0 is Speed Control
    {
        speedloop.Kp =  Vecter_Reg.Kn_SpeedLoop;
        speedloop.Ki =  Vecter_Reg.Tn_SpeedLoop;
        speedloop.Kd = _IQ(0.000);
        speedloop.Kc = _IQ17div(Vecter_Reg.Tn_SpeedLoop,Vecter_Reg.Kn_SpeedLoop);//_IQ(0.8);            Kc = Ki/Kp
        speedloop.OutMax = _IQ17div(Vecter_Reg.Limit_Max_SpeedLoop,MainIQ_Variable.I_base);
        speedloop.OutMin = -speedloop.OutMax;//_IQ17div(Vecter_Reg.Limit_Min_SpeedLoop,MainIQ_Variable.I_base);

        speedloop.Ref = MainIQ_Variable.FCommand;
        speedloop.Fdb = QEP_Reg.PU_RPM;
        speedloop.calc(&speedloop);

        PI_out = speedloop.Out;

        Isq_ref = PI_out;
    }
    else//Torque Control
    {
        Free1 = _IQ17div(grp_num[3].value[7],5000);//F308 Limitation of the speed motor while using the torque control mode.
//      Free1 = _IQ17div(Vecter_Reg.Max_Freq_out,MainIQ_Variable.F_base);//F518
        Flimit = _IQ17abs(Free1);

/*
//====================================== Conventional ================================================

        if(_IQ17abs(QEP_Reg.PU_RPM) >= Flimit)
        {

            speedloop.Kp =  Vecter_Reg.Kn_SpeedLoop;
            speedloop.Ki =  Vecter_Reg.Tn_SpeedLoop;
            speedloop.Kd = _IQ(0.000);
            speedloop.Kc = _IQ17div(Vecter_Reg.Tn_SpeedLoop,Vecter_Reg.Kn_SpeedLoop);//_IQ(0.8);            Kc = Ki/Kp
            speedloop.OutMax = _IQ17abs(Isqrate);//_IQ17div(Vecter_Reg.Limit_Max_SpeedLoop,MainIQ_Variable.I_base);
            speedloop.OutMin = -speedloop.OutMax;//_IQ17div(Vecter_Reg.Limit_Min_SpeedLoop,MainIQ_Variable.I_base);

            if(grp_num[5].value[33] == 1)
            {
                if(QEP_Reg.PU_RPM >= 0)
                    speedloop.Ref = Flimit;
                else
                    speedloop.Ref = -Flimit;
            }
            else if(grp_num[5].value[33] == 2)
            {
                if(MainIQ_Variable.FCommand >= 0)
                    speedloop.Ref = Flimit;
                else
                    speedloop.Ref = -Flimit;
            }


            speedloop.Ref = Flimit;
            speedloop.Fdb = QEP_Reg.PU_RPM;
            speedloop.calc(&speedloop);

            PI_out = speedloop.Out;

            Isq_ref = PI_out;
        }
        else
        {

            //====== Calculation of Isd_ref can be shown following =====
            //    Isd_ref = sqrt(1 - PF^2)*Irate/Ibase;
            //    Isq_ref = limit +- (PF*Irate/Ibase)
            //===========================================================
            //Isq_com = _IQ17mpy(Isqrate,TrqCmnd_Reg.TrqCommand);

            //Isq_com = _IQ17mpy(Isqrate,MainIQ_Variable.FCommand);

            if(grp_num[5].value[33] == 1)
                Isq_com = _IQ17mpy(Isqrate,TrqCmnd_Reg.TrqCommand);
            else if(grp_num[5].value[33] == 2)
                Isq_com = _IQ17mpy(Isqrate,MainIQ_Variable.FCommand);

            if(Isq_com > Isqrate)
                Isq_ref = Isqrate;
            else if(Isq_ref < -Isqrate)
                Isq_ref = -Isqrate;
            else
                Isq_ref = Isq_com;
        }

//====================================== Conventional ================================================
*/

//========================  Minimal command selection ==================
        //-------- Speed Limitation and Iq_ref generated by PI speed Controller -------------
        speedloop.Kp =  Vecter_Reg.Kn_SpeedLoop;
        speedloop.Ki =  Vecter_Reg.Tn_SpeedLoop;
        speedloop.Kd = _IQ(0.000);
        speedloop.Kc = _IQ17div(Vecter_Reg.Tn_SpeedLoop,Vecter_Reg.Kn_SpeedLoop);//_IQ(0.8);            Kc = Ki/Kp
        speedloop.OutMax = _IQ17abs(Isqrate);//_IQ17div(Vecter_Reg.Limit_Max_SpeedLoop,MainIQ_Variable.I_base);
        speedloop.OutMin = -speedloop.OutMax;//_IQ17div(Vecter_Reg.Limit_Min_SpeedLoop,MainIQ_Variable.I_base);

        if(TrqCmnd_Reg.TrqCommand >= 0)
            speedloop.Ref = _IQ17div(Vecter_Reg.Max_Freq_out,MainIQ_Variable.F_base); //; Vecter_Reg.Max_Freq_out
        else
            speedloop.Ref = _IQ17div(-Vecter_Reg.Max_Freq_out,MainIQ_Variable.F_base); //; Vecter_Reg.Max_Freq_out

        speedloop.Fdb = QEP_Reg.PU_RPM;
        speedloop.calc(&speedloop);
        PI_out = speedloop.Out;

//        if(PI_out < 0)
//            Free2 = PI_out;// Free2 is equal to ABS(PI_out).


        //-------- Iq_ref created by TrqCmnd_Reg.TrqCommand -------------
        Isq_com = _IQ17mpy(Isqrate,TrqCmnd_Reg.TrqCommand);

//        if(Isq_com < 0)
//            Free3 = Isq_com;// Free2 is equal to ABS(Isq_com).

        if(TrqCmnd_Reg.TrqCommand >= 0)
        {
            //--------- Select Isq_ref from Speed Controller or %Torque ---------------
            if(PI_out < Isq_com)// if ABS(PI_out) < ABS(Isq_com), speed controller is enable.
                Isq_ref = PI_out;
            else
                Isq_ref = Isq_com;// if ABS(PI_out) > ABS(Isq_com), Iq_ref is equal to TrqCmnd_Reg.TrqCommand.
        }
        else
        {
            //--------- Select Isq_ref from Speed Controller or %Torque ---------------
            if(Isq_com < PI_out)// if PI_out > Isq_com, speed controller is enable.
                Isq_ref = PI_out;
            else
                Isq_ref = Isq_com;// if ABS(PI_out) > ABS(Isq_com), Iq_ref is equal to TrqCmnd_Reg.TrqCommand.
        }




        if(Isq_ref > Isqrate)
            Isq_ref = Isqrate;
        else if(Isq_ref < -Isqrate)
            Isq_ref = -Isqrate;

//========================  Minimal command selection ==================
    }
    //------Select Speed Control or Torque Control------------

//========================================================
//=============== Current Loop - d axis ==================
//========================================================
//------Select Speed Control or Torque Control------------
//      if(grp_num[5].value[33] == 0)//F534 = 0 is Speed Control
//      {
        //====== Calculation of Isd_ref can be showed following =====
        //    Isd_ref = sqrt(1 - PF^2)*Irate/Ibase;
        //    Isq_ref = limit +- (PF*Irate/Ibase)
        //===========================================================


    Isd_ref = _IQ17div(Vecter_Reg.Id_Command,MainIQ_Variable.I_base);//_IQ17

    //===================================================
    //------------ Field Weakening -------------------
    //    Vd,Vq are considered in the L-L axis
    //
    //    Vmax_WF =  sqrt(2)*Sqrt(Vd^2 + Vq^2) : Find the peak of L-L Voltage reference occured by the output of current controller
    //
    //    Id_WF = (Vdc - Vmax_WF)*KpFW :
    KpFW = 13107;//13107;//98304;
    KiFW_Ts = 2621;//2621;//131072;//Q17  kiFW*Ts = 50*200e-6*2^17

    Free1 = _IQ12mag(Vsd_ref, Vsq_ref);
    Free2 = _IQ12mpy(Free1,MainIQ_Variable.V_base);
    Vmax_weaken = _IQ17mpy(Free2,185364);//185364 = sqrt(2)Q17 peak line-to-line

    Verror_FW = _IQ17mpy(VDC_Read_reg.VdcBus,_IQ17(0.95)) - Vmax_weaken;
    Free2 = _IQ17mpy(Verror_FW, KpFW);//Q17         Er = (Vdc-Vmax_weaken)*KpFW
    Free3 = _IQ17div(Free2,MainIQ_Variable.I_Rate);

//      Isd_weaken_Limit = _IQ17div(_IQ17(-2),MainIQ_Variable.I_Rate);// Limited by -2A
    Isd_weaken_Limit = _IQ17mpy(-Isd_ref,_IQ17(0.6));

    if(Free3 >= 0)
        Isd_ref_weaken = 0;
    else if(Free3 <= Isd_weaken_Limit)
        Isd_ref_weaken = Isd_weaken_Limit;

    //------- Integral --------
    if(Isd_ref_weaken == 0)
        integral_Isd_ref_weaken = 0;
    else
    {
        integral_Isd_ref_weaken = _IQ17mpy(Verror_FW, KiFW_Ts) + integral_Isd_ref_weaken;//Q17

        Isd_ref_weaken = Isd_ref_weaken + integral_Isd_ref_weaken;

        if(Isd_ref_weaken >= 0)
            Isd_ref_weaken = 0;
        else if(Isd_ref_weaken <= Isd_weaken_Limit)
            Isd_ref_weaken = Isd_weaken_Limit;
    }
    //------- Integral --------

    //------------ Field Weakening -------------------
    //===================================================

    currentloop_d.Kp =  Vecter_Reg.Kn_CurrentLoop;
    currentloop_d.Ki =  Vecter_Reg.Tn_CurrentLoop;
    currentloop_d.Kd = _IQ(0.000);
    currentloop_d.Kc = _IQ17div(Vecter_Reg.Tn_CurrentLoop,Vecter_Reg.Kn_CurrentLoop);//_IQ(0.8);            Kc = Ki/Kp
    currentloop_d.OutMax = _IQ17div(Vecter_Reg.Limit_Max_CurrentLoop,MainIQ_Variable.V_base);
    currentloop_d.OutMin = -currentloop_d.OutMax;//_IQ17div(Vecter_Reg.Limit_Min_CurrentLoop,MainIQ_Variable.V_base);

    currentloop_d.Ref = Isd_ref + Isd_ref_weaken;
//      currentloop_d.Ref = Isd_ref;
    currentloop_d.Fdb = Id;
    currentloop_d.calc(&currentloop_d);

    Vsd_ref = (int)(currentloop_d.Out>>5);
//========================================================
//============= End Current Loop - d axis ================
//========================================================

//========================================================
//=============== Current Loop - q axis ==================
//========================================================
    currentloop_q.Kp = currentloop_d.Kp;
    currentloop_q.Ki = currentloop_d.Ki;
    currentloop_q.Kd = currentloop_d.Kd;
    currentloop_q.Kc = currentloop_d.Kc;
    currentloop_q.OutMax = currentloop_d.OutMax;
    currentloop_q.OutMin = currentloop_d.OutMin;

    currentloop_q.Ref = Isq_ref;
    currentloop_q.Fdb = Iq;
    currentloop_q.calc(&currentloop_q);

    Vsq_ref = (int)(currentloop_q.Out>>5);
//========================================================
//============= End Current Loop - q axis ================
//========================================================

//========================================================
//========== Instantaneous Active Power ==================
//========================================================
    Free1 = _IQ17mpy(currentloop_d.Out,Id) + _IQ17mpy(currentloop_q.Out,Iq);
    Free2 = _IQ17mpy(MainIQ_Variable.I_base,Free1);
    Free3 = _IQ17mpy(MainIQ_Variable.V_base,Free2);

    if(Free3 < 0)
      InstActivePower = -Free3;
    else
      InstActivePower = Free3;




} 


