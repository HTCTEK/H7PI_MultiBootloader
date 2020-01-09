/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_storage_if.c
  * @version        : v1.0_Cube
  * @brief          : Memory management layer.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_storage_if.h"

/* USER CODE BEGIN INCLUDE */
#include "diskio.h"
#include "spiflash.h"
#include "qspiflash.h"
#include "bsp_driver_sd.h"
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
extern DRESULT SD_read(BYTE lun, BYTE *buff, DWORD sector, UINT count);
extern DRESULT SD_write(BYTE lun, BYTE *buff, DWORD sector, UINT count);
extern DSTATUS SD_initialize(BYTE lun);
/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @defgroup USBD_STORAGE
  * @brief Usb mass storage device module
  * @{
  */

/** @defgroup USBD_STORAGE_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Defines
  * @brief Private defines.
  * @{
  */

#define STORAGE_LUN_NBR                  2
#define STORAGE_BLK_NBR                  0x10000
#define STORAGE_BLK_SIZ                  0x200

/* USER CODE BEGIN PRIVATE_DEFINES */
#if STORAGE_LUN_NBR==1
#undef STORAGE_LUN_NBR
#define STORAGE_LUN_NBR 2
#endif


/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Variables
  * @brief Private variables.
  * @{
  */

/* USER CODE BEGIN INQUIRY_DATA_FS */
/** USB Mass storage Standard Inquiry Data. */
const int8_t STORAGE_Inquirydata_FS[] = {/* 36 */
  
  /* LUN 0 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,	
  0x00,
  'H', 'T', 'C', 'T', 'E', 'K', ' ', ' ', /* Manufacturer : 8 bytes */
  'S', 'p', 'i', ' ', 'F', 'l', 'a', 's', 'h', /* Product      : 16 Bytes */
  ' ', 'D', 'i', 's', 'k', ' ', ' ',
  '0', '.', '0' ,'1',                      /* Version      : 4 Bytes */

	
	/* LUN 1 */
	0x00,
	0x80,		
	0x02,		
	0x02,
	(STANDARD_INQUIRY_DATA_LEN - 5),
	0x00,
	0x00,	
	0x00,
	/* Vendor Identification */
	'H', 'T', 'C', 'T', 'E', 'K', ' ', ' ',	//8字节
	/* Product Identification */				
  'S', 'D', ' ', 'C', 'a', 'r', 'd', ' ', 'D', 'i',//16字节
	's','k', ' ', ' ', ' ', ' ',
    /* Product Revision Level */
  '0', '.', '0' ,'1',                      /* Version      : 4 Bytes */
}; 
/* USER CODE END INQUIRY_DATA_FS */

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */
extern SD_HandleTypeDef hsd2;
/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t STORAGE_Init_FS(uint8_t lun);
static int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady_FS(uint8_t lun);
static int8_t STORAGE_IsWriteProtected_FS(uint8_t lun);
static int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS(void);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_StorageTypeDef USBD_Storage_Interface_fops_FS =
{
  STORAGE_Init_FS,
  STORAGE_GetCapacity_FS,
  STORAGE_IsReady_FS,
  STORAGE_IsWriteProtected_FS,
  STORAGE_Read_FS,
  STORAGE_Write_FS,
  STORAGE_GetMaxLun_FS,
  (int8_t *)STORAGE_Inquirydata_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes over USB FS IP
  * @param  lun:
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Init_FS(uint8_t lun)
{
  /* USER CODE BEGIN 2 */
	switch(lun)
	{
		case 0://SPI FLASH
			W25Q64_SPI_Init();
			break;
		case 1://SD卡
			BSP_SD_Init();
			break; 
	}
  return (USBD_OK);
  /* USER CODE END 2 */
}

/**
  * @brief  .
  * @param  lun: .
  * @param  block_num: .
  * @param  block_size: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
  /* USER CODE BEGIN 3 */
	HAL_SD_CardInfoTypeDef info;
	switch(lun)
	{
		case 0:
			*block_size=512;  
			*block_num=1024*1024*8/512; //8M spi flash
			break;
		case 1:
			HAL_SD_GetCardInfo(&hsd2,&info);
			*block_num = info.LogBlockNbr - 1;
			*block_size = info.LogBlockSize;
			break;
	}
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_IsReady_FS(uint8_t lun)
{
  /* USER CODE BEGIN 4 */
	HAL_SD_CardInfoTypeDef info;
	switch(lun)
	{
		case 0:
			if(W25Q64_SPI_ReadID() == SPI_FLASH_TYPE)
			{
				return (USBD_OK);
			}
			else
			{
				return (USBD_FAIL);
			}
			break;
		case 1: 
			if(HAL_SD_GetCardState(&hsd2) == HAL_SD_CARD_TRANSFER)
			{
				return (USBD_OK);
			}
			else
			{	
				return (USBD_FAIL);
			}
			break;
	}
	return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_IsWriteProtected_FS(uint8_t lun)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  /* USER CODE BEGIN 6 */
	switch(lun)
	{
		case 0:
			W25Q64_SPI_Read(buf,blk_addr*512,blk_len*512);
			break;
		case 1://SD卡
			BSP_SD_ReadBlocks((uint32_t *)buf, blk_addr, blk_len, 1000);
			break; 
	} 
  return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  /* USER CODE BEGIN 7 */
	switch(lun)
	{
		case 0://spi FLASH
			W25Q64_SPI_Write(buf,blk_addr*512,blk_len*512);
			break;
		case 1://SD卡
			BSP_SD_WriteBlocks((uint32_t *)buf, blk_addr, blk_len, 1000);
			break; 
	}
  return (USBD_OK);
  /* USER CODE END 7 */
}

/**
  * @brief  .
  * @param  None
  * @retval .
  */
int8_t STORAGE_GetMaxLun_FS(void)
{
  /* USER CODE BEGIN 8 */
	HAL_SD_CardInfoTypeDef info;
  HAL_SD_GetCardInfo(&hsd2,&info);
  //
	//sd 卡是否存在得判断
  if(info.LogBlockNbr)return STORAGE_LUN_NBR;
	else return STORAGE_LUN_NBR-1;
  /* USER CODE END 8 */
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
