#include <Arduino.h>
#include <ezButton.h>
#include <Servo.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <U8g2lib.h>
#include <Wire.h>

//DEFINING PINS
#define DHTPIN 13 //TEMP/HUM SENSOR PIN 
#define LED_WHITE 12 // WHITE LED PIN
#define LED_BLUE 11 // BLUE LED PIN
#define LED_RED 10 // RED LED PIN
#define BTN_BLUE 9 // PUSHBUTTON BLUE PIN
#define BTN_YELLOW 8 // PUSHBUTTON YELLOW PIN
#define BTN_RED 7 // PUSHBUTTON RED PIN
#define TOGGLE_SWITCH 2 // TOGGLE SWITCH PIN

#define DHTTYPE DHT22   // DHT 22

Servo myservo;
DHT dht = DHT(DHTPIN, DHTTYPE);

const int DEBOUNCE_TIME = 100;
const int DEBOUNCE_TIME_DHT = 2000;
unsigned long button_millis;
unsigned long button_millis2;
bool button_pushed = false;
bool button_pushed2 = false;
int push_count = 0;
bool port_open = false;
unsigned long dht_millis;

U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 19, /* data=*/ 18, /* reset=*/ U8X8_PIN_NONE);

void control_led(){
  unsigned long starttime = millis();
  // CHECK STATUS TOGGLE SWITCH AND EXECUTE CODE WHEN HIGH
  if (digitalRead(TOGGLE_SWITCH) == HIGH){
    // COUNTER GOES UP BY 1 WHEN BLUE BUTTON PRESSED
    if (digitalRead(BTN_BLUE) == HIGH) {
      button_millis = starttime;
      button_pushed = true;
    }
    if(button_pushed){
      if ((starttime - button_millis) >= DEBOUNCE_TIME){
        push_count++;
        //MAKE COUNTER 0 WHEN REACHES 4
          if (push_count == 4){
            push_count = 0;
          }
        button_pushed = false;
      }
    }
    if (push_count == 0) {
      digitalWrite(LED_BLUE, HIGH);
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_WHITE, HIGH);
    }
    if (push_count == 1) {
      digitalWrite(LED_BLUE, HIGH);
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_WHITE, LOW);
    }
    if (push_count == 2) {
      digitalWrite(LED_BLUE, LOW);
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_WHITE, HIGH);
    }
    if (push_count == 3) {
      digitalWrite(LED_BLUE, LOW);
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_WHITE, LOW);
    }

  } else {
  // IF TOGGLE SWITCH STATUS LOW TURN EVERYTHING OFF
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_WHITE, LOW);
    myservo.write(90);
  }
}

void control_servo(){
unsigned long starttime2 = millis();
  if (digitalRead(BTN_YELLOW) == HIGH) {
    button_millis2 = starttime2;
    button_pushed2 = true;
  }
  if(button_pushed2){
    if ((starttime2 - button_millis2) >= DEBOUNCE_TIME){
      if (port_open){
        myservo.write(90);
        port_open = false;
      } else {
        myservo.write(30);
        port_open = true;
      }
      button_pushed2 = false;
    }
  }
}

void control_dht(){
unsigned long delay_time = millis();
  if (delay_time >= (dht_millis + DEBOUNCE_TIME_DHT)){
    // Read the humidity in %:
    float h = dht.readHumidity();
    // Read the temperature as Celsius:
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again):
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  
  u8g2.firstPage();
  u8g2.setCursor(0, 22);
  do {
    u8g2.setFont(u8g2_font_tinytim_tf);
    //u8g2.print("Temperatuur: "); 
    u8g2.print(t);
  } while ( u8g2.nextPage() );
    dht_millis = millis();
  }
}

void setup() {
  pinMode(LED_WHITE, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BTN_BLUE, INPUT);
  pinMode(BTN_YELLOW, INPUT);
  pinMode(BTN_RED, INPUT);
  pinMode(TOGGLE_SWITCH, INPUT);

  myservo.attach(6);  // attaches the servo on pin 9 to the servo object
  myservo.write(90);

  dht.begin();
  u8g2.begin();
}

void loop() {
  control_led();
  control_servo();
  control_dht();
}



