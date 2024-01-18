#include <WiFi.h>
#include "ThingSpeak.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

const char* ssid = "****";   // your network SSID (name) 
const char* password = "****";   // your network password
int EMGPin = 34;
int EMGVal = 0;


WiFiClient  client;

unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "OTVWVUTPYOUY15C6";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 10;

// Variable to hold temperature readings
float temperatureC;
//uncomment if you want to get temperature in Fahrenheit
//float temperatureF;

// Create a sensor object
Adafruit_MPU6050 mpu;; //

void initMPU(){
  if (!mpu.begin()) {
    Serial.println("Could not find a valid IMU sensor, check wiring!");
    while (1);
  }
  Serial.println("MPU6050 Found!");

   mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

void setup() {
  Serial.begin(115200);  //Initialize serial
  initMPU();
  
  WiFi.mode(WIFI_STA);   
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
    /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  if ((millis() - lastTime) > timerDelay) {
    
    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
    }

    // Get a new temperature reading
    temperatureC = temp.temperature;
    Serial.print("Acceleration X: ");
    Serial.print(a.acceleration.x);
    Serial.print(", Y: ");
    Serial.print(a.acceleration.y);
    Serial.print(", Z: ");
    Serial.print(a.acceleration.z);
    Serial.println(" m/s^2");
    Serial.print("Rotation X: ");
    Serial.print(g.gyro.x);
    Serial.print(", Y: ");
    Serial.print(g.gyro.y);
    Serial.print(", Z: ");
    Serial.print(g.gyro.z);
    Serial.println(" rad/s");
    Serial.print("Temperature (ºC): ");
    Serial.println(temperatureC);
    EMGVal = analogRead(EMGPin);
    Serial.println(EMGVal);
    
    
    //uncomment if you want to get temperature in Fahrenheit
    /*temperatureF = 1.8 * bme.readTemperature() + 32;
    Serial.print("Temperature (ºC): ");
    Serial.println(temperatureF);*/
    
     // set the fields with the values
    ThingSpeak.setField(1, a.acceleration.x);
    //ThingSpeak.setField(1, temperatureF);
    ThingSpeak.setField(2, a.acceleration.y);
    ThingSpeak.setField(3, a.acceleration.z);
    ThingSpeak.setField(4, g.gyro.x);
    ThingSpeak.setField(5, g.gyro.y);
    ThingSpeak.setField(6, g.gyro.z)
    ThingSpeak.setField(7, EMGVal);    
    
    // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
    // pieces of information in a channel.  Here, we write to field 1.
    int x = ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);
    //uncomment if you want to get temperature in Fahrenheit
    //int x = ThingSpeak.writeField(myChannelNumber, 1, temperatureF, myWriteAPIKey);

    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lastTime = millis();
  }
}
