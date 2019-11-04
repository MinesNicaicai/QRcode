#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
int grid_width = 80;
int grid_height = 80;
RNG rng(12345);

Mat src_corners;



/// Function header
void thresh_callback(int, void* );

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

void set_image_points(vector<Point> &img_pts);

Mat get_Hr(const vector<Point> &corner_pts, const vector<Point> &img_pts);
