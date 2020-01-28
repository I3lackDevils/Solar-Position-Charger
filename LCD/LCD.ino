#include <LCD.h>
#include <LCD_C0220BiZ.h>
#include <ST7036.h>
//set the LCD address to 0x78, use i2c scanner to find out your address
ST7036 lcd = ST7036 (2, 16, 0x78) ;

#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219(0x41);
Adafruit_INA219 ina219_2(0x44);
String serial = "";

void setup(void) 
{
  Serial.begin(9600);
  while (!Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
      delay(1);
  }

  uint32_t currentFrequency;
    
  //Serial.println("Hello!");
  
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  ina219.begin();
  ina219_2.begin();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  //Serial.println("Measuring voltage and current with INA219 ...");
  lcd.init();
}

void loop(void) 
{
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
  
  //Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  //Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  //Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  //Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  //Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  //Serial.println("");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Arduino:");
  //lcd.setCursor(2, 0);
  //lcd.print("U=" + String(loadvoltage,2) + "V " + "I=" + String(current_mA,1)+ "mA");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Zonnepaneel");
  //lcd.setCursor(2, 0);
  //lcd.print("U=" + String(loadvoltage2,2) + "V " + "I=" + String(current_mA2,1)+ "mA");
  delay(1000);
  serial = String(loadvoltage) + "," + String(current_mA) + "," + String(loadvoltage2) + "," + String(current_mA2) + ",";
  Serial.println(serial); //Write the serial data
  Serial.println(current_mA,1);
  Serial.print('\r');

  
}
