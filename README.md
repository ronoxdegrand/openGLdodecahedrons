# 3 dodecahedrons

## Execution instructions

1. copy the source folder into your directory with the installed libraries

2. Copy the `CMakeFiles.txt` over there as well

3. create a build folder over there and enter it from your command line

4. excecute as:
```
cmake ..
make
./a0
```
Here, a0 is the name of the executable.

## Controls

w/a/s/d: move camera in 2 axes

tab/e: move camera in third axis

arrows: move object in 2 axes

shift/space: move object in third axis

0: object revolves around axis

1/2/3: default camera positions

## Ps:

In the case of movement of the camera:

All inputs are converted to sin*cos functions so the camera can correctly revolve around the object. @ is multiplied simply to have distance from the object.

Since i have already implemented rotation in the movement, the logic behind it is:
* components of a unit vector are sin(x) and cos(x). Therefore to move around the x axis, the eye
* vector must be along the lines of (k, sin(x), cos(x))
* this is expanded to what is above.

I could have simply implemented a regular increment in the eye vector but i felt that this was more natural.
