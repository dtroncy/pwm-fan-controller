/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
  Based on the Dallas Temperature Library example
*********/

#include <OneWire.h>
#include <DallasTemperature.h> //remplacer les () par <>

// Data wire is plugged into port 14 on the Arduino or ESP32
#define ONE_WIRE_BUS 14
#define TEMPERATURE_PRECISION 10
 
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
 
// Tableaux contenant l'adresse de chaque sonde OneWire | arrays to hold device addresses
DeviceAddress insideThermometer = { 0x28,  0xFF,  0x9B,  0xB5,  0xA0,  0x16,  0x3,  0xCD };

const int FANPin = 16;  /* GPIO16 */

int dutyCycle;
/* Setting PWM Properties */
const int PWMFreq = 25000; /* 25 KHz */
const int PWMChannel = 0;
const int PWMResolution = 10;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);

 
void setup() {
  Serial.begin(115200);
  
  // Start up the library
  sensors.begin();
 
  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
 
  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
 
  // Vérifie si les capteurs sont connectés | check and report if sensors are conneted 
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
 
  // set the resolution to 9 bit per device
  sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
 
  // On vérifie que le capteur st correctement configuré | Check that ensor is correctly configured
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC); 
  Serial.println();

  Serial.print("Configuration du pilotage PWM...");
  ledcSetup(PWMChannel, PWMFreq, PWMResolution);
  /* Attach the FAN PWM Channel to the GPIO Pin */
  ledcAttachPin(FANPin, PWMChannel);
  
}
 
void printTemperature(String label, DeviceAddress deviceAddress){
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print(label);
  if (tempC == -127.00) {
    Serial.print("Error getting temperature");
  } else {
    Serial.print(" Temp C: ");
    Serial.print(tempC);
    Serial.print(" Temp F: ");
    Serial.println(DallasTemperature::toFahrenheit(tempC));
  }  
}
 
void loop() {

  sensors.requestTemperatures();
 
  // print the device information
  printTemperature("Inside : ", insideThermometer);

  if (sensors.getTempC(insideThermometer)>22){
    ledcWrite(PWMChannel, MAX_DUTY_CYCLE);
    Serial.println("Réglage sur MAX");
    
  }else{
    //ledcWrite(PWMChannel, 200);

    pinMode(27, OUTPUT); //Il faut déclarer le pin en sortie
    digitalWrite(2, LOW);

    Serial.println("Réglage sur MIN");
  }  
  
  delay(5000);
}
