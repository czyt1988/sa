#include "SAValueOptCommand.h"

SAValueOptCommand::SAValueOptCommand(QUndoCommand *parent):QUndoCommand(parent)
{

}
SAValueOptCommand::SAValueOptCommand(const QString &text, QUndoCommand *parent):QUndoCommand(text,parent)
{

}
SAValueAddCommand::SAValueAddCommand(const QString &text, QUndoCommand *parent):SAValueOptCommand(text,parent)
{

}

SAValueDeleteCommand::SAValueDeleteCommand(const QString &text, QUndoCommand *parent):SAValueOptCommand(text,parent)
{

}

SAValueRenameCommand::SAValueRenameCommand(const QString &text, QUndoCommand *parent):SAValueOptCommand(text,parent)
{

}
