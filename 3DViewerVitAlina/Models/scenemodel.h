#ifndef SCENEMODEL_H
#define SCENEMODEL_H
#include <QObject>
#include <QOpenGLTexture>

#include "Views/config.h"
#include "Views/director.h"
#include "camera.h"
#include "meshmodel.h"
#include "qimage.h"

class SceneModel : public QObject {
  Q_OBJECT
 public:
  SceneModel(QObject *parent = nullptr,
             const Config &config = Director::BuildConfig1())
      : QObject(parent),
        scale_(1.0),
        config_(config),
        texture_(nullptr)

  {}
  ~SceneModel()
  {
      if (texture_ && texture_->isCreated())
          texture_->destroy();
      delete texture_;
      for (auto &texture : created_textures_) {
          texture->destroy();
          delete texture;
      }
  }
  Config GetConfig() const;
  void SetVertexColor(QColor color);
  void SetLightColor(QColor color) { config_.SetLightColor(color); }
  void SetEdgeColor(QColor color);
  void SetFaceColor(QColor color);
  void SetBgColor(QColor color);
  void SetEdgeWidth(unsigned int width);
  void SetVertexSize(unsigned int size);
  void SetVertexType(int type);
  void SetEdgeType(int type);
  void SetFaceType(int type);
  void SetProjectionType(int type);
  void SetStepX(double step);
  void SetStepY(double step);
  void SetStepZ(double step);
  void SetScale(double step);
  void SetAngleX(double step);
  void SetAngleY(double step);
  void SetAngleZ(double step);
  void SetRotationVector(const QVector3D &v);
  void SetTranslationVector(const QVector3D &v);
  void SetCamera(double model_size);
  void SetOrthoProjection(double left, double right, double bottom, double top,
                          double near_plane, double far_plane);

  void SetPerspectiveProjection(double near_plane, double far_plane);
  void SetProjection(double model_size);
  void SetModelMatrix();
  void SetWidgetSizes(float width, float height);
  void SetViewMatrix();
  void SetProjectionMatrix();
  void SetCameraDirection(QVector3D direction);
  void SetModelSize(double size);
  void SetDrawingMode(GLuint mode);
  void UpdateTranslationMatrix();
  void UpdateRotationMatrix();
  void UpdateScaleMatrix();
  void UpdateViewMatrix();
  void MoveCamera(int direction);
  void SetNormalMatrix();
  void SetLightMode(int mode) { config_.SetLightMode(mode); }
  void SetMaterialMode(int mode) { config_.SetMaterialMode(mode); }
  void SetTextureMode(int mode) { config_.SetTextureMode(mode); }

  void SetAmbientLight(double value) { config_.SetAmbientLight(value); }
  void SetDiffuseLight(double value) { config_.SetDiffuseLight(value); }
  void SetSpecularLight(double value) { config_.SetSpecularLight(value); }
  void LoadTexture(const QString &filename) {
      QImage image(filename);
      if (image.isNull()) {
          qDebug() << "Failed to load texture";
          return;
      }

      image.convertTo(QImage::Format_RGBA8888);
      if (texture_ == nullptr) {
          texture_ = new QOpenGLTexture(QOpenGLTexture::Target2D);
          texture_->create();
      } else {
          texture_->destroy();
          texture_->create();
      }

    PrepareTexture(texture_, image);
    config_.SetTexture(texture_);
  }

  void LoadMaterial(const QString &filename) {}
  // void OnTimerTimeout()
  // {
  //     frame_count_++;
  //     QVector3D light_pos = config_.LightPosition();
  //     double new_x = mesh_model_.ModelSize() / 2 * sin(frame_count_ / 5);
  //     light_pos.setX(new_x);
  //     double new_y = mesh_model_.ModelSize() / 2 * cos(frame_count_ / 5);
  //     light_pos.setY(new_y);
  //     config_.SetLightPosition(light_pos);
  //     qDebug()         : vertexBuffer(QOpenGLBuffer::VertexBuffer),
  // normalBuffer(QOpenGLBuffer::VertexBuffer) << light_pos;
  // }

  void PrepareTexture(QOpenGLTexture *texture, const QImage &image)
  {
      texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
      texture->setMagnificationFilter(QOpenGLTexture::Linear);
      texture->setWrapMode(QOpenGLTexture::Repeat);
      texture->setData(image.mirrored());
  }

  const MeshModel &getMeshModel() { return mesh_model_; }

  void AddMeshModel(const MeshModel &mesh_model) {
    mesh_model_ = mesh_model;
    SetCamera(mesh_model_.ModelSize());
    SetProjection(mesh_model_.ModelSize());
    QVector3D light_pos(0, 0, mesh_model_.ModelSize() * 10);
    config_.SetLightPosition(light_pos);
  }

  void ResetModelGeometry() {
    translation_vector_ = rotation_vector_ = {};
    scale_ = 1.0;
    UpdateRotationMatrix();
    UpdateTranslationMatrix();
    UpdateScaleMatrix();
    UpdateViewMatrix();
    SetCamera(mesh_model_.ModelSize());
    SetProjection(mesh_model_.ModelSize());
  }

  float MapValue(float value, float in_min, float in_max, float out_min,
                 float out_max) {
    if (in_min == in_max) {
      return out_min;  // Avoid division by zero
    }
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

  void AddMaterialTexture() {}

  void AddMaterials(const QString &mtllib, QVector<Material> &materials, bool with_textures)
  {
      if (!with_textures) {
          config_.AddMaterialLib(mtllib, materials);
          return;
      }

      for (auto &material : materials) {
          if (material.has_diffuse_texture) {
              QOpenGLTexture *texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
              texture->create();
              PrepareTexture(texture, material.map_kd);
              material.diffuse_sampler = texture;
              created_textures_.push_back(texture);
          }
          if (material.has_ambient_texture) {
              QOpenGLTexture *texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
              texture->create();
              PrepareTexture(texture, material.map_ka);
              material.ambient_sampler = texture;
              created_textures_.push_back(texture);
          }
          if (material.has_specular_texture) {
              QOpenGLTexture *texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
              texture->create();
              PrepareTexture(texture, material.map_ks);
              material.specular_sampler = texture;
              created_textures_.push_back(texture);
          }
          if (material.has_bump_texture) {
              QOpenGLTexture *texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
              texture->create();
              PrepareTexture(texture, material.map_bump);
              material.bump_sampler = texture;
              created_textures_.push_back(texture);
          }
      }
      config_.AddMaterialLib(mtllib, materials);
  }

  private:
  float scale_;
  Config config_;
  Camera camera_;
  MeshModel mesh_model_;
  QVector3D translation_vector_;
  QVector3D rotation_vector_;

  QMatrix4x4 scale_matrix_;
  QMatrix4x4 rotation_matrix_;
  QMatrix4x4 translation_matrix_;
  QMatrix4x4 perspective_matrix_;
  QMatrix4x4 ortho_matrix_;
  QMatrix4x4 view_matrix_;
  QOpenGLTexture *texture_;
  QVector<QOpenGLTexture *> created_textures_;
};

#endif  // SCENEMODEL_H
