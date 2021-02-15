# EmbCV
Embedded CV Libraries

This is more of a C only image processing library

Will eventually have many img processing techniques integrated with examples

download and install gcc,make,CMAKE and nasm, to use examples

The libraries themselves (inside the src folder) are made to not require any OS level libraries.

Images fed into the base libraries should use the img_buf struct defined in the base_methods.h file
w = width of the picture in pixels
h = height of the picture in pixels
c = # of channels, set to 1 or 3 depending on greyscale or rgb
data* = pointer to memory location of image data.
Regardless of how it's read in, it expects the img data buffer to arranged columns counting from left to right, rows counting down from the top of the image
these libraries do not account for RGB arrangement (i.e. if it's RGB or GBR or BGR)
These libraries will attempt to copy an image as little as possible. This is usually not possible for many img processing techniques, but when it is, it overwrites the input image.

clone with
git clone --recurse-submodules
or if already cloned, run:
git submodule update --init --recursive

