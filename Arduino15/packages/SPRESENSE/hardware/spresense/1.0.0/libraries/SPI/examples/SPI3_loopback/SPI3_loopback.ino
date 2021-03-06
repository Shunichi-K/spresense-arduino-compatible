/*
 *  SPI3_loopback.ino - SPI3 loopback example application
 *  Copyright 2019 Sony Semiconductor Solutions Corporation
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/**
 * @file SPI3_loopback.ino
 * @author Sony Semiconductor Solutions Corporation
 * @brief This example is used to demonstrate the SPI3 port on main board.
 * @details This example uses the SPI port (SPI3) and it is used as 
 *          an SPI master. Transfer of 8 bit, 16 bit and an array of 8 bit 
 *          data is demonstrated.\n\n
 * 
 *          The SPI port (SPI3) pinout for Spresense is:\n
 *
 *          - SPI3 SCK  D29 (Serial Clock) - The clock pulses which synchronize data 
 *            transmission generated by the master.
 *          - SPI3 MISO D30 (Master In Slave Out) - The Slave line for sending data 
 *            to the master.
 *          - SPI3 MOSI D31 (Master Out Slave In) - The Master line for sending data 
 *            to the peripherals.
 *          - SPI3 CS   D32 CS (Chip Select) - the pin on each device that the master 
 *            can use to enable and disable specific devices.
 *
 *          For this example you need to link between D31(SPI-MOSI) and D30(SPI-MISO).
 *          This loops back data from out to in to show data transfer.\n\n
 *
 *          The SPI3 CS pin is automatically controlled by Spresense.\n
 *          SPI CS goes low at the start of each transaction and high at the end of 
 *          each transaction.\n\n
 *
 *          16 bit data is sent Little Endian. The first byte is the low byte, 
 *          the second byte if the high byte.\n\n
 *
 *          This example code is in the public domain.
 */

/* include the SPI library */
#include <SPI.h>

/**
 * @brief 8 bit transfer example.
 */
void transfer8bit() {
  /* Define the 8 bit test data to be sent. Binary 1110 1111, decimal 239 */
  const uint8_t tx_8 = 0xEF;

  /* Variable for receiving 8 bit data */
  uint8_t rx_8 = 0;
  
  /* Send the tx data and received the rx data */
  rx_8 = SPI3.transfer(tx_8);
  
  /* Show the data received as a serial output */
  Serial.print("8 bit data received ");
  Serial.println(rx_8, DEC);
  
  /* Check to see if the transmitted data was received corectly */
  if (rx_8 == tx_8)
  {
    /* Success */
    Serial.println("8 bit loopback test passed");
  }
  else
  {
    /* Failure */
    Serial.println("8 bit loopback test failed.");
  }
}

/**
 * @brief 16 bit transfer example.
 */
void transfer16bit() {
  /* Define the 16 bit test data to be sent. 1010 1011 1100 1101 (43981) */
  const uint16_t tx_16 = 0xABCD;

  /* Variable for receiving 16 bit data */
  uint16_t rx_16 = 0; 

  /* Send the tx data and received the rx data */
  rx_16 = SPI3.transfer16(tx_16);
  
  /* Show the data received as a serial output */
  Serial.println();
  Serial.print("16 bit data received ");
  Serial.println(rx_16, DEC);

  /* Check to see if the transmitted data was received corectly */
  if (rx_16 == tx_16)
  {
    /* Success */
    Serial.println("16 bit loopback test passed");
  }
  else
  {
    /* Failure */
    Serial.println("16 bit loopback test failed.");
  }
}

/**
 * @brief array transfer example.
 */
void transferArray() {
  /* Define the number of elements in the array */
  const uint8_t elements = 22;

  /* Allocate memory buffer for the array and get pointer */
  uint8_t* data = (uint8_t*)malloc(elements);

  /* Pointer variable that can be manupulated, while maintaining the original pointer */
  uint8_t* p = data;

  /* Set data element values to an increasing series */
  for (uint8_t i = 0; i < elements; ++i)
  {
    *p++ = i;
  }
  
  /* Transfer the array
     The buffer memory is overwirren with the receievd values during SPI transfer
     so there is no need to allocate a receive buffer. */
  SPI3.transfer(data, elements);

  /* Reset the pointer variable to the original position */
  p = data;

  /* 0 is inital expected results, 255 is when pin is open
     so initialse result variable to 111 to make it clear the value has changed. */
  uint8_t result = 111;

  /* Display titles */
  Serial.println();
  Serial.println("Multi byte transfer test");
  Serial.println("element value result");

  /* Loop through all elements */
  for ( uint8_t i = 0; i < elements; ++i)
  {
    /* Get value refences by pointer and incremnt the pointer */
    result = *p++;
    
    /* Display the array values */
    Serial.print( i, DEC );
    Serial.print("\t");
    Serial.print( result, DEC );
    Serial.print("\t");
 
    /* Check to see if it element matches expected values */
    if ( result == i )   
    {
      /* data element is correct */
      Serial.println("pass");
    }
    else 
    {
      /* Data element is differet */
      Serial.println("fail");      
    }
  }
}

/**
 * @brief Initialize Serial and SPI3.
 * 
 * @details Run 8bit, 16bit and array examples.
 */
void setup() {
  /* put your setup code here, to run once: */

  /* start the serial port */
  Serial.begin(115200);

  /* Title output */
  Serial.println("SPI3 loopback test");
  
  /* Start the SPI library */
  SPI3.begin();

  /* Configure the SPI port */
  SPI3.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));

  /* 8 bit transfer example */
  transfer8bit();

  /* 16 bit transfer example */
  transfer16bit();

  /* Array transfer example */
  transferArray();

  /* End the SPI transation */
  SPI3.endTransaction();

  /* Disable SPI port and free pins for use as GPIO */
  SPI3.end();

  /* Disable serial port and free pins for use as GPIO */
  Serial.end();
}

/**
 * @brief Run repeatedly.
 * 
 * @details Does not do anything.
 */
void loop() {
  /* put your main code here, to run repeatedly: */

}
