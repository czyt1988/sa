#ifndef SAVALUEOPTCOMMAND_H
#define SAVALUEOPTCOMMAND_H
#include <QUndoCommand>
///
/// \brief 用于支持变量管理的命令操作
/// \date 2017-05-30
/// \author czy
///
class SAValueOptCommand : public QUndoCommand
{
public:
    SAValueOptCommand(QUndoCommand *parent = nullptr);
    SAValueOptCommand(const QString &text, QUndoCommand *parent = nullptr);
};

///
/// \brief 变量的添加命令
///
class SAValueAddCommand : public SAValueOptCommand
{
public:
    SAValueAddCommand(const QString &text, QUndoCommand *parent = nullptr);
    void redo() override;
    void undo() override;
};

///
/// \brief 变量的删除命令
///
class SAValueDeleteCommand : public SAValueOptCommand
{
public:
    SAValueDeleteCommand(const QString &text, QUndoCommand *parent = nullptr);
};

///
/// \brief 变量的改名命令
///
class SAValueRenameCommand : public SAValueOptCommand
{
public:
    SAValueRenameCommand(const QString &text, QUndoCommand *parent = nullptr);
};
#endif // SAVALUEOPTCOMMAND_H
