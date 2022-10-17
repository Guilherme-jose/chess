#include <M5StickC.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "chess.h"

float accX = 0;
float accY = 0;
float accZ = 0;
float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;
float gSens = 150;
float aSens = 1.0;

char* direction = "NONE";

char *ssid = "LAO";
char *password = "lao22033";
const char *endpoint = "test.mosquitto.org";
const int port = 1883;
char *deviceID = "M5Stack";
char *pubTopic = "m5Direction";
char *subTopic = "m5Direction";
 
WiFiClient httpsClient;
PubSubClient mqttClient(httpsClient);

void setup_serial(){
  Serial.begin(9600);
  while (!Serial) continue;
}


void setup_wifi(){
  Serial.println("Connecting to ");
  Serial.print(ssid);
  WiFi.disconnect( true, true ); 
  delay(500);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    M5.Lcd.print(".");
  }
  
  Serial.println("\nWiFi Connected.");
  M5.Lcd.setCursor(10, 40);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("WiFi Connected.");
  M5.Lcd.print("IP address: ");
  M5.Lcd.println(WiFi.localIP());
  
}

void connectMQTT(){
 
  while (!mqttClient.connected()) {
    if (mqttClient.connect(deviceID)) {
    
      Serial.println("Connected.");
      int qos = 0;
      mqttClient.subscribe(subTopic, qos);
      Serial.println("Subscribed.");
    } else {
     
      Serial.print("Failed. Error state=");
      Serial.print(mqttClient.state());
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqttLoop(){
  
 
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  //mqttClient.loop();
}

void setup(void) {
    M5.begin();
    M5.IMU.Init();

    M5.Lcd.setRotation(1);

    M5.Lcd.fillScreen(TFT_BLACK);

    M5.Lcd.setTextColor(TFT_GREEN);  // Adding a background colour erases
                                     // previous text automatically

    M5.Lcd.setCursor(10, 30);
    M5.Lcd.setTextSize(3);
    m5.Lcd.print("start");
    delay(1000);
    setup_wifi();
    setup_serial();
    mqttClient.setServer(endpoint, port);
    connectMQTT();
    chessSetup();
    delay(1000);
}

void loop() {
    while (1) {
        M5.IMU.getAccelData(&accX, &accY, &accZ);
        M5.IMU.getGyroData(&gyroZ, &gyroY, &gyroX);
        gyroX *= -1;
        gyroZ *= -1;

        accX -= 0.97;
        if (accX > aSens  && gyroX > gSens) {
          direction = "UP";
          break;
        } else if (accZ > aSens || gyroZ > gSens) {
          direction = "RIGHT";
          break;
        } else if (accX < -1 * aSens && gyroX < -gSens) {
          direction = "DOWN";
          break;
        } else if (accZ < -1 * aSens || gyroZ < -gSens) {
          direction = "LEFT";
          break;
        } else if (direction != "NONE"){
          direction = "NONE";
          break;
        }
        
    }
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextColor(TFT_GREEN);
    
    M5.Lcd.drawNumber(accX*10, 0, 0);
    M5.Lcd.drawNumber(accY*10, 50, 0);
    M5.Lcd.drawNumber(accZ*10, 100, 0);
    if(direction == "UP")
      M5.Lcd.drawChar('^', 0, 50);
    if(direction == "RIGHT")
      M5.Lcd.drawChar('>', 0, 50);
    if(direction == "DOWN")
      M5.Lcd.drawChar('V', 0, 50);
    if(direction == "LEFT")
      M5.Lcd.drawChar('<', 0, 50);
    if(direction == "NONE")
      M5.Lcd.drawChar('o', 0, 50);


    mqttLoop();
    //sprintf("direction", direction);
    mqttClient.publish(pubTopic, direction);
    Serial.println("Published.");    

    /////////////////////////////////////////////////
    if(direction != "NONE") {
      moveSelected(direction);
    }
    chessLoop();
    /////////////////////////////////////////////////
    
    if(direction != 0)
      delay(500);
    
}
