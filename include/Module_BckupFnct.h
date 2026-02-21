
#ifndef MODULE_BCKUPFNCT_H
#define MODULE_BCKUPFNCT_H

//------------------------------------------------------------------
struct BCKUP_TIMER_bit {  
	Uint16 CANUpld:8;	//timer check fail
	Uint16 CANDnld:8;	//timer check fail
} ;

union BCKUP_TIMER {
	Uint16	all;
	struct BCKUP_TIMER_bit bit;
} ;  
//------------------------------------------------------------------
struct DataHbckup_bit {  
	Uint16 U_P:1;	//command uploading=1
} ;

union DataHbckup {
	Uint32	all;
	struct	DataHbckup_bit bit;
} ;  
//------------------------------------------------------------------
struct Bckup_flag_bit {  
	Uint16 UpldSt:2;	//"1"->check baudrate,"2"->run adress and sending
	Uint16 SendDataUpld:1;	//ready to send data upload
	Uint16 AlrdySntUpld:1;	//Already sent the data
	Uint16 UpldFail:1;		//when->no response from backup(bit"send data" not cleared in time)
							//	  ->baudrate!=32us
	Uint16 ScanNode:3;		//scan send out to the node
	Uint16 DnldFail:1;
	Uint16 DnldSt:2;	//"1"->check baudrate,"2"->run adress and sending
	Uint16 SendDataDnld:1;	//ready to send data dnload(adrs of e2prom)
	Uint16 TimeoutDnld:1;	//bit enable counter timeout download
	Uint16 SlvSntData:1;	//=1 slave already sent back the data
} ;

union Bckup_flag {
	Uint16	all;
	struct	Bckup_flag_bit bit;
} ;  
//------------------------------------------------------------------------------------------------------------
struct BCKUP_REG{
	union Bckup_flag	BCKUP_FLAG;
	union BCKUP_TIMER	BCKUP_Timer;
	union DataHbckup	DataH_Bckup;
	Uint32 				MSG_MBX2DataL;//buffer data sent to backup(command)
	Uint32				MSG_MBX2DataH;//buffer data sent to backup(adrs e2prom,data)
	Uint16 				BufferMBX3L;	//data from backup
};
extern struct BCKUP_REG Bckup_reg;

#endif  // 

//===========================================================================
// End of file.
//===========================================================================
	






