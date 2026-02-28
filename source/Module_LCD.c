#include "F28x_Project.h"
#include "include\ND2.h"
#include "f28004x_sw_prioritized_isr_levels.h"

#include "include\Module_LCD.h"

#define Ts 0.001/10

#define PI 3.14159265358979

#define Freq 50



#define HT1621_MAX_ADDR   32U   /* ปกติ 32; แก้ตามจริงถ้าต่ำกว่าหรือสูงกว่า */

/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

/*
 * Shadow RAM: ตัวแปรนี้เก็บค่า segment ทุก address
 * - ใช้ร่วมกับ HT1621_RefreshAll() เพื่อเขียนคืนทั้งจอ
 * - คุณสามารถแก้ ht1621_ram[] ในโค้ดส่วนอื่น แล้วให้ slow-loop เรียก RefreshAll() เป็นช่วง ๆ
 */
uint8_t ht1621_ram[HT1621_MAX_ADDR] = {0U};

//---External variable import--------------------------------------
struct data_grp{
    Uint16 value[100];
    Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber]; 

/* =========================
 *  Internal types & macros
 * ========================= */
typedef struct {
    uint8_t addr;   // 0..31
    uint8_t bit;    // 0..3  (COM1..COM4)
} LcdSeg;

typedef struct {
    LcdSeg A, B, C, D, E, F, G;
    bool   hasDP;
    LcdSeg DP;
} LcdDigitMap;


static void ht1621_write_bits_msb(uint16_t data, uint8_t len);
static void ht1621_write_bits_lsb(uint8_t data, uint8_t len);
void LCD_WriteSeg(LcdSeg seg, uint8_t on);


#define LS(a,b)   ((LcdSeg){ (uint8_t)(a), (uint8_t)(b) })

#define SEG_A   (1u << 0)
#define SEG_B   (1u << 1)
#define SEG_C   (1u << 2)
#define SEG_D   (1u << 3)
#define SEG_E   (1u << 4)
#define SEG_F   (1u << 5)
#define SEG_G   (1u << 6)
#define SEG_DP  (1u << 7)

/* =========================
 *  Zone request flag
 * ========================= */
static volatile uint16_t g_zone_req = 0;

/* =========================
 *  7-seg font table
 * ========================= */
static uint8_t font7seg(int8_t v)
{
    if(v >= 0 && v <= 9)
    {
        static const uint8_t tbl[10] = {
            (SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F),                //0
            (SEG_B|SEG_C),                                                             //1
            (SEG_A|SEG_B|SEG_D|SEG_E|SEG_G),                          //2
            (SEG_A|SEG_B|SEG_C|SEG_D|SEG_G),                          //3
            (SEG_B|SEG_C|SEG_F|SEG_G),                                      //4
            (SEG_A|SEG_C|SEG_D|SEG_F|SEG_G),                          //5
            (SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G),               //6
            (SEG_A|SEG_B|SEG_C),                                                 //7
            (SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G),    //8
            (SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G),               //9
        };
        return tbl[(uint8_t)v];
    }

    if(v >= 'A' && v <= 'Z')
    {
        static const uint8_t tbl_alpha[26] = {
            (SEG_A|SEG_B|SEG_C|SEG_E|SEG_F|SEG_G),        // A
            (SEG_C|SEG_D|SEG_E|SEG_F|SEG_G),              // B (b)
            (SEG_A|SEG_D|SEG_E|SEG_F),                    // C
            (SEG_B|SEG_C|SEG_D|SEG_E|SEG_G),              // D (d)
            (SEG_A|SEG_D|SEG_E|SEG_F|SEG_G),              // E
            (SEG_A|SEG_E|SEG_F|SEG_G),                    // F
            (SEG_A|SEG_C|SEG_D|SEG_E|SEG_F),              // G
            (SEG_B|SEG_C|SEG_E|SEG_F|SEG_G),              // H
            (SEG_B|SEG_C),                                // I
            (SEG_B|SEG_C|SEG_D|SEG_E),                    // J
            (SEG_D|SEG_E|SEG_F|SEG_G),                    // K (same as H/t? used t here as placeholder or specific)
            (SEG_D|SEG_E|SEG_F),                          // L
            (SEG_A|SEG_C|SEG_E),                          // M
            (SEG_C|SEG_E|SEG_G),                          // N (n)
            (SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F),        // O
            (SEG_A|SEG_B|SEG_E|SEG_F|SEG_G),              // P
            (SEG_A|SEG_B|SEG_C|SEG_F|SEG_G),              // Q (q)
            (SEG_E|SEG_G),                                // R (r)
            (SEG_A|SEG_C|SEG_D|SEG_F|SEG_G),              // S (5)
            (SEG_D|SEG_E|SEG_F|SEG_G),                    // T (t)
            (SEG_B|SEG_C|SEG_D|SEG_E|SEG_F),              // U
            (SEG_C|SEG_D|SEG_E),                          // V (u/v)
            (SEG_C|SEG_D|SEG_E|SEG_G),                    // W (approx)
            (SEG_B|SEG_C|SEG_E|SEG_F|SEG_G),              // X (H)
            (SEG_B|SEG_C|SEG_D|SEG_F|SEG_G),              // Y (y)
            (SEG_A|SEG_B|SEG_D|SEG_E|SEG_G)               // Z (2)
        };
        return tbl_alpha[v - 'A'];
    }
    if(v == -2) return (SEG_G); // '-'
    return 0;                   // blank
}

