//completed initial MSGCTRL=0x00000000 by poiter and remove CANB
//completed setting DLC= by poiter
//cmpleted : bit rate = 250kbps
//completed : connect to keypad ND1 LCD
//completed : config DBO=1 (data=01234567)
//2012-5-31 : moved setup GPIO into main
//2012-9-26 : add reset & set again for eCAN module
//2012-9-26 : move eCAN user function to KeyPad.c
//2012-9-26 : completed select boud rate CAN
//2013-1-17 : imported all CAN functions to this file
//2013-1-17 : use common function by bit to send out the message
//2013-1-17 : use common function by structure mailbox
//2013-1-17 : reset bit connect CAN when reset module
//2013-1-22 : add bit protection portout eCAN "eCANTxBusy"
//2013-1-23 : use setup RX mailbox with common function
//2013-1-23 : move initintecana,initrx into init ecana
//2013-1-23 : revise bit MBNR with J
//2013-1-23 : revise bit CDR process
//2013-1-24 : revised data length is argument
//2013-1-24 : can sent out the data with ID=0x711
//2013-1-24 : block reset module CAN baudrate when uploading
//2013-1-25 : send all data of function via CAN
//2013-1-28 : use sub-fuction interrupt eCAN0
//2013-1-29 : revise checking condition uoploadfail
//2013-1-30 : completed timeout function mailbox3
//2013-1-31 : debugged sending the upload data only one time after run adrs
//2013-2-1 : revised algorithm send out CAN by scan
//2013-2-1 : can write all adrs with start baudrate = 32us
//2013-2-1 : remove time with CAN module and move reload time connect keypad to RMP03
//2013-2-4 : revised data lenght of mailbox of backup =8(command)
//2013-2-5 : get request slave to send back the data
//2013-2-6 : revised scan node=0 when connect keypad
//2013-2-6 : remove scan node=0 when connect keypad
//2013-2-6 : revised receive CAN RMP0
//2013-2-15 : move & revise CAN module
//2013-2-15 : checking change rate by bit system ready
//2022-12-9 : The CAN module has been intialized by the TI-Driver(F280049)


/*
 * Module_CANGus.c
 *
 *  Created on: Nov 24, 2022
 *      Author: Piyasak
 */

#include "F28x_Project.h"
#include "ND2.h"
#include "f28004x_sw_prioritized_isr_levels.h"


//---External variable ------------------------------------------
struct data_grp{
    Uint16 value[100];
    Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber];
extern Uint32 MSG_MBX1DataL, MSG_MBX1DataH, BufferMBX0L;
extern Uint16 txMSG49[8];
extern Uint16 BackUpTxMSG49[8];
extern Uint16 rxMSG49[8];

volatile uint32_t errorFlag = 0;

uint16_t txMsgCount = 0;
uint16_t rxMsgCount = 0;


uint16_t txUploadMsgCount = 0;
uint16_t rxDwnloadMsgCount = 0;



uint32_t status, mailbox, mailboxtest;

uint32_t *IDRxKEYPAD_pointer,
         *IDTxKEYPAD_pointer,
         *IDTxUpload_pointer,
         *IDRxDwnload_pointer;

uint32_t IDRxKEYPAD_Value  = IDRxKeypad,
         IDTxKEYPAD_Value  = IDTxKeypad,
         IDTxUpload_Value  = IDTxUpload,
         IDRxDwnload_Value = IDRxDnload;

Uint32 BuffTxMessageH, BuffTxMessageL;


