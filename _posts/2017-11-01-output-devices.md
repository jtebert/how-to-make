---
layout: post
title:  "Output Devices"
#image: "week-8/molds-dark.jpg"
week: Week 9
---

**Assignment:** Add an output device to a microcontroller board you've designed and program it to do something.

<!-- more -->

---

# Design

I'm building a robot, so I really need to make that robot move. That means this week I'm going to make a board to control some regular old DC motors. If all goes as planned, I hope to make a board that will control two motors, like I plan to have on my robot. This week I'd also like to try making a 1.5-sided PCB -- in other words, using the other side of my PCB as a ground plane. I need to figure out how to do that in Eagle, then learn how to connect the vias when I build the board.

![example DC motor board](http://academy.cba.mit.edu/classes/output_devices/H-bridge/hello.H-bridge.44.png){: .small .materialboxed}

I'm starting with the motor hello world board from the class website. I'll re-build it in Eagle, add outputs for a second motor, and maybe buttons to control the motor. (This time maybe I'll even hardware debounce them and save myself a struggle in code.)

Once I started designing in Eagle, it turns out that adding a second h-bridge makes the layout and routing a lot trickier. I ended up using two 0 Ohm resistors to jump some other traces to get the reference voltage to my second h-bridge. Yuval pointed out something useful to me, though, that let me avoid the 1.5-sided approach. The design in Neil's routes the power and ground generally in a ring around the outside, and then components inside can draw from that. That turned out to be useful in laying out my board. Also useful: this time I did my schematic by labeling nets instead of drawing a million lines. This turned out to be crazy helpful, because then when I started a trace on the board, Eagle highlighted all the possible pads and traces for it to properly connect to. It also meant it was a lot easier to keep track of my pins on the 6-pin header and the ATtiny.

# Motors

I found some spare Pololu motors in a drawer full of screws my lab, so I'm going to use those this week. I soldered on some wire and tested them both with a power supply . They are 6 V motors, and draw 60 or 100 mA. (They're apparently slightly different motors.) 