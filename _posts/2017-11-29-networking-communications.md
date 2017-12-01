---
layout: post
title:  "Networking and Communications"
#image: "week-12/flappy-screenshot.png"
week: Week 13
---

**Assignment:** Design and build a wired and/or wireless network connecting at least two processors.

<!-- more -->

---

The final project deadline is coming up really fast, which means I want to aim this week's assignment at helping my final project for time management. I'd really like to learn to use the NRF52 at some point, because the potential for low cost, real-time communication between devices (or in my case, robots) seems like it has some really cool potential. But since it appears to have something of a learning curve, is a completely different MCU than I've been using, and doesn't have nice "Hello World" examples like the alternatives, I think I'll leave that for the post-course future.

For project relevance this week, I'm planning to use optical communication, because that's the only communication my project robots will have between each other. Specifically, they'll use infrared light. The Kilobots also communicate using infrared bounced off of a surface, and they're even programmed by infrared. So I've *used* optical communication before, but I have no idea what the robots are actually doing under the hood.

For board design, I'll be using two separate boards this week. To send messages, I'll use my input device board from two weeks ago (with an infrared LED and phototransistor). Messages will be received by the first version of my final project board. This board has two LEDs and 2 phototransistors, and is controlled by an ATMEGA328p (the first time I'm branching out from the ATTiny MCUs). It also has components for powering and controlling 2 motors (using MOSFETs) and 2 LEDs (green and blue) for debugging. My goal is to have the LEDs turned on and off by the messages from the other controller. (If I get the motor control up and running, I can also have the motors controlled by the incoming messages.)

For the communication design, I'm planning on using serial-type messages that will directly encode a few bytes of data in the light flashing, sent as fixed-sized "packets." For now, I'm just having unidirectional communication between two boards, so I don't need to worry about message conflicts. (Using more boards in the future, a CSMA/CD protocol could be used to avoid message conflicts.) It would also be good to have some sort of checksum byte for the receiver to verify the contents of the message. However, I'm not sure I'll have time to get that working this week (with a different class having a looming project deadline next week).

# Hardware

I started my design with the [Hello World 328p board design](http://academy.cba.mit.edu/classes/embedded_programming/hello.arduino.328P.png), then started adding components for the additional functionality I'll need in my final project: separate input power with a voltage regulator, 2 IR LEDs, 2 IR phototransistors on mini breakout boards, and motor controllers. Apart from my new method of motor control (more on that on my [project page]({{site.baseurl}}/project/)), the added parts all came from things I had done in previous weeks. Neil suggested making our board design modular to make it easier to change things, but I opted not to do that. This is partly because I'm really space-limited by the area on the underside of my robots, and partly because I'll need to make a bunch of these for multiple robots, which I think would make multiple boards per robot more challenging.

I spent a lot of time laying out the board. I suddenly have a lot more pins and space constraints. I measured out the maximum board size I could fit on the underside of my robot and set that up as the dimensions of my board in Eagle. I then added circles for the screws that I'd already designed into my chassis CAD model. I ended up needing *only* 6 0-Ohm resistors to get everything laid out. I have no idea how far from optimal that is, but it got the job done.

I also did a stupid thing with a resistor. After milling my board, I realized that I'd placed a resistor *under* the FTDI header. Luckily (since the PCB took 30 minutes to mill and I didn't want to redo it), the resistor fit in the space between the pins and the plastic support, so I soldered it on and happily went about stuffing the rest of my board. And then I plugged in my board. That power LED looked way too dim. I went back to check my board design and realized my stupid mistake: I'd misread the the Hello World board design and put a 10k resistor in series with the VCC from the FTDI header. And it was the resistor located under the FTDI header. Crap. With solder wick, extra rosin, a solder vacuum, and a heat gun, I managed to angle the FTDI header pins up enough to desolder the resistor and solder in a tiny wire in its place. (I wasn't in the Science Center at this point, otherwise a 0 Ohm resistor would have been a better choice.) I nearly pulled the traces off the board that connected the FTDI header, but luckily when I ran a beep test with the multi-meter after my "repair," everything was still properly connected. If I don't encounter more problems, and I decide to keep using this board, I'll probably add some glue around the FTDI header to keep it from coming loose.

# Software

To make sure I could program the 328p board at all, I started by compiling and uploading the 328p blink code from the embedded programming week ([C code](http://academy.cba.mit.edu/classes/embedded_programming/hello.arduino.328P.blink.c), [Makefile](http://academy.cba.mit.edu/classes/embedded_programming/hello.arduino.328P.blink.make)). I changed the LED pin to one of my debug LEDs, compiled, and programmed with my ISP header. It worked!

... with one issue. I tried to use `program-usbtiny-fuses` before programming and got this message:

{% highlight bash %}
avrdude: safemode: efuse changed! Was 7, and is now ff
Would you like this fuse to be changed back? [y/n]
{% endhighlight %}

If I say "no," it fails. If I say "yes," it hangs and does nothing. Google and the datasheet have failed me trying to figure out the problem here, but somehow the board still works. My guess is that this means it's running on the internal clock instead of using the resonator. But I really don't understand the fuses here.

Next up, I have to figure out how to send messages with light. I'll start with some combination of the code for sending serial messages over FTDI (to understanding encoding and decoding messages) and the synchronous light detection code (for turning the light on and off at the right rate, as well as detecting those on/off states).
