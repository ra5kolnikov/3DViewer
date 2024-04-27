#include "builder.h"

Builder &Builder::ConfigureEdgeSettings(QColor color, double width, int type)
{
    config_.SetEdgeColor(color);
    config_.SetEdgeWidth(width);
    config_.SetEdgeType(type);
    return *this;
}
Builder &Builder::ConfigureVertexSettings(QColor color, double size, int type)
{
    config_.SetVertexColor(color);
    config_.SetVertexSize(size);
    config_.SetVertexType(type);
    return *this;
}

Builder &Builder::ConfigureFaceSettings(QColor color, int type)
{
    config_.SetFaceColor(color);
    config_.SetFaceType(type);
    return *this;
}

Builder &Builder::ConfigureProjectionMatrixSettings(int type, const QMatrix4x4 &matrix)
{
    config_.SetProjectionType(type);
    config_.SetProjectionMatrix(matrix);
    return *this;
}
Builder &Builder::ConfigureProjectionMatrixSettings(
    int type, float fov, QVector2D scene_dimensions, QVector4D view_volume, QVector2D planes)
{
    QMatrix4x4 projection_matrix{};
    if (type == PARALLEL) {
        projection_matrix.perspective(fov,
                                      scene_dimensions.x() / scene_dimensions.y(),
                                      0.1,
                                      planes.y());
    } else {
        const auto &[left, right, bottom, top] = view_volume;
        projection_matrix.ortho(left, right, bottom, top, planes.x(), planes.y());
    }
    config_.SetProjectionType(type);
    config_.SetProjectionMatrix(projection_matrix);
    return *this;
}

Builder &Builder::ConfigureModelMatrixSettings(const QVector3D &translation_vector,
                                               const QVector3D &rotation_vector,
                                               double scale)
{
    const auto &[x, y, z] = rotation_vector;
    QMatrix4x4 translation_matrix{}, rotation_matrix{}, scale_matrix{};

    rotation_matrix.rotate(x, {1, 0, 0});
    rotation_matrix.rotate(y, {0, 1, 0});
    rotation_matrix.rotate(z, {0, 0, 1});

    translation_matrix.translate(translation_vector);
    scale_matrix.scale(scale);

    config_.SetModelMatrix(translation_matrix * rotation_matrix * scale);
    return *this;
}

Builder &Builder::ConfigureViewMatrixSettings(const QMatrix4x4 &matrix)
{
    config_.SetViewMatrix(matrix);
    return *this;
}

Builder &Builder::ConfigureViewMatrixSettings(QVector3D camera_position)
{
    QMatrix4x4 view_matrix{};
    view_matrix.lookAt(camera_position, {0, 0, 0}, {0, 1, 0});
    config_.SetCameraPosition(camera_position);
    config_.SetViewMatrix(view_matrix);
    return *this;
}

Builder &Builder::ConfigureSceneSettings(QColor bg_color)
{
    config_.SetBgColor(bg_color);
    return *this;
}
Config Builder::build()
{
    return config_;
}
