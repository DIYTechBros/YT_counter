/*******************************************************************
 *  Based on                                                       *
 *  YouTube Channel statistics from the YouTube API                *
 *  By Brian Lough                                                 *
 *******************************************************************/

#include <YoutubeApi.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <ArduinoJson.h> 

#include <Wire.h>
#include <TM1650.h>

TM1650 Disp4Seg;

// I2C pins
#define SDA_pin 0 
#define SCL_pin 2 

//------- Replace the following! ------
char ssid[] = "wifi name";       // your network SSID (name)
char password[] = "wifi password";  // your network key
#define API_KEY "YoUr ApI kEy"  // your google apps API Token
#define CHANNEL_ID "your channel id" // makes up the url of channel


WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long api_mtbs = 600000; //mean time between api requests
unsigned long api_lasttime;   //last time api request has been done

long YT_subs = 0;
long YT_views = 0;


void setup() {

  client.setInsecure(); //// <--------------- include this line if your display is only showing 0000!!
  Wire.begin( SDA_pin, SCL_pin );
  Wire.setClock( 400000 );
  
  Disp4Seg.Init();
  Disp4Seg.SetBrightness( 5 );
  Disp4Seg.SetDot( 3, true );
  delay(500); 
  Disp4Seg.SetDot( 3, false );
  delay(500); 
  Disp4Seg.SetDot( 3, true );
  

  // Set WiFi to station mode and disconnect from an AP if it was Previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  IPAddress ip = WiFi.localIP();

  if(api.getChannelStatistics(CHANNEL_ID))
  {
    YT_subs = api.channelStats.subscriberCount;
    YT_views = api.channelStats.viewCount;
  }
  
}

void loop() {

  if (millis() - api_lasttime > api_mtbs)  {
    if(api.getChannelStatistics(CHANNEL_ID))
    {
      YT_subs = api.channelStats.subscriberCount;
      YT_views = api.channelStats.viewCount;
    }
    api_lasttime = millis();
  }

  Disp4Seg.WriteNum( YT_subs );
  Disp4Seg.SetDot( 3, false );

  delay(5000);

  Disp4Seg.WriteNum( YT_views );
  Disp4Seg.SetDot( 3, true );  

  delay(5000);
}
