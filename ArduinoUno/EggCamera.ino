#include <Pixy2.h>
#include <Pixy2SPI_SS.h>
#include <LiquidCrystal_I2C.h>

//settting pixy cam
Pixy2 pixy;

int signature = 0;
static int i = 0;
int x = 0;                      //positon x axis
int y = 0;
unsigned int width = 0;         //object's width
unsigned int height = 0;        //object's height
unsigned int area = 0;

//Setting LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//setting mic
int mic = 8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pixy.init();
  lcd.init(); // initialize the lcd
  lcd.backlight();
  pinMode(mic, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  scan(); 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("InisialiasiAlat");
  lcd.setCursor(0, 1);
  lcd.print("EggMonitoring");
    

  if(signature == 1)//looking for signature 2
  {
    //Serial.println("Percobaan");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Telur Segar");
    Serial.println("Telur Segar");
    lcd.setCursor(0, 1);
    lcd.print("Layak Makan");
    Serial.println("Layak Makan");
    digitalWrite(mic, HIGH);
    delay(200);
    digitalWrite(mic, LOW);
  }else if(signature == 2)
  {
     Serial.println("Insert Egg");
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("KondisiTelurAyam");
     lcd.setCursor(0, 1);
     lcd.print("#LetakkanTelur#");
     digitalWrite(mic, LOW);
  }else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Telur Busuk");
    Serial.println("Telur Busuk");
    lcd.setCursor(0, 1);
    lcd.print("TidakLayakMakan");
    Serial.println("TidakLayakMakan");
    digitalWrite(mic, HIGH);
  }
  
  delay(1000);
}

void scan()
{
  uint16_t blocks;
  blocks = pixy.ccc.getBlocks();  //receive data from pixy 
  signature = pixy.ccc.blocks[i].m_signature;    //get object's signature
  x = pixy.ccc.blocks[i].m_x;                    //get x position
  y = pixy.ccc.blocks[i].m_y;                    //get y position
  width = pixy.ccc.blocks[i].m_width;            //get width
  height = pixy.ccc.blocks[i].m_height;          //get height
}  
