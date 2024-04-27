#ifndef MESHMODEL_H
#define MESHMODEL_H
#include "config.h"
#include "face.h"
#include "qopenglshaderprogram.h"

class MeshModel {
 public:
  MeshModel()
      : size_(0),
        vertex_count_(0),
        face_count_(0),
        config_(Config{}),
        program_(nullptr) {}

  MeshModel(const QVector<Face> &faces, const std::vector<QVector3D> &vertices,
            const std::vector<QVector3D> &normals, unsigned int vertex_count,
            unsigned int face_count, const QVector3D &center, double size)
      : size_(size),
        vertex_count_(vertex_count),
        face_count_(face_count),
        faces_(faces),
        vertices_(vertices),
        normals_(normals),
        center_(center),
        config_(Config{}),
        program_(nullptr) {}

  void Clear() {
    size_ = vertex_count_ = face_count_ = 0;
    faces_.clear();
    vertices_.clear();
    normals_.clear();
    center_ = {};
    config_ = {};
    program_ = nullptr;
  }

  void LoadModel(const QVector<Face> &faces,
                 const std::vector<QVector3D> &vertices,
                 const std::vector<QVector3D> &normals,
                 unsigned int vertex_count, unsigned int face_count,
                 const QVector3D &center, double size) {}
  void SetTextureCoords(const std::vector<QVector2D> &texture_coords)
  {
      texture_coords_ = texture_coords;
  }
  void FinishRendering()
  {
      if (config_.TextureMode() && config_.Texture()) {
          config_.Texture()->release();
      }
  }

  void FinishRenderingGroup(const Material &material)
  {
      if (material.has_diffuse_texture && material.diffuse_sampler) {
          material.diffuse_sampler->release();
      }
      if (material.has_ambient_texture && material.ambient_sampler) {
          material.ambient_sampler->release();
      }
      if (material.has_specular_texture && material.specular_sampler) {
          material.specular_sampler->release();
      }
      if (material.has_bump_texture && material.bump_sampler) {
          material.bump_sampler->release();
      }
  }

  void RenderFaces(QVector<int>::const_iterator start, QVector<int>::const_iterator end)
  {
      for (auto i = start; i != end; i++) {
          const auto &face = faces_[*i];

          // SetLightSettings(face, config_.FaceColor());
          face.RenderFaces();
      }
  }

  void RenderFaces()
  {
      for (const auto &face : faces_) {
          // SetLightSettings(face, config_.FaceColor());
          program_->setUniformValue("color", config_.FaceColor());
          face.RenderFaces();
      }
  }

  void RenderByMaterial()
  {
      for (auto it = faces_with_one_material_.begin(); it != faces_with_one_material_.end(); ++it) {
          const QVector<int> &indices = it.value();
          const auto &material = materials_map_[it.key()];
          SendMaterialDataToShaders(material);
          RenderGroup(indices.begin(), indices.end());
          FinishRenderingGroup(material);
      }
  }

  void RenderEdges(QVector<int>::const_iterator start, QVector<int>::const_iterator end)
  {
      glLineWidth(config_.EdgeWidth());

      if (config_.EdgeType() == DASHED) {
          glLineStipple(1, 0x3333);
          glEnable(GL_LINE_STIPPLE);
      }

      for (auto i = start; i != end; i++) {
          const auto &face = faces_[*i];

          // SetLightSettings(face, config_.EdgeColor());
          face.RenderEdges();
      }

      if (config_.EdgeType() == DASHED) {
          glDisable(GL_LINE_STIPPLE);
      }
  }

  void RenderEdges()
  {
      glLineWidth(config_.EdgeWidth());

      if (config_.EdgeType() == DASHED) {
          glLineStipple(1, 0x3333);
          glEnable(GL_LINE_STIPPLE);
      }

      for (const auto &face : faces_) {
          program_->setUniformValue("color", config_.EdgeColor());
          // SetLightSettings(face, config_.EdgeColor());
          face.RenderEdges();
      }

      if (config_.EdgeType() == DASHED) {
          glDisable(GL_LINE_STIPPLE);
      }
  }
  void RenderPoints(QVector<int>::const_iterator start, QVector<int>::const_iterator end)
  {
      glPointSize(config_.VertexSize());
      // program_->setUniformValue("color", config_.VertexColor());
      if (config_.VertexType() == CIRCLE)
          // glEnable(GL_POINT_SMOOTH);
          glEnable(GL_MULTISAMPLE);
      for (auto i = start; i != end; i++) {
          const auto &face = faces_[*i];

          // program_->setUniformValue("color", config_.VertexColor());
          // SetLightSettings(face, config_.VertexColor());
          face.RenderPoints();
      }
      if (config_.VertexType() == CIRCLE)
          // glDisable(GL_POINT_SMOOTH);
          glDisable(GL_MULTISAMPLE);
  }

