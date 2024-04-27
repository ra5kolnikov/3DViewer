#include "mainview.h"

#include "./ui_mainview.h"
#include "director.h"
MainView::MainView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainView)
{
    ui->setupUi(this);
    QString style = "QDoubleSpinBox {\n"
                    "background-color: rgb(29, 15, 30);\n"
                    "color: rgb(224, 255, 190);\n"
                    "font-size: 24px;\n"
                    "}";
    // DoubleSpinBoxes
    QList<QDoubleSpinBox *> allDSpinBoxes = MainView::findChildren<QDoubleSpinBox *>();
    for (int i = 0; i < allDSpinBoxes.length(); i++) {
        allDSpinBoxes[i]->setButtonSymbols(QAbstractSpinBox::NoButtons);
    }
    setupConnections();
    ui->dbspbx_Diffuse->setStyleSheet(style);
    ui->dbspbx_Ambient->setStyleSheet(style);
    ui->dbspbx_Specular->setStyleSheet(style);
    QString exe_dir_path = QCoreApplication::applicationDirPath();
    models_dir_path_ = QDir(exe_dir_path).filePath("app_data");
}

MainView::~MainView() { delete ui; }

void MainView::setupConnections() {
    connect(ui->btn_LightColor, &QPushButton::clicked, this, &::MainView::onLightColorButtonClicked);
    connect(ui->btn_SelectFile,
            &QPushButton::clicked,
            this,
            &::MainView::onSelectObjectButtonClicked);
    connect(ui->btn_ReadMaterials,
            &QPushButton::clicked,
            this,
            &::MainView::onSelectMaterialButtonClicked);

    connect(ui->btn_BgColor, &QPushButton::clicked, this, &::MainView::onBgColorButtonClicked);
    connect(ui->btn_VertexColor,
            &QPushButton::clicked,
            this,
            &::MainView::onVertexColorButtonClicked);
    connect(ui->btn_EdgeColor, &QPushButton::clicked, this, &::MainView::onEdgeColorButtonClicked);

    connect(ui->btn_FaceColor, &QPushButton::clicked, this, &::MainView::onFaceColorButtonClicked);
    connect(ui->btn_LoadTextures, &QPushButton::clicked, this, &::MainView::onLoadTexturesClicked);
    connect(ui->spbx_EdgeSize,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &MainView::onEdgeSizeValueChanged);
    connect(ui->spbx_VertexSize,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &MainView::onVertexSizeValueChanged);

    // changing appearance
    connect(ui->rdbtn_eTypeDashed, &QRadioButton::clicked, this, &MainView::onEdgeTypeChanged);
    connect(ui->rdbtn_eTypeSolid, &QRadioButton::clicked, this, &MainView::onEdgeTypeChanged);
    connect(ui->rdbtn_eTypeNoEdge, &QRadioButton::clicked, this, &MainView::onEdgeTypeChanged);

    connect(ui->rdbtn_fTypeDisplay, &QRadioButton::clicked, this, &MainView::onFaceTypeChanged);
    connect(ui->rdbtn_fTypeNoFace, &QRadioButton::clicked, this, &MainView::onFaceTypeChanged);

    connect(ui->rdbtn_vTypeCircle, &QRadioButton::clicked, this, &MainView::onVertexTypeChanged);
    connect(ui->rdbtn_vTypeSquare, &QRadioButton::clicked, this, &MainView::onVertexTypeChanged);
    connect(ui->rdbtn_vTypeNoVertex, &QRadioButton::clicked, this, &MainView::onVertexTypeChanged);

    connect(ui->rdbtn_pTypeCentral,
            &QRadioButton::clicked,
            this,
            &MainView::onProjectionTypeChanged);
    connect(ui->rdbtn_pTypeParallel,
            &QRadioButton::clicked,
            this,
            &MainView::onProjectionTypeChanged);

    connect(ui->hsld_xPos, &QSlider::sliderMoved, this, &MainView::xPosSliderMoved);
    connect(ui->hsld_yPos, &QSlider::sliderMoved, this, &MainView::yPosSliderMoved);
    connect(ui->hsld_zPos, &QSlider::sliderMoved, this, &MainView::zPosSliderMoved);

    connect(ui->vsld_Scale, &QAbstractSlider::valueChanged, this, &MainView::scale);

    connect(ui->dial_XRot, &QDial::valueChanged, this, &MainView::xRotationChanged);
    connect(ui->dial_YRot, &QDial::valueChanged, this, &MainView::yRotationChanged);
    connect(ui->dial_ZRot, &QDial::valueChanged, this, &MainView::zRotationChanged);

    connect(ui->btn_GIF, &QAbstractButton::clicked, this, &MainView::onSaveGifButtonCklicked);
    connect(ui->btn_Save, &QAbstractButton::clicked, this, &MainView::onSaveImageButtonCklicked);

    connect(ui->btn_Reset, &QPushButton::clicked, this, &::MainView::onResetButtonClicked);
    QObject::connect(ui->cbx_Light,
                     &QCheckBox::stateChanged,
                     this,
                     &::MainView::onLightCheckBoxClickedSignal);
    QObject::connect(ui->cbx_Texture,
                     &QCheckBox::stateChanged,
                     this,
                     &::MainView::onTextureCheckBoxClickedSignal);
    QObject::connect(ui->cbx_Material,
                     &QCheckBox::stateChanged,
                     this,
                     &::MainView::onMaterialCheckBoxClickedSignal);
    QObject::connect(ui->dbspbx_Ambient,
                     &QDoubleSpinBox::valueChanged,
                     this,
                     &::MainView::onLightSettingsChanged);
    QObject::connect(ui->dbspbx_Diffuse,
                     &QDoubleSpinBox::valueChanged,
                     this,
                     &::MainView::onLightSettingsChanged);
    QObject::connect(ui->dbspbx_Specular,
                     &QDoubleSpinBox::valueChanged,
                     this,
                     &::MainView::onLightSettingsChanged);
}

