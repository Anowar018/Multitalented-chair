#include <NewPing.h>
#include <Servo.h>
int xPin=A0;
int yPin=A1;

String voice;
int trigPin=50;
int echoPin=52;
int horn = 22;

#define ENA_m1 5        // Enable/speed motor Front Right 
#define ENB_m1 6        // Enable/speed motor Back Right
#define ENA_m2 10       // Enable/speed motor Front Left
#define ENB_m2 11       // Enable/speed motor Back Left

#define IN_11  2        // L298N #1 in 1 motor Front Right
#define IN_12  3        // L298N #1 in 2 motor Front Right
#define IN_13  4        // L298N #1 in 3 motor Back Right
#define IN_14  7        // L298N #1 in 4 motor Back Right

#define IN_21  8        // L298N #2 in 1 motor Front Left
#define IN_22  9        // L298N #2 in 2 motor Front Left
#define IN_23  12       // L298N #2 in 3 motor Back Left
#define IN_24  13       // L298N #2 in 4 motor Back Left

int speedCar = 200;     // 50 - 255.
int speed_Coeff = 4;

const int s1= 24;
const int s2 =26;
const int s3 =28;

const int servoPin=23;
// defines variables
long duration;
int distance;
int distance_f;
int distance_r;
int distance_l;
int maxLowDistance=20;
Servo myservo;
int buttonState1 = 0;    
int buttonState2 = 0;    
int buttonState3 = 0;    

void setup() 
{
 pinMode(xPin,INPUT);
  pinMode(yPin,INPUT);

  pinMode(s1, INPUT);
  pinMode(s2, INPUT);
  pinMode(s3, INPUT);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(horn,OUTPUT);

  pinMode(ENA_m1, OUTPUT);
    pinMode(ENB_m1, OUTPUT);
    pinMode(ENA_m2, OUTPUT);
    pinMode(ENB_m2, OUTPUT);
  
    pinMode(IN_11, OUTPUT);
    pinMode(IN_12, OUTPUT);
    pinMode(IN_13, OUTPUT);
    pinMode(IN_14, OUTPUT);
    
    pinMode(IN_21, OUTPUT);
    pinMode(IN_22, OUTPUT);
    pinMode(IN_23, OUTPUT);
    pinMode(IN_24, OUTPUT);
  Serial.begin(9600);
   myservo.attach(servoPin);
myservo.write(90);
}

void loop()
{
  buttonState1 = digitalRead(s1);
buttonState2 = digitalRead(s2);
buttonState3 = digitalRead(s3);

  if (buttonState1==HIGH)
  {
    voice_control();
  }
  else if (buttonState2==HIGH)
  {
    obstacle();
  }
  else 
  {
    gesture();
  }
}
void gesture() 
{
  int xval=analogRead(xPin);
  int yval=analogRead(yPin);
  Serial.println("xval=");
  Serial.println(xval);
  Serial.println("yval=");
  Serial.println(yval);

  if ((xval>294 && xval<340) && (yval>294 && yval<340)) //stop
  {
   stopRobot();
  } 

  else 
  { 
    if ((xval>170 && xval<380) && (yval>180 && yval<340)) //forward
   {
     goAhead();
      
    }
    if ((xval>160 && xval<294) && (yval>130 && yval<340)) //backward
  {
  goBack();
      
    }   

    if ((xval>180 && xval<340) && (yval>160 && yval<380)) //left
    {
      goLeft();
     }


    if ((xval>150 && xval<340) && (yval>160 && yval<340))//right
    {
      goRight();
      
    }
  }
}

  
void voice_control()
{

 while (Serial.available()){ //Check if there is an available byte to read

delay(10); //Delay added to make thing stable

char c = Serial.read(); //Conduct a serial read

if (c == '#') {break;} //Exit the loop when the # is detected after the word

voice += c; //Shorthand for voice = voice + c

}

if (voice.length() > 0) {

Serial.println(voice);

if(voice == "*1") 
{
  goAhead();
  } //Turn Off All Pins (Call Function)

else if(voice == "*2")
{
  goBack();
  } //Turn On All Pins (Call Function)
  else if(voice == "*3")
{
  goLeft();
  } //Turn On All Pins (Call Function)
  else if(voice == "*4")
{
  goRight();
  } //Turn On All Pins (Call Function)
   else if(voice == "*5")
{
  stopRobot();
  } //Turn On All Pins (Call Function)

 
}
voice="";
}

 
void obstacle()
 {
  distance_f=ping();
   if(distance_f > maxLowDistance){
     goAhead();
     delay(300);
   }else{
      stopRobot();
      get_Distance();
      if(distance_r > maxLowDistance){
        goRight();
        delay(300);
        goAhead();
      }else if(distance_l > maxLowDistance){
        goLeft();
        delay(300);
        goAhead();
      }else{
        goBack();
        delay(300);
        stopRobot();
      }
    
   }
 }
   

