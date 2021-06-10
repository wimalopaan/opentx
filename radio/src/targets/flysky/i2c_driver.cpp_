/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "opentx.h"
#include "debug.h"
#include <stdio.h>

/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will 
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define sEE_FLAG_TIMEOUT ((uint32_t)0x1000)
#define sEE_LONG_TIMEOUT ((uint32_t)(10 * sEE_FLAG_TIMEOUT))

/* Maximum number of trials for sEE_WaitEepromStandbyState() function */
#define sEE_MAX_TRIALS_NUMBER 300

#define sEE_OK 0
#define sEE_FAIL 1

uint32_t sEE_ReadBuffer(uint8_t *pBuffer, uint16_t ReadAddr, uint16_t *NumByteToRead);
uint32_t sEE_WritePage(uint8_t *pBuffer, uint16_t WriteAddr, uint16_t *NumByteToWrite);
void sEE_WriteBuffer(uint8_t *pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
uint32_t sEE_WaitEepromStandbyState(void);

/* USER Callbacks: These are functions for which prototypes only are declared in
   EEPROM driver and that should be implemented into user applicaiton. */
/* sEE_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occure during communication (waiting on an event that doesn't occur, bus 
   errors, busy devices ...).
   You can use the default timeout callback implementation by uncommenting the 
   define USE_DEFAULT_TIMEOUT_CALLBACK in stm320518_evel_i2c_ee.h file.
   Typically the user implementation of this callback should reset I2C peripheral
   and re-initialize communication or in worst case reset all the application. */
uint32_t sEE_TIMEOUT_UserCallback(void);

uint16_t sEEAddress = 0;
uint32_t sEETimeout = sEE_LONG_TIMEOUT;
uint16_t sEEDataNum;
volatile uint16_t NumDataRead = 0;

void eepromWaitEepromStandbyState(void)
{
  TRACE("eepromWaitEepromStandbyState...");
  sEE_WaitEepromStandbyState();
  TRACE("done");
}

void eepromInit()
{
  TRACE("eeprom init...");
  i2cInit();
  TRACE("done");
}

void eepromStartRead(uint8_t *buffer, size_t address, size_t size)
{
  NumDataRead = size;

  TRACE("sEE_ReadBuffer addr %d size %d", address, size);
  sEE_ReadBuffer(buffer, address, (uint16_t *)(&NumDataRead));
  TRACE("read: %02X%02X%02X%02X%02X%02X%02X%02X",
        buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7]);
}

void eepromStartWrite(uint8_t *buffer, size_t address, size_t size)
{
  TRACE("sEE_WriteBuffer size %d", size);
  sEE_WriteBuffer(buffer, address, size);
  TRACE("sEE_WriteBuffer end. Wait...");
  /* Wait for EEPROM standby state */
  sEE_WaitEepromStandbyState();
  TRACE("done");
}

void i2cInit()
{
  RCC_AHBPeriphClockCmd(I2C_RCC_AHB1Periph, ENABLE);
  RCC_APB1PeriphClockCmd(I2C_RCC_APB1Periph, ENABLE);

  GPIO_InitTypeDef gpio_init;
  gpio_init.GPIO_Pin = I2C_SCL_GPIO_PIN | I2C_SDA_GPIO_PIN;
  gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
  //gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_Mode = GPIO_Mode_AF;
  gpio_init.GPIO_OType = GPIO_OType_OD;
  gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(I2C_GPIO, &gpio_init);

  GPIO_PinAFConfig(I2C_GPIO, I2C_SCL_GPIO_PinSource, I2C_GPIO_AF);
  GPIO_PinAFConfig(I2C_GPIO, I2C_SDA_GPIO_PinSource, I2C_GPIO_AF);

  I2C_InitTypeDef i2c_init;
  //rise time 100ns fall 10ns 100khz standard mode
  i2c_init.I2C_Timing = 0x00210507; // 0x10805E89; <- original // 0x00210507 <- example // 0x10420F13 <- mine
  i2c_init.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  i2c_init.I2C_DigitalFilter = 0;
  i2c_init.I2C_OwnAddress1 = 0x00;
  i2c_init.I2C_Mode = I2C_Mode_I2C;
  i2c_init.I2C_Ack = I2C_Ack_Enable;
  i2c_init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

  I2C_DeInit(I2C);
  I2C_Cmd(I2C, DISABLE);
  I2C_StretchClockCmd(I2C, ENABLE);
  I2C_Init(I2C, &i2c_init);
  I2C_Cmd(I2C, ENABLE);
}

