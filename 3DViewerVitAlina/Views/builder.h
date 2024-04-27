#ifndef BUILDER_H
#define BUILDER_H

#include <QObject>
#include "config.h"

class Builder
{
public:
    Builder() = default;

public:
    Builder &ConfigureEdgeSettings(QColor color, double width, int type);
    Builder &ConfigureVertexSettings(QColor color, double size, int type);
    Builder &ConfigureFaceSettings(QColor color, int type);
    Builder &ConfigureProjectionMatrixSettings(
        int type, float fov, QVector2D scene_dimensions, QVector4D view_volume, QVector2D planes);
    Builder &ConfigureProjectionMatrixSettings(int type, const QMatrix4x4 &matrix);
    Builder &ConfigureViewMatrixSettings(int type, const QMatrix4x4 &matrix);
    Builder &ConfigureModelMatrixSettings(const QVector3D &translation_vector,
                                          const QVector3D &rotation_vector,
                                          double scale);
    Builder &ConfigureViewMatrixSettings(const QMatrix4x4 &matrix);
    Builder &ConfigureViewMatrixSettings(QVector3D camera_position);
    Builder &ConfigureSceneSettings(QColor bg_color);
    Config build();

signals:

private:
    Config config_;
};

#endif // BUILDER_H
