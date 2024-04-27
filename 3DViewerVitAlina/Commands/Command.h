#ifndef COMMAND_H
#define COMMAND_H

class Command {
 public:
  virtual bool execute() = 0;
  virtual bool undo() = 0;
};

#endif  // COMMAND_H
