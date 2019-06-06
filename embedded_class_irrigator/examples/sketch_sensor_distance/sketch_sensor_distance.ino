#define trigPin 8  // trig pin is 8 DDB4
#define echoPin 9  // echo pin is 9 DDB5
#define led 10  // led1 is pin 10 DDB6 or PWM OC1B
#define led2 11  // led2 is pin 11 DDB7 or PWM OC0A

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
  Serial.println("START");
}

void loop() {
  // put your main code here, to run repeatedly:
  long duration, distance;
  distance = 1;
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  duration = pulseIn(echoPin,HIGH);
  distance = (duration/2)/29.1;
//  distance = 1;

/*
  if(distance<4) {
    digitalWrite(led,HIGH);
    digitalWrite(led2,LOW);
  }
  else {
    digitalWrite(led,LOW);
    digitalWrite(led2,HIGH);
  }
  if(distance >= 200 || distance <=0) {
    Serial.println("OUT OF RANGE");
  }
  else {
    Serial.print(distance);
    Serial.println("cm");
  }
  delay(500);
  */
}
