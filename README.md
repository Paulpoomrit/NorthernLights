# NorthernLights
Basic PPM image processor

### Features
- **Image Filters**: Apply blur, sharpen, edge detection, and emboss filters
- **Grayscale Conversion**: Convert color images to grayscale
- **Geometric Transformations**: Rotate, scale, and translate images
- **Special Filters**: Floyd-Steinberg dithering and in-place emboss (this was, at first, a mistake but looks cool enough, so we've decided to add it here as well).

![image](https://github.com/user-attachments/assets/1c868d4d-4a54-42cb-8d1c-7216ccbc4662)
![Screenshot 2025-04-09 at 12 18 15 PM](https://github.com/user-attachments/assets/b4d16be8-8052-4cad-8599-27d5460dda01)

### Demo:
https://github.com/user-attachments/assets/904172f3-b265-45ca-9c3f-9f3dc96c8526

### Notes
We've decided to differ from the instructions on two points:

1. We made the PPM image object parameter to all of the functions in graphics a `const` reference, this means that we'll create a new object within the function itself and return (and being saved later as another separate file) and not modify the original pixel vector in place ( i.e., O(n) space); however, we'd argue the tradeoff is worth it to make the software safer for the client, since if they mess up they'd still the original copy.

2. Follows from that, to prevent dangling reference, we'll make the return type a `PPM` (i.e., return by value, not by reference). Again, another potential bottleneck when it comes to performance (Kevin pointed out that in later versions of C++, the optimization might be good enough to the point this might not even be a problem), but better for overall usability (we'd argue here that making it hard for clients to misuse these functions, i.e., provide wrong params and break things halfway, is more important than having 1s faster implementation).

### Compilation
Navigate to the project directory and run:

-> On Mac:
`clang++ -o program src/Graphics.cpp src/Pixel.cpp src/PPM.cpp -std=c++17`

-> On Windows:
`g++ -o program src/Graphics.cpp src/Pixel.cpp src/PPM.cpp -std=c++17`

This will compile all the source files and create an executable called program.

### Running the Program
After compilation, run the program:
`./program`

You'll be prompted to:
1. Enter the path to an input PPM file
2. Select an operation to perform
3. Provide any additional parameters required for the selected operation
4. Specify where to save the resulting image
