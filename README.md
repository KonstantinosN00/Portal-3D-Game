# Portal-3D-Game

###### Installation
Use CMakeLists.txt file to build project with CMake and run the solution created (VVR_OGL_LABORATORY.sln)


### Α. INTRODUCTION
This application was developed in the 7th semester of the Electrical and Computer Engineering for the course "Graphics and Virtual Reality". It is developed in C++ for the main program and in GLSL for the GPU shaders programming.

All objects are handled in the 3-dimensional space using **3D Matrix Transformations** in order to align them correctly and create movement.
Several other graphics techniques were implemented in order to create the scene in which collision, teleportation, lighting and shading effects are applied on the physical objects.


### Β. INSTRUCTIONS - SCENE DESCRIPTION
The scene consists of the following objects that were either developed in blender or taken from 3D model distribution pages (or typed point by point for simple layers):
- **Player**\
He moves with the **W-A-S-D** buttons. His field of view, as well as himself, rotates according to the mouse movement. The posture was modified in blender to show the intention of throwing the spheres.
- **Teleportation portals**\
Created by right-clicking on the nearest wall, at the point where the extension of the line of sight intersects it. Use numbers 1 or 2, to select the portal that will be created/moved.\
When the spheres or the player collide with one portal then they appear in the other, in the corresponding position in which they entered the original one. Portals are bidirectional. Also, a repeating entrance - exit can be created that continues indefinitely. The physical properties added, however, limit the speed to a maximum value.\
[An interesting feature of the portals is that if you look from a portal to a parallel portal, you create an effect of an "infinite" loop, similar to looking to two parallel mirrors. The maximum number of recursions that are visible is specified by a parameter, due to the fact that our systems have limited capabilities.]
- **Spheres**\
Created by left-clicking the mouse and launched in the direction of vision, increasing the vertical component so that the user has line of sight to the basket. Press "R" to recollect the spheres.
- **Basketball court**\
Rectangular parallelepiped with "cut-off" roof
- **Sky**\
Large plane elevated above the scene.
- **Basket**\
Model from the web, scaled in the horizontal directions for convenience. 

### C. COLLISION MANAGEMENT
Collision detection functions were implemented so that the spheres can interact with each other, with the walls and with the basket.\
They create one of the most important functionalities of the game, the detection of a person or an object that enters the portal.

![image](https://user-images.githubusercontent.com/117118237/226929277-9cf0e654-ba50-4b0e-821e-3bd7784413d9.png)

![image](https://user-images.githubusercontent.com/117118237/226929738-1706065d-e6aa-480e-9337-7600502d6041.png)


