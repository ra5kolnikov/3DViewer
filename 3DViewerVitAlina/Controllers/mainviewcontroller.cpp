#include "mainviewcontroller.h"
#include "convertmaterialscommand.h"

MainViewController::MainViewController(MainView *View, ObjectModel *Model, SceneModel *scene_model)
    : view_(View)
    , objectModel_(Model)
    , scene_model_(scene_model)
{
    scene_ = view_->getScene();
    scene_model_->SetWidgetSizes(scene_->width(), scene_->height());
    scene_->UpdateConfig(scene_model_->GetConfig());
    connect(view_, &MainView::keyReleaseSignal, this, &::MainViewController::keyReleased);
    connect(view_,
            &MainView::onSelectObjectButtonClickedSignal,
            this,
            &MainViewController::onSelectFileButtonClicked);
    connect(view_,
            &MainView::onSelectMaterialButtonClickedSignal,
            this,
            &MainViewController::onSelectMaterialButtonClicked);
    connect(view_,
            &MainView::onLoadTexturesClickedSignal,
            this,
            &MainViewController::onLoadTexturesClicked);
    connect(view_,
            &MainView::scaleValueChangedSignal,
            this,
            &MainViewController::scaleSliderChangePos);
    connect(view_, &MainView::setBGColorClickedSignal, this, &MainViewController::onBGColorClicked);
    connect(view_,
            &MainView::setEdgeColorClickedSignal,
            this,
            &MainViewController::onEdgeColorClicked);
    connect(view_,
            &MainView::setVertexColorClickedSignal,
            this,
            &MainViewController::onVertexColorClicked);
    connect(view_,
            &MainView::setFaceColorClickedSignal,
            this,
            &MainViewController::onFaceColorClicked);
    connect(view_,
            &MainView::edgesSizeValueChangedSignal,
            this,
            &MainViewController::edgeSizeValueChanged);
    connect(view_,
            &MainView::vertexSizeValueChangedSignal,
            this,
            &MainViewController::vertexSizeValueChanged);
    connect(view_, &MainView::xPosSliderMovedSignal, this, &MainViewController::xPosSliderMoved);
    connect(view_, &MainView::yPosSliderMovedSignal, this, &MainViewController::yPosSliderMoved);
    connect(view_, &MainView::zPosSliderMovedSignal, this, &MainViewController::zPosSliderMoved);
    connect(view_, &MainView::xRotationChangedSignal, this, &MainViewController::xRotationChanged);
    connect(view_, &MainView::yRotationChangedSignal, this, &MainViewController::yRotationChanged);
    connect(view_, &MainView::zRotationChangedSignal, this, &MainViewController::zRotationChanged);
    connect(view_, &MainView::onEdgeTypeChangedSignal, this, &MainViewController::onEdgeTypeChanged);
    connect(view_,
            &MainView::onVertexTypeChangedSignal,
            this,
            &MainViewController::onVertexTypeChanged);
    connect(view_, &MainView::onFaceTypeChangedSignal, this, &MainViewController::onFaceTypeChanged);
    connect(view_,
            &MainView::onProjectionTypeChangedSignal,
            this,
            &MainViewController::onProjectionTypeChanged);
    connect(view_,
            &MainView::onSaveGifButtonCklickedSignal,
            this,
            &MainViewController::onSaveGifButtonCklicked);
    connect(view_,
            &MainView::onSaveImageButtonCklickedSignal,
            this,
            &MainViewController::onSaveImageButtonCklicked);
    connect(view_,
            &MainView::onResetButtonClickedSignal,
            this,
            &::MainViewController::onResetButtonClicked);
    connect(view_,
            &::MainView::onLightCheckBoxClickedSignal,
            this,
            &MainViewController::onLightCheckBoxClicked);
    connect(view_,
            &::MainView::onTextureCheckBoxClickedSignal,
            this,
            &MainViewController::onTextureCheckBoxClicked);
    connect(view_,
            &::MainView::onMaterialCheckBoxClickedSignal,
            this,
            &MainViewController::onMaterialCheckBoxClicked);

    connect(view_,
            &::MainView::onAmbientLightChangedSignal,
            this,
            &MainViewController::onAmbientLightChanged);

    connect(view_,
            &::MainView::onDiffuseLightChangedSignal,
            this,
            &MainViewController::onDiffuseLightChanged);
    connect(view_,
            &MainView::onSpecularLightChangedSignal,
            this,
            &MainViewController::onSpecularLightChanged);
    connect(view_,
            &MainView::setLightColorClickedSignal,
            this,
            &MainViewController::onLightColorClicked);
}

MainViewController::~MainViewController() = default;

void MainViewController::onDiffuseLightChanged(double value)
{
    scene_model_->SetDiffuseLight(value);
    updateWidgetConfig();
}

void MainViewController::onAmbientLightChanged(double value)
{
    scene_model_->SetAmbientLight(value);
    updateWidgetConfig();
}

void MainViewController::onSpecularLightChanged(double value)
{
    scene_model_->SetSpecularLight(value);
    updateWidgetConfig();
}

void MainViewController::onResetButtonClicked()
{
    scene_model_->ResetModelGeometry();
    updateWidgetConfig();
}

void MainViewController::onSelectFileButtonClicked(const QString &fileName) {
  ProcessNewFileCommand *command =
      new ProcessNewFileCommand(fileName, objectModel_, scene_model_);

  if (command->execute()) {
      view_->setData(objectModel_->_filePath,
                     objectModel_->_countVertices,
                     objectModel_->_countFaces);

      scene_->AddMeshModel(scene_model_->getMeshModel());
      updateWidgetConfig();
  }
  delete command;
}

