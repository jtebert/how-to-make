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

Clark also helped me figure out what motors to try; that list of motors is really intimidating. We went through some back of the envelope calculations. First, we estimate how fast we'll need the motor to turn. I started with the estimate of wanting a robot to go up to $v=1 \text{ m/s}$. From the size of the tread guides, we also have $r=17\text{ mm}$. We can calculate the angular velocity:
$$
\omega = \frac{v}{r} = \frac{1\text{ m/s}}{0.017\text{ m}} = 58.5 \text{ rad/s} \approx 560 \text{ rpm}
$$

This is the speed you'd need assuming there's no torque -- that you have a weightless robot. The robot will be pretty light, but it's still best to round up from there to figure out how fast of a motor to get. From the available options here, this makes the $625 \text{ rpm}$ option look best. But how much force can this handle? The stall torque for all of the $625 \text{ rpm}$ motors is $15 \text{ oz-in}$ (do people actually use these units?), which more usefully is $0.106 \text{ Nm}$. To figure out the force at the tread, we use our tread radius as the moment arm:
$$
\frac{0.106\text{ Nm}}{0.017\text{ m}} = 6.23 \text{ N}
$$

To give a sense of what that means, how heavy of a robot could the motor pull vertically? (If the robot were going up a wall while somehow magically stuck to the wall.:
$$
\frac{6.23 \text{ N}}{9.8\text{ m/s}} = 635 \text{ g}
$$

But the robot won't be going straight up. At this point, the ramp slope is $30\deg$, which means only half as much force (a factor of $\cos 30$ is needed to counteract gravity , so the robot can be about $1.27\text{ kg}$.

At this point, we remembered that the robot will actually have two motors, so you'd only need a motor half as powerful, right? But the biggest limitation here is likely not the slope -- it's the fact that robots will be trying to crawl forward with more robots on top of them and pushing them backwards. That means that all of this is just enough to give us an estimate of the motor power. I can start with this and adjust based on my experiments, because the motors should be pretty easy to swap out by unscrewing the bracket.

For the $625\text{ RPM}$ motors, there are both $6\text{ V}$ and $12\text{ V}$ options. The $6\text{ V}$ requires higher current and would be slightly less efficient. But a quick search of my lab's Amazon history showed that people are generally using $7.4\text{ V}$ batteries on their robots, so I'll plan for $6\text{ V}$ motors for now.

Finally, there is an option for a double shaft motor or not. The double shaft lets you attach an encoder to measure the velocity of the motor (and estimate position). For future research and analysis, I'd really like to know how fast the robots are moving, so I'll plan for using encoders. Which finally narrows it down to the [0:1 Micro Metal Gearmotor HP 6V with Extended Motor Shaft](https://www.pololu.com/product/2213).

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

## Parts List

This is based on the parts I've identified so far, and I'll update this as I go. The total per-robot price is inching up, but it's still well within what's considered low-cost robotics.

Item     | Quantity | Price per unit | Total Price
-------- | ---------|----------------|------------
[Raspberry Pi Zero W](https://www.adafruit.com/product/3400) | 1 | \$10.00 | $10.00
[Motors](https://www.pololu.com/product/2213) | 2 | \$16.95 | $33.90
[Extended motor brackets](https://www.pololu.com/product/1089) | 1 | \$4.95 | 4.95
[Tracks](https://www.pololu.com/product/3033) | 1 | \$14.95 | $14.95
[Motor driver](https://www.pololu.com/product/713) | 1 | \$4.95 | $4.95
[Motor encoders](https://www.pololu.com/product/3081) | 1 | \$8.95 | $8.95
[Battery](https://smile.amazon.com/gp/product/B0722Y5ZS9/ref=oh_aui_search_detailpage?ie=UTF8&psc=1) | 0.5 | \$17.99 | $9.00
| | | **$86.70**