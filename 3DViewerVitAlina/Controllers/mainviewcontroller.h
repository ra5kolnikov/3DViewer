#ifndef MAINVIEWCONTROLLER_H
#define MAINVIEWCONTROLLER_H

#include <QObject>

#include "Commands/processNewFileCommand.h"
#include "Models/scenemodel.h"
#include "Models/objectmodel.h"
#include "ThirdParty/QtGifImage/gifimage/qgifimage.h"
#include "Views/mainview.h"

class MainViewController : public QObject {
  Q_OBJECT

 public:
     MainViewController(MainView *View, ObjectModel *Model, SceneModel *scene_model);
     ~MainViewController();

 private slots:
  void onSelectFileButtonClicked(const QString &fileName);
  void onSelectMaterialButtonClicked(const QString &fileName);
  void onLoadTexturesClicked(const QString &fileName);
  void scaleSliderChangePos(const double scale);
  void onBGColorClicked(QColor color);
  void onVertexColorClicked(QColor color);
  void onEdgeColorClicked(QColor color);
  void onFaceColorClicked(QColor color);
  void onLightColorClicked(QColor color);
  void updateWidgetConfig();
  void vertexSizeValueChanged(int size);
  void edgeSizeValueChanged(int size);
  void xPosSliderMoved(int position);
  void yPosSliderMoved(int position);
  void zPosSliderMoved(int position);
  void xRotationChanged(int angle);
  void yRotationChanged(int angle);
  void zRotationChanged(int angle);
  void onEdgeTypeChanged(int type);
  void onVertexTypeChanged(int type);
  void onFaceTypeChanged(int type);
  void onProjectionTypeChanged(int type);
  void onSaveGifButtonCklicked();
  void onSaveImageButtonCklicked();
  void keyReleased(int key);
  void onResetButtonClicked();
  void onLightCheckBoxClicked(int state);
  void onTextureCheckBoxClicked(int state);
  void onMaterialCheckBoxClicked(int state);
  void onAmbientLightChanged(double value);
  void onDiffuseLightChanged(double value);
  void onSpecularLightChanged(double value);

  private:
  std::vector<Command *> command_history_;
  MainView *view_;
  Scene *scene_;
  ObjectModel *objectModel_;
  SceneModel *scene_model_;
};

#endif  // MAINVIEWCONTROLLER_H
