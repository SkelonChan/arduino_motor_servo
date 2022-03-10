#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/UInt16.h>
#include <Servo.h>

const int LEFT_STOP = 2;
const int LEFT_TUNE = 3;
const int LEFT_REV = 4;
const int RIGHT_REV = 7;
const int RIGHT_TUNE = 6;
const int RIGHT_STOP = 5;
int re_data=0;
int spd = 0;
 
ros::NodeHandle nh;

Servo myservo;  
int servoPin=9;

void messageCb(std_msgs::Int16 message)
{
  re_data = message.data;
}

void servomessageCb(std_msgs::UInt16 servo_msg){
 
    digitalWrite(13,HIGH);
    myservo.write(servo_msg.data);              // tell servo to go to position in variable 'pos'
    delay(15);
    digitalWrite(13,LOW);
}

ros::Subscriber<std_msgs::Int16> submotor("motor",&messageCb);
ros::Subscriber<std_msgs::UInt16> subservo("servo_led", &servomessageCb );

void runcommand(){
  if (re_data == 500)
  {
    digitalWrite(LEFT_STOP,LOW);
    digitalWrite(RIGHT_STOP,LOW);
  }
  else{
    digitalWrite(LEFT_STOP,HIGH);
    digitalWrite(RIGHT_STOP,HIGH);
  }


  if (re_data>=1000){
    digitalWrite(LEFT_REV, LOW);
    digitalWrite(RIGHT_REV, LOW);
    spd = re_data - 1000;
  }
  else if (re_data >= 0 && re_data < 1000)
  {
    digitalWrite(LEFT_REV,HIGH);
    digitalWrite(RIGHT_REV,HIGH);
    spd = re_data;
  }
   
  analogWrite(LEFT_TUNE,spd);
  analogWrite(RIGHT_TUNE,spd);
}


void setup()
{
  nh.initNode();
  nh.subscribe(submotor);
  nh.subscribe(subservo);
  pinMode(LEFT_REV,OUTPUT);
  pinMode(LEFT_STOP,OUTPUT);
  pinMode(LEFT_TUNE,OUTPUT);
  pinMode(RIGHT_REV,OUTPUT);
  pinMode(RIGHT_STOP,OUTPUT);
  pinMode(RIGHT_TUNE,OUTPUT);
  pinMode(13, OUTPUT);
  myservo.attach(servoPin);
}
 
void loop()
{
  runcommand();
  nh.spinOnce();
  delay(1);
}
