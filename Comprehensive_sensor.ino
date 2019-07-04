// This code includes there different "functions": 1. Tempereture and humiduty value controlled LED stripes. 2. Air quality sensor controlled LED 3. Foot detecting 
// scheme. 
// Please note that befure running the code, you must download all the supported library which are used in this file.
// Ziran He (James) Date: 29/06/2019
#include "dht.h"
#define dht_apin A0 // Analog Pin sensor is connected to

// I2C Bus
#include <Wire.h>
// Include NewLiquidCrystal Library for I2C
#include <LiquidCrystal_I2C.h>

int sensorValue;

// Define LCD pinout
const int  en = 2, rw = 1, rs = 0, d4 = 4, d5 = 5, d6 = 6, d7 = 7, bl = 3;
//const int  en = 4, rw = 5, rs = 5, d4 = 0, d5 = 1, d6 = 2, d7 = 3, bl = 7;

// Define I2C Address - change if reqiuired
const int i2c_addr = 0x27;

LiquidCrystal_I2C lcd(i2c_addr, en, rw, rs, d4, d5, d6, d7, bl, POSITIVE);

//#include "HX711.h"

//#define DOUT  3
//#define CLK  2
//HX711 scale(DOUT, CLK);
//float calibration_factor = -96650;
 
dht DHT;

#include <FastLED.h>
#define NUM_STRIPS 2
#define NUM_LEDS_PER_STRIP 60
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS

CRGB airleds[NUM_LEDS_PER_STRIP];
CRGB temleds[NUM_LEDS_PER_STRIP];

int Air_arrays [5];
int N_air = 0;
int color_tem;    
int Air_sum;
int Air_mean;
int load;
float sensitivity = 5;
int N_load;

int demo = 1;

#include "HX711.h"  //You must have this library in your arduino library folder
#define DOUT  9
#define CLK  8
 
HX711 scale(DOUT, CLK);
 
//Change this calibration factor as per your load cell once it is found you many need to vary it in thousands
float calibration_factor = -2100; //-106600 worked for my 40Kg max scale setup //-96650
 
//=============================================================================================
//                         SETUP
//=============================================================================================

 
void setup(){
  Serial.begin(9600);

  Serial.println("CLEARDATA");
  Serial.println("LABEL,Acolumn,Bcolumn,...");
  Serial.println("RESETTIMER");
  
  
  delay(500);//Delay to let system boot
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  delay(500);//Wait before accessing Sensor
  pinMode(13, OUTPUT);
  
  FastLED.addLeds<WS2812, 3, GRB>(airleds, NUM_LEDS_PER_STRIP);
  //FastLED.addLeds<WS2812, 5, GRB>(temleds, NUM_LEDS_PER_STRIP);

   //For finding the adress of the LCD display

//    while (!Serial) 
//    {
//    }
//
//    Serial.println ();
//    Serial.println ("I2C scanner. Scanning ...");
//    byte count = 0;
//    Wire.begin();
//    for (byte i = 8; i < 120; i++)
//    {
//      Wire.beginTransmission (i);
//      if (Wire.endTransmission () == 0)
//        {
//        Serial.print ("Found address: ");
//        Serial.print (i, DEC);
//        Serial.print (" (0x");
//        Serial.print (i, HEX);
//        Serial.println (")");
//        count++;
//        delay (1);  // maybe unneeded?
//        } // end of good response
//    } // end of for loop
//    Serial.println ("Done.");
//    Serial.print ("Found ");
//    Serial.print (count, DEC);
//    Serial.println (" device(s).");



// Set display type as 16 char, 2 rows
//  lcd.begin(16,2);
//
//  // Print on first row
//  lcd.setCursor(0,0);
//  lcd.print("Hello!");
//  
//  // Wait 1 second
//  delay(2000);
//  
//  // Print on second row
//  lcd.setCursor(0,0);
//  lcd.print("Welcome to the");
//  lcd.setCursor(0,1);
//  lcd.print("open day!");
//  
  // Wait 8 seconds
  delay(8000);
  
  // Clear the display
  //lcd.clear();


  Serial.println("HX711 Calibration");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press a,s,d,f to increase calibration factor by 10,100,1000,10000 respectively");
  Serial.println("Press z,x,c,v to decrease calibration factor by 10,100,1000,10000 respectively");
  Serial.println("Press t for tare");
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
 
  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
  pinMode(7, INPUT);    // sets the digital pin 7 as input
 
}//end "setup()"
 
