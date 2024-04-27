#ifndef PARSEFILECOMMAND_H
#define PARSEFILECOMMAND_H

#include <QFile>
#include <QString>
#include <QTextStream>

#include "Command.h"
#include "Models/objectmodel.h"
#include "face.h"

class ParseFileCommand : public Command {
 public:
  ParseFileCommand(ObjectModel *model) : model_(model) {}

  ~ParseFileCommand() = default;

  bool execute()
  {
      QFile file(model_->_filePath);
      if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
      unsigned int offset = 0;
      QTextStream in(&file);
      QString mtllib = "";
      QString cur_material = "";
      while (!in.atEnd()) {
          QString line = in.readLine().trimmed();
          if (line.startsWith("v ")) {
              QVector3D vertex;
              QStringList parts = line.split(" ", Qt::SkipEmptyParts);
              vertex = QVector3D(parts[1].toDouble(), parts[2].toDouble(), parts[3].toDouble());
              model_->addVertex(vertex);
          } else if (line.startsWith("vn ")) {
              QVector3D normal;
              QStringList parts = line.split(" ", Qt::SkipEmptyParts);
              normal = QVector3D(parts[1].toDouble(), parts[2].toDouble(), parts[3].toDouble());
              model_->addNormal(normal);
          } else if (line.startsWith("vt ")) {
              QVector2D texture_coords;
              QStringList parts = line.split(" ", Qt::SkipEmptyParts);
              texture_coords = QVector2D(parts[1].toDouble(), parts[2].toDouble());
              model_->addTextureCoords(texture_coords);
          } else if (line.startsWith("usemtl")) {
              QStringList parts = line.split(" ", Qt::SkipEmptyParts);
              if (parts.size() > 1) {
                  // QString materialName = parts[1];
                  // model_->finalizeMaterials(); // Завершаем предыдущий материал
                  // model_->currentMaterial_ = materialName;
                  cur_material = parts[1];
              }
          } else if (line.startsWith("mtllib")) {
              QStringList parts = line.split(" ", Qt::SkipEmptyParts);
              if (parts.size() > 1) {
                  // QString materialName = parts[1];
                  // model_->finalizeMaterials(); // Завершаем предыдущий материал
                  // model_->currentMaterial_ = materialName;
                  mtllib = parts[1];
              }
          } else if (line.startsWith("f ")) {
              Face face = {};
              face.SetOffset(offset);
              face.SetMaterialName(cur_material);
              QStringList parts = line.split(" ", Qt::SkipEmptyParts);
              QVector<unsigned int> polygon;
              // QVector<unsigned int> normalIndices;
              for (int i = 1; i < parts.size(); ++i) {
                  QStringList indices = parts[i].split("/");
                  if (indices.size() >= 1) {
                      unsigned int vertex_index = indices[0].toUInt();
                      face.AddVertexIndex(vertex_index);
                      // polygon.push_back(vertex_index);
                  }

                  if (indices.size() >= 2 && indices[1] != "") {
                      // qDebug() << "i = " << indices[1];
                      unsigned int texture_coords_index = indices[1].toUInt();
                      face.AddTextureCoordsIndex(texture_coords_index);
                  }

                  if (indices.size() >= 3) {
                      unsigned int normal_index = indices[2].toUInt();
                      face.AddNormalIndex(normal_index);
                  }
              }
              // qDebug() << face.TextureCoordsIndices();
              model_->addPolygon(polygon);
              model_->addFace(face);
              offset += face.VertexCount();
          }
      }
      // // model_->finalizeMaterials(); // Завершаем последний материал
      // for (const auto &model_face : model_->_faces) {
      //     qDebug() << model_face.MaterialName();
      // }
      // qDebug() << "Lib: " << mtllib;
      model_->mtllib_ = mtllib;
      return true;
  }

  bool undo() { return true; }

 private:
  ObjectModel *model_;
};

#endif  // PARSEFILECOMMAND_H