/*
 * Next code from 
 https://github.com/ZiB/STM32F0-Discovery/blob/master/%D0%91%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA%D0%B8/STM32F0xx_StdPeriph_Lib_V1.1.0/Project/STM32F0xx_StdPeriph_Examples/I2C/I2C_EEPROM/main.c
 https://github.com/ZiB/STM32F0-Discovery/blob/master/%D0%91%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA%D0%B8/STM32F0xx_StdPeriph_Lib_V1.1.0/Utilities/STM32_EVAL/STM320518_EVAL/stm320518_eval_i2c_ee.c
 */

/**
  * @brief  Reads a block of data from the EEPROM.
  * @param  pBuffer: pointer to the buffer that receives the data read from 
  *         the EEPROM.
  * @param  ReadAddr: EEPROM's internal address to start reading from.
  * @param  NumByteToRead: pointer to the variable holding number of bytes to 
  *         be read from the EEPROM.
  *
  * @retval sEE_OK (0) if operation is correctly performed, else return value 
  *         different from sEE_OK (0) or the timeout user callback.
  */
uint32_t sEE_ReadBuffer(uint8_t *pBuffer, uint16_t ReadAddr, uint16_t *NumByteToRead)
{
  uint32_t NumbOfSingle = 0, Count = 0, DataNum = 0, StartCom = 0;

  /* Get number of reload cycles */
  Count = (*NumByteToRead) / 255;
  NumbOfSingle = (*NumByteToRead) % 255;

  /* Configure slave address, nbytes, reload and generate start */
  TRACE("I2C_TransferHandling");
  I2C_TransferHandling(I2C, I2C_ADDRESS_EEPROM, 2, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

  /* Wait until TXIS flag is set */
  sEETimeout = sEE_LONG_TIMEOUT;
  while (I2C_GetFlagStatus(I2C, I2C_ISR_TXIS) == RESET)
  {
    if ((sEETimeout--) == 0)
      return sEE_TIMEOUT_UserCallback();
  }

  /* Send MSB of memory address */
  TRACE("I2C_SendData");
  I2C_SendData(I2C, (uint8_t)((ReadAddr & 0xFF00) >> 8));

  /* Wait until TXIS flag is set */
  sEETimeout = sEE_LONG_TIMEOUT;
  while (I2C_GetFlagStatus(I2C, I2C_ISR_TXIS) == RESET)
  {
    if ((sEETimeout--) == 0)
      return sEE_TIMEOUT_UserCallback();
  }

  /* Send LSB of memory address  */
  TRACE("I2C_SendData");
  I2C_SendData(I2C, (uint8_t)(ReadAddr & 0x00FF));

  /* Wait until TC flag is set */
  sEETimeout = sEE_LONG_TIMEOUT;
  while (I2C_GetFlagStatus(I2C, I2C_ISR_TC) == RESET)
  {
    if ((sEETimeout--) == 0)
      return sEE_TIMEOUT_UserCallback();
  }

  /* If number of Reload cycles is not equal to 0 */
  if (Count != 0)
  {
    /* Starting communication */
    StartCom = 1;

    /* Wait until all reload cycles are performed */
    while (Count != 0)
    {
      /* If a read transfer is performed */
      if (StartCom == 0)
      {
        /* Wait until TCR flag is set */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (I2C_GetFlagStatus(I2C, I2C_ISR_TCR) == RESET)
        {
          if ((sEETimeout--) == 0)
            return sEE_TIMEOUT_UserCallback();
        }
      }

      /* if remains one read cycle */
      if ((Count == 1) && (NumbOfSingle == 0))
      {
        /* if starting communication */
        if (StartCom != 0)
        {
          /* Configure slave address, end mode and start condition */
          I2C_TransferHandling(I2C, sEEAddress, 255, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
        }
        else
        {
          /* Configure slave address, end mode */
          I2C_TransferHandling(I2C, sEEAddress, 255, I2C_AutoEnd_Mode, I2C_No_StartStop);
        }
      }
      else
      {
        /* if starting communication */
        if (StartCom != 0)
        {
          /* Configure slave address, end mode and start condition */
          I2C_TransferHandling(I2C, sEEAddress, 255, I2C_Reload_Mode, I2C_Generate_Start_Read);
        }
        else
        {
          /* Configure slave address, end mode */
          I2C_TransferHandling(I2C, sEEAddress, 255, I2C_Reload_Mode, I2C_No_StartStop);
        }
      }

      /* Update local variable */
      StartCom = 0;
      DataNum = 0;

      /* Wait until all data are received */
      while (DataNum != 255)
      {
        /* Wait until RXNE flag is set */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (I2C_GetFlagStatus(I2C, I2C_ISR_RXNE) == RESET)
        {
          if ((sEETimeout--) == 0)
            return sEE_TIMEOUT_UserCallback();
        }

        /* Read data from RXDR */
        pBuffer[DataNum] = I2C_ReceiveData(I2C);

        /* Update number of received data */
        DataNum++;
        (*NumByteToRead)--;
      }
      /* Update Pointer of received buffer */
      pBuffer += DataNum;

      /* update number of reload cycle */
      Count--;
    }

    /* If number of single data is not equal to 0 */
    if (NumbOfSingle != 0)
    {
      /* Wait until TCR flag is set */
      sEETimeout = sEE_LONG_TIMEOUT;
      while (I2C_GetFlagStatus(I2C, I2C_ISR_TCR) == RESET)
      {
        if ((sEETimeout--) == 0)
          return sEE_TIMEOUT_UserCallback();
      }

      /* Update CR2 : set Nbytes and end mode */
      I2C_TransferHandling(I2C, sEEAddress, (uint8_t)(NumbOfSingle), I2C_AutoEnd_Mode, I2C_No_StartStop);

      /* Reset local variable */
      DataNum = 0;

      /* Wait until all data are received */
      while (DataNum != NumbOfSingle)
      {
        /* Wait until RXNE flag is set */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (I2C_GetFlagStatus(I2C, I2C_ISR_RXNE) == RESET)
        {
          if ((sEETimeout--) == 0)
            return sEE_TIMEOUT_UserCallback();
        }

        /* Read data from RXDR */
        pBuffer[DataNum] = I2C_ReceiveData(I2C);

        /* Update number of received data */
        DataNum++;
        (*NumByteToRead)--;
      }
    }
  }
  else
  {
    /* Update CR2 : set Slave Address , set read request, generate Start and set end mode */
    I2C_TransferHandling(I2C, sEEAddress, (uint32_t)(NumbOfSingle), I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

    /* Reset local variable */
    DataNum = 0;

    /* Wait until all data are received */
    while (DataNum != NumbOfSingle)
    {
      /* Wait until RXNE flag is set */
      sEETimeout = sEE_LONG_TIMEOUT;
      while (I2C_GetFlagStatus(I2C, I2C_ISR_RXNE) == RESET)
      {
        if ((sEETimeout--) == 0)
          return sEE_TIMEOUT_UserCallback();
      }

      /* Read data from RXDR */
      pBuffer[DataNum] = I2C_ReceiveData(I2C);

      /* Update number of received data */
      DataNum++;
      (*NumByteToRead)--;
    }
  }

  /* Wait until STOPF flag is set */
  sEETimeout = sEE_LONG_TIMEOUT;
  while (I2C_GetFlagStatus(I2C, I2C_ISR_STOPF) == RESET)
  {
    if ((sEETimeout--) == 0)
      return sEE_TIMEOUT_UserCallback();
  }

  /* Clear STOPF flag */
  I2C_ClearFlag(I2C, I2C_ICR_STOPCF);

  /* If all operations OK, return sEE_OK (0) */
  return sEE_OK;
}

/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
  *
  * @note   The number of bytes (combined to write start address) must not 
  *         cross the EEPROM page boundary. This function can only write into
  *         the boundaries of an EEPROM page.
  * @note   This function doesn't check on boundaries condition (in this driver 
  *         the function sEE_WriteBuffer() which calls sEE_WritePage() is 
  *         responsible of checking on Page boundaries).
  * 
  * @param  pBuffer: pointer to the buffer containing the data to be written to 
  *         the EEPROM.
  * @param  WriteAddr: EEPROM's internal address to write to.
  * @param  NumByteToWrite: pointer to the variable holding number of bytes to 
  *         be written into the EEPROM.
  *
  * @retval sEE_OK (0) if operation is correctly performed, else return value 
  *         different from sEE_OK (0) or the timeout user callback.
  */
uint32_t sEE_WritePage(uint8_t *pBuffer, uint16_t WriteAddr, uint16_t *NumByteToWrite)
{
  uint32_t DataNum = 0;

  /* Configure slave address, nbytes, reload and generate start */
  I2C_TransferHandling(I2C, sEEAddress, 2, I2C_Reload_Mode, I2C_Generate_Start_Write);

  /* Wait until TXIS flag is set */
  sEETimeout = sEE_LONG_TIMEOUT;
  while (I2C_GetFlagStatus(I2C, I2C_ISR_TXIS) == RESET)
  {
    if ((sEETimeout--) == 0)
      return sEE_TIMEOUT_UserCallback();
  }

  /* Send MSB of memory address */
  I2C_SendData(I2C, (uint8_t)((WriteAddr & 0xFF00) >> 8));

  /* Wait until TXIS flag is set */
  sEETimeout = sEE_LONG_TIMEOUT;
  while (I2C_GetFlagStatus(I2C, I2C_ISR_TXIS) == RESET)
  {
    if ((sEETimeout--) == 0)
      return sEE_TIMEOUT_UserCallback();
  }

  /* Send LSB of memory address  */
  I2C_SendData(I2C, (uint8_t)(WriteAddr & 0x00FF));

  /* Wait until TCR flag is set */
  sEETimeout = sEE_LONG_TIMEOUT;
  while (I2C_GetFlagStatus(I2C, I2C_ISR_TCR) == RESET)
  {
    if ((sEETimeout--) == 0)
      return sEE_TIMEOUT_UserCallback();
  }

  /* Update CR2 : set Slave Address , set write request, generate Start and set end mode */
  I2C_TransferHandling(I2C, sEEAddress, (uint8_t)(*NumByteToWrite), I2C_AutoEnd_Mode, I2C_No_StartStop);

  while (DataNum != (*NumByteToWrite))
  {
    /* Wait until TXIS flag is set */
    sEETimeout = sEE_LONG_TIMEOUT;
    while (I2C_GetFlagStatus(I2C, I2C_ISR_TXIS) == RESET)
    {
      if ((sEETimeout--) == 0)
        return sEE_TIMEOUT_UserCallback();
    }

    /* Write data to TXDR */
    I2C_SendData(I2C, (uint8_t)(pBuffer[DataNum]));

    /* Update number of transmitted data */
    DataNum++;
  }

  /* Wait until STOPF flag is set */
  sEETimeout = sEE_LONG_TIMEOUT;
  while (I2C_GetFlagStatus(I2C, I2C_ISR_STOPF) == RESET)
  {
    if ((sEETimeout--) == 0)
      return sEE_TIMEOUT_UserCallback();
  }

  /* Clear STOPF flag */
  I2C_ClearFlag(I2C, I2C_ICR_STOPCF);

  /* If all operations OK, return sEE_OK (0) */
  return sEE_OK;
}

/**
  * @brief  Writes buffer of data to the I2C EEPROM.
  * @param  pBuffer: pointer to the buffer  containing the data to be written 
  *         to the EEPROM.
  * @param  WriteAddr: EEPROM's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the EEPROM.
  * @retval None
  */
void sEE_WriteBuffer(uint8_t *pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
  uint16_t NumOfPage = 0, NumOfSingle = 0, count = 0;
  uint16_t Addr = 0;

  Addr = WriteAddr % EEPROM_PAGE_SIZE;
  count = EEPROM_PAGE_SIZE - Addr;
  NumOfPage = NumByteToWrite / EEPROM_PAGE_SIZE;
  NumOfSingle = NumByteToWrite % EEPROM_PAGE_SIZE;

  /*!< If WriteAddr is sEE_PAGESIZE aligned  */
  if (Addr == 0)
  {
    /*!< If NumByteToWrite < sEE_PAGESIZE */
    if (NumOfPage == 0)
    {
      /* Store the number of data to be written */
      sEEDataNum = NumOfSingle;
      /* Start writing data */
      sEE_WritePage(pBuffer, WriteAddr, (uint16_t *)(&sEEDataNum));
      sEE_WaitEepromStandbyState();
    }
    /*!< If NumByteToWrite > sEE_PAGESIZE */
    else
    {
      while (NumOfPage--)
      {
        /* Store the number of data to be written */
        sEEDataNum = EEPROM_PAGE_SIZE;
        sEE_WritePage(pBuffer, WriteAddr, (uint16_t *)(&sEEDataNum));
        sEE_WaitEepromStandbyState();
        WriteAddr += EEPROM_PAGE_SIZE;
        pBuffer += EEPROM_PAGE_SIZE;
      }

      if (NumOfSingle != 0)
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;
        sEE_WritePage(pBuffer, WriteAddr, (uint16_t *)(&sEEDataNum));
        sEE_WaitEepromStandbyState();
      }
    }
  }
  /*!< If WriteAddr is not sEE_PAGESIZE aligned  */
  else
  {
    /*!< If NumByteToWrite < sEE_PAGESIZE */
    if (NumOfPage == 0)
    {
      /*!< If the number of data to be written is more than the remaining space 
      in the current page: */
      if (NumByteToWrite > count)
      {
        /* Store the number of data to be written */
        sEEDataNum = count;
        /*!< Write the data conained in same page */
        sEE_WritePage(pBuffer, WriteAddr, (uint16_t *)(&sEEDataNum));
        sEE_WaitEepromStandbyState();

        /* Store the number of data to be written */
        sEEDataNum = (NumByteToWrite - count);
        /*!< Write the remaining data in the following page */
        sEE_WritePage((uint8_t *)(pBuffer + count), (WriteAddr + count), (uint16_t *)(&sEEDataNum));
        sEE_WaitEepromStandbyState();
      }
      else
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;
        sEE_WritePage(pBuffer, WriteAddr, (uint16_t *)(&sEEDataNum));
        sEE_WaitEepromStandbyState();
      }
    }
    /*!< If NumByteToWrite > sEE_PAGESIZE */
    else
    {
      NumByteToWrite -= count;
      NumOfPage = NumByteToWrite / EEPROM_PAGE_SIZE;
      NumOfSingle = NumByteToWrite % EEPROM_PAGE_SIZE;

      /* Store the number of data to be written */
      sEEDataNum = count;
      sEE_WritePage(pBuffer, WriteAddr, (uint16_t *)(&sEEDataNum));
      sEE_WaitEepromStandbyState();
      WriteAddr += count;
      pBuffer += count;

      while (NumOfPage--)
      {
        /* Store the number of data to be written */
        sEEDataNum = EEPROM_PAGE_SIZE;
        sEE_WritePage(pBuffer, WriteAddr, (uint16_t *)(&sEEDataNum));
        sEETimeout = sEE_LONG_TIMEOUT;
        sEE_WaitEepromStandbyState();
        WriteAddr += EEPROM_PAGE_SIZE;
        pBuffer += EEPROM_PAGE_SIZE;
      }
      if (NumOfSingle != 0)
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;
        sEE_WritePage(pBuffer, WriteAddr, (uint16_t *)(&sEEDataNum));
        sEE_WaitEepromStandbyState();
      }
    }
  }
}

