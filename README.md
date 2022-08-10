# This is a project by Jan Goličnik

Basically I made a simulation.

## How it works

We spawn in 1 000 000 little agents that all leave pheromones behind when they move. They can also sense some distance left and right at some angle.
We can control two species and how they interact, mainly if their peromones repel or attract each other, but it also affects their color :)

## Setup
This was coded in Visual Studio in C++ using OpenGL, glfw, glm and imGui.
In the root folder there is a shortcut called ant sim which should start the simulation. It hasnt been tested on many machines so performance or bugs may vary depending on your setup.
If the window holding the simulation window is outside the main window please dock it in place (drag it to the center).
You should then be ready to mess around with the controls.

## The controls
Please expand the controls panel if you cant see everything.
All the setting should be properly documented in app so please refer to that. Don't be afraid to mess around with the values a lot, especially with sense angle, as that is where most of the fun is.
