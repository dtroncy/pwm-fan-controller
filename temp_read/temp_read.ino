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
 
  // Vérifie sir les capteurs sont connectés | check and report if sensors are conneted 
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
 
  // set the resolution to 9 bit per device
  sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
 
  // On vérifie que le capteur st correctement configuré | Check that ensor is correctly configured
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC); 
  Serial.println();
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
  // put your main code here, to run repeatedly:
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");
 
  // print the device information
  printTemperature("Inside : ", insideThermometer);
  
  delay(5000);
}
