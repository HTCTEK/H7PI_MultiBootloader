#include "qspiflash.h"
#include "spiflash.h"
#include "cJSON.h"
#include "fatfs.h"
#include "led.h"

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_storage_if.h"

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
/******************************************************************

内部声明变量和函数

******************************************************************/
//
//中断映射
#define VECT_TAB_OFFSET      0x00000000UL 
//
//应用程序启动地址
#define APPLICATION_ADDRESS  (uint32_t)0x90000000
//
//跳转指针
typedef  void (*pFunction)(void);
pFunction JumpToApplication;
//
//路径索引
#define SDCARD    0
#define SPIFLASH  1
//
//文件系统挂载状态
uint8_t sys_fatfs_mount_status = 0;


/******************************************************************

外部声明变量和函数


******************************************************************/
extern uint8_t retSD; /* Return value for SD */
extern char SDPath[4]; /* SD logical drive path */
extern FATFS SDFatFS; /* File system object for SD logical drive */
extern FIL SDFile; /* File object for SD */
extern uint8_t retUSER; /* Return value for USER */
extern char USERPath[4]; /* USER logical drive path */
extern FATFS USERFatFS; /* File system object for USER logical drive */
extern FIL USERFile; /* File object for USER */


extern void MX_FATFS_Init(void);
extern void MX_USB_DEVICE_Init(void);
extern USBD_HandleTypeDef hUsbDeviceFS;
/******************************************************************

软件CRC 计算

******************************************************************/
 
