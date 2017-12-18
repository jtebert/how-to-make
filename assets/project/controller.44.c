//
//
// controller.44.c
//
// Control the Overdrive robots with IR light
//
// Julia Ebert
// 2017-12-18
//

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define input(directions,pin) (directions &= (~pin)) // set port direction for input
#define set(port,pin) (port |= pin) // set port pin
#define get(port,pin) ((port) & (pin))
#define clear(port,pin) (port &= (~pin)) // clear port pin

// For sending data
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
//#define bit_delay_time 102 // bit delay (microsec)for 9600 with overhead
#define bit_delay_time 100 // bit delay (microsec)for 9600 with overhead
//#define bit_delay() _delay_us(bit_delay_time) // RS232 bit delay
#define bit_delay() _delay_ms(bit_delay_time)

// Start/stop buttons
#define start_btn_port PORTB
#define start_btn_pins PINB
#define start_btn_direction DDRB
#define start_btn_pin (1 << PB2)
#define stop_btn_port PORTA
#define stop_btn_pins PINA
#define stop_btn_direction DDRA
#define stop_btn_pin (1 << PA7)

// LEDs
#define led_port PORTA
#define led_direction DDRA
#define start_led_pin (1 << PA3)
#define stop_led_pin (1 << PA2)
#define ir_led_pin (1 << PA1)

// Control state constants (which control signal to send)
#define STOP 1
#define START 2
#define NONE 3

#define SIGNAL_BYTE (1 << 3) | (1 << 0)
#define START_BYTE SIGNAL_BYTE | (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4)
#define STOP_BYTE SIGNAL_BYTE
#define TEST_BYTE (1 << 7) | (1 << 5) | (1 << 3) | (1 << 1)
#define FULL_BYTE 0xFF  // Always on byte for simple control

// Control states/button readings set by timer interrupt
#define UP 0
#define DOWN 1
uint8_t control_state = NONE;
uint8_t did_change = 1;
uint8_t start_btn_prev_val = 0;
uint8_t stop_btn_prev_val = 0;
uint8_t start_btn_state = UP;
uint8_t stop_btn_state = UP;

uint8_t message_ind = 0;  // Index in the message being sent (8=stop bit)
uint8_t message_val = 0;  // Message being sent
uint8_t is_tx = 0;  // Is message being transmitted?
uint8_t is_start_bit = 1;
uint16_t ticks = 0;


void put_char(volatile unsigned char *port, unsigned char pin, char txchar) {
    // send character in txchar on port pin
    //    Sent as: [start bit] bits 0 -> 7 [stop bit]
    
    // start bit
    clear(*port,pin);
    bit_delay();
    
    // Send txchar (byte)
    int i;
    for (i=0; i<8; i++) {
        if (bit_test(txchar, i)) {
            set(*port, pin);
        } else {
            clear(*port, pin);
        }
        bit_delay();
    }
    
    // stop bit
    set(*port,pin);
    bit_delay();
    
    // char delay
    bit_delay();
}

ISR(TIM0_COMPA_vect) {
    // Timer interrupt to check button presses
    // Doing as interrupt means that button presses/control state changes will
    // occur even if occur during blocking put_char()
    
    // MESSAGE TRANSMISSION
    ticks++;
    ticks = ticks % 5;
    if (ticks == 0) {
        if (is_tx) {
            if (is_start_bit) {
                // Send start bit
                set(led_port, ir_led_pin);
                //set(led_port, stop_led_pin);
                message_ind = 0;
                is_start_bit = 0;
            } else if (message_ind == 8) {
                // Send stop bit
                set(led_port, ir_led_pin);
                //set(led_port, stop_led_pin);
                message_ind = 0;
                is_start_bit = 1;
            } else {
                // Send message bits
                if (bit_test(message_val, message_ind)) {
                    set(led_port, ir_led_pin);
                    //set(led_port, stop_led_pin);
                } else {
                    clear(led_port, ir_led_pin);
                    //clear(led_port, stop_led_pin);
                }
                message_ind++;
            }
        } else {
            clear(led_port, ir_led_pin);
            //clear(led_port, stop_led_pin);
            message_ind = 0;
            is_start_bit = 1;
        }
    }
    
    // BUTTON CHECKS
    if (get(start_btn_pins, start_btn_pin) != start_btn_prev_val) {
        // Check start button
        //did_change = 1;
        if (start_btn_state == UP) {
            start_btn_state = DOWN;
            if (control_state != START) {
                control_state = START;
            } else {
                control_state = NONE;
                clear(led_port, start_led_pin);
            }
            did_change = 1;
        } else {
            start_btn_state = UP;
        }
        start_btn_prev_val = get(start_btn_pins, start_btn_pin);
    } else if (get(stop_btn_pins, stop_btn_pin) != stop_btn_prev_val) {
        //did_change = 1;
        // Check stop button
        if (stop_btn_state == UP) {
            stop_btn_state = DOWN;
            if (control_state != STOP) {
                control_state = STOP;
            } else {
                control_state = NONE;
            }
            did_change = 1;
        } else {
            stop_btn_state = UP;
        }
        stop_btn_prev_val = get(stop_btn_pins, stop_btn_pin);
    }
}

int main(void) {
    // Main function
    
    // Set clock divider to /1
    CLKPR = (1 << CLKPCE);
    CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);

    // initialize LED pin
    clear(led_port, start_led_pin);
    clear(led_port, stop_led_pin);
    clear(led_port, ir_led_pin);
    output(led_direction, start_led_pin);
    output(led_direction, stop_led_pin);
    output(led_direction, ir_led_pin);

    // Set up buttons and take initial reading
    // Assumes no buttons are pressed during initialization)
    input(start_btn_direction, start_btn_pin);
    input(stop_btn_direction, stop_btn_pin);
    start_btn_prev_val = get(start_btn_pins, start_btn_pin);
    stop_btn_prev_val = get(stop_btn_pins, stop_btn_pin);

    // Set up timer + interrupt (for button debouncing)
    TCCR0B |= ((1 << CS02) | (1 << CS00)); // Timer 0 prescaling - divides by 1024
    TCCR0A |= (1 << WGM01); // Put timer 0 in CTC mode
    OCR0A = 0xFF; // Count 255 cycles for interrupt
    TIMSK0 |= (1 << OCIE0A); // enable timer compare interrupt
    sei(); // Enable global interrupts

    // MAIN LOOP
    while (1) {
        // Change which LEDs on based on button interrupt
        if (did_change) {
            clear(led_port, start_led_pin);
            clear(led_port, stop_led_pin);
            clear(led_port, ir_led_pin);
            did_change = 0;
        }
        // Set LEDs according to state
        if (control_state == START) {
            set(led_port, start_led_pin);
            //message_val = TEST_BYTE;
            message_val = FULL_BYTE;
            is_tx = 1;
            //put_char(&led_port, stop_led_pin, START_BYTE);
            //put_char(&led_port, ir_led_pin, START_BYTE);
        } else if (control_state == STOP) {
            set(led_port, stop_led_pin);
            //message_val = STOP_BYTE;
            message_val = FULL_BYTE;
            is_tx = 1;
            //put_char(&led_port, start_led_pin, STOP_BYTE);
            //put_char(&led_port, ir_led_pin, STOP_BYTE);
        } else {  // control_state == NONE
            is_tx = 0;
        }
    }
    
}
