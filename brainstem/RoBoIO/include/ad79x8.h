/*******************************************************************************
  Update List

   2010/08/25:

    - [by AAA] Add the following functions

				adc_ReadChannel(), adc_Read(), adc_ReadMCH(), adc_ReadMCH2(),
				adc_InUse(), adc_Initialize(), adc_Close(), adc_ChangeChannel(),
                adc_InitializeMCH(), adc_CloseMCH(), adc_ChangeChannels(),
                adc_ReadCH()
				
*******************************************************************************/

#ifndef __AD79X8_H
#define __AD79X8_H

#include "defines.h"


#ifdef __cplusplus
extern "C" {
#endif

#define AD79x8_READFAIL (0x7fff)

//Raw ADC Read/Write Functions:
RBAPI(unsigned) ad79x8_RawRead(void);
//-- if the above function return AD79x8_READFAIL, roboio_GetErrCode() may return:
     #define ERROR_ADCFAIL           (ERR_NOERROR + 310)

RBAPI(bool) ad79x8_RawWrite(unsigned val);
RBAPI(bool) ad7908_WriteCTRLREG(unsigned ctrlreg);
//-- if the above functions return false, roboio_GetErrCode() may return:
//   #define ERROR_ADCFAIL           (ERR_NOERROR + 310)


RBAPI(bool) ad79x8_InUse(void);

RBAPI(bool) ad79x8_Initialize(int channel, int range, int coding);
//-- values for the "range" augment
     #define AD79x8MODE_RANGE_VREF         (0)
     #define AD79x8MODE_RANGE_2VREF        (1)
//-- values for the "coding" augment
     #define AD79x8MODE_CODING_UNSIGNED    (0)
     #define AD79x8MODE_CODING_SIGNED      (1)
//-- if the above function return false, roboio_GetErrCode() may return:
//   #define ERROR_ADCFAIL           (ERR_NOERROR + 310)
     #define ERROR_ADC_NOSPI		 (ERR_NOERROR + 331)
     #define ERROR_ADC_INUSE         (ERR_NOERROR + 332)
     #define ERROR_ADC_WRONGCHANNEL  (ERR_NOERROR + 311)

RBAPI(bool) ad79x8_Close(void);
//-- if the above function return false, roboio_GetErrCode() may return:
//   #define ERROR_ADCFAIL           (ERR_NOERROR + 310)

RBAPI(bool) ad79x8_ChangeChannel(int channel, int range, int coding);
//-- if the above function return false, roboio_GetErrCode() may return:
//   #define ERROR_ADCFAIL           (ERR_NOERROR + 310)
//   #define ERROR_ADC_WRONGCHANNEL  (ERR_NOERROR + 311)


RBAPI(bool) ad79x8_InitializeMCH(unsigned char usedchannels, int range, int coding);
//-- values for the "usedchannels" argument of the above functions (note that
//   different values can be added to apply multiple channels)
     #define AD79x8_USECHANNEL0	     (1<<7)
     #define AD79x8_USECHANNEL1		 (1<<6)
     #define AD79x8_USECHANNEL2	     (1<<5)
     #define AD79x8_USECHANNEL3		 (1<<4)
     #define AD79x8_USECHANNEL4	     (1<<3)
     #define AD79x8_USECHANNEL5      (1<<2)
     #define AD79x8_USECHANNEL6      (1<<1)
     #define AD79x8_USECHANNEL7      (1<<0)
//-- if the above function return false, roboio_GetErrCode() may return:
//   #define ERROR_ADCFAIL           (ERR_NOERROR + 310)
//   #define ERROR_ADC_NOSPI		 (ERR_NOERROR + 331)
//   #define ERROR_ADC_INUSE         (ERR_NOERROR + 332)

RBAPI(bool) ad79x8_ChangeChannels(unsigned char usedchannels, int range, int coding);
//-- values for the "usedchannels" argument of the above functions (note that
//   different values can be added to apply multiple channels)
//   #define AD79x8_USECHANNEL0	     (1<<7)
//   #define AD79x8_USECHANNEL1		 (1<<6)
//   #define AD79x8_USECHANNEL2	     (1<<5)
//   #define AD79x8_USECHANNEL3		 (1<<4)
//   #define AD79x8_USECHANNEL4	     (1<<3)
//   #define AD79x8_USECHANNEL5      (1<<2)
//   #define AD79x8_USECHANNEL6      (1<<1)
//   #define AD79x8_USECHANNEL7      (1<<0)
//-- if the above function return false, roboio_GetErrCode() may return:
//   #define ERROR_ADCFAIL           (ERR_NOERROR + 310)



#ifdef ROBOIO

    #define ADC_READFAIL    (AD79x8_READFAIL)

    // Channel-by-Channel Reading Function:
    // --------------------------------------------
    RBAPI(int) adc_ReadChannel(int channel, int range, int coding);  //don't use in continuous mode
    //-- if the above function return ADC_READFAIL, roboio_GetErrCode() may return:
    //   #define ERROR_ADCFAIL           (ERR_NOERROR + 310)
    //   #define ERROR_ADC_NOSPI		 (ERR_NOERROR + 331)
    
    
    // Single-Channel Batch Mode Functions:
    // --------------------------------------------
    // adc_Initialize() (inlined)        --- cf. ad79x8_Initialize()
    // adc_ChangeChannel() (inlined)     --- cf. ad79x8_ChangeChannel()
    //-- values for the "range" augment
         #define ADCMODE_RANGE_VREF     (AD79x8MODE_RANGE_VREF)
         #define ADCMODE_RANGE_2VREF    (AD79x8MODE_RANGE_2VREF)
    //-- values for the "coding" augment
         #define ADCMODE_UNSIGNEDCODING (AD79x8MODE_CODING_UNSIGNED)
         #define ADCMODE_SIGNEDCODING   (AD79x8MODE_CODING_SIGNED)
    
    // adc_Close() (inlined)             --- cf. ad79x8_Close()
    // adc_InUse() (inlined)             --- cf. ad79x8_InUse()
    
    RBAPI(int) adc_Read(void);
    //-- if the above function return ADC_READFAIL, roboio_GetErrCode() may return:
    //   #define ERROR_ADCFAIL            (ERR_NOERROR + 310)
    
    
    // Multi-Channels Batch Mode Functions:
    // --------------------------------------------
    // adc_InitializeMCH() (inlined)     --- cf. ad79x8_InitializeMCH()
    // adc_ChangeChannels() (inlined)    --- cf. ad79x8_ChangeChannels()
    //-- values for the "usedchannels" argument of the above functions
         #define ADC_USECHANNEL0	      (AD79x8_USECHANNEL0)
         #define ADC_USECHANNEL1		  (AD79x8_USECHANNEL1)
         #define ADC_USECHANNEL2	      (AD79x8_USECHANNEL2)
         #define ADC_USECHANNEL3		  (AD79x8_USECHANNEL3)
         #define ADC_USECHANNEL4	      (AD79x8_USECHANNEL4)
         #define ADC_USECHANNEL5          (AD79x8_USECHANNEL5)
         #define ADC_USECHANNEL6          (AD79x8_USECHANNEL6)
         #define ADC_USECHANNEL7          (AD79x8_USECHANNEL7)
    
    // adc_CloseMCH() (inlined)          --- cf. ad79x8_Close()
    
    RBAPI(int*) adc_ReadMCH(void);
    RBAPI(void) adc_ReadMCH2(int* buf);
#endif



/*******************************  AD7908  *********************************/

#define AD7908_READFAIL (AD79x8_READFAIL)

// AD7908 Channel-by-Channel Reading Function:
// --------------------------------------------
RBAPI(int) ad7908_ReadChannel(int channel, int range, int coding);  //don't use in batch mode
//-- if the above function return AD7908_READFAIL, roboio_GetErrCode() may return:
//   #define ERROR_ADC_NOSPI		 (ERR_NOERROR + 331)
//   #define ERROR_ADCFAIL           (ERR_NOERROR + 310)


// AD7908 Single-Channel Batch Mode Functions:
// --------------------------------------------
// ad7908_Initialize() (inlined)        --- cf. ad79x8_Initialize()
// ad7908_ChangeChannel() (inlined)     --- cf. ad79x8_ChangeChannel()
//-- values for the "range" augment
     #define AD7908MODE_RANGE_VREF    (AD79x8MODE_RANGE_VREF)
     #define AD7908MODE_RANGE_2VREF   (AD79x8MODE_RANGE_2VREF)
//-- values for the "coding" augment
     #define AD7908MODE_CODING_255    (AD79x8MODE_CODING_UNSIGNED)
     #define AD7908MODE_CODING_127    (AD79x8MODE_CODING_SIGNED)

// ad7908_Close() (inlined)             --- cf. ad79x8_Close()
// ad7908_InUse() (inlined)             --- cf. ad79x8_InUse()

RBAPI(int) ad7908_Read(void);
//-- if the above function return AD7908_READFAIL, roboio_GetErrCode() may return:
//   #define ERROR_ADCFAIL            (ERR_NOERROR + 310)


// AD7908 Multi-Channels Batch Mode Functions:
// --------------------------------------------
// ad7908_InitializeMCH() (inlined)     --- cf. ad79x8_InitializeMCH()
// ad7908_ChangeChannels() (inlined)    --- cf. ad79x8_ChangeChannels()
//-- values for the "usedchannels" argument of the above functions
     #define AD7908_USECHANNEL0	      (AD79x8_USECHANNEL0)
     #define AD7908_USECHANNEL1		  (AD79x8_USECHANNEL1)
     #define AD7908_USECHANNEL2	      (AD79x8_USECHANNEL2)
     #define AD7908_USECHANNEL3		  (AD79x8_USECHANNEL3)
     #define AD7908_USECHANNEL4	      (AD79x8_USECHANNEL4)
     #define AD7908_USECHANNEL5       (AD79x8_USECHANNEL5)
     #define AD7908_USECHANNEL6       (AD79x8_USECHANNEL6)
     #define AD7908_USECHANNEL7       (AD79x8_USECHANNEL7)

// ad7908_CloseMCH() (inlined)          --- cf. ad79x8_Close()

RBAPI(int*) ad7908_ReadMCH(void);
RBAPI(void) ad7908_ReadMCH2(int* buf);
/*---------------------------  end of AD7908  ----------------------------*/



/*******************************  AD7918  *********************************/

#define AD7918_READFAIL (AD79x8_READFAIL)

// AD7918 Channel-by-Channel Reading Function:
// --------------------------------------------
RBAPI(int) ad7918_ReadChannel(int channel, int range, int coding);  //don't use in batch mode
//-- if the above function return AD7918_READFAIL, roboio_GetErrCode() may return:
//   #define ERROR_ADCFAIL           (ERR_NOERROR + 310)
//   #define ERROR_ADC_NOSPI		 (ERR_NOERROR + 331)


// AD7918 Single-Channel Batch Mode Functions:
// --------------------------------------------
// ad7918_Initialize() (inlined)        --- cf. ad79x8_Initialize()
// ad7918_ChangeChannel() (inlined)     --- cf. ad79x8_ChangeChannel()
//-- values for the "range" augment
     #define AD7918MODE_RANGE_VREF    (AD79x8MODE_RANGE_VREF)
     #define AD7918MODE_RANGE_2VREF   (AD79x8MODE_RANGE_2VREF)
//-- values for the "coding" augment
     #define AD7918MODE_CODING_1023   (AD79x8MODE_CODING_UNSIGNED)
     #define AD7918MODE_CODING_511    (AD79x8MODE_CODING_SIGNED)

// ad7918_Close() (inlined)             --- cf. ad79x8_Close()
// ad7918_InUse() (inlined)             --- cf. ad79x8_InUse()

RBAPI(int) ad7918_Read(void);
//-- if the above function return AD7918_READFAIL, roboio_GetErrCode() may return:
//   #define ERROR_ADCFAIL            (ERR_NOERROR + 310)


// AD7918 Multi-Channels Batch Mode Functions:
// --------------------------------------------
// ad7918_InitializeMCH() (inlined)     --- cf. ad79x8_InitializeMCH()
// ad7918_ChangeChannels() (inlined)    --- cf. ad79x8_ChangeChannels()
//-- values for the "usedchannels" argument of the above functions
     #define AD7918_USECHANNEL0	      (AD79x8_USECHANNEL0)
     #define AD7918_USECHANNEL1		  (AD79x8_USECHANNEL1)
     #define AD7918_USECHANNEL2	      (AD79x8_USECHANNEL2)
     #define AD7918_USECHANNEL3		  (AD79x8_USECHANNEL3)
     #define AD7918_USECHANNEL4	      (AD79x8_USECHANNEL4)
     #define AD7918_USECHANNEL5       (AD79x8_USECHANNEL5)
     #define AD7918_USECHANNEL6       (AD79x8_USECHANNEL6)
     #define AD7918_USECHANNEL7       (AD79x8_USECHANNEL7)

// ad7918_CloseMCH() (inlined)          --- cf. ad79x8_Close()

RBAPI(int*) ad7918_ReadMCH(void);
RBAPI(void) ad7918_ReadMCH2(int* buf);
/*---------------------------  end of AD7918  ----------------------------*/



/*******************************  AD7928  *********************************/

#define AD7928_READFAIL (AD79x8_READFAIL)

// AD7928 Channel-by-Channel Reading Function:
// --------------------------------------------
RBAPI(int) ad7928_ReadChannel(int channel, int range, int coding);  //don't use in continuous mode
//-- if the above function return AD7928_READFAIL, roboio_GetErrCode() may return:
//   #define ERROR_ADCFAIL           (ERR_NOERROR + 310)
//   #define ERROR_ADC_NOSPI		 (ERR_NOERROR + 331)


// AD7928 Single-Channel Batch Mode Functions:
// --------------------------------------------
// ad7928_Initialize() (inlined)        --- cf. ad79x8_Initialize()
// ad7928_ChangeChannel() (inlined)     --- cf. ad79x8_ChangeChannel()
//-- values for the "range" augment
     #define AD7928MODE_RANGE_VREF    (AD79x8MODE_RANGE_VREF)
     #define AD7928MODE_RANGE_2VREF   (AD79x8MODE_RANGE_2VREF)
//-- values for the "coding" augment
     #define AD7928MODE_CODING_4095   (AD79x8MODE_CODING_UNSIGNED)
     #define AD7928MODE_CODING_2047   (AD79x8MODE_CODING_SIGNED)

// ad7928_Close() (inlined)             --- cf. ad79x8_Close()
// ad7928_InUse() (inlined)             --- cf. ad79x8_InUse()

RBAPI(int) ad7928_Read(void);
//-- if the above function return AD7928_READFAIL, roboio_GetErrCode() may return:
//   #define ERROR_ADCFAIL            (ERR_NOERROR + 310)


// AD7928 Multi-Channels Batch Mode Functions:
// --------------------------------------------
// ad7928_InitializeMCH() (inlined)     --- cf. ad79x8_InitializeMCH()
// ad7928_ChangeChannels() (inlined)    --- cf. ad79x8_ChangeChannels()
//-- values for the "usedchannels" argument of the above functions
     #define AD7928_USECHANNEL0	      (AD79x8_USECHANNEL0)
     #define AD7928_USECHANNEL1		  (AD79x8_USECHANNEL1)
     #define AD7928_USECHANNEL2	      (AD79x8_USECHANNEL2)
     #define AD7928_USECHANNEL3		  (AD79x8_USECHANNEL3)
     #define AD7928_USECHANNEL4	      (AD79x8_USECHANNEL4)
     #define AD7928_USECHANNEL5       (AD79x8_USECHANNEL5)
     #define AD7928_USECHANNEL6       (AD79x8_USECHANNEL6)
     #define AD7928_USECHANNEL7       (AD79x8_USECHANNEL7)

// ad7928_CloseMCH() (inlined)          --- cf. ad79x8_Close()

RBAPI(int*) ad7928_ReadMCH(void);
RBAPI(void) ad7928_ReadMCH2(int* buf);
/*---------------------------  end of AD7928  ----------------------------*/

#ifdef __cplusplus
}
#endif



