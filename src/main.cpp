#include <QApplication>
#include <MainWindow.h>

/* this code runs the basic main window and is created by the Qt Creator app */
int main(int argc, char *argv[])
{
  //Make an instance of the QApplication
  QApplication a(argc, argv);

  //Create a new MainWindow
  MainWindow w;

  //Show it
  w.show();

  //Hand control over to Qt framework
  return a.exec();
}
