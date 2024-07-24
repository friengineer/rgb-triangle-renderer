# RGB Triangle Renderer
A program that, when given 3 coordinates as corners, draws a triangle using pixels where each corner pixel is either red, green or blue and the remaining pixels' colours are interpolated to create a smooth RGB spectrum.

The coordinate of each pixel in the window is logged to a text file including its Barycentric coordinate in relation to the drawn triangle and whether the pixel is inside or outside of the triangle. The generated triangle is also saved as a PPM image.

The program can also draw a line of pixels between a given set of start and end coordinates and create a smooth colour transition along the line.

## Compiling the program
In the terminal, navigate to the directory that contains this README file and type
```shell
$ make
```
This will create a new executable file.

## Executing the program
In the same directory in the terminal, type
```shell
$ ./pixelate
```
and the program will start executing.
