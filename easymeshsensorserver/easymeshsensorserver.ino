
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
String msg="hello";

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
    dtostrf(h, 5, 1, humidityString);
    dtostrf(p, 6, 1, pressureString);
    dtostrf(pin, 5, 2, pressureInchString);
    dtostrf(dp, 5, 1, dpString);
    delay(100); 
}   
void loop() {

  // put your main code here, to run repeatedly:
  mesh.update();
  Serial.println(ID);
  void receivedCallback( uint32_t ID, String &msg ) ;
  void newConnectionCallback( bool adopt );             //noerror
  mesh.sendBroadcast(msg);                              ////error is here
  // Listenning for new clients
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (c == '\n' && blank_line) {
            getWeather();
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            // your actual web page that displays temperature
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head><META HTTP-EQUIV=\"refresh\" CONTENT=\"15\"></head>");
            client.println("<body><h1>ESP8266 Weather Web Server</h1>");
            client.println("<table border=\"2\" width=\"456\" cellpadding=\"10\"><tbody><tr><td>");
            client.println("<h3>Temperature = ");
            client.println(temperatureFString);
            client.println("&deg;F</h3><h3>Humidity = ");
            client.println(humidityString);
            client.println("%</h3><h3>Approx. Dew Point = ");
            client.println(dpString);
            client.println("&deg;F</h3><h3>Pressure = ");
            client.println(pressureString);
            client.println("hPa (");
            client.println(pressureInchString);
            client.println("Inch)</h3></td></tr></tbody></table></body></html>");  
            break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }
    // closing the client connection
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }
}
