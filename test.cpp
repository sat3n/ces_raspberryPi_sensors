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
#include <bits/stdc++.h>
#define BUFFER_SIZE 0x12      //0x00 to 0x12

using namespace std;


// the time is in the registers in encoded decimal form
int bcdToDec(char b) { 
	return (b/16)*10 + (b%16); 
	}

char decToBcd (int b) {
	return (((b/10) << 4) | (b % 10));
	}
	
int modifyBit(int n, int p, int b) 
{ 
    int mask = 1 << p; 
    return (n & ~mask) | ((b << p) & mask); 
} 
char dataBuffer[BUFFER_SIZE];
// Decimal to Binary conversion
int dToB(int x){
int remainder;
int i=1;
int bin_value=0;

while(x!=0){
    remainder = x%2;
    x /= 2;
    bin_value += remainder*i;
    i *= 10;
}
return bin_value;
}
// Binary to Decimal conversion
int bToD(int n) 
{ 
     
    int dec_value = 0; 
    int base = 1; 
    int temp = n; 
    
    while (temp) { 
        int last_digit = temp % 10; 
        temp = temp / 10; 
        dec_value += last_digit * base; 
        base = base * 2; 
    } 
  
    return dec_value; 
}

void togglealarm1(){
 int z =(bcdToDec(dataBuffer[0x0E]));
 modifyBit(z, 0, 1);
 modifyBit(z, 1, 1);
 modifyBit(z, 2, 1);
 modifyBit(z, 3, 1);
 modifyBit(z, 4, 1);
 cout << "Modified 0Eh bits : " << dToB(bcdToDec(dataBuffer[0x0E])) <<endl;
}

void disablealarms(){
 int z =(bcdToDec(dataBuffer[0x0E]));
 modifyBit(z, 0, 0);
 modifyBit(z, 1, 0);
 modifyBit(z, 2, 0);
}

void togglealarm2(){
 int z =(bcdToDec(dataBuffer[0x0E]));
 modifyBit(z, 0, 1);
 modifyBit(z, 2, 1);
}

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
	
        cout<<"Starting the Write section of the code"<<endl;
	cout <<" Hours :"<< bcdToDec(dataBuffer[0x02]) << endl;
	
	cout <<" Minutes:" << bcdToDec(dataBuffer[0x01])<<endl;
        cout << " Seconds : " << bcdToDec(dataBuffer[0x00]) <<endl;
	writeToDevice(decToBcd(2), 0x0E );
	cout << " Fetching value from 0Eh : " << dToB(bcdToDec(dataBuffer[0x0E])) <<endl;
	togglealarm1();

	cout <<endl<<"Minutes in binary:" << dToB(bcdToDec(dataBuffer[0x01]))<<endl; 
	
	
	writeToDevice(decToBcd(2), 0x01 );
	writeToDevice(decToBcd(12), 0x02 );
	cout <<"Modified bit output : " <<modifyBit(bcdToDec(dataBuffer[0x01]), 0, 1)<<endl;
	//writeToDevice(decToBcd(12), 0x00 );
	
   return 0;
}
