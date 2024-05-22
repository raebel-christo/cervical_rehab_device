/*
Code Developed by Raebel Christo

*/
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#define RF 11
#define RB 10
#define RT 9
#define LF 6
#define LB 5
#define LT 3
#define TOTAL_SAMPLES 100

Adafruit_MPU6050 imu;

const float F = 0.08;
const int Fs = 100;
const int n = TOTAL_SAMPLES;
float t;
const unsigned long sampling_interval = 1000/(F*n);
byte samples[TOTAL_SAMPLES];

void setup() {
  // put your setup code here, to run once:
  pinMode(RF, OUTPUT);
  pinMode(LF, OUTPUT);
  pinMode(RB, OUTPUT);
  pinMode(LB, OUTPUT);
  pinMode(RT, OUTPUT);
  pinMode(LT, OUTPUT);
  Serial.begin(115200);

  if(!imu.begin()) {
    Serial.println("Unable to find MPU6050");
    while(1) delay(10);
  }
  Serial.println("MPU6050 Initialized");
  imu.setAccelerometerRange(MPU6050_RANGE_16_G);
  imu.setGyroRange(MPU6050_RANGE_250_DEG);
  imu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  //Calculate sine wave
  for(int i=0; i<n; i++) {
    t = (float)i/Fs;
    samples[i] = (byte)(127.0*sin(2*3.14*t-3.14/2)+127.0); 
  }
}

void actuate(int sma1, int sma2) {
  sensors_event_t a,g,t;
  for(int i=0; i<n; i++) {
    analogWrite(sma1, samples[i]);
    analogWrite(sma2, samples[i]);
    //Serial.println(samples[i]);
    delay(sampling_interval);
    imu.getEvent(&a, &g, &t);
    Serial.print("X:");
    Serial.print(a.acceleration.x);
    Serial.print(",Y:");
    Serial.print(a.acceleration.y);
    Serial.print(",Z:");
    Serial.println(a.acceleration.z);
  }
}

void leftBend() {
  actuate(LF, LT);
}

void rightBend() {
  actuate(RF, RT);
}

void frontBend() {
  actuate(RB, LB);
}

void backBend() {
  actuate(LF, RF);
}

void leftTwist() {
  actuate(LF, RB);
}

void rightTwist() {
  actuate(RF, LB);
}

void killAll() {
  digitalWrite(LF, LOW);
  digitalWrite(LB, LOW);
  digitalWrite(LT, LOW);
  digitalWrite(RF, LOW);
  digitalWrite(RB, LOW);
  digitalWrite(RT, LOW);  
}

void loop() {
  while(Serial.available()>0) {
    delay(10);
    char c = Serial.read();
    switch(c) {
      case '0':
        delay(1000);
        break;
      case '1':
        Serial.println("Left Bend");
        leftBend();
        break;
       case '2':
        Serial.println("Right Bend");
        rightBend();
        break;
       case '3':
        Serial.println("Front Bend");
        frontBend();
        break;
       case '4':
        Serial.println("Back Bend");
        backBend();
        break;
       case '5':
        Serial.println("Left Twist");
        leftTwist();
        break;
       case '6':
        Serial.println("Right Twist");
        rightTwist();
        break;
       default:
        killAll();
    }
  }
}
