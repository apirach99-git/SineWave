/**
 * main.c
 * ND2 Project
 * CPU: TMS320F280049C
 *
 * Piyasak Kranprakon (GUS)
 */
// Test Push to GitHub
#include "F28x_Project.h"
#include "DSP280x_I2C_defines.h"
#include "ND2.h"
#include "VariableDSP2808_Aui.h"

struct MAIN_VARIABLE MainIQ_Variable={0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

struct STALL_VOLTAGE Parameter_stall_V={0,0};

struct DataMotor DataMotor1={0,0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0};

struct DataMotor DataMotor2={0,0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0};

struct DataMotor *CurrentMotor = &DataMotor1;

extern int First_Test_ND2;

volatile bool lcd_refresh_req = false;
uint8_t addr;
uint8_t carry = 0;
uint8_t next_carry;
uint8_t count1=1;
uint16_t test1,ZRequest;
float test;
extern VrmsCalc_t Vrms_In;
extern VrmsCalc_t Vrms_Out;



void main(void)
{

    while(MainIQ_Variable.DelayReset<100000)
    {
        MainIQ_Variable.DelayReset++;
    }

    Device_init();

    Device_initGPIO();

    InitSysCtrl();
    Setup_GPIO();       //Setup all GPIO

    InitPieCtrl();
    IER=0;
    IFR=0;
    InitPieVectTable(); //assign interrupt vector to address ISR

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;

    //Enable Peripheral interrupt
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;      //enable interrupt ADCA
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;      //enable interrupt Timer0 loop 1ms

    PieCtrlRegs.PIEIER2.bit.INTx1 = 1;      //enable interrupt TZ1(int2.1)
    PieCtrlRegs.PIEIER2.bit.INTx2 = 1;
    PieCtrlRegs.PIEIER2.bit.INTx3 = 1;

    PieCtrlRegs.PIEIER3.bit.INTx6 = 1;      //enable PWM6 loop 200us

    PieCtrlRegs.PIEIER5.bit.INTx1 = 1;      //enable interrupt QEP1

    PieCtrlRegs.PIEIER8.bit.INTx1 = 1;      //enable interrupt I2C1A

    PieCtrlRegs.PIEIER9.bit.INTx5 = 1;      //enable interrupt CANA0
    //PieCtrlRegs.PIEIER9.bit.INTx6 = 1;      //enable interrupt CANA1

    //-----------------------------------------------------

    //Clear Peripheral interrupt
    PieCtrlRegs.PIEACK.all = (M_INT1|M_INT2|M_INT3|M_INT5|M_INT8|M_INT9);//only enable used interrupt peripheral

    //Enable CPU register
    IER |= (M_INT1|M_INT2|M_INT3|M_INT5|M_INT8|M_INT9);//only enable used interrupt peripheral
    EINT;   // Enable Global interrupt INTM
    //Peripheral Setup
    TimerInterrupt();    //setup timer0
    CANA_SetUp();        // Initialize eCAN-A module
    I2C_Init();
    ADC_Setup();
    SCFault_Setup();
    AnalogOutSetup();
    SetUpDog();          //Setup Watchdog
   // eQEP_Init();
    ePWM_Setup();
    initE2Prom();

    HT1621_Init();      // config GPIO + ��Դ HT1621 + clear ��
    LCD_Init();
    //Enable 5V supply for DOUT
    GpioDataRegs.GPBSET.bit.GPIO39 = 1;

    GpioDataRegs.GPBSET.bit.GPIO33 = 0;

    LCD_ShowInput3(12.3);
    LCD_ShowCenter2('A','B');
    LCD_ShowOutput3(678);


    test = 0;
    Vrms_Init(&Vrms_In,  200);
    Vrms_Init(&Vrms_Out, 200);

    for(;;)
    {
        ServiceDog();
        Record_fault();         //1    check new fault and save to e2prom
        Normal_mode();      //2    run normal mode i/p=key up/down
        CheckKeyIn();           //3    which key is pressed and up keystate
        CheckMode();            //4    check mode whether normal or setting mode,entry with state norm_qck(0)
        DataDisp();              //5    arrange data to keypad
        SelectFunct();          //6    run key on normal mode function group1-7,entry only setting mode
        SettingSubFNC();     //7    entry only setting mode and >=sub function

        DisplayNormal();
        Chk_Fault();
        Chk_FaultTimer();               //40sec Fault Timer

  //      LCD_ShowInput3(test);
  //      LCD_ShowOutput3(test1);
    //    LCD_ZoneRequest(ZRequest);

        // �ء � 50 ms: update �� 1 �����
       if (lcd_refresh_req)
        {  test=test+0.001;
            lcd_refresh_req = false;
        //    HT1621_RefreshAll();    // �� DINT/EINT ���� � ����� WriteData4() ���� addr
     //       LCD_ZoneRequest(LCD_ZONE_INPUT);
            LCD_Service();
        }
    }

}

