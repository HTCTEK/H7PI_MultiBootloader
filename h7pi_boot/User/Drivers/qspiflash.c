#include "quadspi.h"
#include "qspiflash.h"

extern QSPI_HandleTypeDef hqspi;
QSPI_CommandTypeDef s_command;
//===============================================================//
//W25Q64 QSPI接口初始化
//===============================================================//
void W25Q64_QSPI_Init(void)
{
	W25Q64_QSPI_ReadID();
	W25Q64_QSPI_ResetEnable();
	W25Q64_QSPI_Reset();
	HAL_Delay(10);
}

//===============================================================//
//W25Q64 使能复位
//===============================================================//
void W25Q64_QSPI_ResetEnable(void)
{
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = W25X_ResetEnable;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.AlternateBytes 		= 0x00;
	s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}	  
}
//===============================================================//
//W25Q64 复位
//===============================================================//
void W25Q64_QSPI_Reset(void)
{
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = W25X_Reset;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.AlternateBytes 		= 0x00;
	s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}	  
}

//===============================================================//
//W25Q64 QSPI写使能
//===============================================================//
void W25Q64_QSPI_WriteEnable(void)
{
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = W25X_WriteEnable;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.AlternateBytes 		= 0x00;
	s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}	      
}

//===============================================================//
//W25Q64 QSPI写禁止
//===============================================================//
void W25Q64_QSPI_WriteDisable(void)
{
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = W25X_WriteDisable;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.AlternateBytes 		= 0x00;
	s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}	       
}

unsigned char W25Q64_QSPI_ReadSR(unsigned char SR)
{
	unsigned char byte=0;
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = SR;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.Address           = 0x0;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.AlternateBytes 		= 0x00;
	s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 1;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	if (HAL_QSPI_Command(&hqspi, &s_command,0xffff) != HAL_OK)
	{
		
	}
	if (HAL_QSPI_Receive(&hqspi,&byte,0xffff) != HAL_OK)
	{
		
	}
    return byte;
}

void W25Q64_QSPI_WaitBusy(void)
{
	int i = 4000000;
	while((W25Q64_QSPI_ReadSR(W25X_ReadStatusReg1)&0x01)==0x01)
	{
		while(i>0)
		{
			i--;
		}
		i = 4000000;
	}
}

void W25Q64_QSPI_WriteSR(unsigned char *SR,unsigned char NumSR)
{
	W25Q64_QSPI_WriteEnable();                  
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = W25X_WriteStatusReg;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.Address           = 0;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.AlternateBytes 		= 0x00;
	s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = NumSR;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	if (HAL_QSPI_Command(&hqspi, &s_command,0xffff) != HAL_OK)
	{

	}
	if (HAL_QSPI_Transmit(&hqspi,SR,0xffff) != HAL_OK)
	{
		
	}
	W25Q64_QSPI_WaitBusy();			  
}

void W25Q64_QSPI_QuadWriteEnable(void)
{
	unsigned char temp[2];
	temp[0]=W25Q64_QSPI_ReadSR(W25X_ReadStatusReg1);
	temp[1]=W25Q64_QSPI_ReadSR(W25X_ReadStatusReg2);
	if((temp[1]&0x02) != 0x02)
	{		
		temp[1] = 0x02;
		W25Q64_QSPI_WriteEnable();
		W25Q64_QSPI_WriteSR(temp,2);
	}
}

void W25Q64_QSPI_QuadWriteDisable(void)
{
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = W25X_WriteDisable;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.Address           = 0;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.AlternateBytes 		= 0x00;
	s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}	 
}

//===============================================================//
//W25Q64 QSPI 扇区擦除(Sector = 4KB)接口
//===============================================================//
void W25Q64_QSPI_EraseSector(unsigned long SectorAddress)
{
	SectorAddress*=4096;
	W25Q64_QSPI_WriteEnable();                  
	W25Q64_QSPI_WaitBusy();   
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = W25X_SectorErase;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.Address           = SectorAddress;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.AlternateBytes 		= 0x00;
	s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	if (HAL_QSPI_Command(&hqspi, &s_command,0xffff) != HAL_OK)
	{

	}
	W25Q64_QSPI_WaitBusy();   				   	//等待擦除完成
}