/**
  * @brief  Wait for EEPROM Standby state.
  * 
  * @note  This function allows to wait and check that EEPROM has finished the 
  *        last operation. It is mostly used after Write operation: after receiving
  *        the buffer to be written, the EEPROM may need additional time to actually
  *        perform the write operation. During this time, it doesn't answer to
  *        I2C packets addressed to it. Once the write operation is complete
  *        the EEPROM responds to its address.
  * 
  * @param  None
  *
  * @retval sEE_OK (0) if operation is correctly performed, else return value 
  *         different from sEE_OK (0) or the timeout user callback.
  */
uint32_t sEE_WaitEepromStandbyState(void)
{
  __IO uint32_t sEETrials = 0;

  /* Keep looping till the slave acknowledge his address or maximum number 
  of trials is reached (this number is defined by sEE_MAX_TRIALS_NUMBER define
  in stm32373c_eval_i2c_ee.h file) */

  /* Configure CR2 register : set Slave Address and end mode */
  I2C_TransferHandling(I2C, sEEAddress, 0, I2C_AutoEnd_Mode, I2C_No_StartStop);

  do
  {
    /* Initialize sEETimeout */
    sEETimeout = sEE_FLAG_TIMEOUT;

    /* Clear NACKF */
    I2C_ClearFlag(I2C, I2C_ICR_NACKCF | I2C_ICR_STOPCF);

    /* Generate start */
    I2C_GenerateSTART(I2C, ENABLE);

    /* Wait until timeout elapsed */
    while (sEETimeout-- != 0)
      ;

    /* Check if the maximum allowed numbe of trials has bee reached */
    if (sEETrials++ == sEE_MAX_TRIALS_NUMBER)
    {
      /* If the maximum number of trials has been reached, exit the function */
      return sEE_TIMEOUT_UserCallback();
    }
  } while (I2C_GetFlagStatus(I2C, I2C_ISR_NACKF) != RESET);

  /* Clear STOPF */
  I2C_ClearFlag(I2C, I2C_ICR_STOPCF);

  /* Return sEE_OK if device is ready */
  return sEE_OK;
}

