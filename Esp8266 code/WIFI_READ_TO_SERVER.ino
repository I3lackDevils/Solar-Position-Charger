#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include "splitstring.h"

#ifndef STASSID
#define STASSID "WiFi-2.4-620C"
#define STAPSK  "Ingipa01"
#endif

//Web/Server address to read/write from for POST protocol
const char *host = "192.168.1.37";

//Voor data van arduino naar web
const char* ssid = STASSID;
const char* password = STAPSK;
char serial[5];
//String serial = "serial";
String data = "<!DOCTYPE html>\n<html>\n<style>\n@font-face {\n\t\t\t    font-family: 'dos';\n\t\t\t     format('woff'),\n\t\t\t         url('perfect_dos_vga_437_win-webfont.ttf') format('truetype');\n\t\t\t    font-weight: normal;\n\t\t\t    font-style: normal;\n\t\t\t}\n\t\t\tbody {\n\t\t\t\tmargin: 100px auto;\n\t\t\t\twidth: 320px;\n\t\t\t\tfont-family: 'dos',monospace;\n\t\t\t\tbackground: rgb(0,10,0);\n\t\t\t\tcolor: rgb(50,255,50);\n\t\t\t\t}\n         h1 {font-weight: normal;}\n\t\t\tfieldset {\n\t\t\t\tmargin: 15px 0;\n\t\t\t\topacity: 0.75;\n\t\t\t\tborder: 3px double rgb(50,255,50);\n\t\t\t\tpadding: 2px;\n\t\t\t}\n\t\t\tlegend {\n\t\t\t\tbackground: rgb(50,255,50);\n\t\t\t\ttext-align: center;\n\t\t\t\ttext-transform: uppercase;\n\t\t\t\tcolor: rgb(0,10,0);\n\t\t\t}\n\t\t\t.warn {\n\t\t\t\tcolor: rgb(255,255,50);\n            background: rgb(0,10,0);\n\t\t\t}\n\t\t\t.danger {\n\t\t\t\tcolor: rgb(255,50,50);\n            background: rgb(0,10,0);\n\t\t\t}\n\t\t\ttable {width: 100%;}\n\t\t\ttable, tr, td {\n\t\t\t\tpadding: 0;\n\t\t\t\tmargin: 0;\n\t\t\t\tborder: none;\n\t\t\t}\n\t\t\t.value {text-align: right;}\n</style>\n<body>\n<h1>STATUS MONITOR</h1>\n\t\t<fieldset>\n\t\t\t<legend>Arduino</legend>\n\t\t\t<table>\n\t\t\t\t<tr>\n\t\t\t\t\t<td>Voltage</td>\n\t\t\t\t\t<td class='value'><span>4.756V</span></td>\n\t\t\t\t</tr>\n\t\t\t\t<tr>\n\t\t\t\t\t<td>Current</td>\n\t\t\t\t\t<td class='value'><span>80mA</span></td>\n\t\t\t\t</tr>\n\n\t\t\t</table>\n\t\t</fieldset>\n\t\t<fieldset>\n\t\t\t<legend>Solar Power</legend>\n\t\t\t<table>\n\t\t\t\t<tr>\n\t\t\t\t\t<td>Voltage</td>\n\t\t\t\t\t<td class='value'><span>4.756V</span></td>\n\t\t\t\t</tr>\n\t\t\t\t<tr>\n\t\t\t\t\t<td>Current</td>\n\t\t\t\t\t<td class='value'><span>80mA</span></td>\n\t\t\t\t</tr>\n\t\t\t</table>\n\t\t</fieldset>\n</body>\n</html>\n";
String aray;
String txtMsg = ""; 
String StringSplits[3];

//Voor webserver tijd naar arduino
const long utcOffsetInSeconds = 3600;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int dag;
int maand;
int jaar;
int uur;
int minuut;
int seconde;
int epoch;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

ESP8266WebServer server(80);

const int led = 2;

