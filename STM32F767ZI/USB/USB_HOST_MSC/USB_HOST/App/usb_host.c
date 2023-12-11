/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file            : usb_host.c
  * @version         : v1.0_Cube
  * @brief           : This file implements the USB Host
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_msc.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USB Host core handle declaration */
USBH_HandleTypeDef hUsbHostFS;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*
 * user callback declaration
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */
#include <stdio.h>
#include "fatfs.h"

extern UART_HandleTypeDef huart3;

/**
  * @brief Print the characters to UART (printf).
  * @retval int
  */
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the UART3 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

static FRESULT ETX_MSC_ProcessUsbDevice(void)
{
  FATFS     UsbDiskFatFs;                                 /* File system object for USB disk logical drive */
  char      UsbDiskPath[4] = {0};                         /* USB Host logical drive path */
  FIL       file;                                         /* File object */
  FRESULT   res;                                          /* FatFs function common result code */
  uint32_t  total_space, free_space;                      /* Total Space and Free Space */
  DWORD     fre_clust;                                    /* Freee Cluster */
  uint32_t  byteswritten, bytesread;                      /* File write/read counts */
  uint8_t   wr_data[] = "Welcome to EmbeTronicX!!!";      /* Data buffer */
  uint8_t   rd_data[100];                                 /* Read buffer */
  char      file_name[] = "temp.txt";                     /* File name */

  do
  {
    /* Register the file system object to the FatFs module */
    res = f_mount( &UsbDiskFatFs, (TCHAR const*)UsbDiskPath, 0 );
    if( res != FR_OK )
    {
      /* FatFs Init Error */
      break;
    }

    /* Check the Free Space */
    FATFS *fatFs = &UsbDiskFatFs;
    f_getfree("", &fre_clust, &fatFs);
    total_space = (uint32_t)((UsbDiskFatFs.n_fatent - 2) * UsbDiskFatFs.csize * 0.5);
    free_space = (uint32_t)(fre_clust * UsbDiskFatFs.csize * 0.5);
    printf("USB Device Total Space = %lu MB\n", total_space/1024);
    printf("USB Device Free Space  = %lu MB\n", free_space/1024);

    /* Create a new text file with write access */
    res = f_open( &file, file_name, ( FA_CREATE_ALWAYS | FA_WRITE ) );
    if( res != FR_OK )
    {
      /* File Open Error */
      break;
    }

    /* Write the data to the text file */
    res = f_write( &file, wr_data, sizeof(wr_data), (void *)&byteswritten );

    /* Close the opened file */
    f_close( &file );

    if( res != FR_OK )
    {
      /* File write Error */
      break;
    }

    printf("Data written to the USD Device\n");

    /* Open the text file object with read access */
    res = f_open( &file, file_name, FA_READ );
    if( res != FR_OK )
    {
      /* File Open Error */
      break;
    }

    /* Read data from the file */
    res = f_read( &file, rd_data, sizeof(wr_data), (void *)&bytesread);

    /* Close the file */
    f_close(&file);

    if(res != FR_OK)
    {
      /* File Read Error */
      break;
    }

    /* Print the data */
    printf("Read Data : %s\n", rd_data);

  } while ( 0 );

  /* Unmount the device */
  f_mount(NULL, UsbDiskPath, 0);

  /* Unlink the USB disk driver */
  FATFS_UnLinkDriver(UsbDiskPath);

  return res;
}
/* USER CODE END 1 */

/**
  * Init USB host library, add supported class and start the library
  * @retval None
  */
void MX_USB_HOST_Init(void)
{
  /* USER CODE BEGIN USB_HOST_Init_PreTreatment */

  /* USER CODE END USB_HOST_Init_PreTreatment */

  /* Init host Library, add supported class and start the library. */
  if (USBH_Init(&hUsbHostFS, USBH_UserProcess, HOST_FS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_RegisterClass(&hUsbHostFS, USBH_MSC_CLASS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_Start(&hUsbHostFS) != USBH_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_HOST_Init_PostTreatment */

  /* USER CODE END USB_HOST_Init_PostTreatment */
}

/*
 * Background task
 */
void MX_USB_HOST_Process(void)
{
  /* USB Host Background task */
  USBH_Process(&hUsbHostFS);
}
/*
 * user callback definition
 */
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id)
{
  /* USER CODE BEGIN CALL_BACK_1 */
  switch(id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
  break;

  case HOST_USER_DISCONNECTION:
  Appli_state = APPLICATION_DISCONNECT;
  break;

  case HOST_USER_CLASS_ACTIVE:
  Appli_state = APPLICATION_READY;
  if( ETX_MSC_ProcessUsbDevice() != FR_OK )
  {
    printf("USB Device Process Error\n");
    /* Error : Hang Here */
    while(1);
  }
  break;

  case HOST_USER_CONNECTION:
  Appli_state = APPLICATION_START;
  break;

  default:
  break;
  }
  /* USER CODE END CALL_BACK_1 */
}

/**
  * @}
  */

/**
  * @}
  */

