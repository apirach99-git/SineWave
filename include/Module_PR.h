//added all chanel conversion
#ifndef MODULE_PRCONTROL_H
#define MODULE_PRCONTROL_H


//------------------------------------------------------------------------------------------------------------
//!!!Do not change the order of structure F510-F520
struct PR_REG {
        float32_t Ref;
        float32_t Fdb;
        float32_t Err;
        float32_t Kp;
        float32_t Up;
        float32_t OutPreSat;
        float32_t OutMax;
        float32_t OutMin;
        float32_t Out;
        float32_t SatErr;

        float32_t Ki;
        float32_t Kc;
        float32_t wts;
        float32_t err_old;
        float32_t yk;
        float32_t yk_old;
        float32_t vk;
        float32_t vk_old;

        float32_t Ki3;
        float32_t wts3;
        float32_t yk3;
        float32_t yk_old3;
        float32_t vk3;
        float32_t vk_old3;

        float32_t Ki5;
        float32_t wts5;
        float32_t yk5;
        float32_t yk_old5;
        float32_t vk5;
        float32_t vk_old5;
}; 
extern struct PR_REG PR_Reg; 

struct PR_REG_IQ {
        _iq Ref;
        _iq Fdb;
        _iq Err;
        _iq Kp;
        _iq Up;
        long   OutPreSat;
        _iq OutMax;
        _iq OutMin;
        _iq Out;
        _iq SatErr;

        _iq Ki;
        _iq Kc;
        _iq wts;
        _iq err_old;
        _iq yk;
        _iq yk_old;
        _iq vk;
        _iq vk_old;

        _iq Ki3;
        _iq wts3;
        _iq yk3;
        _iq yk_old3;
        _iq vk3;
        _iq vk_old3;

        _iq Ki5;
        _iq wts5;
        _iq yk5;
        _iq yk_old5;
        _iq vk5;
        _iq vk_old5;
};
extern struct PR_REG_IQ PR_Reg_IQ;

#endif  

//===========================================================================
// End of file.
//===========================================================================


