  void RenderPoints()
  {
      glPointSize(config_.VertexSize());
      // program_->setUniformValue("color", config_.VertexColor());
      if (config_.VertexType() == CIRCLE)
          // glEnable(GL_POINT_SMOOTH);
          glEnable(GL_MULTISAMPLE);
      for (const auto &face : faces_) {
          program_->setUniformValue("color", config_.VertexColor());
          // SetLightSettings(face, config_.VertexColor());
          face.RenderPoints();
      }
      if (config_.VertexType() == CIRCLE)
          // glDisable(GL_POINT_SMOOTH);
          glDisable(GL_MULTISAMPLE);
  }

  void Render(QOpenGLShaderProgram *shader_program)
  {
      program_ = shader_program;

      if (vertex_count_ == 0)
          return;

      program_->bind();

      PrepareTextureInfo();
      PreparePositionInfo();
      PrepareLightInfo();

      if (PrepareMaterialInfo()) {
          RenderByMaterial();
      } else {
          RenderAll();
      }

      FinishRendering();

      program_->release();
  }

  void RenderGroup(QVector<int>::const_iterator start, QVector<int>::const_iterator end)
  { // vertices
      if (config_.FaceType()) {
          RenderFaces(start, end);
      }

      if (config_.EdgeType()) {
          RenderEdges(start, end);
      }

      // vertices
      if (config_.VertexType()) {
          RenderPoints(start, end);
      }
  }

  void RenderAll()
  {
      if (config_.FaceType()) {
          RenderFaces();
      }

      if (config_.EdgeType()) {
          RenderEdges();
      }

      if (config_.VertexType()) {
          RenderPoints();
      }
  }

  // void SetLightSettings(const Face &cur_face, QVector3D base_color)
  // {
  //     if (config_.MaterialMode() && !materials_map_.empty()) {
  //         const auto &cur_m = materials_map_[cur_face.MaterialName()];

  //         if (cur_m.has_textures) {
  //             cur_m.diffuse_sampler->bind();
  //             glActiveTexture(GL_TEXTURE0);
  //         }

  //     } else {
  //         program_->setUniformValue("color", base_color);
  //     }
  // }

  void SendMaterialComponentsToShaders(QVector3D diffuse,
                                       QVector3D ambient,
                                       QVector3D specular,
                                       float shininess)
  {
      program_->setUniformValue("material.diffuse_color", diffuse);
      program_->setUniformValue("material.ambient_color", ambient);
      program_->setUniformValue("material.specular_color", specular);
      program_->setUniformValue("material.shininess", shininess);
  }

  void LoadMaterials()
  {
      const auto &materials = config_.GetMaterialLib(mtllib_);
      if (!materials.empty()) {
          for (const auto &material : materials) {
              materials_map_[material.name] = material;
          }
          int i = 0;
          for (auto &face : faces_) {
              auto &name = face.MaterialName();
              if (name == "") {
                  faces_with_one_material_["empty"] += i++;
              } else {
                  faces_with_one_material_[name] += i++;
              }
          }
      }
  }

