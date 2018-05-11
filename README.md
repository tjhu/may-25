# may-25
A 3D visualization for [Solid of Revolution](https://en.wikipedia.org/wiki/Solid_of_revolution) and [Solids with Known Cross-Sections](http://www.ccsdk12.org/mclemens/courses/APcalcAssign/IA-8.pdf) in Calculus.

## Features:

### Solid of Revolution:
* Disk method
* Washer method

### Solids with Known Cross-Sections
* Semicircles
* Equilidual Triangles
* Squares

### Custom functions, boundaries, and number of solids

## How to run it
Go to [release page](https://github.com/gitletH/may-25/releases/latest) and download binary build (may-25.exe) and run it.

## How to debug it
Open `May 25.sln` with Visual Studio 2017 then build and run/debug it. 
### Development dependencies
* Windows SDK 10.0.16299.0
* VC++ 2017 v141 toolset
* MFC support (required)
You can adjust Windows SDK version or VC++ version, but you must have MFC support. These components can be installed via Visual Studio Installer.

## Supported platforms:
* Windows 8
* Windows 8.1
* Windows 10 (tested)

## How to read the code
All the source code are under "May 25/"

| Key function | Where it is |
| ------------- | ------------- |
| entry point  | wWinMain @ D3D11App.cpp  |
| rendering | Render @ D3D11App.cpp |
| shader | hlsl/LightShader.hlsl |
| handwriting math | MathInput.cpp |
| UI | MyWindowsProcs.cpp |
| geometry building  | Geometry.cpp  |
| math expression parsing/evaluation| EvaluateExpression/tmath.cpp |

## Screenshots

### Coolest picture from development
![alt text](https://github.com/gitletH/may-25/blob/master/screenshot/log_func.png?raw=true)


### Screenshot from latest build
![image](https://user-images.githubusercontent.com/18585305/39908199-dab1feb2-54a1-11e8-8637-ef2b18083ad9.png)

## Reference books
* Programming Windows 5th Edition by Charles Petzold
* Introduction to 3D Game Programming with DirectX 11 by Frank D. Luna

## Something I need to say
Most of the code was rushed to write during the last month of my high school. I did not know much about programming and OOP back then and I tried my best to copy and paste craps that I can find over the internet while making sure that the code compiles. If the code blinds your eyes, I am sorry about that.
<br />
<br />
Many times, I planned to refactor it. But because both style and design of the code is so garbage and it use some old-school API like win32 (Programming Windows 5E was published in 1998), I do not have the gut to refactor it. I plan to rewrite it completly in the future though, probably for Web or UWP. 
