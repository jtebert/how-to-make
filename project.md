---
layout: page
title: Project
permalink: /project/
---

*I'll update this page as I make progress on my class project through the weekly assignments.*

---

# Week 1

![single initial robot design]({{site.baseurl}}/assets/week-1/model_raw.png){: .small}

In the first week, I [proposed a project]({{site.baseurl}}{% post_url 2017-09-06-cad %}) to create a group of robots that could climb over each other to move faster than a single robot on its own. Knowing nothing about mechanical or electrical engineering, I knew from the start that there would be a lot of changes required to make this thing work - both to get a single robot moving as well as to get the collective behavior I'm looking for.

![group initial robot design]({{site.baseurl}}/assets/week-1/collective-behavior.png)

---

# Week 4

![first print of robot cover]({{site.baseurl}}/assets/week-4/cover-top.jpg){: .small .right .materialboxed}

In our 3D printing week, I made the first prototype related to my project when I 3D printed my first cover design for the robot. (You can check out more on my [Week 4 post]({{site.baseurl}}{% post_url 2017-09-27-3d-printing-scanning %}).) From this initial print, there were a couple of things that immediately jumped out as changes I'd need to make:

- It's too small. This would make an adorably-sized robot that will fit in the palm of my hand, but it will be too small to fit all of the electronic components, wheels, and motors underneath the cover. In particular, it will need to be a lot wider.
- I need to make things thicker. The axle supports and lip are very thin, and the lip is so short that it won't do much to keep the robots from driving off the side of each other.
- If I change the angles of the struts, I can get rid of the need for support material inside of the struts.
- The ends are too steep. The robots need to be able to climb onto each other, and the quarter circle at each end means that when a robot tries to climb up the back, it'll be impossible. Their covers will hit each other rather than the rear robot's wheels or tread getting to the cover, and even if the wheels made contact, it would likely be too steep to climb up. The easiest solution is to make the robots asymmetric, with a more gentle ramp off the back to start climbing and a more blunt front end that will help robots get sucked into the bottom layer when the reach the front of the group.
 
 ![termes whegs]({{site.baseurl}}/assets/project/termes.jpg){: .mini}
 
 In addition to these changes to the cover, I also started thinking about additional improvements to my initial design. In particular, I started questioning my plan to use tank-style treads. I initially selected treads because they would provide grippy contact with the surface at all times when climbing and prevent the robot from "bottoming out." However, treads are difficult to adjust and calibrate the tension. Plus, the tension of the treads can quickly cause 3D printed tread guides to warp and become unusable, as people in my lab discovered on [this robot](https://www.youtube.com/watch?v=x6KFnqqDqh4).

Is there a simpler alternative to treads? One option is "whegs" (a portmanteau of "wheels" and "legs"), as shown on the Termes robot to the left. The grabiness of whegs makes them particularly good for climbing things about the same size as them. However, since whegs are not circular, this causes the robot to move slightly up and down as the whegs turn. This might end up being a problem when the robots are stacked on top of each other. Another alternative is to just use some grippy rubber wheels. This is by far the simplest solution, and I could use existing, commercially available parts. Both wheels and whegs could face the problem of bottoming out as they climb, depending on the angle of the climb and the spacing of the wheels. If I don't use treads, I'll have to think more carefully about the length of the robot or keep a third set of wheels/whegs in the middle.

Another way to help the robots climb and move easily is to give them good grip on the backs of others - rubberizing the top of the robot covers. One way to do this would be a two-material printer to print the rubber as part of the cover. But since it's a rectangular piece with curvature only in one direction, it would likely be easier to just cut out and glue on a piece of rubber. This could also end up helping create the ramp for robots behind to climb. With only a rigid solid cover, the longer, lower, and gentler the slope off the back, the smaller of a maximum angle the robot will be able to climb. If more of this ramp tail is made with flexible rubber, the longer, lower, and gentler the ramp can be while still allowing robots to climb at steeper angles. 

Based on these ideas, I'm currently redesigning the next iteration. It will be larger, asymmetric to have a gentler slope in the back, and have greater clearance to allow for swapping out and testing wheels, whegs, and treads.

---

# Week 5

## Mechanical Design

I talked to some labmates about my robot plans and uncertainty about where to start looking for parts. A postdoc, Nic Carrey, suggested [Pololu micro metal gearmotors](https://www.pololu.com/category/60/micro-metal-gearmotors), which it turns out a number of people in our lab use on their robots since they're very small and affordable.

I enlisted the help of my friend [Clark Teeple](http://clarkteeple.weebly.com/) to help me figure out some of the mechanical engineering and design parts of my robot. Some of his recommendations:

- Go for tracks (instead of wheels or whegs). They'll prevent bottoming out and avoid the need for a gearbox or drive belt. We specifically looked at [these tracks from Pololu](https://www.pololu.com/product/3033), which say in the description that the tracks are elastic enough to allow some wiggle room in terms of positioning.
- Add fillets to all the concave corners of my design to avoid stress points.
- To actually screw the motors to my 3D-printed cover, try countersinking the screws through the top surface of the cover. I may or may not have to end up solid-printing the blocks the motors sit on for stability here. Based on the [bracket specs](https://www.pololu.com/product/989) and a [chart of hole sizes](http://harveytool.com/secure/Content/Documents/Chart_ClearanceHoleDrillSizes.pdf), I'm planning to use a 0.0960" free fit hole, planning for the possibility that FDM printing may have oozy material and make the hole a little smaller.
- Since I belong to a lab that's part of the Wyss Institute, I apparently have access to all sorts of awesome 3D printers at 60 Oxford St., some of which can do multi-material printing, including rubbery materials. I could try using this to print a grippy top surface as part of my cover part.

## Motors

Clark also helped me figure out what motors to try; that list of motors is really intimidating. We went through some back of the envelope calculations. First, we estimate how fast we'll need the motor to turn. I started with the estimate of wanting a robot to go up to \\(v=1 \text{ m/s}\\). From the size of the tread guides, we also have \\(r=17\text{ mm}\\). We can calculate the angular velocity:
$$
\omega = \frac{v}{r} = \frac{1\text{ m/s}}{0.017\text{ m}} = 58.5 \text{ rad/s} \approx 560 \text{ rpm}
$$

This is the speed you'd need assuming there's no torque -- that you have a weightless robot. The robot will be pretty light, but it's still best to round up from there to figure out how fast of a motor to get. From the available options here, this makes the \\(625 \text{ rpm}\\) option look best. But how much force can this handle? The stall torque for all of the \\(625 \text{ rpm}\\) motors is \\(15 \text{ oz-in}\\) (do people actually use these units?), which more usefully is \\(0.106 \text{ Nm}\\). To figure out the force at the tread, we use our tread radius as the moment arm:
$$
\frac{0.106\text{ Nm}}{0.017\text{ m}} = 6.23 \text{ N}
$$

To give a sense of what that means, how heavy of a robot could the motor pull vertically? (If the robot were going up a wall while somehow magically stuck to the wall.:
$$
\frac{6.23 \text{ N}}{9.8\text{ m/s}} = 635 \text{ g}
$$

But the robot won't be going straight up. At this point, the ramp slope is \\(30\deg\\), which means only half as much force (a factor of \\(\cos 30\\)) is needed to counteract gravity, so the robot can be about \\(1.27\text{ kg}\\).

At this point, we remembered that the robot will actually have two motors, so you'd only need a motor half as powerful, right? But the biggest limitation here is likely not the slope -- it's the fact that robots will be trying to crawl forward with more robots on top of them and pushing them backwards. That means that all of this is just enough to give us an estimate of the motor power. I can start with this and adjust based on my experiments, because the motors should be pretty easy to swap out by unscrewing the bracket.

For the \\(625\text{ RPM}\\) motors, there are both \\(6\text{ V}\\) and \\(12\text{ V}\\) options. The \\(6\text{ V}\\) requires higher current and would be slightly less efficient. But a quick search of my lab's Amazon history showed that people are generally using \\(7.4\text{ V}\\) batteries on their robots, so I'll plan for \\(6\text{ V}\\) motors for now.

Finally, there is an option for a double shaft motor or not. The double shaft lets you attach an encoder to measure the velocity of the motor (and estimate position). For future research and analysis, I'd really like to know how fast the robots are moving, so I'll plan for using encoders. Which finally narrows it down to the [50:1 Micro Metal Gearmotor HP 6V with Extended Motor Shaft](https://www.pololu.com/product/2213).

I'll probably come back to this more in the week when we look at output and motors.

## Sensing

My robots need to know something of what's going on around them. Specifically, I want a robot to be able to know when (1) there is a robot on top of it and (2) the robot on top is walking over it. I need some sort of force sensing, but all force is relative to something. I need to be able to detect force between that upper robot and the current robot. I could add some sort of flexure in the top of the cover and put a force sensor between that and the solid part of the body. Alternatively, I could put something like these [force-sensitive resistors](https://www.adafruit.com/product/166) between the rubber top coating and the body. However, I don't know how sensitive these sensors are, and the description even says that they can be very inaccurate. This [sensor film kit](https://www.adafruit.com/product/1917) seems interesting. I'll also probably play around with force sensors the week we cover inputs.

Those would allow me to detect the downward force of a robot, but how do I sense a robot driving on top? My first thought was detecting a horizontal (forward/back) force. But I'm having a hard time figuring out how to detect this kind of shear force. Another option is just *more* force sensors, along the length of the body. Then I'd be able to detect as the weight of a robot moved from back to front over the robot below. I've got two concerns here. First, how many sensors would I need for this? Would just two (front and back) be enough? Second, how well would this work as you get more robots piling up? For example, when there are two robots on top of a single robot, or when there are multiple layers of robots.

I'm now feeling a lot more confident about the mechanical design and movement of the robot, but the sensing is looking like more of a challenge than I'd initially given it credit.

## Electronics

Here are the electronics components that at this point I think I'll need:

- Raspberry Pi Zero
- 7.4 V battery
- 6 V voltage regulator to power motors (a linear regulator will likely be fine, since the loss voltage drop is relatively small)
- 5 V voltage regulator for Raspberry Pi Zero
- Motor driver (h-bridge): [TB6612FNG Dual Motor Driver Carrier](https://www.pololu.com/product/713). This one can drive two motors, and it specifically says that it will work with the motors I'm planning to use. Depending on how many other small parts I end up with, I could end up making my own PCB that incorporates the voltage regulators, h-bridge, maybe something for force sensing, and whatever other electronics don't directly connect to the Pi.
- Motor encoders (2): [Magnetic Encoder Pair Kit for Micro Metal Gearmotors, 12 CPR, 2.7-18V (HPCB compatible)](https://www.pololu.com/product/3081)

---

# Week 6

![New part compared to old]({{site.baseurl}}/assets/project/print2-2.jpg){: .small .materialboxed}

At the beginning of the week, I printed the update chassis part. It's now significantly larger than the first version, once I've left space for the motors, Pi Zero, and wheel tracks.<br>

![New part bottom]({{site.baseurl}}/assets/project/print2-1.jpg){: .small .materialboxed}

There's now actually stuff on the bottom as well. In the front are blocks for the motors to sit on, with screws holes through from the top for the motor brackets. In the back are mounts for the rear track wheels. They turn freely and are driven by the motors in the front, which makes things easier here. I did my best with the diagrams from Pololu, but I still feel like the front and back wheels won't line up. If not, though, then I can just make an adjustment and reprint!

In the middle there are offsets and screw holes for a Raspberry Pi Zero. Since I've got a couple of these laying around, I could actually verify that these work! The holes are the right size and line up perfectly. It's really simple, but it's super satisfying when something you make actually works with an existing part.<br>

![New part screw holes]({{site.baseurl}}/assets/project/print2-3.jpg){: .small .materialboxed}

Lastly, about those screw holes. The screw holes for the motor bracket and Pi Zero are countersunk from the top of the chassis. But I printed my part upside down again to reduce support material, which means I ended up with support material inside of the holes. You can see some strands here. I don't have the right screws on hand to test that they freely fit through. If not, I could drill through for this prototype and print larger holes on the next version.

Also, totally unrelated to anything important, but when I print the final versions for my project, I want them all to be different colors. (If I can keep doing it on the 3Dwox, there are a lot of colors.) It is kind of practical, since it will help me easily distinguish the robots once I have more than one of them. 

<br>

---

# Week 7

## Sensing and Control

This week I had some great progress on the design thanks to [Jordan Kennedy's](http://fab.cba.mit.edu/classes/863.17/Harvard/people/Jordan-Kennedy/index.html) husband Skyler Rydberg coming on our lab retreat and being willing to spend part of the evening talking engineering. I brought up my issue of how to detect velocity of a robot moving over the top of another robot. It's easy to detect acceleration of a robot, but my challenge is distinguishing [**zero** acceleration and **zero** velocity] from [**zero** acceleration and **non-zero** velocity].

I was hung up on finding some way to use force sensors for this problem, but from Skyler and Nic Carey, I got a better idea: **light!** In this case, each robot has a light on the bottom of it, pointing downwards. Each robot also has a light sensor on the top of it. When a robot moves over the top of another, the bottom robot will detect a change in light intensity as the upper robot moves. With a single light sensor, the lower robot could detect whether or not the upper robot is moving; with two sensors, the lower robot could estimate the velocity of the upper robot by comparing the timing of light intensity peaks.

Using light sensors can also help solve another problem I've had in the back of my mind: *How does the group decide when to start and stop?* My preliminary idea was that robots on top will stop (like is seen in the larvae video), and when the robots below detect no movement above them, they will also stop moving. There are two problems with this:

1. What triggers the robot(s) on top to stop moving? (And correlated, how do you deal with the fact that there are multiple robots on the top that all have to stop?)
2. Once stopped, how do you trigger the robots to start moving again?

The light sensing can be used as a control signal to determine this behavior. For example, I could use red light as a control signal to tell the robots to stop. If a robot is on top (i.e., no downward force on it and a light sensor clear to the world), shining a red light will signal it to stop moving. This stop will then propagate down through the layers. Conversely, removing this red light (or shining a different color light) will signal the top layer robots to start moving, which will again propagate down to start the entire group moving.

After discussing with Melvin, we refined this approach. First, the plan is to use infrared light instead of visible light, which means much less interference from background light. Second, we'll put two wide angle infrared photodiodes on the top of the chassis. With overlapping visibility cones, we can use parallax to determine the position of the robot on top (and therefore its velocity). We can also use a pattern of infrared light (e.g., flashing) as an external signal to the robots on top without creating problems like visible strobe effects. (Bonus: this also means we have to build a controller.) This ends up being similar to the way that the [Kilobots](https://ssr.seas.harvard.edu/kilobots) are controlled.

At this point, it's also becoming more clear what I'll need in terms of additional electronics. I'll likely make a PCB to incorporate all of my "accessory" electronics, like the h-bridges for the motors, op amps for the photodiodes, and the infrared LED. Mounting things on the limited underbelly of this robot might also prove tricky, so ideally I'd like to be able to mount it directly below the Raspberry Pi using standoffs and the same mounting holes. (Conveniently, this would also center the LED on the bottom of the robot.)

## Simulation

For this class, I'm planning to build and program the physical robots. However, I'll likely be very limited in the number of robots I'll be able to build, and it will be hard to change them once they're built. For another class, though, I've gotten permission to model and simulate this system as a project. In that case, I'll do 2D simulations of larger groups of robots (likely with Pymunk) and can test different parameters of the system, such as weight of the robots, between-robot friction, maximum allowed torque, and shape of the chassis. This should help bootstrap parameter tuning that would be much slower on the physical robots. If time permits, I may also play around with genetic algorithms for design and control of the robot.

---

# Parts List

This is based on the parts I've identified so far, and I'll update this as I go. The total per-robot price is inching up, but it's still well within what's considered low-cost robotics.

Item     | Quantity | Price per unit | Total Price
-------- | ---------|----------------|------------
[Raspberry Pi Zero W](https://www.adafruit.com/product/3400) | 1 | $10.00 | $10.00
[Motors](https://www.pololu.com/product/2213) | 2 | $16.95 | $33.90
[Extended motor brackets](https://www.pololu.com/product/1089) | 1 | $4.95 | $4.95
[Tracks](https://www.pololu.com/product/3033) | 1 | $14.95 | $14.95
[Motor driver](https://www.pololu.com/product/713) | 1 | $4.95 | $4.95
[Motor encoders](https://www.pololu.com/product/3081) | 1 | $8.95 | $8.95
[Infrared LED](https://www.digikey.com/product-detail/en/osram-opto-semiconductors-inc/SFH-4050-Z/475-2864-1-ND/2207282) ([alternatives](https://www.digikey.com/products/en/optoelectronics/infrared-uv-visible-emitters/94?k=infrared+led&k=&pkeyword=infrared+led&pv551=45&pv551=47&pv551=49&FV=2dc0ac1%2C4fc0064%2C4fc0461%2C4fc0017%2C4fc005d%2C4fc005e%2C4fc0060%2C89c0029%2C89c002c%2C89c002d%2C89c002f%2C89c0031%2C1f140000%2Cffe0005e%2C1140003&mnonly=0&ColumnSort=-1000009&page=1&quantity=0&ptm=0&fid=0&pageSize=25)) | 1 | $0.96 | $0.96
[Infrared Photodiode](https://www.digikey.com/product-detail/en/vishay-semiconductor-opto-division/BPW34/751-1015-ND/1681149) ([alternatives](https://www.digikey.com/products/en/sensors-transducers/optical-sensors-photodiodes/543?k=infrared+photodiode&k=&pkeyword=infrared+photodiode&pv551=43&FV=1140050%2C89c0029%2C89c002b%2C89c002d%2C89c002f%2C1f140000%2Cffe0021f%2Ca0000b%2Ca0000c%2Ca0018d%2Ca00009&mnonly=0&ColumnSort=0&page=1&quantity=0&ptm=0&fid=0&pageSize=25)) | 2 | $1.21 | $2.42
[Battery](https://smile.amazon.com/gp/product/B0722Y5ZS9/ref=oh_aui_search_detailpage?ie=UTF8&psc=1) | 0.5 | $17.99 | $9.00
| | **TOTAL:** | **$90.08**

*Last updated: 2017-10-23* 

<!--
10+33.9+4.95+14.95+4.95+8.95+0.96+2.42+9
-->