//-----------------------------------------------------------------
void CANA_SetUp(void)        // Initialize eCAN-A module
{
    KeyPad_reg.TimeoutKeyPad = 0;

    CAN_initModule(CANA_BASE);

    //GPIO_setPinConfig(GPIO_30_CANA_RX);
    //GPIO_setPinConfig(GPIO_31_CANA_TX);

    //CAN_clearGlobalInterruptStatus(CANA_BASE, CAN_GLOBAL_INT_CANINT0);
    //CAN_clearGlobalInterruptStatus(CANA_BASE, CAN_GLOBAL_INT_CANINT1);

    CAN_enableInterrupt(CANA_BASE, CAN_INT_IE0| CAN_INT_ERROR |CAN_INT_STATUS);
    //CAN_enableInterrupt(CANA_BASE, CAN_INT_IE1| CAN_INT_ERROR |CAN_INT_STATUS);

    ClearMBox();//initial function for MSGCTRL=0x00000000

    //CAN_setBitRate(CANA_BASE, DEVICE_SYSCLK_FREQ, 250000, 16);//*

    switch(KeyPad_reg.GENERAL_FLAG.bit.BuffBRP)
    {
        case 1:
            CAN_setBitRate(CANA_BASE, DEVICE_SYSCLK_FREQ, 31250, 25);
            //CAN_setBitTiming(CANA_BASE, 0, 159, 1, 6, 1);
            /*CanaRegs.CAN_BTR.bit.BRPE = 159;
            CanaRegs.CAN_BTR.bit.TSEG2 = 6;
            CanaRegs.CAN_BTR.bit.TSEG1 = 1;*/
            break;
        case 2:
            CAN_setBitRate(CANA_BASE, DEVICE_SYSCLK_FREQ, 62500, 25);
            //CAN_setBitTiming(CANA_BASE, 0, 79, 1, 6, 1);
            /*CanaRegs.CAN_BTR.bit.BRPE = 79;
            CanaRegs.CAN_BTR.bit.TSEG2 = 6;
            CanaRegs.CAN_BTR.bit.TSEG1 = 1;*/
            break;
        case 3:
            CAN_setBitRate(CANA_BASE, DEVICE_SYSCLK_FREQ, 125000, 10);
            //CAN_setBitTiming(CANA_BASE, 0, 39, 1, 6, 1);
            /*CanaRegs.CAN_BTR.bit.BRPE = 39;
            CanaRegs.CAN_BTR.bit.TSEG2 = 6;
            CanaRegs.CAN_BTR.bit.TSEG1 = 1;*/
            break;
        case 4:
            CAN_setBitRate(CANA_BASE, DEVICE_SYSCLK_FREQ, 250000, 10);
            //CAN_setBitTiming(CANA_BASE, 0, 159, 1, 6, 1);
            /*CanaRegs.CAN_BTR.bit.BRPE = 159;
            CanaRegs.CAN_BTR.bit.TSEG2 = 6;
            CanaRegs.CAN_BTR.bit.TSEG1 = 1;*/
            break;
        default :
            CAN_setBitRate(CANA_BASE, DEVICE_SYSCLK_FREQ, 125000, 10);
            //CAN_setBitTiming(CANA_BASE, 0, 159, 15, 2, 1);
            /*CanaRegs.CAN_BTR.bit.BRPE = 159;
            CanaRegs.CAN_BTR.bit.TSEG2 = 6;
            CanaRegs.CAN_BTR.bit.TSEG1 = 1;*/
    }

    CanaRegs.CAN_CTL.bit.DAR = 1;

    CAN_setupMessageObject(CANA_BASE, 1, IDRxKeypad, CAN_MSG_FRAME_STD, CAN_MSG_OBJ_TYPE_RX,
                           0, CAN_MSG_OBJ_RX_INT_ENABLE, 8);
    CAN_setupMessageObject(CANA_BASE, 2, IDTxKeypad, CAN_MSG_FRAME_STD, CAN_MSG_OBJ_TYPE_TX,
                           0, CAN_MSG_OBJ_NO_FLAGS, 8);
    CAN_setupMessageObject(CANA_BASE, 4, IDTxUpload, CAN_MSG_FRAME_STD, CAN_MSG_OBJ_TYPE_TX,
                           0, CAN_MSG_OBJ_NO_FLAGS, 8);
    CAN_setupMessageObject(CANA_BASE, 8, IDRxDnload, CAN_MSG_FRAME_STD, CAN_MSG_OBJ_TYPE_RX,
                           0, CAN_MSG_OBJ_RX_INT_ENABLE, 8);


    CanaRegs.CAN_GLB_INT_EN.bit.GLBINT0_EN = 1;
    //CanaRegs.CAN_GLB_INT_EN.bit.GLBINT1_EN = 1;
/*
    IDRxKEYPAD_pointer  = &IDRxKEYPAD_Value;
    IDTxKEYPAD_pointer  = &IDTxKEYPAD_Value;
    IDTxUpload_pointer  = &IDTxUpload_Value;
    IDRxDwnload_pointer = &IDRxDwnload_Value;
*/
    CAN_startModule(CANA_BASE);
}
void ClearMBox (void)
{
    CAN_clearMessage(CANA_BASE, 1);
    CAN_clearMessage(CANA_BASE, 2);
    CAN_clearMessage(CANA_BASE, 4);
    CAN_clearMessage(CANA_BASE, 8);
}
void eCANController(void)
{

    if(ND2System_Reg.FLAG_SYST_CTRL.bit.Syst_ready == 1)
    {
        if(KeyPad_reg.GENERAL_FLAG.bit.BuffBRP != grp_num[0].value[8])//check CAN bus rate
        {
            KeyPad_reg.GENERAL_FLAG.bit.BuffBRP = grp_num[0].value[8];
            ResetModuleCAN();
        }
    }

    if(KeyPad_reg.TimeoutKeyPad >= TimeOuteCAN)
    {
        ResetModuleCAN();
    }

    else
    {
        KeyPad_reg.TimeoutKeyPad++;
    }

    if(Bckup_reg.BCKUP_FLAG.bit.SendDataUpld==1)
    {
        if(Bckup_reg.BCKUP_Timer.bit.CANUpld >= TimeOuteCAN)
        {
            Bckup_reg.BCKUP_FLAG.bit.UpldFail = 1;
        }
        else
        {
            Bckup_reg.BCKUP_Timer.bit.CANUpld++;
        }
    }

    if(Bckup_reg.BCKUP_FLAG.bit.TimeoutDnld==1)
    {

        Bckup_reg.BCKUP_FLAG.bit.TimeoutDnld = 0;

        if(Bckup_reg.BCKUP_Timer.bit.CANDnld>=TimeOuteCAN)
        {
            Bckup_reg.BCKUP_FLAG.bit.DnldFail = 1;
            Bckup_reg.BCKUP_Timer.bit.CANDnld = 0;
        }
        else
        {
            Bckup_reg.BCKUP_Timer.bit.CANDnld++;
        }
    }
    else
    {
        Bckup_reg.BCKUP_Timer.bit.CANDnld = 0;
    }
}
void ResetModuleCAN(void)
{
    union CAN_CTL_REG CAN_CTL_SHADOW;
    EALLOW;
    CAN_CTL_SHADOW.all     = CanaRegs.CAN_CTL.all;
    CAN_CTL_SHADOW.bit.SWR = 1;
    CanaRegs.CAN_CTL.all   = CAN_CTL_SHADOW.all;
    EDIS;

    DisableDog();
    CANA_SetUp();        // Initialize CAN-A module
    SetUpDog();

    txMsgCount = 0;
    rxMsgCount = 0;
    txUploadMsgCount = 0;
    rxDwnloadMsgCount = 0;

    KeyPad_reg.GENERAL_FLAG.bit.ConnectKeyPad = 0;

}
void SendoutCAN(void)
{

    switch(KeyPad_reg.GENERAL_FLAG.bit.ScanNode)
    {
        case 0://Check status is ready?

            if (rxMsgCount == txMsgCount)
            //if (MsgReadCmplt == 1)
            {

                txMSG49[7] = MSG_MBX1DataH>>24;
                txMSG49[6] = MSG_MBX1DataH>>16  & 0x00FF;
                txMSG49[5] = MSG_MBX1DataH>>8   & 0x00FF;
                txMSG49[4] = MSG_MBX1DataH      & 0x00FF;
                txMSG49[3] = MSG_MBX1DataL>>24;
                txMSG49[2] = MSG_MBX1DataL>>16  & 0x00FF;
                txMSG49[1] = MSG_MBX1DataL>>8   & 0x00FF;
                txMSG49[0] = MSG_MBX1DataL      & 0x00FF;

                //PortOutMBX(MSG_MBX1DataL,MSG_MBX1DataH,0x00000002);
                //sendCANMessage(2, MSG_MBX1DataL, MSG_MBX1DataH);
                //sendCANMessage(2, 0x000A2702, 0x00000000);
                CAN_sendMessage(CANA_BASE, 2, 8, txMSG49);
                txMsgCount++;

                KeyPad_reg.GENERAL_FLAG.bit.ScanNode++;
            }

/*
            txMSG49[7] = MSG_MBX1DataH>>24;
            txMSG49[6] = MSG_MBX1DataH>>16 & 0x00FF;
            txMSG49[5] = MSG_MBX1DataH>>8 & 0x00FF;
            txMSG49[4] = MSG_MBX1DataH & 0x00FF;
            txMSG49[3] = MSG_MBX1DataL>>24;
            txMSG49[2] = MSG_MBX1DataL>>16 & 0x00FF;
            txMSG49[1] = MSG_MBX1DataL>>8 & 0x00FF;
            txMSG49[0] = MSG_MBX1DataL & 0x00FF;

            //PortOutMBX(MSG_MBX1DataL,MSG_MBX1DataH,0x00000002);
            //sendCANMessage(2, MSG_MBX1DataL, MSG_MBX1DataH);
            //sendCANMessage(2, 0x000A2702, 0x00000000);
            CAN_sendMessage(CANA_BASE, 2, 8, txMSG49);
            txMsgCount++;

            KeyPad_reg.GENERAL_FLAG.bit.ScanNode++;
*/
            break;
        case 1:
            //CAN_clearInterruptStatus(CANA_BASE, 4);
/*
            if (txUploadMsgCount == rxDwnloadMsgCount)
            //if (MsgDwnloadCmplt == 1)
            {

                BackUpTxMSG49[7] = Bckup_reg.MSG_MBX2DataH>>24;
                BackUpTxMSG49[6] = Bckup_reg.MSG_MBX2DataH>>16  & 0x00FF;
                BackUpTxMSG49[5] = Bckup_reg.MSG_MBX2DataH>>8   & 0x00FF;
                BackUpTxMSG49[4] = Bckup_reg.MSG_MBX2DataH      & 0x00FF;
                BackUpTxMSG49[3] = Bckup_reg.MSG_MBX2DataL>>24;
                BackUpTxMSG49[2] = Bckup_reg.MSG_MBX2DataL>>16  & 0x00FF;
                BackUpTxMSG49[1] = Bckup_reg.MSG_MBX2DataL>>8   & 0x00FF;
                BackUpTxMSG49[0] = Bckup_reg.MSG_MBX2DataL      & 0x00FF;

                if(Bckup_reg.BCKUP_FLAG.bit.SendDataUpld==1 && Bckup_reg.BCKUP_FLAG.bit.AlrdySntUpld==0)        //the system need send upload data
                {
                    Bckup_reg.BCKUP_FLAG.bit.AlrdySntUpld = 1;

                    //PortOutMBX(Bckup_reg.MSG_MBX2DataL,Bckup_reg.MSG_MBX2DataH,0x00000004);
                    //sendCANMessage(4, Bckup_reg.MSG_MBX2DataL, Bckup_reg.MSG_MBX2DataH);
                    CAN_sendMessage(CANA_BASE, 4, 8, BackUpTxMSG49);

                    txUploadMsgCount++;
                }
                else if(Bckup_reg.BCKUP_FLAG.bit.SendDataDnld == 1)
                {
                    Bckup_reg.BCKUP_FLAG.bit.SendDataDnld = 0;
                    //sendCANMessage(4, Bckup_reg.MSG_MBX2DataL, Bckup_reg.MSG_MBX2DataH);
                    CAN_sendMessage(CANA_BASE, 4, 8, BackUpTxMSG49);
                    txUploadMsgCount++;
                }
                KeyPad_reg.GENERAL_FLAG.bit.ScanNode = 0;
            }*/

            BackUpTxMSG49[7] = Bckup_reg.MSG_MBX2DataH>>24;
            BackUpTxMSG49[6] = Bckup_reg.MSG_MBX2DataH>>16  & 0x00FF;
            BackUpTxMSG49[5] = Bckup_reg.MSG_MBX2DataH>>8   & 0x00FF;
            BackUpTxMSG49[4] = Bckup_reg.MSG_MBX2DataH      & 0x00FF;
            BackUpTxMSG49[3] = Bckup_reg.MSG_MBX2DataL>>24;
            BackUpTxMSG49[2] = Bckup_reg.MSG_MBX2DataL>>16  & 0x00FF;
            BackUpTxMSG49[1] = Bckup_reg.MSG_MBX2DataL>>8   & 0x00FF;
            BackUpTxMSG49[0] = Bckup_reg.MSG_MBX2DataL      & 0x00FF;

            if(Bckup_reg.BCKUP_FLAG.bit.SendDataUpld==1 /*&& Bckup_reg.BCKUP_FLAG.bit.AlrdySntUpld==0*/)        //the system need send upload data
            {
                Bckup_reg.BCKUP_FLAG.bit.AlrdySntUpld = 1;

                //PortOutMBX(Bckup_reg.MSG_MBX2DataL,Bckup_reg.MSG_MBX2DataH,0x00000004);
                //sendCANMessage(4, Bckup_reg.MSG_MBX2DataL, Bckup_reg.MSG_MBX2DataH);
                CAN_sendMessage(CANA_BASE, 4, 8, BackUpTxMSG49);

                //txUploadMsgCount++;
            }
            else if(Bckup_reg.BCKUP_FLAG.bit.SendDataDnld == 1)
            {
                Bckup_reg.BCKUP_FLAG.bit.SendDataDnld = 0;
                //sendCANMessage(4, Bckup_reg.MSG_MBX2DataL, Bckup_reg.MSG_MBX2DataH);
                CAN_sendMessage(CANA_BASE, 4, 8, BackUpTxMSG49);
                //txUploadMsgCount++;
            }
            KeyPad_reg.GENERAL_FLAG.bit.ScanNode = 0;

            break;
    }
}
void interrupt_ecan1(void)
{

    //uint32_t status;

    IER &= MINT9;
    EINT;


    status = CAN_getInterruptCause(CANA_BASE);

    if(status == CAN_INT_INT0ID_STATUS)
    {
        //
        // Read the controller status.  This will return a field of status
        // error bits that can indicate various errors.  Error processing
        // is not done in this example for simplicity.  Refer to the
        // API documentation for details about the error status bits.
        // The act of reading this status will clear the interrupt.
        //
        status = CAN_getStatus(CANA_BASE);

        //
        // Check to see if an error occurred.
        //
        if(((status  & ~(CAN_STATUS_RXOK)) != CAN_STATUS_LEC_MSK) &&
           ((status  & ~(CAN_STATUS_RXOK)) != CAN_STATUS_LEC_NONE))
        {
            //
            // Set a flag to indicate some errors may have occurred.
            //
            errorFlag = 1;
        }
    }

    //--------------Receive section--------------------------------------------------
    else if(status == 1)//Mailbox from keypad
    {
        //status = 1;
        //getCANMessage(1, BufferMBX0L, 0);
        CAN_readMessage(CANA_BASE, 1, rxMSG49);
        //CAN_readMessageWithID(CANA_BASE, 1, CAN_MSG_FRAME_STD, IDRxKEYPAD_pointer, rxMSG49);

        BufferMBX0L = ((Uint32)rxMSG49[3]<<24|
                       (Uint32)rxMSG49[2]<<16|
                       (Uint32)rxMSG49[1]<<8 |
                       (Uint32)rxMSG49[0]);

        rxMsgCount++;

        KeyPad_reg.TimeoutKeyPad = 0;
        if(KeyPad_reg.GENERAL_FLAG.bit.ConnectKeyPad==0)
        {
            KeyPad_reg.GENERAL_FLAG.bit.ConnectKeyPad = 1;
            KeyPad_reg.GENERAL_FLAG.bit.ScanNode      = 0;
        }

        CAN_clearInterruptStatus(CANA_BASE, 1);
    }
    else if (status == 8)
    {
        //status = 8;
        rxDwnloadMsgCount++;

        if(Bckup_reg.BCKUP_FLAG.bit.UpldSt!=0)
        {//system is uploading
            Bckup_reg.BCKUP_FLAG.bit.SendDataUpld = 0;
            Bckup_reg.BCKUP_Timer.bit.CANUpld     = 0;
        }
        else if(Bckup_reg.BCKUP_FLAG.bit.DnldSt!=0)
        {//system is downloading
            Bckup_reg.BCKUP_FLAG.bit.SlvSntData = 1;
        }

        CAN_clearInterruptStatus(CANA_BASE, 8);
    }
/*
    //--------------Transmit section-------------------------------------------------
        else if(status == 2)//Mailbox to keypad
            {
                //status = 2;

                CAN_clearInterruptStatus(CANA_BASE, 2);
                txMsgCount++;
            }
        else if(status == 4)//Mailbox to keypad
            {
                //status = 4;

                CAN_clearInterruptStatus(CANA_BASE, 4);
                txUploadMsgCount++;
            }
*/
            //CAN_clearGlobalInterruptStatus(CANA_BASE, CAN_GLOBAL_INT_CANINT0);


}
/////////////////////////////////////////////////////////////////

