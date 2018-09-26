/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  WARNING!
    It's very tricky to get it working. Please read this article:
    http://help.blynk.cc/hardware-and-libraries/arduino/esp8266-with-at-firmware

  This example shows how to send values to the Blynk App,
  when there is a widget, attached to the Virtual Pin and it
  is set to some frequency

  Project setup in the app:
    Value Display widget attached to V5. Set any reading
    frequency (i.e. 1 second)
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

//Definir variaveis

int solenoidPin = 4;    //This is the output pin on the Arduino we are using
const int trig = 12;
const int echo = 13;
float duracao;
float distancia;
float setpoint = 0;
String estadoValve = "";

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "05fc1eda512a41e087aa93e188d2f2c2";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "NEO";
char pass[] = "matrixe315";

// Hardware Serial on Mega, Leonardo, Micro...
//#define EspSerial Serial1

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.

  //Controle OnOff da Valvula Solenoide
  if (distancia - setpoint < -0.01) {
    digitalWrite(solenoidPin, LOW);
    estadoValve = "OFF";
  }
  else {
    digitalWrite(solenoidPin, HIGH);
    estadoValve = "ON";
  }

  //Sensor Ultrassonico
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duracao = pulseIn(echo, HIGH);
  distancia = duracao / 58; //distancia em cm

  //Enviar dados ao Blynk
  Blynk.virtualWrite(V1, estadoValve);
  Blynk.virtualWrite(V2, distancia);
  
}

BLYNK_WRITE(V3)
{
  setpoint = param.asFloat(); // assigning incoming value from pin V1 to a variable

  // process received value
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  //pinMode
  pinMode(solenoidPin, OUTPUT);//Sets the pin as an output
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, wifi, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, wifi, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);

  //Sincronizar VirtualPin
  Blynk.syncVirtual(3);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