/* =========================
 *  Digit mapping (จากตารางที่คุณส่ง)
 * =========================
 * Digit1: 1F/1G/1E/1D @addr2, 1A/1B/1C @addr3, DP=D1 @addr3 COM4
 * Digit2: 2F/2G/2E/2D @addr4, 2A/2B/2C @addr5, DP=D2 @addr5 COM4
 * Digit3: 3F/3G/3E/3D @addr6, 3A/3B/3C @addr7
 * Digit4: 4F/4G/4E/4D @addr10,4A/4B/4C @addr11,DP=D3 @addr11 COM4
 * Digit5: 5F/5G/5E/5D @addr12,5A/5B/5C @addr13
 * Digit6: 6F/6G/6E/6D @addr16,6A/6B/6C @addr17,DP=D4 @addr17 COM4
 * Digit7: 7F/7G/7E/7D @addr18,7A/7B/7C @addr19,DP=D5 @addr19 COM4
 * Digit8: 8F/8G/8E/8D @addr20,8A/8B/8C @addr21
 */

static const LcdDigitMap DIG1 = {
    .A = LS(3,0), .B = LS(3,1), .C = LS(3,2),
    .D = LS(2,3), .E = LS(2,2), .F = LS(2,0),
    .G = LS(2,1),
    .hasDP = true, .DP = LS(3,3)
};

static const LcdDigitMap DIG2 = {
    .A = LS(5,0), .B = LS(5,1), .C = LS(5,2),
    .D = LS(4,3), .E = LS(4,2), .F = LS(4,0), .G = LS(4,1),
    .hasDP = true, .DP = LS(5,3)
};

static const LcdDigitMap DIG3 = {
    .A = LS(7,0), .B = LS(7,1), .C = LS(7,2),
    .D = LS(6,3), .E = LS(6,2), .F = LS(6,0), .G = LS(6,1),
    .hasDP = false, .DP = LS(0,0)
};

static const LcdDigitMap DIG4 = {
    .A = LS(11,0), .B = LS(11,1), .C = LS(11,2),
    .D = LS(10,3), .E = LS(10,2), .F = LS(10,0), .G = LS(10,1),
    .hasDP = true, .DP = LS(11,3)
};

static const LcdDigitMap DIG5 = {
    .A = LS(13,0), .B = LS(13,1), .C = LS(13,2),
    .D = LS(12,3), .E = LS(12,2), .F = LS(12,0), .G = LS(12,1),
    .hasDP = false, .DP = LS(0,0)
};

