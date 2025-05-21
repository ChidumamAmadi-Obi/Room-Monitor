
 // PROGRAM DISPLAYS TEMP/HUMIDITY AND AIR QUALITY ON LCD SCREEN
 // Chidumam Amadi-Obi B00167937
 // Starting date: 07-02-2025

 // DHT DIGITAL PIN 6/PIN
 // AIR QUAL SENSOR PIN A0/
 // LCD, SCL PIN A5/ SDA PIN A4
 // ENCODER PINS
    // A PIN 2
    // B PIN 3
    // C PIN 4
//________________________________________________________________

#include "DHT.h"
#include "rgb_lcd.h"
#include "Air_Quality_Sensor.h"
#include <Wire.h>
#include <SimpleRotary.h>
#include <avr/wdt.h>
#include "CustomChars.h" //Custom header file was used for testing and editing

#define DHTPIN 6
#define DHTTYPE DHT11

unsigned long previousMillisAlert = 0;
unsigned long previousMillisLCD = 0;
bool LEDBuzzerState = LOW; //stores state of LEDs and buzzer

int col_cnt = 9;

const int rgb[10][3] = { //stores various RGB LCD colors

  {50,50,50},   // grey
  {255,20,20},  // red
  {20,255,20},  // green
  {20,20,255},  // blue
  {255,255,20}, // yellow
  {255,20,255}, // purple
  {20,255,255}, // mint blue

  {255,45,45},  // redish
  {255,178,46}, // greeny orange
  {200,200,200} // white
  };

int tempUpperLimit = 20;
int tempLowerLimit = 10;
int humiUpperLimit = 40;
int humiLowerLimit = 10; // default temp and humidity parameters

int sysState = 0; //monitors and controlls the state of the system
int menuState = 0; //monitors and controls the state of the menu

SimpleRotary rotary(2,3,4); //tell SimpleRotary that is uses pins 2 and 3 for the encoder and pin 4 for the button
AirQualitySensor sensor(A0);
DHT dht(DHTPIN, DHTTYPE);

rgb_lcd lcd;

//_______________________________________________________________

void setup() {

  Serial.begin(9600);

  //LED AND BUZZER CONTROL
  pinMode(7, OUTPUT); //LED pin
  pinMode(5, OUTPUT); //Buzzer

  //LCD SET UP
  lcd.begin(16, 2); //Specifys the amount of colums and rows in LCD
  lcd.setRGB(255,30,30);

  //CUSTOM CHAR SET UP
  lcd.createChar(0, thermometer); // Store the custom character in CGRAM (see CustomChar.h)
  lcd.createChar(1, waterDroplet);
  lcd.createChar(2, degreesSymbol);
  lcd.createChar(3, exclamation);
  lcd.createChar(4, doubleCheck);
  lcd.createChar(5, topCheckBottomX);
  lcd.createChar(6, topXBottomCheck);

  //DHT SET UP
  dht.begin();

  //WATCHDOG TIMER SET UP
  wdt_enable(WDTO_2S); // if the microcontroller does not respond for 2 seconds the microcontroller resets

  //LOADING FOR AIR QUALITY SENSOR TO HEAT UP
  
    while (millis() < 20000) { // loading animation for the first 15 seconds the arduino is on
      wdt_reset(); //alerts the watchdog timer "no errors here, keep running!"
      lcd.setCursor(0, 1);
      lcd.write("Loading.                       ");
      delay(500);
      lcd.setCursor(0, 1);
      lcd.write("Loading..                       ");
      delay(500);
      lcd.setCursor(0, 1);
      lcd.write("Loading...                       ");
      delay(500);
      lcd.clear();
    } 
}

//________________________________________________________________