const uint32_t crc32_tbl[256] = {
    0x00000000L, 0x77073096L, 0xEE0E612CL, 0x990951BAL, 0x076DC419L,
    0x706AF48FL, 0xE963A535L, 0x9E6495A3L, 0x0EDB8832L, 0x79DCB8A4L,
    0xE0D5E91EL, 0x97D2D988L, 0x09B64C2BL, 0x7EB17CBDL, 0xE7B82D07L,
    0x90BF1D91L, 0x1DB71064L, 0x6AB020F2L, 0xF3B97148L, 0x84BE41DEL,
    0x1ADAD47DL, 0x6DDDE4EBL, 0xF4D4B551L, 0x83D385C7L, 0x136C9856L,
    0x646BA8C0L, 0xFD62F97AL, 0x8A65C9ECL, 0x14015C4FL, 0x63066CD9L,
    0xFA0F3D63L, 0x8D080DF5L, 0x3B6E20C8L, 0x4C69105EL, 0xD56041E4L,
    0xA2677172L, 0x3C03E4D1L, 0x4B04D447L, 0xD20D85FDL, 0xA50AB56BL,
    0x35B5A8FAL, 0x42B2986CL, 0xDBBBC9D6L, 0xACBCF940L, 0x32D86CE3L,
    0x45DF5C75L, 0xDCD60DCFL, 0xABD13D59L, 0x26D930ACL, 0x51DE003AL,
    0xC8D75180L, 0xBFD06116L, 0x21B4F4B5L, 0x56B3C423L, 0xCFBA9599L,
    0xB8BDA50FL, 0x2802B89EL, 0x5F058808L, 0xC60CD9B2L, 0xB10BE924L,
    0x2F6F7C87L, 0x58684C11L, 0xC1611DABL, 0xB6662D3DL, 0x76DC4190L,
    0x01DB7106L, 0x98D220BCL, 0xEFD5102AL, 0x71B18589L, 0x06B6B51FL,
    0x9FBFE4A5L, 0xE8B8D433L, 0x7807C9A2L, 0x0F00F934L, 0x9609A88EL,
    0xE10E9818L, 0x7F6A0DBBL, 0x086D3D2DL, 0x91646C97L, 0xE6635C01L,
    0x6B6B51F4L, 0x1C6C6162L, 0x856530D8L, 0xF262004EL, 0x6C0695EDL,
    0x1B01A57BL, 0x8208F4C1L, 0xF50FC457L, 0x65B0D9C6L, 0x12B7E950L,
    0x8BBEB8EAL, 0xFCB9887CL, 0x62DD1DDFL, 0x15DA2D49L, 0x8CD37CF3L,
    0xFBD44C65L, 0x4DB26158L, 0x3AB551CEL, 0xA3BC0074L, 0xD4BB30E2L,
    0x4ADFA541L, 0x3DD895D7L, 0xA4D1C46DL, 0xD3D6F4FBL, 0x4369E96AL,
    0x346ED9FCL, 0xAD678846L, 0xDA60B8D0L, 0x44042D73L, 0x33031DE5L,
    0xAA0A4C5FL, 0xDD0D7CC9L, 0x5005713CL, 0x270241AAL, 0xBE0B1010L,
    0xC90C2086L, 0x5768B525L, 0x206F85B3L, 0xB966D409L, 0xCE61E49FL,
    0x5EDEF90EL, 0x29D9C998L, 0xB0D09822L, 0xC7D7A8B4L, 0x59B33D17L,
    0x2EB40D81L, 0xB7BD5C3BL, 0xC0BA6CADL, 0xEDB88320L, 0x9ABFB3B6L,
    0x03B6E20CL, 0x74B1D29AL, 0xEAD54739L, 0x9DD277AFL, 0x04DB2615L,
    0x73DC1683L, 0xE3630B12L, 0x94643B84L, 0x0D6D6A3EL, 0x7A6A5AA8L,
    0xE40ECF0BL, 0x9309FF9DL, 0x0A00AE27L, 0x7D079EB1L, 0xF00F9344L,
    0x8708A3D2L, 0x1E01F268L, 0x6906C2FEL, 0xF762575DL, 0x806567CBL,
    0x196C3671L, 0x6E6B06E7L, 0xFED41B76L, 0x89D32BE0L, 0x10DA7A5AL,
    0x67DD4ACCL, 0xF9B9DF6FL, 0x8EBEEFF9L, 0x17B7BE43L, 0x60B08ED5L,
    0xD6D6A3E8L, 0xA1D1937EL, 0x38D8C2C4L, 0x4FDFF252L, 0xD1BB67F1L,
    0xA6BC5767L, 0x3FB506DDL, 0x48B2364BL, 0xD80D2BDAL, 0xAF0A1B4CL,
    0x36034AF6L, 0x41047A60L, 0xDF60EFC3L, 0xA867DF55L, 0x316E8EEFL,
    0x4669BE79L, 0xCB61B38CL, 0xBC66831AL, 0x256FD2A0L, 0x5268E236L,
    0xCC0C7795L, 0xBB0B4703L, 0x220216B9L, 0x5505262FL, 0xC5BA3BBEL,
    0xB2BD0B28L, 0x2BB45A92L, 0x5CB36A04L, 0xC2D7FFA7L, 0xB5D0CF31L,
    0x2CD99E8BL, 0x5BDEAE1DL, 0x9B64C2B0L, 0xEC63F226L, 0x756AA39CL,
    0x026D930AL, 0x9C0906A9L, 0xEB0E363FL, 0x72076785L, 0x05005713L,
    0x95BF4A82L, 0xE2B87A14L, 0x7BB12BAEL, 0x0CB61B38L, 0x92D28E9BL,
    0xE5D5BE0DL, 0x7CDCEFB7L, 0x0BDBDF21L, 0x86D3D2D4L, 0xF1D4E242L,
    0x68DDB3F8L, 0x1FDA836EL, 0x81BE16CDL, 0xF6B9265BL, 0x6FB077E1L,
    0x18B74777L, 0x88085AE6L, 0xFF0F6A70L, 0x66063BCAL, 0x11010B5CL,
    0x8F659EFFL, 0xF862AE69L, 0x616BFFD3L, 0x166CCF45L, 0xA00AE278L,
    0xD70DD2EEL, 0x4E048354L, 0x3903B3C2L, 0xA7672661L, 0xD06016F7L,
    0x4969474DL, 0x3E6E77DBL, 0xAED16A4AL, 0xD9D65ADCL, 0x40DF0B66L,
    0x37D83BF0L, 0xA9BCAE53L, 0xDEBB9EC5L, 0x47B2CF7FL, 0x30B5FFE9L,
    0xBDBDF21CL, 0xCABAC28AL, 0x53B39330L, 0x24B4A3A6L, 0xBAD03605L,
    0xCDD70693L, 0x54DE5729L, 0x23D967BFL, 0xB3667A2EL, 0xC4614AB8L,
    0x5D681B02L, 0x2A6F2B94L, 0xB40BBE37L, 0xC30C8EA1L, 0x5A05DF1BL,
    0x2D02EF8DL
};
 
void crc32_Init(uint32_t *crc)
{
   *crc = 0xFFFFFFFFL;
}
 
