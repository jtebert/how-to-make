//
// hello.ftdi.44.blink.c
//
// Testing blinking and button presses for my Hello World Board
// Adapted from Neil Gershenfeld's code: http://academy.cba.mit.edu/classes/embedded_programming/hello.arduino.328P.blink.c
//
// Julia Ebert
// 2017-10-25
//

// All this stuff is copied from the test/example program hello.ftdi.44.echo.c
// because I don't know what I'm doing yet.

#include <avr/io.h>
#include <util/delay.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
#define led_delay() _delay_ms(100) // LED delay

#define led_port PORTA
#define led_direction DDRA
#define led1_pin (1 << PA7)
#define led2_pin (1 << PA3)
#define led3_pin (1 << PA2)

int main(void) {
   //
   // main
   //
   // set clock divider to /1
   //
   CLKPR = (1 << CLKPCE);
   CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
   //
   // initialize LED pin
   //
   clear(led_port, led1_pin);
   output(led_direction, led1_pin);
   clear(led_port, led2_pin);
   output(led_direction, led2_pin);
   clear(led_port, led3_pin);
   output(led_direction, led3_pin);
   //
   // main loop
   //
   while (1) {
      set(led_port, led1_pin);
      set(led_port, led2_pin);
      set(led_port, led3_pin);
      led_delay();
      clear(led_port, led1_pin);
      clear(led_port, led2_pin);
      clear(led_port, led3_pin);
      led_delay();
      }
   }
