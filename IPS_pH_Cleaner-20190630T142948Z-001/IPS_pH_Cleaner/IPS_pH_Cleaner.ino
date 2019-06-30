/* Control pH for 2 mode:
 *  +  Measure and clean PH probe
 *  +  preservation
 *  Writer:
 *    + Ly Hong Phong
 *    + Che Quang Huy
 */

//  Define button Pin
#define mode_1_1 2
#define mode_1_2 3
#define mode_2 4
//  Define switch journey Pin
#define SwitchX1 5
#define SwitchX2 6
#define SwitchY1 7
#define SwitchY2 8
#define Water_Sensor 9
//  Define Led Pin
#define led_1_1 A1
#define led_1_2 A0
#define led_2 13
//  Define Steper Motor Pin
#define stepPin_1 A3
#define dirPin_1 A2
#define stepPin_2 A5
#define dirPin_2 A4
// Define Relay 
#define Xa 11
#define Bom 10
#define Thoi 12

#define steps 800
//#define Delay_xa 80000 // thoi gian xa nuoc den khi can
#define Delay_xa 80000
//define function
void Mode1();
void Mode2();
void Mode3();
void OpenStepMotor(uint8_t Dir,int Dir_Status,uint8_t MotorPin,uint16_t stepss);


void setup() {
  // put your setup code here, to run once:

  //Setup project
  Serial.begin(9600);

  pinMode (stepPin_1, OUTPUT);
  pinMode (dirPin_1, OUTPUT);
  pinMode (stepPin_2, OUTPUT);
  pinMode (dirPin_2, OUTPUT);
  
  pinMode(mode_1_1, INPUT_PULLUP);
  pinMode(mode_1_2, INPUT_PULLUP);
  pinMode(mode_2  , INPUT_PULLUP);
  
  pinMode(SwitchX1, INPUT_PULLUP);
  pinMode(SwitchX2, INPUT_PULLUP);
  pinMode(SwitchY1, INPUT_PULLUP);
  pinMode(SwitchY2, INPUT_PULLUP);
  pinMode(Water_Sensor, INPUT_PULLUP);
  
  pinMode(led_1_1, OUTPUT);
  pinMode(led_1_2, OUTPUT);
  pinMode(led_2  , OUTPUT);

  pinMode(Bom, OUTPUT);
  pinMode(Xa, OUTPUT);
  pinMode(Thoi, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  // show what mode is running
  digitalWrite(led_1_1, LOW);
  digitalWrite(led_1_2, LOW);
  digitalWrite(led_2, LOW);
  
  if (digitalRead(SwitchX2) == 0 && digitalRead(SwitchY2) == 0) 
    digitalWrite(led_1_1, HIGH);
  else 
    if (digitalRead(SwitchX1) == 0 && digitalRead(SwitchY2) == 0)
      digitalWrite(led_2, HIGH);

  // read button add run 
  if(digitalRead(mode_1_1)==LOW) {  // check button 1_1
      Mode1();  // run mpde 1
  }
  else
    if(digitalRead(mode_1_2)==LOW) { // check button 1_2
      if (digitalRead(SwitchX2) == 0)
        Mode2();  // run mode 2
    }
     else
      if (digitalRead(mode_2) == LOW) { // check button 2
        if (digitalRead(SwitchX2) == 0)
          Mode3(); // run mode 3
      }
}

void Mode1(){
  /*
   * + turn on led 1_1
   * + move the pH.. up 
   * + turn left
   * + turn down
   * <-
   *  | ==  ^
   *  v ||  |
   * =--||----------------------------=
   *    ||
   *    ==
   * 
   */
  
  digitalWrite(led_1_1, HIGH);
  digitalWrite(led_1_2, LOW);
  digitalWrite(led_2, LOW);

  while(digitalRead(SwitchY1)!=LOW) OpenStepMotor(dirPin_1,0,stepPin_1,steps);
  delay(100);
  while(digitalRead(SwitchX2)!=LOW) OpenStepMotor(dirPin_2,1,stepPin_2,steps);
  delay(100);
  while(digitalRead(SwitchY2)!=LOW) OpenStepMotor(dirPin_1,1,stepPin_1,steps);
  delay(100); 
  
  digitalWrite(led_1_1, LOW); 
}

void Mode2(){  
   /*   Mode 2 jush clean the pH => mode 1
     * + check, if the pH on mode 1 => do mode 2 else, do mode 1 => mode 2
     * + pumb water 
     * + Clear the pH
     * + Stop pumb water => drying
     * + turn down
     *              
     *            | ==  ^
     *            v ||  |
     * =------------||----=
     *              ||
     *              ==
     * 
   */

  while(digitalRead(SwitchY2)!=LOW) 
    OpenStepMotor(dirPin_1,1,stepPin_1,steps);
      
  digitalWrite(led_1_1, LOW);
  digitalWrite(led_1_2, HIGH);
  digitalWrite(led_2, LOW);
  
  digitalWrite(Xa, HIGH);
  
  delay(Delay_xa);

  digitalWrite(Xa, LOW);
  for (int i=0; i< 8; i++){
    if (i < 5) 
      digitalWrite(Bom, HIGH);
    else {
      digitalWrite(Bom, LOW);
      digitalWrite(Thoi, HIGH);
    }
    
    for (int j=0; j< 2000; j++)
      OpenStepMotor(dirPin_1,0,stepPin_1,steps);
    delay(100);
    for (int j=0; j< 2000; j++)
      OpenStepMotor(dirPin_1, 1, stepPin_1, steps);
    delay(100);
  }
  
  delay(500);
  //digitalWrite(Xa, LOW);
  digitalWrite(Thoi, LOW);
  digitalWrite(Bom, LOW);
  
  delay(1000);
  digitalWrite(Bom, HIGH);
  int check = 0;
  while (check < 10){
    int t = digitalRead(Water_Sensor);
    if (t == LOW) check++;  
    Serial.println(t);
    delay(20);
  }
  digitalWrite(Bom, LOW);
  digitalWrite(led_1_2, LOW); 
}

void Mode3(){  
     /*   Mode 3 like mode 2 but it perform preservation
     * + check, if the pH on mode 1 => do mode 2 else, do mode 1 => mode 2
     * + pumb water 
     * + Clear the pH
     * + Stop pumb water => drying
     * + turn right
     * + turn down
     * + pumb water and check water sensor
     *              ->
     *            | ==  ^
     *            v ||  |
     * =------------||----=
     *              ||
     *              ==
     * 
   */
  while(digitalRead(SwitchY2)!=LOW) 
    OpenStepMotor(dirPin_1,1,stepPin_1,steps);
  
  digitalWrite(led_1_1, LOW);
  digitalWrite(led_1_2, LOW);
  digitalWrite(led_2, HIGH);
  digitalWrite(Xa, HIGH);
  
  delay(Delay_xa);

  digitalWrite(Xa, LOW);
  for (int i=0; i< 8; i++){
    if (i < 5) 
      digitalWrite(Bom, HIGH);
    else {
      digitalWrite(Bom, LOW);
      digitalWrite(Thoi, HIGH);
    }
    unsigned long checkTime=millis();
    for (int j=0; j< 2000; j++)
      OpenStepMotor(dirPin_1,0,stepPin_1,steps);
    delay(100);
    for (int j=0; j< 2000; j++)
      OpenStepMotor(dirPin_1, 1, stepPin_1, steps);
    delay(100);
  }
  delay(500);
  //digitalWrite(Xa, LOW);
  digitalWrite(Thoi, LOW);
  digitalWrite(Bom, LOW);
  
  while(digitalRead(SwitchY1)!=LOW){
    OpenStepMotor(dirPin_1,0,stepPin_1,steps);
  }
  delay(100);
  while(digitalRead(SwitchX1)!=LOW){
    OpenStepMotor(dirPin_2,0,stepPin_2,steps);
  }
  delay(100);
  while(digitalRead(SwitchY2)!=LOW){
    OpenStepMotor(dirPin_1,1,stepPin_1,steps);
  }
  delay(100);

  digitalWrite(Bom, HIGH);
  
  int check = 0;
  while (check < 10){
    int t = digitalRead(Water_Sensor);
    if (t == LOW) check++;  
    Serial.println(t);
    delay(20);
  }
    
  digitalWrite(Bom, LOW);
  digitalWrite(led_2, LOW);
}
// control StepMotor
void OpenStepMotor(uint8_t Dir,int Dir_Status,uint8_t MotorPin,uint16_t stepss){
    digitalWrite(Dir, Dir_Status);
    digitalWrite(MotorPin,HIGH);
    delayMicroseconds(stepss);
    digitalWrite(MotorPin,LOW);
    delayMicroseconds(stepss);
}
