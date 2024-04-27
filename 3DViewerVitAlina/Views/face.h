#ifndef FACE_H
#define FACE_H

#include <QOpenGLTexture>
#include <QVector>
#include "qdebug.h"
#include "qopengltexture.h"
#include "qvectornd.h"
#include <GL/gl.h>

typedef struct MaterialComponents
{
    QVector3D diffuse_color = QVector3D(0.8, 0.8, 0.8);
    QVector3D ambient_color = QVector3D(0.2, 0.2, 0.2);
    QVector3D specular_color = QVector3D(1.0, 1.0, 1.0);
    float shininess_exponent = 16.0f;
} MaterialComponents;

typedef struct Material
{
    QString name = "empty";
    QImage map_kd;
    QImage map_ka;
    QImage map_ks;
    QImage map_bump;
    float dissolve = 1.0;
    float bump_multiplier = 1.0;
    bool has_diffuse_texture = false;
    bool has_ambient_texture = false;
    bool has_specular_texture = false;
    bool has_bump_texture = false;
    MaterialComponents components;
    QOpenGLTexture *diffuse_sampler = nullptr;
    QOpenGLTexture *ambient_sampler = nullptr;
    QOpenGLTexture *specular_sampler = nullptr;
    QOpenGLTexture *bump_sampler = nullptr;
    bool use_base_color = true;

    const MaterialComponents &GetMaterialComponents() const { return components; }
} Material;

class Face
{
public:
    Face(bool to_zero_based_indexing = true)
        : to_zero_based_indexing_(to_zero_based_indexing)
        , offset_(0)
        , num_vertices_(0)
        , material_(Material{})
    {}

    void AddVertexIndex(unsigned int vertex_index)
    {
        if (to_zero_based_indexing_)
            vertex_index -= 1;
        vertex_indices_.push_back(vertex_index);
        num_vertices_++;
    }
    void AddNormalIndex(unsigned int normal_index)
    {
        if (to_zero_based_indexing_)
            normal_index -= 1;
        normal_indices_.push_back(normal_index);
    }

    void AddTextureCoordsIndex(unsigned int texture_index)
    {
        if (to_zero_based_indexing_)
            texture_index -= 1;
        texture_coords_indices_.push_back(texture_index);
    }
    unsigned int Offset() const { return offset_; }
    void SetOffset(unsigned int offset) { offset_ = offset; }
    void SetMaterial(const Material &material) { material_ = material; }
    const QVector<unsigned int> &VertexIndices() const { return vertex_indices_; }
    const QVector<unsigned int> &NormalIndices() const { return normal_indices_; }
    const QVector<unsigned int> &TextureCoordsIndices() const { return texture_coords_indices_; }
    unsigned int VertexCount() const { return num_vertices_; }
    const Material &GetMaterial() const { return material_; }
    void SetMaterialName(const QString &name) { material_.name = name; }
    const QString &MaterialName() const { return material_.name; }
    void AddMaterial(const Material &material) { material_ = material; }
    Material GetMaterial(QVector3D base_color) const
    {
        Material m = material_;
        if (m.use_base_color == true) {
            m.components.ambient_color = base_color;
            m.components.diffuse_color = base_color;
            m.components.specular_color = base_color;
        }
        return m;
    }

    MaterialComponents GetMaterialComponents(QVector3D base_color) const
    {
        return GetMaterial(base_color).components;
    }
    MaterialComponents GetMaterialComponents() const { return GetMaterial().components; }

    void RenderFaces() const
    {
        if (num_vertices_ < 2) {
            RenderPoints();
        } else if (num_vertices_ < 3)
            RenderEdges();
        else
            Render(GL_POLYGON);
    }

    void RenderEdges() const { Render(GL_LINE_LOOP); }
    void RenderPoints() const { Render(GL_POINTS); }

    void Render(unsigned int mode) const { glDrawArrays(mode, offset_, num_vertices_); }

private:
    bool to_zero_based_indexing_;
    unsigned int offset_;
    unsigned int num_vertices_;
    QVector<unsigned int> vertex_indices_;
    QVector<unsigned int> normal_indices_;
    QVector<unsigned int> texture_coords_indices_;
    Material material_;
};

#endif // FACE_H
