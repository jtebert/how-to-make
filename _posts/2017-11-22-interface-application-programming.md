---
layout: post
title:  "Interface and Application Programming"
#image: "week-12/mini-board-dark.jpg"
week: Week 12
---

**Assignment:** write an application that interfaces with an input and/or output device that you made, comparing as many tool options as possible.

<!-- more -->

---

I know how to program (that feels weird to say after feeling like an impostor in a computer science PhD program), so this week I decided to make my life more challenging than it had to be.

# Racket

The first thing I decided to do was invoke masochistic nostalgia and attempt to create an interface in my first programming language: [Racket](https://racket-lang.org/). I haven't touched functional programming since I graduated from undergrad and stopped TAing Northeastern's intro CS class.

Luckily, Racket has a library that makes it easy to interface with serial ports: [libserialport](https://docs.racket-lang.org/libserialport/index.html). In addition to install DrRacket (the IDE for Racket), I installed the `libserialport` module from my terminal with `raco pkg install libserialport`.

From there, it was surprisingly easy to connect to the serial port and read bytes from it out to the console. I pulled out some code back from when I took this class as a sophomore in undergrad to remember how to do some basic functionality. I've got to say, I was really good at documentation back then (probably because we were heavily graded on it and I was a goody two-shoes). I also looked at Neil's [Python hello world interface code from last week](http://academy.cba.mit.edu/classes/input_devices/light/hello.light.45.py) to understand how to interpret the incoming information from the serial port. As a sanity check, I read out 32 bytes from the port:

```
#"\1\2\3\4\326\3\1\2\3\4\327\3\1\2\3\4\326\3\1\2\3\4\327\3\1\2\3\4\326\3\1\2"
```

That looks right. A series of bytes 1, 2, 3, 4, followed by the low byte, then the high byte (0-3), to give a total value for the sensor reading of $$256 \cdot b_\text{high} + b_\text{low}$$.

However, there were two major problems that I realized I was having, after much frustrating debugging:

1. I couldn't figure out how to flush the buffer before reading from it. This meant an accumulation of data in my serial port buffer that I didn't care about, and my program was very quickly very far behind the buffer in the serial port (since I wasn't reading it at 9600 baud).
2. I had trouble converting the byte value into an integer. In Python, this was done with the `ord()` function, which converts the 8-bit string (char) into an integer. At first, I read from the serial port using the `read-bytes` method, and was trying to convert this into an integer really clumsily through lists: `(first (bytes->list (read-byte port)))`. This gave me integers, but they weren't the right ones.

# Python