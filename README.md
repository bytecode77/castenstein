# Castenstein

Castenstein is a raycaster. The name derives from Wolfenstein 3D, a popular game from the 90's. Raycasting is a rendering technique, which was popular back in that time, because computers weren't powerful enough to handle polygonal software rendering. Instead of rendering triangles, a raycaster renders a grid of blocks which are on a 2D basis, displayed in 3D. That's why raycasters are often called 2.5D engines - they look pretty much 3D, but are very much two dimensional in terms of math. Raycasters are considerably faster than polygonal software renderers due to the computational simplicity. In exchange, raycasting has its limitations. For example, you can't really tilt up and down. If you can, it's just vertical shifting of the screen, which results in a skewed image - i.e. if you tilted down at an angle of 90°, the image would be infinitely skewed.

You can, however, do *some* moving up/down and tilting up/down as well as rendering sprites additionally to walls. Different brightness and fog levels are pre-calculated resulting in very high performance with little compromise on graphics. Castenstein also takes benefits from multi-core CPU's, supporting multi-threaded rendering, since raycasting is a task that can be very easily parallelized.

Raycasting in this form doesn't have any particular use nowadays. It has been widely replaced with polygonal renderers, later on implemented by hardware (Direct3D and OpenGL).

## Features

* Textured walls, floors and ceilings
* Sprites
* Sky
* Lighting & Fog
* Drop-down shadows from ceilings
* Collisions
* Moving up/down & tilting up/down
* Minimap overlay
* Multi-threaded rendering

## Screenshots

[![](https://bytecode77.com/cache/thumbs/?path=images/sites/garage/software/castenstein/001.jpg&height=100)](https://bytecode77.com/images/sites/garage/software/castenstein/001.jpg)
[![](https://bytecode77.com/cache/thumbs/?path=images/sites/garage/software/castenstein/002.jpg&height=100)](https://bytecode77.com/images/sites/garage/software/castenstein/002.jpg)
[![](https://bytecode77.com/cache/thumbs/?path=images/sites/garage/software/castenstein/003.jpg&height=100)](https://bytecode77.com/images/sites/garage/software/castenstein/003.jpg)
[![](https://bytecode77.com/cache/thumbs/?path=images/sites/garage/software/castenstein/004.jpg&height=100)](https://bytecode77.com/images/sites/garage/software/castenstein/004.jpg)
[![](https://bytecode77.com/cache/thumbs/?path=images/sites/garage/software/castenstein/005.jpg&height=100)](https://bytecode77.com/images/sites/garage/software/castenstein/005.jpg)
[![](https://bytecode77.com/cache/thumbs/?path=images/sites/garage/software/castenstein/006.jpg&height=100)](https://bytecode77.com/images/sites/garage/software/castenstein/006.jpg)

## Binaries

[![](https://bytecode77.com/images/shared/fileicons/zip.png) Castenstein 1.0.2 Binaries.zip](https://bytecode77.com/downloads/garage/software/Castenstein%201.0.2%20Binaries.zip)

## Project Page

[![](https://bytecode77.com/images/shared/favicon16.png) bytecode77.com/garage/software/castenstein](https://bytecode77.com/garage/software/castenstein)