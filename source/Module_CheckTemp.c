//completed : LPF/Short/open temp circuit
//completed : check max/min temp(155^C/25^C)
//completed : interpolation cal.temp
//completed : converse A2D to ^C
//added : temp = 0,if short/open circuit
//revised : cal from buff_temp and load to register and disable temp sensor
//completed : revised to subroutine
//revised : temp sensor
//completed : convert0-155 of internal sensor
//completed: external/internal sensor
//revised : load A2D temp at first time to avoid the LPF rise time
//completed : check temp-10C to 155C
//2012-6-21 : revised check short/open circuit of temp sensor
//2012-7-30 : revised LPF
//2012-10-16 : revised LPF Ttoa
//2013-4-11 : Disable temperature sensor
//2013-5-21 : revise Tsampling=1ms for more stable reading temp.
//2013-7-20 : remove chanel internal
//2013-7-23 : check circuit 4chanels
//2013-7-23 : add LPF
//2013-7-23 : converse temp 4channels 
//2013-7-23 : add check bit first time for LPF error
//2013-7-23 : debug stage update output of temp. data
//2013-7-24 : revise status circuit disable/normal/short/open

#include "F28x_Project.h"
#include "include\ND2.h"
#include "f28004x_sw_prioritized_isr_levels.h"

struct TEMPERATURE_REG Temperature_reg = {0,0,0};//start A2D temp@25^C
_iq arTemperature[]={0,0,0,0};
_iq arA2DTemperature[]={0,0,0,0};
/*const Uint16 arrA2DTemp[] = {
                             3988, 3866, 3726, 3570, 3398,
                             3213, 3017, 2815, 2610, 2406,
                             2205, 2012, 1828, 1655, 1494,
                             1346, 1210, 1087, 976,  876,
                             787,  707,  635,  572,  515,
                             465,  420,  380,  344,  312,
                             284,  259,  236,  216
                            };//A2D table of External sensor -10C-155C
*/
const Uint16 arrA2DTemp_Int_Sensor[] = {//B value 3305 >7.5HP
                             3601,
                             3559,
                             3514,
                             3466,
                             3416,
                             3364,
                             3310,
                             3253,
                             3195,
                             3134,
                             3072,
                             3007,
                             2942,
                             2875,
                             2806,
                             2737,
                             2666,
                             2596,
                             2524,
                             2453,
                             2381,
                             2309,
                             2238,
                             2167,
                             2097,
                             2027,
                             1959,
                             1891,
                             1825,
                             1760,
                             1696,
                             1634,
                             1574,
                             1514,
                             1457,
                             1401,
                             1348,
                             1295,
                             1244,
                             1195,
                             1149,
                             1103,
                             1059,
                             1016,
                             976,
                             937,
                             899,
                             863,
                             829,
                             795,
                             764,
                             733,
                             704,
                             676,
                             649,
                             624,
                             599,
                             575,
                             553,
                             531,
                             511,
                             491,
                             472,
                             454,
                             437,
                             420,
                             405,
                             389,
                             376,
                             349,
                             349,
                             336,
                             323,
                             312,
                             301,
                             290,
                             280,
                             270,
                             261,
                             251,
                             243,
                             234,
                             227,
                             220
                              };//A2D table of External sensor -10C-155C
const Uint16 arrA2DTemp_Ext_Sensor[] = {//B_value 3366
                                        3613,
                                        3570,
                                        3525,
                                        3478,
                                        3428,
                                        3375,
                                        3321,
                                        3264,
                                        3204,
                                        3143,
                                        3080,
                                        3015,
                                        2948,
                                        2880,
                                        2811,
                                        2740,
                                        2668,
                                        2596,
                                        2524,
                                        2451,
                                        2378,
                                        2304,
                                        2232,
                                        2160,
                                        2088,
                                        2017,
                                        1947,
                                        1879,
                                        1812,
                                        1746,
                                        1681,
                                        1618,
                                        1556,
                                        1496,
                                        1438,
                                        1382,
                                        1327,
                                        1274,
                                        1223,
                                        1174,
                                        1126,
                                        1081,
                                        1036,
                                        994,
                                        953,
                                        914,
                                        877,
                                        840,
                                        806,
                                        773,
                                        741,
                                        711,
                                        682,
                                        654,
                                        628,
                                        602,
                                        578,
                                        555,
                                        532,
                                        511,
                                        491,
                                        471,
                                        453,
                                        436,
                                        418,
                                        403,
                                        387,
                                        372,
                                        358,
                                        331,
                                        331,
                                        320,
                                        307,
                                        296,
                                        286,
                                        275,
                                        266,
                                        256,
                                        246,
                                        238,
                                        229,
                                        222,
                                        214,
                                        206
                            };//A2D table of External sensor -10C-155C

const Uint16 *PntrTemp;
//---External variable import--------------------------------------
extern Uint16 arA2dBuff[];
struct data_grp{
    Uint16 value[100];
    Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber]; 
