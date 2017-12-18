//
// robot.328p.c
//
// Overdrive robot control
//
// Julia Ebert
// 2017-12-18
//

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
//
// MACROS FOR PIN HANDLING
//

#define output(directions,pin) (directions |= pin) // set port direction for output
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
//#define bit_delay_time 102 // bit delay for 9600 with overhead
//#define bit_delay() _delay_us(bit_delay_time) // RS232 bit delay
//#define half_bit_delay() _delay_us(bit_delay_time/2) // RS232 half bit delay
#define bit_delay_time 100 // bit delay for 9600 with overhead
#define bit_delay() _delay_ms(bit_delay_time) // RS232 bit delay
#define half_bit_delay() _delay_ms(bit_delay_time/2) // RS232 half bit delay
#define char_delay() _delay_ms(10) // char delay

////////////////////////////////////////////////////////////////////////////////
//
// SETUP/DEFINE PINS
//

// Green debug LED
#define g_led_port PORTD
#define g_led_direction DDRD
#define g_led_pin (1 << PD7)

// Blue debug LED
#define b_led_port PORTB
#define b_led_direction DDRB
#define b_led_pin (1 << PB0)

// Motors
#define motor_port PORTB
#define motor_direction DDRB
#define r_motor_pin (1 << PB1)
#define l_motor_pin (1 << PB2)

// IR LEDs
#define ir_led_port PORTD
#define ir_led_direction DDRD
#define ir_led_pin (1 << PD6)

// IR Phototransistors
#define f_ir_pt_adc 0  // ADC0
#define r_ir_pt_adc 1  // ADC1

// Serial communication (FTDI header)
#define serial_port PORTD
#define serial_direction DDRD
#define serial_pin_out (1 << PD1)

// Internal clock (updated in ISR)
volatile uint32_t ticks;
#define SECOND 76  // Ticks per second

////////////////////////////////////////////////////////////////////////////////
//
// CONTROL PARAMETERS
//

// Control states
#define SPEED_STOP 0
#define SPEED_SLOW 1
#define SPEED_FULL 2
uint8_t mvmt_state = SPEED_STOP;  // Control state for movement
#define speedup_delay 0.2*SECOND  // Time (ticks) after IR last seen to speed back up/start moving
#define CTRL_INIT 0  // Waiting for start IR signal
#define CTRL_STARTUP 1  // Initial run (motors on) after start signal
#define CTRL_RUN 2  // Normal running state
uint8_t ctrl_state = CTRL_INIT;
#define startup_dur 1*SECOND
#define run_dur 15*SECOND

// Motors
#define base_motor_rate 30  // Default % duty cycle of motors
#define slow_motor_rate 10  // % duty cycle when underneath robots

// Light detection
#define ir_thresh 255  // [0-1023] IR value to determine whether robot is above
#define ir_comp_interval 0.3 // seconds between IR light checks (for velocity)
#define ir_vel_thresh 250 // IR change per sec to consider overhead robot moving
#define num_ir_samples 100  // Number of samples to average to get IR value

// Start/stop signal values
#define SIGNAL_BYTE (1 << 3) | (1 << 0)
#define START_BYTE SIGNAL_BYTE | (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4)
#define STOP_BYTE SIGNAL_BYTE
#define TEST_BYTE 0  // 11111111

uint16_t f_curr_ir = 0;  // Check light every cycle
uint16_t r_curr_ir = 0;  // Check light every cycle
uint8_t message_ind = 0;  // Index of bit being read
uint8_t leading_bit = 1;  // Is the data a leading bit?
uint8_t new_message_ready = 0;  // Has a complete new message been received
uint8_t message_val = 255;  // Variable to read message into

////////////////////////////////////////////////////////////////////////////////
//
// HELPER FUNCTIONS
//

void get_char(volatile unsigned char *pins, unsigned char pin, char *rxbyte) {
    // read character into rxbyte on pins pin
    // Order: bit 7 -> bit 0
    // TODO: A non-blocking alternative?
    
    *rxbyte = 0;
    while (pin_test(*pins,pin));  // Wait for start bit

    // delay to middle of first data bit
    half_bit_delay();
    bit_delay();
   
    uint8_t i;
    for (i=7; i>=0; i--) {
        if pin_test(*pins, pin) {
            *rxbyte |= (1 << i);
        }
        // Otherwise stays 0
    }

    // wait for stop bit
    bit_delay();
    half_bit_delay();
}

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

