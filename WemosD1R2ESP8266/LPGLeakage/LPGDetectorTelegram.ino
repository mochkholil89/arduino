#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <MQ2.h>
#include <LiquidCrystal_I2C.h>

//Setting LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//inisialisasi sensor mq-2
int Analog_Input = A0;
int lpg, co, smoke;
//int lpg = 0;
String ppmLpg;
MQ2 mq2(Analog_Input);

//inisialisasi sound
int pinsuara = D13;
  
// Initialize Wifi connection to the router
char ssid[] = "xxxxxx";     // your network SSID (name)
char password[] = "xxxxxx"; // your network key

// Initialize Telegram BOT
#define BOTtoken "" // your Bot Token (Get from Botfather)
#define chatid "" //idbot

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

//fuzzy
float a = 0;
float minB = 1000;
float maxB = 3000;
float c = 4000;
float selisih = (maxB - minB) / 2;
float b = minB + selisih;
float aman, resiko, bahaya, out;
unsigned long start, finished, elapsed;


unsigned char batasAman(){
  if (lpg <= a){aman = 1;}
  else if (lpg >= a && lpg <= b){aman = (b-lpg)/(b-a);}
  else if (lpg >= b){aman = 0;}
  return aman;
}

unsigned char batasResiko(){
  if (lpg <= minB || lpg >= maxB){resiko = 0;}
  else if (lpg >= minB && lpg <= b){resiko = (lpg-minB)/(b-minB);}
  else if (lpg >= b && lpg <= maxB){resiko = (maxB-lpg)/(maxB-b);}
  return resiko;
}

unsigned char batasBahaya(){
  if (lpg <= b){bahaya = 0;}
  else if (lpg >= b && lpg <= c){bahaya = (lpg-b)/(c-b);}
  else if (lpg >= c){bahaya = 1;}
  return bahaya;
}

void fuzzifikasi(){
  batasAman();
  batasResiko();
  batasBahaya();
}

void fuzzy_rule (){
  fuzzifikasi();
  // jika suhu dingin maka motor lambat
  //rule1 = 150 - (dingin*50);
  // jika suhu hangat maka motor sedang
  //rule2a = 100 + (hangat*50);
  //rule2b = 200 - (hangat*50);
  // jika suhu panas maka motor cepat
  //rule3 = 150 + (panas*50);
  //defuzifikasi
  //out = ((rule1*dingin) + (rule2a*hangat) + (rule2b*hangat) + (rule3*panas)) / (dingin+hangat+hangat+panas);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.init(); // initialize the lcd
  lcd.backlight();
  //wifiStatus();
  mq2.begin();
  pinMode(pinsuara, OUTPUT);
  digitalWrite(pinsuara, LOW);
  delay(5000); 
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status() == WL_CONNECTED) {
    float* values = mq2.read(true); //set it false if you don't want to print the values in the Serial
    lpg = values[0];
    lpg = mq2.readLPG();
    //Serial.println(lpg);
    //smoke = values[2];
    //smoke = mq2.readSmoke();
  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connected");
    lcd.setCursor(0, 1);
    lcd.print("LPG:");
    lcd.print(lpg);
    //lcd.print(" Asap:");
    //lcd.print(smoke);
    lcd.print(" PPM");

    fuzzifikasi();
    
    if(resiko >= aman && resiko >= bahaya){
      digitalWrite(pinsuara, HIGH);
      delay(1000);
      digitalWrite(pinsuara, LOW);
      //delay(1000);
      //bot.sendChatAction(chatid, "Sedang mengetik...");
      //delay(100);
      Serial.println("Ambang Beresiko");
      Serial.print("LPG ");
      Serial.println(lpg);
      Serial.print("Aman ");
      Serial.println(aman);
      Serial.print("Resiko ");
      Serial.println(resiko);
      Serial.print("Bahaya ");
      Serial.println(bahaya);
      start=millis();
      ppmLpg = "Nilai Ambang Batas LPG dalam Ruangan Saat Ini : ";
      ppmLpg += int(lpg);
      ppmLpg += " PPM\n";
      ppmLpg += "Nilai Ambang Batas Aman LPG < 1000 PPM (SNI) \n";
      ppmLpg += "Silahkan Periksa Ruangan Anda ! \n";
      ppmLpg += "Dimungkinkan Terjadi Kebocoran LPG \n";
      ppmLpg += "Nilai Derajat Keanggotaan \n";
      ppmLpg += "Kategori Aman : ";
      ppmLpg += float(aman);
      ppmLpg += "\n";
      ppmLpg += "Kategori Beresiko : ";
      ppmLpg += float(resiko);
      ppmLpg += "\n";
      ppmLpg += "Kategori Bahaya : ";
      ppmLpg += float(bahaya);
      bot.sendMessage(chatid, ppmLpg, "");
      finished = millis();
      elapsed = finished - start;
      Serial.print("Waktu ");
      Serial.print(elapsed);
      Serial.println(" ms");
      //Serial.println("Berhasil Kirim Pesan");
      //Serial.println("Nilai Ambang Batas LPG Kondisi Beresiko");
      //digitalWrite(pinsuara, HIGH);
      //delay(1000);
      digitalWrite(pinsuara, LOW);
      //delay(1000);
    }else if(bahaya >= aman && bahaya >= resiko){
      digitalWrite(pinsuara, HIGH);
      //delay(1000);
      //digitalWrite(pinsuara, LOW);
      //delay(1000);
      //bot.sendChatAction(chatid, "Sedang mengetik...");
      //delay(100);
      Serial.println("Ambang Berbahaya");
      Serial.print("LPG ");
      Serial.println(lpg);
      Serial.print("Aman ");
      Serial.println(aman);
      Serial.print("Resiko ");
      Serial.println(resiko);
      Serial.print("Bahaya ");
      Serial.println(bahaya);
      start=millis();
      ppmLpg = "Nilai Ambang Batas LPG dalam Ruangan Saat Ini : ";
      ppmLpg += int(lpg);
      ppmLpg += " PPM\n";
      ppmLpg += "Nilai Ambang Batas Aman LPG < 1000 PPM (SNI) \n";
      ppmLpg += "Silahkan Periksa Ruangan Anda ! \n";
      ppmLpg += "Dimungkinkan Terjadi Kebocoran LPG \n";
      ppmLpg += "Nilai Derajat Keanggotaan \n";
      ppmLpg += "Kategori Aman : ";
      ppmLpg += float(aman);
      ppmLpg += "\n";
      ppmLpg += "Kategori Beresiko : ";
      ppmLpg += float(resiko);
      ppmLpg += "\n";
      ppmLpg += "Kategori Bahaya : ";
      ppmLpg += float(bahaya);
      bot.sendMessage(chatid, ppmLpg, "");
      finished = millis();
      elapsed = finished - start;
      Serial.print("Waktu :");
      Serial.print(elapsed);
      Serial.println(" ms");
      //Serial.println("Berhasil Kirim Pesan");
      //Serial.println("Nilai Ambang Batas LPG Kondisi Berbahaya");
      //digitalWrite(pinsuara, HIGH);
      //delay(1000);
      digitalWrite(pinsuara, LOW);
      //delay(1000);
    }else{
      Serial.println("Nilai Ambang Batas LPG Kondisi Aman");
      Serial.println("Ambang Aman");
      //digitalWrite(pinsuara, LOW);
      Serial.print("LPG ");
      Serial.println(lpg);
      Serial.print("Aman ");
      Serial.println(aman);
      Serial.print("Resiko ");
      Serial.println(resiko);
      Serial.print("Bahaya ");
      Serial.println(bahaya);
    }
    /**
    if(lpg >= 998.03){
      digitalWrite(pinsuara, HIGH);
      //bot.sendChatAction(chatid, "Sedang mengetik...");
      //delay(100);
      ppmLpg = "Nilai Ambang Batas LPG dalam Ruangan Saat Ini : ";
      ppmLpg += int(lpg);
      ppmLpg += " PPM\n";
      ppmLpg += "Nilai Ambang Batas Aman LPG < 1000 PPM \n";
      ppmLpg += "Silahkan Periksa Ruangan Anda ! \n";
      ppmLpg += "Dimungkinkan Terjadi Kebocoran LPG\n";
      bot.sendMessage(chatid, ppmLpg, "");
      Serial.println("Berhasil Kirim Pesan");
    }else{
      //ppmLpg = "Nilai Ambang Batas LPG Kondisi Aman";
      Serial.println("Nilai Ambang Batas LPG Kondisi Aman");
      //bot.sendMessage(chatid, ppmLpg, "");
      digitalWrite(pinsuara, LOW);
    }
    **/
  }else {
    wifiStatus1();
  }
  delay(1000);
}

