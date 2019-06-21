# Old Physics Engine Demos

Here are some simple executable programs utilizing and demonstrating the capabilities of a (deceased) physics engine that I made a while ago, which I am trying to replicate and extend in Brazen.

**Disclaimer**: Although all software very well should be free of malware, I cannot for legal reasons offer any guarantees/warranty.

## Getting Started

Download and run any of the programs in the [downloads](#Downloads) section. Unfortunately, they are only compiled for Windows and have only been tested in Windows 10.

### Usage Instructions

#### Maneuvering the Virtual Camera

**W** - move the camera in the direction it is facing
**S** - move the camera in the reverse of the direction it is facing
**A** - move the camera to the left along a vector that lies in the xy-plane and is orthogonal to the camera's direction
**D** - move the camera to the right along the vector as described for 'A'
**Left Shift** - move the camera downward along a vector that is orthogonal to both the camera's direction and the vector described for 'A'
**Space** - move the camera upward along the vector described for 'Left Shift'
**R** - reset the state of the camera(s)
**Escape** - toggle display interactivity

**Mouse drag** - rotates the camera about the vectors described for 'A' and 'Left Shift'
**Mouse scroll** - adjusts the zoom of the camera ...sort of
**Mouse right click + scroll** - adjust the distance between virtual cameras if in stereoscopic mode
**Mouse left click** - make the display interactive if it is not already

## Downloads

Download the first two prerequisites and then try running any of the example programs. If there are any issues regarding the Visual Studio Redistributable, try running the third prerequisite. Alternatively, you can just download a [zip archive](old-physics-engine.zip) containing all the files.

### Prerequisites

`SDL2.dll` and `circle.bmp` must be in the same folder as the programs in order to run them.

* [circle.bmp](circle.bmp)
* [SDL DLL](SDL2.dll) (Credit [Simple Directmedia Layer authors](https://www.libsdl.org/credits.php))
* [Microsoft Visual C++ Runtime Installer](VC_redist.x64.exe) (May be required to run the programs)

### Example Programs

* [Stereoscopic Demo](Demo.exe)
* [Cubes](Cubes.exe)
* [HyperCubes](HyperCubes.exe)
* [Glitchy Hypercubes](Glitchy-Hypercubes.exe) (not intentionally glitchy)
* [Dice](Dice.exe)
* [Bouncy Cube](Bouncy-Cube.exe)
* [Gelatin Cube](Gelatin-Cube.exe) (also not perfect)
* [Slinky Cube](Slinky-Cube.exe)
* [Ragdoll](Ragdoll.exe)