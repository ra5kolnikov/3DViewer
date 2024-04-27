#ifndef CONVERTDATACOMMAND_H
#define CONVERTDATACOMMAND_H

#include "Command.h"
#include "Models/objectmodel.h"
#include "Models/scenemodel.h"
#include "qfileinfo.h"
#include "qfontinfo.h"
class ConvertDataCommand : public Command
{
public:
    explicit ConvertDataCommand(ObjectModel *model, SceneModel *scene_model)
        : model_(model)
        , scene_model_(scene_model)
    {}

#include <QVector3D>
#include <unordered_map>
#include <vector>

    double dcos(float angle_in_degrees) { return std::cos(angle_in_degrees * M_PI / 180.0); }

    // for smooth faces [use with treshold = dcos(30)]
    std::vector<QVector3D> GenVertexNormals(float threshold)
    {
        std::unordered_map<size_t, QVector3D> normalCache;

        std::vector<QVector3D> normals(model_->_vertices.size(), {0, 0, 0});

        for (const auto &face : model_->_faces) {
            size_t numVertices = face.VertexCount();

            // Skip faces with less than 3 vertices
            if (numVertices < 3) {
                QVector3D defaultNormal = {0.0f, 0.0f, 1.0f};
                for (size_t idx : face.VertexIndices()) {
                    normals[idx] = defaultNormal;
                    normalCache[idx] = defaultNormal;
                }
                continue;
            }

            // Compute normals for each triangle formed by the face's vertices
            for (size_t i = 1; i < numVertices - 1; ++i) {
                QVector3D v1 = model_->getVertex(face.VertexIndices()[0]);
                QVector3D v2 = model_->getVertex(face.VertexIndices()[i]);
                QVector3D v3 = model_->getVertex(face.VertexIndices()[i + 1]);

                QVector3D e1 = v2 - v1;
                QVector3D e2 = v3 - v1;

                QVector3D norm = QVector3D::crossProduct(e1, e2).normalized();

                // Add the computed normal to each vertex of the current triangle
                normals[face.VertexIndices()[0]] += norm;
                normals[face.VertexIndices()[i]] += norm;
                normals[face.VertexIndices()[i + 1]] += norm;

                // Cache the normal for each vertex
                for (size_t idx : face.VertexIndices()) {
                    if (normalCache.find(idx) == normalCache.end()) {
                        normalCache[idx] = norm;
                    } else {
                        normalCache[idx] += norm;
                    }
                }
            }
        }

        for (const auto &face : model_->_faces) {
            for (size_t idx : face.VertexIndices()) {
                QVector3D existingNormal = normals[idx];
                QVector3D smoothedNormal = normalCache[idx].normalized();

                if (QVector3D::dotProduct(existingNormal, smoothedNormal) > threshold) {
                    normals[idx] = smoothedNormal;
                }
            }
        }

        return normals;
    }

