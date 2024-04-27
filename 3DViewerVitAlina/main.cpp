#include <QApplication>

#include "Controllers/mainviewcontroller.h"
#include "Models/objectmodel.h"
#include "Views/mainview.h"

int main(int argc, char *argv[]) {
  setenv("LC_NUMERIC", "C", 1);
  QApplication a(argc, argv);
  QCoreApplication::setApplicationVersion(QT_VERSION_STR);

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setSamples(16);
  QSurfaceFormat::setDefaultFormat(format);
  QIcon icon(":/icons/planet-earth.png");
  QApplication::setWindowIcon(icon);
  MainView view;
  view.setFocus();
  ObjectModel model;
  SceneModel scene_model;
  MainViewController controller(&view, &model, &scene_model);
  view.show();
  return a.exec();
}