//----------------------------------------------------------------- 
////////////////////////////////////////////////////////////////
void ReadTemperature(void)
{
struct {
    Uint16 NoChnls:4;       //number of channel
    Uint16 ActiveChnl:1;    //Active channel
    Uint16 i:3;             //tester of channel
    Uint16 shiftCircuit:3;  //shft buffer for checking circuit
}readtemp1={0};
//-----------------------------------------------------------------
struct {
    Uint16 StCircuit:8;     //Buffer Status of circuit
    Uint16 buffStCircuit:8;
}readtemp2={0}; 
_iq bufftemp=0; 

    if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
    {
        readtemp1.NoChnls = grp_num[0].value[28];   //Load number of chanel     RamTestTemp.i=0;
        for(readtemp1.i=0;readtemp1.i<4;readtemp1.i++)
        {
            if(Temperature_reg.GENERAL_TEMPFLAG.bit.FirstTemp==0)
            {
                arA2DTemperature[readtemp1.i]=_IQ17(arA2dBuff[A2dA5Buff+readtemp1.i]);
            }
            readtemp1.ActiveChnl = 0x0001 & readtemp1.NoChnls;
            if(readtemp1.ActiveChnl!=0)
            {
                readtemp1.shiftCircuit = readtemp1.i*2;
                if(arA2dBuff[A2dA5Buff+readtemp1.i] <= MinA2DTemp)
                {
                    readtemp2.StCircuit = EnableShort;
                }else if(arA2dBuff[A2dA5Buff+readtemp1.i] >= MaxA2DTemp)
                {
                    readtemp2.StCircuit = EnableOpen;
                }else
                {
                    readtemp2.StCircuit = EnableNormal;
                    arA2DTemperature[readtemp1.i] = LowPassFilter(_IQ17(arA2dBuff[A2dA5Buff+readtemp1.i]),
                                                    arA2DTemperature[readtemp1.i],Cons_Temp,IQ17_1);
                    bufftemp = ConverseTemp(arA2DTemperature[readtemp1.i]);//converse A2D to TempC
                }
            }else
            {
                readtemp2.StCircuit = DisableTemp;
            }
            arTemperature[readtemp1.i] = bufftemp;
            readtemp2.buffStCircuit    = readtemp2.buffStCircuit|(readtemp2.StCircuit<<readtemp1.shiftCircuit);
            readtemp1.NoChnls          = readtemp1.NoChnls>>1;
            bufftemp = 0;
        }
        Temperature_reg.GENERAL_TEMPFLAG.bit.FirstTemp = 1;
        Temperature_reg.CircuitTemp.all                = readtemp2.buffStCircuit;
    }
} 
////////////////////////////////////////////////////////////////
_iq ConverseTemp(_iq inputA2D)
{
Uint16 buff_cal= 0;//index pointer array A2D,buffer for calculate the temp diff.
_iq buff_delta = 0;//buffer for interpolation,buffer current temp.
_iq buff_temp=0, n=_IQ17(-10);
//Uint16 A2DMinus10C = 3988, A2D155C=216;
Uint16 A2DMinus10C, A2D156C;
Uint16 buffA2Dtemp = _IQ17int(inputA2D);
const Uint16 *buff_n_1;

    //PntrTemp = &arrA2DTemp[0];

    switch (grp_num[0].value[10])
    {
    case 1:
        PntrTemp    = &arrA2DTemp_Ext_Sensor[0];
        A2DMinus10C = 3613;
        A2D156C     = 206;
        break;
    case 2:
        PntrTemp    = &arrA2DTemp_Int_Sensor[0];
        A2DMinus10C = 3601;
        A2D156C     = 220;
        break;
    }

    //check max/min temp(155^C/-10^C)/normal
    if(buffA2Dtemp>=A2DMinus10C)
    {
        buff_temp = _IQ17(-10); //Temp. = -10^C
    }
    else if(buffA2Dtemp<=A2D156C)
    {
        //buff_temp = _IQ17(155); //Temp. = 155^C
        buff_temp = _IQ17(156); //Temp. = 155^C
    }
    else
    {
        while(buffA2Dtemp<*PntrTemp)
        {
            PntrTemp++;//increase pointer in A2D array
            //n=n+_IQ17(5);//increase temperature 5^C a step
            n=n+_IQ17(2);//Gus
        }
        /*
                5*{data[i-1]-A2D}
            X = -----------------  //eq. calculate delta interpolation
                data[i-1]-data[i]
        */
        buff_n_1 = --PntrTemp;
        PntrTemp++;

        buff_cal   = *buff_n_1-buffA2Dtemp; //data[i-1]-A2D
        //buff_cal   = buff_cal*5;
        buff_cal   = buff_cal*2;
        buff_delta = _IQ17((float32)buff_cal); //backup 5*{data[i-1]-A2D} as iq format
        buff_cal   = *buff_n_1-*PntrTemp; //data[i-1]-data[i]
        buff_delta = _IQ17div(buff_delta,_IQ17((float32)buff_cal));
        //n = n-_IQ17(5);
        n = n-_IQ17(2);//Gus
        buff_temp = n+buff_delta;//temp. = (n-5)+X
    }
    return buff_temp;
}
////////////////////////////////////////////////////////////////