static const LcdDigitMap DIG6 = {
    .A = LS(17,0), .B = LS(17,1), .C = LS(17,2),
    .D = LS(16,3), .E = LS(16,2), .F = LS(16,0), .G = LS(16,1),
    .hasDP = true, .DP = LS(17,3)
};

static const LcdDigitMap DIG7 = {
    .A = LS(19,0), .B = LS(19,1), .C = LS(19,2),
    .D = LS(18,3), .E = LS(18,2), .F = LS(18,0), .G = LS(18,1),
    .hasDP = true, .DP = LS(19,3)
};

static const LcdDigitMap DIG8 = {
    .A = LS(21,0), .B = LS(21,1), .C = LS(21,2),
    .D = LS(20,3), .E = LS(20,2), .F = LS(20,0), .G = LS(20,1),
    .hasDP = false, .DP = LS(0,0)
};

static const LcdDigitMap* getDigitMap(uint8_t digit_id)
{
    switch(digit_id)
    {
        case 1: return &DIG1;
        case 2: return &DIG2;
        case 3: return &DIG3;
        case 4: return &DIG4;
        case 5: return &DIG5;
        case 6: return &DIG6;
        case 7: return &DIG7;
        case 8: return &DIG8;
        default: return &DIG1;
    }
}

/* =========================
 *  Low-level helpers
 * ========================= */
static inline void segWrite(const LcdSeg *s, bool on)
{
    uint8_t m = (uint8_t)(1U << s->bit);
    if(on)  ht1621_ram[s->addr] |=  m;
    else    ht1621_ram[s->addr] &= (uint8_t)~m;
}

void LCD_Update(void)
{
    segWrite(&LCDi_INV,1);
    segWrite(&LCDi_Hz,1);
}




static void refresh_range(uint8_t s, uint8_t e)
{
    uint8_t a;
    for(a = s; a <= e; a++)
        HT1621_WriteData4(a, (uint8_t)(ht1621_ram[a] & 0x0F));
}

/* =========================
 *  Public: Zone
 * ========================= */
void LCD_ZoneRequest(LcdZone z)
{
    g_zone_req |= (uint16_t)(1U << (uint16_t)z);
}


// Static update functions for each zone
static void LCD_Update_Input_Zone_Data(void)
{
    
    segWrite(&LCDi_INPUT, 1);
    // Update Input Zone (Digit 1-3)
    // Example: Show Input Voltage (Vrms_In)
    LCD_ShowInput3(Vrms_In.rms);
    segWrite(&LCDi_V, 1); // Turn on 'V' icon
}

static void LCD_Update_Center_Zone_Data(void)
{
    // Update Center Zone (Digit 4-5)
    // Example: Show state or dummy value
    // LCD_ShowCenter2(state_tens, state_ones);

    segWrite(&LCDc_Line, 1);

}

static void LCD_Update_Output_Zone_Data(void)
{
    segWrite(&LCDo_OUTPUT, 1);
    // Update Output Zone (Digit 6-8)
    // Example: Show Output Voltage (Vrms_Out)
    //LCD_ShowOutput3(Vrms_Out.rms); // remove cast to int16_t
    LCD_ShowOutput3(Irms_Out.rms);
    segWrite(&LCDo_V, 1); // Turn on 'V' icon
}