  void SendMaterialDataToShaders(const Material &material)
  {
      const auto &[diffuse, ambient, specular, shininess] = material.components;
      SendMaterialComponentsToShaders(diffuse, ambient, specular, shininess);
      program_->setUniformValue("material.dissolve", material.dissolve);
      if (material.has_diffuse_texture && material.diffuse_sampler) {
          material.diffuse_sampler->bind();
          glActiveTexture(GL_TEXTURE0);
          program_->setUniformValue("material.diffuse_sampler", 0);
          program_->setUniformValue("material.has_diffuse_texture", true);
      } else {
          program_->setUniformValue("material.has_diffuse_texture", false);
      }
      if (material.has_ambient_texture && material.ambient_sampler) {
          material.ambient_sampler->bind();
          glActiveTexture(GL_TEXTURE1);
          program_->setUniformValue("material.ambient_sampler", 1);
          program_->setUniformValue("material.has_ambient_texture", true);
      } else {
          program_->setUniformValue("material.has_ambient_texture", false);
      }
      if (material.has_specular_texture && material.specular_sampler) {
          material.specular_sampler->bind();
          glActiveTexture(GL_TEXTURE2);
          program_->setUniformValue("material.specular_sampler", 2);
          program_->setUniformValue("material.has_specular_texture", true);
      } else {
          program_->setUniformValue("material.has_specular_texture", false);
      }
      if (material.has_bump_texture && material.bump_sampler) {
          material.bump_sampler->bind();
          glActiveTexture(GL_TEXTURE3);
          program_->setUniformValue("material.bump_multiplier", material.bump_multiplier);
          program_->setUniformValue("material.bump_sampler", 3);
          program_->setUniformValue("material.has_bump_texture", true);
      } else {
          program_->setUniformValue("material.has_bump_texture", false);
      }
  }

  bool PrepareMaterialInfo()
  {
      if (materials_map_.empty()) {
          LoadMaterials();
      }
      if (config_.MaterialMode() && !materials_map_.empty()) {
          program_->setUniformValue("material_mode", true);
          return true;
      } else {
          program_->setUniformValue("material_mode", false);
      }
      return false;
  }

  void PrepareTextureInfo()
  {
      if (config_.TextureMode() && config_.Texture()) {
          config_.Texture()->bind();
          glActiveTexture(GL_TEXTURE0);
          program_->setUniformValue("textureSampler", 0);
          program_->setUniformValue("texture_mode", config_.TextureMode());
      } else {
          program_->setUniformValue("texture_mode", false);
      }
  }

  void PrepareLightInfo()
  {
      if (config_.LightMode()) {
          QVector3D view_pos = config_.CameraPosition();

          const auto &[position, ambient_c, diffuse_c, specular_c, ambient_i, diffuse_i, specular_i]
              = config_.Light();

          program_->setUniformValue("light_pos", position);
          program_->setUniformValue("light.ambient_color", ambient_c);
          program_->setUniformValue("light.diffuse_color", diffuse_c);
          program_->setUniformValue("light.specular_color", specular_c);
          program_->setUniformValue("light.ambient_intensity", ambient_i);
          program_->setUniformValue("light.diffuse_intensity", diffuse_i);
          program_->setUniformValue("light.specular_intensity", specular_i);
          program_->setUniformValue("view_pos", view_pos);
      }
  }

  void PreparePositionInfo()
  {
      program_->setUniformValue("view_matrix", config_.ViewMatrix());
      program_->setUniformValue("projection_matrix", config_.ProjectionMatrix());
      program_->setUniformValue("model_matrix", config_.ModelMatrix());
  }

  void SetSettings(const Config &config) { config_ = config; }
  void SetMaterialLib(const QString &name) { mtllib_ = name; }
  QVector3D Center() const { return center_; }
  const std::vector<QVector3D> &Vertices() const { return vertices_; }
  const std::vector<QVector3D> &Normals() const { return normals_; }
  const std::vector<QVector2D> &TextureCoords() const { return texture_coords_; }
  double VertexCount() const { return vertex_count_; }
  double ModelSize() const { return size_; }

  const QVector<Face> &Faces() const { return faces_; }
  const QString &MaterialLibrary() const { return mtllib_; }

  private:
  double size_;
  unsigned int vertex_count_;
  unsigned int face_count_;
  QVector<Face> faces_;
  QMap<QString, QVector<int>> faces_with_one_material_;
  std::vector<QVector3D> vertices_;
  std::vector<QVector3D> normals_;
  QVector3D center_;
  Config config_;
  QOpenGLShaderProgram *program_;
  std::vector<QVector2D> texture_coords_;
  QString mtllib_;
  QMap<QString, Material> materials_map_;
};

#endif  // MESHMODEL_H
