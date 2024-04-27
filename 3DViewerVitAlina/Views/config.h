#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include "face.h"
#include "qcolor.h"
#include "qmatrix4x4.h"
#include "qopengltexture.h"
#include "qvectornd.h"
#include <GL/gl.h>
// projection type
#define PARALLEL 1
#define CENTRAL 2

// edge type
#define NOEDGE 0
#define SOLID 1
#define DASHED 2

// vertex type
#define NOVERTEX 0
#define CIRCLE 1
#define SQUARE 2

// face type
#define NOFACE 0
#define DISPLAY 1

// drawing mode
#define OFF 0
#define ON 1

// #define MATERIALSON 2

typedef struct light_settings
{
    QVector3D position = {0.0f, 1.0f, 0.0f};
    QVector3D ambient_color = {1.0f, 1.0f, 1.0f};
    QVector3D diffuse_color = {1.0f, 1.0f, 1.0f};
    QVector3D specular_color = {1.0, 1.0, 1.0};
    float ambient_intensity = 0.2;
    float diffuse_intensity = 0.85;
    float specular_intensity = 0.5;
} l_settings;

typedef struct scene_settings
{
    QColor bg_color = QColor(0, 0, 0);
    QVector3D camera_position{};
    QMatrix4x4 model_matrix{};
    QMatrix4x4 view_matrix{};
    QMatrix4x4 projection_matrix{};
    QMatrix3x3 normal_matrix{};
    l_settings light_settings{};
    QVector2D dimensions{};
    float ratio{};
    int projection_type{};
} s_settings;

typedef struct vertex_settings
{
    int type;
    double size;
    QVector3D color;
} v_settings;

typedef struct edge_settings
{
    int type;
    double width;
    QVector3D color;
} e_settings;

typedef struct face_settings
{
    int type;
    QVector3D color;
} f_settings;

typedef struct drawing_settings
{
    int light_mode = OFF;
    int texture_mode = OFF;
    int material_mode = OFF;
    QMap<QString, QVector<Material>> materials;
    QMap<QString, QVector<QOpenGLTexture *>> material_textures;
    QOpenGLTexture *texture = nullptr;
    v_settings vertex_settings{};
    e_settings edge_settings{};
    f_settings face_settings{};
} d_settings;

class Config
{
public:
    Config()
        : drawing_settings_{}
        , scene_settings_{}
    {}
    ~Config()
    {
        // for (auto material : drawing_settings_.material_textures) {
        //     for (auto texture : material) {
        //         texture->destroy();
        //         delete texture;
        //     }
        // }
    }

    // drawing settings

    void SetBgColor(QColor color);
    void SetVertexColor(QColor color);
    void SetEdgeColor(QColor color);
    void SetFaceColor(QColor color);
    void SetLightColor(QColor color)
    {
        scene_settings_.light_settings.ambient_color = {color.redF(), color.greenF(), color.blueF()};
        scene_settings_.light_settings.diffuse_color = {color.redF(), color.greenF(), color.blueF()};
        scene_settings_.light_settings.specular_color = {color.redF(),
                                                         color.greenF(),
                                                         color.blueF()};
    }
    void SetEdgeWidth(unsigned int width);
    void SetVertexSize(unsigned int size);
    void SetVertexType(int type);
    void SetEdgeType(int type);
    void SetFaceType(int type);

    // scene settings

    void SetLightPosition(QVector3D position)
    {
        scene_settings_.light_settings.position = position;
    }
    void SetWidgetWidth(float width);
    void SetWidgetHeight(float height);
    void SetProjectionType(int type);
    void SetAspectRatio(float ratio);
    void SetProjectionMatrix(const QMatrix4x4 &matrix);
    void SetViewMatrix(const QMatrix4x4 &matrix);
    void SetCameraPosition(const QVector3D &position);
    void SetModelMatrix(const QMatrix4x4 &matrix);
    void SetNormalMatrix(const QMatrix3x3 &matrix);
    void SetTexture(QOpenGLTexture *texture) { drawing_settings_.texture = texture; }

    void SetAmbientLight(double value) { scene_settings_.light_settings.ambient_intensity = value; }
    void SetDiffuseLight(double value) { scene_settings_.light_settings.diffuse_intensity = value; }
    void SetSpecularLight(double value)
    {
        scene_settings_.light_settings.specular_intensity = value;
    }
    void SetLightMode(int mode);
    void SetTextureMode(int mode) { drawing_settings_.texture_mode = mode; }
    void SetMaterialMode(int mode) { drawing_settings_.material_mode = mode; }
    void AddMaterialLib(const QString &name, const QVector<Material> &materials)
    {
        drawing_settings_.materials[name] = materials;
    }
    void AddMaterialTexture(const QString &name, QOpenGLTexture *texture)
    {
        drawing_settings_.material_textures[name].push_back(texture);
    }

    QVector<QOpenGLTexture *> GetMaterialTextures(const QString &name)
    {
        QVector<QOpenGLTexture *> material_textures;
        if (drawing_settings_.material_textures.find(name)
            != drawing_settings_.material_textures.end()) {
            return material_textures;
        } else {
            return drawing_settings_.material_textures[name];
        }
    }

    QVector<Material> GetMaterialLib(const QString &mtllib_name) const
    {
        QVector<Material> materials;
        if (drawing_settings_.materials.find(mtllib_name) == drawing_settings_.materials.end()) {
            return materials;
        } else {
            return drawing_settings_.materials[mtllib_name];
        }
    }

    // drawing settings
    QColor BgColor();
    QVector3D VertexColor();
    QVector3D EdgeColor();
    QVector3D FaceColor();
    unsigned int EdgeWidth();
    unsigned int VertexSize();
    int VertexType();
    int EdgeType();
    int FaceType();
    QOpenGLTexture *Texture() { return drawing_settings_.texture; }
    QOpenGLTexture *MaterialTexture() { return drawing_settings_.texture; }
    int TextureMode() { return drawing_settings_.texture_mode; }
    int MaterialMode() { return drawing_settings_.material_mode; }
    int LightMode() { return drawing_settings_.light_mode; }

    // scene settings
    float AspectRatio();
    float WidgetWidth();
    float WidgetHeight();
    int ProjectionType();
    const QMatrix4x4 &ProjectionMatrix();
    const QMatrix4x4 &ViewMatrix();
    QVector3D CameraPosition();
    light_settings Light() { return scene_settings_.light_settings; }

    const QMatrix4x4 &ModelMatrix();
    QMatrix3x3 NormalMatrix();

private:
    d_settings drawing_settings_;
    s_settings scene_settings_;

signals:
};

#endif // CONFIG_H