//===============================================================//
//W25Q64 QSPI 块擦除(Block32KB = 32KB)接口
//===============================================================//
void W25Q64_QSPI_EraseBlock32KB(unsigned long Block32KBAddress)
{

}

//===============================================================//
//W25Q64 QSPI 块擦除(Block64KB = 64KB)接口
//===============================================================//
void W25Q64_QSPI_EraseBlock64KB(unsigned long Block64KBAddress)
{
	
}


void W25Q64_QSPI_EraseChip(void)
{
	W25Q64_QSPI_WriteEnable();                  //SET WEL 
	W25Q64_QSPI_WaitBusy();   
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = W25X_ChipErase;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.Address           = 0;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.AlternateBytes 		= 0x00;
	s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	if (HAL_QSPI_Command(&hqspi, &s_command,0xffff) != HAL_OK)
	{

	} 
	W25Q64_QSPI_WaitBusy();   				   	//等待芯片擦除结束
}

//===============================================================//
//W25Q64 QSPI 读操作模式接口
//===============================================================//
void W25Q64_QSPI_ReadMode(unsigned char ReadModeCmd)
{
	
}

//===============================================================//
//W25Q64 QSPI 读数据接口
//===============================================================//
void W25Q64_QSPI_Read(unsigned long Address,unsigned char *buff,unsigned long NumBytes)
{
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = W25X_FastReadQuad;
	s_command.AddressMode       = QSPI_ADDRESS_4_LINES;

	s_command.Address           = Address;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES;
	s_command.AlternateBytes 		= 0x00;
	s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
	s_command.DataMode          = QSPI_DATA_4_LINES;
	s_command.DummyCycles       = 4;
	s_command.NbData            = NumBytes;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &s_command,0xffff) != HAL_OK)
	{

	}
	if (HAL_QSPI_Receive(&hqspi,buff,0xffff) != HAL_OK)
	{

	}
	W25Q64_QSPI_WaitBusy();
}

//===============================================================//
//W25Q64 QSPI 读页数据(Page = 256Bytes)接口
//===============================================================//
void W25Q64_QSPI_WritePage(unsigned long Address,unsigned char *buff,unsigned long NumBytes)
{
	W25Q64_QSPI_WriteEnable();            
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = W25X_PageProgramQuad;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.Address           = Address;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.AlternateBytes 		= 0x00;
	s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
	s_command.DataMode          = QSPI_DATA_4_LINES;
	s_command.DummyCycles       = 0;
	s_command.NbData            = NumBytes;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	if (HAL_QSPI_Command(&hqspi, &s_command,0xffff) != HAL_OK)
	{

	}
	if (HAL_QSPI_Transmit(&hqspi,buff,0xffff) != HAL_OK)
	{
		
	}
	W25Q64_QSPI_WaitBusy();						//等待写入结束
}
//===============================================================//
//W25Q64 QSPI 写数据
//===============================================================//

void W25Q64_QSPI_WriteNoCheck(unsigned long Address,unsigned char *buff,unsigned long NumBytes)
{
	unsigned short pageremain;	   
	pageremain=256-Address%256; 				//单页剩余的字节数		 	    
	if(NumBytes<=pageremain)pageremain=NumBytes;//不大于256个字节
	while(1)
	{	   
		W25Q64_QSPI_WritePage(Address,buff,pageremain);
		if(NumBytes==pageremain)break;			//写入结束了
	 	else 									//NumByteToWrite>pageremain
		{
			buff+=pageremain;
			Address+=pageremain;	
			NumBytes-=pageremain;			  	//减去已经写入了的字节数
			if(NumBytes>256)pageremain=256; 	//一次可以写入256个字节
			else pageremain=NumBytes; 	  		//不够256个字节了
		}
	};	
}

