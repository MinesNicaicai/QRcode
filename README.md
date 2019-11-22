# QRcode

## Dependencies:

* OpenCV 4

* CMake minimum 2.8

* Test passed on Linux (Ubuntu 18.04)

## Get started:

To build the project:

`git clone https://github.com/MinesNicaicai/QRcode.git`

`mkdir build && cd build`

`cmake ..`

`make`

To run the program:

`./QRparser <input_image> <output_file>`

for example:

`./QRparser 6_LI_Yanhao.bmp output.txt`

and then check the output file "output.txt".

Or you can run the program in debug mode:

`DEBUG=1 ./QRparser <input_image> <output_file>`

with more details in the process of recognizing a QR code.



