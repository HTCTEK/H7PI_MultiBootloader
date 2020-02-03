
#ifndef __USBD_COMPOSITE_H
#define __USBD_COMPOSITE_H

#include  "usbd_ioreq.h"
#include  "usbd_cdc.h"
#include  "usbd_msc.h"

#define MC_MAX_FS_PACKET            0x40

#define USB_MC_CONFIG_DESC_SIZ      106 

#define MC_MSC_EPIN_ADDR                MSC_EPIN_ADDR 
#define MC_MSC_EPOUT_ADDR               MSC_EPOUT_ADDR 

#define MC_CDC_IN_EP                   CDC_IN_EP 
#define MC_CDC_OUT_EP                  CDC_OUT_EP  
#define MC_CDC_CMD_EP                  CDC_CMD_EP 

extern USBD_ClassTypeDef  USBD_COMPOSITE;

#endif  /* __USBD_MC_H */

