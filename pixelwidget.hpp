#ifndef _PIXEL_WIDGET_
#define _PIXEL_WIDGET_

#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QLabel>
#include <QDesktopWidget>
#include <iostream>
#include <vector>
#include "RGBVal.hpp"

class PixelWidget : public QWidget {
public:

  // set the number of pixels that the widget is meant to display
  PixelWidget
  (
   unsigned int n_horizontal, // the first integer determines the number of horizontal pixels
   unsigned int n_vertical    // the second integer determines the number of vertical pixels
   );

  // sets a pixel at the specified RGBVal value; ignores non-existing pixels without warning
  void SetPixel
  (
   unsigned int  i_x, // horizontal pixel coordinate
   unsigned int  i_y, // vertical pixel coordinate
   const RGBVal& c    // RBGVal object for RGB values
    );

  void DefinePixelValues();

  // draws the line between the specified coordinates using pixels and interpolates the
  // colour of the pixels along the line
  void DrawLine
  (
    float start_x,
    float start_y,
    float end_x,
    float end_y,
    RGBVal start_colour,
    RGBVal end_colour
  );

  // draws and fills in a triangle specified by the coordinates and interpolates the
  // colour of the pixels in the triangle
  void DrawTriangle
  (
    float x_1,
    float y_1,
    float x_2,
    float y_2,
    float x_3,
    float y_3,
    RGBVal colour_1,
    RGBVal colour_2,
    RGBVal colour_3
  );

  // checks if a given pixel is inside or outside the triangle defined by the given points
  bool IsInside
  (
    float x_1,
    float y_1,
    float x_2,
    float y_2,
    float x_3,
    float y_3,
    int point_x,
    int point_y
  );

  void TestNormal
  (
    float x_1,
    float y_1,
    float x_2,
    float y_2,
    int point_x,
    int point_y
  );

protected:

  virtual void paintEvent(QPaintEvent*);


private:

  unsigned int _n_vertical;
  unsigned int _n_horizontal;

  std::vector<std::vector<RGBVal> > _vec_rects;
};

#endif
