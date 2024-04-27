#include "director.h"

Config Director::BuildConfig1()
{
    QMatrix4x4 view_matrix;
    QMatrix4x4 projection_matrix;
    view_matrix.setToIdentity();
    projection_matrix.setToIdentity();
    Builder builder;
    builder.ConfigureEdgeSettings(QColor(137, 255, 238), 1, SOLID);
    builder.ConfigureVertexSettings(QColor(255, 128, 129), 1, CIRCLE);
    builder.ConfigureFaceSettings(QColor(137, 255, 238), DISPLAY);
    builder.ConfigureProjectionMatrixSettings(PARALLEL, projection_matrix);
    builder.ConfigureModelMatrixSettings({}, {}, 1.0);
    builder.ConfigureViewMatrixSettings(view_matrix);
    builder.ConfigureSceneSettings(QColor(36, 71, 71));

    return builder.build();
}

Config Director::BuildConfig2()
{
    QMatrix4x4 view_matrix;
    QMatrix4x4 projection_matrix;
    view_matrix.setToIdentity();
    projection_matrix.setToIdentity();
    Builder builder;
    builder.ConfigureEdgeSettings(QColor(137, 255, 238), 1, DISPLAY);
    builder.ConfigureVertexSettings(QColor(255, 128, 129), 1, CIRCLE);
    builder.ConfigureProjectionMatrixSettings(CENTRAL, projection_matrix);
    builder.ConfigureModelMatrixSettings({}, {}, 0.7);
    builder.ConfigureViewMatrixSettings(view_matrix);
    builder.ConfigureSceneSettings(QColor(36, 71, 71));
    return builder.build();
}
