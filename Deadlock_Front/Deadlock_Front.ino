/** Deadlock's front LED panel

   @author Robert Karpinski
   @date 30/01/2019
   @board Adafruit Trinket M0

   LEDs fit in a 350x30mm panel.

*/
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SleepyDog.h>
#include <math.h>

/** Settings */
#define BAUD_RATE       (115200)
#define PIXEL_W         (24) // pixels wide
#define PIXEL_H         (5) // pixels high
#define MIN_PULSE       (1000) // ms long
#define MAX_PULSE       (2000) // ms long
#define ERR_PULSE       (30*MAX_PULSE) // ms long
#define FRAME_RATE      (50000)

/** Pin Map */
#define PIN_SPARE       (0) // used for seeding rand()
#define PIN_NEOPIXEL    (4)
#define PIN_B_LED       (13)
#define PIN_RC_RX       (3)

/** Globals */
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXEL_W * PIXEL_H, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
volatile unsigned long last_activity = 0x00UL;
volatile unsigned long pulse_length = 0x00UL;
enum _states {s_failsafe, s_breathe_red, s_larson_scan, s_orange_term, s_rainbow_sparkles} current_state;
void breathe(char colour);
void rainbow_sparkles(void);


void setup(void) {

  Serial.begin(BAUD_RATE);
  Serial.println("Deadlock Bling starting...");

  int wdt_timeout = Watchdog.enable(1000);
  Serial.print("Watchdog timeout set for: ");
  Serial.print(wdt_timeout, DEC);
  Serial.println("ms.");

  pinMode(PIN_NEOPIXEL, OUTPUT);
  pinMode(PIN_B_LED, OUTPUT);
  pinMode(PIN_RC_RX, INPUT);

  randomSeed(analogRead(A2));

  digitalWrite(PIN_NEOPIXEL, LOW);
  digitalWrite(PIN_B_LED, LOW);
  attachInterrupt(digitalPinToInterrupt(PIN_RC_RX), rc_handler, CHANGE);

  current_state = s_failsafe;
  //current_state = s_rainbow_sparkles;

  pixels.begin();

}

void loop(void) {
  unsigned int loop_timer = micros();

  if ((micros() - last_activity) > ERR_PULSE)
  {
    Serial.println("No RX pulses!");
    current_state = s_failsafe;
    pulse_length = 0x00UL;
  }

  // monster switch case, where each case should draw a frame
  switch (current_state)
  {
    case s_breathe_red:
      breathe('R'); // breathe red
      break;
    case s_larson_scan:
      break;
    case s_orange_term:
      break;
    case s_rainbow_sparkles:
      rainbow_sparkles();
      break;
    case s_failsafe:
    default:
      breathe('G'); // breathe green
  }
  pixels.show(); // write out

  Watchdog.reset();

  while ((micros() - loop_timer) < FRAME_RATE);
}

/** Servo signal grabber

    interrupts on changes to PIN_RC_RX
    saves a timestamp on rising edges
    calculates the duration and saves to a global on falling edges
    on each edge updates a global activity counter
*/
void rc_handler(void) {
  static byte last_status = LOW;
  static unsigned long last_time = 0x00UL;

  if (last_status == LOW && digitalRead(PIN_RC_RX) == HIGH)
  {
    // seen rising edge
    last_time = micros();
    last_status = HIGH;
  }
  else if (last_status == HIGH && digitalRead(PIN_RC_RX) == LOW)
  {
    // seen falling edge
    pulse_length = constrain((micros() - last_time), (MIN_PULSE * 0.9), (MAX_PULSE * 1.1));
    last_status = LOW;
  }

  last_activity = micros();

  return;
}

/** Breath a colour
* Code taken from here: https://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
*/
void breathe(char colour) {
  const unsigned int duration = 60;
  const float min_val = 0.18393972; // 1-e
  const float max_val = 100.833798; // 255 / (e - (1/e))
  
  static unsigned int animation_step = 0x00;

  float brightness = (exp(sin((float(animation_step)/duration)*PI)) - min_val) * max_val;

  // consider using gamma values?
//  Serial.print(animation_step, DEC);
//  Serial.print("\t");
//  Serial.println(brightness);
  
  brightness = constrain(brightness, 0, 255);

  if(animation_step == 2*duration) animation_step = 0x00;
  else animation_step += 1;

  if(colour == 'R') pixels.fill(pixels.Color(brightness,0,0));
  if(colour == 'G') pixels.fill(pixels.Color(0,brightness,0));
  if(colour == 'B') pixels.fill(pixels.Color(0,0,brightness));

  return;
}

/** Generates random sparkles
    Inspired by: https://github.com/pimoroni/unicorn-hat
*/
void rainbow_sparkles(void) {
  const unsigned int pixel_count = 8;
  unsigned int x, y, r, g, b;

  for (int i = 0; i < pixel_count; i++)
  {
    x = random(0, PIXEL_W);
    y = random(0, PIXEL_H);
    r = random(0, 255);
    g = random(0, 255);
    b = random(0, 255);
    pixels.setPixelColor(((y * PIXEL_W) + x), r, g, b);
  }
  return;
}