int ping(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  Serial.print("Distance is= ");
  Serial.print(distance);
  Serial.println("CM");
  // Prints the distance on the Serial Monitor
  return distance;
}
void get_Distance(){
  myservo.write(0);
  delay(500);
  int temp_r1=ping();
  myservo.write(45);
  delay(500);
  int temp_r2=ping();
  if(temp_r1<temp_r2){
    distance_r=temp_r1;
  }else{
    distance_r=temp_r2;
  }
  myservo.write(90);
  delay(500);
  distance_f=ping();
  
  myservo.write(135);
  delay(500);
  int temp_l1=ping();
  myservo.write(180);
  delay(500);
  int temp_l2=ping();
  if(temp_l1<temp_l2){
    distance_l=temp_l1;
  }else{
    distance_l=temp_l2;
  }
  myservo.write(90);
  
}

void goAhead(){ 
  

      digitalWrite(IN_11, HIGH);
      digitalWrite(IN_12, LOW);
      analogWrite(ENA_m1, speedCar);

      digitalWrite(IN_13, HIGH);
      digitalWrite(IN_14, LOW);
      analogWrite(ENB_m1, speedCar);


      digitalWrite(IN_21, HIGH);
      digitalWrite(IN_22, LOW);
      analogWrite(ENA_m2, speedCar);


      digitalWrite(IN_23, HIGH);
      digitalWrite(IN_24, LOW);
      analogWrite(ENB_m2, speedCar);

  }

void goBack(){ 
      digitalWrite(IN_11, LOW);
      digitalWrite(IN_12, HIGH);
      analogWrite(ENA_m1, speedCar);


      digitalWrite(IN_13, LOW);
      digitalWrite(IN_14, HIGH);
      analogWrite(ENB_m1, speedCar);


      digitalWrite(IN_21, LOW);
      digitalWrite(IN_22, HIGH);
      analogWrite(ENA_m2, speedCar);


      digitalWrite(IN_23, LOW);
      digitalWrite(IN_24, HIGH);
      analogWrite(ENB_m2, speedCar);

  }

void goRight(){ 

      digitalWrite(IN_11, LOW);
      digitalWrite(IN_12, LOW);
      analogWrite(ENA_m1, speedCar);


      digitalWrite(IN_13, HIGH);
      digitalWrite(IN_14, LOW);
      analogWrite(ENB_m1, speedCar);


      digitalWrite(IN_21, LOW);
      digitalWrite(IN_22, LOW);
      analogWrite(ENA_m2, speedCar);


      digitalWrite(IN_23, HIGH);
      digitalWrite(IN_24, LOW);
      analogWrite(ENB_m2, speedCar);
      //digitalWrite(light_BR, HIGH);

  }

