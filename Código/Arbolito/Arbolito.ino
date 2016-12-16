#include <SPI.h>
#include "hsv2rgb.h"

//Data es MOSI (atmega168/328: pin 11. Mega: 51) 
//Clock es SCK (atmega168/328: pin 13. Mega: 52)
//Latch es pin 8

const int ShiftPWM_latchPin=8;
const bool ShiftPWM_invertOutputs = 0;

#include <ShiftPWM.h>   //Descargar de aqui: http://arduino.cc/forum/index.php?action=dlattach;topic=66988.0;attach=18001
                        //Video de Elliot Matson: https://www.youtube.com/watch?v=eyhpEVQhKLc

unsigned char maxBrightness = 255;
unsigned char pwmFrequency = 75;
int numRegisters = 2;

void setup()   {                
  pinMode(ShiftPWM_latchPin, OUTPUT);  
  SPI.setBitOrder(LSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4); 
  SPI.begin(); 
  ShiftPWM.SetAmountOfRegisters(numRegisters);
  ShiftPWM.Start(pwmFrequency,maxBrightness);  
}



void loop(){    

  Navidad(2,25,5);    //Rise Time, OnTime, Reps
  Todos(10,1,2);
  Todos(100,0,2);   //Delay, RGB o HUE, Reps
  Fades(500,5);     //Delay en Micros, Repeticiones
  Arcoiris(numRegisters*8/3, 7, 5, maxBrightness, numRegisters*8/3*4,1); //Numero de tiras, Delay, Repeticiones, Brillo maximo, Ancho del arcoiris, Rebota
  ShiftPWM.OneByOneFast();  
  Arcoiris(numRegisters*8/3, 7, 5, maxBrightness, numRegisters*8/3*4,0);
}


void Fades(int Delay, int Rep){

for(int i=0;i<Rep;i++){
  for(int output=0;output<numRegisters*8-4;output++){
    ShiftPWM.SetAll(0);
    for(int brightness=0;brightness<maxBrightness;brightness++){
      ShiftPWM.SetOne(output+1,brightness);
      ShiftPWM.SetOne(output,maxBrightness-brightness);
      delayMicroseconds(Delay);
      }
    }

   for(int output=numRegisters*8-1;output>0;output--){
    ShiftPWM.SetAll(0);
    for(int brightness=0;brightness<maxBrightness;brightness++){
      ShiftPWM.SetOne(output-1,brightness);
      ShiftPWM.SetOne(output,maxBrightness-brightness);
      delayMicroseconds(Delay);
      }
    }
  }
}


void Arcoiris(int numRGBLeds, int delayVal, int numCycles, int maxBrightness, int rainbowWidth,bool Rebota){
  int hue, sat, val; 
  unsigned char red, green, blue;

  ShiftPWM.SetAll(0);

if(Rebota==0){  
  for(int cycle=0;cycle<numCycles;cycle++){ 
    for(int colorshift=0;colorshift<360;colorshift++){ 
      for(int led=0;led<numRGBLeds;led++){ 
        hue = ((led)*360/(rainbowWidth-1)+colorshift)%360;
        sat = 255;
        val = 255;
        hsv2rgb(hue, sat, val, &red, &green, &blue, maxBrightness); 
        ShiftPWM.SetGroupOf3(led, red, green, blue); 
      }
      delay(delayVal);
    } 
  }
}

if(Rebota==1){

  for(int i=0;i<numCycles;i++){
      for(int colorshift=0;colorshift<360;colorshift++){ 
        for(int led=0;led<numRGBLeds;led++){ 
          hue = ((led)*360/(rainbowWidth-1)+colorshift)%360; 
          sat = 255;
          val = 255;
          hsv2rgb(hue, sat, val, &red, &green, &blue, maxBrightness); 
          ShiftPWM.SetGroupOf3(led, red, green, blue); 
        }
        delay(delayVal);
      } 
    
    for(int colorshift=360;colorshift>0;colorshift--){
      for(int led=0;led<numRGBLeds;led++){ 
        hue = ((led)*360/(rainbowWidth-1)+colorshift)%360; 
        sat = 255;
        val = 255;
        hsv2rgb(hue, sat, val, &red, &green, &blue, maxBrightness);
        ShiftPWM.SetGroupOf3(led, red, green, blue); 
      }
      delay(delayVal);
    } 
  }
}
    
}

