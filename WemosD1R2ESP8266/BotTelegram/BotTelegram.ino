// The version of ESP8266 core needs to be 2.5 or higher
// or else your bot will not connect.

// ----------------------------
// Standard ESP8266 Libraries
// ----------------------------

#include <ESP8266WiFi.h>

#include <WiFiClientSecure.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <UniversalTelegramBot.h>

// Initialize Wifi connection to the router
char ssid[] = "AshfaKiya";     // your network SSID (name)
char password[] = "shinobi2019"; // your network key

// Initialize Telegram BOT
#define BOTtoken "YOUR TOKEN BOT TELEGRAM" // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

//Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

//const int ledPin = LED_BUILTIN;
const int ledPin1 = D0;
const int ledPin2 = D1;
const int ledPin3 = D2;
const int ledPin4 = D3;
const int ledPin5 = D4;

//status
int ledStatus1 = 0;
int ledStatus2 = 0;
int ledStatus3 = 0;
int ledStatus4 = 0;
int ledStatus5 = 0;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/TerasOn") {
      digitalWrite(ledPin1, HIGH);   // turn the LED on (HIGH is the voltage level)
      unsigned long StartTime = millis();
      Serial.println(StartTime);
      ledStatus1 = 1;
      bot.sendMessage(chat_id, "Teras is ON", "");
      unsigned long CurrentTime = millis();
      Serial.println(CurrentTime);
      unsigned long ElapsedTime = CurrentTime - StartTime;
      Serial.println(ElapsedTime);
    }
    if (text == "/Kamar1On") {
      digitalWrite(ledPin2, HIGH);   // turn the LED on (HIGH is the voltage level)
      ledStatus2 = 1;
      bot.sendMessage(chat_id, "Kamar 1 is ON", "");
    }

    if (text == "/Kamar2On") {
      digitalWrite(ledPin3, HIGH);   // turn the LED on (HIGH is the voltage level)
      ledStatus3 = 1;
      bot.sendMessage(chat_id, "Kamar 2 is ON", "");
    }

    if (text == "/Bermain1On") {
      digitalWrite(ledPin4, HIGH);   // turn the LED on (HIGH is the voltage level)
      ledStatus4 = 1;
      bot.sendMessage(chat_id, "Bermain 1 is ON", "");
    }
    if (text == "/Bermain2On") {
      digitalWrite(ledPin5, HIGH);   // turn the LED on (HIGH is the voltage level)
      ledStatus5 = 1;
      bot.sendMessage(chat_id, "Bermain 2 is ON", "");
    }
    
    if (text == "/TerasOff") {
      ledStatus1 = 0;
      digitalWrite(ledPin1, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Teras is OFF", "");
    }
    if (text == "/Kamar1Off") {
      ledStatus2 = 0;
      digitalWrite(ledPin2, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Kamar 1 is OFF", "");
    }

    if (text == "/Kamar2Off") {
      ledStatus3 = 0;
      digitalWrite(ledPin3, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Kamar 2 is OFF", "");
    }

    if (text == "/Bermain1Off") {
      ledStatus4 = 0;
      digitalWrite(ledPin4, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Bermain 1 is OFF", "");
    }

    if (text == "/Bermain2Off") {
      ledStatus5 = 0;
      digitalWrite(ledPin5, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Bermain 2 is OFF", "");
    }

    if (text == "/OnAll") {
      digitalWrite(ledPin1, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(ledPin2, HIGH);
      digitalWrite(ledPin3, HIGH);
      digitalWrite(ledPin4, HIGH);
      digitalWrite(ledPin5, HIGH);
      ledStatus1 = 1;
      ledStatus2 = 1;
      ledStatus3 = 1;
      ledStatus4 = 1;
      ledStatus5 = 1;
      bot.sendMessage(chat_id, "On All", "");     
    }

    if (text == "/OffAll") {
      digitalWrite(ledPin1, LOW);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);
      digitalWrite(ledPin4, LOW);
      digitalWrite(ledPin5, LOW);
      ledStatus1 = 0;
      ledStatus2 = 0;
      ledStatus3 = 0;
      ledStatus4 = 0;
      ledStatus5 = 0;
      bot.sendMessage(chat_id, "Off All", "");     
    }

    if (text == "/Riwayat") {
      if(ledStatus1){
        bot.sendMessage(chat_id, "Lampu Teras On", "");
      } else {
        bot.sendMessage(chat_id, "Lampu Teras Off", "");
      }
      if(ledStatus2){
        bot.sendMessage(chat_id, "Lampu Kamar 1 On", "");
      } else {
        bot.sendMessage(chat_id, "Lampu Kamar 1 Off", "");
      }
      if(ledStatus3){
        bot.sendMessage(chat_id, "Lampu Kamar 2 On", "");
      } else {
        bot.sendMessage(chat_id, "Lampu Kamar 2 Off", "");
      }
      if(ledStatus4){
        bot.sendMessage(chat_id, "Lampu Bermain 1 On", "");
      } else {
        bot.sendMessage(chat_id, "Lampu Bermain 1 Off", "");
      }
      if(ledStatus5){
        bot.sendMessage(chat_id, "Lampu Bermain 2 On", "");
      } else {
        bot.sendMessage(chat_id, "Lampu Bermain 2 Off", "");
      }
    }

   if (text == "/Start") {
      String welcome = "Selamat Datang di Home Smart Ashfa Kiya, " + from_name + ".\n";
      welcome += "Berikut perintah kontrol Home Smart Bot.\n\n";
      welcome += "/TerasOn : Lampu Teras On\n";
      welcome += "/Kamar1On : Lampu Kamar 1 On\n";
      welcome += "/Kamar2On : Lampu Kamar 2 On\n";
      welcome += "/Bermain1On : Lampu Bermain 1 On\n";
      welcome += "/Bermain1On : Lampu Bermain 2 On\n";
      welcome += "/TerasOff : Lampu Teras Off\n";
      welcome += "/Kamar1Off : Lampu Kamar 1 Off\n";
      welcome += "/Kamar2Off : Lampu Kamar 2 Off\n";
      welcome += "/Bermain1Off : Lampu Bermain 1 Off\n";
      welcome += "/Bermain1Off : Lampu Bermain 2 Off\n";
      welcome += "/OnAll : Lampu On Semua\n";
      welcome += "/OffAll :Lampu Off Semua\n";
      welcome += "/Riwayat : Status Lampu\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}


void setup() {
  Serial.begin(115200);
 
  // This is the simplest way of getting this working
  // if you are passing sensitive information, or controlling
  // something important, please either use certStore or at
  // least client.setFingerPrint
  client.setInsecure();

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(10);

  pinMode(ledPin1, OUTPUT); // initialize digital ledPin as an output.
  //pinMode(ledPin2, OUTPUT); // initialize digital ledPin as an output.
  //pinMode(ledPin3, OUTPUT); // initialize digital ledPin as an output.
  //pinMode(ledPin4, OUTPUT); // initialize digital ledPin as an output.
  //pinMode(ledPin5, OUTPUT); // initialize digital ledPin as an output.
  //delay(10);
  //digitalWrite(ledPin, HIGH); // initialize pin as off (active high)
  digitalWrite(ledPin1, LOW); // initialize pin as off (active low)
  //digitalWrite(ledPin2, LOW); // initialize pin as off (active low)
  //digitalWrite(ledPin3, LOW); // initialize pin as off (active low)
  //digitalWrite(ledPin4, LOW); // initialize pin as off (active low)
  //digitalWrite(ledPin5, LOW); // initialize pin as off (active low)
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    
    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    //delay(1);
    lastTimeBotRan = millis();
    
  }
}