void loop() {

  unsigned long currentMillis = millis();// keeps track of the current time in milli seconds

  wdt_reset(); //alerts the watchdog timer "no errors here, keep running!"

  int quality = sensor.slope();
  int h = dht.readHumidity();
  int t = dht.readTemperature(); //Measuring air qual, temp and humidity

  int i = rotary.rotate(); // checking if encoder turns cw or ccw
  int j = rotary.push(); // checking if encoder button is pushed

  if (sysState == 0){ // when sysState is 0, the system is in its main state, checking and displaying all data from sensors

  // ERROR CHECKING
    errorCheck(); //calling custom error checking function

  // AIR QUALITY DISPLAY------------------------------------------------------------------
    lcd.setCursor(0,0);
    if (currentMillis - previousMillisLCD >= 1000) { //checks and displays airquality every half second to prevent flickering between messages
      previousMillisLCD = currentMillis;
      if (quality == AirQualitySensor::FORCE_SIGNAL) {
        lcd.print("Dangerous levels!            "); //Spaces clear the screen of any previous characters, LCDs do not clear automatically when a new mssage is shorter than the old one
      } else if (quality == AirQualitySensor::HIGH_POLLUTION) {
          lcd.print("High pollution!                 ");
      } else if (quality == AirQualitySensor::LOW_POLLUTION) {
          lcd.print("Low pollution!                  ");
      } else if (quality == AirQualitySensor::FRESH_AIR) {
          lcd.print("Fresh air                     ");
      }
    }

  // TEMP & HUMIDITY DISPLAY---------------------------------------------------------------------
    tempHumiDisplay(t,h); // calls tempHumiDisplay function and passes variables t and h
  // TEMP & HUMIDITY ALERTS----------------------------------------------------------------------
      
    if (h > humiUpperLimit) { //humidity alerts
      lcd.setCursor(12,1);
      lcd.write(byte(6));
      }

      else if (h < humiLowerLimit) {
        lcd.setCursor(12,1);
        lcd.write(byte(5));
      }

      else {
        lcd.setCursor(12,1);
        lcd.write(byte(4));
      }

      if (t > tempUpperLimit) { // temp alert
        lcd.setCursor(5,1);
        lcd.write(byte(6));
      }

      else if (t < tempLowerLimit) {
        lcd.setCursor(5,1);
        lcd.write(byte(5));
      }

      else {
        lcd.setCursor(5,1);
        lcd.write(byte(4));
      }
  // AIR QUALITY ALERTS------------------------------------------------------------------------------

    if (quality == AirQualitySensor::FORCE_SIGNAL) {
      digitalWrite(7,HIGH);
      digitalWrite(5,HIGH);

    } else if (quality == AirQualitySensor::HIGH_POLLUTION) {
        blinkLEDBuzzer(200,250); //calls blinkLEDBuzzer function and passes intergers 500 and 200, this makes the LED blink twice as fast

    } else if (quality == AirQualitySensor::LOW_POLLUTION) {
        blinkLEDBuzzer(200,500);  //calls blinkLEDBuzzer function and passes intergers 200 and 500

    } else if (quality == AirQualitySensor::FRESH_AIR) {
        digitalWrite(7,LOW);
        digitalWrite(5,LOW);
    }

  // STATE SWITCHING
    if (j == 1) { 
      sysState++; 
      lcd.clear();
      Serial.print("button pressed");
      }  // when the button of the encoder is pushed, the state is changed and menu will appear on the LCD
  
    colorSelect(); } //calling custom color select function
  

  //MENU----------------------------------------------------------------------------------------------
  //when the user opens the menu they are able to edit and update the alert parameters 

  else if (sysState == 1){ //sysState 1 opens user menu

    if (menuState == 0){ //setting temp upper limit

      lcd.setCursor(0,0);
      lcd.write(byte(0));
      lcd.setCursor(2,0);
      lcd.print("Upper Limit:");

      if (i == 1) tempUpperLimit++;
      if (i == 2) tempUpperLimit--;

      lcd.setCursor(0,1);

      if ( (tempUpperLimit < 10) && (tempUpperLimit >= 0)) { 
        lcd.print(tempUpperLimit);
        lcd.print(" "); // getting rid of the 0 left behind
      } else {
        lcd.print(tempUpperLimit);
      }

      if (j == 1) { //when button of encoder is pressed again the state of the menue changes
        menuState++;
        lcd.clear();
      }
    }

    else if(menuState == 1){ //Temp lower limit
        
      lcd.setCursor(0,0);
      lcd.write(byte(0));
      lcd.setCursor(2,0);
      lcd.print("Lower Limit:");

      if (i == 1) tempLowerLimit++;
      if (i == 2) tempLowerLimit--;

      lcd.setCursor(0,1);

      if ( (tempLowerLimit < 10) && (tempLowerLimit >= 0)) { 
        lcd.print(tempLowerLimit);
        lcd.print(" "); // getting rid of the 0 left behind
      } else {
        lcd.print(tempLowerLimit);
      }

      if (j == 1) {
        menuState++;
        lcd.clear();
      } 
    }

    else if (menuState == 2){ // humidity upper limit

      lcd.setCursor(0,0);
      lcd.write(byte(1));
      lcd.setCursor(2,0);
      lcd.print("Upper Limit:");

      if (i == 1) humiUpperLimit++;
      if (i == 2) humiUpperLimit--;

      lcd.setCursor(0,1);              

      if ( (humiUpperLimit < 10) && (humiUpperLimit >= 0)) { 
        lcd.print(humiUpperLimit);
        lcd.print(" "); // getting rid of the 0 left behind
      } else {
        lcd.print(humiUpperLimit);
      }

      if (j == 1) {
        menuState++;
        lcd.clear();
      }
    }

    else if (menuState == 3){ // humidity lower limit

      lcd.setCursor(0,0);
      lcd.write(byte(1));
      lcd.setCursor(2,0);
      lcd.print("Lower Limit:");

      if (i == 1) humiLowerLimit++;
      if (i == 2) humiLowerLimit--;

      lcd.setCursor(0,1); 

      if ( (humiLowerLimit < 10) && (humiLowerLimit >= 0)) { 
        lcd.print(humiLowerLimit);
        lcd.print(" "); // getting rid of the 0 left behind
      } else {
        lcd.print(humiLowerLimit);
      }

      if (j == 1) {
        menuState = 0;
        sysState = 0;
        lcd.clear();
      }
          
    }
    wdt_reset();//at the end of state 1 loop
  }
}

