#ifndef CONVERTMATERIALSCOMMAND_H
#define CONVERTMATERIALSCOMMAND_H


#include "Commands/Command.h"
#include "Models/objectmodel.h"
#include "Models/scenemodel.h"
#include "qfileinfo.h"

#include <QFile>
class ConvertMaterialsCommand : public Command {
 public:
     ConvertMaterialsCommand(const QString &file_name, ObjectModel *model, SceneModel *scene_model)
         : mtllib_(file_name)
         , model_(model)
         , scene_model_(scene_model)
     {}
     bool execute() override
     {
         MTLModel materialModel;
         QFileInfo file_info(mtllib_);

         QFile file(mtllib_);
         QString path = file_info.path();
         if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
             qDebug() << "Failed to open MTL file: " << mtllib_;
             return false;
         }

         QTextStream in(&file);
         QVector<Material> materials;
         Material cur_material;
         bool textures = false;
         while (!in.atEnd()) {
             QString line = in.readLine().trimmed();
             QStringList parts = line.split(" ", Qt::SkipEmptyParts);
             if (parts.isEmpty())
                 continue;

             QString keyword = parts[0].toLower();
             if (keyword == "newmtl") {
                 if (cur_material.name != "") {
                     materials.push_back(cur_material);
                     cur_material = {};
                 }
                 cur_material.name = parts[1];
             } else if (keyword == "ns") {
                 cur_material.components.shininess_exponent = parts[1].toFloat();
             } else if (keyword == "ka") {
                 cur_material.components.ambient_color = QVector3D(parts[1].toFloat(),
                                                                   parts[2].toFloat(),
                                                                   parts[3].toFloat());
             } else if (keyword == "kd") {
                 cur_material.components.diffuse_color = QVector3D(parts[1].toFloat(),
                                                                   parts[2].toFloat(),
                                                                   parts[3].toFloat());
             } else if (keyword == "ks") {
                 cur_material.components.specular_color = QVector3D(parts[1].toFloat(),
                                                                    parts[2].toFloat(),
                                                                    parts[3].toFloat());
             } else if (keyword == "map_kd") {
                 QString filename = parts[1];
                 QImage texture_image(path + "/" + filename);
                 if (!texture_image.isNull()) {
                     cur_material.map_kd = texture_image;
                     cur_material.has_diffuse_texture = true;
                     textures = true;
                 } else {
                     qDebug() << "Failed to load texture " << path + filename;
                 }
             } else if (keyword == "map_bump" || keyword == "bump") {
                 QStringList parts = line.split(" ", Qt::SkipEmptyParts);
                 float bump_multiplier = 1.0; // Default value
                 if (parts.size() > 2 && parts[1] == "-bm") {
                     bump_multiplier = parts[2].toFloat();
                 }
                 QString filename = parts.last();
                 QImage texture_image(path + "/" + filename);
                 if (!texture_image.isNull()) {
                     cur_material.bump_multiplier = bump_multiplier;
                     cur_material.map_bump = texture_image;
                     cur_material.has_bump_texture = true;
                     textures = true;
                 } else {
                     qDebug() << "Failed to load texture " << path + filename;
                 }
             } else if (keyword == "map_ka") {
                 QString filename = parts[1];
                 qDebug() << filename;
                 QImage texture_image(path + "/" + filename);
                 if (!texture_image.isNull()) {
                     cur_material.map_ka = texture_image;
                     cur_material.has_ambient_texture = true;
                     textures = true;
                 } else {
                     qDebug() << "Failed to load texture " << path + filename;
                 }
             } else if (keyword == "map_ks") {
                 QString filename = parts[1];
                 qDebug() << filename;
                 QImage texture_image(path + "/" + filename);
                 if (!texture_image.isNull()) {
                     cur_material.map_ks = texture_image;
                     cur_material.has_specular_texture = true;
                     textures = true;
                 } else {
                     qDebug() << "Failed to load texture " << path + filename;
                 }
             } else if (keyword == "d ") {
                 QStringList parts = line.split(" ", Qt::SkipEmptyParts);
                 if (parts.size() > 1) {
                     cur_material.dissolve = parts[1].toFloat();
                 }
             }
         }

         if (cur_material.name != "") {
             materials.push_back(cur_material);
         }

         file.close();
         QFileInfo lib_name(mtllib_);
         scene_model_->AddMaterials(lib_name.fileName(), materials, textures);
         return true;
     }
  bool undo() override { return true; }

 private:
  ObjectModel *model_;
  SceneModel *scene_model_;
  QString mtllib_;
};

#endif  // CONVERTMATERIALSCOMMAND_H
