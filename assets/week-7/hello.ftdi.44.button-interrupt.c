//
//
// hello.ftdi.44.button-interrupt.c
//
// Cycle through 3 LED states when the button is pressed.
//
// Julia Ebert
// 2017-10-24
//
// With interrupt design help from here:
// https://embeddedthoughts.com/2016/06/10/attiny85-debounce-your-pushbuttons/


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define input(directions,pin) (directions &= (~pin)) // set port direction for input
#define set(port,pin) (port |= pin) // set port pin
#define get(port,pin) ((port) & (pin))
#define clear(port,pin) (port &= (~pin)) // clear port pin

// Button
#define button_port PORTB
#define button_pins PINB
#define button_direction DDRB
#define button_pin (1 << PB2)
#define button_interrupt (1 << PCIE1)
#define button_interrupt_pin (1 << PCINT10)

// LEDs
#define led_port PORTA
#define led_direction DDRA
#define led1_pin (1 << PA7)
#define led2_pin (1 << PA3)
#define led3_pin (1 << PA2)
uint8_t led_state = 0;
uint8_t previous_reading = 0;
uint8_t button_was_pressed = 0;

#define max_buffer 25

ISR(TIM0_COMPA_vect) {
    //
    // Timer interrupt
    //
    if (get(button_pins, button_pin) != previous_reading) {
        if (!button_was_pressed) {
            led_state ++;
            led_state = led_state % 4;
            button_was_pressed = 1;
        } else {
            button_was_pressed = 0;
        }
        previous_reading = get(button_pins, button_pin);
    }
}

int main(void) {
    //
    // main
    //

    // Set clock divider to /1
    CLKPR = (1 << CLKPCE);
    CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);

    // initialize LED pin
    clear(led_port, led1_pin);
    clear(led_port, led2_pin);
    clear(led_port, led3_pin);
    output(led_direction, led1_pin);
    output(led_direction, led2_pin);
    output(led_direction, led3_pin);

    // Set up button
    set(button_port, button_pin);
    input(button_direction, button_pin);

    // Set up timer + interrupt (for button debouncing)
    TCCR0B |= ((1 << CS02) | (1 << CS00)); // Timer 0 prescaling - divides by 1024 */
    TCCR0A |= (1 << WGM01); // Put timer 0 in CTC mode
    OCR0A = 100; // Count 100 cycles for interrupt
    TIMSK0 |= (1 << OCIE0A); // enable timer compare interrupt
    sei(); // Enable global interrupts

    // MAIN LOOP
    while (1) {
        // Change which LEDs on based on button interrupt
        if (button_was_pressed) {
            clear(led_port, led1_pin);
            clear(led_port, led2_pin);
            clear(led_port, led3_pin);
        }
        // Set LEDs according to state
        if (led_state >= 1) {
            set(led_port, led1_pin);
        }
        if (led_state >= 2) {
            set(led_port, led2_pin);
        }
        if (led_state >= 3) {
            set(led_port, led3_pin);
        };
    }
}
