#include "scene.h"

Scene::~Scene()
{
    delete shader_program_;
    delete light_shader_program_;
    delete vertices_buffer_;
    delete normals_buffer_;
    delete uvs_buffer_;
    delete vao_main_;
    delete vao_light_;
}
Scene::Scene(QWidget *parent)
    : QOpenGLWidget{parent}
    , model_count_(0)
    , config_(Config{})
    , mesh_model_(MeshModel{})
    , shader_program_(new QOpenGLShaderProgram())
    , light_shader_program_(new QOpenGLShaderProgram())
    , vao_main_(new QOpenGLVertexArrayObject())
    , vao_light_(new QOpenGLVertexArrayObject())
    , vertices_buffer_(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer))
    , normals_buffer_(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer))
    , uvs_buffer_(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer))
{
}

void Scene::BindMaterials() {}

std::string glErrorToString(GLenum error) {
  switch (error) {
    case GL_NO_ERROR:
      return "No error";
    case GL_INVALID_ENUM:
      return "Invalid enum";
    case GL_INVALID_VALUE:
      return "Invalid value";
    case GL_INVALID_OPERATION:
      return "Invalid operation";
    case GL_STACK_OVERFLOW:
      return "Stack overflow";
    case GL_STACK_UNDERFLOW:
      return "Stack underflow";
    case GL_OUT_OF_MEMORY:
      return "Out of memory";
    default:
      return "Unknown error";
  }
}

void checkGLError(const char *file, int line) {
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    qDebug() << "OpenGL error (" << glErrorToString(error) << ") at " << file
             << ":" << line;
  }
}

void Scene::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CCW);
    CreateShaderProgram();
}

bool Scene::CreateShaderProgram()
{
    // data buffers on gpu
    vertices_buffer_->create();
    vertices_buffer_->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    normals_buffer_->create();
    normals_buffer_->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    uvs_buffer_->create();
    uvs_buffer_->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    // base program
    shader_program_->create();
    shader_program_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertexshader.vs");
    shader_program_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragmentshader.fs");

    if (!shader_program_->link()) {
        qDebug() << "Shader linking failed:" << shader_program_->log();
        return false;
    }

    // save data buffers attributes for base program in vao_main
    vao_main_->create();

    vao_main_->bind();
    shader_program_->bind();

    vertices_buffer_->bind();
    shader_program_->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));
    shader_program_->enableAttributeArray(0);
    vertices_buffer_->release();

    uvs_buffer_->bind();
    shader_program_->setAttributeBuffer(1, GL_FLOAT, 0, 2, sizeof(QVector2D));
    shader_program_->enableAttributeArray(1);
    uvs_buffer_->release();

    shader_program_->release();
    vao_main_->release();

    // light program
    light_shader_program_->create();
    light_shader_program_->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                   ":/shaders/lightshader.vert");
    light_shader_program_->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                   ":/shaders/lightshader.frag");

    if (!light_shader_program_->link()) {
        qDebug() << "Shader linking failed:" << light_shader_program_->log();
        return false;
    }

    // save data buffers attributes for light program in vao_light
    vao_light_->create();

    vao_light_->bind();
    light_shader_program_->bind();

    vertices_buffer_->bind();
    light_shader_program_->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));
    light_shader_program_->enableAttributeArray(0);
    vertices_buffer_->release();

    normals_buffer_->bind();
    light_shader_program_->setAttributeBuffer(1, GL_FLOAT, 0, 3, sizeof(QVector3D));
    light_shader_program_->enableAttributeArray(1);
    normals_buffer_->release();

    uvs_buffer_->bind();
    light_shader_program_->setAttributeBuffer(2, GL_FLOAT, 0, 2, sizeof(QVector2D));
    light_shader_program_->enableAttributeArray(2);
    uvs_buffer_->release();

    light_shader_program_->release();
    vao_light_->release();
    return true;
}

void Scene::UpdateBuffers()
{
    // Update vertices buffer
    vertices_buffer_->bind();
    vertices_buffer_->allocate(mesh_model_.Vertices().data(),
                               mesh_model_.Vertices().size() * sizeof(QVector3D));
    vertices_buffer_->release();

    // Update normals buffer
    normals_buffer_->bind();
    normals_buffer_->allocate(mesh_model_.Normals().data(),
                              mesh_model_.Normals().size() * sizeof(QVector3D));
    normals_buffer_->release();

    // Update texture coords buffer
    uvs_buffer_->bind();
    uvs_buffer_->allocate(mesh_model_.TextureCoords().data(),
                          mesh_model_.TextureCoords().size() * sizeof(QVector2D));
    uvs_buffer_->release();
}

void Scene::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    checkGLError(__FILE__, __LINE__);
}
void Scene::paintGL()
{
    glClearColor(config_.BgColor().redF(),
                 config_.BgColor().greenF(),
                 config_.BgColor().blueF(),
                 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (model_count_ == 0)
        return;

    if (config_.LightMode())
        RenderSceneWithLight();
    else
        RenderBasicScene();
}

void Scene::AddMeshModel(const MeshModel &mesh_model)
{
    mesh_model_ = mesh_model;
    model_count_ = 1;
    mesh_model_.SetSettings(config_);
    UpdateBuffers();
    update();
}

void Scene::RemoveMeshModel()
{
    mesh_model_.Clear();
    model_count_ = 0;
    update();
}

void Scene::UpdateConfig(const Config &config)
{
    config_ = config;
    mesh_model_.SetSettings(config);
    update();
}

void Scene::RenderBasicScene()
{
    vao_main_->bind();
    mesh_model_.Render(shader_program_);
    vao_main_->release();
}
void Scene::RenderSceneWithLight()
{
    vao_light_->bind();
    mesh_model_.Render(light_shader_program_);
    vao_light_->release();
}
