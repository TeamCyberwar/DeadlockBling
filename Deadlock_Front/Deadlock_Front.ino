/** Deadlock's front LED panel
 * 
 * @author Robert Karpinski
 * @date 30/01/2019
 * @board Adafruit Trinket M0 
 *  
 */
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SleepyDog.h>

/** Settings */
#define PIXEL_W         (16) // pixels wide
#define PIXEL_H         (6) // pixels high
#define MIN_PULSE       (1000) // ms long
#define MAX_PULSE       (2000) // ms long
#define ERR_PULSE       (30*MAX_PULSE) // ms long
#define FRAME_RATE      (100)

/** Pin Map */
#define PIN_NEOPIXEL    (4)
#define PIN_B_LED       (13)
#define PIN_RC_RX       (3)

/** Globals */
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXEL_W * PIXEL_H, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
volatile unsigned long last_activity = 0x00UL;
volatile unsigned long pulse_length = 0x00UL;


void setup(void) {

  pinMode(PIN_NEOPIXEL, OUTPUT);
  pinMode(PIN_B_LED, OUTPUT);
  pinMode(PIN_RC_RX, INPUT);

  digitalWrite(PIN_NEOPIXEL, LOW);
  digitalWrite(PIN_B_LED, LOW);
  attachInterrupt(digitalPinToInterrupt(PIN_RC_RX), rc_handler, CHANGE);
  
  pixels.begin();
    
}

void loop(void) {
  unsigned int loop_timer = micros();

  if((micros() - last_activity) > ERR_PULSE) Serial.println("No RX pulses!");

  // some large switch case
  // each case should draw a frame

  //cases:
  // breathing red LEDs
  // breathing green LEDs
  // larson scanner
  // white flood
  // orange flood
  // rainbow cycle



  while((micros() - loop_timer) < 100000);
}

void rc_handler(void) {
  static byte last_status = LOW;
  static unsigned long last_time = 0x00UL;

  if(last_status == LOW && digitalRead(PIN_RC_RX) == HIGH)
  {
    // seen rising edge
    last_time = micros();
    last_status = HIGH;
  }
  else if(last_status == HIGH && digitalRead(PIN_RC_RX) == LOW)
  {
    // seen falling edge
    pulse_length = constrain((micros() - last_time), (MIN_PULSE * 0.9), (MAX_PULSE * 1.1));
    last_status = LOW;
  }

  last_activity = micros();
  
  return;
}
