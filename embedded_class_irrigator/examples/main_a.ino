#include "Adafruit_seesaw.h"
#define WAITTIMEMIN 10000
#define trigPin 8  // trig pin is 8 DDB4
#define echoPin 9  // echo pin is 9 DDB5
#define led 10  // led1 is pin 10 DDB6 or PWM OC1B  Only used in tests
#define led2 11  // led2 is pin 11 DDB7 or PWM OC0A  Only used in tests
#define valveGate 7  // Signal to valve
#define powerPin 12  // Powers GPIO, soil sensor, water level sensor

Adafruit_seesaw ss;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("HERE");
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, HIGH);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(valveGate, OUTPUT);
  digitalWrite(valveGate, LOW);
  delay(10000);
}
long ndis, ndur;
long duration, distance;  // distance pinging variables
long arr[5];
int close_distance;
int maxi, mini;

void clearArr() {
  for (int i=0; i<5; i++) {
    arr[i]=0;
  }
}

void loop() {
  /****** Distance Read Block *******/
  Serial.println("STARTLOOP");
  for(int i=0; i<5; i++) {
    digitalWrite(trigPin,LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    duration = pulseIn(echoPin,HIGH);
    arr[i] = duration/58.2;
  }
  distance = 0;
  maxi=0;
  mini=0;
  for(int i=1; i<5; i++) {
    if(arr[i]>arr[maxi]){
      maxi=i;
    }
    if(arr[i]<arr[mini]){
      mini=i;
    }
  }
  for(int i=0; i<5; i++) {
    if(i==mini || i==maxi){
      continue;
    } else {
      distance+=arr[i];
    }
  }
  clearArr();
  distance=distance/3;
  Serial.println(distance);
  /****** Distance Read Block *******/

  /***** SOIL Read Block **********/
  float tempC = ss.getTemp();
  uint16_t capread = ss.touchRead(0);
  delay(2);
  /*******SOIL Read Block ******/

  /*******DETERMINE WHAT TO DO ****/
  if(capread<400) {  // water plant for <2cm of water
    Serial.println("OPEN");
    digitalWrite(valveGate,HIGH);
    ndis = distance;
    while(ndis <= distance + 5) {
      for(int i=0; i<5; i++) {
        digitalWrite(trigPin,LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin,HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin,LOW);
        ndur = pulseIn(echoPin,HIGH);
        arr[i] = ndur/58.2;
      }
      ndis=0;
      maxi=0;
      mini = 0;
      for(int i=1; i<5; i++) {
        if(arr[i]>arr[maxi]){
          maxi=i;
        }
        if(arr[i]<arr[mini]){
          mini=i;
        }
      }
      for(int i=0; i<5; i++) {
        if(i==mini || i==maxi){
          continue;
        } else {
          ndis+=arr[i];
        }
      }
      clearArr();
      ndis=ndis/3;
      Serial.print("ndis:");
      Serial.println(ndis);
      delayMicroseconds(50);
    }
    digitalWrite(valveGate,LOW);
    Serial.println("Closed");
    delay(WAITTIMEMIN);  // Don't want to pour more water before water absorbs to soil
    Serial.println("DONEWAIT");
  }
  /****** DETERMINE WHAT TO DO***/
}