//_______________________________________________________________


void errorCheck() { //defining error checking function

  int quality = sensor.slope();
  int h = dht.readHumidity();
  int t = dht.readTemperature(); //Measuring air qual, temp and humidity

  if (isnan(h) || isnan(t)) { // If DHT is not detecting any temp or humidity...
        lcd.setCursor(0, 1);
        lcd.write("Error DHT                        ");
        return;
      }

      if (isnan(quality)) { // If air quality sensor is not detecting quality...
        lcd.setCursor(0,0);
        lcd.write("Error Air Qual                   ");
        return;
      }

}

void colorSelect() { // defining custom color selsct function

  int i = rotary.rotate(); // checking if encoder turns cw or ccw

  if ( i == 1 ) {  //if rotary turns clockwise
    col_cnt++;
    if (col_cnt > 9){
      col_cnt=0;
      }
    lcd.setRGB(rgb[col_cnt][0],rgb[col_cnt][1],rgb[col_cnt][2]);
  }

  if ( i == 2 ){   //if rotary turns anticlockwise
    col_cnt--;
    if (col_cnt==0){
      col_cnt=9;
      }
    lcd.setRGB(rgb[col_cnt][0],rgb[col_cnt][1],rgb[col_cnt][2]); //this looks up the colour array and displays the correct colour 
  }
}

void tempHumiDisplay(int temp, int humi){ // displays tempurature and hunidity readings on LCD display
  lcd.setCursor(0,1); //Displaying Temp
  lcd.write(byte(0));
  lcd.setCursor(1,1);
  lcd.print(temp);
  lcd.setCursor(3,1);
  lcd.write(byte(2));
  lcd.print("C ");

  lcd.setCursor(9,1); //Displaying Humidity
  lcd.write(byte(1));
  lcd.setCursor(10,1);
  lcd.print(humi);
  lcd.setCursor(12,1);
  lcd.print("%");
}

void blinkLEDBuzzer(int a, int b) { // this function uses millis() to blink the LED and Buzzer when the airquality gets too low                              
  unsigned long currentMillis = millis(); // since millis() is being used instead of delay() this function does not disturb the rest of the program
  
  if (LEDBuzzerState == HIGH) {
    if (currentMillis - previousMillisAlert >= a) { //turns red LED and buzzer on for "a" milliseconds
      previousMillisAlert = currentMillis;          //when the state of alert is high this checks if it is time to turn the alert state low
      LEDBuzzerState = LOW;
      digitalWrite(7, LEDBuzzerState);
      digitalWrite(5, LEDBuzzerState);
    }
  } else {                                          //if the alert state is low it checks if it is time to turn the alert state  high
    if (currentMillis - previousMillisAlert >= b) { //turns green LED on and buzzer off for "b" milliseconds
      previousMillisAlert = currentMillis;
      LEDBuzzerState = HIGH;
      digitalWrite(7, LEDBuzzerState);
      digitalWrite(5, LEDBuzzerState);
    }
  }
}
