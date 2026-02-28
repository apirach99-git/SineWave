/*
 * Module_PR.h
 * Proportional-Resonant (PR) Controller — Floating Point
 *
 * Controller structure (Tustin discrete-time, zero-pole matching):
 *
 *   y(k) = y(k-1) + Ki*e(k-1) + wts*v(k-1)
 *   v(k) = v(k-1) - wts*y(k)
 *   Out   = Kp*e(k) + y(k)         [fundamental]
 *         + y3(k)                   [3rd harmonic, optional]
 *         + y5(k)                   [5th harmonic, optional]
 *
 * where:
 *   wts  = Ts * w0        (fundamental,  w0 = 2*PI*50)
 *   wts3 = Ts * 3*w0      (3rd harmonic)
 *   wts5 = Ts * 5*w0      (5th harmonic)
 *   Ts   = 1 / Fsw        (switching period, e.g. 100us @ 10kHz)
 *
 * ND2 Project — TMS320F280049C
 * Author: Piyasak Kranprakon (GUS)
 */

#ifndef MODULE_PRCONTROL_H
#define MODULE_PRCONTROL_H

#include <stdint.h>
#include <math.h>

/*--------------------------------------------------------------------------
 * PR_t  —  Proportional-Resonant Controller (pure float)
 *--------------------------------------------------------------------------*/
typedef struct
{
    /* ── Inputs ── */
    float   Ref;            /* Voltage reference  (per-unit or volts)         */
    float   Fdb;            /* Voltage feedback   (from ADC, same unit as Ref) */

    /* ── Gains ── */
    float   Kp;             /* Proportional gain                               */
    float   Ki;             /* Resonant gain (fundamental)                     */
    float   Ki3;            /* Resonant gain (3rd harmonic)                    */
    float   Ki5;            /* Resonant gain (5th harmonic)                    */

    float   wts;            /* Ts * w0        (fundamental)                   */
    float   wts3;           /* Ts * 3*w0      (3rd harmonic)                  */
    float   wts5;           /* Ts * 5*w0      (5th harmonic)                  */

    /* ── Output limits ── */
    float   OutMax;         /* Upper saturation limit  (e.g.  0.975)          */
    float   OutMin;         /* Lower saturation limit  (e.g. -0.975)          */

    /* ── Internal states — fundamental ── */
    float   Err;            /* Current error   = Ref - Fdb                    */
    float   err_old;        /* Previous error                                 */
    float   yk;             /* Resonant integrator output (k)                 */
    float   yk_old;         /* Resonant integrator output (k-1)               */
    float   vk;             /* Resonant quadrature state  (k)                 */
    float   vk_old;         /* Resonant quadrature state  (k-1)               */

    /* ── Internal states — 3rd harmonic ── */
    float   yk3;
    float   yk_old3;
    float   vk3;
    float   vk_old3;

    /* ── Internal states — 5th harmonic ── */
    float   yk5;
    float   yk_old5;
    float   vk5;
    float   vk_old5;

    /* ── Outputs ── */
    float   Up;             /* Proportional term output                       */
    float   OutPreSat;      /* Output before saturation                       */
    float   Out;            /* Final saturated output                         */
    float   SatErr;         /* Saturation error  (Out - OutPreSat)            */

} PR_t;

/* Global PR controller instance */
extern PR_t PR_Reg;

/* ── Public API ── */
void PR_Init(PR_t *pr, float Kp, float Ki, float Ki3, float Ki5,
             float w0, float Ts, float OutMax);
void PR_Reset(PR_t *pr);
void PR_Update(PR_t *pr);   /* Call in ISR every Ts */

#endif  /* MODULE_PRCONTROL_H */

/*===========================================================================
 * End of file.
 *===========================================================================*/
