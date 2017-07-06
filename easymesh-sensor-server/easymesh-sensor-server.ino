
//file to server side to connect the sensor data and send them to the Client side 
#include <easyMesh.h>
#include <easyMeshSync.h>
#include <SimpleList.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define   MESH_PREFIX     "WKI"
#define   MESH_PASSWORD   "WKI1234"
#define   MESH_PORT       5555

Adafruit_BME280 bme; // I2C

const char* ssid = "AndroidAP";
const char* password = "sadbhavna12";
float h, t, p, pin, dp;
char temperatureFString[6];
char dpString[6];
char humidityString[6];
char pressureString[7];
char pressureInchString[6];
uint32_t ID;
String msg,msg1,msg2,msg3,msg4,msgFinal;
String xyz="hello";
// Web Server on port 80
WiFiServer server(80);

easyMesh mesh;

static void (*receivedCallback)( uint32_t ID, String &msg);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);
  Wire.begin(D3, D4);
  Wire.setClock(100000);
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT);  
  ID=system_get_chip_id();  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  mesh.setReceiveCallback(receivedCallback);  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());
  Serial.println(F("BME280 test"));

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}
void getWeather() {
  
    h = bme.readHumidity();
    t = bme.readTemperature();
    t = t*1.8+32.0;
    dp = t-0.36*(100.0-h);
    p = bme.readPressure()/100.0F;
    pin = 0.02953*p;
    dtostrf(t, 5, 1, temperatureFString);
    String msg(temperatureFString);
    dtostrf(h, 5, 1, humidityString);
    String msg1(humidityString);
    dtostrf(p, 6, 1, pressureString);
    String msg2(pressureString);
    dtostrf(pin, 5, 2, pressureInchString);
    String msg3(pressureInchString);
    dtostrf(dp, 5, 1, dpString);
    String msg4(dpString);
    msgFinal=msg+msg1+msg2+msg3+msg4;
    delay(100); 
}
void loop() {

  // put your main code here, to run repeatedly:
  getWeather();
  mesh.update();
  
 // void receivedCallback( uint32_t ID, String &msg ) ;
  void newConnectionCallback( bool adopt );            
  mesh.sendBroadcast(msgFinal);
  if (mesh.sendBroadcast(msgFinal)){
    Serial.println("success");
    }
   
  delay(100);
  mesh.sendBroadcast(xyz);
  Serial.println(msgFinal);
  
}

