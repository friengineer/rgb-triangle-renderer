#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QLabel>
#include <QDesktopWidget>
#include "pixelwidget.hpp"

// make main window appear bigger by default
class MyMainWindow : public QMainWindow {

  public: 
    
    MyMainWindow();

  private:

};

MyMainWindow::MyMainWindow()
{
  resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
}

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  MyMainWindow window;

  // place widgets and draw on them 
  PixelWidget w(70,70);
  window.setCentralWidget(&w);
  window.show();

  return app.exec();
}