void MainView::onLoadTexturesClicked()
{
    QString newTexturePath = QFileDialog::getOpenFileName(0,
                                                          ("Select Texture"),
                                                          models_dir_path_ + "/textures",
                                                          "Images (*.png *.jpg)");
    emit onLoadTexturesClickedSignal(newTexturePath);
}

void MainView::onSelectObjectButtonClicked() {
    QString newModelPath = QFileDialog::getOpenFileName(0,
                                                        ("Select Model"),
                                                        models_dir_path_ + "/models",
                                                        "3D Image Files (*.obj)");
    emit onSelectObjectButtonClickedSignal(newModelPath);
}

void MainView::onSelectMaterialButtonClicked() {
    QString newModelPath = QFileDialog::getOpenFileName(0,
                                                        ("Select Materials"),
                                                        models_dir_path_ + "/materials",
                                                        "3D Image Files (*.mtl)");
    emit onSelectMaterialButtonClickedSignal(newModelPath);
}

void MainView::setData(QString filePath, int countVertices, int countFaces)
{
    ui->lbl_FlieName->setText(filePath);
    ui->lbl_NumFaces->setText(QString::number(countFaces));
    ui->lbl_NumVertices->setText(QString::number(countVertices));
}

void MainView::onBgColorButtonClicked() {
  QColor color = QColorDialog::getColor();
  if (color.isValid()) {
    emit setBGColorClickedSignal(color);
    ui->lbl_BgColor->setStyleSheet(
        "background-color: " + color.name(QColor::HexArgb) + ";");
  }
}
void MainView::onLightColorButtonClicked()
{
    QColor color = QColorDialog::getColor();
    if (color.isValid()) {
        emit setLightColorClickedSignal(color);
        ui->lbl_LightColor->setStyleSheet("background-color: " + color.name(QColor::HexArgb) + ";");
    }
}

void MainView::onEdgeColorButtonClicked() {
  QColor color = QColorDialog::getColor();
  if (color.isValid()) {
    emit setEdgeColorClickedSignal(color);
    ui->lbl_EdgeColor->setStyleSheet(
        "background-color: " + color.name(QColor::HexArgb) + ";");
  }
}

void MainView::onVertexColorButtonClicked() {
  QColor color = QColorDialog::getColor();
  if (color.isValid()) {
    emit setVertexColorClickedSignal(color);
    ui->lbl_VertexColor->setStyleSheet(
        "background-color: " + color.name(QColor::HexArgb) + ";");
  }
}

void MainView::onFaceColorButtonClicked()
{
    QColor color = QColorDialog::getColor();
    if (color.isValid()) {
        emit setFaceColorClickedSignal(color);
        ui->lbl_FaceColor->setStyleSheet("background-color: " + color.name(QColor::HexArgb) + ";");
    }
}
void MainView::onEdgeSizeValueChanged(int size) {
  emit edgesSizeValueChangedSignal(size);
}