void crc32_Update(uint32_t *crc, const uint8_t *data, size_t len)
{
	for (size_t i = 0; i < len; i++)
	{
		 *crc = (*crc >> 8) ^ crc32_tbl[(*crc & 0xFF) ^  *data++];
	}
}
 
void crc32_Final(uint32_t *crc)
{
	(*crc) ^= 0xFFFFFFFFUL;
}

/***********************************************************
遍历删除非空文件夹

************************************************************/
FRESULT f_DelInterFile(char* path)
{
    UINT i, j;
    FRESULT res;
    DIR dir;
    FILINFO fno;

    res = f_opendir(&dir, path);
    if (res == FR_OK)
    {
        for (i = 0; path[i]; i++) ;
        path[i++] = '/';
        for (;;)
        {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || !fno.fname[0]) break;
            if (fno.fname[0] == '.') continue;
            j = 0;
            do
            path[i+j] = fno.fname[j];
            while (fno.fname[j++]);
            if (fno.fattrib & AM_DIR)
            {
                res = f_DelInterFile(path);
                if (res != FR_OK) break;
            }
            res = f_unlink(path);

            if ((res != FR_OK) && (res != FR_DENIED)) break;
        }
        path[--i] = '\0';
    }
    return res;
}

/* 删除文件夹或文件 */
FRESULT f_del(char* path)
{
    FRESULT res;
    res = f_DelInterFile(path);
    if (res == FR_OK)
    {
        res = f_unlink(path);
    }
    else if (FR_NO_PATH == res)
    {
        res = f_unlink(path);
    }
		HAL_Delay(1000);
    return res;
}
/***********************************************************
LED闪烁任务

************************************************************/
uint8_t num = 0;
void led_flash(uint8_t _num)
{
	num = _num;
}

void StartLedTask(void const * argument)
{
	while(1)
	{
			switch(num)
			{
				case 5:
					led(2,1);
					led(1,1);
					break;
				case 0:
					led(2,0);
					led(1,0);
					break;
				case 1:		
					led(2,0);
					led(1,1);
					HAL_Delay(100);
					led(1,0);
					HAL_Delay(100);
					led(1,1);
					HAL_Delay(100);
					led(1,0);
					HAL_Delay(100);
					break;
				case 2:		
					led(1,0);
					led(2,1);
					HAL_Delay(100);
					led(2,0);
					HAL_Delay(100);
					led(2,1);
					HAL_Delay(100);
					led(2,0);
					HAL_Delay(100);
					break;
				case 3:		
					led(1,1);
					led(2,1);
					HAL_Delay(100);
					led(1,0);
					led(2,0);
					HAL_Delay(100);
					led(1,1);
					led(2,1);
					HAL_Delay(100);
					led(1,0);
					led(2,0);
					HAL_Delay(1000);
					break;
				case 4:		
					led(1,1);
					led(2,1);
					HAL_Delay(100);
					led(1,0);
					led(2,0);
					HAL_Delay(100);
					led(1,1);
					led(2,1);
					HAL_Delay(100);
					led(1,0);
					led(2,0);
					HAL_Delay(100);
					break;
			}
	}
}

