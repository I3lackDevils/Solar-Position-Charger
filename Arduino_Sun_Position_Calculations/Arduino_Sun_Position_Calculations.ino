#include <Braccio.h>
#include <Servo.h>
#include <LCD.h>
#include <LCD_C0220BiZ.h>
#include <ST7036.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include "splitstring.h"

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;



ST7036 lcd = ST7036 (2, 16, 0x78) ;
Adafruit_INA219 ina219(0x41);
Adafruit_INA219 ina219_2(0x44);


String txtMsg = ""; 
String StringSplits[5];

//////////////////////////////////////////////////  
//PUT YOUR LATITUDE, LONGITUDE, AND TIME ZONE HERE
  float latitude = 51.16600;
  float longitude = 4.98917;
  float timezone = 2;
//////////////////////////////////////////////////  
  
//If you live in the southern hemisphere, it would probably be easier
//for you if you make north as the direction where the azimuth equals
//0 degrees. To do so, switch the 0 below with 180.  
  float northOrSouth = 180;

/////////////////////////////////////////////////////////// 
//MISC. VARIABLES
///////////////////////////////////////////////////////////  
  float pi = 3.14159265;
  float altitude;
  float azimuth;
  float delta;
  float h;
/////////////////////////////////////////////////////////// 
//END MISC. VARIABLES
///////////////////////////////////////////////////////////
  String serial = "";
  
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  float shuntvoltage2 = 0;
  float busvoltage2 = 0;
  float current_mA2 = 0;
  float loadvoltage2 = 0;
  float power_mW2 = 0;

  float tmonth = 1;
  float tday = 13;
  float thour = 9;
  float tminute = 40;
  float month2 = 1;
  float day = 13;
  float hour2 = 9;
  float minute2 = 40;
  int scherm = 1;
  int simulatie = 0;
  String TEST;
  String TEST2;
  
void setup(){
  pinMode(2, INPUT);           // set pin to input
  digitalWrite(2, HIGH);       // turn on pullup resistors
  Serial.begin(9600);
  latitude = latitude * pi/180;
  Braccio.begin();
  ina219.begin();
  ina219_2.begin();
  lcd.init();
}

