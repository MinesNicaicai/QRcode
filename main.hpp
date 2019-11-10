#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265

using namespace cv;
using namespace std;

// Input
Mat src;

// Output 
const int QRsize = 42;
unsigned int out_matrix[QRsize][QRsize];

// Other global variables
Mat src_gray;
int thresh = 100;
int max_thresh = 255;
int grid_width = 14;
int grid_height = 14;
int origin_x = 300;
int origin_y = 300;
RNG rng(12345);

Mat src_corners;

bool DEBUG = true;


/// Function headers
void code_recognition(Mat, unsigned int[QRsize][QRsize]);

void resort_corners(vector<Point> &, vector<int> & );

double get_rotation(Point);

int get_distance(Point &, Point &);

template <typename T>
void sort_indexes(vector<T>, int* );

void resort_corners(vector<Point> &);

template <typename T>
void reorder_array(vector<T> &array, int* order);

void sort_corners(vector<Point> &);

void sort_corner0(vector<Point> &, int*);

void sort_corner1(vector<Point> &, int*);

void sort_corner2(vector<Point> &, int*);

void set_dst_points(vector<Point2f> &img_pts);

void transformToMatrix(Mat, unsigned int[QRsize][QRsize]);

void output_result(string);

Point get_center(vector<Point> &, int);
