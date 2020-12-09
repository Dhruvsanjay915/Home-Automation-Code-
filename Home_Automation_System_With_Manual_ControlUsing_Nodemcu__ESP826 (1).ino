/* The Following Code Has Been Made By Our Team.. Subscribe Our Channel For More Such Videos
    Feel Free To Edit The Code As Per Your Requirements.

    Important Links:
    Boards Manager Link For Nodemcu : https://arduino.esp8266.com/stable/package_esp8266com_index.json

    Components Required:
    Nodemcu(ESP8266)*1
    2 Channel Relay Module(5V)*1
    Push Buttons*2
    Jumper Wires
    PCB(Optional)
*/

#include <BlynkSimpleEsp8266.h>
#define RelayPin1 5
#define RelayPin2 4

#define SwitchPin1 10
#define SwitchPin2 0

#define wifiLed   16   //D0

#define VPIN_BUTTON_1    V1
#define VPIN_BUTTON_2    V2


int toggleState_1 = 1;
int toggleState_2 = 1;


int wifiFlag = 0;

#define AUTH "AUTH TOKEN"                 // You should get Auth Token in the Blynk App.  
#define WIFI_SSID "WIFI NAME"             //Enter Wifi Name
#define WIFI_PASS "WIFI PASSWORD"         //Enter wifi Password

BlynkTimer timer;

void relayOnOff(int relay) {

  switch (relay) {
    case 1:
      if (toggleState_1 == 1) {
        digitalWrite(RelayPin1, LOW); 
        toggleState_1 = 0;
        Serial.println("Device1 ON");
      }
      else {
        digitalWrite(RelayPin1, HIGH); 
        toggleState_1 = 1;
        Serial.println("Device1 OFF");
      }
      delay(100);
      break;
    case 2:
      if (toggleState_2 == 1) {
        digitalWrite(RelayPin2, LOW); 
        toggleState_2 = 0;
        Serial.println("Device2 ON");
      }
      else {
        digitalWrite(RelayPin2, HIGH); 
        toggleState_2 = 1;
        Serial.println("Device2 OFF");
      }
      delay(100);
      break;
    default : break;
  }

}

void with_internet() {
  if (digitalRead(SwitchPin1) == LOW) {
    delay(200);
    relayOnOff(1);
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);   
  }
  else if (digitalRead(SwitchPin2) == LOW) {
    delay(200);
    relayOnOff(2);
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);  
  }
  
}
void without_internet() {
  if (digitalRead(SwitchPin1) == LOW) {
    delay(200);
    relayOnOff(1);
  }
  else if (digitalRead(SwitchPin2) == LOW) {
    delay(200);
    relayOnOff(2);
  }
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
}

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, toggleState_1);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  digitalWrite(RelayPin2, toggleState_2);
}


void checkBlynkStatus() { 

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    digitalWrite(wifiLed, HIGH); //Turn off WiFi LED
  }
  if (isconnected == true) {
    wifiFlag = 0;
    digitalWrite(wifiLed, LOW); //Turn on WiFi LED
  }
}
void setup()
{
  Serial.begin(9600);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  digitalWrite(RelayPin1, toggleState_1);
  digitalWrite(RelayPin2, toggleState_2);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer.setInterval(3000L, checkBlynkStatus);
  Blynk.config(AUTH);
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi Not Connected");
  }
  else
  {
    Serial.println("WiFi Connected");
    Blynk.run();
  }

  timer.run(); // Initiates SimpleTimer
  if (wifiFlag == 0)
    with_internet();
  else
    without_internet();
}
