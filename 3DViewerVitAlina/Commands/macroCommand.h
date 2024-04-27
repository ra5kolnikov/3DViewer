#ifndef MACROCOMMAND_H
#define MACROCOMMAND_H
#include <vector>

#include "Command.h"

// MacroCommand that executes a sequence of sub-commands
class MacroCommand : public Command {
 public:
  void addCommand(Command *command) { commands.push_back(command); }

  bool execute() override {
    for (Command *command : commands) {
        if (!command->execute())
            return false;
    }
    return true;
  }

  bool undo() override {
    // Undo commands in reverse order
    for (auto it = commands.rbegin(); it != commands.rend(); ++it) {
      (*it)->undo();
    }
    return true;
  }

 private:
  std::vector<Command *> commands;
};

#endif  // MACROCOMMAND_H
