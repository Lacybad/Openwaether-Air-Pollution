// simple Code for reading information from openweathermap.org 

  //#include <WiFi.h> //Library for ESP32
  #include <ESP8266WiFi.h> //Library for ESP8266
  #include <ArduinoJson.h>
  #include <Wire.h>
  #include <TFT_eSPI.h> // Hardware-specific library
  #include <SPI.h>
  #include "Adafruit_GFX.h"

  #define FS_NO_GLOBALS
  #include <FS.h>
  

  TFT_eSPI tft = TFT_eSPI();       // Invoke custom library


  #define TFT_GREY        0x5AEB
  #define TFT_BLACK       0x0000      
  #define TFT_NAVY        0x000F
  #define TFT_DARKGREEN   0x03E0      
  #define TFT_DARKCYAN    0x03EF      
  #define TFT_MAROON      0x7800      
  #define TFT_PURPLE      0x780F      
  #define TFT_OLIVE       0x7BE0      
  #define TFT_LIGHTGREY   0xC618      
  #define TFT_DARKGREY    0x7BEF      
  #define TFT_BLUE        0x001F  
  #define TFT_GREEN       0x07E0
  #define TFT_CYAN        0x07FF
  #define TFT_RED         0xF800
  #define TFT_MAGENTA     0xF81F      
  #define TFT_YELLOW      0xFFE0      
  #define TFT_WHITE       0xFFFF    
  #define TFT_ORANGE      0xFDA0      
  #define TFT_GREENYELLOW 0xB7E0  
  #define TFT_PINK        0xFC9F    

  // Don't change!!!
  #define TFT_DC  D3
  #define TFT_CS  D8
  #define TFT_MOSI D7
  #define TFT_SCK D5
  #define TFT_RST D4

 


  const char* ssid     = "****************";                 // SSID of local network
  const char* password = "***********";                    // Password on network
                   
  

  WiFiClient client;
  char servername[]="api.openweathermap.org";              // remote server we will connect to
  String result;

  int  counter = 10;                                      

  float AQI;
  float CO;
  float O3;
  float SO2;
  float PM25;
  float NO;
  float PM10;
  float  NH3;
  float NO2;


