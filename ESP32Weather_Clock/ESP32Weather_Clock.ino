#define _DICT_USE_PSRAM  // read more on https://github.com/arkhipenko/Dictionary used to limit RAM usage

#include <WiFi.h>
#include <HTTPClient.h>
#include <SSD1306Wire.h>
#include <time.h>
#include <ArduinoJson.h> // read more on https://arduinojson.org/
#include <Dictionary.h>  // read more on https://github.com/arkhipenko/Dictionary
 
const char* ssid     = "SSID";     
const char* password = "PASSWORD";
                                                                                    //Your city
const String endpoint = "http://api.openweathermap.org/data/2.5/weather?units=metric&q=London&appid=";
const String key = "api key from openweathermap";

const char* ntpServer = "pool.ntp.org"; // ntp server address
const long  gmtOffset_sec = 7200;       // gmt time offset
const int   daylightOffset_sec = 3600;  // daylight savings time offset

unsigned long lastUpdate;
unsigned long period;

const unsigned long patterntimerepeat = 20000; //display pattern repeats every 20 seconds
const unsigned long patterntime = 10000; // 10 secons of witch time is dispaled

String ico = "A";
String state = "";
String description = "";
double temp = 0;
double feels = 0;
double wind = 0;

Dictionary *IcoCon;

// OLED Display 128x64 build in 
SSD1306Wire  display(0x3c, 5, 4);

void WeatherCall(){
  HTTPClient http;
    
    http.begin(endpoint + key); // OpenWeatherMap api call
    int httpCode = http.GET();  // get request

    
    if (httpCode > 0) { //Check for the returning code
      String payload = http.getString();
      //Serial.println(httpCode); //can be used for debugging 
      handleReceivedMessage(payload);
    }
    else {
      Serial.println("HTTP Request error");
    }

    http.end();
}

void handleReceivedMessage(String message) {
  StaticJsonDocument<1500> doc;    //Memory pool. 

  DeserializationError error = deserializeJson(doc, message);
  
  Serial.println(message);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    ico = "A";
    return;
  }

  Serial.println();
  Serial.println("----- DATA FROM OPENWEATHER ----");

  // In order to change displaed information read ArduinoJson documentation and openweathermap JSON response structure:https://openweathermap.org/current
  String states = doc["weather"][0]["main"];
  state = states;
  Serial.print("State: ");
  Serial.println(states);

  String descriptions = doc["weather"][0]["description"];
  description = descriptions;
  Serial.print("Description: ");
  Serial.println(descriptions);

  //this code used to convert open weather icon id to DSEG_Weather font 
  String icocode = doc["weather"][0]["icon"];
  icocode = icocode.substring(0, 2);  
  ico = IcoCon->search(icocode);   

  temp = doc["main"]["temp"];
  Serial.print("Temp: ");
  Serial.println(temp);

  feels = doc["main"]["feels_like"];
  Serial.print("Feels: ");
  Serial.println(temp);

  wind = doc["wind"]["speed"];
  Serial.print("wind speeed: ");
  Serial.println(wind);

  Serial.println("------------------------------");
}
 
void setup() {
  // initialisation of openweather icon id to DSEG WEtaher font
  IcoCon = new Dictionary();
  IcoCon->insert("01","1");  
  IcoCon->insert("02","9");
  IcoCon->insert("03","2");
  IcoCon->insert("04","2");
  IcoCon->insert("09","4");
  IcoCon->insert("10","3");
  IcoCon->insert("11","7");
  IcoCon->insert("13","5");
  IcoCon->insert("50","2");
  Serial.begin(115200);         
  delay(10);
  Serial.println('\n');
  
  WiFi.begin(ssid, password);             // Connect to the network
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(500);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.println("Connection established");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP()); 

  // Get the NTP time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // OLED display init
  display.init();
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Access Point connected");
  display.drawString(0, 24, "AP IP address: ");
  display.drawString(0, 36, WiFi.localIP().toString());
  display.display();
  delay(1000);
  
  WeatherCall();

  lastUpdate = millis();
  period = millis();
  
}

void draw_weather() {
  display.clear();
  
  display.setTextAlignment(TEXT_ALIGN_LEFT);   
  display.setFont(Weather_24);  // this font consists only of sveral weather icons
  display.drawString(5, -5, ico); // draw icon
  
  display.setFont(ArialMT_Plain_16);  
  display.drawString(30, 0, description);   
  
  display.setFont(ArialMT_Plain_10); 
  display.drawString(0, 19, "Outside:    " + String(temp,1) + "C"); 
  display.drawString(0, 30, "Feels like: " + String(feels,1) + "C");  
  display.drawString(0, 41, "Wind speed: " + String(wind,1) + "m/s");  
  
  display.display();
}

void draw_time(char *timestr, char * date) {
  display.clear();
  
  display.setTextAlignment(TEXT_ALIGN_CENTER);  
  display.setFont(DSEG14_Modern_Regular_22); // this font mimics 14 segment diaply look
  display.drawString(display.getWidth()/2, 5, timestr);
    
  display.setFont(ArialMT_Plain_16);
  display.drawString(display.getWidth()/2, 40, date);
  
  display.display();
}
 
void loop() { 
  
  if(millis() - period > patterntimerepeat) {
    period = millis();
  }

  if(millis() - period < patterntime) {     
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        char time_str[16];
        strftime(time_str, 16, "%H:%M:%S", &timeinfo); 

        char date_str[16];
        strftime(date_str, 16, "%a %e %b %G", &timeinfo);// to change format read time.h documentation
  
        draw_time(time_str, date_str);
    }  
  }
  else {
    draw_weather();
  }  

  if(millis() - lastUpdate > 5 * 60000) {
    lastUpdate = millis();
    WeatherCall(); // update weather date every 5 minutes
  }

  
  delay(500);
  
}
