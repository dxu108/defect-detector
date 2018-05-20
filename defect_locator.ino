#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;
const int HALL_PIN = 5; // analog

Adafruit_MMA8451 mma = Adafruit_MMA8451();

void setup()
{
  Serial.begin(9600);

  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect))
  {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  
  Serial.println("card initialized.");

  Serial.println("Adafruit MMA8451 test!");
  
  if (!mma.begin())
  {
    Serial.println("Couldnt start");
    while (1);
  }
  
  Serial.println("MMA8451 found!");
  
  mma.setRange(MMA8451_RANGE_8_G);
}

void loop()
{
  // make a string for assembling the data to log:
  String dataString = "";

  int raw_hall = analogRead(HALL_PIN);
  Serial.println(String(raw_hall));
  dataString += String(raw_hall);
  dataString += ",";

  mma.read();
  sensors_event_t event; 
  mma.getEvent(&event);

  dataString += "X: ";
  dataString += event.acceleration.x;
  dataString += ",";

  dataString += "Y: ";
  dataString += event.acceleration.y;
  dataString += ",";

  dataString += "Z: ";
  dataString += event.acceleration.z;

  File dataFile = SD.open("datalog_4_23_2018.txt", FILE_WRITE);

  if (dataFile)
  {
    dataFile.println(dataString);
    dataFile.close();
  }
  else
  {
    Serial.println("error opening datalog.txt");
  }
}