void MainViewController::onLoadTexturesClicked(const QString &fileName)
{
    scene_model_->LoadTexture(fileName);
    updateWidgetConfig();
}

void MainViewController::onSelectMaterialButtonClicked(
    const QString &fileName) {
    ConvertMaterialsCommand *command = new ConvertMaterialsCommand(fileName,
                                                                   objectModel_,
                                                                   scene_model_);

    if (command->execute()) {
        // view_->setData(objectModel_->_filePath,
        //                objectModel_->_countVertices,
        //                objectModel_->_countFaces);

        updateWidgetConfig();
    }
    delete command;
}

void MainViewController::scaleSliderChangePos(const double scale) {
  scene_model_->SetScale(scale);
  updateWidgetConfig();
}

void MainViewController::onBGColorClicked(QColor color) {
  scene_model_->SetBgColor(color);
  updateWidgetConfig();
}
void MainViewController::onLightColorClicked(QColor color)
{
    scene_model_->SetLightColor(color);
    updateWidgetConfig();
}
void MainViewController::onEdgeColorClicked(QColor color) {
  scene_model_->SetEdgeColor(color);
  updateWidgetConfig();
}

void MainViewController::onVertexColorClicked(QColor color) {
  scene_model_->SetVertexColor(color);
  updateWidgetConfig();
}

void MainViewController::onFaceColorClicked(QColor color) {
  scene_model_->SetFaceColor(color);
  updateWidgetConfig();
}
void MainViewController::edgeSizeValueChanged(int size) {
  scene_model_->SetEdgeWidth(size);
  updateWidgetConfig();
}

void MainViewController::vertexSizeValueChanged(int size) {
  scene_model_->SetVertexSize(size);
  updateWidgetConfig();
}

void MainViewController::xPosSliderMoved(int position) {
  scene_model_->SetStepX(position);
  updateWidgetConfig();
}

void MainViewController::yPosSliderMoved(int position) {
  scene_model_->SetStepY(position);
  updateWidgetConfig();
}

void MainViewController::zPosSliderMoved(int position) {
  scene_model_->SetStepZ(position);
  updateWidgetConfig();
}

void MainViewController::xRotationChanged(int angle) {
  scene_model_->SetAngleX(angle);
  updateWidgetConfig();
}

void MainViewController::yRotationChanged(int angle) {
  scene_model_->SetAngleY(angle);
  updateWidgetConfig();
}

void MainViewController::zRotationChanged(int angle) {
  scene_model_->SetAngleZ(angle);
  updateWidgetConfig();
}

void MainViewController::onEdgeTypeChanged(int type) {
  scene_model_->SetEdgeType(type);
  updateWidgetConfig();
}

void MainViewController::onVertexTypeChanged(int type) {
  scene_model_->SetVertexType(type);
  updateWidgetConfig();
}

void MainViewController::onFaceTypeChanged(int type) {
  scene_model_->SetFaceType(type);
  updateWidgetConfig();
}
void MainViewController::onProjectionTypeChanged(int type) {
  scene_model_->SetProjectionType(type);
  updateWidgetConfig();
}

void MainViewController::updateWidgetConfig() {
  scene_->UpdateConfig(scene_model_->GetConfig());
}

void MainViewController::onSaveGifButtonCklicked() {
  auto gif = new QGifImage;
  auto timer = new QTimer(this);
  gif->setDefaultDelay(100);
  gif->setDefaultTransparentColor(QColorConstants::Black);

  connect(timer, &QTimer::timeout, this, [=]() {
    QPixmap pix = QPixmap::fromImage(view_->getScene()->grabFramebuffer());
    QPixmap scaledPix = pix.scaled(QSize(640, 480), Qt::IgnoreAspectRatio,
                                   Qt::SmoothTransformation);
    gif->addFrame(scaledPix.toImage());

    if (gif->frameCount() == 50) {
      timer->stop();
      QString pathGIF = QFileDialog::getSaveFileName(
          view_, "Save as GIF", "image.gif", "GIF Image Files (*.gif)");
      if (!pathGIF.isEmpty()) {
        gif->save(pathGIF);
      }
      delete timer;
      delete gif;
    }
  });

  timer->start(100);
}

void MainViewController::onSaveImageButtonCklicked() {
  QString strfilter = "";
  QFileDialog dialog;
  QImage img = view_->getScene()->grabFramebuffer();
  QString file_name = dialog.getSaveFileName(view_, tr("save"), "~/",
                                             tr("*.bmp ;; *.jpg"), &strfilter);

  if (!file_name.isEmpty()) {
    if (strfilter.contains("jpg"))
      img.save(file_name, "JPG");
    else
      img.save(file_name, "BMP");
  }
}

void MainViewController::keyReleased(int key) {
  if (key == Qt::Key_A || key == Qt::Key_D || key == Qt::Key_W ||
      key == Qt::Key_S || key == Qt::Key_2 || key == Qt::Key_1) {
    scene_model_->MoveCamera(key);
    updateWidgetConfig();
  }
}

void MainViewController::onLightCheckBoxClicked(int state)
{
    scene_model_->SetLightMode(state);
    updateWidgetConfig();
}

void MainViewController::onTextureCheckBoxClicked(int state)
{
    scene_model_->SetTextureMode(state);
    updateWidgetConfig();
}

void MainViewController::onMaterialCheckBoxClicked(int state)
{
    scene_model_->SetMaterialMode(state);
    updateWidgetConfig();
}