void wifiStatus() {
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    lcd.setCursor(0, 0);         // move cursor to   (0, 0)
    lcd.print("Device Not Ready");        // print message at (0, 0)
    lcd.setCursor(0, 1);         // move cursor to   (2, 1)
    lcd.print("WiFiNotConnected");
    delay(500);
  }
  client.setInsecure();
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void wifiStatus1() {
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

//while(lpg <= 4000){
    fuzzifikasi();
    //Serial.println(aman);
    //Serial.println(resiko);
    //Serial.println(bahaya);

    if(resiko >= aman && resiko >= bahaya){
      digitalWrite(pinsuara, HIGH);
      delay(100);
      digitalWrite(pinsuara, LOW);
      delay(1000);
      Serial.println("Nilai Ambang Batas LPG Kondisi Beresiko");
      Serial.print("LPG ");
      Serial.println(lpg);
      Serial.print("Aman ");
      Serial.println(aman);
      Serial.print("Resiko ");
      Serial.println(resiko);
      Serial.print("Bahaya ");
      Serial.println(bahaya);
    }else if(bahaya >= aman && bahaya >= resiko){
      digitalWrite(pinsuara, HIGH);
      delay(1000);
      digitalWrite(pinsuara, LOW);
      delay(1000);
      Serial.println("Nilai Ambang Batas LPG Kondisi Berbahaya");
      Serial.print("LPG ");
      Serial.println(lpg);
      Serial.print("Aman ");
      Serial.println(aman);
      Serial.print("Resiko ");
      Serial.println(resiko);
      Serial.print("Bahaya ");
      Serial.println(bahaya);
    }else{
      Serial.println("Nilai Ambang Batas LPG Kondisi Aman");
      //digitalWrite(pinsuara, LOW);
      Serial.print("LPG ");
      Serial.println(lpg);
      Serial.print("Aman ");
      Serial.println(aman);
      Serial.print("Resiko ");
      Serial.println(resiko);
      Serial.print("Bahaya ");
      Serial.println(bahaya);
    }
//lpg++;
//delay(100);
//}

    /**if(lpg >= 998.03){
      digitalWrite(pinsuara, HIGH);
      delay(1000);
      digitalWrite(pinsuara, LOW);
      delay(1000);
      Serial.println("Nilai Ambang Batas LPG Kondisi Tidak Aman");
    }else{
      Serial.println("Nilai Ambang Batas LPG Kondisi Aman");
      digitalWrite(pinsuara, LOW);
    }
    **/   
    delay(1000);
  }
  client.setInsecure();
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