static void LCD_Update_Minic_Zone_Data(void)
{
    // Update Minic Zone
    // Example: Update status icons
    segWrite(&LCDmLine, 1);
    segWrite(&LCDmOVERLOAD, 1);
    segWrite(&LCDmBYPASS, 1);
    segWrite(&LCDmLine, 1);
    segWrite(&LCDm25p100p, 1);
    segWrite(&LCDmLamp, 1);
    segWrite(&LCDmLout, 1);
    segWrite(&LCDmP, 1);
    
    segWrite(&LCDmLoadLv1, 1);
    segWrite(&LCDmLoadLv2, 1);
    segWrite(&LCDmLoadLv3, 1);
    segWrite(&LCDmLoadLv4, 1);
    segWrite(&LCDmLi, 1);
    segWrite(&LCDmSLA, 1);
    segWrite(&LCDmINV, 1);
    segWrite(&LCDmACtoLoad, 1);
    
    segWrite(&LCDmBATTtoLoad, 1);
    segWrite(&LCDmRECtoBATT, 1);
    segWrite(&LCDmRECtoINV, 1);
    segWrite(&LCDmECO, 1);
    segWrite(&LCDmCHARGING, 1);
    segWrite(&LCDmBATT, 1);
    segWrite(&LCDmREC, 1);
    segWrite(&LCDmPVtoREC, 1);

    segWrite(&LCDmBATTLv1, 1);
    segWrite(&LCDmBATTLv2, 1);
    segWrite(&LCDmBATTLv3, 1);
    segWrite(&LCDmBATTLv4, 1);
    segWrite(&LCDmAC, 1);
    segWrite(&LCDmWING, 1);
    segWrite(&LCDmACtoREC, 1);
    segWrite(&LCDmWINGtoREC, 1);

    segWrite(&LCDmPV, 1);
    segWrite(&LCDmPVLv3, 1);
    segWrite(&LCDmPVLv2, 1);
    segWrite(&LCDmPVLv1, 1);
}

static void refresh_zone(LcdZone z)
{
    switch(z)
    {
        case LCD_ZONE_INPUT:  
            LCD_Update_Input_Zone_Data();
            refresh_range(0, 9); 
            LCD_ZoneRequest(LCD_ZONE_CENTER);  
            break;
        case LCD_ZONE_CENTER: 
            LCD_Update_Center_Zone_Data();
            refresh_range(10,14);  
            LCD_ZoneRequest(LCD_ZONE_OUTPUT);
            break;
        case LCD_ZONE_OUTPUT: 
            LCD_Update_Output_Zone_Data();
            refresh_range(15,22); 
            LCD_ZoneRequest(LCD_ZONE_MINIC); 
            break;
        case LCD_ZONE_MINIC:  
            LCD_Update_Minic_Zone_Data();
            refresh_range(23,31);  
            LCD_ZoneRequest(LCD_ZONE_INPUT);
            break;
        default: break;
    }
}

void LCD_Service(void)
{
    uint16_t req = g_zone_req;
    if(req == 0) return;

    g_zone_req = 0;   // เคลียร์ก่อน
 //   req = 2;

    if(req & (1U << LCD_ZONE_INPUT))  refresh_zone(LCD_ZONE_INPUT);
    if(req & (1U << LCD_ZONE_CENTER)) refresh_zone(LCD_ZONE_CENTER);
    if(req & (1U << LCD_ZONE_OUTPUT)) refresh_zone(LCD_ZONE_OUTPUT);
    if(req & (1U << LCD_ZONE_MINIC))  refresh_zone(LCD_ZONE_MINIC);
}




void LCD_Init(void)
{
    /* ถ้าคุณอยากให้ LCD_Init() เรียก HT1621_Init() ด้วย ก็ทำได้
       แต่ถ้าคุณเรียก HT1621_Init() อยู่แล้ว ก็ปล่อยไว้เฉย ๆ */
    // HT1621_Init();
    LCD_ZoneRequest(LCD_ZONE_INPUT);
//    LCD_ZoneRequest(LCD_ZONE_CENTER);
//    LCD_ZoneRequest(LCD_ZONE_OUTPUT);
 //   LCD_ZoneRequest(LCD_ZONE_MINIC);
}

/* =========================
 *  Public: digits / numbers
 * ========================= */
