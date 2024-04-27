#include "config.h"
// setters

void Config::SetLightMode(int mode)
{
    drawing_settings_.light_mode = mode;
}
void Config::SetVertexType(int type)
{
    drawing_settings_.vertex_settings.type = type;
}
void Config::SetEdgeType(int type)
{
    drawing_settings_.edge_settings.type = type;
}

void Config::SetFaceType(int type)
{
    drawing_settings_.face_settings.type = type;
}
void Config::SetProjectionType(int type)
{
    scene_settings_.projection_type = type;
}

void Config::SetBgColor(QColor color)
{
    scene_settings_.bg_color = color;
}

void Config::SetFaceColor(QColor color)
{
    drawing_settings_.face_settings.color = {color.redF(), color.greenF(), color.blueF()};
}

void Config::SetVertexSize(unsigned int size)
{
    drawing_settings_.vertex_settings.size = size;
}
void Config::SetEdgeWidth(unsigned int width)
{
    drawing_settings_.edge_settings.width = width;
}
void Config::SetVertexColor(QColor color)
{
    drawing_settings_.vertex_settings.color = {color.redF(), color.greenF(), color.blueF()};
}
void Config::SetEdgeColor(QColor color)
{
    drawing_settings_.edge_settings.color = {color.redF(), color.greenF(), color.blueF()};
}

void Config::SetModelMatrix(const QMatrix4x4 &matrix)
{
    scene_settings_.model_matrix = matrix;
}

void Config::SetProjectionMatrix(const QMatrix4x4 &matrix)
{
    scene_settings_.projection_matrix = matrix;
}
void Config::SetWidgetWidth(float width)
{
    scene_settings_.dimensions.setX(width);
}
void Config::SetWidgetHeight(float height)
{
    scene_settings_.dimensions.setY(height);
}

void Config::SetViewMatrix(const QMatrix4x4 &matrix)
{
    scene_settings_.view_matrix = matrix;
}

void Config::SetAspectRatio(float ratio)
{
    scene_settings_.ratio = ratio;
}

void Config::SetCameraPosition(const QVector3D &camera_position)
{
    scene_settings_.camera_position = camera_position;
}

void Config::SetNormalMatrix(const QMatrix3x3 &matrix)
{
    scene_settings_.normal_matrix = matrix;
}

// getters

QVector3D Config::VertexColor()
{
    return drawing_settings_.vertex_settings.color;
}

QVector3D Config::EdgeColor()
{
    return drawing_settings_.edge_settings.color;
}

QVector3D Config::FaceColor()
{
    return drawing_settings_.face_settings.color;
}
QColor Config::BgColor()
{
    return scene_settings_.bg_color;
}
unsigned int Config::EdgeWidth()
{
    return drawing_settings_.edge_settings.width;
    ;
}
unsigned int Config::VertexSize()
{
    return drawing_settings_.vertex_settings.size;
}
int Config::VertexType()
{
    return drawing_settings_.vertex_settings.type;
}
int Config::EdgeType()
{
    return drawing_settings_.edge_settings.type;
}
int Config::FaceType()
{
    return drawing_settings_.face_settings.type;
}

int Config::ProjectionType()
{
    return scene_settings_.projection_type;
}

const QMatrix4x4 &Config::ModelMatrix()
{
    return scene_settings_.model_matrix;
}

const QMatrix4x4 &Config::ProjectionMatrix()
{
    return scene_settings_.projection_matrix;
}

const QMatrix4x4 &Config::ViewMatrix()
{
    return scene_settings_.view_matrix;
}

float Config::WidgetWidth()
{
    return scene_settings_.dimensions.x();
}

float Config::WidgetHeight()
{
    return scene_settings_.dimensions.y();
}

float Config::AspectRatio()
{
    return scene_settings_.ratio;
}

QVector3D Config::CameraPosition()
{
    return scene_settings_.camera_position;
}

QMatrix3x3 Config::NormalMatrix()
{
    return scene_settings_.normal_matrix;
}
