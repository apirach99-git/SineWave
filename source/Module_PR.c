/*
 * Module_PR.c
 * Proportional-Resonant (PR) Controller — Floating Point
 *
 * Discrete-time PR algorithm (Tustin / bilinear approximation):
 *
 *   Resonant integrator (per harmonic h):
 *       yh(k)    = yh(k-1) + Kih * e(k-1) + wtsh * vh(k-1)
 *       vh(k)    = vh(k-1) - wtsh * yh(k)
 *
 *   Controller output:
 *       OutPreSat = Kp*e(k) + y1(k) [+ y3(k)] [+ y5(k)]
 *       Out       = clamp(OutPreSat, OutMin, OutMax)
 *
 * Tuning guidelines:
 *   Kp  : Proportional gain → bandwidth.  Start: 0.3
 *   Ki  : Resonant gain at fundamental.   Ts / Ti  (Ti ~ 0.02–0.1)
 *   Ki3 : Resonant gain at 3rd harmonic.  Ts / Ti3 (Ti3 ~ 0.3–0.7)
 *   Ki5 : Resonant gain at 5th harmonic.  Ts / Ti5 (Ti5 ~ 0.5–1.0)
 *   wts : Ts * 2*PI*F_fund   (e.g. 0.0001 * 314.16 = 0.031416 @ 10kHz, 50Hz)
 *
 * ND2 Project — TMS320F280049C
 * Author: Piyasak Kranprakon (GUS)
 */

#include "F28x_Project.h"
#include "include/ND2.h"

/*--------------------------------------------------------------------------
 * Constants
 *--------------------------------------------------------------------------*/
#define PR_PI_F     3.14159265358979f
#define PR_FOUT_HZ  50.0f               /* Fundamental output frequency     */
#define PR_FSW_HZ   10000.0f            /* Switching / sampling frequency   */
#define PR_TS       (1.0f / PR_FSW_HZ) /* Sampling period  = 100 µs        */
#define PR_W0       (2.0f * PR_PI_F * PR_FOUT_HZ)   /* 314.159 rad/s      */

/*--------------------------------------------------------------------------
 * Global instance
 *--------------------------------------------------------------------------*/
PR_t PR_Reg = {0};

/*--------------------------------------------------------------------------
 * PR_Init  —  Setup gains and pre-compute wts coefficients
 *
 *  pr      : pointer to PR_t instance
 *  Kp      : proportional gain
 *  Ki      : resonant gain (fundamental)
 *  Ki3     : resonant gain (3rd harmonic, set 0.0f to disable)
 *  Ki5     : resonant gain (5th harmonic, set 0.0f to disable)
 *  w0      : fundamental angular frequency (rad/s), e.g. 2*PI*50
 *  Ts      : sampling period (s), e.g. 1/10000
 *  OutMax  : positive output saturation limit (OutMin = -OutMax)
 *--------------------------------------------------------------------------*/
void PR_Init(PR_t *pr, float Kp, float Ki, float Ki3, float Ki5,
             float w0, float Ts, float OutMax)
{
    pr->Kp     = Kp;
    pr->Ki     = Ki;
    pr->Ki3    = Ki3;
    pr->Ki5    = Ki5;

    pr->wts    = Ts * w0;           /* fundamental  ω₀·Ts  */
    pr->wts3   = Ts * 3.0f * w0;   /* 3rd harmonic 3ω₀·Ts */
    pr->wts5   = Ts * 5.0f * w0;   /* 5th harmonic 5ω₀·Ts */

    pr->OutMax =  OutMax;
    pr->OutMin = -OutMax;

    PR_Reset(pr);
}

/*--------------------------------------------------------------------------
 * PR_Reset  —  Clear all internal states (call after fault / stop)
 *--------------------------------------------------------------------------*/
void PR_Reset(PR_t *pr)
{
    pr->Err        = 0.0f;
    pr->err_old    = 0.0f;

    pr->yk         = 0.0f;  pr->yk_old   = 0.0f;
    pr->vk         = 0.0f;  pr->vk_old   = 0.0f;

    pr->yk3        = 0.0f;  pr->yk_old3  = 0.0f;
    pr->vk3        = 0.0f;  pr->vk_old3  = 0.0f;

    pr->yk5        = 0.0f;  pr->yk_old5  = 0.0f;
    pr->vk5        = 0.0f;  pr->vk_old5  = 0.0f;

    pr->Up         = 0.0f;
    pr->OutPreSat  = 0.0f;
    pr->Out        = 0.0f;
    pr->SatErr     = 0.0f;
}

