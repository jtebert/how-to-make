---
layout: post
title:  "Embedded Programming"
image: "week-7/lit-board.jpg"
week: Week 7
---

**Assignment:** Read a microcontroller data sheet and program your board to do something.

<!-- more -->

---

This week it's time to program that [Hello World board from a few weeks ago]({{site.baseurl}}{% post_url 2017-10-04-electronics-design %}). I have three LEDs and a button on my board, so my goal is to make LEDs cycle through when you press the button. I've done C programming on a microcontroller before for the Kilobots and on an Arduino, but both involve an extra layer of abstraction. (Like Arduino, the kilobots also have a [library](https://www.kilobotics.com/docs/index.html).) So my challenge this week will be programming without this library overhead.

It turns out that I already had an FTDI header laying around, since we use it to debug the Kilobots. That means I don't have to leave the comfort of my office for this week's assignment!

To start writing my code, I looked at examples from the class website: [hello.ftdi.44.echo.c](http://academy.cba.mit.edu/classes/embedded_programming/hello.ftdi.44.echo.c) and [hello.arduino.328P.blink.c](http://academy.cba.mit.edu/classes/embedded_programming/hello.arduino.328P.blink.c).

# Blink!

<video loop autoplay muted>
    <source src="{{site.baseurl}}/assets/week-7/blink.mp4" type="video/mp4">
    Your browser does not support the video tag.
</video>

First up, I tested translating the [Arduino blink code](http://academy.cba.mit.edu/classes/embedded_programming/hello.arduino.328P.blink.c) to run on the ATtiny-based Hello World board. In the C code itself, all I had to do was change what pin I was pointing at for my LEDs, and set the clear/output/set for all three LEDs. I was also able to shamelessly steal (er, reuse) the Makefile from the original hello world code and simply change the project name:

- [C code]({{site.baseurl}}/assets/week-7/hello.ftdi.44.blink.c)
- [Makefile]({{site.baseurl}}/assets/week-7/hello.ftdi.44.blink.c.make)

I tested this with all three LEDs on my board (`PA7`, `PA3`, and `PA2`.) The first two worked, but the last one didn't. Troubleshooting, I found a 1.8 V drop over the functional LEDs, but 3.3 V on the non-functional LED, which means the full current was passing through. The most likely cause was that I'd put the LED in backwards. I de-soldered and re-soldered it in the lab, hooked it back up to the FTDI cable, and it works! (Side note: it's really hard to solder tiny components when albuterol makes your hands really jittery.)

# Button Interrupts

Next up: responding to the button. The best way to handle this is probably as an interrupt when the button is pressed. I started with the [example interrupt code](http://academy.cba.mit.edu/classes/embedded_programming/hello.ftdi.44.echo.interrupt.c) and went to work on it. It took a while to read through the datasheet and find what I needed to set to enable the second interrupt (like creating `ISR(PCINT1_vect)` and setting `PCMSK1`). I set a state in my interrupt function that would rotate through 0, 1, 2, or 3 LEDs being on.

It took a lot of trial and error to figure out that it was working, especially given the problem I next ran into: my board seems to be haunted. More specifically, the interrupts seem to happen randomly. When I press the button, the interrupt often isn't triggered. But if I touch the board somewhere else or shake it by the cord, the interrupt is triggered. (I started logging to the console via FTDI to track the interrupts.) Sometimes, a huge string of interrupts is spontaneously triggered and the LEDs start strobing. In the event that there was something weird going on with the FTDI header, I tried directly connecting my board to a 3.3 V power supply, touching the ground with one hand, and then poking around the board with my other hand. It didn't help.

<video loop autoplay muted>
    <source src="{{site.baseurl}}/assets/week-7/failure.mp4" type="video/mp4">
    Your browser does not support the video tag.
</video>

I'm currently at a loss as to what's going on with the interrupts, especially since the FTDI serial inputs are still working just fine. Is something short-circuiting on my board? Do I have a loose connection somewhere? At this point, at least it's only Monday. Hopefully I can get some help tomorrow sorting this out.

### One day later...

This morning a TA linked to this [tutorial on buttons](http://fab.cba.mit.edu/classes/863.16/doc/tutorials/programming/Simple_examples/c_prog_examples.html) that I'd previously missed. (Oops.) Aside from the fact that they're not doing it with interrupts, my problem became immediately clear: I never set the pull up resistor to be on! So I was getting meaningless readings like I had no resistor at all. I adding a line before the loop in my main function setting the button pin to high, and now my button actually works!

Sort of.

Now when I press the button it triggers as multiple button presses. This is a problem I previously encountered when I connected a button to a Raspberry Pi Zero. The solution there was to set a "bouncetime," where additional interrupts within, say 300 ms would be ignored. Apparently this is more generally called "debouncing." And it seems to be a lot trickier in this minimal C environment than it was when adding a keyword to a function with Python GPIO. At this point, I could undo all this, follow the tutorial, and get rid of interrupts. But I've come so far! And I'm apparently a programming masochist, and this might actually be a useful thing to know how to do. (On second thought, the non-interrupt tutorial still likely wouldn't help me, because it wouldn't help with my counter being triggered multiple times.)

According to my Googling, one way to fix this is to add a capacitor to the button to smooth out the changes. But I really don't feel like figuring out to squeeze in a capacitor to my existing board right now. I ended up finding a [useful tutorial](https://embeddedthoughts.com/2016/06/10/attiny85-debounce-your-pushbuttons/) that I was able to adapt to the ATtiny44. I added a timer interrupt that occurs every 10 ticks. If the button state changed, I set a flag that will result in changing the LED in the main loop.

One last problem I encountered was failing to understand the difference between `PORTB` (used for writing to pins) and `PINB` (used for reading from pins. I got really frustrated that my button presses weren't being detected when I tried to run `get(button_port, button_pin)` instead of `get(button_pins, button_pin)`. I also settled

<video loop autoplay muted>
    <source src="{{site.baseurl}}/assets/week-7/success.mp4" type="video/mp4">
    Your browser does not support the video tag.
</video>

Now look at the minimal result of all that work! I finally cleaned up my own code, which I have here:

- [C code]({{site.baseurl}}/assets/week-7/hello.ftdi.44.button-interrupt.c)
- [Makefile]({{site.baseurl}}/assets/week-7/hello.ftdi.44.button-interrupt.c.make)