#ifndef PROCESSNEWFILECOMMAND_H
#define PROCESSNEWFILECOMMAND_H
#include "Commands/macroCommand.h"
#include "Models/scenemodel.h"
#include "convertdatacommand.h"
#include "parsefilecommand.h"
class ProcessNewFileCommand : public MacroCommand
{
public:
    ProcessNewFileCommand(const QString &file_name, ObjectModel *model, SceneModel *scene_model)

    {
        model->clear();
        parse_command_ = new ParseFileCommand(model);
        convert_command_ = new ConvertDataCommand(model, scene_model);
        addCommand(parse_command_);
        addCommand(convert_command_);
        model->setFilePath(file_name);
    }

    ~ProcessNewFileCommand()
    {
        delete parse_command_;
        delete convert_command_;
    }

private:
    ParseFileCommand *parse_command_;
    ConvertDataCommand *convert_command_;
};

#endif  // PROCESSNEWFILECOMMAND_H
