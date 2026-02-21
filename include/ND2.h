
#ifndef ND2_H
#define ND2_H

//All Variable For ND2
#include "IQmathLib.h"                 // this header must be first line
#include <math.h>
#include "device.h"
#include "driverlib.h"
#include "Module_ADC.h"                // ADC Registers
#include "KeyPad_Reg.h"
#include "Module_QEP.h"
#include "Module_ChkFault.h"
#include "Module_PID.h"
#include "Main_Variable.h"
#include "Module_DgtlPrt.h"
#include "Module_SttsCtrl.h"			//System status control
#include "Module_AnalogOutput.h"		//Analog output
#include "Module_CheckTemp.h"			//read temperature
#include "Module_CheckIV.h"				//read A2D current U,V,W
#include "Module_CheckVDC.h"			//read A2D VDC bus
#include "Module_Freq.h"				//Group3(freq.) manager
#include "Module_FlyStrt.h"	
#include "Module_FlyStrt.h"	
#include "Module_BckupFnct.h"	//function backup setting of function
#include "Function_BootStrap.h"	
#include "Module_Lift.h"
#include "Function_Autotune.h"
#include "Module_TorqueCmnd.h"


//All Prototype for ND2
#include "ND2_GlobalPrototypes.h"	

//=======Aui================
#include "Module_Vecter.h"	
//#include "VariableDSP2808_Aui.h"
#include "Module_PR.h"
#include "Module_LCD.h"
#endif  // end of DSP280x_DEVICE_H definition

					

//===========================================================================
// End of file.
//===========================================================================
