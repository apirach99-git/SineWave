$word = New-Object -ComObject Word.Application
$word.Visible = $false
$doc = $word.Documents.Add()
$selection = $word.Selection

function Write-Header($text) {
    $selection.ParagraphFormat.Alignment = 0
    $selection.Font.Size = 14
    $selection.Font.ColorIndex = 2 # Blue
    $selection.Font.Bold = 1
    $selection.TypeText($text)
    $selection.TypeParagraph()
}

function Write-Text($text) {
    $selection.ParagraphFormat.Alignment = 0
    $selection.Font.Size = 12
    $selection.Font.ColorIndex = 0 # Auto
    $selection.Font.Bold = 0
    $selection.TypeText($text)
    $selection.TypeParagraph()
}

function Write-Code($title, $code) {
    $selection.ParagraphFormat.Alignment = 0
    $selection.Font.Size = 10
    $selection.Font.Bold = 1
    $selection.Font.ColorIndex = 0
    $selection.TypeText($title)
    $selection.TypeParagraph()
    
    $selection.Font.Name = "Consolas"
    $selection.Font.Bold = 0
    $selection.Font.Size = 9
    $selection.TypeText($code)
    $selection.TypeParagraph()
    $selection.TypeParagraph()
    $selection.Font.Name = "Calibri" # Reset font
}

$selection.ParagraphFormat.Alignment = 1
$selection.Font.Size = 16
$selection.Font.Bold = 1
$selection.TypeText("สรุปการแก้ไข Phase 2: การเปลี่ยนตัวแปรกลุ่ม RMS และ Peak เป็น Floating Point")
$selection.TypeParagraph()
$selection.TypeParagraph()

Write-Text("เอกสารนี้สรุปการทำงานใน Phase 2 ซึ่งมีเป้าหมายหลักเพื่อเปลี่ยนชนิดตัวแปรจัดเก็บค่ากระแสและแรงดัน (RMS และ Peak) จากเดิม _iq ให้กลายเป็น float มาตรฐาน รวมถึงการแก้ไข Code ในไฟล์ต่างๆ ที่เกี่ยวข้องเพื่อให้ระบบทำงานร่วมกันได้อย่างถูกต้อง")
$selection.TypeParagraph()

Write-Header("1. ไฟล์ Module_CheckIV.h")
Write-Text("แก้ไขโครงสร้างตัวแปรของ IV_READ_REG โดยเปลี่ยน I_Peak, I_rms, Vout_Rms และ Vin_Rms เป็น float")
Write-Code("โค้ดเดิม (เก่า):", "struct IV_READ_REG{`n    _iq I_Peak;`n    _iq I_rms;`n    _iq Vout_Rms;`n    _iq Vin_Rms;`n};")
Write-Code("โค้ดใหม่:", "struct IV_READ_REG{`n    float I_Peak;`n    float I_rms;`n    float Vout_Rms;`n    float Vin_Rms;`n};")

Write-Header("2. ไฟล์ Module_CheckIV.c")
Write-Text("ปรับปรุงฟังก์ชันเพื่อกำหนดค่าลงตัวแปร float แบบตรงๆ โดยไม่ต้องแปลงเป็น _iq ก่อน")
Write-Code("โค้ดเดิม (เก่า):", "IV_Read_reg.Vout_Rms = _IQ17(buff_float);`nIV_Read_reg.I_rms = _IQ17div(_IQ17mpy(buff_iq,107020),_IQ17(1.4142));")
Write-Code("โค้ดใหม่:", "IV_Read_reg.Vout_Rms = buff_float;`nIV_Read_reg.I_rms = buff_float / 1.41421356237f;")

Write-Header("3. ไฟล์ Module_AnalogOutput.c")
Write-Text("ฟังก์ชัน SelectAOUT() ต้องแปลงค่า float กลับไปเป็น _iq โดยคูณด้วย 131072.0f เพื่อนำไปใช้กับระบบ Output")
Write-Code("โค้ดเดิม (เก่า):", "buffdataiq = _IQ17mpy(IV_Read_reg.I_Peak, Analog_reg.GainAOUT1);")
Write-Code("โค้ดใหม่:", "buffdataiq = _IQ17mpy((_iq)(IV_Read_reg.I_Peak * 131072.0f), Analog_reg.GainAOUT1);")

