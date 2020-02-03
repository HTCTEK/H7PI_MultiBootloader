/**
  ******************************************************************************
  * File Name          : QUADSPI.h
  * Description        : This file provides code for the configuration
  *                      of the QUADSPI instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __w25q64_qspi_H
#define __w25q64_qspi_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define W25X_WriteEnable              0x06
#define W25X_WriteDisable             0x04
#define W25X_ReadStatusReg            0x05
#define W25X_WriteStatusReg           0x01
#define W25X_ReadData_32Bit           0x13    //32Bit Address
#define W25X_ReadData_24Bit           0x03    //24Bit Address
#define W25X_FastReadData             0x0B
#define W25X_FastReadQuad             0xEB
#define W25X_PageProgramQuad          0x32
#define W25X_FastReadDual             0x3B
#define W25X_PageProgram              0x02
#define W25X_BlockErase               0xD8
#define W25X_SectorErase              0x20
#define W25X_ChipErase                0xC7
#define W25X_PowerDown                0xB9
#define W25X_ReleasePowerDown         0xAB
#define W25X_DeviceID                 0xAB
#define W25X_ManufactDeviceID         0x90
#define W25X_JedecDeviceID            0x9F
#define W25X_ReadStatusReg1           0x05
#define W25X_WriteStatusReg1          0x01
#define W25X_ReadStatusReg2           0x35
#define W25X_WriteStatusReg2          0x31
#define W25X_ReadStatusReg3           0x15
#define W25X_WriteStatusReg3          0x11
#define W25X_ResetEnable              0x66
#define W25X_Reset                    0x99

#define S25X_BRRD                     0x16
#define S25X_BRWR                     0x17

#define MT25X_ReadNonvolatileConfig   0xB5
#define MT25X_WriteNonvolatileConfig  0xB1
#define MT25X_Enter_4_ByteAddress     0xB7
#define MT25X_Exit_4_ByteAddress      0xE9


#define WIP_Flag                      0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte                    0xFF
void W25Q64_QSPI_ResetEnable(void);
void W25Q64_QSPI_Reset(void);

void W25Q64_QSPI_Init(void);
void W25Q64_QSPI_WriteEnable(void);
void W25Q64_QSPI_WriteDisable(void);
unsigned char W25Q64_QSPI_ReadSR(unsigned char SR);
void W25Q64_QSPI_WaitBusy(void);
void W25Q64_QSPI_WriteSR(unsigned char *SR,unsigned char NumSR);
void W25Q64_QSPI_QuadWriteEnable(void);
void W25Q64_QSPI_QuadWriteDisable(void);
void W25Q64_QSPI_EraseSector(unsigned long SectorAddress);
void W25Q64_QSPI_EraseBlock32KB(unsigned long Block32KBAddress);
void W25Q64_QSPI_EraseBlock64KB(unsigned long Block64KBAddress);
void W25Q64_QSPI_EraseChip(void);
void W25Q64_QSPI_ReadMode(unsigned char ReadModeCmd);
void W25Q64_QSPI_Read(unsigned long Address,unsigned char *buff,unsigned long NumBytes);
void W25Q64_QSPI_ReadPage(unsigned long Address,unsigned char *buff);
void W25Q64_QSPI_Write(unsigned long Address,unsigned char *buff,unsigned long NumBytes);
void W25Q64_QSPI_WritePage(unsigned long Address,unsigned char *buff,unsigned long NumBytes);
void W25Q64_QSPI_WriteBlock4KB(unsigned long Block4KBAdress,unsigned char *buff);
void W25Q64_QSPI_ReadBlock4KB(unsigned long Block4KBAdress,unsigned char *buff);
unsigned long W25Q64_QSPI_ReadID(void);
void W25QXX_Memory_Mapped_Enable(void);


#ifdef __cplusplus
}
#endif
#endif /*__ quadspi_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