const unsigned char wifiBitmap [] PROGMEM = {
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 31, 252, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 128,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 255, 255, 240, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 15, 255, 255, 248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 255, 255, 254, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 127, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
 255, 255, 255, 255, 128, 0, 0, 0, 0, 0, 0, 0, 1, 255, 255, 255, 255, 192, 0, 0,
 0, 0, 0, 0, 0, 3, 255, 255, 255, 255, 224, 0, 0, 0, 0, 0, 0, 0, 7,
 255, 255, 255, 255, 240, 0, 0, 0, 0, 0, 0, 0, 15, 255, 255, 255, 255, 248, 0, 0,
 0, 0, 0, 0, 0, 15, 255, 255, 255, 255, 252, 0, 0, 0, 0, 0, 0, 7, 255, 255,
 255, 255, 255, 255, 248, 0, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 0, 0,
 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 192, 0, 0, 0, 3, 255, 255, 255, 255,
 255, 255, 255, 255, 224, 0, 0, 0, 7, 255, 255, 255, 255, 255, 128, 0, 1, 240, 0, 0, 
 0, 7, 255, 255, 255, 255, 255, 0, 0, 0, 120, 0, 0, 0, 15, 255, 255, 255, 255, 254,
 0, 0, 0, 60, 0, 0, 0, 31, 255, 255, 255, 255, 252, 0, 0, 0, 28, 0, 0, 0,
 31, 255, 255, 255, 255, 248, 0, 0, 0, 30, 0, 0, 0, 63, 255, 255, 255, 31, 240, 0,
 0, 30, 14, 0, 0, 0, 63, 7, 7, 6, 15, 240, 255, 255, 62, 14, 0, 0, 0, 63, 7,
 7, 6, 15, 224, 255, 255, 62, 7, 0, 0, 0, 127, 7, 7, 14, 15, 224, 255, 255, 62, 7, 0, 0, 0, 127, 135, 7, 15, 
 31, 224, 255, 255, 30, 7, 0, 0, 0, 127, 135, 7, 15, 255, 224, 255, 255, 0, 3, 0, 0, 0, 127,
 134, 6, 15, 255, 224, 252, 0, 30, 3, 0, 0, 0, 127, 130, 2, 14, 31, 224, 252, 0,
 62, 3, 0, 0, 0, 127, 130, 2, 30, 31, 224, 252, 0, 62, 3, 0, 0, 0, 127, 194, 34, 30, 31, 224,
 252, 0, 62, 3, 0, 0, 0, 127, 194, 34, 30, 31, 224, 255, 254, 62, 3, 0, 0, 0, 127, 194, 34, 30,
 31, 224, 255, 254, 62, 3, 0, 0, 0, 127, 194, 34, 30, 31, 224, 255, 254, 62, 3, 0, 0, 0, 127,
 192, 32, 62, 31, 224, 255, 254, 62, 3, 0, 0, 0, 127, 224, 32, 62, 31, 224, 255, 254, 62, 3, 0, 0,
 0, 127, 224, 96, 62, 31, 224, 255, 254, 62, 3, 0, 0, 0, 127, 224, 112, 62, 31, 224, 252, 0, 62, 3, 0, 0,
 0, 127, 224, 112, 62, 31, 224, 252, 0, 62, 3, 0, 0, 0, 127, 224, 112, 126, 31, 224,
 252, 0, 62, 3, 0, 0, 0, 127, 240, 112, 126, 31, 192, 252, 0, 62, 7, 0, 0, 0, 127, 240, 240, 126, 31, 192,
 252, 0, 62, 7, 0, 0, 0, 63, 255, 255, 255, 255, 192, 248, 0, 62, 7, 0, 0, 0, 63, 255, 255, 255, 255,
 192, 0, 0, 0, 14, 0, 0, 0, 63, 255, 255, 255, 255, 128, 0, 0, 0, 14, 0, 0, 0, 31, 255, 255, 255, 255,
 128, 0, 0, 0, 30, 0, 0, 0, 31, 255, 255, 255, 255, 0, 0, 0, 0, 28, 0, 0, 0, 15, 255, 255, 255,
 255, 0, 0, 0, 0, 56, 0, 0, 0, 7, 255, 255, 255, 254, 0, 0, 0, 0, 248, 0, 0, 0, 7, 255, 255, 255, 252, 
 0, 0, 0, 1, 240, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 255, 224, 0, 0, 0, 0, 255, 255, 255,
 255, 255, 255, 255, 255, 192, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0,
 0, 0, 7, 255, 255, 255, 255, 255, 255, 240, 0, 0, 0, 0, 0, 0, 7, 255, 255, 255, 255, 248, 0, 0, 0,
 0, 0, 0, 0, 3, 255, 255, 255, 255, 240, 0, 0, 0, 0, 0, 0, 0, 3, 255, 255, 255, 255, 224, 0, 0, 0, 0,
 0, 0, 0, 1, 255, 255, 255, 255, 192, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 128, 0, 0, 0, 0, 0,
 0, 0, 0, 63, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 255, 255, 254, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 7, 255, 255, 248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 255, 255, 224, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 127, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  void setup()
  {
  Serial.begin(115200);

  SPIFFS.begin();
  listFiles();
  
  
  int cursorPosition=0;
  tft.init();
  tft.fillScreen(TFT_BLACK );
  tft.setRotation(0); //2
  tft.setTextWrap(0);
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  tft.drawBitmap(70, 120, wifiBitmap, 100, 70, TFT_WHITE);
  drawBmp("/openweather_logo.bmp", 7,230);
  delay(5000);
  
  tft.init();
  tft.fillScreen(TFT_BLACK );
  
  

  
  tft.setCursor(10,10);
  tft.print("Connecting");  
  Serial.println("Connecting");
  WiFi.begin(ssid, password);
  
             while (WiFi.status() != WL_CONNECTED) 
            {
            {
            delay(1000);
            tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
            tft.setTextSize(2);
            tft.setCursor(180,10);
            tft.print(cursorPosition); 
            tft.print(".");
            cursorPosition++;
            }

             
  }
  
  tft.setCursor(10,10);
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print("Connected to Wifi!");
  Serial.println("Connected to WIFI!");
  delay(1000);
    }







    void loop()
    
    {
    if(counter == 10)                                 //Get new data every 10 minutes
    {
       
      counter = 0;

      getWeatherData();
      delay(1000);
    }else
    {
      counter++;
     
      displayConditions(AQI,CO,O3,NO2, NO , PM10 , NH3, PM25);
      delay(1000);
      
      
{     tft.init();
    //  tft.fillScreen(TFT_BLACK );
      tft.drawCircle(220,305,10,TFT_WHITE);
      tft.fillCircle(220,305,8,TFT_GREEN);
      delay(6000);
      tft.fillCircle(220,305,8,TFT_BLACK);


      }
    }
    }

    void getWeatherData()                                //client function to send/receive GET request data.
   {
   if (client.connect(servername, 80))   
          {                                         //starts client connection, checks for connection
          client.println("GET /data/2.5/air_pollution?lat=00.000000&lon00.000000&appid=OWMAPIKEY");
          client.println("Host: api.openweathermap.org");
          client.println("User-Agent: ArduinoWiFi/1.1");
          client.println("Connection: close");
          client.println();
          } 
  else {
         Serial.println("connection failed");        //error message if no client connect
          Serial.println();
       }

  while(client.connected() && !client.available()) 
  delay(1);                                          //waits for data
  while (client.connected() || client.available())    
       {                                             //connected or data available
         char c = client.read();                     //gets byte from ethernet buffer
         result = result+c;
       }

  client.stop();                                      //stop client
  result.replace('[', ' ');
  result.replace(']', ' ');
  Serial.println(result);
  char jsonArray [result.length()+1];
  result.toCharArray(jsonArray,sizeof(jsonArray));
  jsonArray[result.length() + 1] = '\0';
  StaticJsonBuffer<1024> json_buf;
  JsonObject &root = json_buf.parseObject(jsonArray);

  if (!root.success())
  {
    Serial.println("parseObject() failed");
  }

 
  float aqi = root["list"]["main"]["aqi"];
  float co = root["list"]["components"]["co"];
  float o3= root["list"]["components"]["o3"];
  float so2 = root["list"]["components"]["so2"];
  float pm25 = root["list"]["components"]["pm2_5"];
  float pm10 = root["list"]["components"]["pm10"];
  float nh3 = root["list"]["components"]["nh3"];
  float no = root["list"]["components"]["no"];
  float no2 = root["list"]["components"]["no2"];
  
  NO2 = no2;
  AQI = aqi;
  CO = co;
  O3 = o3;
  SO2 = so2;
  NO = no;
  NH3 = nh3;
  PM25 = pm25;
  PM10 = pm10;
  
  
  
  Serial.print("Air Quality ");
  Serial.println(AQI);

  Serial.print("CO ");
  Serial.println(CO);

  Serial.print("O3 ");
  Serial.println(O3);

  Serial.print("NO2 ");
  Serial.println(NO2);

  Serial.print("SO2 ");
  Serial.println(SO2);

  Serial.print("PM2.5 ");
  Serial.println(PM25);

  Serial.print("PM10 ");
  Serial.println(PM10);

  Serial.print("NH3 ");
  Serial.println( NH3);

  Serial.print("NO ");
  Serial.println(NO);

 

 
 
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(10,296);
  tft.print("IP:");
  tft.print(WiFi.localIP());
  
  
  delay(1000);
 }



  void displayConditions(float AQI,float CO, float O3, float NO2 , float NO , float PM10, float  NH3, float PM25)
 {
 //tft.init();    
 tft.setTextSize(2);
 tft.setTextColor(TFT_YELLOW, TFT_BLACK);
 tft.setCursor(20,60);//Printing Temperature
 tft.print("CO: "); 
 tft.print( CO); 
 
 
 tft.setTextSize(2);
 tft.setTextColor(TFT_YELLOW, TFT_BLACK);
 tft.setCursor(20,80);//Printing Temperature
 tft.print("O3: "); 
 tft.print(O3);

 tft.setTextSize(2);
 tft.setTextColor(TFT_YELLOW, TFT_BLACK);
 tft.setCursor(20,100);//Printing Temperature
 tft.print("NO2: "); 
 tft.print(NO2);

 tft.setTextSize(2);
 tft.setTextColor(TFT_YELLOW, TFT_BLACK);
 tft.setCursor(20,120);//Printing Temperature
 tft.print("NO: "); 
 tft.print(NO);

 tft.setTextSize(2);
 tft.setTextColor(TFT_YELLOW, TFT_BLACK);
 tft.setCursor(20,140);//Printing Temperature
 tft.print("PM10: "); 
 tft.print(PM10);

 tft.setTextSize(2);
 tft.setTextColor(TFT_YELLOW, TFT_BLACK);
 tft.setCursor(20,160);//Printing Temperature
 tft.print("PM2.5: "); 
 tft.print(PM25);

 tft.setTextSize(2);
 tft.setTextColor(TFT_YELLOW, TFT_BLACK);
 tft.setCursor(20,180);//Printing Temperature
 tft.print("NH3: "); 
 tft.print(NH3);

 tft.setTextSize(2);
 tft.setTextColor(TFT_YELLOW, TFT_BLACK);
 tft.setCursor(20,200);//Printing Temperature
 tft.print("AQI:"); 
 //tft.print(AQI);

 if (AQI=1.00)
 {
   drawBmp("/1.bmp",20, 230);
   drawBmp("/11.bmp",80, 220);
 tft.setTextSize(2);
 tft.setTextColor(TFT_GREEN, TFT_BLACK);
 tft.setCursor(70,200);//Printing Temperature
 tft.print("Good ");
} else

if (AQI=2.00)
 {
   drawBmp("/2.bmp",20, 230);
   drawBmp("/22.bmp",80, 220);
   tft.setTextSize(2);
 tft.setTextColor(TFT_BLUE, TFT_BLACK);
 tft.setCursor(70,200);//Printing Temperature
 tft.print("Fair ");
} else

if (AQI=3.00)
 {
   drawBmp("/3.bmp",20, 230);
   drawBmp("/33.bmp",80, 220);
   tft.setTextSize(2);
 tft.setTextColor(TFT_YELLOW, TFT_BLACK);
 tft.setCursor(70,200);//Printing Temperature
 tft.print("Moderate ");
} else

if (AQI=4.00)
 {
   drawBmp("/4.bmp",20, 230);
   drawBmp("/44.bmp",80, 220);
   tft.setTextSize(2);
 tft.setTextColor(TFT_RED, TFT_BLACK);
 tft.setCursor(70,200);//Printing Temperature
 tft.print("Poor ");
} else

if (AQI=5.00)
 {
   drawBmp("/5.bmp",20, 230);
   drawBmp("/55.bmp",80, 220);
  tft.setTextSize(2);
 tft.setTextColor(TFT_RED, TFT_BLACK);
 tft.setCursor(80,200);//Printing Temperature
 tft.print("Very poor "); 
} 

 
 drawBmp("/mg.bmp",190, 90);
 
 

 
 delay(1000);
 }

    /*====================================================================================
  This sketch contains support functions for the ESP6266 SPIFFS filing system

  Created by Bodmer 15th Jan 2017
  ==================================================================================*/
void listDir(fs::FS &fs, const char * dirname, uint8_t levels);

// -------------------------------------------------------------------------
// List SPIFFS files in a neat format for ESP8266 or ESP32
// -------------------------------------------------------------------------
void listFiles(void) {
  Serial.println();
  Serial.println("SPIFFS files found:");

#ifdef ESP32
  listDir(SPIFFS, "/", true);
#else
  fs::Dir dir = SPIFFS.openDir("/"); // Root directory
  String  line = "=====================================";

  Serial.println(line);
  Serial.println("  File name               Size");
  Serial.println(line);

  while (dir.next()) {
    String fileName = dir.fileName();
    Serial.print(fileName);
    // File path can be 31 characters maximum in SPIFFS
    int spaces = 33 - fileName.length(); // Tabulate nicely
    if (spaces < 1) spaces = 1;
    while (spaces--) Serial.print(" ");
    fs::File f = dir.openFile("r");
    Serial.print(f.size()); Serial.println(" bytes");
    yield();
  }

  Serial.println(line);
#endif
  Serial.println();
  delay(1000);
}

#ifdef ESP32
void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  fs::File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  fs::File file = root.openNextFile();
  while (file) {

    if (file.isDirectory()) {
      Serial.print("DIR : ");
      String fileName = file.name();
      Serial.print(fileName);
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      String fileName = file.name();
      Serial.print("  " + fileName);
      // File path can be 31 characters maximum in SPIFFS
      int spaces = 33 - fileName.length(); // Tabulate nicely
      if (spaces < 1) spaces = 1;
      while (spaces--) Serial.print(" ");
      String fileSize = (String) file.size();
      spaces = 8 - fileSize.length(); // Tabulate nicely
      if (spaces < 1) spaces = 1;
      while (spaces--) Serial.print(" ");
      Serial.println(fileSize + " bytes");
    }

    file = root.openNextFile();
  }
}
#endif

