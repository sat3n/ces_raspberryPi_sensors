/*
 * DS3231.c
 * 
 * Copyright 2020  <pi@Rpi-sat3n>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include</usr/include/arm-linux-gnueabihf/sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#define BUFFER_SIZE 0x12      //0x00 to 0x12

using namespace std;


// the time is in the registers in encoded decimal form
int bcdToDec(char b) { 
	return (b/16)*10 + (b%16); 
	}

char decToBcd (int b) {
	return (((b/10) << 4) | (b % 10));
	}
char dataBuffer[BUFFER_SIZE];

int readSensorState(){
	
   int file;
   printf("Starting the DS3231 test application\n");
   if((file=open("/dev/i2c-1", O_RDWR)) < 0){
      cout<<"failed to open the bus\n"<<endl;
      return 1;
   }
   if(ioctl(file, I2C_SLAVE, 0x68) < 0){
      cout<<"Failed to connect to the sensor\n"<<endl;
      return 1;
   }
   char writeBuffer[1] = {0x00};
   if(write(file, writeBuffer, 1)!=1){
      cout<<"Failed to reset the read address\n"<<endl;
      return 1;
   }
   
   if(read(file, dataBuffer, BUFFER_SIZE)!=BUFFER_SIZE){
      cout<<"Failed to read in the buffer\n"<<endl;
      return 1;
   }
   cout << "The RTC time is - " << bcdToDec(dataBuffer[0x02])<<":"<<
         bcdToDec(dataBuffer[0x01])<<":"<<bcdToDec(dataBuffer[0x00]) << endl;
   close(file);
   
   return 0;
}

int writeToDevice (char data, char address){
	
	int file;
    printf("Starting the Write section of the coden");
    
    if((file=open("/dev/i2c-1", O_RDWR)) < 0){
      cout<<"failed to open the bus\n"<< endl;
      return 1;
   }
   if(ioctl(file, I2C_SLAVE, 0x68) < 0){
      cout << "Failed to connect to the sensor\n" <<endl;
      return 2;
   }
   
   char writeBuffer[2];
   writeBuffer[0] = address;
   writeBuffer[1] = data;
   
   if(write (file, writeBuffer, 2) != 2){
	   cout<<"failed to write to device"<<endl;
	   return 0;
   }
	return 0;
}

int main(){
	readSensorState();
	cout << "second : " << bcdToDec(dataBuffer[0x00]) 
	<<" Hours :"<< bcdToDec(dataBuffer[0x02])<<endl;
	writeToDevice(decToBcd(18), 0x01 );
	writeToDevice(decToBcd(12), 0x02 );
	
   return 0;
}