unsigned char W25QxxCache[4096] = {0};
//===============================================================//
//W25Q64 QSPI 写数据接口
//===============================================================//
void W25Q64_QSPI_Write(unsigned long Address,unsigned char *buff,unsigned long NumBytes)
{
	unsigned long secpos;
	unsigned short secoff;
	unsigned short secremain;	   
 	volatile unsigned short i;    
	unsigned char * W25Q64_BUF;	
    
	W25Q64_BUF = W25QxxCache;
 	secpos = Address / 4096; //扇区地址
	secoff = Address % 4096; //在扇区内的偏移
	secremain = 4096 - secoff; //扇区剩余空间大小

 	if (NumBytes <= secremain) secremain = NumBytes; //不大于4096个字节
	while(1)
	{
		W25Q64_QSPI_Read(secpos * 4096, W25Q64_BUF, 4096); //读出整个扇区的内容
		for (i = 0;i < secremain; i++) //校验数据
		{
			if (W25Q64_BUF[secoff+i] != 0XFF) break; //需要擦除
		}
		if (i < secremain) //需要擦除
		{
			W25Q64_QSPI_EraseSector(secpos); //擦除这个扇区
			for (i = 0; i < secremain; i++) //复制
			{
				W25Q64_BUF[i + secoff] = buff[i];
			}
			W25Q64_QSPI_WriteNoCheck(secpos * 4096, W25Q64_BUF, 4096); //写入整个扇区
		}
		else
		{
			W25Q64_QSPI_WriteNoCheck(Address, buff, secremain); //写已经擦除了的,直接写入扇区剩余区间.
		}
		if (NumBytes == secremain)
		{
			break; //写入结束了
		}
		else//写入未结束
		{
			secpos++; //扇区地址增1
			secoff = 0; //偏移位置为0

			buff += secremain;  //指针偏移
			Address += secremain;//写地址偏移
			NumBytes -= secremain; //字节数递减
			if (NumBytes > 4096)
				secremain = 4096; //下一个扇区还是写不完
			else
				secremain = NumBytes; //下一个扇区可以写完了
		}
	}
}

//===============================================================//
//W25Q64 QSPI 写页数据(Page = 256Bytes)接口
//===============================================================//
//void W25Q64_QSPI_WritePage(unsigned long Address,unsigned char *buff)
//{
//	
//}

//===============================================================//
//W25Q64 QSPI 写块数据(Block = 256Bytes)接口
//===============================================================//
void W25Q64_QSPI_WriteBlock4KB(unsigned long Block4KBAdress,unsigned char *buff)
{
	
}

//===============================================================//
//W25Q64 QSPI 读块数据(Page = 256Bytes)接口
//===============================================================//
void W25Q64_QSPI_ReadBlock4KB(unsigned long Block4KBAdress,unsigned char *buff)
{
	
}

unsigned long W25Q64_QSPI_ReadID(void)
{
	uint8_t Temp[2];	
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = W25X_ManufactDeviceID;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.Address           = 0;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.AlternateBytes 		= 0x00;
	s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 2;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &s_command,0xffff) != HAL_OK)
	{

	}
	if (HAL_QSPI_Receive(&hqspi,Temp,0xffff) != HAL_OK)
	{
		
	}		 
	return (Temp[0]<<8) + Temp[1];
}

void W25QXX_Memory_Mapped_Enable(void)   
{
	QSPI_MemoryMappedTypeDef sMemMappedCfg;

	sMemMappedCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;

	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = W25X_FastReadQuad;
	s_command.AddressMode       = QSPI_ADDRESS_4_LINES;
	s_command.Address           = 0;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES;
	s_command.AlternateBytes 		= 0x00;
	s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
	s_command.DataMode          = QSPI_DATA_4_LINES;
	s_command.DummyCycles       = 4;
	s_command.NbData            = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
//	s_command.SIOOMode          = QSPI_SIOO_INST_ONLY_FIRST_CMD;		//只发送一次指令

	if (HAL_QSPI_MemoryMapped(&hqspi, &s_command, &sMemMappedCfg) != HAL_OK)
	{
		Error_Handler();
	} 
} 