void LCD_SetDigit(uint8_t digit_id, int8_t value, bool dp_on)
{
    const LcdDigitMap *m;
    uint8_t mask;

    m = getDigitMap(digit_id);
    mask = font7seg(value);

    segWrite(&m->A, (mask & SEG_A) != 0);
    segWrite(&m->B, (mask & SEG_B) != 0);
    segWrite(&m->C, (mask & SEG_C) != 0);
    segWrite(&m->D, (mask & SEG_D) != 0);
    segWrite(&m->E, (mask & SEG_E) != 0);
    segWrite(&m->F, (mask & SEG_F) != 0);
    segWrite(&m->G, (mask & SEG_G) != 0);

    if(m->hasDP)
        segWrite(&m->DP, dp_on);

  //  LCD_ZoneRequest(digitToZone(digit_id));
}

void LCD_ShowInput3(float n)
{
    int16_t val;
    int8_t d1, d2, d3;
    bool dp1 = false;
    bool dp2 = false;

    if(n < 0.0f) n = 0.0f;
    if(n > 999.0f) n = 999.0f;

    if(n < 10.0f)
    {
        // 0.00 - 9.99
        val = (int16_t)(n * 100.0f + 0.5f);
        if(val > 999) val = 999;
        
        d1 = (int8_t)((val / 100) % 10);
        d2 = (int8_t)((val / 10) % 10);
        d3 = (int8_t)(val % 10);
        dp1 = true;
    }
    else if(n < 100.0f)
    {
        // 10.0 - 99.9
        val = (int16_t)(n * 10.0f + 0.5f);
        if(val > 999) val = 999;

        d1 = (int8_t)((val / 100) % 10);
        d2 = (int8_t)((val / 10) % 10);
        d3 = (int8_t)(val % 10);
        dp2 = true;
    }
    else
    {
        // >= 100
        val = (int16_t)(n + 0.5f);
        if(val > 999) val = 999;

        d1 = (int8_t)((val / 100) % 10);
        d2 = (int8_t)((val / 10) % 10);
        d3 = (int8_t)(val % 10);
    }

    LCD_SetDigit(1, d1, dp1);
    LCD_SetDigit(2, d2, dp2);
    LCD_SetDigit(3, d3, false);
}

void LCD_ShowCenter2(int8_t tens, int8_t ones)
{
    LCD_SetDigit(4, tens, false);
    LCD_SetDigit(5, ones, false);
}

void LCD_ShowOutput3(float n)
{
    int16_t val;
    int8_t d6, d7, d8;
    bool dp6 = false;
    bool dp7 = false;

    if(n < 0.0f) n = 0.0f;
    if(n > 999.0f) n = 999.0f;

    if(n < 10.0f)
    {
        // 0.00 - 9.99
        val = (int16_t)(n * 100.0f + 0.5f);
        if(val > 999) val = 999;
        
        d6 = (int8_t)((val / 100) % 10);
        d7 = (int8_t)((val / 10) % 10);
        d8 = (int8_t)(val % 10);
        dp6 = true;
    }
    else if(n < 100.0f)
    {
        // 10.0 - 99.9
        val = (int16_t)(n * 10.0f + 0.5f);
        if(val > 999) val = 999;

        d6 = (int8_t)((val / 100) % 10);
        d7 = (int8_t)((val / 10) % 10);
        d8 = (int8_t)(val % 10);
        dp7 = true;
    }
    else
    {
        // >= 100
        val = (int16_t)(n + 0.5f);
        if(val > 999) val = 999;

        d6 = (int8_t)((val / 100) % 10);
        d7 = (int8_t)((val / 10) % 10);
        d8 = (int8_t)(val % 10);
    }

    if(d6 == 0 && !dp6 && !dp7) // Leading zero suppression for integer part
    {
        LCD_SetDigit(6, -1, false); 
        if(d7 == 0) LCD_SetDigit(7, -1, false); else LCD_SetDigit(7, d7, dp7);
    }
    else
    {
         LCD_SetDigit(6, d6, dp6);
         LCD_SetDigit(7, d7, dp7);
    }
    LCD_SetDigit(8, d8, false);
}