void loop(){
  if (demo == 1)
  {
    
    //  Start of Program
    ////////////////////////////////////////
    //Demo 1 Temperature and Humidity
 
    DHT.read11(dht_apin);
    
    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");

    // Print temperature on top line
    lcd.setCursor(0,0);
    lcd.print("Temp:  ");
    lcd.print(DHT.temperature);
    lcd.print(" C");
    
    // Print humidity on bottom line
    lcd.setCursor(0,1);
    lcd.print("Humid: ");
    lcd.print(DHT.humidity);
    lcd.print(" %");

    //color_tem = (DHT.temperature-21)*255/5;
    color_tem = (DHT.humidity-60)*255/30;
    if (color_tem > 255)
    {
      color_tem = 255;
      }
    if (color_tem < 0)
    {
      color_tem  = 0;
      }

    Serial.print("This is color_tem: ");
    Serial.print(color_tem, DEC);
    
     for(int i = 0; i < 60; i++)
    {
      //temleds[i] = CRGB(color_tem,255-color_tem,0);
      //airleds[i] = CRGB(0, 0, 0);
      airleds[i] = CRGB(color_tem,255-color_tem,0);
      temleds[i] = CRGB(0, 0, 0);

      }
    FastLED.show();
    delay (500);
    //FastLED.clear();
    
    }
  else if (demo == 2)
  {
    //////////////////////////////////////////////////////////
    // Demo 2 Air quality
    //Fastest should be once every two seconds.
    sensorValue = analogRead(1);       // read analog input pin 1
    Serial.print("AirQua=");
    Serial.print(sensorValue, DEC);               // prints the value read
    Serial.println(" PPM");

    lcd.setCursor(0,0);
    lcd.print("AirQua: ");
    lcd.print(sensorValue);
    lcd.print(" PPM");
    lcd.setCursor(0,1);
    lcd.print("                  ");

    


    for (int i=4; i>0; i--)
    {
      Air_arrays[i] = Air_arrays[i-1];
      }
      
     Air_arrays[0] = sensorValue;

    Air_sum = 0;
    for (int i=0; i<5; i++)
    {
      Air_sum = Air_sum + Air_arrays[i];
      }
    Air_mean = int(Air_sum/5);
    
    Serial.print("This is Air_mean: ");
    Serial.print(Air_mean, DEC);

    N_air = (sensorValue-50)*60/(40);
    if (N_air > 54)
    {
      N_air = 54;
      }
    if (N_air < 0)
    {
      N_air  = 0;
      }

    Serial.print("This is the position of Air: ");
    Serial.print(N_air, DEC);

     for(int i = 0; i < 60; i++)
    {
      temleds[i] = CRGB(0, 0, 0);
      airleds[i] = CRGB(0, 0, 0);
    }
    for (int i = 0; i < 7; i++)
    {
      airleds[N_air+i] = CRGB(0, 0, 255);
      }
    
    FastLED.show();
    delay (500);
    //FastLED.clear();
    }
  else if (demo == 3)
  {
    /////////////////////////////////////////////////////////////////
    //Demo 3: Load cell force sensing

    scale.set_scale(calibration_factor); //Adjust to this calibration factor
    load  = abs(float (scale.get_units()));


    if (load > sensitivity)
    {
      Serial.print("Force detected!     ");
      lcd.setCursor(0,0);
      lcd.print("Force detected!  ");
      }
    else
    {
      Serial.print("No force detected!     ");
      lcd.setCursor(0,0);
      lcd.print("No force!         ");
      }
 
    Serial.print("Reading: ");
    Serial.print(scale.get_units(), 3);
    Serial.print(" kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
    //Serial.print(" calibration_factor: ");
    //Serial.print(calibration_factor);
    Serial.println();


    lcd.setCursor(0,1);
    lcd.print("Load: ");
    if (abs(scale.get_units()) > 50 )
    {
      lcd.print("50");
      }
    else
    {
      lcd.print(abs(scale.get_units()), 3);
      }
    lcd.print(" kg    ");

    N_load = (load)*60/50;
    if (N_load > 60)
    {
      N_load = 60;
      }
    if (N_load < 0)
    {
      N_load  = 0;
      }

    Serial.print("This is the position of load: ");
    Serial.print(N_load, DEC);
    
    for (int i = 0; i < N_load; i++)
    {
      airleds[i] = CRGB(0, 0, 255);
    }
    for(int i = N_load; i < 60; i++)
    {
      airleds[N_load+i] = CRGB(0, 0, 0);
    }
    
    
    FastLED.show();
    //delay (500);
    //FastLED.clear();
    
//   
//    if(Serial.available())
//    {
//      char temp = Serial.read();
//      if(temp == '+' || temp == 'a')
//        calibration_factor += 10;
//      else if(temp == '-' || temp == 'z')
//        calibration_factor -= 10;
//      else if(temp == 's')
//        calibration_factor += 100;  
//      else if(temp == 'x')
//        calibration_factor -= 100;  
//      else if(temp == 'd')
//        calibration_factor += 1000;  
//      else if(temp == 'c')
//        calibration_factor -= 1000;
//      else if(temp == 'f')
//        calibration_factor += 10000;  
//      else if(temp == 'v')
//        calibration_factor -= 10000;  
//      else if(temp == 't')
//        scale.tare();  //Reset the scale to zero
//     }
    }
    else if (demo == 0)
    {
      lcd.setCursor(0,0);
      lcd.print("Welcome to the");
      lcd.setCursor(0,1);
      lcd.print("Open Day!        ");
      for(int i = 0; i < 60; i++)
      {
        airleds[i] = CRGB(0, 0, 0);
        temleds[i] = CRGB(0, 0, 0);
      }
      FastLED.show();
      }
      

//   if (Serial.available())
//   {
//    char in_demo = Serial.read();
//    if (in_demo == '1')
//    {
//      demo = 1;
//      }
//    else if (in_demo == '2')
//    {
//      demo = 2;
//      }
//    else if (in_demo == '3')
//    {
//      demo = 3;
//      }
//    else if (in_demo == '0')
//    {
//      demo = 0;
//      }
//    }

  if (digitalRead(7) == 1)
  {
    demo =1;
    Serial.print("This is the position of load: ");
    
    }
  else
  {
    demo = 2;
    }
    
    

 
}// end loop() 
