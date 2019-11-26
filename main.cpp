#include "main.hpp"

/** @function main */
int main( int argc, char** argv ){
  DEBUG = false;

  string src_path = argv[1];
  string dst_path = argv[2];

  // src_path = "../6_LI_Yanhao.bmp";
  // src_path = "../5_GU_Yuanzhe.bmp";
  // src_path = "../16_BIAD_Zine-Eddine.bmp";
  // src_path = "../1_JING_Ge.bmp";
  
  
  
  // dst_path = "out.txt";

  char* parse_debug = getenv ("DEBUG");
  if (parse_debug!=NULL){
    DEBUG = string(parse_debug).compare("1") == 0 ? true : false;
  }
  

  /// Load source image and convert it to gray
  src = imread(src_path, 1);

  const clock_t begin_time = clock();

  // Convert the source image into a binary matrix
  code_recognition(src, out_matrix);

  const clock_t finish_time = clock ();


  std::cout << float( finish_time - begin_time ) /  CLOCKS_PER_SEC * 1000 << " ms" << endl;


  // Output the results to a file
  output_result(src_path, dst_path);

  waitKey(0);
  return(0);
}




/** @function thresh_callback */
void code_recognition(Mat src, unsigned int out_matrix[QRsize][QRsize]){
  
  Mat canny_output;
  Mat bin_output;
  vector< vector<Point> > contours;
  vector<Vec4i> hierarchy;


  cvtColor( src, src_gray, COLOR_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );
  
  double thres = threshold(src_gray, bin_output, 0, 255, THRESH_OTSU);
  /// Detect edges using canny
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( bin_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Find corner candidates
  vector<int> corner_candidates;
  bool is_child = false;
  int count_childs;
  int parent_index;
  for (int i = 0; i < contours.size(); i++){
    if (!is_child && hierarchy[i][2] != -1){
      parent_index = i;
      is_child = true;
      count_childs = 0;
    }
    else if (is_child && hierarchy[i][2] != -1){
      count_childs++;
    }
    else if (is_child && hierarchy[i][2] == -1){
      is_child = false;
      if (count_childs >= 1){
        corner_candidates.push_back(parent_index);
      }
    }
  }


  /// Draw contours and detect corners 

  vector<Point> corner;
  vector<Point> corner_pts;
  for( int i = 0; i < corner_candidates.size(); i++ ){
    src_corners = Mat::zeros(canny_output.size(), CV_8UC1);
    corner.clear();
    Scalar color = Scalar(200);
    drawContours(src_corners, contours, corner_candidates[i], color, 1, 8 );
    goodFeaturesToTrack(src_corners, corner, 4, 0.01, 20, noArray(), 3, false, 0.04 );
    for (int j = 0; j < 4; j++)
      corner_pts.push_back(corner[j]);
  }



  /// Corner detectors


  for (int i = 0; i < corner_pts.size(); i++){
    circle( src, corner_pts.at(i), 4,  Scalar(200), 2, 8, 0 );
  }

  // Sort the 12 points to be in order
  resort_corners(corner_pts);


  // Convert the corner points from Point2i to Point2f
  vector<Point2f> corner_pts_f;
  for(int i = 0; i < corner_pts.size(); i++){
    corner_pts_f.push_back(Point2f(corner_pts[i].x, corner_pts[i].y));
  }

  // Set the destination points after homography transformation  
  vector<Point2f> img_pts_f;
  set_dst_points(img_pts_f);

  // Do the homography transformation
  Mat Hr = findHomography(corner_pts_f, img_pts_f, 0, 3);
  Mat dst;
  warpPerspective(bin_output, dst, Hr, src.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar());

  // Transform the regulated binary image to a 42x42 binary matrix 
  transformToMatrix(dst, out_matrix);
  

  if (DEBUG){
    cout << "M = "<< endl << " "  << Hr << endl << endl;
    namedWindow( "Binary QR code", WINDOW_AUTOSIZE );
    imshow( "Binary QR code", src );
    // namedWindow( "Transformed QR code", WINDOW_AUTOSIZE );
    // imshow( "Transformed QR code", dst );
  }




  if (DEBUG){
    for (int i = 0; i <= QRsize; i++){
      line(dst, Point(origin_x + i * grid_width, 0), Point(origin_x + i * grid_width, 1176), Scalar(127));
      line(dst, Point(0, origin_y + i * grid_height), Point(1176, origin_y + i * grid_height), Scalar(127));

    }
    namedWindow( "Transformed QR code", WINDOW_AUTOSIZE);
    imshow("Transformed QR code", dst);
  }


  if (DEBUG){
    for (int j = 0; j < QRsize; j++){
      for (int i = 0; i < QRsize; i++){
        cout << out_matrix[i][j] << ", ";
      }
      cout << endl;
    }
  }
  

}


/** @function resort_corners */
void resort_corners(vector<Point> &points){
  /** 
   * input: points = 12 x Point, 0-3, 4-7, 8-11 are three groups of points 
   * belonging to three corners.
   * First we reorder the three corners as follows:
   *  c0-----------c2       -----> x
   *  |          /          |   
   *  |        /            | 
   *  |      /              v  y
   *  |    /
   *  |  /
   *  c1
   * 
   * Then we reorder the 4 points of each corner
   *  p0----p1     p8----p9  
   *  |     |      |     |
   *  |     |      |     |
   *  p2----p3     p10---p11
   * 
   * 
   *  p4----p5 
   *  |     |
   *  |     |
   *  p6----p7
  */
  sort_corners(points);
  
  int pos[12] = {};

  sort_corner0(points, pos);
  sort_corner2(points, pos);
  sort_corner1(points, pos);

  reorder_array(points, pos);
}

int get_distance(Point &p1, Point &p2){
  Point p = p1 - p2;
  return p.dot(p);
}

template <typename T>
void sort_indexes(vector<T>& array, int* pos){
  /** 
   *  Sort the array and give the indices of sorted elements
   */
  for (int i = 0; i < array.size(); i++){
    pos[i] = i;
  }
  for (int i = array.size() - 2; i >= 0; i--){
    for (int j = i; j < array.size() - 1; j++){
      if (array[j] > array[j + 1]){
        swap(array[j], array[j + 1]);
        swap(pos[j], pos[j + 1]);
      }
    }
  }
}

template <typename T>
void reorder_array(vector<T> &array, int* order){
  /** 
   *  Reorder the array based on the given order
   *  e.g. array = [2,5,8,9] order = [1,2,3,0]
   *       then after reordering
   *       array = [5,8,9,2]
   */
  vector<T> array_out;
  array_out.resize(array.size());
  for (int i = 0; i < array_out.size(); i++){
    array_out[order[i]] = array[i];
  }
  array = array_out;
}

void sort_corners(vector<Point> &points){

  vector<Point> centers;
  vector<Point> vectors;

  for (int i = 0; i < 3; i++){
    centers.push_back(get_center(points, i));
  }
  
  
  vectors.push_back(centers[2] - centers[1]);
  vectors.push_back(centers[0] - centers[2]);
  vectors.push_back(centers[1] - centers[0]);



  int norm2, norm_max = 0;
  int index;
  // Find the longest edge of the triangle corner0-corner1-corner2
  for (int i = 0; i < vectors.size(); i++){
    norm2 = vectors[i].ddot(vectors[i]);
    if (norm2 > norm_max){
      index = i;
      norm_max = norm2;
    }
  }

  // Swap the 0-th group of points and index-th group of points
  if (index != 0){
    swap(vectors[0], vectors[index]);
    for (int i = 0; i < 4; i++){
      swap(points[i], points[4 * index + i]);
    }
  }


  // Swap the 1st group of points and 2nd group of points
  if (vectors[1].cross(vectors[2]) > 0){
    for (int i = 0; i < 4; i++){
      swap(points[4 + i], points[4 * 2 + i]);
    }
  }
}

void sort_corner0(vector<Point> &points, int* pos){
  /**  Sort the 4 points of corner0 (the top left corner)
   *   p0----p1
   *   |     |
   *   |     |
   *   p2----p3
   *
   */ 

  int center_x = 0;
  int center_y = 0;

  // c1 is the barycenter of corner 1 (bottom left corner)
  for (int i = 4; i < 8; i++){
    center_x += points[i].x;
    center_y += points[i].y;
  }
  Point c1(center_x, center_y);

  /// c2 is the barycenter of corner 2 (top right corner)
  center_x = 0;
  center_y = 0;
  for (int i = 8; i < 12; i++){
    center_x += points[i].x;
    center_y += points[i].y;
  }
  Point c2(center_x, center_y);

  /// dist_p2c is the distance from a point of corner 0 (top left corner) to corner 1 or 2
  vector<int> dist_p2c;


  int pos_local[4];

  // The points of corner 0 closer to corner 1 are either p2 or p3 
  for (int i = 0; i < 4; i++){
    dist_p2c.push_back(get_distance(points[i], c1));
  }
  sort_indexes(dist_p2c, pos_local);
  for (int i = 0; i < 2; i++){
    pos[pos_local[i]] = 2;
  }
  
  // The points of corner 0 closer to corner 2 are either p0 or p2 
  for (int i = 0; i < 4; i++){
    dist_p2c[i] = get_distance(points[i], c2);
  }
  sort_indexes(dist_p2c, pos_local);
  for (int i = 0; i < 2; i++){
    pos[pos_local[i]] += 1;
  }
}

void sort_corner1(vector<Point> &points, int* pos){

  /**  Sort the 4 points of corner1 (the bottom left corner)
   *   p4----p5
   *   |     |
   *   |     |
   *   p6----p7
   *
   */  
  Point c0 = points[0];
  Point c2 = points[8];

  // dist_p2c is the distance from a point of corner 0 (top left corner) to corner 1 or 2
  vector<int> dist_p2c;
  int pos_local[4];


  for (int i = 0; i < 4; i++){
    dist_p2c.push_back(get_distance(points[i + 4], c0));
  }
  sort_indexes(dist_p2c, pos_local);
  for (int i = 0; i < 4; i++){
    pos[pos_local[i] + 4] = i < 2 ? 4 : 7;
  }


  for (int i = 0; i < 4; i++){
    dist_p2c[i] = get_distance(points[i + 4], c2);
  }
  sort_indexes(dist_p2c, pos_local);

  pos[pos_local[0] + 4] = 5;
  pos[pos_local[3] + 4] = 6;

}

void sort_corner2(vector<Point> &points, int* pos){

  /**  Sort the 4 points of corner2 (the top right corner)
   *   p8-----p9
   *   |      |
   *   |      |
   *   p10----p11
   *
   */  

  // The top left point of corner 0 (top left corner)
  Point c0 = points[0];

  // The bottom left point of corner 1 (bottom left corner)
  Point c1 = points[6];

  // Distance from a point of corner 2 to another corner (corner 0 or 1)
  vector<int> dist_p2c;

  // The posit
  int pos_local[4];


  for (int i = 0; i < 4; i++){
    dist_p2c.push_back(get_distance(points[i + 8], c0));
  }
  sort_indexes(dist_p2c, pos_local);
  for (int i = 0; i < 4; i++){
    pos[pos_local[i] + 8] = i < 2 ? 8 : 11;
  }


  for (int i = 0; i < 4; i++){
    dist_p2c[i] = get_distance(points[i + 8], c1);
  }
  sort_indexes(dist_p2c, pos_local);

  pos[pos_local[0] + 8] = 10;
  pos[pos_local[3] + 8] = 9;

}

Point get_center(vector<Point> &points, int group){
  int x = 0, y = 0;
  Point p;
  for (int i = 0; i < 4; i++){
    p = points[i + group * 4];
    x += p.x;
    y += p.y;
  }
  return Point(x/4, y/4);
}

void set_dst_points(vector<Point2f> &img_pts){

  vector<Point2f> square;
  square.push_back(Point2f(0, 0));
  square.push_back(Point2f(6 * grid_width, 0));
  square.push_back(Point2f(0, 6 * grid_height));
  square.push_back(Point2f(6 * grid_width, 6 * grid_height));

  img_pts.clear();
  for (int j = 0; j < 4; j++){
    img_pts.push_back(square[j] + Point2f(origin_x, origin_y));
  }
  for (int j = 0; j < 4; j++){
    img_pts.push_back(square[j] + Point2f(origin_x, origin_y + 36 * grid_height));
  }
  for (int j = 0; j < 4; j++){
    img_pts.push_back(square[j] + Point2f(origin_x + 36 * grid_width, origin_y));
  }
  
}

void transformToMatrix(Mat in_bin_img, unsigned int out_matrix[QRsize][QRsize]){
  Mat tmp;
  for (int i = 0; i < QRsize; i++){
    for (int j = 0; j < QRsize; j++){
      tmp = in_bin_img(Rect(origin_x + i * grid_width, origin_y + j * grid_height, 
                              grid_width, grid_height));

      double m = mean(tmp).val[0];
      out_matrix[i][j] = m > 128 ? 1 : 0;
    }
  }
}  

void output_result(string src, string dst){
  /**
   * Output the matrix to a file
   */
  ofstream dst_file;
  dst_file.open (dst);
  dst_file << src << "\n\n";
  for (int j = 0; j < QRsize; j++){
    for (int i = 0; i < QRsize; i++){
      dst_file << out_matrix[i][j] << ", ";
    }
    dst_file << endl;
  }
  dst_file.close();
}