void LCD_WriteSeg(LcdSeg seg, uint8_t on)
{
    uint8_t m = (uint8_t)(1U << seg.bit);
    if(on)  ht1621_ram[seg.addr] |=  m;
    else    ht1621_ram[seg.addr] &= (uint8_t)~m;
}


///////////////////////////////////////////////////////////////////////////////////////////
/*
 * เขียน shadow RAM ทั้งชุดลง HT1621
 * - แนะนำให้เรียกจาก slow-loop (เช่น ทุก 50–100 ms) ไม่ใช่จาก ISR เร็ว
 */
void HT1621_RefreshAll(void)
{uint8_t addr;
    for( addr = 0U; addr < HT1621_MAX_ADDR; addr++)
    {
        HT1621_WriteData4(addr, ht1621_ram[addr] & 0x0FU);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////
/*
 * เขียนข้อมูล 4-bit ณ address หนึ่ง
 * - address = 6-bit (0..63) แต่ในที่นี้เราใช้ไม่เกิน HT1621_MAX_ADDR
 * - data4   = 4-bit (0..15) (ปกติใช้ LSB-first ตาม datasheet)
 */
void HT1621_WriteData4(uint8_t addr, uint8_t data4)
{
    addr &= 0x3FU;      /* limit to 6 bits */
    data4 &= 0x0FU;     /* limit to 4 bits */

 //   HT1621_ENTER_CRITICAL();
    DEVICE_DELAY_US(2);
    HT1621_CS_LOW();
    DEVICE_DELAY_US(2);
    /* 3-bit header: 101 = write data */
    ht1621_write_bits_msb(0b101U, 3U);
    DEVICE_DELAY_US(2);
    /* 6-bit address, MSB-first */

    ht1621_write_bits_msb(addr, 6U);
   // data4=0x0FU;
    /* 4-bit data, LSB-first (ตามตัวอย่างส่วนใหญ่ของ HT1621) */
    ht1621_write_bits_lsb(data4, 4U);
    DEVICE_DELAY_US(2);
    HT1621_CS_HIGH();
    //DEVICE_DELAY_US(2);
    DEVICE_DELAY_US(2);
//    HT1621_EXIT_CRITICAL();
}
///////////////////////////////////////////////////////////////////////////////////////////
/* ---------- Low-level bit-bang helpers (static) ---------- */

/* ส่ง 1 บิตไปที่ HT1621 (DATA → WR toggle) */
static void ht1621_write_bit(uint8_t bit)
{
    DEVICE_DELAY_US(2);
    HT1621_WR_LOW();
    DEVICE_DELAY_US(2);
    if(bit) HT1621_DATA_HIGH();
    else    HT1621_DATA_LOW();


    /* ถ้าจำเป็นค่อยเติม delay สั้น ๆ ตรงนี้ */
    DEVICE_DELAY_US(2);
    HT1621_WR_HIGH();
    DEVICE_DELAY_US(2);
}
///////////////////////////////////////////////////////////////////////////////////////////
/*
 * ส่งหลายบิต (MSB-first) ใช้สำหรับ prefix 3 บิต และ address/command ที่ส่ง MSB ก่อน
 * len <= 16
 */
static void ht1621_write_bits_msb(uint16_t data, uint8_t len)
{int8_t i;
    for(i = (int8_t)len - 1; i >= 0; i--)
    {  DEVICE_DELAY_US(2);
        ht1621_write_bit((data >> i) & 0x1U);
        DEVICE_DELAY_US(2);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////
/*
 * ส่งหลายบิต (LSB-first) ใช้สำหรับ data 4-bit ของ HT1621 ที่บางตัวอย่างใช้ LSB-first
 * คุณสามารถเปลี่ยนเป็น MSB-first หาก panel/mapping ของคุณต้องการ
 */
static void ht1621_write_bits_lsb(uint8_t data, uint8_t len)
{ uint8_t i;
    for( i = 0; i < len; i++)
    {  DEVICE_DELAY_US(2);
        ht1621_write_bit((data >> i) & 0x1U);
        DEVICE_DELAY_US(2);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////
/*
 * Set/clear bit หนึ่งใน shadow RAM
 * - addr: address ของ HT1621 (0..HT1621_MAX_ADDR-1)
 * - bit : 0..3 (เพราะเราใช้ 4-bit ต่อ address)
 * - on  : true = set bit, false = clear bit
 * - writeNow: true = เขียน address นี้ลง HT1621 ทันที
 */
void HT1621_SetBit(uint8_t addr, uint8_t bit, bool on, bool writeNow)
{
    if(addr >= HT1621_MAX_ADDR || bit > 3U)
    {
        return;
    }

    uint8_t mask = (uint8_t)(1U << bit);

    if(on)
        ht1621_ram[addr] |= mask;
    else
        ht1621_ram[addr] &= (uint8_t)(~mask);

    if(writeNow)
    {
        HT1621_WriteData4(addr, ht1621_ram[addr] & 0x0FU);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////
/*
 * HT1621_Init:
 * - config GPIO
 * - ตั้งค่าเริ่มต้น HT1621
 * - clear ทั้งจอ
 */
void HT1621_Init(void)
{
    /* ตั้งทิศทางขา, ค่าเริ่มต้น */
  //  HT1621_CONFIG_PINS();

    HT1621_CS_HIGH();
    HT1621_WR_HIGH();
    HT1621_DATA_LOW();

    HT1621_Command(0x52);
    HT1621_Command(0X30);
    HT1621_Command(0X00);
    HT1621_Command(0X0A);
    HT1621_Command(0X02);
    HT1621_Command(0X06);

    /* เคลียร์จอ + shadow RAM */
    HT1621_ClearAll();
}
///////////////////////////////////////////////////////////////////////////////////////////
/* ---------- Public API Implementation ---------- */

void HT1621_Command(uint8_t cmd)
{
   // HT1621_ENTER_CRITICAL();

    HT1621_CS_LOW();
    DEVICE_DELAY_US(2);
    /* 3-bit header: 100 = command mode */
    ht1621_write_bits_msb(0b1000U, 4U);

    /* 8-bit command (MSB-first) */
    ht1621_write_bits_msb(cmd, 8U);

    HT1621_CS_HIGH();

   DEVICE_DELAY_US(2);

  //  HT1621_EXIT_CRITICAL();
}
///////////////////////////////////////////////////////////////////////////////////////////
/*
 * เคลียร์ shadow RAM + เขียน 0 ลงทุก address ใน HT1621
 */
void HT1621_ClearAll(void)
{uint8_t i;
    for(i = 0; i < HT1621_MAX_ADDR; i++)
    {
        ht1621_ram[i] = 0U;
        HT1621_WriteData4(i, 0U);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////
/*
 * Soft reset HT1621:
 * - ปิด system
 * - เปิด system
 * - เปิด LCD
 * - เคลียร์ RAM ภายใน
 * - เขียน shadow RAM ปัจจุบันกลับลงไป
 */
void HT1621_SoftReset(void)
{uint8_t addr;
    /* Disable system, then re-enable */
    HT1621_Command(HT1621_CMD_SYS_DIS);
    HT1621_Command(HT1621_CMD_SYS_EN);

    /* Config RC/BIAS/COM ตามที่คุณกำหนดใน header */
    HT1621_Command(HT1621_CMD_RC_4COM_3BIAS);

    /* Turn LCD back on */
    HT1621_Command(HT1621_CMD_LCD_ON);

    /* เคลียร์ RAM ภายในก่อน */
    for(addr = 0U; addr < HT1621_MAX_ADDR; addr++)
    {
        HT1621_WriteData4(addr, 0U);
    }

    /* เขียน shadow RAM ปัจจุบันกลับไป */
    HT1621_RefreshAll();
}
///////////////////////////////////////////////////////////////////////////////////////////