// Bodmers BMP image rendering function

void drawBmp(const char *filename, int16_t x, int16_t y) {

  if ((x >= tft.width()) || (y >= tft.height())) return;

  fs::File bmpFS;

  // Open requested file on SD card
  bmpFS = SPIFFS.open(filename, "r");

  if (!bmpFS)
  {
    Serial.print("File not found");
    return;
  }

  uint32_t seekOffset;
  uint16_t w, h, row, col;
  uint8_t  r, g, b;

  uint32_t startTime = millis();

  if (read16(bmpFS) == 0x4D42)
  {
    read32(bmpFS);
    read32(bmpFS);
    seekOffset = read32(bmpFS);
    read32(bmpFS);
    w = read32(bmpFS);
    h = read32(bmpFS);

    if ((read16(bmpFS) == 1) && (read16(bmpFS) == 24) && (read32(bmpFS) == 0))
    {
      y += h - 1;

      bool oldSwapBytes = tft.getSwapBytes();
      tft.setSwapBytes(true);
      bmpFS.seek(seekOffset);

      uint16_t padding = (4 - ((w * 3) & 3)) & 3;
      uint8_t lineBuffer[w * 3 + padding];

      for (row = 0; row < h; row++) {
        
        bmpFS.read(lineBuffer, sizeof(lineBuffer));
        uint8_t*  bptr = lineBuffer;
        uint16_t* tptr = (uint16_t*)lineBuffer;
        // Convert 24 to 16 bit colours
        for (uint16_t col = 0; col < w; col++)
        {
          b = *bptr++;
          g = *bptr++;
          r = *bptr++;
          *tptr++ = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
        }

        // Push the pixel row to screen, pushImage will crop the line if needed
        // y is decremented as the BMP image is drawn bottom up
        tft.pushImage(x, y--, w, 1, (uint16_t*)lineBuffer);
      }
      tft.setSwapBytes(oldSwapBytes);
     // Serial.print("Loaded in "); Serial.print(millis() - startTime);
     // Serial.println(" ms");
    }
    else Serial.println("BMP format not recognized.");
  }
  bmpFS.close();
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(fs::File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(fs::File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
