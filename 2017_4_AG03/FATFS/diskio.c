#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "stm32f10x.h"
// #include "w25qxx.h"
/* Definitions of physical drive number for each drive */
#define ATA			    0     // 预留SD卡使用
#define SPI_FLASH		1     // 外部SPI Flash


#define FLASH_SECTOR_SIZE 	512	
u16	    FLASH_SECTOR_COUNT=2048*12;	//W25Q1218,前12M字节给FATFS占用
#define FLASH_BLOCK_SIZE   	8     	//每个BLOCK有8个扇区
/*-----------------------------------------------------------------------*/
/* 获取设备状态                                                          */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = RES_OK;
 
	switch (pdrv) {
	case DEV_RAM :
		stat = STA_NOINIT;
		stat &= ~STA_NOINIT;
		// translate the reslut code here
 
		return stat;
	}
	return STA_NOINIT;
}
/*-----------------------------------------------------------------------*/
/* 设备初始化                                                            */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS status = STA_NOINIT;	
	switch (pdrv) {
		case ATA:	         /* SD CARD */
			break;
    
		case SPI_FLASH:    /* SPI Flash */ 
			/* 初始化SPI Flash */
			W25QXX_Init();
			/* 获取SPI Flash芯片状态 */
			status=disk_status(SPI_FLASH);
			break;
      
		default:
			status = STA_NOINIT;
	}
	return status;
}
/*-----------------------------------------------------------------------*/
//读扇区：读取扇区内容到指定存储区
//drv:磁盘编号0~9
//*buff:数据接收缓冲首地址
//sector:扇区地址
//count:需要读取的扇区数
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT status = RES_PARERR;
	if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误	
	switch (pdrv) 
	{
		case ATA:	/* SD CARD */
			break;
    
		case SPI_FLASH:
			for(;count>0;count--)
			{
				W25QXX_Read(buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
				sector++;
				buff+=FLASH_SECTOR_SIZE;
			}
			status = RES_OK;
		break;
    
		default:
			status = RES_PARERR;
	}
	return status;
}
/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{

	DRESULT status = RES_PARERR;
	if (!count) {
		return RES_PARERR;		/* Check parameter */
	}

	switch (pdrv) 
	{
		case ATA:	/* SD CARD */      
			break;

		case SPI_FLASH:
			for(;count>0;count--)
			{										    
				W25QXX_Write((u8*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
				sector++;
				buff+=FLASH_SECTOR_SIZE;
			}
			status = RES_OK;
			break;

		default:
			status = RES_PARERR;
	}
	return status;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT status = RES_PARERR;
	switch (pdrv) 
	{
		case ATA:	/* SD CARD */
		break;

		case SPI_FLASH:
			switch (cmd) 
			{
				case CTRL_SYNC:
					break;	 
				case GET_SECTOR_SIZE:
					*(WORD*)buff = FLASH_SECTOR_SIZE;
					break;	 
				case GET_BLOCK_SIZE:
					*(WORD*)buff = FLASH_BLOCK_SIZE;
					break;	 
				case GET_SECTOR_COUNT:
					*(DWORD*)buff = FLASH_SECTOR_COUNT;
					break;       
			}
			status = RES_OK;
			break;

		default:
		status = RES_PARERR;
	}
	return status;
}

__weak DWORD get_fattime(void) {
	/* 返回当前时间戳 */
	return	  ((DWORD)(2015 - 1980) << 25)	/* Year 2015 */
			| ((DWORD)1 << 21)				/* Month 1 */
			| ((DWORD)1 << 16)				/* Mday 1 */
			| ((DWORD)0 << 11)				/* Hour 0 */
			| ((DWORD)0 << 5)				  /* Min 0 */
			| ((DWORD)0 >> 1);				/* Sec 0 */
}
