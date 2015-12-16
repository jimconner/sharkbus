#ifndef __I2C_H
#define __I2C_H

#include "defines.h"
#include "i2cdx.h"


#ifdef __cplusplus
extern "C" {
#endif

RBAPI(bool) i2c_InUse(void);

//note: I2C lib assumes that I/O pins for GPIO3/I2C have functioned as GPIO3 or I2C pins;
//      this is BIOS default setting in general.
RBAPI(bool) i2c_Initialize2(unsigned devs, int i2c0irq, int i2c1irq);
//-- values for the "devs" argument (note that different values
//   can be added to use multiple modules)
     #define I2C_USEMODULE0     (1<<0)
     #define I2C_USEMODULE1     (1<<1)
//-- values for the "i2c0irq" and "i2c1irq" arguments (having been defined in "i2cdx.h")
//   #define I2CIRQ1         (8)
//   #define I2CIRQ3         (2)
//   #define I2CIRQ4         (4)
//   #define I2CIRQ5         (5)
//   #define I2CIRQ6         (7)
//   #define I2CIRQ7         (6)
//   #define I2CIRQ9         (1)
//   #define I2CIRQ10        (3)
//   #define I2CIRQ11        (9)
//   #define I2CIRQ12        (11)
//   #define I2CIRQ14        (13)
//   #define I2CIRQ15        (15)
//   #define I2CIRQ_DISABLE  (0)
//-- if the above functions return false, roboio_GetErrCode() may return:
//   #define ERROR_IOINITFAIL       (ERR_NOERROR + 100) //need include <io.h>
//   #define ERROR_IOSECTIONFULL    (ERR_NOERROR + 101) //need include <io.h>
//   #define ERROR_CPUUNSUPPORTED	(ERR_NOERROR + 102) //need include <io.h>
     #define ERROR_I2C_INUSE		(ERR_NOERROR + 600)
     #define ERROR_I2C_INITFAIL		(ERR_NOERROR + 602)

RBAPI(void) i2c_Close(void);


RBAPI(unsigned long) i2c_SetSpeed(int dev, int mode, unsigned long bps);
//-- values for the "mode" argument
//   #define I2CMODE_STANDARD       (0)                 //need include <i2cdx.h>
//   #define I2CMODE_FAST           (1)                 //need include <i2cdx.h>
     #define I2CMODE_HIGHSPEED      (2)
     #define I2CMODE_AUTO           (3)
//-- if the above functions return 0xffffffffL, roboio_GetErrCode() may return:
     #define ERROR_I2CWRONGUSAGE    (ERR_NOERROR + 630)



/*******************  Master Functions for Individual Module *****************/
RBAPI(bool) i2cmaster_Start(int dev, unsigned char addr, unsigned char rwbit);
//-- values for the "rwbit" argument (having been defined in "i2cdx.h")
//   #define I2C_WRITE              (0)
//   #define I2C_READ               (1)
//-- if the above functions return false, roboio_GetErrCode() may return:
//   #define ERROR_I2CFAIL		    (ERR_NOERROR + 610)
//   #define ERROR_I2CWRONGUSAGE    (ERR_NOERROR + 630)
     #define ERROR_I2CARLOSS        (ERR_NOERROR + 621)
     #define ERROR_I2CACKERR        (ERR_NOERROR + 622)

RBAPI(bool) i2cmaster_Write(int dev, unsigned char val);
RBAPI(bool) i2cmaster_WriteLast(int dev, unsigned char val);
//-- if the above functions return false, roboio_GetErrCode() may return:
//   #define ERROR_I2CFAIL		    (ERR_NOERROR + 610)
//   #define ERROR_I2CWRONGUSAGE    (ERR_NOERROR + 630)
//   #define ERROR_I2CARLOSS        (ERR_NOERROR + 621)
//   #define ERROR_I2CACKERR        (ERR_NOERROR + 622)

RBAPI(unsigned) i2cmaster_Read1(int dev, bool secondlast);
RBAPI(unsigned) i2cmaster_ReadLast(int dev);
//-- if the above functions return 0xffff, roboio_GetErrCode() may return:
//   #define ERROR_I2CFAIL		    (ERR_NOERROR + 610)
//   #define ERROR_I2CWRONGUSAGE    (ERR_NOERROR + 630)
//   #define ERROR_I2CARLOSS        (ERR_NOERROR + 621)


RBAPI(bool) i2cmaster_StartN(int dev, unsigned char addr, unsigned char rwbit, int count);
//-- values for the "rwbit" argument (having been defined in "i2cdx.h")
//   #define I2C_WRITE              (0)
//   #define I2C_READ               (1)
//-- if the above functions return false, roboio_GetErrCode() may return:
//   #define ERROR_I2CFAIL		    (ERR_NOERROR + 610)
//   #define ERROR_I2CWRONGUSAGE    (ERR_NOERROR + 630)
//   #define ERROR_I2CARLOSS        (ERR_NOERROR + 621)
//   #define ERROR_I2CACKERR        (ERR_NOERROR + 622)

RBAPI(bool) i2cmaster_WriteN(int dev, unsigned char val);
//-- if the above functions return false, roboio_GetErrCode() may return:
//   #define ERROR_I2CFAIL		    (ERR_NOERROR + 610)
//   #define ERROR_I2CWRONGUSAGE    (ERR_NOERROR + 630)
//   #define ERROR_I2CARLOSS        (ERR_NOERROR + 621)
//   #define ERROR_I2CACKERR        (ERR_NOERROR + 622)

RBAPI(unsigned) i2cmaster_ReadN(int dev);
//-- if the above functions return 0xffff, roboio_GetErrCode() may return:
//   #define ERROR_I2CFAIL		    (ERR_NOERROR + 610)
//   #define ERROR_I2CWRONGUSAGE    (ERR_NOERROR + 630)
//   #define ERROR_I2CARLOSS        (ERR_NOERROR + 621)


RBAPI(bool) i2cmaster_SetRestart(int dev, unsigned char addr, unsigned char rwbit);
RBAPI(bool) i2cmaster_SetRestartN(int dev, unsigned char rwbit, int count);
//-- if the above functions return false, roboio_GetErrCode() may return:
//   #define ERROR_I2CWRONGUSAGE    (ERR_NOERROR + 630)
/*----------------------  end of Master Functions  --------------------------*/



/********************  Slave Functions for Individual Module *****************/
RBAPI(unsigned) i2cslave_Listen(int dev);
//-- return values of the above functions
     #define I2CSLAVE_NOTHING       (0)
     #define I2CSLAVE_START         (1)
     #define I2CSLAVE_END           (2)
     #define I2CSLAVE_WAITING       (3)
     #define I2CSLAVE_WRITEREQUEST  (4)
     #define I2CSLAVE_READREQUEST   (5)
//-- if the aboves functions return 0xffff, roboio_GetErrCode() may return:
//   #define ERROR_I2CFAIL		    (ERR_NOERROR + 610)
//   #define ERROR_I2CWRONGUSAGE    (ERR_NOERROR + 630)

RBAPI(bool) i2cslave_Write(int dev, unsigned char val);
//-- if the above functions return false, roboio_GetErrCode() may return:
//   #define ERROR_I2CFAIL		    (ERR_NOERROR + 610)
//   #define ERROR_I2CWRONGUSAGE    (ERR_NOERROR + 630)

RBAPI(unsigned) i2cslave_Read(int dev);
//-- if the above function return 0xffff, roboio_GetErrCode() may return:
//   #define ERROR_I2CWRONGUSAGE    (ERR_NOERROR + 630)
/*-----------------------  end of Slave Functions  --------------------------*/

#ifdef __cplusplus
}
#endif