/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval 0.
  */
uint32_t sEE_TIMEOUT_UserCallback(void)
{
  /* The following code allows I2C error recovery and return to normal communication
     if the error source doesnt still exist (ie. hardware issue..) */

  /* Reinitialize all resources */
  TRACE("Timeout!");
  i2cInit();

  /* At this stage the I2C error should be recovered and device can communicate
     again (except if the error source still exist).
     User can implement mechanism (ex. test on max trial number) to manage situation
     when the I2C can't recover from current error. */

  return 0;
}

// /**
//    * @brief  Reads a block of data from the EEPROM.
//    * @param  rxbuf : pointer to the buffer that receives the data read
//    *   from the EEPROM.
//    * @param  address : EEPROM's internal address to read from.
//    * @param  NumByteToRead : number of bytes to read from the EEPROM.
//    * @retval None
//    */
// bool I2C_EE_ReadBlock(uint8_t *rxbuf, uint16_t address, uint16_t NumByteToRead)
// {

// }

// void eepromReadBlock(uint8_t *buffer, size_t address, size_t size)
// {
//   while (!I2C_EE_ReadBlock(buffer, address, size))
//   {
//     i2cInit();
//   }
// }

uint8_t eepromReadStatus() { return 0; }
void eepromBlockErase(uint32_t address)
{
  TRACE("eepromBlockErase called");
}
// void eepromStartRead(uint8_t *buffer, size_t address, size_t size) {}
// void eepromStartWrite(uint8_t *buffer, size_t address, size_t size) {}

