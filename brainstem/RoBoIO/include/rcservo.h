#ifndef __RCSERVO_H
#define __RCSERVO_H

#include "defines.h"


#ifdef __cplusplus
extern "C" {
#endif

RBAPI(bool) rcservo_InUse(void);

RBAPI(bool) rcservo_Initialize(unsigned long usedchannels);
//-- values for the "usedchannels" argument (note that different values
//   can be added to use multiple channels)
     #define RCSERVO_USENOCHANNEL			(0L)
     #define RCSERVO_USECHANNEL0			(1L)
     #define RCSERVO_USECHANNEL1			(1L<<1)
     #define RCSERVO_USECHANNEL2			(1L<<2)
     #define RCSERVO_USECHANNEL3			(1L<<3)
     #define RCSERVO_USECHANNEL4			(1L<<4)
     #define RCSERVO_USECHANNEL5			(1L<<5)
     #define RCSERVO_USECHANNEL6			(1L<<6)
     #define RCSERVO_USECHANNEL7			(1L<<7)
     #define RCSERVO_USECHANNEL8			(1L<<8)
     #define RCSERVO_USECHANNEL9			(1L<<9)
     #define RCSERVO_USECHANNEL10			(1L<<10)
     #define RCSERVO_USECHANNEL11			(1L<<11)
     #define RCSERVO_USECHANNEL12			(1L<<12)
     #define RCSERVO_USECHANNEL13			(1L<<13)
     #define RCSERVO_USECHANNEL14			(1L<<14)
     #define RCSERVO_USECHANNEL15			(1L<<15)
     #define RCSERVO_USECHANNEL16			(1L<<16)
     #define RCSERVO_USECHANNEL17			(1L<<17)
     #define RCSERVO_USECHANNEL18			(1L<<18)
     #define RCSERVO_USECHANNEL19			(1L<<19)
     #define RCSERVO_USECHANNEL20			(1L<<20)
     #define RCSERVO_USECHANNEL21			(1L<<21)
     #define RCSERVO_USECHANNEL22			(1L<<22)
     #define RCSERVO_USECHANNEL23			(1L<<23)
//   #define RCSERVO_USECHANNEL24			(1L<<24)
//   #define RCSERVO_USECHANNEL25			(1L<<25)
//   #define RCSERVO_USECHANNEL26			(1L<<26)
//   #define RCSERVO_USECHANNEL27			(1L<<27)
//   #define RCSERVO_USECHANNEL28			(1L<<28)
//   #define RCSERVO_USECHANNEL29			(1L<<29)
//   #define RCSERVO_USECHANNEL30			(1L<<30)
//   #define RCSERVO_USECHANNEL31			(1L<<31)
//-- if the above function returns false, roboio_GetErrCode() may return:
//   #define ERROR_IOINITFAIL       (ERR_NOERROR + 100) //need include <io.h>
//   #define ERROR_IOSECTIONFULL    (ERR_NOERROR + 101) //need include <io.h>
//   #define ERROR_CPUUNSUPPORTED	(ERR_NOERROR + 102) //need include <io.h>
//   #define ERROR_PWM_INUSE	    (ERR_NOERROR + 300) //need include <pwm.h>
     #define ERROR_RCSERVO_INUSE    (ERR_NOERROR + 400)

RBAPI(void) rcservo_Close(void);


RBAPI(bool) rcservo_SetServo(int channel, unsigned servono);
RBAPI(bool) rcservo_SetServos(unsigned long channels, unsigned servono);
//-- values for the "servono" argument
     #define RCSERVO_SERVO_DEFAULT    		(0x00)  //conservative setting for most servos with pulse feedback
     #define RCSERVO_SERVO_DEFAULT_NOFB     (0x01)  //conservative setting for most servos without pulse feedback
     #define RCSERVO_KONDO_KRS786	        (0x11)
     #define RCSERVO_KONDO_KRS788			(0x12)
     #define RCSERVO_KONDO_KRS78X			(0x13)
     #define RCSERVO_KONDO_KRS4014          (0x14)
     #define RCSERVO_HITEC_HSR8498			(0x22)
//-- if the above functions return false, roboio_GetErrCode() may return:
//   #define ERROR_RCSERVO_INUSE            (ERR_NOERROR + 400)
     #define ERROR_RCSERVO_UNKNOWNSERVO     (ERR_NOERROR + 461)
     #define ERROR_RCSERVO_WRONGCHANNEL     (ERR_NOERROR + 462)

RBAPI(bool) rcservo_SetServoType(int channel, unsigned type, unsigned fbmethod);
//-- values for the "type" argument
     #define RCSERVO_SV_FEEDBACK            (1)     //servo with pulse feedback
     #define RCSERVO_SV_NOFEEDBACK          (2)     //servo without pulse feedback
//-- values for the "fbmethod" argument (note that different values
//   can be added to use multiple settings)
     #define RCSERVO_FB_SAFEMODE            (0)
     #define RCSERVO_FB_FASTMODE            (1)     //much faster than safe mode but could cause servo's shake
     #define RCSERVO_FB_DENOISE             (1<<1)  //use denoise filter; more accurate but very slow
//-- if the above function returns false, roboio_GetErrCode() may return:
//   #define ERROR_RCSERVO_WRONGCHANNEL     (ERR_NOERROR + 462)

RBAPI(bool) rcservo_SetServoParams1(int channel, unsigned long period, unsigned long minduty, unsigned long maxduty);
RBAPI(bool) rcservo_SetServoParams2(int channel, unsigned long invalidduty, unsigned long minlowphase);
//-- if the above functions return false, roboio_GetErrCode() may return:
//   #define ERROR_RCSERVO_INUSE            (ERR_NOERROR + 400)
//   #define ERROR_RCSERVO_WRONGCHANNEL     (ERR_NOERROR + 462)
     #define ERROR_RCSERVO_INVALIDPARAMS    (ERR_NOERROR + 463)

RBAPI(bool) rcservo_SetReadFBParams1(int channel, unsigned long initdelay, unsigned long lastdelay, unsigned long maxwidth);
RBAPI(bool) rcservo_SetReadFBParams2(int channel, unsigned long resolution, long offset);
RBAPI(bool) rcservo_SetReadFBParams3(int channel, int maxfails, int filterwidth);
//-- if the above functions return false, roboio_GetErrCode() may return:
//   #define ERROR_RCSERVO_INUSE            (ERR_NOERROR + 400)
//   #define ERROR_RCSERVO_WRONGCHANNEL     (ERR_NOERROR + 462)
//   #define ERROR_RCSERVO_INVALIDPARAMS    (ERR_NOERROR + 463)

RBAPI(bool) rcservo_SetCmdPulse(int channel, int cmd, unsigned long duty);
RBAPI(bool) rcservo_SetPlayModeCMD(int channel, int cmd);
//-- values for the "cmd" argument
     #define RCSERVO_CMD_POWEROFF           (0)
     #define RCSERVO_CMD1                   (1)
     #define RCSERVO_CMD2                   (2)
     #define RCSERVO_CMD3                   (3)
     #define RCSERVO_CMD4                   (4)
     #define RCSERVO_CMD5                   (5)
     #define RCSERVO_CMD6                   (6)
     #define RCSERVO_CMD7                   (7)
//-- if the above function return false, roboio_GetErrCode() may return:
//   #define ERROR_RCSERVO_WRONGCHANNEL     (ERR_NOERROR + 462)
//   #define ERROR_RCSERVO_INVALIDPARAMS    (ERR_NOERROR + 463)



//******************  Functions in Capture Mode  *********************
RBAPI(void) rcservo_EnterCaptureMode(void);

RBAPI(unsigned long) rcservo_ReadPosition(int channel, int cmd);
RBAPI(unsigned long) rcservo_ReadPositionDN(int channel, int cmd); //for backward compatibility to RoBoIO library 1.1
RBAPI(void)          rcservo_ReadPositions(unsigned long channels, int cmd, unsigned long* width);
RBAPI(void)          rcservo_ReadPositionsDN(unsigned long channels, int cmd, unsigned long* width); //for backward compatibility to RoBoIO library 1.1
//-- values for the "cmd" argument
//   #define RCSERVO_CMD_POWEROFF           (0)
//   #define RCSERVO_CMD1                   (1)
//   #define RCSERVO_CMD2                   (2)
//   #define RCSERVO_CMD3                   (3)
//   #define RCSERVO_CMD4                   (4)
//   #define RCSERVO_CMD5                   (5)
//   #define RCSERVO_CMD6                   (6)
//   #define RCSERVO_CMD7                   (7)
//-- return value (= 0xffffffffL if fails) indicates the feedback pulse width, which
//   is equal to return value * resolution * 0.1us, where "resolution" is set by rcservo_SetReadFBParams2()

//enable/disable multiprogramming OS heuristic
RBAPI(void) rcservo_EnableMPOS(void);
RBAPI(void) rcservo_DisableMPOS(void);



//******************  Functions in Play Mode  ************************
RBAPI(void) rcservo_EnterPlayMode(void);
RBAPI(void) rcservo_EnterPlayMode_NOFB(unsigned long* width);
RBAPI(void) rcservo_EnterPlayMode_HOME(unsigned long* width);

RBAPI(void) rcservo_SetFPS(int fps);
RBAPI(void) rcservo_SetAction(unsigned long* width, unsigned long playtime);

RBAPI(int)  rcservo_PlayAction(void);
RBAPI(int)  rcservo_PlayActionMix(long* mixwidth);
//-- return values of rcservo_PlayAction()
     #define RCSERVO_PLAYEND (0)
     #define RCSERVO_PLAYING (1)
     #define RCSERVO_PAUSED  (2)
//-- special width values for mixwidth[i]
     #define RCSERVO_MIXWIDTH_POWEROFF      (0x7fffff00L)
     #define RCSERVO_MIXWIDTH_CMD1          (0x7fffff01L)
     #define RCSERVO_MIXWIDTH_CMD2          (0x7fffff02L)
     #define RCSERVO_MIXWIDTH_CMD3          (0x7fffff03L)
     #define RCSERVO_MIXWIDTH_CMD4          (0x7fffff04L)
     #define RCSERVO_MIXWIDTH_CMD5          (0x7fffff05L)
     #define RCSERVO_MIXWIDTH_CMD6          (0x7fffff06L)
     #define RCSERVO_MIXWIDTH_CMD7          (0x7fffff07L)

RBAPI(void) rcservo_PauseAction(void);
RBAPI(void) rcservo_ReleaseAction(void);
RBAPI(void) rcservo_StopAction(void);

RBAPI(void) rcservo_MoveTo(unsigned long* width, unsigned long playtime);
RBAPI(void) rcservo_MoveToMix(unsigned long* width, unsigned long playtime, long* mixwidth);



//******************  Functions in PWM Mode  *************************
RBAPI(void) rcservo_EnterPWMMode(void);

RBAPI(bool) rcservo_SendPWMPulses(int channel, unsigned long period, unsigned long duty, unsigned long count);
//-- if the above function returns false, roboio_GetErrCode() may return:
     #define ERROR_RCSERVO_PWMFAIL      (ERR_NOERROR + 430)

RBAPI(bool) rcservo_IsPWMCompleted(int channel);



//******************  Functions for Unused Channels  *****************
RBAPI(void) rcservo_Outp(int channel, int value);
RBAPI(int) rcservo_Inp(int channel);

RBAPI(void) rcservo_Outps(unsigned long channels, unsigned long value);
RBAPI(unsigned long) rcservo_Inps(unsigned long channels);

#ifdef __cplusplus
}
#endif

#endif

