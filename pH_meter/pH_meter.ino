#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SensorPin 0          //pH meter Analog output to Arduino Analog Input 0
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b, phValue;
int buf[10],temp;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "fc8fc345dec94b3bb01156d2e397981a";   // you can change auth (Auth Token) to connect to your Blynk

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "UIT Public";
char pass[] = "";

void setup()
{
  // Debug console
  Serial.begin(9600);
  lcd.begin(5, 4);   // ESP8266 I2C with pin 5-SDA 4-SCL
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  LCD_display(Read_pH());
  Blynk.virtualWrite(2, phValue);
  delay(1000);
}

float Read_pH(){
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
  avgValue+=buf[i];
  phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  return phValue;
}

void LCD_display(float t){
  lcd.setCursor(0,0);
  lcd.print("pH la : ");
  lcd.setCursor(9,0);
  lcd.print(t);
  if (t == 7) {
    lcd.setCursor(2, 1);
    lcd.print("Trung tinh");
  }
  else {
    if (t > 7) {
      lcd.setCursor(5, 1);
      lcd.print("Bazo");
    }
    else {
      lcd.setCursor(5, 1);
      lcd.print("Axit");
    }
  }
}
