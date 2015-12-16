/*******************************************************************************
  Update List

   2010/08/25:

    - [by AAA] Add the following functions

				io_SetISASpeed(), io_EnableIRQ(), io_DisableIRQ(), 
				read_sb_regw(), write_sb_regw(), read_nb_regw(), write_nb_regw()
				
				
*******************************************************************************/

#ifndef __IO_H
#define __IO_H

#include "defines.h"

#if defined(RB_MSVC_WIN32)  //currently used in io.cpp & rcservo.cpp
    #define USE_WINIO
    //#define USE_PCIDEBUG
#endif

#ifdef __cplusplus
extern "C" {
#endif

RBAPI(bool) io_InUse(void);

RBAPI(int)  io_Init(void);
RBAPI(bool) io_init(void); //only for backward compatibility to RoBoIO v1.0
//-- if return -1 or false, roboio_GetErrCode() may return:
     #define ERROR_IOINITFAIL		(ERR_NOERROR + 100)
     #define ERROR_IOSECTIONFULL	(ERR_NOERROR + 101)
     #define ERROR_CPUUNSUPPORTED	(ERR_NOERROR + 102)

RBAPI(void) io_Close(int section);
RBAPI(void) io_close(void); //only for backward compatibility to RoBoIO v1.0


RBAPI(void) io_outpdw(unsigned addr, unsigned long val);
RBAPI(unsigned long) io_inpdw(unsigned addr);

RBAPI(void) io_outpw(unsigned addr, unsigned val);
RBAPI(unsigned) io_inpw(unsigned addr);

RBAPI(void) io_outpb(unsigned addr, unsigned char val);
RBAPI(unsigned char) io_inpb(unsigned addr);

RBAPI(void) io_SetISASpeed(int mode);
//-- values for the "mode" argument
     #define ISAMODE_NORMAL	           (0)
     #define ISAMODE_HIGHSPEED	       (1)

RBAPI(int) io_CpuID(void);
//-- return-values of io_CpuID()
     #define CPU_UNSUPPORTED	       (0)
     #define CPU_VORTEX86SX		       (10)
     #define CPU_VORTEX86DX_1	       (21)
     #define CPU_VORTEX86DX_2	       (22)
     #define CPU_VORTEX86DX_3	       (23)
     #define CPU_VORTEX86DX_UNKNOWN    (24)
     #define CPU_VORTEX86MX            (30)
     #define CPU_VORTEX86MX_PLUS       (33)

RBAPI(void) io_EnableIRQ(void);
RBAPI(void) io_DisableIRQ(void);

#ifdef __cplusplus
}
#endif


/****************************  Inline Functions  **************************/
#ifdef ROBOIO_DLL //use no inline functions for DLL
#ifdef __cplusplus
extern "C" {
#endif
    RBAPI(unsigned char) read_sb_regb(unsigned char idx);
    RBAPI(unsigned) read_sb_regw(unsigned char idx);
    RBAPI(unsigned long) read_sb_reg(unsigned char idx);
    RBAPI(void) write_sb_regb(unsigned char idx, unsigned char val);
    RBAPI(void) write_sb_regw(unsigned char idx, unsigned val);      //idx must = 0 (mod 2)
    RBAPI(void) write_sb_reg(unsigned char idx, unsigned long val);  //idx must = 0 (mod 4)

    RBAPI(unsigned char) read_nb_regb(unsigned char idx);
    RBAPI(unsigned) read_nb_regw(unsigned char idx);
    RBAPI(unsigned long) read_nb_reg(unsigned char idx);
    RBAPI(void) write_nb_regb(unsigned char idx, unsigned char val);
    RBAPI(void) write_nb_regw(unsigned char idx, unsigned val);      //idx must = 0 (mod 2)
    RBAPI(void) write_nb_reg(unsigned char idx, unsigned long val);  //idx must = 0 (mod 4)
#ifdef __cplusplus
}
#endif
#endif

