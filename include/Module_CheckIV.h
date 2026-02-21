
#ifndef MODULE_CHECKIV_H
#define MODULE_CHECKIV_H

//#define Cons_I _IQ17(5) //default
#define Cons_I	_IQ17(11)//time constant for I sensor, it should not be too much because could not determind the difference

#define Cons_I_per_phase    _IQ17(4)

#define Cons_V	_IQ17(200)//time constant for Vout
#define OffsetI 2048

//------------------------------------------------------------------------------------------------------------
struct IVFlag_bit{   					// bits   description 
   Uint16 Timer_ReadI:4;				//timer for sampling read current U,V,W
   Uint16 OffsetA2DI:1;					//bit completed A2D offset after mcu reset
   Uint16 FrstIp:1;						//bit check first offset
   Uint16 FirstStopLPFIp:1;				//bit check first stop for clear LPF 
} ;

union IVFlag {
	Uint16	all;
	struct IVFlag_bit	bit;
} ;  
//------------------------------------------------------------------------------------------------------------
struct IV_READ_REG{
	union IVFlag			IV_FLAG;
	_iq						I_Alpha;
	_iq						I_Beta;
	_iq						I_Peak;			//I peak PU
	_iq						I_rms;			//Irms PU
	_iq						PU_IU;			//per unit line U
	_iq						PU_IV;			//per unit line V
	_iq						PU_IW;			//per unit line W
    _iq                     PU_VO;          //per unit Vout
    _iq                     PU_VI;          //per unit Vout
    float                     PU_VI_last;
	_iq						Vout_Rms;		//V output rms
	_iq                     Vin_Rms;       //V output rms
	Uint32					BuffA2DA0;		//buffer A2D A0 before average
	Uint32					BuffA2DA1;		//buffer A2D A1 before average
	Uint32					BuffA2DA2;		//buffer A2D A2 before average
	Uint32                  BuffA2DA7;        //buffer A2D A7 before average
	Uint32                  BuffA2DA4;        //buffer A2D A7 before average
	int16					CenterIu;		//Center I of phaseU
	int16					CenterIw;		//Center I of phaseW
	int16					CenterIv;		//Center I of phaseV
	int16                   CenterVo;       //Center Vout
	int16                   CenterVi;       //Center Vout
	int16                     FreqIn;       //V output rms
	_iq						BuffIp;			//buffer Ip after cal. loop 5k
	_iq						BuffAvrgIp;
	Uint16					CntrAvrgA2D;
	_iq						Isd;
	_iq						Isq;

//Gus
//Power monitoring
	_iq                     Filtered_P;
	_iq                     Filtered_Q;
	_iq                     S;
	_iq                     pf;


};
extern struct IV_READ_REG IV_Read_reg;

typedef struct
{
        float             sum_sq;
        uint16_t        count;
        uint16_t        window;
        float             rms; // renamed from vrms
} RMSCalc_t;

static inline void RMS_Init(RMSCalc_t *v, uint16_t window)
{
        v->sum_sq = 0.0f;
        v->count  = 0;
        v->window = window;
        v->rms    = 0.0f;
}

static inline void RMS_Update(RMSCalc_t *v, float val)
{
        v->sum_sq += val * val;
        v->count++;

        if(v->count >= v->window)
        {
                v->rms    = sqrtf(v->sum_sq / (float)v->count);
                v->sum_sq = 0.0f;
                v->count  = 0;
        }
}

extern RMSCalc_t Vrms_In;
extern RMSCalc_t Vrms_Out;
extern RMSCalc_t Irms_Out; // Added external declaration

#endif  // 

//===========================================================================
// End of file.
//===========================================================================
