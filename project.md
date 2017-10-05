---
layout: page
title: Project
---

*I'll update this page as I make progress on my class project through the weekly assignments.*

# Week 1

![single initial robot design]({{site.baseurl}}/assets/week-1/model_raw.png){: .small}

In the first week, I [proposed a project]({{site.baseurl}}{% post_url 2017-09-06-cad %}) to create a group of robots that could climb over each other to move faster than a single robot on its own. Knowing nothing about mechanical or electrical engineering, I knew from the start that there would be a lot of changes required to make this thing work - both to get a single robot moving as well as to get the collective behavior I'm looking for.

![group initial robot design]({{site.baseurl}}/assets/week-1/collective-behavior.png)

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