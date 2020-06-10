/*
AD9833 Waveform Module vwlowen.co.uk
*/

#include <SPI.h>
                   
const int SQUARE = 0x2028;                  // SINE -> 0x2000 , TRIANGLE -> 0x2002.                                   

const float refFreq = 25000000.0;           // Reference frequency.

const int FSYNC = 53;                       // Active low control.
const int CLK = 52;                         // Clock.                        
const int DATA = 51;                        // Data.

unsigned long freq = 50;                    // Square wave frequency.

void setup() {
  
  SPI.begin();
  
  AD9833reset();                            // Reset to '0' all the registers.                                   
  delay(50);
  AD9833setFrequency(freq, SQUARE);         // Set the frequency and the waveform.                          
 }



void loop() {
  
}

void AD9833reset() {
  WriteRegister(0x100);                     // Write '1' to AD9833 control register bit D8.             
  delay(10);
}

void AD9833setFrequency(long frequency, int waveform) {

  long FreqWord = (frequency * pow(2, 28)) / refFreq;

  int LSB = (int)(FreqWord & 0x3FFF);
  
  LSB |= 0x4000;                            // AD9833 control register bits D15 and D14 must be '0' and '1', respectively.
  
  WriteRegister(0x2100);                    // Control register initialization.  
  WriteRegister(LSB);                       // Write lower 16 bits to AD9833 registers.               
  WriteRegister(0x4000);                    // Write upper 16 bits to AD9833 registers.                  
  WriteRegister(0xC000);                    // Phase register.              
  WriteRegister(waveform);                  // Exit and reset.             

}

void WriteRegister(int dat) {
  
  SPI.setDataMode(SPI_MODE2);      
  
  digitalWrite(FSYNC, LOW);                 // Set FSYNC low before writing to AD9833 registers.          
  delayMicroseconds(10);             
  
  SPI.transfer(highByte(dat));              // It has to be divided into 2x 8 bits.        
  SPI.transfer(lowByte(dat));         

  digitalWrite(FSYNC, HIGH);                // Set FSYNC high after writing to AD9833 registers.          
}