/****************************  Inline Functions  **************************/
#ifdef ROBOIO_DLL //use no inline functions for DLL
#ifdef __cplusplus
extern "C" {
#endif
    RBAPI(bool) i2c_Initialize(int i2cirq);

    RBAPI(unsigned long) i2c0_SetSpeed(int mode, unsigned long bps);
    RBAPI(unsigned long) i2c1_SetSpeed(int mode, unsigned long bps);

    RBAPI(bool) i2c0master_Start(unsigned char addr, unsigned char rwbit);
    RBAPI(bool) i2c1master_Start(unsigned char addr, unsigned char rwbit);

    RBAPI(bool) i2c0master_Write(unsigned char val);
    RBAPI(bool) i2c1master_Write(unsigned char val);
    RBAPI(bool) i2c0master_WriteLast(unsigned char val);
    RBAPI(bool) i2c1master_WriteLast(unsigned char val);
    
    RBAPI(unsigned) i2c0master_Read(void);
    RBAPI(unsigned) i2c1master_Read(void);
    RBAPI(unsigned) i2c0master_ReadSecondLast(void);
    RBAPI(unsigned) i2c1master_ReadSecondLast(void);
    RBAPI(unsigned) i2c0master_ReadLast(void);
    RBAPI(unsigned) i2c1master_ReadLast(void);
    
    RBAPI(bool) i2c0master_StartN(unsigned char addr, unsigned char rwbit, int count);
    RBAPI(bool) i2c1master_StartN(unsigned char addr, unsigned char rwbit, int count);

    RBAPI(bool) i2c0master_WriteN(unsigned char val);
    RBAPI(bool) i2c1master_WriteN(unsigned char val);

    RBAPI(unsigned) i2c0master_ReadN(void);
    RBAPI(unsigned) i2c1master_ReadN(void);
    
    RBAPI(bool) i2c0master_SetRestart(unsigned char addr, unsigned char rwbit);
    RBAPI(bool) i2c1master_SetRestart(unsigned char addr, unsigned char rwbit);
    RBAPI(bool) i2c0master_SetRestartN(unsigned char rwbit, int count);
    RBAPI(bool) i2c1master_SetRestartN(unsigned char rwbit, int count);

    RBAPI(unsigned) i2c0slave_Listen(void);
    RBAPI(unsigned) i2c1slave_Listen(void);

    RBAPI(bool) i2c0slave_Write(unsigned char val);
    RBAPI(bool) i2c1slave_Write(unsigned char val);

    RBAPI(unsigned) i2c0slave_Read(void);
    RBAPI(unsigned) i2c1slave_Read(void);
#ifdef __cplusplus
}
#endif
#endif

