#include "scenemodel.h"

Config SceneModel::GetConfig() const { return config_; }

void SceneModel::SetVertexType(int type) { config_.SetVertexType(type); }

void SceneModel::SetEdgeType(int type) { config_.SetEdgeType(type); }

void SceneModel::SetFaceType(int type) { config_.SetFaceType(type); }

void SceneModel::SetProjectionType(int type) {
  config_.SetProjectionType(type);
  SetProjectionMatrix();
}

void SceneModel::SetBgColor(QColor color) { config_.SetBgColor(color); }
void SceneModel::SetVertexSize(unsigned int size) {
  config_.SetVertexSize(size);
}
void SceneModel::SetEdgeWidth(unsigned int width) {
  config_.SetEdgeWidth(width);
}
void SceneModel::SetVertexColor(QColor color) { config_.SetVertexColor(color); }

void SceneModel::SetEdgeColor(QColor color) { config_.SetEdgeColor(color); }

void SceneModel::SetFaceColor(QColor color) { config_.SetFaceColor(color); }

void SceneModel::SetStepX(double step) {
    double mapped_step = MapValue(step,
                                  -500,
                                  500,
                                  -mesh_model_.ModelSize() * 1.5,
                                  mesh_model_.ModelSize() * 1.5);
    translation_vector_.setX(mapped_step);
    UpdateViewMatrix();
    UpdateTranslationMatrix();
    SetModelMatrix();
}

void SceneModel::SetStepY(double step) {
    double mapped_step = MapValue(step,
                                  -500,
                                  500,
                                  -mesh_model_.ModelSize() * 1.5,
                                  mesh_model_.ModelSize() * 1.5);
    translation_vector_.setY(mapped_step);
    UpdateViewMatrix();
    UpdateTranslationMatrix();
    SetModelMatrix();
}

void SceneModel::SetStepZ(double step) {
    double mapped_step = MapValue(step,
                                  -500,
                                  500,
                                  -mesh_model_.ModelSize() * 1.5,
                                  mesh_model_.ModelSize() * 1.5);
    translation_vector_.setZ(mapped_step);
    UpdateTranslationMatrix();
    SetModelMatrix();
}

void SceneModel::SetScale(double step) {
    scale_ = MapValue(step, -500, 500, 0.1, 10.0);
    // // Check if scale_ is too small and set a minimum scale value
    const double minScale = 0.001; // Adjust as needed
    if (std::abs(scale_) < minScale) {
        scale_ = (scale_ >= 0) ? minScale : -minScale;
    }
    UpdateScaleMatrix();
}

void SceneModel::SetAngleX(double angle) {
    rotation_vector_.setX(angle);
    UpdateRotationMatrix();
}

void SceneModel::SetAngleY(double angle) {
    rotation_vector_.setY(angle);
    UpdateRotationMatrix();
}

void SceneModel::SetAngleZ(double angle) {
    rotation_vector_.setZ(angle);
    UpdateRotationMatrix();
}

void SceneModel::SetRotationVector(const QVector3D &v) { rotation_vector_ = v; }

void SceneModel::SetTranslationVector(const QVector3D &v) {
  translation_vector_ = v;
}

void SceneModel::SetCamera(double model_size) {
  double camera_distance = model_size * 2;
  QVector3D camera_position = QVector3D(0, 0, camera_distance);
  camera_.SetCameraPosition(camera_position);
  UpdateViewMatrix();
  // SetNormalMatrix();
}

void SceneModel::SetCameraDirection(QVector3D direction) {
  camera_.SetCameraDirection(direction);
}

void SceneModel::SetModelMatrix() {
  config_.SetModelMatrix(translation_matrix_ * rotation_matrix_ *
                         scale_matrix_);
  SetNormalMatrix();
}

void SceneModel::SetOrthoProjection(double left, double right, double bottom,
                                    double top, double near_plane,
                                    double far_plane) {
  ortho_matrix_.setToIdentity();
  ortho_matrix_.ortho(left, right, bottom, top, near_plane, far_plane);
}

void SceneModel::SetPerspectiveProjection(double near_plane, double far_plane) {
  double field_of_view = 45.f;
  perspective_matrix_.setToIdentity();
  perspective_matrix_.perspective(field_of_view, config_.AspectRatio(),
                                  near_plane, far_plane);
}

void SceneModel::SetProjection(double model_size) {
  double near_plane = 0.001;
  double far_plane = near_plane + model_size * 100;

  // Set up view volume
  double box_size = model_size * 0.6;

  double left = -box_size;
  double right = box_size;
  double bottom = -box_size;
  double top = box_size;

  if (config_.AspectRatio() < 1.0) {
    bottom /= config_.AspectRatio();
    top /= config_.AspectRatio();
  } else {
    left *= config_.AspectRatio();
    right *= config_.AspectRatio();
  }

  SetOrthoProjection(left, right, bottom, top, near_plane, far_plane);
  SetPerspectiveProjection(near_plane, far_plane);
  SetProjectionMatrix();
}

void SceneModel::SetWidgetSizes(float width, float height) {
  config_.SetWidgetWidth(width);
  config_.SetWidgetHeight(height);
  config_.SetAspectRatio(width / height);
}

void SceneModel::SetViewMatrix() {
  config_.SetViewMatrix(view_matrix_);
  SetNormalMatrix();
}

void SceneModel::SetProjectionMatrix() {
  switch (config_.ProjectionType()) {
    case CENTRAL:
      config_.SetProjectionMatrix(ortho_matrix_);
      break;
    case PARALLEL:
      config_.SetProjectionMatrix(perspective_matrix_);
      break;
  }
}

void SceneModel::UpdateTranslationMatrix() {
  translation_matrix_.setToIdentity();
  translation_matrix_.translate(translation_vector_);
  SetModelMatrix();
}
void SceneModel::UpdateRotationMatrix() {
  rotation_matrix_.setToIdentity();
  const auto &[x, y, z] = rotation_vector_;
  rotation_matrix_.rotate(x, QVector3D(1, 0, 0));
  rotation_matrix_.rotate(y, QVector3D(0, 1, 0));
  rotation_matrix_.rotate(z, QVector3D(0, 0, 1));
  SetModelMatrix();
}

void SceneModel::UpdateScaleMatrix() {
  scale_matrix_.setToIdentity();
  scale_matrix_.scale(scale_);
  SetModelMatrix();
}

void SceneModel::UpdateViewMatrix() {
  view_matrix_.setToIdentity();
  view_matrix_ = camera_.GetViewMatrix();
  SetViewMatrix();
}

void SceneModel::MoveCamera(int direction) {
  camera_.Move(direction);
  UpdateViewMatrix();
}

void SceneModel::SetNormalMatrix() {
  config_.SetNormalMatrix(
      QMatrix4x4(config_.ViewMatrix() * config_.ModelMatrix()).normalMatrix());
}