#if !defined(ROBOIO_DLL) || defined(__IO_LIB)
    // read south bridge register
    RB_INLINE RBAPI(unsigned char) read_sb_regb(unsigned char idx) {
        unsigned char tmp;

        io_DisableIRQ();
	    io_outpdw(0x0cf8, (0x80003800L+(unsigned long)idx) & 0xfffffffcL);
        tmp = 0xff & (unsigned char)(io_inpdw(0x0cfc) >> ((idx & 0x03) * 8));
        io_EnableIRQ();

        return tmp;
    }

    RB_INLINE RBAPI(unsigned) read_sb_regw(unsigned char idx) {
        unsigned tmp;

        io_DisableIRQ();
	    io_outpdw(0x0cf8, (0x80003800L+(unsigned long)idx) & 0xfffffffcL);
        tmp = 0xffff & (unsigned)(io_inpdw(0x0cfc) >> ((idx & 0x03) * 8));
        io_EnableIRQ();

        return tmp;
    }

    RB_INLINE RBAPI(unsigned long) read_sb_reg(unsigned char idx) {
        unsigned long tmp;

        io_DisableIRQ();
	    io_outpdw(0x0cf8, (0x80003800L+(unsigned long)idx) & 0xfffffffcL);
        tmp = io_inpdw(0x0cfc);
        io_EnableIRQ();

        return tmp;
    }

    // write south bridge register
    RB_INLINE RBAPI(void) write_sb_regb(unsigned char idx, unsigned char val) {
	    int i = (idx & 0x03) * 8;

        io_DisableIRQ();
	    io_outpdw(0x0cf8, (0x80003800L+(unsigned long)idx) & 0xfffffffcL);
	    io_outpdw(0x0cfc, (io_inpdw(0x0cfc) & (~(0x000000ffL << i))) | ((unsigned long)val << i));
        io_EnableIRQ();
    }

    RB_INLINE RBAPI(void) write_sb_regw(unsigned char idx, unsigned val) {
	    int i = (idx & 0x03) * 8;

        io_DisableIRQ();
	    io_outpdw(0x0cf8, (0x80003800L+(unsigned long)idx) & 0xfffffffcL);
	    io_outpdw(0x0cfc, (io_inpdw(0x0cfc) & (~(0x0000ffffL << i))) | ((unsigned long)val << i));
        io_EnableIRQ();
    }

    RB_INLINE RBAPI(void) write_sb_reg(unsigned char idx, unsigned long val) {
        io_DisableIRQ();
	    io_outpdw(0x0cf8, (0x80003800L+(unsigned long)idx) & 0xfffffffcL);
	    io_outpdw(0x0cfc, val);
        io_EnableIRQ();
    }


    // read north bridge register
    RB_INLINE RBAPI(unsigned char) read_nb_regb(unsigned char idx) {
        unsigned char tmp;

        io_DisableIRQ();
	    io_outpdw(0x0cf8, (0x80000000L+(unsigned long)idx) & 0xfffffffcL);
        tmp = 0xff & (unsigned char)(io_inpdw(0x0cfc) >> ((idx & 0x03) * 8));
        io_EnableIRQ();

        return tmp;
    }

    RB_INLINE RBAPI(unsigned) read_nb_regw(unsigned char idx) {
        unsigned tmp;

        io_DisableIRQ();
	    io_outpdw(0x0cf8, (0x80000000L+(unsigned long)idx) & 0xfffffffcL);
        tmp = 0xffff & (unsigned)(io_inpdw(0x0cfc) >> ((idx & 0x03) * 8));
        io_EnableIRQ();

        return tmp;
    }

    RB_INLINE RBAPI(unsigned long) read_nb_reg(unsigned char idx) {
        unsigned long tmp;

        io_DisableIRQ();
	    io_outpdw(0x0cf8, (0x80000000L+(unsigned long)idx) & 0xfffffffcL);
        tmp = io_inpdw(0x0cfc);
        io_EnableIRQ();

        return tmp;
    }

    // write north bridge register
    RB_INLINE RBAPI(void) write_nb_regb(unsigned char idx, unsigned char val) {
	    int i = (idx & 0x03) * 8;

        io_DisableIRQ();
	    io_outpdw(0x0cf8, (0x80000000L+(unsigned long)idx) & 0xfffffffcL);
	    io_outpdw(0x0cfc, (io_inpdw(0x0cfc) & (~(0x000000ffL << i))) | ((unsigned long)val << i));
        io_EnableIRQ();
    }

    RB_INLINE RBAPI(void) write_nb_regw(unsigned char idx, unsigned val) {
	    int i = (idx & 0x03) * 8;

        io_DisableIRQ();
	    io_outpdw(0x0cf8, (0x80000000L+(unsigned long)idx) & 0xfffffffcL);
	    io_outpdw(0x0cfc, (io_inpdw(0x0cfc) & (~(0x0000ffffL << i))) | ((unsigned long)val << i));
        io_EnableIRQ();
    }

    RB_INLINE RBAPI(void) write_nb_reg(unsigned char idx, unsigned long val) {
        io_DisableIRQ();
	    io_outpdw(0x0cf8, (0x80000000L+(unsigned long)idx) & 0xfffffffcL);
	    io_outpdw(0x0cfc, val);
        io_EnableIRQ();
     }
#endif
/*-----------------------  end of Inline Functions  ----------------------*/

#endif

