#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN            6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(180, PIN, NEO_GRB + NEO_KHZ800);

/* Button */
const int buttonShootPin = 7;     // the number of the pushbutton pin
const int buttonArmsPin = 8;     // the number of the pushbutton pin
int buttonShootState = LOW;
int buttonArmsState = LOW;
int toggle = LOW;

/* This example shows how to smoothly control a single servo on a
Maestro servo controller using the PololuMaestro library. It
assumes you have an RC hobby servo connected on channel 0 of your
Maestro, and that you have already used the Maestro Control
Center software to verify that the servo is powered correctly and
moves when you command it to from the Maestro Control Center
software.

Before using this example, you should go to the Serial Settings
tab in the Maestro Control Center and apply these settings:

* Serial mode: UART, fixed baud rate
* Baud rate: 9600
* CRC disabled

Be sure to click "Apply Settings" after making any changes.

This example also assumes you have connected your Arduino to your
Maestro appropriately. If you have not done so, please see
https://github.com/pololu/maestro-arduino for more details on how
to make the connection between your Arduino and your Maestro. */

#include <PololuMaestro.h>

/* On boards with a hardware serial port available for use, use
that port to communicate with the Maestro. For other boards,
create a SoftwareSerial object using pin 10 to receive (RX) and
pin 11 to transmit (TX). */
#ifdef SERIAL_PORT_HARDWARE_OPEN
  #define maestroSerial SERIAL_PORT_HARDWARE_OPEN
#else
  #include <SoftwareSerial.h>
  SoftwareSerial maestroSerial(12, 11);
#endif

/* Next, create a Maestro object using the serial port.

Uncomment one of MicroMaestro or MiniMaestro below depending
on which one you have. */
MicroMaestro maestro(maestroSerial);
//MiniMaestro maestro(maestroSerial);

void setup()
{
  // Set the serial baud rate.
  maestroSerial.begin(9600);
  // neopixel setup
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(buttonShootPin, INPUT);
  pinMode(buttonArmsPin, INPUT);
  //Serial.begin(9600);
  armsContract();
}
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void armsExtend() {
  // Channel, speed 0 = unlimited, 1 slow, 255 fast
  maestro.setSpeed(0, 0);
  maestro.setSpeed(1, 0);
  maestro.setSpeed(2, 7);
  maestro.setSpeed(3, 128);
  maestro.setSpeed(5, 128); // speed 15
  maestro.setSpeed(11, 10);
  
  maestro.setAcceleration(2, 128);
  // small number is out, big number is in
  // L16 full stroke is 4000-8200 us (microseconds)
  // limit for twisting elbow is 4000-10000us
  // PQ12 full stroke is 4200-8000 us arm 2.5 is 4100-7400
  // top max 8500 low 5600 mid 7100
  // bottom max 2450 low 5600 mid 3900
  
  maestro.setTarget(0, 6000); // lower
  maestro.setTarget(3, 7000); // tip PQ12
  maestro.setTarget(5, 4000); // arm 2.5 PQ12
  delay(1000);
  //maestro.setTarget(2, 10000); // wrist SERVO
  maestro.setTarget(2, 6800); // wrist SERVO
  maestro.setTarget(10, 6500); // elbow
  maestro.setTarget(11, 7800); // splay
  toggle = HIGH;
}

void armsContract() {
  maestro.setSpeed(0, 0);
  maestro.setSpeed(1, 0);
  maestro.setSpeed(2, 20);
  maestro.setSpeed(3, 128);
  maestro.setSpeed(4, 128); // speed 15
  maestro.setSpeed(5, 128); // speed 15
  maestro.setSpeed(11, 10);
  
  //Set the target of channel 0 to 1500 us, and wait 2 seconds.
  //maestro.setTarget(2, 1800); // wrist SERVO
  maestro.setTarget(2, 4600); // wrist SERVO
  delay(1000);
  
  maestro.setTarget(5, 7800 ); // arm 2.5 PQ12
   //delay(1000);
  maestro.setTarget(3, 4400 ); // tip PQ12 
  //maestro.setTarget(4, 8500 ); // tip top servo
  //maestro.setTarget(5, 5600 ); // tip bottom servo
  //delay(1000);
  maestro.setTarget(0, 4000); // lower
  maestro.setTarget(10, 7900); // elbow
  maestro.setTarget(11, 5200); // splay
  toggle = LOW;
}

// Run the program
void loop()
{
  buttonShootState = digitalRead(buttonShootPin);
  buttonArmsState = digitalRead(buttonArmsPin);
  
  if (buttonShootState == HIGH && toggle == 1) {
    //Neopixel stuff
    for (uint16_t i=0; i < strip.numPixels(); i++ ) {
      strip.setPixelColor(i, 0, 0, 255);    //turn every pixel on
      strip.show();
      delay(1);
      if (i == 179) {
        // white riple animation, 2 on 1 off "wave"
        for (uint16_t j=0; j < strip.numPixels(); j++ ) {
          strip.setPixelColor(j, 50, 50, 255);
          strip.setPixelColor(j-1, 127, 127, 255);
          strip.setPixelColor(j-2, 127, 127, 255);
          strip.setPixelColor(j-3, 0, 0, 50);
          strip.setPixelColor(j-4, 127, 127, 255);
          strip.setPixelColor(j-5, 127, 127, 255);
          strip.setPixelColor(j-6, 0, 0, 50);
          strip.setPixelColor(j-7, 127, 127, 255);
          strip.setPixelColor(j-8, 127, 127, 255);
          strip.setPixelColor(j-9, 0, 0, 50);
          strip.setPixelColor(j-10, 127, 127, 255);
          strip.setPixelColor(j-11, 127, 127, 255);
          strip.setPixelColor(j-12, 0, 0, 0);
          strip.show();
          
          delay(6);
        }
          strip.setPixelColor(180, 0, 0, 0);
          strip.setPixelColor(179, 0, 0, 0);
          strip.setPixelColor(178, 0, 0, 0);
          strip.setPixelColor(177, 0, 0, 0);
          strip.setPixelColor(176, 0, 0, 0);
          strip.setPixelColor(175, 0, 0, 0);
          strip.setPixelColor(174, 0, 0, 0);
          strip.setPixelColor(173, 0, 0, 0);
          strip.setPixelColor(172, 0, 0, 0);
          strip.setPixelColor(171, 0, 0, 0);
          strip.setPixelColor(170, 0, 0, 0);
          strip.setPixelColor(169, 0, 0, 0);
          strip.setPixelColor(168, 0, 0, 0);
          strip.setPixelColor(167, 0, 0, 0);
          
          strip.show();
        //darken();
        //colorWipe(strip.Color(0, 0, 0), 0);
      }
    }
  }

// ARM CONTROL
  Serial.print(buttonArmsState);
  Serial.println("");

  if (buttonArmsState == HIGH && toggle == LOW) {
    delay(150); // let switch settle, primitave debounce
    armsExtend();
  
  } else if (buttonArmsState == HIGH && toggle == HIGH) {
  // CONTRACT
    delay(150); // let switch settle, primitave debounce
    armsContract();
  }
  // end arms button
}