/*--------------------------------------------------------------------------
 * PR_Update  —  Execute one PR control step
 *               Call this inside the PWM ISR at every Ts
 *
 *  Before calling: set pr->Ref and pr->Fdb
 *  After calling : read pr->Out  (modulation index, clamped ±OutMax)
 *--------------------------------------------------------------------------*/
void PR_Update(PR_t *pr)
{
    /* ── 1. Error ── */
    pr->Err = pr->Ref - pr->Fdb;

    /* ── 2. Fundamental resonant integrator ──
     *   yk(k)  = yk(k-1) + Ki  * e(k-1) + wts  * vk(k-1)
     *   vk(k)  = vk(k-1) - wts  * yk(k)
     */
    pr->yk    = pr->yk_old  + pr->Ki  * pr->err_old  + pr->wts  * pr->vk_old;
    pr->vk    = pr->vk_old  - pr->wts  * pr->yk;

    /* ── 3. 3rd harmonic resonant integrator (optional) ── */
    pr->yk3   = pr->yk_old3 + pr->Ki3 * pr->err_old  + pr->wts3 * pr->vk_old3;
    pr->vk3   = pr->vk_old3 - pr->wts3 * pr->yk3;

    /* ── 4. 5th harmonic resonant integrator (optional) ── */
    pr->yk5   = pr->yk_old5 + pr->Ki5 * pr->err_old  + pr->wts5 * pr->vk_old5;
    pr->vk5   = pr->vk_old5 - pr->wts5 * pr->yk5;

    /* ── 5. Proportional term ── */
    pr->Up    = pr->Kp * pr->Err;

    /* ── 6. Sum output ── */
    pr->OutPreSat = pr->Up + pr->yk + pr->yk3 + pr->yk5;

    /* ── 7. Saturation ── */
    if      (pr->OutPreSat >  pr->OutMax) pr->Out = pr->OutMax;
    else if (pr->OutPreSat <  pr->OutMin) pr->Out = pr->OutMin;
    else                                  pr->Out = pr->OutPreSat;

    pr->SatErr = pr->Out - pr->OutPreSat;   /* negative when sat-high, positive when sat-low */

    /* ── 7b. Anti-windup — back-calculation on resonant integrators ──
     *  When output saturates, pull each integrator back by SatErr before
     *  storing as the next-cycle state.  This limits resonant peak build-up
     *  without requiring a separate Kaw tuning parameter.
     */
    pr->yk  += pr->SatErr;   /* fundamental  integrator */
    pr->yk3 += pr->SatErr;   /* 3rd harmonic integrator */
    pr->yk5 += pr->SatErr;   /* 5th harmonic integrator */

    /* ── 8. Shift state registers ── */
    pr->err_old  = pr->Err;

    pr->yk_old   = pr->yk;    pr->vk_old   = pr->vk;
    pr->yk_old3  = pr->yk3;   pr->vk_old3  = pr->vk3;
    pr->yk_old5  = pr->yk5;   pr->vk_old5  = pr->vk5;
}

/*--------------------------------------------------------------------------
 * SetupPRControl  —  Legacy wrapper (backward compatible call)
 *   Initialises PR_Reg with default gains for 50Hz, 10kHz switching
 *--------------------------------------------------------------------------*/
void SetupPRControl(void)
{
    PR_Init(&PR_Reg,
            /*Kp  */ 0.3f,
            /*Ki  */ PR_TS / 0.0215f,   /* Ti  ~ 21.5 ms  */
            /*Ki3 */ PR_TS / 0.7f,      /* Ti3 ~ 700  ms  */
            /*Ki5 */ PR_TS / 0.999f,    /* Ti5 ~ 999  ms  */
            /*w0  */ PR_W0,
            /*Ts  */ PR_TS,
            /*Lim */ 0.975f);
}

/*--------------------------------------------------------------------------
 * CalPRControl  —  Legacy wrapper (backward compatible call)
 *   Runs PR_Update on the global PR_Reg instance
 *--------------------------------------------------------------------------*/
void CalPRControl(void)
{
    PR_Update(&PR_Reg);
}

/*===========================================================================
 * End of file.
 *===========================================================================*/