void Todos(int Delay,bool RGB,int Reps){
  int hue, sat, val; 
  unsigned char red, green, blue;
if(RGB==1){  
  for(int i=0;i<Reps;i++){
    // Fade in all outputs
  for(int j=0;j<maxBrightness;j++){
    ShiftPWM.SetAllRGB(j,0,0);  
    delay(Delay);
  }
  // Fade out all outputs
  for(int j=maxBrightness;j>=0;j--){
    ShiftPWM.SetAllRGB(j,0,0);  
    delay(Delay);
  }
  delay(Delay);
    for(int j=0;j<maxBrightness;j++){
    ShiftPWM.SetAllRGB(0,j,0);  
    delay(Delay);
  }
  // Fade out all outputs
  for(int j=maxBrightness;j>=0;j--){
    ShiftPWM.SetAllRGB(0,j,0);  
    delay(Delay);
    delay(Delay);
  }
    for(int j=0;j<maxBrightness;j++){
    ShiftPWM.SetAllRGB(0,0,j);  
    delay(Delay);
  }
  // Fade out all outputs
  for(int j=maxBrightness;j>=0;j--){
    ShiftPWM.SetAllRGB(0,0,j);  
    delay(Delay);
  }
  delay(Delay);
  }
 }
else{
  for(int i=0;i<Reps;i++){
     for(int colorshift=0;colorshift<360;colorshift++){ 
        hue = (colorshift)%360; 
        sat = 255;
        val = 255;
        hsv2rgb(hue, sat, val, &red, &green, &blue, maxBrightness); 
        ShiftPWM.SetAllRGB(red, green, blue); 
        delay(Delay);
      } 
    }
  }
}

void Navidad(int Rise,int OnTime, int Reps){

for(int i;i<Reps;i++){
    for(int j=0;j<maxBrightness;j++){
        ShiftPWM.SetGroupOf3(0,j,0,0);
        ShiftPWM.SetGroupOf3(2,j,0,0);  
        ShiftPWM.SetGroupOf3(4,j,0,0);
        ShiftPWM.SetGroupOf3(1,0,j,0); 
        ShiftPWM.SetGroupOf3(3,0,j,0);          
      delay(Rise);
    }
    for(int j=maxBrightness;j>=0;j--){
        ShiftPWM.SetGroupOf3(0,j,0,0);
        ShiftPWM.SetGroupOf3(2,j,0,0);  
        ShiftPWM.SetGroupOf3(4,j,0,0);
        ShiftPWM.SetGroupOf3(1,0,j,0); 
        ShiftPWM.SetGroupOf3(3,0,j,0); 
      delay(Rise);
  }

      delay(OnTime);
      
      for(int j=0;j<maxBrightness;j++){
        ShiftPWM.SetGroupOf3(0,0,j,0);
        ShiftPWM.SetGroupOf3(2,0,j,0);  
        ShiftPWM.SetGroupOf3(4,0,j,0);
        ShiftPWM.SetGroupOf3(1,j,0,0); 
        ShiftPWM.SetGroupOf3(3,j,0,0);          
      delay(Rise);
    }
    for(int j=maxBrightness;j>=0;j--){
        ShiftPWM.SetGroupOf3(0,0,j,0);
        ShiftPWM.SetGroupOf3(2,0,j,0);  
        ShiftPWM.SetGroupOf3(4,0,j,0);
        ShiftPWM.SetGroupOf3(1,j,0,0); 
        ShiftPWM.SetGroupOf3(3,j,0,0); 
      delay(Rise);
  }
      delay(OnTime);
  }
}

