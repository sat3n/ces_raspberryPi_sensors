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
#define BUFFER_SIZE 0x19      //0x00 to 0x19

using namespace std;

char dataBuffer[BUFFER_SIZE];
// the time is in the registers in encoded decimal form
int bcdToDec(char b) { 
	return (b/16)*10 + (b%16); 
	}

char decToBcd (int b) {
	return (((b/10) << 4) | (b % 10));
	}
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
int bToD(int n) { 
     
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

int modifyBit(int n, int p, int b) { 
    int mask = 1 << p; 
    return (n & ~mask) | ((b << p) & mask); 
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

void togglealarm1(){

 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x07])), 7, 1), 0x07);
 //manipulating Second bits 0-3
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x07])), 0, 0), 0x07);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x07])), 1, 1), 0x07);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x07])), 2, 0), 0x07);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x07])), 3, 1), 0x07);
 
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x08])), 7, 1), 0x08);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x09])), 7, 1), 0x09);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0A])), 7, 1), 0x0A);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 0, 1), 0x0E);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 2, 1), 0x0E);
 
 cout<<"Alarm 1 activated"<<endl;
 cout << " Val@07h : " << dToB(bcdToDec(dataBuffer[0x07]))<<endl;
 cout << " Val@08h : " << dToB(bcdToDec(dataBuffer[0x08]))<<endl;
 cout << " Val@09h : " << dToB(bcdToDec(dataBuffer[0x09]))<<endl;
 cout << " Val@0Ah : " << dToB(bcdToDec(dataBuffer[0x0A]))<<endl;
}


void togglealarm2(){

 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0B])), 7, 1), 0x0B);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0C])), 7, 1), 0x0B);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0D])), 7, 1), 0x0B);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 2, 1), 0x0B);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 1, 1), 0x0B);
 cout<<"Alarm 2-8 activated" <<endl;
 
 cout << " Val@0Bh : " << dToB(bcdToDec(dataBuffer[0x0B]))<<endl;
 cout << " Val@0Ch : " << dToB(bcdToDec(dataBuffer[0x0C]))<<endl;
 cout << " Val@0Dh : " << dToB(bcdToDec(dataBuffer[0x0D]))<<endl;
/* modifyBit((bcdToDec(dataBuffer[0x0C])), 7, 1);
 modifyBit((bcdToDec(dataBuffer[0x0D])), 7, 1);
 modifyBit((bcdToDec(dataBuffer[0x0E])), 2, 1);
 modifyBit((bcdToDec(dataBuffer[0x0E])), 1, 1);*/
}



void disablealarms(){
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 2, 0), 0x0E);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 1, 0), 0x0E);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 0, 0), 0x0E);
 cout<<"Alarms disabled" <<endl;
 cout << " Val@07h : " << dToB(bcdToDec(dataBuffer[0x07]))<<endl;
 cout << " Val@08h : " << dToB(bcdToDec(dataBuffer[0x08]))<<endl;
 cout << " Val@09h : " << dToB(bcdToDec(dataBuffer[0x09]))<<endl;
 cout << " Val@0Ah : " << dToB(bcdToDec(dataBuffer[0x0A]))<<endl;
 cout << " Val@0Bh : " << dToB(bcdToDec(dataBuffer[0x0B]))<<endl;
 cout << " Val@0Ch : " << dToB(bcdToDec(dataBuffer[0x0C]))<<endl;
 cout << " Val@0Dh : " << dToB(bcdToDec(dataBuffer[0x0D]))<<endl;
 
 //modifyBit((bcdToDec(dataBuffer[0x0E])), 1, 0);
 //modifyBit((bcdToDec(dataBuffer[0x0E])), 1, 0);
}

void checkDefault(){
   cout<<"Default register values are :" <<endl;
 cout << " Val@07h : " << dToB(bcdToDec(dataBuffer[0x07]))<<endl;
 cout << " Val@08h : " << dToB(bcdToDec(dataBuffer[0x08]))<<endl;
 cout << " Val@09h : " << dToB(bcdToDec(dataBuffer[0x09]))<<endl;
 cout << " Val@0Ah : " << dToB(bcdToDec(dataBuffer[0x0A]))<<endl;
 cout << " Val@0Bh : " << dToB(bcdToDec(dataBuffer[0x0B]))<<endl;
 cout << " Val@0Ch : " << dToB(bcdToDec(dataBuffer[0x0C]))<<endl;
 cout << " Val@0Dh : " << dToB(bcdToDec(dataBuffer[0x0D]))<<endl;
}
void setDefault(){
   cout<<"Register values reset to default:" <<endl;
    writeToDevice(modifyBit((bcdToDec(dataBuffer[0x07])), 7, 0), 0x07);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x08])), 7, 0), 0x08);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x09])), 7, 0), 0x09);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0A])), 7, 0), 0x0A);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0B])), 7, 0), 0x0B);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0C])), 7, 0), 0x0B);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0D])), 7, 0), 0x0B);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 1, 0), 0x0B);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 0, 0), 0x0E);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 2, 0), 0x0E);
 cout << " Val@07h : " << dToB(bcdToDec(dataBuffer[0x07]))<<endl;
 cout << " Val@08h : " << dToB(bcdToDec(dataBuffer[0x08]))<<endl;
 cout << " Val@09h : " << dToB(bcdToDec(dataBuffer[0x09]))<<endl;
 cout << " Val@0Ah : " << dToB(bcdToDec(dataBuffer[0x0A]))<<endl;
 cout << " Val@0Bh : " << dToB(bcdToDec(dataBuffer[0x0B]))<<endl;
 cout << " Val@0Ch : " << dToB(bcdToDec(dataBuffer[0x0C]))<<endl;
 cout << " Val@0Dh : " << dToB(bcdToDec(dataBuffer[0x0D]))<<endl;
}

int main(){
	readSensorState();
	
        cout<<"Starting the Write section of the code"<<endl;
	cout <<" Hours :"<< bcdToDec(dataBuffer[0x02]) << endl;
	
	cout <<" Minutes:" << bcdToDec(dataBuffer[0x01])<<endl;
        cout << " Seconds : " << bcdToDec(dataBuffer[0x00]) <<endl;
	
	//disablealarms();
	
	togglealarm1();
	//togglealarm18();
	//togglealarm2();
	//togglealarm28();
	//checkDefault();
	//setDefault();
         cout << " Val@0Eh : " << dToB(bcdToDec(dataBuffer[0x0E]))<<endl
	 <<" DeciamlVal@0Eh : "<< (bcdToDec(dataBuffer[0x0E])) <<endl;
	//cout <<endl<<"Minutes in binary:" << dToB(bcdToDec(dataBuffer[0x01]))<<endl; 
	
	//or
	//writeToDevice(decToBcd(22), 0x0E);
	
	//writeToDevice(decToBcd(2), 0x01 );
	//writeToDevice(decToBcd(12), 0x02 );
	//cout <<"Modified bit output : " <<modifyBit(bcdToDec(dataBuffer[0x01]), 0, 1)<<endl;
	//writeToDevice(decToBcd(12), 0x00 );
	
   return 0;
}
