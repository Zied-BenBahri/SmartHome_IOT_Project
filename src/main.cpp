#define BLYNK_TEMPLATE_ID "TMPL2h3QlEmms"
#define BLYNK_TEMPLATE_NAME "Home AutomationCopy"
#define BLYNK_AUTH_TOKEN "D3wyyON9SinVNlB1JcfX4LN8sHnFCTFl"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <DHT.h>
#define DHTPIN 5  // Assuming your DHT22 data pin is connected to GPIO 5 on the ESP32
#define DHTTYPE DHT22
#define PIR_PIN 14

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Wokwi-GUEST";
char pass[] = "";
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;


#define button1_pin 26


#define relay1_pin 13
#define relay2_pin 12


int relay1_state = 0;
int relay2_state = 0;



//Change the virtual pins according the rooms
#define button1_vpin    V1
#define button2_vpin    V2
#define PIR_VPIN V3



//------------------------------------------------------------------------------
// This function is called every time the device is connected to the Blynk.Cloud
// Request the latest state from the server
BLYNK_CONNECTED() {
  Blynk.syncVirtual(button1_vpin);
  Blynk.syncVirtual(button2_vpin);

}

//--------------------------------------------------------------------------
// This function is called every time the Virtual Pin state change
//i.e when web push switch from Blynk App or Web Dashboard
BLYNK_WRITE(button1_vpin) {
  relay1_state = param.asInt();
  digitalWrite(relay1_pin, relay1_state);
}
//--------------------------------------------------------------------------
BLYNK_WRITE(button2_vpin) {
  int pirValue = digitalRead(PIR_PIN);
  relay2_state = param.asInt();
  if (relay2_state && pirValue)
  {
    digitalWrite(relay2_pin, LOW);
  }
  else
  digitalWrite(relay2_pin, HIGH);

  
}
//--------------------------------------------------------------------------


void setup()
{
  // Debug console
  Serial.begin(115200);
  //--------------------------------------------------------------------
  pinMode(button1_pin, INPUT_PULLUP);
  //--------------------------------------------------------------------
  pinMode(relay1_pin, OUTPUT);
  pinMode(relay2_pin, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  //--------------------------------------------------------------------
  //During Starting all Relays should TURN OFF
  digitalWrite(relay1_pin, HIGH);
  digitalWrite(relay2_pin, HIGH);

  //--------------------------------------------------------------------
  Blynk.begin(auth, ssid, pass);
  //--dht22
  Blynk.virtualWrite(V5, dht.readTemperature());
  Blynk.virtualWrite(V6, dht.readHumidity());
  timer.setInterval(1000L, sendPIRStatus); // Check every second
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  //--------------------------------------------------------------------
  //Blynk.virtualWrite(button1_vpin, relay1_state);
  Blynk.virtualWrite(button2_vpin, relay2_state);
  //Blynk.virtualWrite(button3_vpin, relay3_state);
  //Blynk.virtualWrite(button4_vpin, relay4_state);
  //--------------------------------------------------------------------
}
void sendPIRStatus() {
    int pirValue = digitalRead(PIR_PIN);
    Blynk.virtualWrite(PIR_VPIN, pirValue);
}

void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  
  listen_push_buttons();
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  Blynk.virtualWrite(V5, temperature);
  Blynk.virtualWrite(V6, humidity);

}

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
void listen_push_buttons(){
    //--------------------------------------------------------------------------
    if(digitalRead(button1_pin) == LOW){
      delay(200);
      control_relay(1);
      Blynk.virtualWrite(button1_vpin, relay1_state); //update button state
    }
    //--------------------------------------------------------------------------
    /*else if (digitalRead(button2_pin) == LOW){
      delay(200);
      control_relay(2);
      Blynk.virtualWrite(button2_vpin, relay2_state); //update button state
    }*/
    //--------------------------------------------------------------------------
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM




//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
void control_relay(int relay){
  //------------------------------------------------
  if(relay == 1){
    relay1_state = !relay1_state;
    digitalWrite(relay1_pin, relay1_state);
    Serial.print("Relay1 State = ");
    Serial.println(relay1_state);
    delay(50);
  }
  //------------------------------------------------
  else if(relay == 2){
    relay2_state = !relay2_state;
    digitalWrite(relay2_pin, relay2_state);
    delay(50);
  }
  //----------------------------------------------------
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
