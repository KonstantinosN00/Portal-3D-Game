# Portal-3D-Game

Α. INTRODUCTION\
In the 7th semester project for the course "Graphics and Virtual Reality" an application was developed in C++ for the main program and in GLSL for programming the shaders. The techniques explained during the semester were applied in order to create a scene in which collision, teleportation, lighting and shading effects take place on the physical objects for which the user of the application has control over the events based on his/her movements.

Β. SCENE DESCRIPTION - INSTRUCTIONS\
The scene consists of the following objects that were either developed in blender or taken from 3D model distribution pages (or typed point by point for simple layers):
- Basketball court 
Rectangular parallelepiped with "cut-off" roof
- Sky
Large level elevated above the stage.
- Basket 
Model from the web, enlarged by the horizontal plane for convenience. 
- Spheres 
Created by left-clicking the mouse and launched in the direction of vision, increasing the vertical component so that the user has line of sight to the basket.
- Player 
The posture was modified in blender to show the intention of throwing the balls. He moves with the W-A-S-D buttons and his field of view, as well as himself, rotates according to the mouse movement.
- Teleportation portals
Created by right-clicking on the nearest wall, at the point where the extension of the line of sight intersects it. It must be selected with number 1 or 2 which portal will be created.
When the orbs or the player collide with one portal then they appear in the other, in the corresponding position in which they entered the original one. Portals are bidirectional. Also, a repeating entrance - exit can be created that continues indefinitely. The physical properties added, however, limit the speed to a limiting value.
C. OBJECTS
Each visible object in the scene is represented by a class that includes:
- Object Drawable
The Drawable class was used for the semester labs and receives vectors that include 3d points, normals, and 2D point correspondences (UV coordinates). It can also receive these vectors from reading an obj file type. In addition, it maps VAO and VBO.
- Draw() function
Draws the model one at a time.
- Update() function
Refreshes the object's transformation table every frame.

Specifically for objects that need to have physical properties then the class describing them must inherit from the RigidBody class. This includes values for the quantities of mass, velocity, position, angular velocity, momentum and angular momentum. Also included is a function variable that returns the forces and moments exerted on the objects. Based on all this, the function advanceState(t,h) calculates the physical quantities of the bodies for the next state by numerical methods.