    std::vector<QVector3D> GenVertexNormals2(float threshold)
    {
        std::unordered_map<size_t, QVector3D> normalCache;
        std::vector<QVector3D> normals(model_->_vertices.size(), {0, 0, 0});

        for (const auto &face : model_->_faces) {
            // Compute face normal using cross product of triangle edges
            QVector3D faceNormal
                = QVector3D::crossProduct(model_->getVertex(face.VertexIndices()[1])
                                              - model_->getVertex(face.VertexIndices()[0]),
                                          model_->getVertex(face.VertexIndices()[2])
                                              - model_->getVertex(face.VertexIndices()[0]))
                      .normalized();

            // Accumulate face normal for each vertex of the face
            for (size_t idx : face.VertexIndices()) {
                normalCache[idx] += faceNormal;
            }
        }

        // Apply thresholding logic to decide whether to use smoothed normal or existing normal
        for (const auto &[idx, cachedNormal] : normalCache) {
            QVector3D existingNormal = normals[idx];
            QVector3D smoothedNormal = cachedNormal.normalized();

            // if (QVector3D::dotProduct(existingNormal, smoothedNormal) > threshold) {
            normals[idx] = smoothedNormal;
            // } else {
            // normals[idx] = existingNormal;
            // }
        }

        return normals;
    }
    QVector3D ComputeNormal(const QVector3D &v1, const QVector3D &v2, const QVector3D &v3)
    {
        QVector3D edge1 = v2 - v1;
        QVector3D edge2 = v3 - v1;
        return QVector3D::crossProduct(edge1, edge2).normalized();
    }
    // Compute normals for each triangle face
    std::vector<QVector3D> ComputeTriangleNormals(const std::vector<QVector3D> &vertices,
                                                  const std::vector<unsigned int> &indices)
    {
        std::vector<QVector3D> normals;

        for (size_t i = 0; i < indices.size(); i += 3) {
            QVector3D normal = ComputeNormal(vertices[0], vertices[1], vertices[2]);

            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
        }

        return normals;
    }
    bool execute() override
    {
        model_->centerVertices();

        const auto &min = model_->getMin();
        const auto &max = model_->getMax();
        const auto &model_center = model_->getCenter();

        double model_depth = fabs(min.z() - max.z());
        double model_width = fabs(min.x() - max.x());
        double model_height = fabs(min.y() - max.y());
        double model_size = std::max<double>({model_width, model_height, model_depth});

        int count_vertices = model_->_countVertices;
        int count_faces = model_->_countFaces;

        // populate data
        std::vector<QVector3D> vertices;
        std::vector<QVector3D> normals;
        std::vector<QVector2D> texture_coords;

        for (const auto &_face : model_->_faces) {
            // vertices from the model
            QVector<QVector3D> centered_vertices;
            for (const auto &vertex_index : _face.VertexIndices()) {
                const auto &vertex = model_->getVertex(vertex_index);
                vertices.emplace_back(vertex);
                centered_vertices.push_back(vertex);
            }

            // normals from the model
            if (!model_->_normals.empty()) {
                for (const auto &normal_index : _face.NormalIndices()) {
                    const auto &normal = model_->getNormal(normal_index);
                    normals.emplace_back(normal);
                }
            } else {
                // // calculate normals manually
                // QVector3D faceNormal = {0, 0, 0};
                // if (centered_vertices.size() >= 3) {
                //     faceNormal = ComputeNormal(centered_vertices[0],
                //                                centered_vertices[1],
                //                                centered_vertices[2]);

                //     for (const auto &_ : _face.VertexIndices()) {
                //         normals.push_back(faceNormal);
                //     }
                // }
            }
            // texture coords from the model
            if (!model_->_texture_coords.empty()) {
                for (const auto &texture_coords_index : _face.TextureCoordsIndices()) {
                    const auto &uv = model_->getTextureCoords(texture_coords_index);
                    texture_coords.emplace_back(uv);
                }
                if (_face.TextureCoordsIndices().size() < _face.VertexCount()) {
                    int num = _face.VertexCount() - _face.TextureCoordsIndices().size();
                    for (int i = 0; i < num; i++) {
                        const auto &uv = model_->getVertex(i);
                        texture_coords.emplace_back(uv * 100);
                    }
                }
            }
        }

        // if smooth normals are enabled, use this function instead for calculating normals
        if (normals.empty()) {
            // calculate normals manyally
            const auto &vertices_normals = GenVertexNormals(dcos(35));
            // qDebug() << vertices_normals;
            for (const auto &_face : model_->_faces) {
                for (const auto &vertex_index : _face.VertexIndices()) {
                    normals.push_back(vertices_normals[vertex_index]);
                }
            }
        }

        // use vertices coords as texture coords
        if (texture_coords.empty()) {
            QVector2D range = max.toVector2D() - min.toVector2D();
            for (const auto &vertex : vertices) {
                QVector2D uv = (QVector2D(vertex.x(), vertex.y()) - min.toVector2D()) / range;
                uv.setY(1.0 - uv.y()); // Flip Y-coordinate
                texture_coords.push_back(uv);
                // float U = vertex.x() == 0 ? 1.0 : vertex.x();
                // float V = vertex.y() == 0 ? 1.0 : vertex.y();
                // QVector2D uv = QVector2D(1.0, 1.0) / QVector2D(U, V);
                // uv.setX(uv.x() < 0 ? uv.x() + 0.5 : uv.x());
                // uv.setY(uv.y() < 0 ? uv.y() + 0.5 : uv.y());
            }
        }

        MeshModel mesh_model(model_->_faces,
                             vertices,
                             normals,
                             count_vertices,
                             count_faces,
                             model_center,
                             model_size);
        mesh_model.SetTextureCoords(texture_coords);
        qDebug() << texture_coords.back();

        QFileInfo lib_name(model_->mtllib_);
        mesh_model.SetMaterialLib(lib_name.fileName());
        scene_model_->AddMeshModel(mesh_model);
        return true;
    }
    bool undo() override { return true; }
    float MapValue(float value, float in_min, float in_max, float out_min, float out_max)
    {
        if (in_min == in_max) {
            return out_min; // Avoid division by zero
        }
        return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

private:
    ObjectModel *model_;
    SceneModel *scene_model_;
};

#endif // CONVERTDATACOMMAND_H
