# QRcode

## Dependencies:

* OpenCV 4

* CMake minimum 2.8

* Test passed on Linux (Ubuntu 18.04)

## Get started:

### Linux

To build the project:

```
git clone https://github.com/MinesNicaicai/QRcode.git

mkdir build && cd build

cmake ..

make
```

To run the program:

```
./QRparser <input_image> <output_file>
```

for example:

```
./QRparser 6_LI_Yanhao.bmp output.txt
```

and then check the output file "output.txt".

Or you can run the program in debug mode:

```
DEBUG=1 ./QRparser <input_image> <output_file>
```

with more details in the process of recognizing a QR code.

### Windows

To run the program in cmd:

```
QRparser.exe  <input_image> <output_file>
```

for example:

```
QRparser.exe  6_LI_Yanhao.bmp output.txt
```

Or you can set DEBUG=1 to run the program in debug mode:

```
set DEBUG=1

QRparser.exe  6_LI_Yanhao.bmp output.txt
```