/*
void getCANMessage(uint32_t objID, uint32_t msgDATAL, uint32_t msgDATAH)
{
    union CAN_IF2CMD_REG CAN_IF2CMD_SHADOW;

    //
    // Set the Message Data A, Data B, and control values to be read
    // on request for data from the message object.
    //
    CAN_IF2CMD_SHADOW.all = 0;
    CAN_IF2CMD_SHADOW.bit.Control = 1;
    CAN_IF2CMD_SHADOW.bit.DATA_A  = 1;
    CAN_IF2CMD_SHADOW.bit.DATA_B  = 1;

    //
    // Transfer the message object to the message object IF register.
    //
    CAN_IF2CMD_SHADOW.bit.MSG_NUM = objID;
    CanaRegs.CAN_IF2CMD.all = CAN_IF2CMD_SHADOW.all;

    //
    // Wait for busy bit to clear.
    //
    while(CanaRegs.CAN_IF2CMD.bit.Busy)
    {
    }

    //
    // See if there is new data available.
    //
    if(CanaRegs.CAN_IF2MCTL.bit.NewDat == 1)
    {
        //
        // Read out the data from the CAN registers.
        //

        CanaRegs.CAN_IF2DATA.all = msgDATAL;
        CanaRegs.CAN_IF2DATB.all = msgDATAH;

        //
        // Populate Shadow Variable
        //
        CAN_IF2CMD_SHADOW.all = CanaRegs.CAN_IF2CMD.all;

        //
        // Clear New Data Flag
        //
        CAN_IF2CMD_SHADOW.bit.TxRqst = 1;

        //
        // Transfer the message object to the message object IF register.
        //
        CAN_IF2CMD_SHADOW.bit.MSG_NUM = objID;
        CanaRegs.CAN_IF2CMD.all = CAN_IF2CMD_SHADOW.all;
    }
}

void sendCANMessage(uint32_t objID, uint32_t msgDATAL, uint32_t msgDATAH)
{
    //
    // Use Shadow variable for IF1CMD. IF1CMD should be written to in
    // single 32-bit write.
    //
    union CAN_IF1CMD_REG CAN_IF1CMD_SHADOW;

    //
    // Wait for busy bit to clear.
    //
    while(CanaRegs.CAN_IF1CMD.bit.Busy)
    {
    }

    //
    // Write data to transfer into DATA-A and DATA-B interface registers
    //

    CanaRegs.CAN_IF1DATA.all = msgDATAL;
    CanaRegs.CAN_IF1DATB.all = msgDATAH;

    //
    // Set Direction to write and set DATA-A/DATA-B to be transfered to
    // message object
    //
    CAN_IF1CMD_SHADOW.all = 0;
    CAN_IF1CMD_SHADOW.bit.DIR    = 1;
    CAN_IF1CMD_SHADOW.bit.DATA_A = 1;
    CAN_IF1CMD_SHADOW.bit.DATA_B = 1;

    //
    // Set Tx Request Bit
    //
    CAN_IF1CMD_SHADOW.bit.TXRQST = 1;

    //
    // Transfer the message object to the message object specified by
    // objID.
    //
    CAN_IF1CMD_SHADOW.bit.MSG_NUM = objID;
    CanaRegs.CAN_IF1CMD.all = CAN_IF1CMD_SHADOW.all;
}
*/