// /**
//    * @brief  Writes buffer of data to the I2C EEPROM.
//    * @param  buffer : pointer to the buffer containing the data to be
//    *   written to the EEPROM.
//    * @param  address : EEPROM's internal address to write to.
//    * @param  size : number of bytes to write to the EEPROM.
//    * @retval None
//    */
// void eepromWriteBlock(uint8_t *buffer, size_t address, size_t size)
// {
//   uint8_t offset = address % EEPROM_PAGE_SIZE;
//   uint8_t count = EEPROM_PAGE_SIZE - offset;
//   if (size < count)
//   {
//     count = size;
//   }
//   while (count > 0)
//   {
//     eepromPageWrite(buffer, address, count);
//     eepromWaitEepromStandbyState();
//     address += count;
//     buffer += count;
//     size -= count;
//     count = EEPROM_PAGE_SIZE;
//     if (size < EEPROM_PAGE_SIZE)
//     {
//       count = size;
//     }
//   }
// }

uint8_t eepromIsTransferComplete()
{
  TRACE("eepromIsTransferComplete called");
  return 1;
}

// /**
//    * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
//    * @note   The number of byte can't exceed the EEPROM page size.
//    * @param  pBuffer : pointer to the buffer containing the data to be
//    *   written to the EEPROM.
//    * @param  WriteAddr : EEPROM's internal address to write to.
//    * @param  NumByteToWrite : number of bytes to write to the EEPROM.
//    * @retval None
//    */
// bool I2C_EE_PageWrite(uint8_t *pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite)
// {

//   return true;
// }

// void eepromPageWrite(uint8_t *pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite)
// {
//   while (!I2C_EE_PageWrite(pBuffer, WriteAddr, NumByteToWrite))
//   {
//     i2cInit();
//   }
// }

// /**
//    * @brief  Wait for EEPROM Standby state
//    * @param  None
//    * @retval None
//    */
// bool I2C_EE_WaitEepromStandbyState(void)
// {
//   do
//   {
//     I2C_GenerateSTART(I2C, ENABLE);
//     if (!I2C_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT))
//       return false;

//     I2C_Send7bitAddress(I2C, I2C_ADDRESS_EEPROM, I2C_Direction_Transmitter);
//   } while (!I2C_WaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

//   I2C_GenerateSTOP(I2C, ENABLE);
//   return true;
// }

// void eepromWaitEepromStandbyState(void)
// {
//   while (!I2C_EE_WaitEepromStandbyState())
//   {
//     i2cInit();
//   }
// }
