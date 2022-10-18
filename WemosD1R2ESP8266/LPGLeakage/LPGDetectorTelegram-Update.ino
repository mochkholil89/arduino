#include <TelegramCertificate.h>
#include <UniversalTelegramBot.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <MQ2.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

//Setting LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//inisialisasi sensor mq-2
int Analog_Input = A0;
int lpg;
//int lpg = 0;
String ppmLpg;
MQ2 mq2(Analog_Input);

//inisialisasi sound
int pinsuara = D13;
  
// Initialize Wifi connection to the router
char ssid[] = "LPGDetector";     // your network SSID (name)
char password[] = "qwerty2021"; // your network key

// Initialize Telegram BOT
#define BOTtoken "2112889776:AAENf9PfRWqnc4YlEr4vEW1SiHDSQO8yp1k"
#define chatId "2146349907"
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.init(); // initialize the lcd
  lcd.backlight();
  mq2.begin();
  pinMode(pinsuara, OUTPUT);
  digitalWrite(pinsuara, LOW);
  wifiStatus();
  delay(1000); 
}

void loop() {
  // put your main code here, to run repeatedly:
  //client.setInsecure();
  if(WiFi.status() == WL_CONNECTED) {
    wifiStatusKonek();
  }else{
    wifiStatus();
  }
}

void wifiStatus() {
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    //Serial.print(".");
    float* values = mq2.read(true); //set it false if you don't want to print the values in the Serial
    lpg = values[0];
    lpg = mq2.readLPG();
    lcd.clear();
    lcd.setCursor(0, 0);         // move cursor to   (2, 1)
    lcd.print("WiFiNotConnected");
    lcd.setCursor(0, 1);
    lcd.print("LPG:");
    lcd.print(lpg);
    lcd.print(" PPM");
    delay(1000);

    if(lpg > 1000){
      digitalWrite(pinsuara, HIGH);
      delay(1000);
      digitalWrite(pinsuara, LOW);
      delay(1000);
      Serial.println("Nilai Ambang Batas LPG Kondisi Berbahaya");
      Serial.print("LPG ");
      Serial.println(lpg);    
    }
  }
  client.setInsecure();  
  bot.sendMessage(chatId, "Perangkat LPG Detector Tersambung Dengan Internet", "");
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void wifiStatusKonek() {
  WiFi.disconnect();
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setInsecure();
  delay(5000);
  float* values = mq2.read(true); //set it false if you don't want to print the values in the Serial
  lpg = values[0];
  lpg = mq2.readLPG();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print("LPG:");
  lcd.print(lpg);
  lcd.print(" PPM");
    
  if(lpg > 1000){
    digitalWrite(pinsuara, HIGH);
    Serial.println("Ambang Berbahaya");
    Serial.print("LPG ");
    Serial.println(lpg);
    ppmLpg = "Nilai Ambang Batas LPG dalam Ruangan Saat Ini : ";
    ppmLpg += int(lpg);
    ppmLpg += " PPM\n";
    ppmLpg += "Nilai Ambang Batas Aman LPG < 1000 PPM (SNI) \n";
    ppmLpg += "Silahkan Periksa Ruangan Anda ! \n";
    ppmLpg += "Dimungkinkan Terjadi Kebocoran LPG \n";
    //WiFi.disconnect();
    //WiFi.mode(WIFI_STA);
    //WiFi.begin(ssid, password);
    //client.setInsecure();
    bot.sendMessage(chatId, ppmLpg, "");
    digitalWrite(pinsuara, LOW);
  }    
  //bot.sendMessage(chatId, "ppmLpg", "");
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
