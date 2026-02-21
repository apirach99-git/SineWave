
#ifndef FUNCTION_AUTOTUNE_H
#define FUNCTION_AUTOTUNE_H

#define FindRs                  0
#define FindSigmaLs             1   //FindSigmaLs
#define FindRr                  2
#define FindLs                  10//2  xxxxx
#define FindLm                  3
#define FindRotorTimeConstant   11//3  xxxxx
#define FindSaturationCurve     4
#define FindAutoTuneEnd         7
#define FindRsRr_LsLr           5

 
extern void Function_AutoTune(void);
extern void Find_Rs(void);
extern void Find_SigmaLs(void);
extern void Find_Rr(void);
extern void Find_Lm(void);
extern void Find_Ls(void);
extern void ResetAutoTune(void);
extern void ResetAutoTune1(void);
extern void Find_RotorTimeConstant(void);
extern void AbsoluteTransCurrent(void);
extern void Find_RsRr_LsLr(void);
extern void Save_and_End_Autotune(void);
/////////////////////////////////////////////////////////////////
struct FINDRS_BITS   {      // bits  description
   Uint16 INITAIL:1;        // 0     Initial Auto tune
   Uint16 FindRsStep:4;     // 4:1   Find Rs Step
   Uint16 FlagDECVoltage:1; // 5     Decrete Voltage flag
};
union FINDRS_REG {
   Uint16              all;
   struct FINDRS_BITS  bit;
};  
/////////////////////////////////////////////////////////////////
struct FINDALLPARAMETER_BITS   {        // bits  description
   Uint16 AllParaStep:3;    // 2:0    All Parameter Step
   Uint16 EnableAT:1;       // 3 Enable autotune after checking all condition
   Uint16 ActiveAT:1;       //auto tune is operating
};

union FINDALLPARAMETER_REG {
   Uint16              all;
   struct FINDALLPARAMETER_BITS  bit;
};   
/////////////////////////////////////////////////////////////////
struct FINDLM_BITS   {      // bits  description
   Uint16 FINDLmStep:2;     // 1:0      FINDSIGMALS Step
   Uint16 INITAIL:1;            // 2
   Uint16 rsvd2:1;              // 3
   Uint16 rsvd3:1;              // 4
   Uint16 rsvd4:1;              // 5
   Uint16 rsvd5:1;              // 6
   Uint16 rsvd6:1;              // 7
   Uint16 rsvd7:8;              // 15:8  reserved
};

union FINDLM_REG{
   Uint16               all;
   struct FINDLM_BITS  bit;
};
/////////////////////////////////////////////////////////////////
struct FINDSIGMALS_BITS   {     // bits  description
   Uint16 FINDSIGMALSStep:2;    // 1:0      FINDSIGMALS Step
   Uint16 INITAIL:1;            // 2
   Uint16 rsvd2:1;              // 3
   Uint16 rsvd3:1;              // 4
   Uint16 rsvd4:1;              // 5
   Uint16 rsvd5:1;              // 6
   Uint16 rsvd6:1;              // 7
   Uint16 rsvd7:8;              // 15:8  reserved
};

union FINDSIGMALS_REG{
   Uint16               all;
   struct FINDSIGMALS_BITS  bit;
};  
/////////////////////////////////////////////////////////////////

struct FINDROTORTIMECONSTANT_BITS   {       // bits  description
   Uint16 FINDROTORTIMECONSTANTStep:2;      // 1:0      FINDSIGMALS Step
   Uint16 INITAIL:1;            // 2
   Uint16 rsvd2:1;              // 3
   Uint16 rsvd3:1;              // 4
   Uint16 rsvd4:1;              // 5
   Uint16 rsvd5:1;              // 6
   Uint16 rsvd6:1;              // 7
   Uint16 rsvd7:8;              // 15:8  reserved
}; 
union FINDROTORTIMECONSTANT_REG{
   Uint16               all;
   struct FINDROTORTIMECONSTANT_BITS bit;
}; 
/////////////////////////////////////////////////////////////////

struct FINDLS_BITS   {      // bits  description
   Uint16 FindLsStep:2;     // 1:0      FINDSIGMALS Step
   Uint16 INITAILFLUX1:1;           // 2
   Uint16 INITAILFLUX2:1;               // 3
   Uint16 Count100ms:3;             // 4:6
   Uint16 rsvd4:1;              // 7
   Uint16 rsvd5:1;              // 8
   Uint16 rsvd6:1;              // 9
   Uint16 rsvd7:6;              // 15:8  reserved
}; 
union FINDLS_REG{
   Uint16               all;
   struct FINDLS_BITS   bit;
}; 
///////////////////////////////////////////////////////////////// 
struct FINDRR_BITS   {      // bits  description
   Uint16 FindRrStep:2;     // 1:0      FINDSIGMALS Step
   Uint16 INITAIL:1;            // 2
   Uint16 Count100ms:3;         // 3:5
   Uint16 rsvd3:1;              // 6
   Uint16 rsvd4:1;              // 7
   Uint16 rsvd5:1;              // 8
   Uint16 rsvd6:1;              // 9
   Uint16 rsvd7:6;              // 10:15  reserved
};
union FINDRR_REG{
   Uint16               all;
   struct FINDRR_BITS   bit;
};
/////////////////////////////////////////////////////////////////

struct AUTOTUNE_CTRL_REGS {
   union    FINDRS_REG  FINDRS;
   union    FINDALLPARAMETER_REG FLAGALLPARA;
   union    FINDLS_REG  FINDLS;
   union    FINDRR_REG  FINDRR;
   long     CountDelayAutoTune;
   long     FindRsInitVol;
   long     FindRsVolStep;
   long     V_old;
   long     I_old;
   long     Delta_V;
   long     Delta_I;
   long     Ix_alpha;
   long     Ix_beta;
   long     Ix_mag;
   long     Count_Delta;
   float    Rs;
   float    sum_Rs;
   union    FINDSIGMALS_REG FINDSIGMALS;
   union    FINDLM_REG FINDLM;
   long     Intregral_Lm;
   long     FindSigmaLsInitVol;
   long     TEMP1;
   long     TEMP2;
   long     TEMP3;
   float    TEMP4;
   float    TEMP5;
   float    TEMP6;

   float    SigmaLs;
   float    IntregrateSigmaLs;

   union    FINDROTORTIMECONSTANT_REG FINDROTORTIMECONSTANT;
   long     IntregratePI_Is;
   long     Isref;

   long     Y_RLS;
   long     Xnew_RLS;
   long     Er_RLS ;

   long     FLUX1;
   long     FLUX2;
   long     IntregrateFlux;
   long     Is;

   float    Ls;
   float    Rr;
   float    Lm;
};
extern struct AUTOTUNE_CTRL_REGS AutoTuneCtrlRegs;  


#endif  // 

//===========================================================================
// End of file.
//===========================================================================







