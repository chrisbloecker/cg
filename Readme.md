# cg
This repository contains some of the programming assignments from my computer graphics and image processing lectures. All programs are written in C.

## Computer Graphics
These programs are implemented using OpenGL and cover computer graphics basics, such as
* 2D and 3D real-time rendering
* texturing
* shadowing
* picking
* algorithmic animation and mesh generation
* interpolation

### 01 - Pong 2D
A single player version of the classic game Pong. The focus of this assignment was to get familiar with OpenGL.

Key bindings:
* <kbd>up</kbd>/<kbd>down</kbd> move the racket
* <kbd>left</kbd>/<kbd>right</kbd> rotate the racket
* <kbd>q</kbd>/<kbd>ESC</kbd> quit the game

![Pong 2D](https://github.com/chrisbloecker/cg/blob/master/img/pong-2d.png?raw=true)

### 02 - Hugo 2D
A game similar to frogger, however, the player is running on the road rather than trying to cross it. The focus of this assigment was to get familiar with modelling by composing basic shapes and algorithmic "animation".

Key bindings:
* <kbd>h</kbd> toggle help
* <kbd>left</kbd>/<kbd>right</kbd> move Hugo
* <kbd>q</kbd>/<kbd>ESC</kbd> quit the game

![Hugo 2D](https://github.com/chrisbloecker/cg/blob/master/img/hugo-2d.png?raw=true)

### 03 - Hugo 3D
A game similar to frogger, however, the player is running on the road rather than trying to cross it. Main foci of this assignment were lighting and 3D rendering.

Key bindings:
* <kbd>h</kbd> toggle help
* <kbd>w</kbd>/<kbd>a</kbd>/<kbd>s</kbd>/<kbd>d</kbd> move the camera
* <kbd>left</kbd>/<kbd>right</kbd> move Hugo
* <kbd>l</kbd>/<kbd>f</kbd> switch wireframe mode on/off
* <kbd>n</kbd> toggle normals
* <kbd>F1</kbd> toggle fixed light source
* <kbd>F2</kbd> toggle moving light source
* <kbd>q</kbd>/<kbd>ESC</kbd> quit the game

![Hugo 3D](https://github.com/chrisbloecker/cg/blob/master/img/hugo-3d-1.png?raw=true)
![Hugo 3D](https://github.com/chrisbloecker/cg/blob/master/img/hugo-3d-2.png?raw=true)

### 04 - Zoo
A cage that contains a giraffe, a pig and a fish. Main foci of this assignment were texturing and picking. The animals can be picked with a left click an dragged. An animation can be triggerd with a right click on the animals.

Key bindings:
* <kbd>w</kbd>/<kbd>a</kbd>/<kbd>s</kbd>/<kbd>d</kbd> move the camera
* <kbd>l</kbd>/<kbd>f</kbd> switch texturing on/off
* <kbd>g</kbd> swith cage model
* <kbd>t</kbd> switch textures
* <kbd>n</kbd> toggle normals
* <kbd>F1</kbd> toggle light source
* <kbd>q</kbd>/<kbd>ESC</kbd> quit the game

![Zoo](https://github.com/chrisbloecker/cg/blob/master/img/zoo.png?raw=true)

### 05 - Pong 3D
A single player version of the classic game Pong in 3D. The main focus of this assignment was shadowing using stenceling.

Key bindings:
* <kbd>c</kbd> cheat mode (the ball can't escape anymore)
* <kbd>w</kbd>/<kbd>a</kbd>/<kbd>s</kbd>/<kbd>d</kbd> move
* <kbd>z</kbd>/<kbd>u</kbd>/<kbd>i</kbd>/<kbd>h</kbd>/<kbd>j</kbd>/<kbd>k</kbd> move primary light source
* <kbd>m</kbd> toggle shadows
* <kbd>n</kbd> toggle shadow volumes
* <kbd>F1</kbd> toggle primary light source
* <kbd>F2</kbd> toggle ball light source
* <kbd>q</kbd>/<kbd>ESC</kbd> quit the game

![Pong 3D](https://github.com/chrisbloecker/cg/blob/master/img/pong-3d-1.png?raw=true)
![Pong 3D](https://github.com/chrisbloecker/cg/blob/master/img/pong-3d-2.png?raw=true)

### 06 - Cube
Algorithmic generation of a cube and its meshes. The cube can be rotated by dragging with the left mouse button pressed. Zooming can be done by dragging with the right mouse button pressed.

Key bindings:
* <kbd>h</kbd> toggle help
* <kbd>+</kbd>/<kbd>-</kbd>
* <kbd>l</kbd> toggle wireframe mode
* <kbd>a</kbd>/<kbd>d</kbd> jump through vertices
* <kbd>c</kbd> assign random colour to selected vertex
* <kbd>s</kbd> toggle vertices
* <kbd>q</kbd>/<kbd>ESC</kbd> quit

![Cube](https://github.com/chrisbloecker/cg/blob/master/img/cube.png?raw=true)

### 07 - Interpolation
This program interpolates curves between points using one of the types of interpolation:
* b-spling
* bezier curve
* hermite
* catmull rom

New points can be introduced with a right click. Existing points can be picked with a right click and be dragged while the left mouse button is held down.

Key bindings:
* <kbd>+</kbd>/<kbd>-</kbd> increase/decrease number of curve pieces between pairs of points
* <kbd>p</kbd>/<kbd>o</kbd> add/remove point
* <kbd>n</kbd> toggle normals
* <kbd>b</kbd> switch interpolation
* <kbd>c</kbd> toggle convex hull/interpolation
* <kbd>q</kbd>/<kbd>ESC</kbd> quit

![Interpolation](https://github.com/chrisbloecker/cg/blob/master/img/interpolation-1.png?raw=true)
![Interpolation](https://github.com/chrisbloecker/cg/blob/master/img/interpolation-2.png?raw=true)

## Image Processing
These programs are gimp plugins. Examples of their effects are shows on the standard image processing image of Lenna.

![Lenna](https://github.com/chrisbloecker/cg/blob/master/img/Lenna.jpg?raw=true)

### 08 - Histogram Transformations
The plugin can be used to perform the following histogram transformations:
* linear adjustment
* exponential adjustment
* equalisation

![Equalised version of Lenna](https://github.com/chrisbloecker/cg/blob/master/img/Lenna-equalised.jpg?raw=true)

### 09 - Edge Detection
The plugin can be used to run the following types of edge detectors over an image:
* sobel in x or y direction
* combined sobel
* mexican hat

The boundary problem is solved by treating edges with one of the following strategies
* constant background colour
* constant continuation
* periodic continuation

![Edges as detected using mexican hat filter on Lenna](https://github.com/chrisbloecker/cg/blob/master/img/Lenna-mexican-hat.jpg?raw=true)

### 10 - Embossing
The plugin performs emboss filtering and pencil sketch.

![Emboss filter applied to Lenna](https://github.com/chrisbloecker/cg/blob/master/img/Lenna-emboss.jpg?raw=true)
