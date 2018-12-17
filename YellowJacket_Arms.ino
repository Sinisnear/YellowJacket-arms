#include <ShiftOutX.h>
#include <ShiftPinNo.h>
#include <PololuMaestro.h>

/* On boards with a hardware serial port available for use, use
that port to communicate with the Maestro. For other boards,
create a SoftwareSerial object using pin 10 to receive (RX) and
pin 11 to transmit (TX). */
#ifdef SERIAL_PORT_HARDWARE_OPEN
  #define maestroSerial SERIAL_PORT_HARDWARE_OPEN
#else
  #include <SoftwareSerial.h>
  SoftwareSerial maestroSerial(10, 11);
#endif

// Next, create a Maestro object using the serial port.
MiniMaestro maestro(maestroSerial);

void setup()
{
  // Set the serial baud rate.
  maestroSerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}
