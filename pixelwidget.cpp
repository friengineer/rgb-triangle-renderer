#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QLabel>
#include <QDesktopWidget>
#include <iostream>
#include <fstream>
#include "pixelwidget.hpp"

using namespace std;

void PixelWidget::DefinePixelValues(){
  SetPixel(10,30,RGBVal(255,0,0));
  SetPixel(10,60,RGBVal(0,255,0));
}

// draws the line between the specified coordinates using pixels and interpolates the
// colour of the pixels along the line
void PixelWidget::DrawLine(float start_x, float start_y, float end_x, float end_y,
  RGBVal start_colour, RGBVal end_colour){

  float t = 0.01;

  for (float i = 0; i <= 1; i += t){
    float x = ((1 - i) * start_x) + (i * end_x);
    float y = ((1 - i) * start_y) + (i * end_y);

    float red = ((1 - i) * start_colour._red) + (i * end_colour._red);
    float green = ((1 - i) * start_colour._green) + (i * end_colour._green);
    float blue = ((1 - i) * start_colour._blue) + (i * end_colour._blue);

    SetPixel(round(x),round(y),RGBVal(round(red),round(green),round(blue)));
  }

  SetPixel(round(end_x),round(end_y),RGBVal(end_colour._red,end_colour._green,end_colour._blue));
}

// draws and fills in a triangle specified by the coordinates and interpolates the
// colour of the pixels in the triangle
void PixelWidget::DrawTriangle(float x_1, float y_1, float x_2, float y_2, float x_3, float y_3,
  RGBVal colour_1, RGBVal colour_2, RGBVal colour_3){

  float t = 0.01;
  float s = 0.01;

  for (float i = 0; i <= 1; i += t){

    for (float j = 0; j <= 1; j += s){
      float x = ((1 - j) * (1 - i) * x_1) + ((1 - j) * i * x_2) + (j * x_3);
      float y = ((1 - j) * (1 - i) * y_1) + ((1 - j) * i * y_2) + (j * y_3);

      float red = ((1 - j) * (1 - i) * colour_1._red) + ((1 - j) * i * colour_2._red) + (j * colour_3._red);
      float green = ((1 - j) * (1 - i) * colour_1._green) + ((1 - j) * i * colour_2._green) + (j * colour_3._green);
      float blue = ((1 - j) * (1 - i) * colour_1._blue) + ((1 - j) * i * colour_2._blue) + (j * colour_3._blue);

      SetPixel(round(x),round(y),RGBVal(round(red),round(green),round(blue)));
    }
  }

  // save generated triangle as PPM image file
  ofstream file;
  file.open("triangle.ppm");

  float largest_x = max(max(x_1,x_2),x_3);
  float largest_y = max(max(y_1,y_2),y_3);
  float smallest_x = min(min(x_1,x_2),x_3);
  float smallest_y = min(min(y_1,y_2),y_3);

  file << "P3  " << ((ceil(largest_x) - floor(smallest_x)) + 1) << "  " << ((ceil(largest_y) - floor(smallest_y)) + 1) << "  255\n";

  for (unsigned int i_row = floor(smallest_y); i_row <= ceil(largest_y); i_row++){

    for(unsigned int i_column = floor(smallest_x); i_column <= ceil(largest_x); i_column++){
      file << _vec_rects[i_column][i_row]._red << "\n";
      file << _vec_rects[i_column][i_row]._green << "\n";
      file << _vec_rects[i_column][i_row]._blue << "\n";
    }
  }

  file.close();
}

// checks if a given pixel is inside or outside the triangle defined by the given points
bool PixelWidget::IsInside(float x_1, float y_1, float x_2, float y_2, float x_3, float y_3,
  int point_x, int point_y){

  float side_1_x = x_2 - x_1;
  float side_1_y = y_2 - y_1;
  float side_2_x = x_3 - x_2;
  float side_2_y = y_3 - y_2;
  float side_3_x = x_1 - x_3;
  float side_3_y = y_1 - y_3;

  float normal_1_x = -side_1_y;
  float normal_1_y = side_1_x;
  float normal_2_x = -side_2_y;
  float normal_2_y = side_2_x;
  float normal_3_x = -side_3_y;
  float normal_3_y = side_3_x;

  if (((normal_1_x * ((float)point_x - x_1)) + (normal_1_y * ((float)point_y - y_1)) > 0) ||
  ((normal_2_x * ((float)point_x - x_2)) + (normal_2_y * ((float)point_y - y_2)) > 0) ||
  ((normal_3_x * ((float)point_x - x_3)) + (normal_3_y * ((float)point_y - y_3)) > 0)){
    return false;
  }

  return true;
}

