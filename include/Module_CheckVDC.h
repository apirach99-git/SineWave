
#ifndef MODULE_CHECKVDC_H
#define MODULE_CHECKVDC_H

#define VDCmax1 _IQ17(1000) //3V ~ 909.09091VDC
#define VDCmax0 _IQ17(500)  //3V ~ 454.54545VDC
#define ConsTVDC _IQ17(1)           //constant time for LPF VDC bus
//------------------------------------------------------------------------------------------------------------
struct VDCFLAG_bit{                     // bits   description
   Uint16 Timer_Readv:4;                //timer for sampling read current U,V,W
} ;

union VDCFlag {
    Uint16  all;
    struct VDCFLAG_bit  bit;
} ;  
//------------------------------------------------------------------------------------------------------------
struct VDC_READ_REG{
    union VDCFlag               VDC_Flag;
    Uint16                      Timer_ReadVDC;  //timer for sampling rate VDC
    _iq                         VDCGain;        //F020 gain VDC
    _iq                         VdcBus;         //last output VDCBus
    _iq                         PUVDC;          //PU VDC with 1000 or 500
    _iq                         BuffPUVDC;      //PU VDC before LPF
    uint16_t                    CntAvgPUVDC;
    _iq                         BuffVDCcomp;
    _iq                         VdcDisplay;     //DC bus voltage monitoring
};

extern struct VDC_READ_REG VDC_Read_reg; 

#endif  // 

//===========================================================================
// End of file.
//===========================================================================
