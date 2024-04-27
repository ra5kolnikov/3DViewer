#ifndef SCENE_H
#define SCENE_H

#include <GL/gl.h>

#include <QDebug>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QWidget>
#include <QtOpenGL>
#include "Models/meshmodel.h"
#include "Views/config.h"
// projection type
#define PARALLEL 1
#define CENTRAL 2

// edge type
#define NOFACET 0
#define SOLID 1
#define DASHED 2

// vertex type
#define NOVERTEX 0
#define CIRCLE 1
#define SQUARE 2

class Scene : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    Scene(QWidget *parent = nullptr);
    ~Scene();
    void AddMeshModel(const MeshModel &mesh_model);
    void RemoveMeshModel();
    void UpdateConfig(const Config &config_);
    void RenderBasicScene();
    void RenderSceneWithLight();

private:
    void UpdateBuffers();
    void BindMaterials();
    bool CreateShaderProgram();
    // ogl functions
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    unsigned int model_count_;
    MeshModel mesh_model_;
    Config config_;
    QOpenGLShaderProgram *shader_program_;
    QOpenGLShaderProgram *light_shader_program_;
    QOpenGLVertexArrayObject *vao_main_;
    QOpenGLVertexArrayObject *vao_light_;
    QOpenGLBuffer *vertices_buffer_;
    QOpenGLBuffer *normals_buffer_;
    QOpenGLBuffer *uvs_buffer_;
};

#endif // SCENE_H
