#include "objectmodel.h"

ObjectModel::ObjectModel() {}

void ObjectModel::setFilePath(const QString &str) { _filePath = str; }

void ObjectModel::addVertex(const QVector3D &vertex)
{
    if (_vertices.empty())
        min_ = max_ = vertex;

    _vertices.push_back(vertex);
    _countVertices++;
    auto &[x, y, z] = vertex;

    min_.setX(std::min(x, min_.x()));
    min_.setY(std::min(y, min_.y()));
    min_.setZ(std::min(z, min_.z()));
    max_.setX(std::max(x, max_.x()));
    max_.setY(std::max(y, max_.y()));
    max_.setZ(std::max(z, max_.z()));
}

void ObjectModel::addNormal(const QVector3D &normal)
{
    _normals.push_back(normal);
}

void ObjectModel::addPolygon(const QVector<unsigned int> &polygon) {
  _polygons.push_back(polygon);
  _countPolygons++;
}

void ObjectModel::addFace(const Face &face)
{
    _faces.push_back(face);
    _countFaces++;
}

void ObjectModel::addMaterial(const QString &materialName,
                              const QVector<QVector<unsigned int>> &polygons) {
  polygonsWithMaterial_[materialName] = polygons;
}

void ObjectModel::finalizeMaterials() {
  if (!_polygons.isEmpty()) {
    // Добавляем оставшиеся полигоны к последнему материалу
    polygonsWithMaterial_[currentMaterial_] += _polygons;
    _polygons.clear();  // Очищаем временное хранилище полигонов
  }
}

void ObjectModel::centerVertices()
{
    QVector3D center = getCenter();
    for (auto &vertex : _vertices) {
        vertex -= center;
    }
}

QVector3D ObjectModel::getNormal(unsigned int index)
{
    return _normals[index];
}

QVector3D ObjectModel::getVertex(unsigned int index)
{
    return _vertices[index];
}

QVector3D ObjectModel::getCenter()
{
    return (max_ + min_) / 2;
}

QVector3D ObjectModel::getMax()
{
    return max_;
}
QVector3D ObjectModel::getMin()
{
    return min_;
}

void ObjectModel::clearPolygons() { _polygons.clear(); }

void ObjectModel::clear() {
    mtllib_.clear();
    _faces.clear();
    _vertices.clear();
    _normals.clear();
    _texture_coords.clear();
    _countVertices = 0;
    _countFaces = 0;
    _filePath.clear();
    _polygons.clear();
    polygonsWithMaterial_.clear();
    currentMaterial_.clear();
    _countPolygons = 0;
}
