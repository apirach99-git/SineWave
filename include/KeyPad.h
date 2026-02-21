//for KeyPad4.c
//for KeyPad27.c
//for KeyPad33.c-->correct factor F002,F703=0x0801
//for KeyPad37.c
//revised factor F230 and F232
//compiled passed
/*
    note : Factor definition
    15 14 13 12 / 11 10 9 8 / 7 6 5 4 / 3 2 1 0                
    -  -  -  -  / -  -  - - / - - - - / - - - -

    bit13  : 1=-,0=+
    bit12  : 1; there are + and- value , 0; + only
    bit11  : 1=setting by digit(passward befor to change),0=normal setting
    bit10  : 1=no setting when run,0=no effect running
    bit9-8 : current position of DECIB (number position)
    bit7-6 : min position of DECIB (number position)
    bit5-4 : max position of DECIB (number position)
    bit3-2 : how many digit display;
             00->4digit
             01->3digit
             10->2digit
             11->1digit
    bit1-0 : counting/step
*/
#ifndef KEYPAD_H
#define KEYPAD_H


#ifdef __cplusplus
extern "C" {
#endif

// Define key value
//#define SWV         217 //2023-02-17
//#define SWV         218 //2023-10-09
//#define SWV         219
//#define SWV         220
//#define SWV         221 //2024-06-06
//#define SWV         222 //2024-07-27
//#define SWV         223 //2024-08-27
//#define SWV         224 //2024-10-18
//#define SWV         225 //2024-12-12
//#define SWV         226 //2025-02-08
#define SWV         227 //2025-07-18

//#define maindefault 0x74 //1110100 (PRG+DOWN+DATA)
#define maindefault 0x7A //1111010 (PRG+UP)
#define no_key      0x7F //1111111
#define sw_prog     0x7E //1111110
#define sw_data     0x7D //1111101
#define sw_up       0x7B //1111011
#define sw_down     0x77 //1110111
#define sw_run      0x6F
#define sw_stop     0x5F
#define sw_shift    0x3f

// Define interval key for next step/speed
#define t_key_next_step   300 //time for change speed up/down key
#define t_key_clrflt      2000 //time enable clear fault

// Define key speed
#define FirstSpeed  198//minimum = FirstSpeed-(stepdown*7)
#define stepdown    28//stepdown

// Define value in package TX
#define tab_disp    0//display table
#define data_disp   1//display data
#define grp_disp    2//display group number
#define func_disp   3//display function number

//  sign
#define nosign  0
#define plus    1//+
#define minus   3//-

//  fualt display
#define no_fault    0
#define disp_fault  1

//  led run
#define no_run      0
#define led_run     1

//  vr com
#define no_vr       0
#define freq_vol    1

//  show password
#define no_pass     0//no password
#define disp_pass   1//display password


//  mode state
#define state_norm_qck  0//state normal or quick mode(000-802)
#define state_grp       1//state group of function(GRP1-GRP8)
#define state_subfunct  2//state subfunction(sub funct in each group101,201)
#define state_setting   3//under setting

#define tm_delay_norm   3000//timer delay before save data in normal mode
//-----Gus-----
#define tm_delay_data   60000
//-----Gus-----
#define tm_blnk         500//blinking toggle max=4096ms
#define tm_blnk1        250//blinking during setting
#define noshow_hilo     0
#define disphi_lo       1

//  max/min in function
#define max_qck     29
#define min_qck     0

#define MaxMainGrp  8
#define MinMainGrp  0

// Motor Direction
#define Dir_fwd     0
#define Dir_rev     1

// Data For E2Prom
#define Start_adrsH_E2prom      0x00
#define Start_adrsL_E2prom      0x00
#define I2C_SLAVE_ADDR          0x50
#define I2C_NUMBYTES            8

#define I2C_EEPROM_HIGH_ADDR    0x00
#define I2C_EEPROM_LOW_ADDR     0x00

#define timeout_e2p             50//timeout 10ms
#define cntr_err                10//times of error

#define Input       0
#define Output      1

//Data must less than 16
#define UnitHz      0//Hz output frequency
#define UnitA       1//A output current
#define UnitV       2//V command voltage
#define UnitVdc     3//Vdc
#define UnitRPM     4//synchronous speed,machine speed
#define UnitMPM     5//line speed
#define UnitC       6//heat sink temperature
#define UnitRPM1    7
#define UnitKw      8
#define UnitKHz     9
#define UnitS       10
#define UnitmS      11
#define UnitPercent 12
#define UnitUnit    13
#define UnitNo      14

#define delay_wrE2prom      15 //7

#define UpDEC       1
#define DownDEC     0

#define load_min    1
#define no_load_min 0
#define load_max    1
#define no_load_max 0

//Display table data
#define dash    1
#define hi      2
#define lo      3

Uint16  ar_qckmode[] = {0,101,102,103,107,201,225,227,258,301,302,303,309,311,312,
                        313,314,315,316,317,345,346,419,421,503,522,523,524,528,802};
//------------------------------------------------------------------ 
_iq ar_buffdisp[] = {0,0,0,0,0,
                     0,0,0,0,0};//buffer after filter display(Fcmd,Ipeak,VDC,Vout,Actual RPM,TcmdTemp.1-4)
                     //0};
Uint16  Norm_Val_Disp  =0;
Uint16  Norm_Dec_Disp  =0;
Uint16  Norm_Unit_Disp =0;

//------------------------------------------------------------------------------------------------------------
Uint16  ar_fault[] = {0,0,0,0,0};
//------------------------------------------------------------------------------------------------------------
Uint16  ar_PSW_Tester[] = {0,9944};
//------------------------------------------------------------------------------------------------------------
//Gus
//------------Gus-------------------------
Uint16 txMSG49[8];
Uint16 BackUpTxMSG49[8];
Uint16 rxMSG49[8];
//-------------------------------------------

Uint32  MSG_MBX1DataL = 0;
Uint32  MSG_MBX1DataH = 0;
Uint32  BufferMBX0L   = 0;

Uint16  Tmr_key_next_step   = 0;//timer for enabling up spped for
Uint16  Tmr_CNTR_UpDown     = FirstSpeed;//preram
Uint16  Tmr_CNTR_UpDown1    = 0;//interval for counting++,--
Uint16  Tmr_Norm_Set        = 0;//interval for setting in normal mode
//---------Gus-----------
Uint16  Tmr_Data_Set     = 0;
Uint16  Tmr_Mode_Set     = 0;
Uint16  Tmr_Button_Set   = 0;
//---------Gus-----------
Uint16  PreBufCtrSetting = 0;//pre-buffer for setting in function before data enter
Uint16  BufferPreDECIB   = 0;
Uint16  BufferPreSIGN    = 0;
//------------Gus----------------
Uint16  BufFMax = 0;//pre-buffer for setting in function before data enter
Uint16  BufferPreDECIB_FMax   = 0;
Uint16  BufferPreSIGN_FMax    = 0;

Uint16  BufFMin = 0;//pre-buffer for setting in function before data enter
Uint16  BufferPreDECIB_FMin   = 0;
Uint16  BufferPreSIGN_FMin    = 0;
//------------Gus----------------
//-----------------------------Gus-----------------------------
Uint16  PreBufCtrSetting2 = 0;  //buffer
Uint16  BufferPreDECIB2   = 0;
Uint16  BufferPreSIGN2    = 0;

Uint16  *PntrMaxset;
Uint16  *PntrMinset;
Uint16  *PntrFact_Maxset;

_iq  MaxSet2 = 0;
Uint16  MinSet2 = 0;
//-----------------------------Gus-----------------------------

Uint16  *PntrVal;
Uint16  *PntrFact;
Uint16  *PntrDataE2Prom;//pointer for data with e2prom
Uint16  MaxSet      = 0;
Uint16  MinSet      = 0;
Uint16  DevideDigit = 0;
Uint16  BuffDigit   = 0;
Uint16  PSW_Tester  = 0;

Uint16  TimeOutE2p  = 0;//timer for timeout e2p

//------------------------------------------------------------------
const Uint16 Table_UnitL[] =
{ 
        UnitHz,         //output freq
        UnitA,          //output current
        UnitVdc,        //vdc bus
        UnitRPM,        //sync speed
        UnitMPM,        //line speed
        UnitV,          //output voltage
        UnitRPM,        //machine speed
        UnitRPM,        //Actual RPM
        UnitNo,         //output torque
        UnitC,          //temperature
        UnitC,          //temperature
        UnitC,          //temperature
        UnitC,          //temperature
};  
//------------------------------------------------------------------ 
const Uint16 Table_UnitGrp0[] =
{
        UnitNo,             //  Unit_001
        UnitNo,             //  Unit_002
        UnitS,              //  Unit_003
        UnitA,              //  Unit_004
        UnitNo,             //  Unit_005
        UnitNo,             //  Unit_006
        UnitNo,             //  Unit_007
        UnitNo,             //  Unit_008
        UnitNo,             //  Unit_009
        UnitC,              //  Unit_010
        UnitNo,             //  Unit_011
        UnitNo,             //  Unit_012
        UnitmS,             //  Unit_013
        UnitUnit,           //  Unit_014
        UnitNo,             //  Unit_015
        UnitHz,             //  Unit_016
        UnitNo,             //  Unit_017
        UnitNo,             //  Unit_018
        UnitNo,             //  Unit_019
        UnitNo,             //  Unit_020
        UnitNo,             //  Unit_021
        UnitNo,             //  Unit_022
        UnitS,              //  Unit_023
        UnitS,              //  Unit_024
        UnitS,              //  Unit_025
        UnitmS,             //  Unit_026
        UnitmS,             //  Unit_027
        UnitNo,             //  Unit_028
        UnitNo,             //  Unit_029
        UnitC,              //  Unit_030
        UnitC,              //  Unit_031
        UnitC,              //  Unit_032
        UnitC,              //  Unit_033
        UnitNo,             //  Unit_034
};
//------------------------------------------------------------------
const Uint16 Table_UnitGrp1[] =
{
        UnitHz,     //  Unit_101
        UnitV,      //  Unit_102
        UnitA,      //  Unit_103
        UnitRPM,    //  Unit_104
        UnitKw,     //  Unit_105
        UnitNo,     //  Unit_106
        UnitNo,     //  Unit_107
        UnitA,      //  Unit_108
        UnitNo,     //  Unit_109
        UnitNo,     //  Unit_110
        UnitNo,     //  Unit_111
        UnitNo,     //  Unit_112
        UnitNo,     //  Unit_113
        UnitNo,     //  Unit_114
        UnitNo,     //  Unit_115
        UnitNo,     //  Unit_116
        UnitHz,     //  Unit_117
        UnitV,      //  Unit_118
        UnitA,      //  Unit_119
        UnitRPM,    //  Unit_120
        UnitKw,     //  Unit_121
        UnitNo,     //  Unit_122
        UnitNo,     //  Unit_123
        UnitA,      //  Unit_124
        UnitNo,     //  Unit_125
        UnitNo,     //  Unit_126
        UnitNo,     //  Unit_127
        UnitNo,     //  Unit_128
        UnitNo,     //  Unit_129
        UnitNo,     //  Unit_130
        UnitNo,     //  Unit_131
        UnitNo,     //  Unit_132
};
//------------------------------------------------------------------ 
const Uint16 Table_UnitGrp2[] =
{
        UnitNo,             //  Unit_201
        UnitNo,             //  Unit_202
        UnitNo,             //  Unit_203
        UnitNo,             //  Unit_204
        UnitNo,             //  Unit_205
        UnitNo,             //  Unit_206
        UnitNo,             //  Unit_207
        UnitNo,             //  Unit_208
        UnitNo,             //  Unit_209
        UnitNo,             //  Unit_210
        UnitNo,             //  Unit_211
        UnitNo,             //  Unit_212
        UnitNo,             //  Unit_213
        UnitNo,             //  Unit_214
        UnitNo,             //  Unit_215
        UnitmS,             //  Unit_216
        UnitNo,             //  Unit_217
        UnitNo,             //  Unit_218
        UnitNo,             //  Unit_219
        UnitNo,             //  Unit_220
        UnitNo,             //  Unit_221
        UnitNo,             //  Unit_222
        UnitmS,             //  Unit_223
        UnitV,              //  Unit_224
        UnitPercent,        //  Unit_225
        UnitV,              //  Unit_226
        UnitPercent,        //  Unit_227
        UnitV,              //  Unit_228
        UnitmS,             //  Unit_229
        UnitV,              //  Unit_230
        UnitPercent,        //  Unit_231
        UnitV,              //  Unit_232
        UnitPercent,        //  Unit_233
        UnitV,              //  Unit_234
        UnitmS,             //  Unit_235
        UnitNo,             //  Unit_236
        UnitPercent,        //  Unit_237
        UnitNo,             //  Unit_238
        UnitPercent,        //  Unit_239
        UnitNo,             //  Unit_240
        UnitNo,             //  Unit_241
        UnitNo,             //  Unit_242
        UnitmS,             //  Unit_243
        UnitPercent,        //  Unit_244
        UnitV,              //  Unit_245
        UnitPercent,        //  Unit_246
        UnitV,              //  Unit_247
        UnitPercent,        //  Unit_248
        UnitmS,             //  Unit_249
        UnitPercent,        //  Unit_250
        UnitV,              //  Unit_251
        UnitPercent,        //  Unit_252
        UnitV,              //  Unit_253
        UnitPercent,        //  Unit_254
        UnitHz,             //  Unit_255
        UnitHz,             //  Unit_256
        UnitHz,             //  Unit_257
        UnitHz,             //  Unit_258
        UnitV,              //  Unit_259
        UnitA,              //  Unit_260
};
//------------------------------------------------------------------  
const Uint16 Table_UnitGrp3[] =
{
        UnitNo,     //  Unit_301
        UnitHz,     //  Unit_302
        UnitHz,     //  Unit_303
        UnitHz,     //  Unit_304
        UnitHz,     //  Unit_305
        UnitS,      //  Unit_306
        UnitS,      //  Unit_307
        UnitHz,     //  Unit_308
        UnitNo,     //  Unit_309
        UnitNo,     //  Unit_310
        UnitHz,     //  Unit_311
        UnitHz,     //  Unit_312
        UnitHz,     //  Unit_313
        UnitHz,     //  Unit_314
        UnitHz,     //  Unit_315
        UnitHz,     //  Unit_316
        UnitHz,     //  Unit_317
        UnitHz,     //  Unit_318
        UnitHz,     //  Unit_319
        UnitHz,     //  Unit_320
        UnitHz,     //  Unit_321
        UnitHz,     //  Unit_322
        UnitHz,     //  Unit_323
        UnitHz,     //  Unit_324
        UnitHz,     //  Unit_325
        UnitNo,     //  Unit_326
        UnitNo,     //  Unit_327
        UnitNo,     //  Unit_328
        UnitNo,     //  Unit_329
        UnitNo,     //  Unit_330
        UnitNo,     //  Unit_331
        UnitNo,     //  Unit_332
        UnitNo,     //  Unit_333
        UnitNo,     //  Unit_334
        UnitNo,     //  Unit_335
        UnitNo,     //  Unit_336
        UnitNo,     //  Unit_337
        UnitNo,     //  Unit_338
        UnitNo,     //  Unit_339
        UnitNo,     //  Unit_340
        UnitS,      //  Unit_341
        UnitS,      //  Unit_342
        UnitS,      //  Unit_343
        UnitS,      //  Unit_344
        UnitS,      //  Unit_345
        UnitS,      //  Unit_346
        UnitS,      //  Unit_347
        UnitS,      //  Unit_348
        UnitS,      //  Unit_349
        UnitS,      //  Unit_350
        UnitS,      //  Unit_351
        UnitS,      //  Unit_352
        UnitNo,     //  Unit_353
        UnitNo,     //  Unit_354
        UnitS,      //  Unit_355
        UnitS,      //  Unit_356
        UnitS,      //  Unit_357
        UnitS,      //  Unit_358
        UnitS,      //  Unit_359
        UnitS,      //  Unit_360
        UnitS,      //  Unit_361
        UnitNo,     //  Unit_362
        UnitNo,     //  Unit_363
        UnitNo,     //  Unit_364
        UnitNo,     //  Unit_365
        UnitNo,     //  Unit_366
        UnitNo,     //  Unit_367
        UnitNo,     //  Unit_368
};
//------------------------------------------------------------------ 
const Uint16 Table_UnitGrp4[] =
{
        UnitNo,         //  Unit_401
        UnitPercent,    //  Unit_402
        UnitPercent,    //  Unit_403
        UnitNo,         //  Unit_404
        UnitS,          //  Unit_405
        UnitNo,         //  Unit_406
        UnitV,          //  Unit_407
        UnitNo,         //  Unit_408
        UnitS,          //  Unit_409
        UnitNo,         //  Unit_410
        UnitV,          //  Unit_411
        UnitNo,         //  Unit_412
        UnitPercent,    //  Unit_413
        UnitS,          //  Unit_414
        UnitHz,         //  Unit_415
        UnitHz,         //  Unit_416
        UnitHz,         //  Unit_417
        UnitHz,         //  Unit_418
        UnitNo,         //  Unit_419
        UnitPercent,    //  Unit_420
        UnitS,          //  Unit_421
        UnitNo,         //  Unit_422
        UnitS,          //  Unit_423
        UnitS,          //  Unit_424
        UnitNo,         //  Unit_425
        UnitS,          //  Unit_426
        UnitNo,         //  Unit_427
//LU Auto reset enable/disable
        UnitNo,         //  Unit_428
};
//------------------------------------------------------------------  
const Uint16 Table_UnitGrp5[] =
{
        UnitNo,         //  Unit_501
        UnitNo,         //  Unit_502
        UnitPercent,    //  Unit_503
        UnitHz,         //  Unit_504
        UnitHz,         //  Unit_505
        UnitHz,         //  Unit_506
        UnitV,          //  Unit_507
        UnitV,          //  Unit_508
        UnitV,          //  Unit_509
        UnitNo,         //  Unit_510
        UnitNo,         //  Unit_511
        UnitNo,         //  Unit_512
        UnitNo,         //  Unit_513
        UnitNo,         //  Unit_514
        UnitNo,         //  Unit_515
        UnitNo,         //  Unit_516
        UnitNo,         //  Unit_517
        UnitNo,         //  Unit_518
        UnitNo,         //  Unit_519
        UnitNo,         //  Unit_520
        UnitNo,         //  Unit_521
        UnitPercent,    //  Unit_522
        UnitS,          //  Unit_523
        UnitHz,         //  Unit_524
        UnitHz,         //  Unit_525
        UnitS,          //  Unit_526
        UnitHz,         //  Unit_527
        UnitNo,         //  Unit_528
        UnitKHz,        //  Unit_529
        UnitNo,         //  Unit_530
        UnitNo,         //  Unit_531
        UnitNo,         //  Unit_532
        UnitNo,         //  Unit_533
        UnitNo,         //  Unit_534
        UnitS,          //  Unit_535
        UnitS,          //  Unit_536
        UnitPercent,    //  Unit_537
        UnitNo,         //  Unit_538
        UnitNo,         //  Unit_539
        UnitHz,         //  Unit_540
        UnitHz,         //  Unit_541
        UnitNo,         //  Unit_542
};
//------------------------------------------------------------------
const Uint16 Table_UnitGrp6[] =
{
        UnitNo,         //  Unit_601
        UnitNo,         //  Unit_602
        UnitNo,         //  Unit_603
        UnitNo,         //  Unit_604
}; 
//------------------------------------------------------------------
const Uint16 Table_UnitGrp7[] =
{
        UnitNo,         //  Unit_701
        UnitNo,         //  Unit_702
        UnitNo,         //  Unit_703
        UnitNo,         //  Unit_704
        UnitS,          //  Unit_705
        UnitS,          //  Unit_706
        UnitPercent,    //  Unit_707
        UnitNo,         //  Unit_708
        UnitPercent,    //  Unit_709
        UnitPercent,    //  Unit_710
        UnitPercent,    //  Unit_711
        UnitNo,         //  Unit_712
        UnitNo,         //  Unit_713
        UnitS,          //  Unit_714
        UnitS,          //  Unit_715
        UnitPercent,    //  Unit_716
        UnitPercent,    //  Unit_717
        UnitS,          //  Unit_718
        UnitPercent,    //  Unit_719
        UnitNo,         //  Unit_720
        UnitPercent,    //  Unit_721
        UnitPercent,    //  Unit_722
        UnitPercent,    //  Unit_723
        UnitPercent,    //  Unit_724
        UnitPercent,    //  Unit_725
        UnitPercent,    //  Unit_726
        UnitPercent,    //  Unit_727
        UnitPercent,    //  Unit_728
        UnitPercent,    //  Unit_729
        UnitPercent,    //  Unit_730
        UnitPercent,    //  Unit_731
        UnitPercent,    //  Unit_732
        UnitPercent,    //  Unit_733
        UnitPercent,    //  Unit_734
        UnitPercent,    //  Unit_735
};
//------------------------------------------------------------------
const Uint16 Table_UnitGrp8[] =
{
        UnitNo,         //  Unit_801
        UnitNo,         //  Unit_802
        UnitNo,         //  Unit_803
        UnitNo,         //  Unit_804
        UnitNo,         //  Unit_805
        UnitNo,         //  Unit_806
};
//------------------------------------------------------------------
const Uint16 *Pntr_UnitSubFN[]={&Table_UnitGrp0[0],&Table_UnitGrp1[0],  //main pointer selectable pointer will be moved to here
                                &Table_UnitGrp2[0],&Table_UnitGrp3[0],
                                &Table_UnitGrp4[0],&Table_UnitGrp5[0],
                                &Table_UnitGrp6[0],&Table_UnitGrp7[0],
                                &Table_UnitGrp8[0]};
//------------------------------------------------------------------
const Uint16 Table_MaxNumGrp[] =
{ 
         34,   //max_grpf0
         32,   //max_grpf1
         60,   //max_grpf2
         68,   //max_grpf3
         28,   //max_grpf4
         42,   //max_grpf5       38 + 4
         4,    //max_grpf6
         35,   //max_grpf7
         6,    //max_grpf8
};
//------------------------------------------------------------------
const Uint16 Table_Disp[] =
{
         0,
         52, //dash
         54, //hi
         55, //lo
}; 
//------------------------------------------------------------------
const Uint16 Table_value0[] =
{
         0,              // default_001    adrse2prom 0x0100
         1234,           // default_002    adrse2prom 0x0102
         2000,           // default_003    adrse2prom 0x0104
         1,              // default_004    adrse2prom 0x0106
         1,              // default_005    adrse2prom 0x0108
         1,              // default_006    adrse2prom 0x010A
         30,             // default_007    adrse2prom 0x010C
         30,             // default_008    adrse2prom 0x010E
         3,              // default_009    adrse2prom 0x0110
         900,            // default_010    adrse2prom 0x0112
         1,              // default_011    adrse2prom 0x0114
         0,              // default_012    adrse2prom 0x0116
         300,            // default_013    adrse2prom 0x0118
         5,              // default_014    adrse2prom 0x011A
         0,              // default_015    adrse2prom 0x011C
         50,             // default_016    adrse2prom 0x011E
         22,             // default_017    adrse2prom 0x0120
         0,              // default_018    adrse2prom 0x0122
         1000,           // default_019    adrse2prom 0x0124
         0,              // default_020    adrse2prom 0x0126
         0,              // default_021    adrse2prom 0x0128
         0,              // default_022    adrse2prom 0x012A
         0,              // default_023    adrse2prom 0x012C
         0,              // default_024    adrse2prom 0x012E
         0,              // default_025    adrse2prom 0x0130
         0,              // default_026    adrse2prom 0x0132
         0,              // default_027    adrse2prom 0x0134
         0,              // default_028    adrse2prom 0x0136
         1,              // default_029    adrse2prom 0x0138
         900,            // default_030    adrse2prom 0x0140
         900,            // default_031    adrse2prom 0x0142
         900,            // default_032    adrse2prom 0x0144
         900,            // default_033    adrse2prom 0x0146
         1,              // default_034    adrse2prom 0x0148
};  
//------------------------------------------------------------------
const Uint16 Table_max0[] =
{ 
        1,        //    max_f001
        9,        //    max_f002
        6000,     //    max_f003
        2000,     //    max_f004
        1,        //    max_f005
        1,        //    max_f006
        60,       //    max_f007
        60,       //    max_f008
        4,        //    max_f009
        1500,     //    max_f010
        2,        //    max_f011
        1,        //    max_f012
        9999,     //    max_f013
        9999,     //    max_f014
        1,        //    max_f015
        200,      //    max_f016
        40,       //    max_f017
        1,        //    max_f018
        2000,     //    max_f019
        1,        //    max_f020
        1,        //    max_f021
        1,        //    max_f022
        1200,     //    max_f023
        1200,     //    max_f024
        1200,     //    max_f025
        9999,     //    max_f026
        9999,     //    max_f027
        1,        //    max_f028
        15,       //    max_f029
        1500,     //    max_f030
        1500,     //    max_f031
        1500,     //    max_f032
        1500,     //    max_f033
        1,        //    max_f034
}; 
//------------------------------------------------------------------
const Uint16 Table_min0[] =
{ 
        0,        //    min_f001
        0,        //    min_f002
        0,        //    min_f003
        1,        //    min_f004
        0,        //    min_f005
        0,        //    min_f006
        2,        //    min_f007
        2,        //    min_f008
        0,        //    min_f009
        500,      //    min_f010
        0,        //    min_f011
        0,        //    min_f012
        0,        //    min_f013
        0,        //    min_f014
        0,        //    min_f015
        0,        //    min_f016
        10,       //    min_f017
        0,        //    min_f018
        0,        //    min_f019
        0,        //    min_f020
        0,        //    min_f021
        0,        //    min_f022
        0,        //    min_f023
        0,        //    min_f024
        0,        //    min_f025
        0,        //    min_f026
        0,        //    min_f027
        0,        //    min_f028
        0,        //    min_f029
        500,      //    min_f030
        500,      //    min_f031
        500,      //    min_f032
        500,      //    min_f033
        0,        //    min_f034
};
//------------------------------------------------------------------
const Uint16 Table_factor0[] =
{
        0x0401,   //factor_001  adrse2prom 0x0200
        0x0C01,   //factor_002  adrse2prom 0x0202
        0x07B1,   //factor_003  adrse2prom 0x0204
        0x0511,   //factor_004  adrse2prom 0x0206
        0x0401,   //factor_005  adrse2prom 0x0208
        0x0401,   //factor_006  adrse2prom 0x020A
        0x0551,   //factor_007  adrse2prom 0x020C
        0x0551,   //factor_008  adrse2prom 0x020E
        0x0001,   //factor_009  adrse2prom 0x0210
        0x0151,   //factor_010  adrse2prom 0x0212
        0x0401,   //factor_011  adrse2prom 0x0214
        0x0401,   //factor_012  adrse2prom 0x0216
        0x0001,   //factor_013  adrse2prom 0x0218
        0x0401,   //factor_014  adrse2prom 0x021A
        0x0401,   //factor_015  adrse2prom 0x021C
        0x06A1,   //factor_016  adrse2prom 0x021E
        0x0401,   //factor_017  adrse2prom 0x0220
        0x0001,   //factor_018  adrse2prom 0x0222
        0x03F1,   //factor_019  adrse2prom 0x0224
        0x0401,   //factor_020  adrse2prom 0x0226
        0x0401,   //factor_021  adrse2prom 0x0228
        0x0001,   //factor_022  adrse2prom 0x022A
        0x0151,   //factor_023  adrse2prom 0x022C
        0x0151,   //factor_024  adrse2prom 0x022E
        0x0151,   //factor_025  adrse2prom 0x0230
        0x0001,   //factor_026  adrse2prom 0x0232
        0x0001,   //factor_027  adrse2prom 0x0234
        0x0401,   //factor_028  adrse2prom 0x0236
        0x0401,   //factor_029  adrse2prom 0x0238
        0x0151,   //factor_030  adrse2prom 0x0240
        0x0151,   //factor_031  adrse2prom 0x0242
        0x0151,   //factor_032  adrse2prom 0x0244
        0x0151,   //factor_033  adrse2prom 0x0246
        0x0001,   //factor_034  adrse2prom 0x0248
};
//------------------------------------------------------------------
const Uint16 Table_value1A[] =
{ 
        5000,   //default_101   adrse2prom 0x0300
        2200,   //default_102   adrse2prom 0x0302
        1,      //default_103   adrse2prom 0x0304
        1450,   //default_104   adrse2prom 0x0306
        1,      //default_105   adrse2prom 0x0308
        88,     //default_106   adrse2prom 0x030A
        4,      //default_107   adrse2prom 0x030C
        1,      //default_108   adrse2prom 0x030E
        1,      //default_109   adrse2prom 0x0310
        1,      //default_110   adrse2prom 0x0312
        1,      //default_111   adrse2prom 0x0314
        1,      //default_112   adrse2prom 0x0316
        1,      //default_113   adrse2prom 0x0318
        1,      //default_114   adrse2prom 0x031A
        1,      //default_115   adrse2prom 0x031C
        600,    //default_116   adrse2prom 0x031E
        5000,   //default_117   adrse2prom 0x0320
        2200,   //default_118   adrse2prom 0x0322
        1,      //default_119   adrse2prom 0x0324
        1450,   //default_120   adrse2prom 0x0326
        1,      //default_121   adrse2prom 0x0328
        88,     //default_122   adrse2prom 0x032A
        4,      //default_123   adrse2prom 0x032C
        1,      //default_124   adrse2prom 0x032E
        1,      //default_125   adrse2prom 0x0330
        1,      //default_126   adrse2prom 0x0332
        1,      //default_127   adrse2prom 0x0334
        1,      //default_128   adrse2prom 0x0336
        1,      //default_129   adrse2prom 0x0338
        1,      //default_130   adrse2prom 0x033A
        1,      //default_131   adrse2prom 0x033C
        600,    //default_132   adrse2prom 0x033E
};
//------------------------------------------------------------------
const Uint16 Table_value1B[] =
{ 
        5000,   //default_101   adrse2prom 0x0300
        3800,   //default_102   adrse2prom 0x0302
        1,      //default_103   adrse2prom 0x0304
        1450,   //default_104   adrse2prom 0x0306
        1,      //default_105   adrse2prom 0x0308
        88,     //default_106   adrse2prom 0x030A
        4,      //default_107   adrse2prom 0x030C
        1,      //default_108   adrse2prom 0x030E
        1,      //default_109   adrse2prom 0x0310
        1,      //default_110   adrse2prom 0x0312
        1,      //default_111   adrse2prom 0x0314
        1,      //default_112   adrse2prom 0x0316
        1,      //default_113   adrse2prom 0x0318
        1,      //default_114   adrse2prom 0x031A
        1,      //default_115   adrse2prom 0x031C
        600,    //default_116   adrse2prom 0x031E
        5000,   //default_117   adrse2prom 0x0320
        3800,   //default_118   adrse2prom 0x0322
        1,      //default_119   adrse2prom 0x0324
        1450,   //default_120   adrse2prom 0x0326
        1,      //default_121   adrse2prom 0x0328
        88,     //default_122   adrse2prom 0x032A
        4,      //default_123   adrse2prom 0x032C
        1,      //default_124   adrse2prom 0x032E
        1,      //default_125   adrse2prom 0x0330
        1,      //default_126   adrse2prom 0x0332
        1,      //default_127   adrse2prom 0x0334
        1,      //default_128   adrse2prom 0x0336
        1,      //default_129   adrse2prom 0x0338
        1,      //default_130   adrse2prom 0x033A
        1,      //default_131   adrse2prom 0x033C
        600,    //default_132   adrse2prom 0x033E
};
//------------------------------------------------------------------ 
const Uint16 Table_max1A[] =
{
         4000,     //max_f101
         2400,     //max_f102
         5000,     //max_f103
         3600,     //max_f104
         4000,     //max_f105
         95,       //max_f106
         16,       //max_f107
         5000,     //max_f108
         9999,     //max_f109
         9999,     //max_f110
         9999,     //max_f111
         9999,     //max_f112
         9999,     //max_f113
         9999,     //max_f114
         6000,     //max_f115
         9999,     //max_f116
         4000,     //max_f117
         2400,     //max_f118
         5000,     //max_f119
         3600,     //max_f120
         4000,     //max_f121
         95,       //max_f122
         16,       //max_f123
         5000,     //max_f124
         6000,     //max_f125
         6000,     //max_f126
         6000,     //max_f127
         6000,     //max_f128
         6000,     //max_f129
         6000,     //max_f130
         6000,     //max_f131
         9999,     //max_f132
};
//------------------------------------------------------------------
const Uint16 Table_max1B[] =
{
        4000,     //max_f101
        4800,     //max_f102
        5000,     //max_f103
        3600,     //max_f104
        4000,     //max_f105
        95,       //max_f106
        16,       //max_f107
        5000,     //max_f108
        6000,     //max_f109
        6000,     //max_f110
        6000,     //max_f111
        6000,     //max_f112
        6000,     //max_f113
        6000,     //max_f114
        6000,     //max_f115
        9999,     //max_f116
        4000,     //max_f117
        4800,     //max_f118
        5000,     //max_f119
        3600,     //max_f120
        4000,     //max_f121
        95,       //max_f122
        16,       //max_f123
        5000,     //max_f124
        6000,     //max_f125
        6000,     //max_f126
        6000,     //max_f127
        6000,     //max_f128
        6000,     //max_f129
        6000,     //max_f130
        6000,     //max_f131
        9999,     //max_f132
};
//------------------------------------------------------------------ 
const Uint16 Table_min1A[] =
{
        500,      //min_f101
        800,      //min_f102
        1,        //min_f103
        0,        //min_f104
        1,        //min_f105
        50,       //min_f106
        2,        //min_f107
        1,        //min_f108
        1,        //min_f109
        1,        //min_f110
        1,        //min_f111
        1,        //min_f112
        1,        //min_f113
        1,        //min_f114
        1,        //min_f115
        0,        //min_f116
        500,      //min_f117
        800,      //min_f118
        1,        //min_f119
        0,        //min_f120
        1,        //min_f121
        50,       //min_f122
        2,        //min_f123
        1,        //min_f124
        1,        //min_f125
        1,        //min_f126
        1,        //min_f127
        1,        //min_f128
        1,        //min_f129
        1,        //min_f131
        1,        //min_f132
        0,        //min_f133
};
//------------------------------------------------------------------ 
const Uint16 Table_min1B[] =
{
        500,      //min_f101
        1600,     //min_f102
        1,        //min_f103
        0,        //min_f104
        1,        //min_f105
        50,       //min_f106
        2,        //min_f107
        1,        //min_f108
        1,        //min_f109
        1,        //min_f110
        1,        //min_f111
        1,        //min_f112
        1,        //min_f113
        1,        //min_f114
        1,        //min_f115
        0,        //min_f116
        500,      //min_f117
        1600,     //min_f118
        1,        //min_f119
        0,        //min_f120
        1,        //min_f121
        50,       //min_f122
        2,        //min_f123
        1,        //min_f124
        1,        //min_f125
        1,        //min_f126
        1,        //min_f127
        1,        //min_f128
        1,        //min_f129
        1,        //min_f131
        1,        //min_f132
        0,        //min_f133
};
//------------------------------------------------------------------
const Uint16 Table_factor1[] =
{ 
            0x0661,     //factor_101  adrse2prom 0x0400
            0x0551,     //factor_102  adrse2prom 0x0402
            0x0551,     //factor_103  adrse2prom 0x0404
            0x0001,     //factor_104  adrse2prom 0x0406
            0x0261,     //factor_105  adrse2prom 0x0408
            0x02A1,     //factor_106  adrse2prom 0x040A
            0x0002,     //factor_107  adrse2prom 0x040C
            0x0151,     //factor_108  adrse2prom 0x040E
            0x07F1,     //factor_109  adrse2prom 0x0410
            0x07F1,     //factor_110  adrse2prom 0x0412
            0x07F1,     //factor_111  adrse2prom 0x0414
            0x07F1,     //factor_112  adrse2prom 0x0416
            0x07F1,     //factor_113  adrse2prom 0x0418
            0x07F1,     //factor_114  adrse2prom 0x041A
            0x07F1,     //factor_115  adrse2prom 0x041C
            0x0401,     //factor_116  adrse2prom 0x041E
            0x0661,     //factor_117  adrse2prom 0x0420
            0x0551,     //factor_118  adrse2prom 0x0422
            0x0551,     //factor_119  adrse2prom 0x0424
            0x0001,     //factor_120  adrse2prom 0x0426
            0x0261,     //factor_121  adrse2prom 0x0428
            0x02A1,     //factor_122  adrse2prom 0x042A
            0x0002,     //factor_123  adrse2prom 0x042C
            0x0151,     //factor_124  adrse2prom 0x042E
            0x07F1,     //factor_125  adrse2prom 0x0430
            0x07F1,     //factor_126  adrse2prom 0x0432
            0x07F1,     //factor_127  adrse2prom 0x0434
            0x07F1,     //factor_128  adrse2prom 0x0436
            0x07F1,     //factor_129  adrse2prom 0x0438
            0x07F1,     //factor_130  adrse2prom 0x043A
            0x07F1,     //factor_131  adrse2prom 0x043C
            0x0401,     //factor_132  adrse2prom 0x043E
};
//------------------------------------------------------------------
const Uint16 Table_value2[] =
{ 
            0,           //default_201 adrse2prom 0x0500
            1,           //default_202 adrse2prom 0x0502
            2,           //default_203 adrse2prom 0x0504
            10,          //default_204 adrse2prom 0x0506
            11,          //default_205 adrse2prom 0x0508
            12,          //default_206 adrse2prom 0x050A
            3,           //default_207 adrse2prom 0x050C
            15,          //default_208 adrse2prom 0x050E
            1,           //default_209 adrse2prom 0x0510
            1,           //default_210 adrse2prom 0x0512
            1,           //default_211 adrse2prom 0x0514
            1,           //default_212 adrse2prom 0x0516
            1,           //default_213 adrse2prom 0x0518
            1,           //default_214 adrse2prom 0x051A
            1,           //default_215 adrse2prom 0x051C
            4,           //default_216 adrse2prom 0x051E
            4,           //default_217 adrse2prom 0x0520
            1,           //default_218 adrse2prom 0x0522
            0,           //default_219 adrse2prom 0x0524
            0,           //default_220 adrse2prom 0x0526
            0,           //default_221 adrse2prom 0x0528
            0,           //default_222 adrse2prom 0x052A
            20,          //default_223 adrse2prom 0x052C
            0,           //default_224 adrse2prom 0x052E
            0,           //default_225 adrse2prom 0x0530
            100,         //default_226 adrse2prom 0x0532
            100,         //default_227 adrse2prom 0x0534
            5,           //default_228 adrse2prom 0x0536
            20,          //default_229 adrse2prom 0x0538
            0,           //default_230 adrse2prom 0x053A
            0,           //default_231 adrse2prom 0x053C
            100,         //default_232 adrse2prom 0x053E
            100,         //default_233 adrse2prom 0x0540
            5,           //default_234 adrse2prom 0x0542
            20,          //default_235 adrse2prom 0x0544
            40,          //default_236 adrse2prom 0x0546
            0,           //default_237 adrse2prom 0x0548
            200,         //default_238 adrse2prom 0x054A
            100,         //default_239 adrse2prom 0x054C
            40,          //default_240 adrse2prom 0x054E
            0,           //default_241 adrse2prom 0x0550
            1,           //default_242 adrse2prom 0x0552
            4,           //default_243 adrse2prom 0x0554
            0,           //default_244 adrse2prom 0x0556
            0,           //default_245 adrse2prom 0x0558
            1000,        //default_246 adrse2prom 0x055A
            100,         //default_247 adrse2prom 0x055C
            0,           //default_248 adrse2prom 0x055E
            4,           //default_249 adrse2prom 0x0560
            0,           //default_250 adrse2prom 0x0562
            0,           //default_251 adrse2prom 0x0564
            1000,        //default_252 adrse2prom 0x0566
            100,         //default_253 adrse2prom 0x0568
            0,           //default_254 adrse2prom 0x056A
            100,         //default_255 adrse2prom 0x056C
            5000,        //default_256 adrse2prom 0x056E
            100,         //default_257 adrse2prom 0x0570
            5000,        //default_258 adrse2prom 0x0572
            1000,        //default_259 adrse2prom 0x0574
            1000,        //default_260 adrse2prom 0x0576
};
//------------------------------------------------------------------
//------------------------------------------------------------------
const Uint16 Table_min2[] =
{
        0,            //min_f201
        0,            //min_f202
        0,            //min_f203
        0,            //min_f204
        0,            //min_f205
        0,            //min_f206
        0,            //min_f207
        0,            //min_f208
        0,            //min_f209
        0,            //min_f210
        0,            //min_f211
        0,            //min_f212
        0,            //min_f213
        0,            //min_f214
        0,            //min_f215
        0,            //min_f216
        0,            //min_f217
        0,            //min_f218
        0,            //min_f219
        0,            //min_f220
        0,            //min_f221
        0,            //min_f222
        0,            //min_f223
        0,            //min_f224
        0,            //min_f225
        0,            //min_f226
        0,            //min_f227
        0,            //min_f228
        0,            //min_f229
        0,            //min_f230
        0,            //min_f231
        0,            //min_f232
        0,            //min_f233
        0,            //min_f234
        0,            //min_f235
        0,            //min_f236
        0,            //min_f237
        0,            //min_f238
        0,            //min_f239
        0,            //min_f240
        0,            //min_f241
        0,            //min_f242
        0,            //min_f243
        0,            //min_f244
        0,            //min_f245
        0,            //min_f246
        0,            //min_f247
        0,            //min_f248
        0,            //min_f249
        0,            //min_f250
        0,            //min_f251
        0,            //min_f252
        0,            //min_f253
        0,            //min_f254
        0,            //min_f255
        0,            //min_f256
        0,            //min_f257
        1000,         //min_f258
        100,          //min_f259
        1,            //min_f260
}; 
//------------------------------------------------------------------
const Uint16 Table_max2[] =
{
        1,            //max_f201
        20,           //max_f202
        20,           //max_f203
        20,           //max_f204
        20,           //max_f205
        20,           //max_f206
        20,           //max_f207
        20,           //max_f208
        1,            //max_f209
        1,            //max_f210
        1,            //max_f211
        1,            //max_f212
        1,            //max_f213
        1,            //max_f214
        1,            //max_f215
        20,           //max_f216
        22,           //max_f217
        22,           //max_f218
        22,           //max_f219
        1,            //max_f220
        1,            //max_f221
        1,            //max_f222
        9999,         //max_f223
        100,          //max_f224
        300,          //max_f225
        100,          //max_f226
        300,          //max_f227
        100,          //max_f228
        9999,         //max_f229
        100,          //max_f230
        300,          //max_f231
        100,          //max_f232
        300,          //max_f233
        100,          //max_f234
        9999,         //max_f235
        200,          //max_f236
        300,          //max_f237
        200,          //max_f238
        300,          //max_f239
        200,          //max_f240
        6,            //max_f241
        6,            //max_f242
        9999,         //max_f243
        5000,         //max_f244
        100,          //max_f245
        5000,         //max_f246
        100,          //max_f247
        1000,         //max_f248
        9999,         //max_f249
        5000,         //max_f250
        100,          //max_f251
        5000,         //max_f252
        100,          //max_f253
        1000,         //max_f254
        1000,         //max_f255
        4000,         //max_f256
        3000,         //max_f257
        4000,         //max_f258
        1000,         //max_f259
        5000,         //max_f260
};
const Uint16 Table_factor2[] =
{ 
        0x0401,      //factor_201 adrse2prom 0x0600
        0x0401,      //factor_202 adrse2prom 0x0602
        0x0401,      //factor_203 adrse2prom 0x0604
        0x0401,      //factor_204 adrse2prom 0x0606
        0x0401,      //factor_205 adrse2prom 0x0608
        0x0401,      //factor_206 adrse2prom 0x060A
        0x0401,      //factor_207 adrse2prom 0x060C
        0x0401,      //factor_208 adrse2prom 0x060E
        0x0401,      //factor_209 adrse2prom 0x0610
        0x0401,      //factor_210 adrse2prom 0x0612
        0x0401,      //factor_211 adrse2prom 0x0614
        0x0401,      //factor_212 adrse2prom 0x0616
        0x0401,      //factor_213 adrse2prom 0x0618
        0x0401,      //factor_214 adrse2prom 0x061A
        0x0401,      //factor_215 adrse2prom 0x061C
        0x0001,      //factor_216 adrse2prom 0x061E
        0x0401,      //factor_217 adrse2prom 0x0620
        0x0401,      //factor_218 adrse2prom 0x0622
        0x0401,      //factor_219 adrse2prom 0x0624
        0x0401,      //factor_220 adrse2prom 0x0626
        0x0401,      //factor_221 adrse2prom 0x0628
        0x0401,      //factor_222 adrse2prom 0x062A
        0x0001,      //factor_223 adrse2prom 0x062C
        0x0151,      //factor_224 adrse2prom 0x062E
        0x1115,      //factor_225 adrse2prom 0x0630
        0x0151,      //factor_226 adrse2prom 0x0632
        0x1015,      //factor_227 adrse2prom 0x0634
        0x0151,      //factor_228 adrse2prom 0x0636
        0x0001,      //factor_229 adrse2prom 0x0638
        0x1155,      //factor_230 adrse2prom 0x063A
        0x1115,      //factor_231 adrse2prom 0x063C
        0x1155,      //factor_232 adrse2prom 0x063E
        0x1015,      //factor_233 adrse2prom 0x0640
        0x0151,      //factor_234 adrse2prom 0x0642
        0x0001,      //factor_235 adrse2prom 0x0644
        0x0151,      //factor_236 adrse2prom 0x0646
        0x1115,      //factor_237 adrse2prom 0x0648
        0x0151,      //factor_238 adrse2prom 0x064A
        0x1015,      //factor_239 adrse2prom 0x064C
        0x0151,      //factor_240 adrse2prom 0x064E
        0x0001,      //factor_241 adrse2prom 0x0650
        0x0001,      //factor_242 adrse2prom 0x0652
        0x0001,      //factor_243 adrse2prom 0x0654
        0x0151,      //factor_244 adrse2prom 0x0656
        0x0151,      //factor_245 adrse2prom 0x0658
        0x0151,      //factor_246 adrse2prom 0x065A
        0x0151,      //factor_247 adrse2prom 0x065C
        0x0151,      //factor_248 adrse2prom 0x065E
        0x0001,      //factor_249 adrse2prom 0x0660
        0x0151,      //factor_250 adrse2prom 0x0662
        0x0151,      //factor_251 adrse2prom 0x0664
        0x0151,      //factor_252 adrse2prom 0x0666
        0x0151,      //factor_253 adrse2prom 0x0668
        0x0151,      //factor_254 adrse2prom 0x066A
        0x02A1,      //factor_255 adrse2prom 0x066C
        0x0261,      //factor_256 adrse2prom 0x066E
        0x02A1,      //factor_257 adrse2prom 0x0670
        0x0261,      //factor_258 adrse2prom 0x0672
        0x0011,      //factor_259 adrse2prom 0x0674
        0x0151,      //factor_260 adrse2prom 0x0676
};
//------------------------------------------------------------------
const Uint16 Table_value3[] =
{ 
        0,           //default_301 adrse2prom 0x0700
        0,           //default_302 adrse2prom 0x0702
        5000,        //default_303 adrse2prom 0x0704
        500,         //default_304 adrse2prom 0x0706
        500,         //default_305 adrse2prom 0x0708
        25,          //default_306 adrse2prom 0x070A
        15,          //default_307 adrse2prom 0x070C
        5000,        //default_308 adrse2prom 0x070E
        0,           //default_309 adrse2prom 0x0710
        1,           //default_310 adrse2prom 0x0712
        500,         //default_311 adrse2prom 0x0714
        1000,        //default_312 adrse2prom 0x0716
        1500,        //default_313 adrse2prom 0x0718
        2000,        //default_314 adrse2prom 0x071A
        2500,        //default_315 adrse2prom 0x071C
        3000,        //default_316 adrse2prom 0x071E
        4000,        //default_317 adrse2prom 0x0720
        5000,        //default_318 adrse2prom 0x0722
        4000,        //default_319 adrse2prom 0x0724
        3000,        //default_320 adrse2prom 0x0726
        2500,        //default_321 adrse2prom 0x0728
        2000,        //default_322 adrse2prom 0x072A
        1500,        //default_323 adrse2prom 0x072C
        1000,        //default_324 adrse2prom 0x072E
        500,         //default_325 adrse2prom 0x0730
        0,           //default_326 adrse2prom 0x0732
        0,           //default_327 adrse2prom 0x0734
        0,           //default_328 adrse2prom 0x0736
        0,           //default_329 adrse2prom 0x0738
        0,           //default_330 adrse2prom 0x073A
        0,           //default_331 adrse2prom 0x073C
        0,           //default_332 adrse2prom 0x073E
        0,           //default_333 adrse2prom 0x0740
        0,           //default_334 adrse2prom 0x0742
        0,           //default_335 adrse2prom 0x0744
        0,           //default_336 adrse2prom 0x0746
        0,           //default_337 adrse2prom 0x0748
        0,           //default_338 adrse2prom 0x074A
        0,           //default_339 adrse2prom 0x074C
        0,           //default_340 adrse2prom 0x074E
        0,           //default_341 adrse2prom 0x0750
        0,           //default_342 adrse2prom 0x0752
        0,           //default_343 adrse2prom 0x0754
        0,           //default_344 adrse2prom 0x0756
        200,         //default_345 adrse2prom 0x0758
        200,         //default_346 adrse2prom 0x075A
        100,         //default_347 adrse2prom 0x075C
        100,         //default_348 adrse2prom 0x075E
        150,         //default_349 adrse2prom 0x0760
        150,         //default_350 adrse2prom 0x0762
        200,         //default_351 adrse2prom 0x0764
        200,         //default_352 adrse2prom 0x0766
        0,           //default_353 adrse2prom 0x0768
        1,           //default_354 adrse2prom 0x076A
        50,          //default_355 adrse2prom 0x076C
        0,           //default_356 adrse2prom 0x076E
        0,           //default_357 adrse2prom 0x0770
        0,           //default_358 adrse2prom 0x0772
        0,           //default_359 adrse2prom 0x0774
        0,           //default_360 adrse2prom 0x0776
        0,           //default_361 adrse2prom 0x0778
        1,           //default_362 adrse2prom 0x077A
        1,           //default_363 adrse2prom 0x077C
        1,           //default_364 adrse2prom 0x077E
        1,           //default_365 adrse2prom 0x0780
        1,           //default_366 adrse2prom 0x0782
        1,           //default_367 adrse2prom 0x0784
        1,           //default_368 adrse2prom 0x0786
};
//------------------------------------------------------------------
const Uint16 Table_max3[] =
{
        15,       //max_f301
        4000,     //max_f302
        4000,     //max_f303
        4000,     //max_f304
        4000,     //max_f305
        3600,     //max_f306
        3600,     //max_f307
        4000,     //max_f308
        1,        //max_f309
        1,        //max_f310
        4000,     //max_f311
        4000,     //max_f312
        4000,     //max_f313
        4000,     //max_f314
        4000,     //max_f315
        4000,     //max_f316
        4000,     //max_f317
        4000,     //max_f318
        4000,     //max_f319
        4000,     //max_f320
        4000,     //max_f321
        4000,     //max_f322
        4000,     //max_f323
        4000,     //max_f324
        4000,     //max_f325
        1,        //max_f326
        1,        //max_f327
        1,        //max_f328
        1,        //max_f329
        1,        //max_f330
        1,        //max_f331
        1,        //max_f332
        1,        //max_f333
        1,        //max_f334
        1,        //max_f335
        1,        //max_f336
        1,        //max_f337
        1,        //max_f338
        1,        //max_f339
        1,        //max_f340
        1200,     //max_f341
        1200,     //max_f342
        1200,     //max_f343
        1200,     //max_f344
        3600,     //max_f345
        3600,     //max_f346
        3600,     //max_f347
        3600,     //max_f348
        3600,     //max_f349
        3600,     //max_f350
        3600,     //max_f351
        3600,     //max_f352
        3,        //max_f353
        1,        //max_f354
        6000,     //max_f355
        6000,     //max_f356
        6000,     //max_f357
        6000,     //max_f358
        6000,     //max_f359
        6000,     //max_f360
        6000,     //max_f361
        4,        //max_f362
        4,        //max_f363
        4,        //max_f364
        4,        //max_f365
        4,        //max_f366
        4,        //max_f367
        4,        //max_f368
};
//------------------------------------------------------------------
const Uint16 Table_min3[] =
{
        0,        //min_f301
        0,        //min_f302
        1000,     //min_f303
        0,        //min_f304
        0,        //min_f305
        0,        //min_f306
        0,        //min_f307
        0,        //min_f308
        0,        //min_f309
        0,        //min_f310
        0,        //min_f311
        0,        //min_f312
        0,        //min_f313
        0,        //min_f314
        0,        //min_f315
        0,        //min_f316
        0,        //min_f317
        0,        //min_f318
        0,        //min_f319
        0,        //min_f320
        0,        //min_f321
        0,        //min_f322
        0,        //min_f323
        0,        //min_f324
        0,        //min_f325
        0,        //min_f326
        0,        //min_f327
        0,        //min_f328
        0,        //min_f329
        0,        //min_f330
        0,        //min_f331
        0,        //min_f332
        0,        //min_f333
        0,        //min_f334
        0,        //min_f335
        0,        //min_f336
        0,        //min_f337
        0,        //min_f338
        0,        //min_f339
        0,        //min_f340
        0,        //min_f341
        0,        //min_f342
        0,        //min_f343
        0,        //min_f344
        0,        //min_f345
        0,        //min_f346
        0,        //min_f347
        0,        //min_f348
        0,        //min_f349
        0,        //min_f350
        0,        //min_f351
        0,        //min_f352
        0,        //min_f353
        0,        //min_f354
        0,        //min_f355
        0,        //min_f356
        0,        //min_f357
        0,        //min_f358
        0,        //min_f359
        0,        //min_f360
        0,        //min_f361
        1,        //min_f362
        1,        //min_f363
        1,        //min_f364
        1,        //min_f365
        1,        //min_f366
        1,        //min_f367
        1,        //min_f368
}; 
//------------------------------------------------------------------
const Uint16 Table_factor3[] =
{ 
        0x0401,   //factor_301 adrse2prom 0x0800
        0x0261,   //factor_302 adrse2prom 0x0802
        0x0261,   //factor_303 adrse2prom 0x0804
        0x0261,   //factor_304 adrse2prom 0x0806
        0x0261,   //factor_305 adrse2prom 0x0808
        0x0111,   //factor_306 adrse2prom 0x080A
        0x0111,   //factor_307 adrse2prom 0x080C
        0x0261,   //factor_308 adrse2prom 0x080E
        0x0001,   //factor_309 adrse2prom 0x0800
        0x0001,   //factor_310 adrse2prom 0x0812
        0x0261,   //factor_311 adrse2prom 0x0814
        0x0261,   //factor_312 adrse2prom 0x0816
        0x0261,   //factor_313 adrse2prom 0x0818
        0x0261,   //factor_314 adrse2prom 0x081A
        0x0261,   //factor_315 adrse2prom 0x081C
        0x0261,   //factor_316 adrse2prom 0x081E
        0x0261,   //factor_317 adrse2prom 0x0820
        0x0261,   //factor_318 adrse2prom 0x0822
        0x0261,   //factor_319 adrse2prom 0x0824
        0x0261,   //factor_320 adrse2prom 0x0826
        0x0261,   //factor_321 adrse2prom 0x0828
        0x0261,   //factor_322 adrse2prom 0x082A
        0x0261,   //factor_323 adrse2prom 0x082C
        0x0261,   //factor_324 adrse2prom 0x082E
        0x0261,   //factor_325 adrse2prom 0x0830
        0x0001,   //factor_326 adrse2prom 0x0832
        0x0001,   //factor_327 adrse2prom 0x0834
        0x0001,   //factor_328 adrse2prom 0x0836
        0x0001,   //factor_329 adrse2prom 0x0838
        0x0001,   //factor_330 adrse2prom 0x083A
        0x0001,   //factor_331 adrse2prom 0x083C
        0x0001,   //factor_332 adrse2prom 0x083E
        0x0001,   //factor_333 adrse2prom 0x0840
        0x0001,   //factor_334 adrse2prom 0x0842
        0x0001,   //factor_335 adrse2prom 0x0844
        0x0001,   //factor_336 adrse2prom 0x0846
        0x0001,   //factor_337 adrse2prom 0x0848
        0x0001,   //factor_338 adrse2prom 0x084A
        0x0001,   //factor_339 adrse2prom 0x084C
        0x0001,   //factor_340 adrse2prom 0x084E
        0x0151,   //factor_341 adrse2prom 0x0850
        0x0151,   //factor_342 adrse2prom 0x0852
        0x0151,   //factor_343 adrse2prom 0x0854
        0x0151,   //factor_344 adrse2prom 0x0856
        0x0111,   //factor_345 adrse2prom 0x0858
        0x0111,   //factor_346 adrse2prom 0x085A
        0x0111,   //factor_347 adrse2prom 0x085C
        0x0111,   //factor_348 adrse2prom 0x085E
        0x0111,   //factor_349 adrse2prom 0x0860
        0x0111,   //factor_350 adrse2prom 0x0862
        0x0111,   //factor_351 adrse2prom 0x0864
        0x0111,   //factor_352 adrse2prom 0x0866
        0x0001,   //factor_353 adrse2prom 0x0868
        0x0001,   //factor_354 adrse2prom 0x086A
        0x0111,   //factor_355 adrse2prom 0x086C
        0x0111,   //factor_356 adrse2prom 0x086E
        0x0111,   //factor_357 adrse2prom 0x0870
        0x0111,   //factor_358 adrse2prom 0x0872
        0x0111,   //factor_359 adrse2prom 0x0874
        0x0111,   //factor_360 adrse2prom 0x0876
        0x0111,   //factor_361 adrse2prom 0x0878
        0x0001,   //factor_362 adrse2prom 0x087A
        0x0001,   //factor_363 adrse2prom 0x087C
        0x0001,   //factor_364 adrse2prom 0x087E
        0x0001,   //factor_365 adrse2prom 0x0880
        0x0001,   //factor_366 adrse2prom 0x0882
        0x0001,   //factor_367 adrse2prom 0x0884
        0x0001,   //factor_368 adrse2prom 0x0886
};
//------------------------------------------------------------------
const Uint16 Table_value4A[] =
{
        1,       //default_401  adrse2prom 0x0900
        150,     //default_402  adrse2prom 0x0902
        150,     //default_403  adrse2prom 0x0904
        1000,    //default_404  adrse2prom 0x0906
        1000,    //default_405  adrse2prom 0x0908
        1,       //default_406  adrse2prom 0x090A
        3700,    //default_407  adrse2prom 0x090C
        1000,    //default_408  adrse2prom 0x090E
        1000,    //default_409  adrse2prom 0x0900
        1,       //default_410  adrse2prom 0x0912
        3600,    //default_411  adrse2prom 0x0914
        1,       //default_412  adrse2prom 0x0916
        100,     //default_413  adrse2prom 0x0918
        60,      //default_414  adrse2prom 0x091A
        0,       //default_415  adrse2prom 0x091C
        0,       //default_416  adrse2prom 0x091E
        0,       //default_417  adrse2prom 0x0920
        0,       //default_418  adrse2prom 0x0922
        0,       //default_419  adrse2prom 0x0924
        20,      //default_420  adrse2prom 0x0926
        10,      //default_421  adrse2prom 0x0928
        0,       //default_422  adrse2prom 0x092A
        1,       //default_423  adrse2prom 0x092C
        100,     //default_424  adrse2prom 0x092E
        0,       //default_425  adrse2prom 0x0930
        5,       //default_426  adrse2prom 0x0932
        0,       //default_427  adrse2prom 0x0934
        //LU Auto reset enable/disable
        0,       //default_428  adrse2prom 0x0936
};
//------------------------------------------------------------------
const Uint16 Table_value4B[] =
{
        1,       //default_401  adrse2prom 0x0900
        150,     //default_402  adrse2prom 0x0902
        150,     //default_403  adrse2prom 0x0904
        1000,    //default_404  adrse2prom 0x0906
        1000,    //default_405  adrse2prom 0x0908
        1,       //default_406  adrse2prom 0x090A
        7200,    //default_407  adrse2prom 0x090C
        1000,    //default_408  adrse2prom 0x090E
        1000,    //default_409  adrse2prom 0x0900
        1,       //default_410  adrse2prom 0x0912
        6800,    //default_411  adrse2prom 0x0914
        1,       //default_412  adrse2prom 0x0916
        100,     //default_413  adrse2prom 0x0918
        60,      //default_414  adrse2prom 0x091A
        0,       //default_415  adrse2prom 0x091C
        0,       //default_416  adrse2prom 0x091E
        0,       //default_417  adrse2prom 0x0920
        0,       //default_418  adrse2prom 0x0922
        0,       //default_419  adrse2prom 0x0924
        20,      //default_420  adrse2prom 0x0926
        10,      //default_421  adrse2prom 0x0928
        0,       //default_422  adrse2prom 0x092A
        1,       //default_423  adrse2prom 0x092C
        100,     //default_424  adrse2prom 0x092E
        0,       //default_425  adrse2prom 0x0930
        5,       //default_426  adrse2prom 0x0932
        0,       //default_427  adrse2prom 0x0934
        //LU Auto reset enable/disable
        0,       //default_428  adrse2prom 0x0936
};
//------------------------------------------------------------------ 
const Uint16 Table_max4A[] =
{ 
        1,          //max_f401
        200,        //max_f402
        200,        //max_f403
        6000,       //max_f404
        6000,       //max_f405
        1,          //max_f406
        4000,       //max_f407
        6000,       //max_f408
        6000,       //max_f409
        1,          //max_f410
        4000,       //max_f411
        1,          //max_f412
        135,        //max_f413
        600,        //max_f414
        4000,       //max_f415
        4000,       //max_f416
        4000,       //max_f417
        1000,       //max_f418
        1,          //max_f419
        60,         //max_f420
        3600,       //max_f421
        4,          //max_f422
        50,         //max_f423
        3600,       //max_f424
        7,          //max_f425
        20,         //max_f426
        1,          //max_f427
        //LU Auto reset enable/disable
        1,          //max_f428
};
//------------------------------------------------------------------
const Uint16 Table_max4B[] =
{ 
        1,          //max_f401
        200,        //max_f402
        200,        //max_f403
        6000,       //max_f404
        6000,       //max_f405
        1,          //max_f406
        8000,       //max_f407
        6000,       //max_f408
        6000,       //max_f409
        1,          //max_f410
        8000,       //max_f411
        1,          //max_f412
        135,        //max_f413
        600,        //max_f414
        4000,       //max_f415
        4000,       //max_f416
        4000,       //max_f417
        1000,       //max_f418
        1,          //max_f419
        60,         //max_f420
        3600,       //max_f421
        4,          //max_f422
        50,         //max_f423
        3600,       //max_f424
        7,          //max_f425
        20,         //max_f426
        1,          //max_f427
        //LU Auto reset enable/disable
        1,          //max_f428
};
//------------------------------------------------------------------ 
const Uint16 Table_min4A[] =
{ 
        0,          //min_f401
        20,         //min_f402
        20,         //min_f403
        0,          //min_f404
        0,          //min_f405
        0,          //min_f406
        3300,       //min_f407
        0,          //min_f408
        0,          //min_f409
        0,          //min_f410
        3300,       //min_f411
        0,          //min_f412
        20,         //min_f413
        30,         //min_f414
        0,          //min_f415
        0,          //min_f416
        0,          //min_f417
        0,          //min_f418
        0,          //min_f419
        0,          //min_f420
        0,          //min_f421
        0,          //min_f422
        0,          //min_f423
        0,          //min_f424
        0,          //min_f425
        2,          //min_f426
        0,          //min_f427
        0,          //min_f424
        0,          //min_f425
        0,          //min_f426
        0,          //min_f427
//LU Auto reset enable/disable
        0,          //min_f428
}; 
//------------------------------------------------------------------
const Uint16 Table_min4B[] =
{ 
        0,          //min_f401
        20,         //min_f402
        20,         //min_f403
        0,          //min_f404
        0,          //min_f405
        0,          //min_f406
        6600,       //min_f407
        0,          //min_f408
        0,          //min_f409
        0,          //min_f410
        6600,       //min_f411
        0,          //min_f412
        20,         //min_f413
        30,         //min_f414
        0,          //min_f415
        0,          //min_f416
        0,          //min_f417
        0,          //min_f418
        0,          //min_f419
        0,          //min_f420
        0,          //min_f421
        0,          //min_f422
        0,          //min_f423
        0,          //min_f424
        0,          //min_f425
        2,          //min_f426
        0,          //min_f427
        0,          //min_f424
        0,          //min_f425
        0,          //min_f426
        0,          //min_f427
//LU Auto reset enable/disable
        0,          //min_f428
}; 
//------------------------------------------------------------------ 
const Uint16 Table_factor4[] =
{
        0x0001,     //factor_401 adrse2prom 0x0A00
        0x0001,     //factor_402 adrse2prom 0x0A02
        0x0001,     //factor_403 adrse2prom 0x0A04
        0x03B1,     //factor_404 adrse2prom 0x0A06
        0x03B1,     //factor_405 adrse2prom 0x0A08
        0x0001,     //factor_406 adrse2prom 0x0A0A
        0x0151,     //factor_407 adrse2prom 0x0A0C
        0x03B1,     //factor_408 adrse2prom 0x0A0E
        0x03B1,     //factor_409 adrse2prom 0x0A00
        0x0001,     //factor_410 adrse2prom 0x0A12
        0x0151,     //factor_411 adrse2prom 0x0A14
        0x0001,     //factor_412 adrse2prom 0x0A16
        0x0001,     //factor_413 adrse2prom 0x0A18
        0x0001,     //factor_414 adrse2prom 0x0A1A
        0x0261,     //factor_415 adrse2prom 0x0A1C
        0x0261,     //factor_416 adrse2prom 0x0A1E
        0x0261,     //factor_417 adrse2prom 0x0A20
        0x02A1,     //factor_418 adrse2prom 0x0A22
        0x0001,     //factor_419 adrse2prom 0x0A24
        0x0001,     //factor_420 adrse2prom 0x0A26
        0x0111,     //factor_421 adrse2prom 0x0A28
        0x0001,     //factor_422 adrse2prom 0x0A2A
        0x0151,     //factor_423 adrse2prom 0x0A2C
        0x0111,     //factor_424 adrse2prom 0x0E18
        0x0001,     //factor_425 adrse2prom 0x0E1A
        0x0001,     //factor_426 adrse2prom 0x0E18
        0x0001,     //factor_427 adrse2prom 0x0E1A
        //LU Auto reset enable/disable
        0x0001,     //factor_428 adrse2prom 0x0E1C
};
//------------------------------------------------------------------
const Uint16 Table_value5[] =
{ 
            0,     //default_501 adrse2prom 0x0B00
            0,     //default_502 adrse2prom 0x0B02
            20,    //default_503 adrse2prom 0x0B04
            0,     //default_504 adrse2prom 0x0B06
            0,     //default_505 adrse2prom 0x0B08
            0,     //default_506 adrse2prom 0x0B0A
            0,     //default_507 adrse2prom 0x0B0C
            0,     //default_508 adrse2prom 0x0B0E
            0,     //default_509 adrse2prom 0x0B00
            400,   //default_510 adrse2prom 0x0B12
            1,     //default_511 adrse2prom 0x0B14
            1900,  //default_512 adrse2prom 0x0B16
            1900,  //default_513 adrse2prom 0x0B18
            1500,  //default_514 adrse2prom 0x0B1A
            1,     //default_515 adrse2prom 0x0B1C
            3800,  //default_516 adrse2prom 0x0B1E
            3800,  //default_517 adrse2prom 0x0B20
            1500,  //default_518 adrse2prom 0x0B22
            500,   //default_519 adrse2prom 0x0B24
            500,   //default_520 adrse2prom 0x0B26
            0,     //default_521 adrse2prom 0x0B28
            100,   //default_522 adrse2prom 0x0B2A
            0,     //default_523 adrse2prom 0x0B2C
            1000,  //default_524 adrse2prom 0x0B2E
            0,     //default_525 adrse2prom 0x0B30
            0,     //default_526 adrse2prom 0x0B32
            0,     //default_527 adrse2prom 0x0B34
            0,     //default_528 adrse2prom 0x0B36
            4,     //default_529 adrse2prom 0x0B38
            0,     //default_530 adrse2prom 0x0B3A
            0,     //default_531 adrse2prom 0x0B3C
            0,     //default_532 adrse2prom 0x0B3E
            1,     //default_533 adrse2prom 0x0B40
            1,     //default_534 adrse2prom 0x0B42
            200,   //default_535 adrse2prom 0x0B44
            200,   //default_536 adrse2prom 0x0B46
            100,   //default_537 adrse2prom 0x0B48
            0,     //default_538 adrse2prom 0x0B4A
            0,     //default_539 adrse2prom 0x0B4C
            3000,  //default_540 adrse2prom 0x0B4E
            2500,  //default_541 adrse2prom 0x0B51
            0,     //default_541 adrse2prom 0x0B53
};
//------------------------------------------------------------------
const Uint16 Table_max5A[] =
{ 
            4,        //max_f501
            1,        //max_f502
            200,      //max_f503
            4000,     //max_f504
            4000,     //max_f505
            4000,     //max_f506
            2400,     //max_f507
            2400,     //max_f508
            2400,     //max_f509
            1000,     //max_f510
            9999,     //max_f511
            9999,     //max_f512
            9999,     //max_f513
            1000,     //max_f514
            9999,     //max_f515
            9999,     //max_f516
            9999,     //max_f517
            9999,     //max_f518
            9999,     //max_f519
            9999,     //max_f520
            1,        //max_f521
            300,      //max_f522
            600,      //max_f523
            2000,     //max_f524
            2000,     //max_f525
            600,      //max_f526
            1000,     //max_f527
            1,        //max_f528
            15,       //max_f529
            3,        //max_f530
            1,        //max_f531
            1,        //max_f532
            1,        //max_f533
            1,        //max_f534
            3600,     //max_f535
            3600,     //max_f536
            100,      //max_f537
            1,        //max_f538
            1,        //max_f539
            4000,     //max_f540
            4000,     //max_f541
            5,        //max_f542
};
//------------------------------------------------------------------
const Uint16 Table_max5B[] =
{ 
            4,        //max_f501
            1,        //max_f502
            200,      //max_f503
            4000,     //max_f504
            4000,     //max_f505
            4000,     //max_f506
            4800,     //max_f507
            4800,     //max_f508
            4800,     //max_f509
            1000,     //max_f510
            9999,     //max_f511
            9999,     //max_f512
            9999,     //max_f513
            1000,     //max_f514
            9999,     //max_f515
            9999,     //max_f516
            9999,     //max_f517
            9999,     //max_f518
            9999,     //max_f519
            9999,     //max_f520
            1,        //max_f521
            300,      //max_f522
            600,      //max_f523
            2000,     //max_f524
            2000,     //max_f525
            600,      //max_f526
            1000,     //max_f527
            1,        //max_f528
            15,       //max_f529
            3,        //max_f530
            1,        //max_f531
            1,        //max_f532
            1,        //max_f533
            1,        //max_f534
            3600,     //max_f535
            3600,     //max_f536
            100,      //max_f537
            1,        //max_f538
            1,        //max_f539
            4000,     //max_f540
            4000,     //max_f541
            5,        //max_f542
};
//------------------------------------------------------------------ 
const Uint16 Table_min5[] =
{ 
            0,        //min_f501
            0,        //min_f502
            0,        //min_f503
            0,        //min_f504
            0,        //min_f505
            0,        //min_f506
            0,        //min_f507
            0,        //min_f508
            0,        //min_f509
            0,        //min_f510
            0,        //min_f511
            0,        //min_f512
            0,        //min_f513
            0,        //min_f514
            0,        //min_f515
            0,        //min_f516
            0,        //min_f517
            0,        //min_f518
            0,        //min_f519
            0,        //min_f520
            0,        //min_f521
            0,        //min_f522
            0,        //min_f523
            0,        //min_f524
            0,        //min_f525
            0,        //min_f526
            0,        //min_f527
            0,        //min_f528
            1,        //min_f529
            0,        //min_f530
            0,        //min_f531
            0,        //min_f532
            0,        //min_f533
            0,        //min_f534
            0,        //min_f535
            0,        //min_f536
            0,        //min_f537
            0,        //min_f538
            0,        //min_f539
            0,        //min_f540
            0,        //min_f541
            0,        //min_f542
}; 
//------------------------------------------------------------------
const Uint16 Table_factor5[] =
{
            0x0401,      //factor_501 adrse2prom 0x0C00
            0x0401,      //factor_502 adrse2prom 0x0C02
            0x0151,      //factor_503 adrse2prom 0x0C04
            0x0661,      //factor_504 adrse2prom 0x0C06
            0x0661,      //factor_505 adrse2prom 0x0C08
            0x0661,      //factor_506 adrse2prom 0x0C0A
            0x0551,      //factor_507 adrse2prom 0x0C0C
            0x0551,      //factor_508 adrse2prom 0x0C0E
            0x0551,      //factor_509 adrse2prom 0x0C00
            0x03B1,      //factor_510 adrse2prom 0x0C12
            0x0371,      //factor_511 adrse2prom 0x0C14         0x0171
            0x0271,      //factor_512 adrse2prom 0x0C16
            0x3271,      //factor_513 adrse2prom 0x0C18
            0x03B1,      //factor_514 adrse2prom 0x0C1A
            0x0371,      //factor_515 adrse2prom 0x0C1C
            0x0171,      //factor_516 adrse2prom 0x0C1E
            0x3171,      //factor_517 adrse2prom 0x0C20
            0x1261,      //factor_518 adrse2prom 0x0C22
            0x0171,      //factor_519 adrse2prom 0x0C24
            0x3171,      //factor_520 adrse2prom 0x0C26
            0x0001,      //factor_521 adrse2prom 0x0C28
            0x0151,      //factor_522 adrse2prom 0x0C2A
            0x0001,      //factor_523 adrse2prom 0x0C2C
            0x02A1,      //factor_524 adrse2prom 0x0C2E
            0x02A1,      //factor_525 adrse2prom 0x0C30
            0x0151,      //factor_526 adrse2prom 0x0C32
            0x02A1,      //factor_527 adrse2prom 0x0C34
            0x0001,      //factor_528 adrse2prom 0x0C36
            0x0001,      //factor_529 adrse2prom 0x0C38
            0x0001,      //factor_530 adrse2prom 0x0C3A
            0x0001,      //factor_531 adrse2prom 0x0C3C
            0x0401,      //factor_532 adrse2prom 0x0C3E
            0x0401,      //factor_533 adrse2prom 0x0C40
            0x0401,      //factor_534 adrse2prom 0x0C42
            0x0111,      //factor_535 adrse2prom 0x0C44
            0x0111,      //factor_536 adrse2prom 0x0C46
            0x1025,      //factor_537 adrse2prom 0x0C48
            0x0401,      //factor_538 adrse2prom 0x0C4A
            0x0001,      //factor_539 adrse2prom 0x0C4C
            0x0261,      //factor_540 adrse2prom 0x0C4E
            0x0261,      //factor_541 adrse2prom 0x0C51
            0x0001,      //factor_542 adrse2prom 0x0C4C
};
//------------------------------------------------------------------
const Uint16 Table_value6[] =
{ 
            0,      //default_601 adrse2prom 0x0D00
            0,      //default_602 adrse2prom 0x0D02
            100,    //default_603 adrse2prom 0x0D04
            0,      //default_604 adrse2prom 0x0D06
};
//------------------------------------------------------------------
const Uint16 Table_max6[] =
{
        9,                   // max_f601
        1,                   // max_f602
        1000,                // max_f603
        1,                   // max_f604
};
//------------------------------------------------------------------
const Uint16 Table_min6[] =
{
        0,                   // min_f601
        0,                   // min_f602
        0,                   // min_f603
        0,                   // min_f604
};
//------------------------------------------------------------------ 
const Uint16 Table_factor6[] =
{ 
        0x0001,              // factor_601 adrse2prom 0x0E00
        0x0001,              // factor_602 adrse2prom 0x0E02
        0x0261,              // factor_603 adrse2prom 0x0E04
        0x0001,              // factor_603 adrse2prom 0x0E06

};
//------------------------------------------------------------------
const Uint16 Table_value7[] =
{
        0,                  // default_701 adrse2prom
        0,                  // default_702 adrse2prom
        0,                  // default_703 adrse2prom
        0,                  // default_704 adrse2prom
        0,                  // default_705 adrse2prom
        0,                  // default_706 adrse2prom
        1000,               // default_707 adrse2prom
        0,                  // default_708 adrse2prom
        100,                // default_709 adrse2prom
        0,                  // default_710 adrse2prom
        1000,               // default_711 adrse2prom
        0,                  // default_712 adrse2prom
        1000,               // default_713 adrse2prom
        0,                  // default_714 adrse2prom
        0,                  // default_715 adrse2prom
        100,                // default_716 adrse2prom
        0,                  // default_717 adrse2prom
        0,                  // default_718 adrse2prom
        100,                // default_719 adrse2prom
        1,                  // default_720 adrse2prom
        100,                // default_721 adrse2prom
        200,                // default_722 adrse2prom
        300,                // default_723 adrse2prom
        400,                // default_724 adrse2prom
        500,                // default_725 adrse2prom
        600,                // default_726 adrse2prom
        700,                // default_727 adrse2prom
        800,                // default_728 adrse2prom
        900,                // default_729 adrse2prom
        100,                // default_730 adrse2prom
        110,                // default_731 adrse2prom
        120,                // default_732 adrse2prom
        130,                // default_733 adrse2prom
        140,                // default_734 adrse2prom
        150,                // default_735 adrse2prom
};
//------------------------------------------------------------------
const Uint16 Table_max7[] =
{ 
            1,                  //  max_f701
            1,                  //  max_f702
            1,                  //  max_f703
            14,                 //  max_f704
            3600,               //  max_f705
            3600,               //  max_f706
            2000,               //  max_f707
            1,                  //  max_f708
            200,                //  max_f709
            200,                //  max_f710
            2000,               //  max_f711
            1,                  //  max_f712
            6000,               //  max_f713
            6000,               //  max_f714
            6000,               //  max_f715
            150,                //  max_f716
            150,                //  max_f717
            3600,               //  max_f718
            200,                //  max_f719
            1,                  //  max_f720
            200,                //  max_f721
            200,                //  max_f722
            200,                //  max_f723
            200,                //  max_f724
            200,                //  max_f725
            200,                //  max_f726
            200,                //  max_f727
            200,                //  max_f728
            200,                //  max_f729
            200,                //  max_f730
            200,                //  max_f731
            200,                //  max_f732
            200,                //  max_f733
            200,                //  max_f734
            200,                //  max_f735
};
//------------------------------------------------------------------
const Uint16 Table_min7[] =
{
            0,              //  min_f701
            0,              //  min_f702
            0,              //  min_f703
            0,              //  min_f704
            0,              //  min_f705
            0,              //  min_f706
            0,              //  min_f707
            0,              //  min_f708
            0,              //  min_f709
            0,              //  min_f710
            0,              //  min_f711
            0,              //  min_f712
            0,              //  min_f713
            0,              //  min_f714
            0,              //  min_f715
            0,              //  min_f716
            0,              //  min_f717
            0,              //  min_f718
            0,              //  min_f719
            0,              //  min_f720
            0,              //  min_f721
            0,              //  min_f722
            0,              //  min_f723
            0,              //  min_f724
            0,              //  min_f725
            0,              //  min_f726
            0,              //  min_f727
            0,              //  min_f728
            0,              //  min_f729
            0,              //  min_f730
            0,              //  min_f731
            0,              //  min_f732
            0,              //  min_f733
            0,              //  min_f734
            0,              //  min_f735
};
//------------------------------------------------------------------ 
const Uint16 Table_factor7[] =
{
        0x0401,              // factor_701  adrse2prom
        0x0401,              // factor_702  adrse2prom
        0x0401,              // factor_703  adrse2prom
        0x0401,              // factor_704  adrse2prom
        0x0111,              // factor_705  adrse2prom
        0x0111,              // factor_706  adrse2prom
        0x0161,              // factor_707  adrse2prom
        0x0401,              // factor_708  adrse2prom
        0x1025,              // factor_709  adrse2prom
        0x1225,              // factor_710  adrse2prom
        0x0161,              // factor_711  adrse2prom
        0x0401,              // factor_712  adrse2prom
        0x03B1,              // factor_713  adrse2prom
        0x03B1,              // factor_714  adrse2prom
        0x03B1,              // factor_715  adrse2prom
        0x1025,              // factor_716  adrse2prom
        0x1225,              // factor_717  adrse2prom
        0x0111,              // factor_718  adrse2prom
        0x1025,              // factor_719  adrse2prom
        0x0001,              // factor_720  adrse2prom
        0x1125,              // factor_721  adrse2prom
        0x1125,              // factor_722  adrse2prom
        0x1125,              // factor_723  adrse2prom
        0x1125,              // factor_724  adrse2prom
        0x1125,              // factor_725  adrse2prom
        0x1125,              // factor_726  adrse2prom
        0x1125,              // factor_727  adrse2prom
        0x1125,              // factor_728  adrse2prom
        0x1125,              // factor_729  adrse2prom
        0x1025,              // factor_730  adrse2prom
        0x1025,              // factor_731  adrse2prom
        0x1025,              // factor_732  adrse2prom
        0x1025,              // factor_733  adrse2prom
        0x1025,              // factor_734  adrse2prom
        0x1025,              // factor_735  adrse2prom
}; 
//------------------------------------------------------------------
const Uint16 Table_value8[] =
{
           0,               // default_801 adrse2prom 0x1100
           0,               // default_802 adrse2prom 0x1102
           1234,            // default_803 adrse2prom 0x1104
           0,               // default_804 adrse2prom 0x1106
           SWV,             // default_805 adrse2prom 0x1108
           1,               // default_806 adrse2prom 0x110A
};
//------------------------------------------------------------------
const Uint16 Table_max8[] =
{ 
            1,              //  max_f801
            1,              //  max_f802
            9,              //  max_f803
            4,              //  max_f804
            SWV,            //  max_f805
            1,              //  max_f806
};
//------------------------------------------------------------------
const Uint16 Table_min8[] =
{
            0,              //  min_f801
            0,              //  min_f802
            0,              //  min_f803
            0,              //  min_f804
            SWV,            //  min_f805
            0,              //  min_f806
};
//------------------------------------------------------------------ 
const Uint16 Table_factor8[] =
{
        0x0401,              // factor_801  adrse2prom 0x1200
        0x0001,              // factor_802  adrse2prom 0x1202
        0x0801,              // factor_803  adrse2prom 0x1204
        0x0001,              // factor_804  adrse2prom 0x1206
        0x02A0,              // factor_805  adrse2prom 0x1208
        0x0001,              // factor_806  adrse2prom 0x120A
};
//------------------------------------------------------------------ 
struct data_grp{
    Uint16 value[100];
    Uint16 factor[100];
};

struct data_grp grp_num[GrpNumber]; 
//------------------------------------------------------------------
const Uint16 *Pntr_def[]={&Table_value0[0],&Table_value1B[0],   //main pointer selectable pointer will be moved to here
                          &Table_value2[0],&Table_value3[0],
                          &Table_value4B[0],&Table_value5[0],
                          &Table_value6[0],&Table_value7[0],
                          &Table_value8[0]};
//------------------------------------------------------------------
const Uint16 *Pntr_factor[]={&Table_factor0[0],&Table_factor1[0],
                             &Table_factor2[0],&Table_factor3[0],
                             &Table_factor4[0],&Table_factor5[0],
                             &Table_factor6[0],&Table_factor7[0],
                             &Table_factor8[0]};
//------------------------------------------------------------------
const Uint16 *Pntr_max[]={&Table_max0[0],&Table_max1B[0],
                          &Table_max2[0],&Table_max3[0],
                          &Table_max4B[0],&Table_max5B[0],
                          &Table_max6[0],&Table_max7[0],
                          &Table_max8[0]};
//------------------------------------------------------------------ 
const Uint16 *Pntr_min[]={&Table_min0[0],&Table_min1B[0],
                          &Table_min2[0],&Table_min3[0],
                          &Table_min4B[0],&Table_min5[0],
                          &Table_min6[0],&Table_min7[0],
                          &Table_min8[0]};
//------------------------------------------------------------------  
const Uint16 *Pntr_defA[]={&Table_value0[0],&Table_value1A[0],
                          &Table_value2[0],&Table_value3[0],
                          &Table_value4A[0],&Table_value5[0],
                          &Table_value6[0],&Table_value7[0],
                          &Table_value8[0]};
//------------------------------------------------------------------
const Uint16 *Pntr_maxA[]={&Table_max0[0],&Table_max1A[0],
                          &Table_max2[0],&Table_max3[0],
                          &Table_max4A[0],&Table_max5A[0],
                          &Table_max6[0],&Table_max7[0],
                          &Table_max8[0]};
//------------------------------------------------------------------ 
const Uint16 *Pntr_minA[]={&Table_min0[0],&Table_min1A[0],
                          &Table_min2[0],&Table_min3[0],
                          &Table_min4A[0],&Table_min5[0],
                          &Table_min6[0],&Table_min7[0],
                          &Table_min8[0]};
//------------------------------------------------------------------
const Uint16 *Pntr_defB[]={&Table_value0[0],&Table_value1B[0],
                          &Table_value2[0],&Table_value3[0],
                          &Table_value4B[0],&Table_value5[0],
                          &Table_value6[0],&Table_value7[0],
                          &Table_value8[0]};
//------------------------------------------------------------------
const Uint16 *Pntr_maxB[]={&Table_max0[0],&Table_max1B[0],
                          &Table_max2[0],&Table_max3[0],
                          &Table_max4B[0],&Table_max5B[0],
                          &Table_max6[0],&Table_max7[0],
                          &Table_max8[0]};
//------------------------------------------------------------------ 
const Uint16 *Pntr_minB[]={&Table_min0[0],&Table_min1B[0],
                          &Table_min2[0],&Table_min3[0],
                          &Table_min4B[0],&Table_min5[0],
                          &Table_min6[0],&Table_min7[0],
                          &Table_min8[0]};
//------------------------------------------------------------------
/*
const Uint16 *Pntr_HP[]={

}

const Uint16 Table_HPA[]={
                         75,
                         10,
                         15,
                         20,
};

const Uint16 Table_HPB[]={
                         5,
                         1,
                         2,
                         3,
};

const Uint16 Table_factor_HP[]={
                         0x0511,   //factor_004  adrse2prom 0x0206
                         0x0401,   //factor_005  adrse2prom 0x0208
                         0x0401,   //factor_005  adrse2prom 0x0208
                         0x0401,   //factor_005  adrse2prom 0x0208
}
*/
#ifdef __cplusplus
}
#endif

#endif  // 

//===========================================================================
// End of file.
//===========================================================================


