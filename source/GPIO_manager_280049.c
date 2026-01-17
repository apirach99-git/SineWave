/*
 * GPIO_manager_280049.c
 *
 *  Created on: Dec 29, 2022
 *      Author: Piyasak
 */


#include "F28x_Project.h"
#include "ND2.h"

int First_Test_ND2 = 0;//1 = First Test, 0 = Real Test

void Setup_GPIO(void)
{
/*    //EQEP
    GPIO_setPinConfig(GPIO_56_EQEP1_A);
    GPIO_setPadConfig(56, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(56, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(GPIO_57_EQEP1_B);
    GPIO_setPadConfig(57, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(57, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(GPIO_59_EQEP1_INDEX);
    GPIO_setPadConfig(59, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(59, GPIO_QUAL_SYNC);
*/
    GPIO_setPadConfig(56, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_56_GPIO56);              // GPIO12 = GPIO12
    GPIO_setDirectionMode(56, GPIO_DIR_MODE_OUT);   // GPIO12 = output
    GPIO_writePin(56, 0);

    GPIO_setPadConfig(57, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_57_GPIO57);              // GPIO13 = GPIO13
    GPIO_setDirectionMode(57, GPIO_DIR_MODE_OUT);   // GPIO13 = output
    GPIO_writePin(57, 0);

    GPIO_setPadConfig(59, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_59_GPIO59);              // GPIO16 = GPIO16
    GPIO_setDirectionMode(59, GPIO_DIR_MODE_OUT);   // GPIO16 = output
    GPIO_writePin(59, 0);


    //ECAN1A Section
    GPIO_setPinConfig(GPIO_30_CANA_RX);
    GPIO_setPadConfig(30, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(30, GPIO_QUAL_ASYNC);

    GPIO_setPinConfig(GPIO_31_CANA_TX);
    GPIO_setPadConfig(31, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(31, GPIO_QUAL_ASYNC);

    //I2C1A Section

    GPIO_setPinConfig(GPIO_26_I2CA_SDA);
    GPIO_setPadConfig(26U, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(26U, GPIO_QUAL_ASYNC);

    GPIO_setPinConfig(GPIO_27_I2CA_SCL);
    GPIO_setPadConfig(27U, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(27U, GPIO_QUAL_ASYNC);

    //SC interrupt tripper Section
    GPIO_setPinConfig(GPIO_40_GPIO40);
    GPIO_setPadConfig(40, GPIO_PIN_TYPE_STD|GPIO_PIN_TYPE_PULLUP);
    //GPIO_setQualificationMode(40, GPIO_QUAL_ASYNC); // asynch input
    GPIO_setQualificationMode(40, GPIO_QUAL_6SAMPLE);
    GPIO_setQualificationPeriod(40, 100); // filter 1 us.
    GPIO_setDirectionMode(40, GPIO_DIR_MODE_IN);
    XBAR_setInputPin(XBAR_INPUT1, 40); //Determine the GPIO40 as an input Xbar1

    //Digital Output Port Section

    GPIO_setPadConfig(35, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_35_GPIO35);              // GPIO12 = GPIO12
    GPIO_setDirectionMode(35, GPIO_DIR_MODE_OUT);   // GPIO12 = output

    GPIO_setPadConfig(37, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_37_GPIO37);              // GPIO13 = GPIO13
    GPIO_setDirectionMode(37, GPIO_DIR_MODE_OUT);   // GPIO13 = output

    GPIO_setPadConfig(16, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_16_GPIO16);              // GPIO16 = GPIO16
    GPIO_setDirectionMode(16, GPIO_DIR_MODE_OUT);   // GPIO16 = output


    //Analog Output Section
/*
    GPIO_setPadConfig(6, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO6
    GPIO_setPinConfig(GPIO_6_EPWM4_A);                // GPIO6 = EPWM4A

    GPIO_setPadConfig(8, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO8
    GPIO_setPinConfig(GPIO_8_EPWM5_A);                // GPIO8 = EPWM5A

    GPIO_setPadConfig(8, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO8
    GPIO_setPinConfig(GPIO_8_EPWM5_A);                // GPIO8 = EPWM5A

    GPIO_setPadConfig(24, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO24
    GPIO_setPinConfig(GPIO_24_EPWM8_A);                // GPIO24 = EPWM8A
*/
    //On/Off PWM Section

    GPIO_setPadConfig(33, GPIO_PIN_TYPE_STD);     // Enable pullup on GPIO33
    GPIO_setPinConfig(GPIO_33_GPIO33);                // GPIO33 = GPIO33
    GPIO_setDirectionMode(33, GPIO_DIR_MODE_OUT);    // GPIO33 = output
    GPIO_writePin(33, 0);

    //Fan Section

    GPIO_setPadConfig(22, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_22_GPIO22);                // GPIO22 = GPIO22
    GPIO_setDirectionMode(22, GPIO_DIR_MODE_OUT);    // GPIO22 = output
    GPIO_writePin(22, 0);

    //R Charge C Section

    GPIO_setPadConfig(15, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_15_GPIO15);                // GPIO15 = GPIO15
    GPIO_setDirectionMode(15, GPIO_DIR_MODE_OUT);    // GPIO15 = output
    GPIO_writePin(15, 0);

    //PWM

    GPIO_setPadConfig(0, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO0
    GPIO_setPinConfig(GPIO_0_GPIO0);                // GPIO0 = EPWM1A
    GPIO_setDirectionMode(0, GPIO_DIR_MODE_OUT);    // GPIO0 = output
    while(GpioDataRegs.GPADAT.bit.GPIO0 == 1)
        GpioDataRegs.GPADAT.bit.GPIO0 = 0;//'1'ON '0'OFF

    GPIO_setPadConfig(1, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO15
    GPIO_setPinConfig(GPIO_1_GPIO1);                // GPIO15 = GPIO15
    GPIO_setDirectionMode(1, GPIO_DIR_MODE_OUT);    // GPIO15 = output
    while(GpioDataRegs.GPADAT.bit.GPIO1 == 1)
        GpioDataRegs.GPADAT.bit.GPIO1 = 0;//'1'ON '0'OFF

    GPIO_setPadConfig(2, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO15
    GPIO_setPinConfig(GPIO_2_GPIO2);                // GPIO15 = GPIO15
    GPIO_setDirectionMode(2, GPIO_DIR_MODE_OUT);    // GPIO15 = output
    while(GpioDataRegs.GPADAT.bit.GPIO2 == 1)
        GpioDataRegs.GPADAT.bit.GPIO2 = 0;//'1'ON '0'OFF

    GPIO_setPadConfig(3, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO15
    GPIO_setPinConfig(GPIO_3_GPIO3);                // GPIO15 = GPIO15
    GPIO_setDirectionMode(3, GPIO_DIR_MODE_OUT);    // GPIO15 = output
    while(GpioDataRegs.GPADAT.bit.GPIO3 == 1)
        GpioDataRegs.GPADAT.bit.GPIO3 = 0;//'1'ON '0'OFF

    GPIO_setPadConfig(4, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO15
    GPIO_setPinConfig(GPIO_4_GPIO4);                // GPIO15 = GPIO15
    GPIO_setDirectionMode(4, GPIO_DIR_MODE_OUT);    // GPIO15 = output
    while(GpioDataRegs.GPADAT.bit.GPIO4 == 1)
        GpioDataRegs.GPADAT.bit.GPIO4 = 0;//'1'ON '0'OFF

    GPIO_setPadConfig(5, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO15
    GPIO_setPinConfig(GPIO_5_GPIO5);                // GPIO15 = GPIO15
    GPIO_setDirectionMode(5, GPIO_DIR_MODE_OUT);    // GPIO15 = output
    while(GpioDataRegs.GPADAT.bit.GPIO5 == 1)
        GpioDataRegs.GPADAT.bit.GPIO5 = 0;//'1'ON '0'OFF

//------------------------------------------------------------------------------------

//====================================================================================
/*Select Port under development
Traditional :
            GPIO27 ======> Dynamic Brake
            GPIO26 ======> Output On / Off
            GPIO24 ======> NON

            GPIO28 ======> MOD_RX
            GPIO29 ======> MOD_TX

Frist test :
            GPIO27 ======> SPISTEB  (DAC)
            GPIO26 ======> SPICLKB  (DAC)
            GPIO24 ======> SPISIMOB (DAC)

            GPIO28 ======> Dynamic Brake
            GPIO29 ======> Output On / Off
*/
//====================================================================================

    //-----------------------------R Break------------------------------------------------

    GPIO_setPadConfig(14, GPIO_PIN_TYPE_PULLUP);    // Enable pullup on GPIO14
    GPIO_setPinConfig(GPIO_14_GPIO14);              // GPIO14 = GPIO14
    GPIO_setDirectionMode(14, GPIO_DIR_MODE_OUT);   // GPIO14 = output
    GPIO_writePin(14, 1);                           // Load output latch
    //-----------------------------R Break------------------------------------------------

    //--------------------------on/off output---------------------------------------------

    GPIO_setPadConfig(39, GPIO_PIN_TYPE_PULLUP);    // Enable pullup on GPIO39
    GPIO_setPinConfig(GPIO_39_GPIO39);              // GPIO39 = GPIO39
    GPIO_setDirectionMode(39, GPIO_DIR_MODE_OUT);   // GPIO39 = output
    //--------------------------on/off output---------------------------------------------

    //Digital Input

    GPIO_setPadConfig(6, GPIO_PIN_TYPE_PULLUP);    // Enable pullup on GPIO6
    GPIO_setPinConfig(GPIO_6_GPIO6);              // GPIO6 = GPIO6
    GPIO_setDirectionMode(6, GPIO_DIR_MODE_IN);   // GPIO6 = input

    GPIO_setPadConfig(7, GPIO_PIN_TYPE_PULLUP);    // Enable pullup on GPIO7
    GPIO_setPinConfig(GPIO_7_GPIO7);              // GPIO7 = GPIO7
    GPIO_setDirectionMode(7, GPIO_DIR_MODE_IN);   // GPIO7 = input

    GPIO_setPadConfig(17, GPIO_PIN_TYPE_PULLUP);    // Enable pullup on GPIO17
    GPIO_setPinConfig(GPIO_17_GPIO17);              // GPIO17 = GPIO17
    GPIO_setDirectionMode(17, GPIO_DIR_MODE_IN);   // GPIO17 = input

    GPIO_setPadConfig(23, GPIO_PIN_TYPE_PULLUP);    // Enable pullup on GPIO23
    GPIO_setPinConfig(GPIO_23_GPIO23);              // GPIO23 = GPIO23
    GPIO_setDirectionMode(23, GPIO_DIR_MODE_IN);   // GPIO23 = input

    GPIO_setPadConfig(25, GPIO_PIN_TYPE_PULLUP);    // Enable pullup on GPIO25
    GPIO_setPinConfig(GPIO_25_GPIO25);              // GPIO25 = GPIO25
    GPIO_setDirectionMode(25, GPIO_DIR_MODE_IN);   // GPIO25 = input

    GPIO_setPadConfig(34, GPIO_PIN_TYPE_PULLUP);    // Enable pullup on GPIO34
    GPIO_setPinConfig(GPIO_34_GPIO34);              // GPIO34 = GPIO34
    GPIO_setDirectionMode(34, GPIO_DIR_MODE_IN);   // GPIO34 = input

    GPIO_setPadConfig(58, GPIO_PIN_TYPE_PULLUP);    // Enable pullup on GPIO58
    GPIO_setPinConfig(GPIO_58_GPIO58);              // GPIO58 = GPIO58
    GPIO_setDirectionMode(58, GPIO_DIR_MODE_IN);   // GPIO58 = input

    EDIS;
}
/////////////////////////////////////////////////////////////////////////
/*
    Analog port information
    -----------------------
    A0  (AIN0/Con1P1)-->Iu
    B0  (AIN1/Con1P2)-->AIN-12(i/p port)
    A1  (AIN2/Con1P3)-->Iv(big)/Iw(small)
    B1  (AIN3/Con1P4)-->AIN-A2(i/p port)
    A2  (AIN4/Con1P5)-->Iw(big)
    B2  (AIN5/Con1P6)-->AIN-A3(i/p port)
    A3  (AIN6/Con1P7)-->VDC
    B3  (AIN7/Con1P8)-->
    A4  (AIN8/Con1P11)-->
    B4  (AIN9/Con1P12)-->
    A5  (AIN10/Con1P13)-->Temperature1
    B5  (AIN11/Con1P14)-->Temperature2
    A6  (AIN12/Con1P15)-->Temperature3
    B6  (AIN13/Con1P16)-->Temperature4
    A7  (AIN14/Con1P17)-->
    B7  (AIN15/Con1P18)-->

    GPIO Information
    ---------------------
    GPIO0-->PWM-U
    GPIO1-->PWM-X
    GPIO2-->PWM-V
    GPIO3-->PWM-Y
    GPIO4-->PWM-W
    GPIO5-->PWM-Z
    GPIO6-->AOUT-FMA
    GPIO7-->DOUT
    GPIO8-->AOUT-AOUT
    GPIO9-->DOUT
    GPIO10-->DOUT
    GPIO11-->On/Off PWM
    GPIO12-->SC
    GPIO13-->DIN
    GPIO14-->DIN
    GPIO15-->DIN
    GPIO16-->DIN
    GPIO17-->DIN
    GPIO18-->FAN,BootLD
    GPIO19-->DIN
    GPIO20-->QEPA
    GPIO21-->QEPB
    GPIO22-->DIN
    GPIO23-->QEPI
    GPIO24-->SPI For test***
    GPIO25-->-
    GPIO26-->On/off DOUT,SPI For test***
    GPIO27-->RBrk
    GPIO28-->RBrk for test***
    GPIO29-->On/off O/P for test***,BootLD
    GPIO30-->CAN
    GPIO31-->CAN
    GPIO32-->I2C
    GPIO33-->I2C
    GPIO34-->RChrgC,BootLD

        GPIO0-->PWM-U
    GPIO1-->PWM-X
    GPIO2-->PWM-V
    GPIO3-->PWM-Y
    GPIO4-->PWM-W
    GPIO5-->PWM-Z
    GPIO6-->AOUT-FMA
    GPIO16-->DOUT
    GPIO8-->AOUT-AOUT
    GPIO12-->DOUT
    GPIO13-->DOUT
    GPIO33-->On/Off PWM
    GPIO40-->SC
    GPIO17-->DIN
    GPIO58-->DIN
    GPIO25-->DIN
    GPIO34-->DIN
    GPIO23-->DIN
    GPIO22-->FAN,BootLD
    GPIO7-->DIN
    GPIO56-->QEPA
    GPIO57-->QEPB
    GPIO6-->DIN
    GPIO59-->QEPI
    GPIO25-->-
    GPIO14-->RBrk
    GPIO29-->On/off O/P for test***,BootLD
    GPIO30-->CAN
    GPIO31-->CAN
    GPIO26-->I2C
    GPIO27-->I2C
    GPIO15-->RChrgC,BootLD
*/
