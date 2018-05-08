#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "Adafruit_FRAM_I2C.h"
//#include <XLR8ADC.h>;

// pin 11 = MOSI, pin 12 = MISO, pin 13 = SCK.
#define SPI_SCK 13
#define SPI_DI  12
#define SPI_DO  11

#define SD_CS    4  // Chip select line for SD card
#define TFT_CS  10  // Chip select line for TFT display
#define TFT_DC   9  // Data/command line for TFT
#define TFT_RST  8  // Reset line for TFT (or connect to +5V)

Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();
uint16_t framAddr = 0;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, SPI_DO, SPI_SCK, TFT_RST);

File test_file; //created a file called test_file.txt
char filename[9] = "test.txt";//if name is changed, you must change to size +1


int test_ram = 3.116;
int test_ram2 = 9.91;
int ADC_val = 0;
int i = 0;
int x =0;

//setup timers
long stime;
long endtime1;
long endtime2;
long endtime3;
//----------------convert int to byte-----------------
// MACRO : Combines FROM_BYTES into TO_OBJECT of datatype "DATATYPE"
#define CombineBytes(FROM_BYTES, DATATYPE, TO_OBJECT) \
  TO_OBJECT = *((DATATYPE *)FROM_BYTES);

// MACRO : Splits FROM_DATA of DATATYPE into equivalent byte array TO_BYTES
#define GetBytes(FROM_DATA, DATATYPE, TO_BYTES) \
  *((DATATYPE *)TO_BYTES) = FROM_DATA;



// Writes int's equivalent bytes to Address, Address + 1, Address + 2 and Address + 3
void fram_Write_int(unsigned int Address, int Data)
{
  // Get the size of datatype to write number of bytes
  const byte SizeOf_DataType = sizeof(int);

  // Create tem buffer to get Bytes of Datatype
  byte Temp_Buffer[SizeOf_DataType];

  // Call macro to store byte array into Temp_Buffer from Data
  GetBytes(Data, int, Temp_Buffer);

  // Write number of bytes to memory location specified in Address and so on
  for(byte Index = 0; Index < SizeOf_DataType; Index++)
  {
    fram.write8(Address + Index, Temp_Buffer[Index]);
  }
}

// Reads four bytes from Address, Address + 1, Address + 2 and Address + 3 and returns equivalent int
int fram_Read_int(unsigned int Address)
{
  // Get the size of datatype to write number of bytes
  const byte SizeOf_DataType = sizeof(int);

  // Create tem buffer to get Bytes of Datatype
  byte Temp_Buffer[SizeOf_DataType];

  int Result;

  // Read number of bytes from memory location
  for(byte Index = 0; Index < SizeOf_DataType; Index++)
  {
    Temp_Buffer[Index] = fram.read8(Address + Index);
  }

  // Call macro to combine bytes into equivalent to int
  CombineBytes(Temp_Buffer, int, Result);

  return Result;
}
//------------------------end macros------------------------------


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial){
    ;
    }
      //-------------------initialize ADC
//    XLR8ADC.begin();
//  XLR8ADC.set12bitMode();
  stime = millis(); //starts timer 
  cli(); // disables interupts
 
  ADCSRA = 0;             // clear ADCSRA register
  ADCSRB = 0;             // clear ADCSRB register
  ADMUX |= (0 & 0x07);    // set A0 analog input pin
  ADMUX |= (1 << REFS0);  // set reference voltage
  ADMUX |= (1 << ADLAR);  // left align ADC value to 8 bits from ADCH register

  // sampling rate is [ADC clock] / [prescaler] / [conversion clock cycles]
  // for Arduino Uno ADC clock is 16 MHz and a conversion takes 13 clock cycles
  
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0);    // 32 prescaler for 38.5 KHz
  
  //  ADCSRA |= (1 << ADPS2);                     // 16 prescaler for 76.9 KHz
  //ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    // 8 prescaler for 153.8 KHz

  ADCSRA |= (1 << ADATE); // enable auto trigger
  ADCSRA |= (1 << ADIE);  // enable interrupts when measurement complete

 //------------- CHECK ME!!!!!!!!!!!!------------------------------------
//  ADCSRA |= (1 << AD12EN);  // enable ADC xlr8
  ADCSRA |= (1 << ADEN); //enable ADC arduino
  ADCSRA |= (1 << ADSC);  // start ADC measurements

 
  
  // If your TFT's plastic wrap has a Black Tab, use the following:
    tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  // If your TFT's plastic wrap has a Red Tab, use the following:
//  tft.initR(INITR_REDTAB);   // initialize a ST7735R chip, red tab
  // If your TFT's plastic wrap has a Green Tab, use the following:
  //  tft.initR(INITR_GREENTAB); // initialize a ST7735R chip, green tab
   
  //-----------------initialize ram
  Serial.print("Initializing RAM card...");
  if(!fram.begin()){
    Serial.println("failed!");
    return;
    }
   Serial.println("OK!");
   
  //initialize sd card
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS, SPI_DO, SPI_DI, SPI_SCK)) {
    Serial.println("failed!");
    return;
  }
  x = 1;
  Serial.println("OK!");

 //-----------------------begin writing to RAM------------

 //-----------------------wait til all ADC values are collected---
 
 Serial.println("Ryan1");
 
  while (i < 24000){
    Serial.print(i);
    }
Serial.println("Ryan2");
  //check whether sd card file exists
  if(SD.exists(filename)){
    SD.remove(filename);
    Serial.println("exists");
    }
  else {
    Serial.println("does not exist");
    }

  //write from RAM to sd card
  test_file = SD.open(filename,FILE_WRITE); //file is now open and primed to be written from

  if (test_file) {
    Serial.println("Writing to test.txt");
//    test_file.println("here is my variables");




//    stime = micros();
  for(int i = 0; i < 24000 ;i=i+2){
    test_file.println(fram_Read_int(i));
  }
//    endtime3 = micros()-stime;

    Serial.println("done writing to sd card");
    
    

//int    x = fram_Read_int(0x0);
//    test_file.print(x);
    
     //close file
     test_file.close();
  }
  

}

ISR(ADC_vect){
  ADC_val = ADCH;
  if((i<24000) && (x == 1)){
    fram_Write_int(i,ADC_val);//address filled from 0-1
    i = i+2;  
    Serial.println("Ryan");
    }
  }

void loop() {
  //  Serial.end();
  //  Serial.begin(9600);
  // put your main code here, to run repeatedly:



  //  File test_file; //created a file called test_file.txt
  //
  //    test_file = SD.open(test_file.txt, FILE_WRITE); //file is now open and primed to be written to
  //
  //    if (test_file){
  //      Serial.print("Writing to test_file.txt");
  //      test_file.println("I have written something interesting"); //actual writing to our file
  //      test_file.close(); //close file b/c we are done writing
  //      }
  //
  //      else{
  //        Serial.println("it didnt work");
  //      }

}
