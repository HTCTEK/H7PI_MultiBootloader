# H7PI_MultiBootloader

## 介绍
stm32H7 bootloader,支持U盘更新，支持SD卡更新，支持启动到QSPI, [点击查看H7PI的硬件设计参考](https://gihub.com/PinoDM/H7PI)

## Bootloader
1. 此Bootloader需要硬件在qspi flash上运行程序，请确认自己的硬件是否支持
2. 默认支持H7PI开发板，主控为STM32H750VBT6
3. 使用前需要根据自己的硬件设计变更引脚并重新编译
4. 此Bootloader要求硬件至少存在spi flash或者SD Card

## FLM
1. flm文件时keil用于代码下载接口，目前仅开发了keil下的下载，其他IDE加入roadmap待开发
2. 当前flm文件用qspi代码下载调试，与bootloader合用，可以直接下载调试，与内部flash的使用一致

## JlinkScript
1. 用于使用Jlink下载代码到qspi flash的接口，可以支持到各种IDE
2. 待开发

## openOCD Script
1. 用于使用openOCD调试器下载代码到qspi flash的接口，可以支持到各种IDE
2. 待开发

## 如何进入U盘更新模式
1. 短接3V3和IO1(PD10)
2. 通过USB接口接入电脑，此时电脑枚举出8MB的U盘
3. 新建一个update文件夹，将固件放入update文件夹
4. 在update文件夹新建一个json文件，命名为fw.json
5. 在fw.json文件中建立三个字段如下：

```
{
    "path":"1:/update/app_test.bin",
    "version":"0.1",
    "crc32":"653E571F"
}
```
6. path字段为固件路径，spiflash请使用磁盘号1，SD Card请使用磁盘号0
7. crc32 请使用hash工具计算bin文件的crc32值，并填入字段
8. 目前没有对版本号进行比较，version无关紧要

#### 1. 移植关键
* 引脚修改  
* 芯片信息修改
* MCU初始化修改（主要是初始化，STM32系列通用，略加修改即可，这里不提）

#### 2. 引脚修改
###### 2.1 查找QSPI_Init函数，并根据自己的设计，修改下方这段代码的初始化，主要是IO口和复用功能
```
u8 QSPI_Init(void)
{
	u32 tempreg=0;

	RCC->AHB4ENR|=1<<0; //??GPIOA??
	RCC->AHB4ENR|=1<<1; //??GPIOB??
	RCC->AHB4ENR|=1<<2;	//??GPIOC??
	RCC->AHB4ENR|=1<<4;	//??GPIOE??
	RCC->AHB3ENR|=1<<14; //QSPI????

	//PB2 QUADSPI1_CLK
	GPIO_Set(GPIOB,1<<2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU);
	//PB6 QUADSPI1_BK1_NCS
	GPIO_Set(GPIOB,1<<6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU);
	//PC9,PC10 QUADSPI1_BK1_IO0, IO1, 
	GPIO_Set(GPIOC,1<<9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU);
	GPIO_Set(GPIOC,1<<10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU);
	//IO3
	GPIO_Set(GPIOA,1<<1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU);
	//PE2 QUADSPI1_BK1_IO2
	GPIO_Set(GPIOE, 1<<2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU);

	GPIO_AF_Set(GPIOB,2,9); //PB2,AF9
 	GPIO_AF_Set(GPIOB,6,10); //PB6,AF10
 	GPIO_AF_Set(GPIOC,9,9); //PC9,AF9
 	GPIO_AF_Set(GPIOC,10,9); //PC10,AF9
 	GPIO_AF_Set(GPIOA,1,9); //Pa1,AF9
 	GPIO_AF_Set(GPIOE,2,9); //PE2,AF9
```
###### 2.2 同样在QSPI_Init函数中修改闪存模式

```
tempreg|=0<<7;			//选择FLASH1，flash1将0改为1
tempreg|=0<<6;			//禁止双闪存模式，开启双闪存的话将0改为1
```


#### 3. 修改芯片信息
###### 3.1 找到这段代码，根据自己的芯片修改相关信息

```
#ifdef W25Q64JV
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "H7PI_W25Q64",  	   // Device Name
   EXTSPI,                     // Device Type
   0x90000000,                 // Device Start Address
   0x00800000,                 // Device Size (8MB)
   4096,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 300 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x001000, 0x000000,         // Sector Size 4kB (2048 Sectors)
   SECTOR_END
};
#endif
```
###### 3.2 在工程配置里修改芯片的声明，需要跟上面的芯片信息代码中，#ifdef W25Q64JV 这里保持一致

![修改芯片声明](https://images.gitee.com/uploads/images/2019/1121/092027_5f50ee1b_1586921.png "1.png")

###### 3.3 编译

![编译](https://images.gitee.com/uploads/images/2019/1121/092326_78279460_1586921.png "2.png")

###### 3.4 复制flm文件到keil安装目录```\Keil_v5\ARM\Flash```（这个根据自己keil的安装目录定位）

![flm文件位置](https://images.gitee.com/uploads/images/2019/1121/092511_06fc26da_1586921.png "4.png")  

###### 3.5 最后一步在自己的app工程里进行，删除原来的Programing Algorithm，然后通过add按钮选择刚才自己生成的，名字和刚才这里修改的device name一致，同时修改ram for algorithm，如果出现校验失败，请不要勾选Verify
```
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "H7PI_W25Q64",  	   // Device Name
```

![选择flm文件](https://images.gitee.com/uploads/images/2019/1121/093026_a09628ea_1586921.png "5.png")