void PixelWidget::TestNormal(float x_1, float y_1, float x_2, float y_2, int point_x, int point_y){
  float side_x = x_2 - x_1;
  float side_y = y_2 - y_1;
  float normal_x = -side_y;
  float normal_y = side_x;

  cout << "Vector: (" << side_x << ", " << side_y << ")\n";
  cout << "Normal: (" << normal_x << ", " << normal_y << ")\n";
  cout << "Above?: " << (normal_x * (float)point_x) + (normal_y * (float)point_y) << "\n";
}

PixelWidget::PixelWidget(unsigned int n_vertical, unsigned int n_horizontal):
  _n_vertical   (n_vertical),
  _n_horizontal (n_horizontal),
  _vec_rects(0)
{
  // all pixels are initialized to black
     for (unsigned int i_col = 0; i_col < n_vertical; i_col++)
       _vec_rects.push_back(std::vector<RGBVal>(n_horizontal));
}

void PixelWidget::SetPixel(unsigned int i_column, unsigned int i_row, const RGBVal& rgb)
{

  // if the pixel exists, set it
  if (i_column < _n_vertical && i_row < _n_horizontal)
    _vec_rects[i_column][i_row] = rgb;
}

void PixelWidget::paintEvent( QPaintEvent * )
{

  QPainter p( this );
  // no antialiasing, want thin lines between the cell
  p.setRenderHint( QPainter::Antialiasing, false );

  // set window/viewport so that the size fits the screen, within reason
  p.setWindow(QRect(-1.,-1.,_n_vertical+2,_n_horizontal+2));
  int side = qMin(width(), height());
  p.setViewport(0, 0, side, side);

  // black thin boundary around the cells
  QPen pen( Qt::black );
  pen.setWidth(0.);

  // here the pixel values defined by the user are set in the pixel array
  DefinePixelValues();
  DrawLine(20,30,20,60,RGBVal(255,0,0),RGBVal(0,255,0));

  float p_x = 30;
  float p_y = 20;
  float q_x = 60;
  float q_y = 20;
  float r_x = 40;
  float r_y = 0;

  DrawTriangle(p_x,p_y,q_x,q_y,r_x,r_y,RGBVal(255,0,0), RGBVal(0,255,0),RGBVal(0,0,255));

  // save the information of every pixel to a text file including coordinates, Barycentric
  // coordinates and if the pixel is inside or outside of the triangle
  ofstream file;
  file.open("Pixels.txt");

  file << "Barycentric coordinates calculated using the points P=(" << p_x << "," << p_y;
  file << "), Q=(" << q_x << "," << q_y << ") and R=(" << r_x << "," << r_y << ").\n\n";

  float determinant = ((p_x - r_x) * (q_y - r_y)) - ((q_x - r_x) * (p_y - r_y));

  for (unsigned int i_column = 0 ; i_column < _n_vertical; i_column++){
    for(unsigned int i_row = 0; i_row < _n_horizontal; i_row++){
      QRect rect(i_column,i_row,1,1);
      QColor c = QColor(_vec_rects[i_column][i_row]._red, _vec_rects[i_column][i_row]._green, _vec_rects[i_column][i_row]._blue);

      // fill with color for the pixel
      p.fillRect(rect, QBrush(c));
      p.setPen(pen);
      p.drawRect(rect);

      float alpha = ((((q_y - r_y) / determinant)) * ((float)i_column - r_x)) + ((-(q_x - r_x) / determinant) * ((float)i_row - r_y));
      float beta = ((-(p_y - r_y) / determinant) * ((float)i_column - r_x)) + ((((p_x - r_x) / determinant)) * ((float)i_row - r_y));
      float gamma = 1 - alpha - beta;

      file << "Pixel location: (" << i_column << "," << i_row << ")\n";
      file << "Barycentric coordinates: (" << alpha << "," << beta << "," << gamma << ")\n";
      file << "Pixel is ";

      if (IsInside(p_x,p_y,q_x,q_y,r_x,r_y,i_column,i_row)){
        file << "inside the triangle\n\n";
      }
      else{
        file << "outside the triangle\n\n";
      }
    }
  }

  file.close();
}
