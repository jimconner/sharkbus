/*******************************************************************************
  Update List

   2010/05/18:

    - [by acen] rename the following functions' names from "spi_..." to "spidx_..."

	            spi_Write(), spi_WriteFlush(),
				spi_FIFOFlush(), spi_Read(), spi_Read2(), spi_ReadStrict()

	- [by acen] Add spisw_Exchange()

*******************************************************************************/

#ifndef __SPIDX_H
#define __SPIDX_H

#include "defines.h"


#ifdef __cplusplus
extern "C" {
#endif

RBAPI(void) spi_SetBaseAddress(unsigned baseaddr);
RBAPI(unsigned) spi_SetDefaultBaseAddress(void);

RBAPI(bool) spi_SetControlREG(unsigned char ctrlreg);
//-- if the above function returns false, roboio_GetErrCode() may return:
     #define ERROR_SPIBUSY           (ERR_NOERROR + 210)

RBAPI(void) spi_SetErrorREG(unsigned char errreg);

RBAPI(unsigned char) spi_ReadControlREG(void);
RBAPI(unsigned char) spi_ReadStatusREG(void);
RBAPI(unsigned char) spi_ReadCSREG(void);
RBAPI(unsigned char) spi_ReadErrorREG(void);

RBAPI(bool) spi_EnableFIFO(void);
RBAPI(bool) spi_DisableFIFO(void);
RBAPI(bool) spi_SetClockDivisor(unsigned char clkdiv);
//-- if the above functions return false, roboio_GetErrCode() may return:
     #define ERROR_SPIWRONGCLOCK     (ERR_NOERROR + 211)


RBAPI(bool) spi_InputReady(void);
RBAPI(bool) spi_OutputFIFOEmpty(void);
RBAPI(bool) spi_OutputFIFOFull(void);
RBAPI(bool) spi_Busy(void);

RBAPI(bool) spierror_SPIBusy(void);
RBAPI(bool) spierror_InputOverlap(void);
RBAPI(bool) spierror_FIFOUnderrun(void);
RBAPI(bool) spierror_FIFOOverrun(void);

RBAPI(void) spierror_ClearSPIBusy(void);
RBAPI(void) spierror_ClearInputOverlap(void);
RBAPI(void) spierror_ClearFIFOUnderrun(void);
RBAPI(void) spierror_ClearFIFOOverrun(void);
RBAPI(void) spi_ClearErrors(void);


RBAPI(bool) spidx_Write(unsigned char val);
RBAPI(bool) spidx_WriteFlush(unsigned char val);
//-- if the above functions return false, roboio_GetErrCode() may return:
     #define ERROR_SPIFIFOFULL      (ERR_NOERROR + 214)
     #define ERROR_SPIFIFOFAIL      (ERR_NOERROR + 215)

RBAPI(bool) spidx_FIFOFlush(void);
//-- if the above function returns false, roboio_GetErrCode() may return:
//   #define ERROR_SPIFIFOFAIL      (ERR_NOERROR + 215)

RBAPI(unsigned) spidx_Read(void);
//-- if the above function returns 0xffff, roboio_GetErrCode() may return:
//   #define ERROR_SPIFIFOFAIL      (ERR_NOERROR + 215)
     #define ERROR_SPIREADFAIL      (ERR_NOERROR + 219)

RBAPI(unsigned) spidx_Read2(void);
RBAPI(unsigned) spidx_ReadStrict(void); //avoid to read overdue data in SPI input buffer
//-- if the above functions return 0xffff, roboio_GetErrCode() may return:
//   #define ERROR_SPIREADFAIL      (ERR_NOERROR + 219)


RBAPI(unsigned) spisw_Exchange(unsigned char val, int mode, unsigned long delay);
//-- values for the "mode" argument (note that different values
//   can be added to use mixed modes)
	 #define SPIMODE_CPOL0    (0x00)
	 #define SPIMODE_CPOL1    (0x04)
	 #define SPIMODE_SMOD0    (0x00)
	 #define SPIMODE_SMOD1    (0x02)
	 #define SPIMODE_CPHA0    (0x00)
	 #define SPIMODE_CPHA1    (0x01)
//-- if the above functions return 0xffff, roboio_GetErrCode() may return:
     #define ERROR_SPISW_INVALIDMODE    (ERR_NOERROR + 230)

#ifdef __cplusplus
}
#endif

#endif

