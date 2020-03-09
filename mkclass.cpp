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
class DS3231{
	private: 
	
public:
	int readSensorState(); //read rtc memory registers
	int writeToDevice(char data, char address); //writes to rtc memory registers
	//virtual void alram1(); //sets the second alarm
	//virtual void alarm2(); //sets the minute alarm
	//virtual void alramsoff(); //turns the alarm off
	//virtual void chkbinval(); //checks binary values at registers
	//virtual void checktemp(); //checks the temperature
};
int DS3231::readSensorState(){
	
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

int DS3231::writeToDevice(char data, char address){
	
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
   this->char address;
   cout<< "The value on register " 
   << "is : " << bcdToDec(dataBuffer[char address]) << endl;
	return 0;
}
/*
void DS3231::alram1(){

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


void DS3231::alarm2(){

 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0B])), 7, 1), 0x0B);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0C])), 7, 1), 0x0B);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0D])), 7, 1), 0x0B);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 2, 1), 0x0B);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 1, 1), 0x0B);
 cout<<"Alarm 2-8 activated" <<endl;
 
 cout << " Val@0Bh : " << dToB(bcdToDec(dataBuffer[0x0B]))<<endl;
 cout << " Val@0Ch : " << dToB(bcdToDec(dataBuffer[0x0C]))<<endl;
 cout << " Val@0Dh : " << dToB(bcdToDec(dataBuffer[0x0D]))<<endl;
 modifyBit((bcdToDec(dataBuffer[0x0C])), 7, 1);
 modifyBit((bcdToDec(dataBuffer[0x0D])), 7, 1);
 modifyBit((bcdToDec(dataBuffer[0x0E])), 2, 1);
 modifyBit((bcdToDec(dataBuffer[0x0E])), 1, 1);
}

void DS3231::alramsoff(){
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

void DS3231::chkbinval(){
   cout<<"Register values in binary format are :" <<endl;
 cout << " Val@07h : " << dToB(bcdToDec(dataBuffer[0x07]))<<endl;
 cout << " Val@08h : " << dToB(bcdToDec(dataBuffer[0x08]))<<endl;
 cout << " Val@09h : " << dToB(bcdToDec(dataBuffer[0x09]))<<endl;
 cout << " Val@0Ah : " << dToB(bcdToDec(dataBuffer[0x0A]))<<endl;
 cout << " Val@0Bh : " << dToB(bcdToDec(dataBuffer[0x0B]))<<endl;
 cout << " Val@0Ch : " << dToB(bcdToDec(dataBuffer[0x0C]))<<endl;
 cout << " Val@0Dh : " << dToB(bcdToDec(dataBuffer[0x0D]))<<endl;
}

void DS3231::checktemp(){

}*/

int main(){
DS3231 rtc;
rtc.readSensorState();
rtc.writeToDevice(decToBcd(2), 0x01 );
rtc.writeToDevice(decToBcd(12), 0x02 );
rtc.writeToDevice(decToBcd(12), 0x00 );
	cout <<" Hours : "<< bcdToDec(dataBuffer[0x02]) << endl;
	cout <<" Minutes: " << bcdToDec(dataBuffer[0x01])<<endl;
    cout << " Seconds : " << bcdToDec(dataBuffer[0x00]) <<endl;
}
