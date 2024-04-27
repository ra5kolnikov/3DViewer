#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <QMap>
#include <QString>
#include <QVector>

#include "face.h"
#include "meshmodel.h"
#include "mtlmodel.h"
#include "qvectornd.h"

class ObjectModel
{
public:
    ObjectModel();

    void addPolygon(const QVector<unsigned int> &polygon);
    void addTextureCoords(const QVector2D &texture_coords)
    {
        _texture_coords.emplace_back(texture_coords);
    }

    void addVertex(const QVector3D &vertex);
    void addMaterialLib(const QString &mtllib) { mtllib_ = mtllib; }
    void addNormal(const QVector3D &normal);
    void setFilePath(const QString &str);
    void clear();
    void addFace(const Face &face);
    void addMaterial(const QString &materialName, const QVector<QVector<unsigned int>> &polygons);
    void finalizeMaterials();
    void clearPolygons();
    QVector3D getMax();
    QVector3D getMin();
    QVector3D getCenter();
    QVector3D getNormal(unsigned int index);
    QVector3D getVertex(unsigned int index);
    QVector2D getTextureCoords(unsigned int index) { return _texture_coords[index]; }

    void centerVertices();

    QVector3D max_;
    QVector3D min_;
    QMap<QString, QVector<QVector<unsigned int>>> polygonsWithMaterial_;
    QVector<QVector<unsigned int>> _polygons;
    QVector<QVector3D> _vertices;
    QVector<QVector3D> _normals;
    QVector<QVector2D> _texture_coords;
    QVector<Face> _faces;
    QVector<MTLModel> materials_;
    QString currentMaterial_;
    QString mtllib_;
    QString _filePath;
    int _countFaces = 0;
    int _countPolygons = 0;
    int _countVertices = 0;
};

#endif  // OBJECTMODEL_H