/****************************  Inline Functions  **************************/
#ifdef ROBOIO_DLL //use no inline functions for DLL
#ifdef __cplusplus
extern "C" {
#endif

    #ifdef ROBOIO
        RBAPI(bool) adc_InUse(void);
        RBAPI(bool) adc_Initialize(int channel, int range, int coding);
        RBAPI(bool) adc_Close(void);
        RBAPI(bool) adc_ChangeChannel(int channel, int range, int coding);
        RBAPI(bool) adc_InitializeMCH(unsigned char usedchannels, int range, int coding);
        RBAPI(bool) adc_CloseMCH(void);
        RBAPI(bool) adc_ChangeChannels(unsigned char usedchannels, int range, int coding);
        RBAPI(int)  adc_ReadCH(int channel);
    #endif

    RBAPI(bool) ad7908_InUse(void);
    RBAPI(bool) ad7918_InUse(void);
    RBAPI(bool) ad7928_InUse(void);

    RBAPI(bool) ad7908_Initialize(int channel, int range, int coding);
    RBAPI(bool) ad7918_Initialize(int channel, int range, int coding);
    RBAPI(bool) ad7928_Initialize(int channel, int range, int coding);

    RBAPI(bool) ad7908_Close(void);
    RBAPI(bool) ad7918_Close(void);
    RBAPI(bool) ad7928_Close(void);

    RBAPI(bool) ad7908_ChangeChannel(int channel, int range, int coding);
    RBAPI(bool) ad7918_ChangeChannel(int channel, int range, int coding);
    RBAPI(bool) ad7928_ChangeChannel(int channel, int range, int coding);

    RBAPI(bool) ad7908_InitializeMCH(unsigned char usedchannels, int range, int coding);
    RBAPI(bool) ad7918_InitializeMCH(unsigned char usedchannels, int range, int coding);
    RBAPI(bool) ad7928_InitializeMCH(unsigned char usedchannels, int range, int coding);

    RBAPI(bool) ad7908_CloseMCH(void);
    RBAPI(bool) ad7918_CloseMCH(void);
    RBAPI(bool) ad7928_CloseMCH(void);

    RBAPI(bool) ad7908_ChangeChannels(unsigned char usedchannels, int range, int coding);
    RBAPI(bool) ad7918_ChangeChannels(unsigned char usedchannels, int range, int coding);
    RBAPI(bool) ad7928_ChangeChannels(unsigned char usedchannels, int range, int coding);
#ifdef __cplusplus
}
#endif
#endif

