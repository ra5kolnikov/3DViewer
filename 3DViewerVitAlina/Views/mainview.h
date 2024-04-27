#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <qstring.h>

#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>

#include "Views/scene.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainView;
}
QT_END_NAMESPACE

class MainView : public QMainWindow {
  Q_OBJECT

 public:
     MainView(QWidget *parent = nullptr);
     ~MainView();
     void setData(QString filePath, int countVertices, int countFaces);
     Scene *getScene();
     QString models_dir_path_;

 protected:
     void keyPressEvent(QKeyEvent *event) override;
     void keyReleaseEvent(QKeyEvent *event) override;

 private:
     void onLoadTexturesClicked();
     void onResetButtonClicked();
     void setupConnections();
     void onSelectObjectButtonClicked();
     void onSelectMaterialButtonClicked();
     void onLightColorButtonClicked();
     void onBgColorButtonClicked();
     void onEdgeColorButtonClicked();
     void onVertexColorButtonClicked();
     void onFaceColorButtonClicked();
     void onEdgeSizeValueChanged(int size);
     void onVertexSizeValueChanged(int size);
     void xPosSliderMoved(int position);
     void yPosSliderMoved(int position);
     void zPosSliderMoved(int position);
     void xRotationChanged(int angle);
     void yRotationChanged(int angle);
     void zRotationChanged(int angle);
     void onEdgeTypeChanged();
     void onVertexTypeChanged();
     void onFaceTypeChanged();
     void onProjectionTypeChanged();
     void scale(double scale);
     void onSaveGifButtonCklicked();
     void onSaveImageButtonCklicked();
     void onLightSettingsChanged(double value);

 signals:
     void onAmbientLightChangedSignal(double value);
     void onDiffuseLightChangedSignal(double value);
     void onSpecularLightChangedSignal(double value);
     void onResetButtonClickedSignal();
     void onLoadTexturesClickedSignal(QString fileName);
     void onSelectObjectButtonClickedSignal(QString fileName);
     void onSelectMaterialButtonClickedSignal(QString fileName);
     void scaleValueChangedSignal(double scale);
     void setLightColorClickedSignal(QColor color);
     void setBGColorClickedSignal(QColor color);
     void setEdgeColorClickedSignal(QColor color);
     void setFaceColorClickedSignal(QColor color);
     void setVertexColorClickedSignal(QColor color);
     void vertexSizeValueChangedSignal(int size);
     void edgesSizeValueChangedSignal(int size);
     void xPosSliderMovedSignal(int position);
     void yPosSliderMovedSignal(int position);
     void zPosSliderMovedSignal(int position);
     void xRotationChangedSignal(int angle);
     void yRotationChangedSignal(int angle);
     void zRotationChangedSignal(int angle);
     void onEdgeTypeChangedSignal(int type);
     void onVertexTypeChangedSignal(int type);
     void onFaceTypeChangedSignal(int type);
     void onProjectionTypeChangedSignal(int type);
     void onSaveGifButtonCklickedSignal();
     void onSaveImageButtonCklickedSignal();
     void keyReleaseSignal(int key);
     void onLightCheckBoxClickedSignal(int state);
     void onTextureCheckBoxClickedSignal(int state);
     void onMaterialCheckBoxClickedSignal(int state);

 private:
     Ui::MainView *ui;
};
#endif  // MAINVIEW_H