void MainView::onVertexSizeValueChanged(int size) {
  emit vertexSizeValueChangedSignal(size);
}

void MainView::xPosSliderMoved(int position) {
  emit xPosSliderMovedSignal(position);
}

void MainView::yPosSliderMoved(int position) {
  emit yPosSliderMovedSignal(position);
}

void MainView::zPosSliderMoved(int position) {
  emit zPosSliderMovedSignal(position);
}

void MainView::xRotationChanged(int angle) {
  emit xRotationChangedSignal(angle);
}

void MainView::yRotationChanged(int angle) {
  emit yRotationChangedSignal(angle);
}

void MainView::zRotationChanged(int angle) {
  emit zRotationChangedSignal(angle);
}

void MainView::onFaceTypeChanged()
{
    QRadioButton *rdbtn = qobject_cast<QRadioButton *>(sender());
    if (rdbtn == ui->rdbtn_fTypeDisplay) {
        emit onFaceTypeChangedSignal(DISPLAY);
    } else if (rdbtn == ui->rdbtn_fTypeNoFace) {
        emit onFaceTypeChangedSignal(NOFACE);
    }
}
void MainView::onEdgeTypeChanged() {
  QRadioButton *rdbtn = qobject_cast<QRadioButton *>(sender());
  if (rdbtn == ui->rdbtn_eTypeDashed) {
      emit onEdgeTypeChangedSignal(DASHED);
  } else if (rdbtn == ui->rdbtn_eTypeSolid) {
      emit onEdgeTypeChangedSignal(SOLID);
  } else if (rdbtn == ui->rdbtn_eTypeNoEdge) {
      emit onEdgeTypeChangedSignal(NOEDGE);
  }
}

void MainView::onVertexTypeChanged() {
  QRadioButton *rdbtn = qobject_cast<QRadioButton *>(sender());
  if (rdbtn == ui->rdbtn_vTypeCircle) {
    emit onVertexTypeChangedSignal(CIRCLE);
  } else if (rdbtn == ui->rdbtn_vTypeSquare) {
    emit onVertexTypeChangedSignal(SQUARE);
  } else if (rdbtn == ui->rdbtn_vTypeNoVertex) {
    emit onVertexTypeChangedSignal(NOVERTEX);
  }
}

void MainView::onLightSettingsChanged(double value)
{
    QDoubleSpinBox *dspbx = qobject_cast<QDoubleSpinBox *>(sender());
    if (dspbx == ui->dbspbx_Ambient) {
        emit onAmbientLightChangedSignal(value);
    } else if (dspbx == ui->dbspbx_Diffuse) {
        emit onDiffuseLightChangedSignal(value);
    } else if (dspbx == ui->dbspbx_Specular) {
        emit onSpecularLightChangedSignal(value);
    }
}

void MainView::onProjectionTypeChanged() {
  QRadioButton *rdbtn = qobject_cast<QRadioButton *>(sender());
  if (rdbtn == ui->rdbtn_pTypeCentral) {
    emit onProjectionTypeChangedSignal(CENTRAL);
  } else if (rdbtn == ui->rdbtn_pTypeParallel) {
    emit onProjectionTypeChangedSignal(PARALLEL);
  }
}

void MainView::scale(double scale) { emit scaleValueChangedSignal(scale); }

Scene *MainView::getScene()
{
    return ui->scene;
}

void MainView::onSaveGifButtonCklicked() {
  emit onSaveGifButtonCklickedSignal();
}

void MainView::onSaveImageButtonCklicked() {
  emit onSaveImageButtonCklickedSignal();
}

void MainView::onResetButtonClicked()
{
    ui->vsld_Scale->setValue(0);
    ui->dial_XRot->setValue(0);
    ui->dial_YRot->setValue(0);
    ui->dial_ZRot->setValue(0);
    ui->hsld_xPos->setValue(0);
    ui->hsld_yPos->setValue(0);
    ui->hsld_zPos->setValue(0);
    emit onResetButtonClickedSignal();
}

void MainView::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
}

void MainView::keyReleaseEvent(QKeyEvent *event)
{
    emit keyReleaseSignal(event->key());
    QWidget::keyReleaseEvent(event);
}