ISR(TIMER0_COMPA_vect) {
    // Timer0 interrupt
    // Used to update clock ticks
    // TODO: use to send messages?
    ticks++;
    
    // Read 1 bit from front IR every tick
    // (Need to figure out the tick rate to make this work)
    if (ticks % 5 == 0) {
        uint8_t bit_val = f_curr_ir > ir_thresh;
        if (bit_val && leading_bit) {
            // Message start detected
            leading_bit = 0;
            message_ind = 0;
            message_val = 0;
        } else if (!leading_bit && message_ind < 8) {
            // In the midst of message
            if (bit_val) {
                message_val |= (1 << message_ind);
            }  // Otherwise bit is 0
            message_ind++;
        } else if (message_ind == 8) {
            // End bit reached
            if (bit_val) {
                // End bit value should be high
                new_message_ready = 1;
                // new_message_ready will be set back to 0 when read in main loop
            }
        }
    }
}

void set_motors(uint8_t motor_rate) {
    // Set R and L motors to the given motor rate (% duty cycle, 0-100)
    uint16_t motor_duty = 65535 * (motor_rate/100.0);
    OCR1A = motor_duty;
    OCR1B = motor_duty;
}

uint16_t sample_light(uint8_t adc) {
    // Measure light level from phototransistors (multiple samples)
    
    // Set pin to sample from
    ADMUX = adc;
    
    // Read once and throw away to enact change channel?
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    
    // Collect and average multiple samples
    uint32_t accum = 0;
    uint16_t i;
    for (i=0; i<num_ir_samples; i++) {
        // Initiate conversion
        ADCSRA |= (1 << ADSC);
        // Wait for completion
        while (ADCSRA & (1 << ADSC));
        // Accumulate result
        accum += ADC;
    }
    // Get average light sample
    return 1023 - (accum/num_ir_samples);
}

uint8_t is_robot_over(uint16_t f_ir, uint16_t r_ir) {
    // Check if robot is overhead
    return f_ir > ir_thresh || r_ir > ir_thresh;
}

uint8_t is_robot_over_moving(uint16_t f_curr_ir, uint16_t r_curr_ir, uint16_t f_prev_ir, uint16_t r_prev_ir) {
    // Check if robot overhead is moving
    uint16_t f_ir_vel = abs(f_curr_ir - f_prev_ir) * ir_comp_interval;
    uint16_t r_ir_vel = abs(r_curr_ir - r_prev_ir) * ir_comp_interval;
    return f_ir_vel > ir_vel_thresh || r_ir_vel > ir_vel_thresh;
}

////////////////////////////////////////////////////////////////////////////////
//
// MAIN FUNCTION
//