#if !defined(ROBOIO_DLL) || defined(__AD79X8_LIB)

    #ifdef ROBOIO
        RB_INLINE RBAPI(bool) adc_InUse(void) {
            return ad79x8_InUse();
        }
        RB_INLINE RBAPI(bool) adc_Initialize(int channel, int range, int coding) {
            return ad79x8_Initialize(channel, range, coding);
        }
        RB_INLINE RBAPI(bool) adc_Close(void) {
            return ad79x8_Close();
        }
        RB_INLINE RBAPI(bool) adc_ChangeChannel(int channel, int range, int coding) {
            return ad79x8_ChangeChannel(channel, range, coding);
        }
        RB_INLINE RBAPI(bool) adc_InitializeMCH(unsigned char usedchannels, int range, int coding) {
            return ad79x8_InitializeMCH(usedchannels, range, coding);
        }
        RB_INLINE RBAPI(bool) adc_CloseMCH(void) {
            return ad79x8_Close();
        }
        RB_INLINE RBAPI(bool) adc_ChangeChannels(unsigned char usedchannels, int range, int coding) {
            return ad79x8_ChangeChannels(usedchannels, range, coding);
        }
        RB_INLINE RBAPI(int) adc_ReadCH(int channel) {
            return adc_ReadChannel(channel, ADCMODE_RANGE_2VREF, ADCMODE_UNSIGNEDCODING);
        }
    #endif

    RB_INLINE RBAPI(bool) ad7908_InUse(void) {
        return ad79x8_InUse();
    }
    RB_INLINE RBAPI(bool) ad7918_InUse(void) {
        return ad79x8_InUse();
    }
    RB_INLINE RBAPI(bool) ad7928_InUse(void) {
        return ad79x8_InUse();
    }

    RB_INLINE RBAPI(bool) ad7908_Initialize(int channel, int range, int coding) {
        return ad79x8_Initialize(channel, range, coding);
    }
    RB_INLINE RBAPI(bool) ad7918_Initialize(int channel, int range, int coding) {
        return ad79x8_Initialize(channel, range, coding);
    }
    RB_INLINE RBAPI(bool) ad7928_Initialize(int channel, int range, int coding) {
        return ad79x8_Initialize(channel, range, coding);
    }

    RB_INLINE RBAPI(bool) ad7908_Close(void) {
        return ad79x8_Close();
    }
    RB_INLINE RBAPI(bool) ad7918_Close(void) {
        return ad79x8_Close();
    }
    RB_INLINE RBAPI(bool) ad7928_Close(void) {
        return ad79x8_Close();
    }

    RB_INLINE RBAPI(bool) ad7908_ChangeChannel(int channel, int range, int coding) {
        return ad79x8_ChangeChannel(channel, range, coding);
    }
    RB_INLINE RBAPI(bool) ad7918_ChangeChannel(int channel, int range, int coding) {
        return ad79x8_ChangeChannel(channel, range, coding);
    }
    RB_INLINE RBAPI(bool) ad7928_ChangeChannel(int channel, int range, int coding) {
        return ad79x8_ChangeChannel(channel, range, coding);
    }

    RB_INLINE RBAPI(bool) ad7908_InitializeMCH(unsigned char usedchannels, int range, int coding) {
        return ad79x8_InitializeMCH(usedchannels, range, coding);
    }
    RB_INLINE RBAPI(bool) ad7918_InitializeMCH(unsigned char usedchannels, int range, int coding) {
        return ad79x8_InitializeMCH(usedchannels, range, coding);
    }
    RB_INLINE RBAPI(bool) ad7928_InitializeMCH(unsigned char usedchannels, int range, int coding) {
        return ad79x8_InitializeMCH(usedchannels, range, coding);
    }

    RB_INLINE RBAPI(bool) ad7908_CloseMCH(void) {
        return ad79x8_Close();
    }
    RB_INLINE RBAPI(bool) ad7918_CloseMCH(void) {
        return ad79x8_Close();
    }
    RB_INLINE RBAPI(bool) ad7928_CloseMCH(void) {
        return ad79x8_Close();
    }

    RB_INLINE RBAPI(bool) ad7908_ChangeChannels(unsigned char usedchannels, int range, int coding) {
        return ad79x8_ChangeChannels(usedchannels, range, coding);
    }
    RB_INLINE RBAPI(bool) ad7918_ChangeChannels(unsigned char usedchannels, int range, int coding) {
        return ad79x8_ChangeChannels(usedchannels, range, coding);
    }
    RB_INLINE RBAPI(bool) ad7928_ChangeChannels(unsigned char usedchannels, int range, int coding) {
        return ad79x8_ChangeChannels(usedchannels, range, coding);
    }
#endif
/*-----------------------  end of Inline Functions  ----------------------*/

#endif

