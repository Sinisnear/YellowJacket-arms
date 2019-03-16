#include <PololuMaestro.h>
#include <Adafruit_NeoPixel.h>

/* NeoPixel */
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN 6
// set number of (neopxels in chain, PIN, pixel_type + speed)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(180, PIN, NEO_GRB + NEO_KHZ800);

#ifdef SERIAL_PORT_HARDWARE_OPEN
  #define maestroSerialB SERIAL_PORT_HARDWARE_OPEN
  #define maestroSerialL SERIAL_PORT_HARDWARE_OPEN
  #define maestroSerialR SERIAL_PORT_HARDWARE_OPEN
#else
  #include <SoftwareSerial.h>
  SoftwareSerial maestroSerialB(11, 12);
  SoftwareSerial maestroSerialL(7, 8);
  SoftwareSerial maestroSerialR(9, 10);
#endif

/* button */
const int buttonShootPin = 3;     // the number of the pushbutton pin
const int buttonArmsPin = 4;     // the number of the pushbutton pin
int buttonShootState = LOW;
int buttonArmsState = LOW;
int toggle = LOW;

// Next, create a Maestro object using the serial port.
MicroMaestro maestroB(maestroSerialB); //Backpack
MiniMaestro maestroL(maestroSerialL); //Arm left
MiniMaestro maestroR(maestroSerialR); //Arm right

void setup()
{
  // Set the serial baud rate.
  maestroSerialB.begin(9600);
  maestroSerialL.begin(9600);
  maestroSerialR.begin(9600);
  // neopixel setup
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(buttonShootPin, INPUT);
  pinMode(buttonArmsPin, INPUT);
  
  // serial output for serial monitor
  //Serial.begin(9600);
  
  armsContract();
}

void armsExtend() {
  // Channel, speed 0 = unlimited, 1 slow, 255 fast
  // arm right
  maestroR.setSpeed(0, 7); // wrist
  maestroR.setSpeed(1, 0); // lower
  maestroR.setSpeed(2, 128); // arm 2.5 PQ12
  maestroR.setSpeed(3, 128); // tip PQ12
  // arm left
  maestroL.setSpeed(0, 7); // wrist
  maestroL.setSpeed(1, 0); // lower
  maestroL.setSpeed(2, 128); // arm 2.5 PQ12
  maestroL.setSpeed(3, 128); // tip PQ12
  // backpack
  maestroB.setSpeed(0, 10); // splay
  maestroB.setSpeed(1, 128); // elbow R 
  maestroB.setSpeed(2, 128); // elbow L
  maestroB.setSpeed(3, 0); // wing R
  maestroB.setSpeed(4, 0); // wing L
  maestroB.setSpeed(5, 10); // TopShell R
  maestroB.setSpeed(6, 10); // TopShell L
  maestroR.setAcceleration(1, 128);
  maestroL.setAcceleration(1, 128);
  
  // small number is out, big number is in
  // L16 full stroke is 4000-8200 us (microseconds)
  // limit for twisting elbow is 4000-10000us
  // PQ12 full stroke is 4200-8000 us arm 2.5 is 4100-7400
  // top max 8500 low 5600 mid 7100
  // bottom max 2450 low 5600 mid 3900
  
  maestroR.setTarget(1, 5900); // lower
  maestroL.setTarget(1, 6100); // lower
  maestroB.setTarget(0, 7800); // splay new
  delay(150); // splay new
  maestroR.setTarget(3, 4400 ); // tip PQ12
  maestroL.setTarget(3, 4400 ); // tip PQ12
  maestroR.setTarget(2, 7800 ); // arm 2.5 PQ12
  maestroL.setTarget(2, 7800 ); // arm 2.5 PQ12
  delay(300); // old delay 1000
      //maestroR.setTarget(0, 10000); // wrist R SERVO
  maestroR.setTarget(0, 10000); // wrist R SERVO
  maestroL.setTarget(0, 8500); // wrist L SERVO
  maestroB.setTarget(1, 6300); // elbow R
  maestroB.setTarget(2, 6300); // elbow L
  maestroB.setTarget(5, 3850); // TopShell R
  maestroB.setTarget(6, 3850); // TopShell L
  // old splay
  delay(1500); // so that wings go out last
  maestroB.setTarget(3, 6300); // wing R/L
  toggle = HIGH;
}

void armsContract() {
// Channel, speed 0 = unlimited, 1 slow, 255 fast
  // arm right
  maestroR.setSpeed(0, 20); // wrist
  maestroR.setSpeed(1, 0); // lower
  maestroR.setSpeed(2, 128); // arm 2.5 PQ12
  maestroR.setSpeed(3, 128); // tip PQ12
  // arm left
  maestroL.setSpeed(0, 20); // wrist
  maestroL.setSpeed(1, 0); // lower
  maestroL.setSpeed(2, 128); // arm 2.5 PQ12
  maestroL.setSpeed(3, 128); // tip PQ12
  // backpack
  maestroB.setSpeed(0, 10); // splay
  maestroB.setSpeed(1, 128); // elbow R 
  maestroB.setSpeed(2, 128); // elbow L
  maestroB.setSpeed(3, 0); // wing R/L
  maestroB.setSpeed(5, 10); // TopShell R
  maestroB.setSpeed(6, 10); // TopShell L
  
  maestroB.setTarget(3, 4500); // wingR
  maestroB.setTarget(4, 4500); // wingL
  delay(800); // so that wings go in first
  //maestroB.setTarget(17, 10000); // wrist SERVO
  maestroR.setTarget(0, 8500); // wrist SERVO
  maestroL.setTarget(0, 10000); // wrist SERVO
  delay(800);
  maestroR.setTarget(2, 4000); // arm 2.5 PQ12
  maestroL.setTarget(2, 4000); // arm 2.5 PQ12
   //delay(1000);
  
  maestroR.setTarget(3, 7000); // tip PQ12
  maestroL.setTarget(3, 7000); // tip PQ12
  //delay(1000);
  maestroR.setTarget(1, 4000); // lower
  maestroL.setTarget(1, 4000); // lower
  maestroB.setTarget(5, 5450); // TopShell R
  maestroB.setTarget(6, 5450); // TopShell L
  maestroB.setTarget(1, 8200); // elbow R
  maestroB.setTarget(2, 8200); // elbow L
  maestroB.setTarget(0, 5200); // splay
  toggle = LOW;
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonShootState = digitalRead(buttonShootPin);
  buttonArmsState = digitalRead(buttonArmsPin);
  
  if (buttonShootState == HIGH && toggle == 1) {
    //Neopixel stuff
    for (uint16_t i=0; i < strip.numPixels(); i++ ) {
      strip.setPixelColor(i, 0, 0, 255);    //turn every pixel on
      strip.show();
      delay(1);
      if (i == 179) { // i = the number of neopixels, adjust as necessary
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
      }
    }
  }
  
  // output stuff to serial monitor
  //Serial.print(buttonArmsState);Serial.println();
  
  // ARM CONTROL
  // EXTEND
  if (buttonArmsState == HIGH && toggle == LOW) {
    delay(150); // let switch settle, primitave debounce
    armsExtend();
  
  } else if (buttonArmsState == HIGH && toggle == HIGH) {
  // CONTRACT
    delay(150); // let switch settle, primitave debounce
    armsContract();
  }
  // end arms
}