int main(void) {
    // Set clock divider to /1
    CLKPR = (1 << CLKPCE);
    CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
    
    // Set up timer for ticks
    // I think this is ~76 ticks/s: 20 MHz / 1024 prescale / 256 counter ~= 76
    TCCR0A |= (1 << WGM01);  // Set timer mode to CTC
    OCR0A = 0xFF;  // Value to count to (256)
    TIMSK0 |= (1 << OCIE0A);  // Set the ISR COMPA vect
    sei();  // Enable interrupts
    TCCR0B |= (1 << CS02) | (1 << CS00);  // Set prescaler to 1024, start timer

    // Initialize PWM for motor
    ICR1 = 0xFFFF;  // Set top bit to 16
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1);  // set non-inverting mode
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12) | (1 << WGM13);  // Set fast PWM mode using ICR1 as TOP
    TCCR1B |= (1 << CS10);  // Start timer with no prescaler

    // Initialize motor pins
    clear(motor_port, r_motor_pin);
    output(motor_direction, r_motor_pin);
    clear(motor_port, l_motor_pin);
    output(motor_direction, l_motor_pin);

    // Initialize green/blue debug LED
    clear(g_led_port, g_led_pin);
    output(g_led_direction, g_led_pin);
    clear(b_led_port, b_led_pin);
    output(b_led_direction, b_led_pin);
    
    // Initialize IR LED (turn on)
    clear(ir_led_port, ir_led_pin);
    output(ir_led_direction, ir_led_pin);

    // Initialize analog to digital (ADC) (for phototransistors)
    //ADMUX = (0 << REFS1) | (0 << REFS0);  // Use VCC for reference voltage
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // prescaler /128; enable
    // Set other bits (MUX[3:0]) in light sampling function to select input pin
    
    
    // Initialize variables
    ticks = 0;
    uint8_t robot_over = 0;  // Is there a robot above?
    //uint8_t robot_over_moving = 0;  // Is robot above moving?
    //uint16_t f_prev_ir = 0;  // Compare to old measure to check velocity
    //uint16_t r_prev_ir = 0;  // Compare to old measure to check velocity
    //uint32_t last_vel_sample = 0;  // Ticks of last IR sample for velocity
    uint8_t prev_mvmt_state;
    uint32_t ir_last_seen = ticks;  // Time when IR last seen
    uint32_t startup_timer = 0;
    uint32_t run_timer = 0;
    
    // Initialize serial output (FTDI)
    set(serial_port, serial_pin_out);
    output(serial_direction, serial_pin_out);

    // Main loop
    while (1) {
        
        // Get IR readings
        f_curr_ir = sample_light(f_ir_pt_adc);
        r_curr_ir = sample_light(r_ir_pt_adc);
        
        prev_mvmt_state = mvmt_state;
        if (ctrl_state == CTRL_INIT) {
            clear(ir_led_port, ir_led_pin);
            set(b_led_port, b_led_pin);
            mvmt_state = SPEED_STOP;
            if (is_robot_over(f_curr_ir, r_curr_ir)) {
                // Start when IR detected for first time
                set(ir_led_port, ir_led_pin);
                clear(b_led_port, b_led_pin);
                ctrl_state = CTRL_STARTUP;
                startup_timer = ticks;
            }
        } else if (ctrl_state == CTRL_STARTUP) {
            // Run for a certain duration before paying attention to IR and control signals
            mvmt_state = SPEED_FULL;
            if (ticks > startup_timer + startup_dur) {
                ctrl_state = CTRL_RUN;
                run_timer = ticks;
            }
        } else if (ctrl_state == CTRL_RUN) {
            // Check if robot is overhead        
            robot_over = is_robot_over(f_curr_ir, r_curr_ir);
            
            // TODO: Check if overhead robot is moving
            
            // Simple version: stop if robot or controller overhead (IR over threshold)
            if (robot_over) {
                // robot overhead = stop moving
                ir_last_seen = ticks;
                mvmt_state = SPEED_STOP;
                // TODO: Option for slow speed if robot overhead and moving
            } else if (mvmt_state == SPEED_STOP && ticks > ir_last_seen + speedup_delay) {
                // End of delay after overhead robot left
                mvmt_state = SPEED_FULL;
            }
            
            // After running for run_dur, reset to init state for next run
            if (ticks > run_timer + run_dur) {
                ctrl_state = CTRL_INIT;
            }
        }
        
        // Change motors if mvmt_state change
        if (mvmt_state != prev_mvmt_state) {
            if (mvmt_state == SPEED_STOP) {
                set_motors(0);
            } else if (mvmt_state == SPEED_FULL) {
                set_motors(base_motor_rate);
            } else if (mvmt_state == SPEED_SLOW) {
                set_motors(slow_motor_rate);
            }
        }
        
        
        // Check velocity and update previous IR sample
        /*if (ticks > last_vel_sample+ir_comp_interval*SECOND) {
            robot_over_moving = is_robot_over_moving(f_curr_ir, r_curr_ir, f_prev_ir, r_prev_ir);
            f_prev_ir = r_curr_ir;
        }
        if (robot_over) {
            if (robot_over_moving) {
                // Start slowdown if robot moving overhead
                set_motors(slow_motor_rate);
                start_slowdown = ticks;
            } else {
                // Stopped robot overhead = stop myself
                set_motors(0);
            }
        } else {
            if (ticks > start_slowdown+slow_dur*SECOND) {
                // End slowdown is no robot detected overhead for awhile
                set_motors(base_motor_rate);
            }
            // Check for control signal
        }*/
        
        /*if (new_message_ready) {
            if (message_val != TEST_BYTE) {
                set(g_led_port, g_led_pin);
            } else {
                clear(g_led_port, g_led_pin);
            }
            new_message_ready = 0;
        }*/
    }
}
