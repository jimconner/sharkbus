#ifndef __PWMDX_H
#define __PWMDX_H

#ifdef __cplusplus
extern "C" {
#endif

RBAPI(void) pwm_SetBaseAddress(unsigned baseaddr);
RBAPI(unsigned) pwm_SetDefaultBaseAddress(void);

RBAPI(void) pwm_SelectClock(int clock);
//-- values for the "clock" argument
#define PWMCLOCK_10MHZ    (0) //don't want to use enum:p
#define PWMCLOCK_50MHZ    (1)

RBAPI(void) pwm_SetIRQ(int pwmirq);
//-- values for the "pwmirq" argument
#define PWMIRQ1         (8)   //don't want to use enum:p
#define PWMIRQ3         (2)
#define PWMIRQ4         (4)
#define PWMIRQ5         (5)
#define PWMIRQ6         (7)
#define PWMIRQ7         (6)
#define PWMIRQ9         (1)
#define PWMIRQ10        (3)
#define PWMIRQ11        (9)
#define PWMIRQ12        (11)
#define PWMIRQ14        (13)
#define PWMIRQ15        (15)
#define PWMIRQ_DISABLE  (0)

RBAPI(void) pwmdx_EnablePin(int channel);
RBAPI(void) pwmdx_DisablePin(int channel);
RBAPI(void) pwmdx_EnableMultiPin(unsigned long channels);
RBAPI(void) pwmdx_DisableMultiPin(unsigned long channels);

RBAPI(void) pwmdx_Lock(int channel);
RBAPI(void) pwmdx_Unlock(int channel);
RBAPI(void) pwmdx_LockMulti(unsigned long channels);
RBAPI(void) pwmdx_UnlockMulti(unsigned long channels);

RBAPI(void) pwmdx_EnableINT(int channel);
RBAPI(void) pwmdx_DisableINT(int channel);
RBAPI(void) pwmdx_EnableMultiINT(unsigned long channels);
RBAPI(void) pwmdx_DisableMultiINT(unsigned long channels);

RBAPI(void) pwmdx_ClearMultiFLAG(unsigned long channels);
RBAPI(unsigned long) pwmdx_ReadMultiFLAG(unsigned long channels);

RBAPI(void) pwmdx_EnablePWM(int channel);
RBAPI(void) pwmdx_DisablePWM(int channel);
RBAPI(void) pwmdx_EnableMultiPWM(unsigned long channels);
RBAPI(void) pwmdx_DisableMultiPWM(unsigned long channels);

RBAPI(int)  pwmdx_ReadCountingMode(int channel);
RBAPI(void) pwmdx_SetCountingMode(int channel, int mode);
//-- values for the "mode" argument
#define PWM_COUNT_MODE        (0)  //don't want to use enum:p
#define PWM_CONTINUE_MODE     (1)

RBAPI(void) pwmdx_SetWaveform(int channel, int mode);
//-- values for the "mode" argument
#define PWM_WAVEFORM_NORMAL   (0)  //don't want to use enum:p
#define PWM_WAVEFORM_INVERSE  (1)

RBAPI(void) pwmdx_SetPulseCount(int channel, unsigned long count);
RBAPI(void) pwmdx_SetCtrlREG(int channel, unsigned long creg);
RBAPI(void) pwmdx_SetHREG(int channel, unsigned long hreg);
RBAPI(void) pwmdx_SetLREG(int channel, unsigned long lreg);

RBAPI(unsigned long) pwmdx_ReadPulseCount(int channel);
RBAPI(unsigned long) pwmdx_ReadCtrlREG(int channel);
RBAPI(unsigned long) pwmdx_ReadHREG(int channel);
RBAPI(unsigned long) pwmdx_ReadLREG(int channel);

RBAPI(void) pwm_SetSyncREG(unsigned long sreg);
RBAPI(void) pwm_SetINTREG(unsigned long ireg);
RBAPI(unsigned long) pwm_ReadSyncREG(void);
RBAPI(unsigned long) pwm_ReadINTREG(void);

RBAPI(void) pwm_SetPWMSettingREG(unsigned long psreg);
RBAPI(unsigned long) pwm_ReadPWMSettingREG(void);

#ifdef __cplusplus
}
#endif 

#endif

