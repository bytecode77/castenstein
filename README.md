# Castenstein

## Raycaster

Castenstein is a raycaster. The name derives from Wolfenstein 3D, a game from the 90's. Raycasting is a commonly used rendering technique, in a time where computers weren't powerful enough to handle polygonal software rendering. Instead of rendering triangles, a raycaster renders a grid of blocks on a 2D basis, displayed in 3D. That's why raycasters are often referred to as "2.5D engines" - they look 3D, but they're very much two dimensional in terms of math.

[![](https://bytecode77.com/images/pages/castenstein/001.thumb.jpg)](https://bytecode77.com/images/pages/castenstein/001.jpg)
[![](https://bytecode77.com/images/pages/castenstein/002.thumb.jpg)](https://bytecode77.com/images/pages/castenstein/002.jpg)
[![](https://bytecode77.com/images/pages/castenstein/003.thumb.jpg)](https://bytecode77.com/images/pages/castenstein/003.jpg)
[![](https://bytecode77.com/images/pages/castenstein/004.thumb.jpg)](https://bytecode77.com/images/pages/castenstein/004.jpg)
[![](https://bytecode77.com/images/pages/castenstein/005.thumb.jpg)](https://bytecode77.com/images/pages/castenstein/005.jpg)
[![](https://bytecode77.com/images/pages/castenstein/006.thumb.jpg)](https://bytecode77.com/images/pages/castenstein/006.jpg)

Raycasters are considerably faster than polygonal software rasterizers due to the computational simplicity. In exchange, raycasting has limitations. For example, tilting up and down is not possible. It's actually implemented by vertical shifting of the screen, which results in a skewed image.

It's possible, however, to do some moving up/down and tilting as well as rendering sprites in addition to walls. For different brightness and fog levels, pre-calculated texture images are used, resulting in negligible performance impact. Castenstein also supports multi-threaded rendering, because raycasting is a task that can be very easily parallelized.

Raycasting in this form doesn't have any particular use nowadays. It has gone obsolete with polygonal renderers becoming the default. This is a tech-demo.

## Features

- Textured walls, floors and ceilings
- Sprites
- Sky
- Lighting & Fog
- Drop-down shadows from ceilings
- Collision detection
- Moving up/down & tilting up/down
- Minimap overlay
- Multi-threaded rendering

## Downloads

[![](http://bytecode77.com/public/fileicons/zip.png) Castenstein 1.0.2.zip](https://bytecode77.com/downloads/Castenstein%201.0.2.zip)

## Project Page

[![](https://bytecode77.com/public/favicon16.png) bytecode77.com/castenstein](https://bytecode77.com/castenstein)