void goLeft(){

      digitalWrite(IN_11, HIGH);
      digitalWrite(IN_12, LOW);
      analogWrite(ENA_m1, speedCar);


      digitalWrite(IN_13, LOW);
      digitalWrite(IN_14, LOW);
      analogWrite(ENB_m1, speedCar);

        
      digitalWrite(IN_21, HIGH);
      digitalWrite(IN_22, LOW);
      analogWrite(ENA_m2, speedCar);


      digitalWrite(IN_23, LOW);
      digitalWrite(IN_24, LOW);
      analogWrite(ENB_m2, speedCar);
//      digitalWrite(light_BL, HIGH);
        
  }

void goAheadRight(){
      
      digitalWrite(IN_11, HIGH);
      digitalWrite(IN_12, LOW);
      analogWrite(ENA_m1, speedCar/speed_Coeff);

      digitalWrite(IN_13, LOW);
      digitalWrite(IN_14, HIGH);
      analogWrite(ENB_m1, speedCar/speed_Coeff);


      digitalWrite(IN_21, LOW);
      digitalWrite(IN_22, HIGH);
      analogWrite(ENA_m2, speedCar);


      digitalWrite(IN_23, HIGH);
      digitalWrite(IN_24, LOW);
      analogWrite(ENB_m2, speedCar);
 
  }

void goAheadLeft(){
      
      digitalWrite(IN_11, HIGH);
      digitalWrite(IN_12, LOW);
      analogWrite(ENA_m1, speedCar);

      digitalWrite(IN_13, LOW);
      digitalWrite(IN_14, HIGH);
      analogWrite(ENB_m1, speedCar);


      digitalWrite(IN_21, LOW);
      digitalWrite(IN_22, HIGH);
      analogWrite(ENA_m2, speedCar/speed_Coeff);


      digitalWrite(IN_23, HIGH);
      digitalWrite(IN_24, LOW);
      analogWrite(ENB_m2, speedCar/speed_Coeff);
 
  }

void goBackRight(){ 

      digitalWrite(IN_11, LOW);
      digitalWrite(IN_12, HIGH);
      analogWrite(ENA_m1, speedCar/speed_Coeff);


      digitalWrite(IN_13, HIGH);
      digitalWrite(IN_14, LOW);
      analogWrite(ENB_m1, speedCar/speed_Coeff);


      digitalWrite(IN_21, HIGH);
      digitalWrite(IN_22, LOW);
      analogWrite(ENA_m2, speedCar);


      digitalWrite(IN_23, LOW);
      digitalWrite(IN_24, HIGH);
      analogWrite(ENB_m2, speedCar);

  }

void goBackLeft(){ 

      digitalWrite(IN_11, LOW);
      digitalWrite(IN_12, HIGH);
      analogWrite(ENA_m1, speedCar);


      digitalWrite(IN_13, HIGH);
      digitalWrite(IN_14, LOW);
      analogWrite(ENB_m1, speedCar);


      digitalWrite(IN_21, HIGH);
      digitalWrite(IN_22, LOW);
      analogWrite(ENA_m2, speedCar/speed_Coeff);


      digitalWrite(IN_23, LOW);
      digitalWrite(IN_24, HIGH);
      analogWrite(ENB_m2, speedCar/speed_Coeff);

  }

void stopRobot(){  

      digitalWrite(IN_11, LOW);
      digitalWrite(IN_12, LOW);
      analogWrite(ENA_m1, speedCar);


      digitalWrite(IN_13, LOW);
      digitalWrite(IN_14, LOW);
      analogWrite(ENB_m1, speedCar);

  
      digitalWrite(IN_21, LOW);
      digitalWrite(IN_22, LOW);
      analogWrite(ENA_m2, speedCar);

      
      digitalWrite(IN_23, LOW);
      digitalWrite(IN_24, LOW);
      analogWrite(ENB_m2, speedCar);
  digitalWrite(horn, HIGH);
  delay(500);
  digitalWrite(horn, LOW);
  delay(500);
  }
  
