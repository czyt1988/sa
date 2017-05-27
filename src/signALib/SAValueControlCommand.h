#ifndef SAVALUECONTROLCOMMAND_H
#define SAVALUECONTROLCOMMAND_H
#include <QUndoCommand>
///
/// \brief 变量管理的redo/undo Command
///
class SAValueControlCommand : public QUndoCommand
{
public:
    SAValueControlCommand();
};

#endif // SAVALUECONTROLCOMMAND_H
