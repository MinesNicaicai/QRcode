# QRcode

## Dependencies:

* OpenCV 4

* CMake minimum 2.8

* Test passed on Linux (Ubuntu 18.04)

## Getting started:

The QR code images are placed in `image` folder, and executable 
files for linux and windows systems are placed in `bin` folder.

### Linux

To build the project:

```
git clone https://github.com/MinesNicaicai/QRcode.git

mkdir build && cd build

cmake ..

make
```

To run the executable file (in `bin` folder):

```
./QRparser <input_image> <output_file>
```

for example:

```
./QRparser qrcode01.bmp output.txt
```

and then check the output file "output.txt".

Or you can run the program in debug mode:

```
DEBUG=1 ./QRparser qrcode01.bmp output.txt
```

with more details in the process of recognizing a QR code.

### Windows

To run the executable file in cmd:

```
> QRparser.exe  <input_image> <output_file>
```

for example:

```
> QRparser.exe  qrcode01.bmp output.txt
```

Or you can set DEBUG=1 to run the program in debug mode:

```
set DEBUG=1

> QRparser.exe  qrcode01.bmp output.txt
```





