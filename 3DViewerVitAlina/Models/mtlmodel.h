#ifndef MATERIALMODEL_H
#define MATERIALMODEL_H

#include <QString>
#include <QColor>

struct MTLModel {
    QString materialName_;
    float shininess_ = 0.0f;
    QColor ambient_;
    QColor diffuse_;
    QColor specular_;
    QString textureMapDiffuse_;
    QString textureMapBump_;
    QString textureMapSpecular_;
};

#endif // MATERIALMODEL_H
