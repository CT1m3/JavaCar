#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

const char* ssid = "JavaCar";
const char* password = "12345678";
IPAddress apIP(192,168,1,1);

//Ultasonic sensor pins
const int echoPin = 5;
const int trigPin = 16;

//l298n motor driver pins

//Left motor
const int ENA = 13;
const int IN1 = 12;
const int IN2 = 14;

//Right motor
const int ENB = 4;
const int IN3 = 2;
const int IN4 = 0;

int zOver;
int zUnder;

WebSocketsServer webSocket = WebSocketsServer(81);

float getDistance() {
  float duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2; //speed of sound 0.0343 c/μS approx.
  return distance;
}

void motorDirection(uint8_t * input){
  analogWrite(ENA, 170);
  analogWrite(ENB, 170);
  if(*input == 'w'){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }

  else if(*input == 's'){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  else if(*input == 'a'){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  else if(*input == 'd'){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }

  else if(*input == 'z'){
    while(true){
    float distance = getDistance();

    if(distance > 15){
      zOver = round(map(distance, 15, 50, 40, 170));\
      if(zOver > 170){
        zOver = 170;
      }
      analogWrite(ENA, zOver);
      analogWrite(ENB, zOver);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);

      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }
    else if(distance < 12){
      zUnder = round(map(distance, 12, 1, 20, 170));
      if(zUnder > 170){
        zUnder = 170;
      }
      analogWrite(ENA, zUnder);
      analogWrite(ENB, zUnder);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);

      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
    else{
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);

      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }}
  }

  else{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connection from ", num);
        Serial.println(ip.toString());
      }
      break;

    case WStype_TEXT:
      Serial.printf("[%u] Text %s\n", num, payload);
      // Sends distance to java program
      motorDirection(payload);
      break;

    //Do nothing
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
      break;
  }
}

void setup() {
  //Ultrasonic sensor pin IO
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  //l298n motor driver pin IO
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  analogWrite(ENA, 170);
  analogWrite(ENB, 170);


  Serial.begin(115200);
  delay(20);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  Serial.println(WiFi.softAPIP());
  
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  String info = String(getDistance());
  webSocket.loop();
  webSocket.sendTXT(0, info);
}