void handleRoot() {
  server.send(200, "text/html", data);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  timeClient.begin();

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
  while (Serial.available() > 0) {
    char inChar = Serial.read();
    txtMsg += inChar;
  }
  if(txtMsg.length()> 0){
    for (int i = 0; i < 4; i++)  //this for loop runs 4 times, bus just the number of parts which are possible will be saved in StringSplits[i]
    {
    StringSplits[i] = GetStringPartAtSpecificIndex(txtMsg, ',', i); //give this function your string to split, your char to split and the index of the StringPart you want to get
    }
    
    data = "<!DOCTYPE html>\n<html>\n<style>\n@font-face {\n\t\t\t    font-family: 'dos';\n\t\t\t     format('woff'),\n\t\t\t         url('perfect_dos_vga_437_win-webfont.ttf') format('truetype');\n\t\t\t    font-weight: normal;\n\t\t\t    font-style: normal;\n\t\t\t}\n\t\t\tbody {\n\t\t\t\tmargin: 100px auto;\n\t\t\t\twidth: 320px;\n\t\t\t\tfont-family: 'dos',monospace;\n\t\t\t\tbackground: rgb(0,10,0);\n\t\t\t\tcolor: rgb(50,255,50);\n\t\t\t\t}\n         h1 {font-weight: normal;}\n\t\t\tfieldset {\n\t\t\t\tmargin: 15px 0;\n\t\t\t\topacity: 0.75;\n\t\t\t\tborder: 3px double rgb(50,255,50);\n\t\t\t\tpadding: 2px;\n\t\t\t}\n\t\t\tlegend {\n\t\t\t\tbackground: rgb(50,255,50);\n\t\t\t\ttext-align: center;\n\t\t\t\ttext-transform: uppercase;\n\t\t\t\tcolor: rgb(0,10,0);\n\t\t\t}\n\t\t\t.warn {\n\t\t\t\tcolor: rgb(255,255,50);\n            background: rgb(0,10,0);\n\t\t\t}\n\t\t\t.danger {\n\t\t\t\tcolor: rgb(255,50,50);\n            background: rgb(0,10,0);\n\t\t\t}\n\t\t\ttable {width: 100%;}\n\t\t\ttable, tr, td {\n\t\t\t\tpadding: 0;\n\t\t\t\tmargin: 0;\n\t\t\t\tborder: none;\n\t\t\t}\n\t\t\t.value {text-align: right;}\n</style>\n<body>\n<h1>STATUS MONITOR</h1>\n\t\t<fieldset>\n\t\t\t<legend>Arduino</legend>\n\t\t\t<table>\n\t\t\t\t<tr>\n\t\t\t\t\t<td>Voltage</td>\n\t\t\t\t\t<td class='value'><span>"+StringSplits[0]+"V</span></td>\n\t\t\t\t</tr>\n\t\t\t\t<tr>\n\t\t\t\t\t<td>Current</td>\n\t\t\t\t\t<td class='value'><span>" + StringSplits[1] + "mA</span></td>\n\t\t\t\t</tr>\n\n\t\t\t</table>\n\t\t</fieldset>\n\t\t<fieldset>\n\t\t\t<legend>Solar Power</legend>\n\t\t\t<table>\n\t\t\t\t<tr>\n\t\t\t\t\t<td>Voltage</td>\n\t\t\t\t\t<td class='value'><span>"+StringSplits[2]+"V</span></td>\n\t\t\t\t</tr>\n\t\t\t\t<tr>\n\t\t\t\t\t<td>Current</td>\n\t\t\t\t\t<td class='value'><span>"+StringSplits[3]+"mA</span></td>\n\t\t\t\t</tr>\n\t\t\t</table>\n\t\t</fieldset>\n</body>\n</html>\n";
    //......
    String postdata = StringSplits[0] + ";" + StringSplits[1] + ";" + StringSplits[2] + ";" + StringSplits[3] + ";";
    HTTPClient http;    //Declare object of class HTTPClient
  
    http.begin("http://192.168.1.37:8090/post");              //Specify request destination
    http.addHeader("Content-Type", "text/plain");    //Specify content-type header
 
    int httpCode = http.POST(postdata);   //Send the request
    String payload = http.getString();    //Get the response payload

    //Serial.println(httpCode);   //Print HTTP return code
    //Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection
    
    txtMsg = "";
  }

  //Tijd van server halen
  timeClient.update();
  epoch = timeClient.getEpochTime();
  //epoch = 1581756477;
  dag = ((((epoch/864)%36524)%3044)/100)+1;
  maand = ((epoch/2629743)%12) + 1;
  jaar = (epoch/31556926) + 1970;
  uur = timeClient.getHours();
  minuut = timeClient.getMinutes();
  seconde = timeClient.getSeconds();
  String tijd = String(dag) + "," + String(maand) + "," + String(jaar) + "," + String(uur) + "," + String(minuut) + "," + String(seconde) + ",";
  Serial.println(tijd);
  delay(2000);
  
}
