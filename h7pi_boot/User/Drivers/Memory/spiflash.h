#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__
 
/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif
	
extern  SPI_HandleTypeDef hspi2;
#define hspi hspi2

/* USER CODE BEGIN Includes */
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18

#define W25Q64_CS_Pin GPIO_PIN_12
#define W25Q64_CS_GPIO_Port GPIOB
	 
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg1		0x05 
#define W25X_ReadStatusReg2		0x35 
#define W25X_ReadStatusReg3		0x15 
#define W25X_WriteStatusReg1    0x01 
#define W25X_WriteStatusReg2    0x31 
#define W25X_WriteStatusReg3    0x11 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			  0xD8 
#define W25X_SectorErase		  0x20 
#define W25X_ChipErase			  0xC7 
#define W25X_PowerDown			  0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			    0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9


/****************************************************************
这里为了兼容fatfs和windows上U盘的文件系统
使用512的sector size
实际参数应该是
#define   SPI_FLASH_SECTOR_SIZE      4096
#define   SPI_FLASH_BLOCK_SIZE       128 
#define   SPI_FLASH_SECTOR_COUNT   	 2048
*****************************************************************/
#define   SPI_FLASH_TYPE W25Q64	//默认是W25Q64
#define   SPI_FLASH_SECTOR_SIZE      512
#define   SPI_FLASH_BLOCK_SIZE       128 
#define   SPI_FLASH_SECTOR_COUNT   	 16384
/* USER CODE END Includes */


extern void _Error_Handler(char *, int);
uint8_t W25Q64_SPI_Init(void);
uint32_t W25Q64_SPI_ReadID(void);
uint8_t W25Q64_SPI_Read_SR(uint8_t regno);
void W25Q64_SPI_Write_SR(uint8_t regno,uint8_t sr);
void W25Q64_SPI_Write_Enable(void);
void W25Q64_SPI_Write_Disable(void);
uint8_t W25Q64_SPI_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);
uint8_t W25Q64_SPI_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void W25Q64_SPI_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void W25Q64_SPI_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);   
void W25Q64_SPI_Erase_Chip(void);
void W25Q64_SPI_Erase_Sector(uint32_t Dst_Addr);
void W25Q64_SPI_PowerDown(void);
void W25Q64_SPI_Wake_Up(void);

	
#ifdef __cplusplus
}
#endif

#endif