void loop(){
  while (Serial.available() > 0) {
    char inChar = Serial.read();
    txtMsg += inChar;
  }

  if(txtMsg.length()> 0){
      for (int i = 0; i < 6; i++)  //this for loop runs 4 times, bus just the number of parts which are possible will be saved in StringSplits[i]
      {
      StringSplits[i] = GetStringPartAtSpecificIndex(txtMsg, ',', i); //give this function your string to split, your char to split and the index of the StringPart you want to get
      }
      tmonth = StringSplits[1].toFloat();
      tday = StringSplits[0].toFloat();
      thour = StringSplits[3].toFloat();//Use 24hr clock (ex: 1:00pm = 13:00) and don't use daylight saving time.
      tminute = StringSplits[4].toFloat();
      
      txtMsg = "";
    }
    month2 = tmonth;
    day = tday;
    hour2 = thour;
    minute2 = tminute;
    
  //SET TIME AND DATE HERE//////////////
  if(digitalRead(2) == 0){
    if(simulatie == 1){
      simulatie = 0;
    }else{
      simulatie = 1;
    }
  }
  if(simulatie == 1){
    month2 = 1;
    day = 13;
    hour2 = 9+(analogRead(A1)/113); 
    minute2 = 40;
  }
  //END SET TIME AND DATE /////////////
  TEST2 = String(month2,0) + "-" + String(day,0) + " " + String(hour2,0) + ":" + String(minute2,0);

  //START OF THE CODE THAT CALCULATES THE POSITION OF THE SUN
  float n = daynum(month2) + day;//NUMBER OF DAYS SINCE THE START OF THE YEAR. 
  delta = .409279 * sin(2 * pi * ((284 + n)/365.25));//SUN'S DECLINATION.
  day = dayToArrayNum(day);//TAKES THE CURRENT DAY OF THE MONTH AND CHANGES IT TO A LOOK UP VALUE ON THE HOUR ANGLE TABLE.
  h = (FindH(day,month2)) + longitude + (timezone * -1 * 15);//FINDS THE NOON HOUR ANGLE ON THE TABLE AND MODIFIES IT FOR THE USER'S OWN LOCATION AND TIME ZONE.
  h = ((((hour2 + minute2/60) - 12) * 15) + h)*pi/180;//FURTHER MODIFIES THE NOON HOUR ANGLE OF THE CURRENT DAY AND TURNS IT INTO THE HOUR ANGLE FOR THE CURRENT HOUR AND MINUTE.
  altitude = (asin(sin(latitude) * sin(delta) + cos(latitude) * cos(delta) * cos(h)))*180/pi;//FINDS THE SUN'S ALTITUDE.
  azimuth = ((atan2((sin(h)),((cos(h) * sin(latitude)) - tan(delta) * cos(latitude)))) + (northOrSouth*pi/180)) *180/pi;//FINDS THE SUN'S AZIMUTH.
  //END OF THE CODE THAT CALCULATES THE POSITION OF THE SUN23112313453455654312654311511511231123134534556543126543115115112311231345345565431265431151151123112313453455654312654311511511231123134534556543126543115115112311231345345565431265431151151123112313453455654312654311511511231123134534556543126543115115112311231345345565431265431151151123112313453455654312654311511511231123134534556543126543115115112311231345345565431265431151151123112313453455654312654311511511231123134534556543126543115115112311231345345565431265431151151123112313453455654312654311511511231123134534556543126543115115112311231345345565431265431151151123112313453455654312654311511511231123134534556543126543115115112311231345345565431265431151151123112313453455654312654311511511231123134534556543126543115115112311231345345565431265431151151123112313453455654312654311511511231123134534556543126543115115112311231345345565431265431151151123112313453455654312654311511511231123134534556543126543115115112311231345345565431265431151151123112313453455654312654311511511231123134534556543126543115115112311231345345565431265431151151123112313453455654312654311511511231123134534556543126543115115112311231345345565431265431151151123112313453455654312654311511511231123134534556543126543115115112311231345345565431265431151151123112313453455654312654311511511231123134534556


  Braccio.ServoMovement(30, azimuth-70, altitude+30, 90, 0, 90, 10);
  //Braccio.ServoMovement(30, azimuth-70, 180, 0, 0, 90, 10);
  TEST = String(azimuth,1) + " " + String(altitude,1);


  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  shuntvoltage2 = ina219_2.getShuntVoltage_mV();
  busvoltage2 = ina219_2.getBusVoltage_V();
  current_mA2 = ina219_2.getCurrent_mA();
  power_mW2 = ina219_2.getPower_mW();
  loadvoltage2 = busvoltage2 + (shuntvoltage2 / 1000);
  if(analogRead(A0) < 341){
    scherm = 1;
  }
  if(analogRead(A0) > 341 && analogRead(A0) < 682){
    scherm = 2;
  }
  if(analogRead(A0) > 682){
    scherm = 3;
  }
  if(scherm == 1){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Arduino:");
    lcd.setCursor(2, 0);
    lcd.print("U=" + String(loadvoltage,2) + "V " + "I=" + String(current_mA,1)+ "mA");
  }
  if(scherm == 2){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Zonnepaneel");
    lcd.setCursor(2, 0);
    lcd.print("U=" + String(loadvoltage2,2) + "V " + "I=" + String(current_mA2,1)+ "mA");
  }
  if(scherm == 3){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(TEST2);
    lcd.setCursor(2, 0);
    lcd.print(TEST);
  }
  serial = String(loadvoltage) + "," + String(current_mA) + "," + String(loadvoltage2) + "," + String(current_mA2) + ",";
  Serial.println(serial); //Write the serial data
  Serial.print('\r');
  delay(500);

}//End Void Loop