/***********************************************************
文件系统挂载

************************************************************/
void sys_FatfsMount(void)
{
	__IO FRESULT  res = FR_OK;
	BYTE work[_MAX_SS]={0};	 
	uint8_t sys_sdcard_mount_status = 0;
	uint8_t sys_spiflash_mount_status = 0;
	//
	//初始化文件系统
   MX_FATFS_Init();
	//
	//挂载SPI FLASH文件系统
	led(2,1);
	HAL_Delay(100);
	led(2,0);
	HAL_Delay(100);
	res = f_mount(&USERFatFS, (TCHAR const*)USERPath, 1);
	if(res != FR_OK)
	{
		if(res == FR_NO_FILESYSTEM)
		{
			res = f_mkfs(USERPath, FM_ANY,0 , work, sizeof work);
		}
	}
	else
	{
		sys_spiflash_mount_status = 1;
	}
	//
	//挂载sdcard文件系统
	led(1,1);
	HAL_Delay(100);
	led(1,0);
	HAL_Delay(100);
	res = f_mount(&SDFatFS, (TCHAR const*)SDPath, 1);
	if(res != FR_OK)
	{
		if(res == FR_NO_FILESYSTEM)
		{
			res = f_mkfs(SDPath, FM_FAT32,0 , work, sizeof work);
		}
	}else//sd card挂载成功
	{
		sys_sdcard_mount_status = 1;
	}
	sys_fatfs_mount_status = sys_sdcard_mount_status | (sys_spiflash_mount_status<<1);
}
/***********************************************************
从映射得地址开始进行固件校验
addr: 内存映射地址，映射固件位置
len：crc长度
crc：校对得crc值
return：0-校验失败，1-校验成功
************************************************************/
uint8_t sys_VerifyFw(const uint8_t* addr, uint32_t len,uint32_t crc)
{
	uint32_t crcvalue = 0;
	crc32_Init(&crcvalue);
	crc32_Update(&crcvalue, addr, len);
	crc32_Final(&crcvalue);
	if(crcvalue == crc)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/***********************************************************
固件更新
src: 选择固件来源，0-sdcard，1-spiflash
************************************************************/
uint8_t sys_FwUpdate(uint8_t src)
{
	char* sys_default_config_file[2]  = {"0:/update/fw.json","1:/update/fw.json"};
	char* sys_default_update_path[2]  = {"0:/update","1:/update"};
	__IO FRESULT  res = FR_OK;
	uint32_t  index = 0;
	uint32_t  crcValue = 0;
	uint32_t  address = 0;
	uint8_t 	led_state = 0;
	//
	//文件buffer
	FIL       configfile;
	FIL       fwfile;
	char      filebuffer[512]={0};
	char      crcbuffer[10240]={0};
	//
	//配置文件信息
	cJSON*    config_json = NULL;
	cJSON*    version_json = NULL;
	char      version[32]={0};
	cJSON*    path_json = NULL;
	char      path[256]={0};
	cJSON*    crc_json = NULL;
	char      crc[32]={0};
	char      crcVal[32] = {0}; 
	//
	//
	//查找配置spi flash 内的文件和固件信息
	led(src+1,1);
	HAL_Delay(100);
	led(src+1,0);
	HAL_Delay(100);
	if((sys_fatfs_mount_status&(src+1))==1) //挂在成功，打开配置文件文件
	{
		return 0;
	}
	else
	{		
		res = f_open(&configfile,sys_default_config_file[src],FA_READ | FA_OPEN_EXISTING);
		if(res != FR_OK) 
		{
			f_close(&configfile);f_del(sys_default_update_path[src]); 
			return 0;
		}
		else //打开成功，读取文件
		{ 
			res = f_read(&configfile,filebuffer,sizeof filebuffer,(uint32_t*)&index);
			if(res != FR_OK)
			{
				f_close(&configfile);f_del(sys_default_update_path[src]); 
				return 0;
			}
			else //读取成功，获取版本号，路径，CRC32值
			{
				config_json = cJSON_Parse((const char*)filebuffer);
				//查找版本号
				version_json = cJSON_GetObjectItem(config_json, "version");
				//查找路径
				path_json = cJSON_GetObjectItem(config_json, "path");
				//查找CRC32值
				crc_json = cJSON_GetObjectItem(config_json, "crc32");
				//复制信息到缓存区
				strcpy(path,path_json->valuestring);
				strcpy(version,version_json->valuestring);
				strcpy(crc,crc_json->valuestring);
				f_close(&configfile);
				if(path_json==NULL || version_json==NULL || crc_json == NULL)
				{
					cJSON_free(config_json);//cJSON_free(path_json);cJSON_free(version_json);cJSON_free(crc_json);
					f_del(sys_default_update_path[src]); 
					return 0;
				}
				else  //根据之前获取的信息打开固件
				{
					cJSON_free(config_json);//cJSON_free(path_json);cJSON_free(version_json);cJSON_free(crc_json);
					res = f_open(&fwfile,path,FA_READ);
					if(res != FR_OK)
					{
						f_close(&fwfile);f_del(sys_default_update_path[src]); 
						return 0;
					}
					else  //读取固件并进行CRC32计算
					{
						index =0;
						crc32_Init(&crcValue);
						while(1)
						{
							led(src+1,led_state);
							HAL_Delay(100);
							led(src+1,!led_state);
							HAL_Delay(100);
							memset(crcbuffer,0,sizeof crcbuffer);
							res = f_read(&fwfile,crcbuffer,(sizeof crcbuffer),(uint32_t*)&index);
							if(res != FR_OK)
							{
								f_close(&fwfile);f_del(sys_default_update_path[src]); 
								return 0;
							}
							else
							{	
								if(index == sizeof crcbuffer) //读出文件很大，装满buffer，说明还有未读部分
								{
									crc32_Update(&crcValue, (const uint8_t*)crcbuffer, index);
									continue;
								}	
								else if(index < sizeof crcbuffer)//读出文件很小或者到了最后一段，没有装满buffer
								{
									if(index!=0) //如果等于0，说明上一次读出的数据刚好装满buffer，这一次没有读出数据
									{
										crc32_Update(&crcValue, (const uint8_t*)crcbuffer, index);
									}
									crc32_Final(&crcValue);//获得CRC值，比对校验
									sprintf(crcVal,"%X",crcValue);
									if(strstr(crc,crcVal)==NULL) //比对失败
									{
										f_close(&fwfile);f_del(sys_default_update_path[src]); 
										return 0;
									}
									else //比对校验成功，读取文件写入qspi flash
									{
										f_rewind(&fwfile); //设置读取端点为0，重新读取并写入qspi flash
										index = 0;
										memset(crcbuffer,0,sizeof crcbuffer);
										while(1)
										{
											led(src+1,led_state);
											HAL_Delay(100);
											led(src+1,!led_state);
											HAL_Delay(100);
											res = f_read(&fwfile,crcbuffer,sizeof crcbuffer,&index);
											if(res != FR_OK)
											{
												f_close(&fwfile);f_del(sys_default_update_path[src]);
												return 0;
											}
											else if (index!=0)
											{
												W25Q64_QSPI_Write(address,(uint8_t*)crcbuffer,index);
												address+=index;
											}
											else if(index==0)
											{
												break;
											}
										}
										f_close(&fwfile);
										//
										//烧录完成之后，需要映射内存地址，然后直接校验以下CRC
										W25QXX_Memory_Mapped_Enable();
										if(sys_VerifyFw((const uint8_t*)0x90000000,fwfile.obj.objsize,crcValue)==1)
										{
											res = f_del(sys_default_update_path[src]);  //删除已更新文件
											return 1;
										}
										else
										{
											return 0xff;          //校验失败，不删除更新文件，重启再次更新
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
/***********************************************************
重定义外设逆初始化

************************************************************/
void HAL_MspDeInit(void)
{
	HAL_GPIO_DeInit(FLASH_CS_GPIO_Port,FLASH_CS_Pin);
	HAL_GPIO_DeInit(SDMMC_CDN_GPIO_Port,SDMMC_CDN_Pin);
}
/***********************************************************
系统主要线程
argument：传入参数
return：  void
************************************************************/
void StartDefaultTask(void const * argument)
{
	static __IO uint8_t update_status = 0;

	//
	//枚举U盘更新程序
	if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_10) == 1)   
	{
			MX_USB_DEVICE_Init();
			HAL_Delay(100);
			CDC_Printf("boot in H7PI_MultiBootloader......");
			while(1)
			{
				
				led(1,1);led(2,1);
				HAL_Delay(100);
				led(1,0);led(2,0);
				HAL_Delay(100);
				led(1,1);led(2,1);
				HAL_Delay(100);
				led(1,0);led(2,0);
				CDC_Printf("\r\ncopy you update folder into mass storage......\r\nand reset the device......\r\n");
				HAL_Delay(1000);
			}
	}
	
	//挂载文件系统
	sys_FatfsMount();
	//
	//检查是否需要更新固件
	update_status = sys_FwUpdate(SPIFLASH);
	//打开文件或者校验失败，文件不存在或者文件错误，这个时候
	//从SD卡找文件
	if(update_status == 0)
	{
		update_status = sys_FwUpdate(SDCARD);
	}
	if(update_status != 1)
	{ 
		W25QXX_Memory_Mapped_Enable();
	}
	//
	//系统跳转,亮起两个灯，万一系统写入错误，可以表示启动失败
	//挂起所有RTOS任务,避免跳转之后系统初始化产生问题
	HAL_MspDeInit();
	led(1,1);led(2,1);
	SCB_DisableICache();
    SCB_DisableDCache();__disable_irq();
	SCB->VTOR = QSPI_BASE | VECT_TAB_OFFSET;
	__set_CONTROL(0);
	__set_MSP((*(__IO uint32_t*)0x90000000));
	JumpToApplication = (pFunction)(*(__IO uint32_t*)(0x90000000+4));
	JumpToApplication();
}



