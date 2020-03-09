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
	virtual void alarm1(); //sets the second alarm
	virtual void alarm2(); //sets the minute alarm
	virtual void alarmsoff(); //turns the alarm off
	virtual void SqrWave(); //checks binary values at registers
	virtual void checktemp(); //checks the temperature
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
         bcdToDec(dataBuffer[0x01])<<":"<<bcdToDec(dataBuffer[0x00]) << endl 
	 << " And Date is : " << bcdToDec(dataBuffer[0x04])<<"-"
   <<bcdToDec(dataBuffer[0x05])<<"-"<< bcdToDec(dataBuffer[0x06])<<endl;

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
  return 0;
}

void DS3231::alarm1(){
cout<<"Alarm 1 activated"<<endl;
readSensorState();
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x07])), 7, 1), 0x07);
 readSensorState();
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x08])), 7, 1), 0x08);
 readSensorState();
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x09])), 7, 1), 0x09);
 readSensorState();
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0A])), 7, 1), 0x0A);
 readSensorState();
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 0, 1), 0x0E);
 readSensorState();
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 2, 1), 0x0E);
}


void DS3231::alarm2(){
cout<<"Alarm 2 activated!!" <<endl;
readSensorState();
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0B])), 7, 1), 0x0B);
 readSensorState();
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0C])), 7, 1), 0x0C);
 readSensorState();
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0D])), 7, 1), 0x0D);
 readSensorState();
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 1, 1), 0x0E);
 readSensorState();
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 2, 1), 0x0E);

}

void DS3231::alarmsoff(){
    cout<<"Alarms disabled" <<endl;
   readSensorState();
 
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 2, 0), 0x0E);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 1, 0), 0x0E);
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 0, 0), 0x0E);

}

void DS3231::SqrWave(){
   cout<<"Generating SqrWave at 4.096kHz" <<endl;
   readSensorState();
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 0, 0), 0x0E);
 readSensorState();
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 3, 1), 0x0E);
 readSensorState();
 writeToDevice(modifyBit((bcdToDec(dataBuffer[0x0E])), 4, 0), 0x0E);
}

void DS3231::checktemp(){
    cout <<"Temperature is " << bcdToDec(dataBuffer[0x11])<<"."<< bcdToDec(dataBuffer[0x12]) <<endl;
}

int main(){
    DS3231 rtc;
	cout <<"DS3231 application initiated.  Select from below ptions" <<endl;
	cout <<"1 . Read RTC data" <<endl << "2. Write Time to RTC" << endl
	<<"3. Toggle first alarm" <<endl << "4. Toggle second alarm" <<endl
	<<"5. Disable Alarms" <<endl<< "6. Check Temperature" <<endl
	<<"7. Generate Square Wave"<<endl;
	int i;
	cout<<"Choose option : "; cin >> i;
	
	if(i==1){
	rtc.readSensorState();
	}
	
	else if(i==2){
	    int i, j, k;
	    cout<<"Enter hours :";
	    cin>>i;
	    rtc.writeToDevice(decToBcd(i), 0x02);
	    cout<<endl<<"Enter minutes :";
	    cin>>j;
	    rtc.writeToDevice(decToBcd(j), 0x01);
	    cout<<endl<<"Enter seconds :";
	    cin>>k;
	    rtc.writeToDevice(decToBcd(k), 0x00);
	    rtc.readSensorState();
	}
	else if(i==3){
	    rtc.alarm1();
	}
	else if(i==4){
	    rtc.alarm2();
	}
	else if(i==5){
	    rtc.alarmsoff();
	}
	else if(i==6){
	    rtc.checktemp();
	}
	else if(i==7){
	    rtc.SqrWave();
	    }

return 0;

}