#if !defined(ROBOIO_DLL) || defined(__I2C_LIB)
    RB_INLINE RBAPI(bool) i2c_Initialize(int i2cirq) {
        return i2c_Initialize2(I2C_USEMODULE0, i2cirq, I2CIRQ_DISABLE);
    }

    RB_INLINE RBAPI(unsigned long) i2c0_SetSpeed(int mode, unsigned long bps) {
        return i2c_SetSpeed(0, mode, bps);
    }
    RB_INLINE RBAPI(unsigned long) i2c1_SetSpeed(int mode, unsigned long bps) {
        return i2c_SetSpeed(1, mode, bps);
    }

    RB_INLINE RBAPI(bool) i2c0master_Start(unsigned char addr, unsigned char rwbit) {
        return i2cmaster_Start(0, addr, rwbit);
    }
    RB_INLINE RBAPI(bool) i2c1master_Start(unsigned char addr, unsigned char rwbit) {
        return i2cmaster_Start(1, addr, rwbit);
    }

    RB_INLINE RBAPI(bool) i2c0master_Write(unsigned char val) {
        return i2cmaster_Write(0, val);
    }
    RB_INLINE RBAPI(bool) i2c1master_Write(unsigned char val) {
        return i2cmaster_Write(1, val);
    }
    RB_INLINE RBAPI(bool) i2c0master_WriteLast(unsigned char val) {
        return i2cmaster_WriteLast(0, val);
    }
    RB_INLINE RBAPI(bool) i2c1master_WriteLast(unsigned char val) {
        return i2cmaster_WriteLast(1, val);
    }

    RB_INLINE RBAPI(unsigned) i2c0master_Read(void) {
        return i2cmaster_Read1(0, false);
    }
    RB_INLINE RBAPI(unsigned) i2c1master_Read(void) {
        return i2cmaster_Read1(1, false);
    }
    RB_INLINE RBAPI(unsigned) i2c0master_ReadSecondLast(void) {
        return i2cmaster_Read1(0, true);
    }
    RB_INLINE RBAPI(unsigned) i2c1master_ReadSecondLast(void) {
        return i2cmaster_Read1(1, true);
    }
    RB_INLINE RBAPI(unsigned) i2c0master_ReadLast(void) {
        return i2cmaster_ReadLast(0);
    }
    RB_INLINE RBAPI(unsigned) i2c1master_ReadLast(void) {
        return i2cmaster_ReadLast(1);
    }

    RB_INLINE RBAPI(bool) i2c0master_StartN(unsigned char addr, unsigned char rwbit, int count) {
        return i2cmaster_StartN(0, addr, rwbit, count);
    }
    RB_INLINE RBAPI(bool) i2c1master_StartN(unsigned char addr, unsigned char rwbit, int count) {
        return i2cmaster_StartN(1, addr, rwbit, count);
    }

    RB_INLINE RBAPI(bool) i2c0master_WriteN(unsigned char val) {
        return i2cmaster_WriteN(0, val);
    }
    RB_INLINE RBAPI(bool) i2c1master_WriteN(unsigned char val) {
        return i2cmaster_WriteN(1, val);
    }

    RB_INLINE RBAPI(unsigned) i2c0master_ReadN(void) {
        return i2cmaster_ReadN(0);
    }
    RB_INLINE RBAPI(unsigned) i2c1master_ReadN(void) {
        return i2cmaster_ReadN(1);
    }

    RB_INLINE RBAPI(bool) i2c0master_SetRestart(unsigned char addr, unsigned char rwbit) {
        return i2cmaster_SetRestart(0, addr, rwbit);
    }
    RB_INLINE RBAPI(bool) i2c1master_SetRestart(unsigned char addr, unsigned char rwbit) {
        return i2cmaster_SetRestart(1, addr, rwbit);
    }
    RB_INLINE RBAPI(bool) i2c0master_SetRestartN(unsigned char rwbit, int count) {
        return i2cmaster_SetRestartN(0, rwbit, count);
    }
    RB_INLINE RBAPI(bool) i2c1master_SetRestartN(unsigned char rwbit, int count) {
        return i2cmaster_SetRestartN(1, rwbit, count);
    }

    RB_INLINE RBAPI(unsigned) i2c0slave_Listen(void) {
        return i2cslave_Listen(0);
    }
    RB_INLINE RBAPI(unsigned) i2c1slave_Listen(void) {
        return i2cslave_Listen(1);
    }

    RB_INLINE RBAPI(bool) i2c0slave_Write(unsigned char val) {
        return i2cslave_Write(0, val);
    }
    RB_INLINE RBAPI(bool) i2c1slave_Write(unsigned char val) {
        return i2cslave_Write(1, val);
    }

    RB_INLINE RBAPI(unsigned) i2c0slave_Read(void) {
        return i2cslave_Read(0);
    }
    RB_INLINE RBAPI(unsigned) i2c1slave_Read(void) {
        return i2cslave_Read(1);
    }
#endif
/*-----------------------  end of Inline Functions  ----------------------*/

#endif

