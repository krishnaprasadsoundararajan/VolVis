#include <QApplication>
#include "VolVis.h"

int main( int argc, char** argv )
{
  // QT Stuff
  QApplication app( argc, argv );
  VolVis VolVis;
  VolVis.show();
  
  
  return app.exec();
}
