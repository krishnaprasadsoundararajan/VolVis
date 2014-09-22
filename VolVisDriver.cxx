#include <QApplication>
#include "VolVis.h"

int main( int argc, char** argv )
{
  // QT Stuff

  QApplication app( argc, argv );
  VolVis VolVis( argc, argv);
  VolVis.show();
  
  
  return app.exec();
}