Write-Header("4. ไฟล์ Function_V_PER_F.c")
Write-Text("ปรับการคำนวณกำลังไฟฟ้า (Power) ให้เรียกใช้ float โดยตรง และใช้วิธี Casting แทนเมื่อต้องการแปลงกลับไปเป็น _iq")
Write-Code("โค้ดเดิม (เก่า):", "buffdata = _IQ17mpy(IV_Read_reg.Vout_Rms, MainIQ_Variable.I_Rate);")
Write-Code("โค้ดใหม่:", "buffdata = _IQ17mpy((_iq)(IV_Read_reg.Vout_Rms * 131072.0f), MainIQ_Variable.I_Rate);")

Write-Header("5. ไฟล์ KeyPad.c")
Write-Text("แปลงค่าในขั้นตอนการนำข้อมูลไปตั้งค่าหน้าจอ เพื่อให้สามารถแสดงผลบน Panel ได้สำเร็จ")
Write-Code("โค้ดเดิม (เก่า):", "ar_buffdisp[1] = IV_Read_reg.I_Peak;`nar_buffdisp[3] = IV_Read_reg.Vout_Rms;")
Write-Code("โค้ดใหม่:", "ar_buffdisp[1] = (_iq)(IV_Read_reg.I_Peak * 131072.0f);`nar_buffdisp[3] = (_iq)(IV_Read_reg.Vout_Rms * 131072.0f);")

Write-Header("6. ไฟล์ Module_ChkFault.c")
Write-Text("เปลี่ยนมาตรวจสอบค่า Over Current (OC) จาก ChkFault_Reg.I_Fault ซึ่งเป็นแบบ _iq แทนตัวแปรเดิมที่เป็น float ไปแล้ว ทำให้หลีกเลี่ยงข้อผิดพลาดเรื่อง Type Mismatch ได้")
Write-Code("โค้ดเดิม (เก่า):", "if(IV_Read_reg.I_Peak < _IQ17(1.95))`n{ ... }`nelse if(IV_Read_reg.BuffIp >= _IQ17(2.25))")
Write-Code("โค้ดใหม่:", "if(ChkFault_Reg.I_Fault < _IQ17(1.95))`n{ ... }`nelse if(ChkFault_Reg.I_Fault >= _IQ17(2.00))")

Write-Header("7. ไฟล์ Module_FlyStrt.c")
Write-Text("เนื่องจากการคำนวณใน Flying Start ทำงานบน IQ Math จึงต้องเพิ่มการ Casting จาก float เป็น _iq")
Write-Code("โค้ดเดิม (เก่า):", "FlyStrt_Reg.IFly_K_1 = IV_Read_reg.I_Peak;`nif(IV_Read_reg.I_Peak>FlyStrt_Reg.IFly_K_1)")
Write-Code("โค้ดใหม่:", "FlyStrt_Reg.IFly_K_1 = (_iq)(IV_Read_reg.I_Peak * 131072.0f);`nif((_iq)(IV_Read_reg.I_Peak * 131072.0f)>FlyStrt_Reg.IFly_K_1)")

Write-Header("8. ไฟล์ Function_AutoTune.c")
Write-Text("ตัวควบคุมกระแสในลูป AutoTune ถูกเปลี่ยนชั่วคราวให้ใช้ Isrms (ที่มีอยู่เป็น _iq) แทน IV_Read_reg.I_rms (ที่ถูกเปลี่ยนเป็น float แล้ว)")
Write-Code("โค้ดเดิม (เก่า):", "Err = AutoTuneCtrlRegs.Isref - IV_Read_reg.I_rms; //Q17")
Write-Code("โค้ดใหม่:", "Err = AutoTuneCtrlRegs.Isref - Isrms; //Q17")

$doc.SaveAs("e:\OneDrive\1\Workspace_v20_1\SineWave\Phase2_Summary_TH.docx")
$doc.Close()
$word.Quit()
[System.Runtime.Interopservices.Marshal]::ReleaseComObject($word) | Out-Null
