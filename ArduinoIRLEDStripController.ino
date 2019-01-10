#include <FastLED.h>
#include <RC5.h>

// consts
unsigned const int IR_SENSOR_PIN = 6;
unsigned const int DATA_PIN = 10;
const unsigned char MY_DESTINATION_NUMBER = 0; // <- the destination ID of the led controler
const unsigned int LED_NB = 60; // because I have 60 leds on my led strip

// RC5 decoder
RC5 rc5(IR_SENSOR_PIN);

// other variables
unsigned int colorIndex = 0;
int prevMessage = -1;

unsigned int r = 0;
unsigned int g = 0;
unsigned int b = 0;

unsigned long startTime = 0;

CRGB led[LED_NB];

// setup
void setup() {
  pinMode(DATA_PIN, OUTPUT);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(led, LED_NB);
}

// loop
void loop() {
  unsigned int frame;
  unsigned long const currentTime = millis();

  // if we have a message that is too old we admit it's corrupted so we move on
  if(currentTime - startTime >= 2000) {
    startTime = 0;
    colorIndex = 0;
  }

  // if there's something
  if (rc5.read(&frame))
  {
    // if it's the first value init the start time
    if(startTime == 0)
      startTime = currentTime;
    
    // delete the first bit
    frame -= 8192; // 2^13

    // decode
    unsigned char dest;
    int value;
    decode(&frame, &dest, &value);

    // if the trame is for us
    if (MY_DESTINATION_NUMBER == dest) {

      // if no message before it's a prev message
      if (prevMessage == -1)
        prevMessage = value;

      // if there was a message before (each RGB value is sent twice)
      else {
        // check the values are the same
        if (value == prevMessage) {

          // modify the good value of the color
          changeColor(value);

          // apply if it is the last color
          if(colorIndex == 2) {
            applyRGBValuesToLeds();
            startTime = 0;  // don't forget tot reset the star time
          }

          // modify the color index
          colorIndex++;
          if (colorIndex == 3) 
            colorIndex = 0;
        }

        // reset prevMessage
        prevMessage = -1;
      }
    }
  }
}

// extract dest and value from the frame
void decode(unsigned int *frame, unsigned char *destination,  int *value)
{
  *value = (*frame & 0xFF);
  *destination = (*frame & 0xF00) >> 8;
}

// change the value pointed by the color index 
void changeColor(int value)
{
  // change the value for the right color
  switch (colorIndex) {
    case 0:
      r = value;
      break;
    case 1:
      g = value;
      break;
    case 2:
      b = value;
      break;
  }
}

// send the RGB values to the led strip
void applyRGBValuesToLeds() {
  for (int i = 0; i < LED_NB; i++)
    led[i] = CRGB(r, g, b);

  FastLED.show();
}

