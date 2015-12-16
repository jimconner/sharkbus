/*******************************************************************************
  Update List

   2010/08/25:

    - [by AAA] Add pwm_NumCh()
				
*******************************************************************************/

#ifndef __PWM_H
#define __PWM_H

#include "defines.h"
#include "pwmdx.h"

#ifdef __cplusplus
extern "C" {
#endif

RBAPI(int)  pwm_NumCh(void);

RBAPI(bool) pwm_InUse(void);

RBAPI(bool) pwm_Initialize(unsigned baseaddr, int clkmode, int irqmode);
//-- if the above function return false, roboio_GetErrCode() may return:
//   #define ERROR_IOINITFAIL       (ERR_NOERROR + 100) //need include <io.h>
//   #define ERROR_IOSECTIONFULL    (ERR_NOERROR + 101) //need include <io.h>
//   #define ERROR_CPUUNSUPPORTED	(ERR_NOERROR + 102) //need include <io.h>
     #define ERROR_PWM_INUSE	    (ERR_NOERROR + 300)

RBAPI(void) pwm_Close(void);


RBAPI(void) pwm_SetBaseClock(int clock);
//-- values for the "clock" argument
//   #define PWMCLOCK_10MHZ    (0)
//   #define PWMCLOCK_50MHZ    (1)

RBAPI(bool) pwm_SetPulse(int channel, unsigned long period, unsigned long duty);		//resolution: 1us
RBAPI(bool) pwm_SetPulse_10MHZ(int channel, unsigned long period, unsigned long duty);	//resolution: 0.1us
RBAPI(bool) pwm_SetPulse_50MHZ(int channel, unsigned long period, unsigned long duty);	//resolution: 20ns
//-- if the above function return false, roboio_GetErrCode() may return:
     #define ERROR_PWM_WRONGCHANNEL     (ERR_NOERROR + 310)
     #define ERROR_PWM_INVALIDPULSE     (ERR_NOERROR + 311)
     #define ERROR_PWM_CLOCKMISMATCH    (ERR_NOERROR + 312)


/*
void pwm_BackupAllSettings(void);
void pwm_RestoreAllSettings(void);
*/


RBAPI(void) pwm_EnablePin(int channel);
RBAPI(void) pwm_DisablePin(int channel);
RBAPI(void) pwm_EnableMultiPin(unsigned long channels);
RBAPI(void) pwm_DisableMultiPin(unsigned long channels);

RBAPI(void) pwm_Lock(int channel);
RBAPI(void) pwm_Unlock(int channel);
RBAPI(void) pwm_LockMulti(unsigned long channels);
RBAPI(void) pwm_UnlockMulti(unsigned long channels);

RBAPI(void) pwm_EnableINT(int channel);
RBAPI(void) pwm_DisableINT(int channel);
RBAPI(void) pwm_EnableMultiINT(unsigned long channels);
RBAPI(void) pwm_DisableMultiINT(unsigned long channels);

RBAPI(void) pwm_ClearMultiFLAG(unsigned long channels);
RBAPI(unsigned long) pwm_ReadMultiFLAG(unsigned long channels);

RBAPI(void) pwm_EnablePWM(int channel);
RBAPI(void) pwm_DisablePWM(int channel);
RBAPI(void) pwm_EnableMultiPWM(unsigned long channels);
RBAPI(void) pwm_DisableMultiPWM(unsigned long channels);

RBAPI(int)  pwm_ReadCountingMode(int channel);
RBAPI(void) pwm_SetCountingMode(int channel, int mode);
RBAPI(void) pwm_SetWaveform(int channel, int mode);

RBAPI(void) pwm_SetPulseCount(int channel, unsigned long count);
RBAPI(void) pwm_SetCtrlREG(int channel, unsigned long creg);
RBAPI(void) pwm_SetHREG(int channel, unsigned long hreg);
RBAPI(void) pwm_SetLREG(int channel, unsigned long lreg);

RBAPI(unsigned long) pwm_ReadPulseCount(int channel);
RBAPI(unsigned long) pwm_ReadCtrlREG(int channel);
RBAPI(unsigned long) pwm_ReadHREG(int channel);
RBAPI(unsigned long) pwm_ReadLREG(int channel);

#ifdef __cplusplus
}
#endif

#endif

