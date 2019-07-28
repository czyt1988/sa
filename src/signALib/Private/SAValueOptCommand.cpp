#include "SAValueOptCommand.h"
#include "SAValueManager.h"
SAValueOptCommand::SAValueOptCommand(const QString &cmdName):QUndoCommand(cmdName)
{

}

//=============================================================


SAValueAddCommand::SAValueAddCommand(const QString &cmdName, const std::shared_ptr<SAAbstractDatas> &ptr)
    :SAValueOptCommand(cmdName)
{
    m_addedPtr = ptr;
}

void SAValueAddCommand::redo()
{
    saValueManager->__addData(m_addedPtr);
}

void SAValueAddCommand::undo()
{
    saValueManager->__removeData(m_addedPtr.get());
}
//=============================================================

SAValuesAddCommand::SAValuesAddCommand(const QString &cmdName, const QList<std::shared_ptr<SAAbstractDatas> > &ptrs)
    :SAValueOptCommand(cmdName)
{
    m_addedPtrs = ptrs;
}

void SAValuesAddCommand::redo()
{
    saValueManager->__addDatas(m_addedPtrs);
}

void SAValuesAddCommand::undo()
{
    saValueManager->__removeDatas(SAValueManager::fromSmartPtr( m_addedPtrs));
}


//=============================================================

SAValueRemoveCommand::SAValueRemoveCommand(const QString &cmdName, const std::shared_ptr<SAAbstractDatas> &ptr)
    :SAValueOptCommand(cmdName)
{
    m_deletedPtr = ptr;
    m_parent = ptr->parent();
    m_row = ptr->fieldRow();
}

void SAValueRemoveCommand::redo()
{
    saValueManager->__removeData(m_deletedPtr.get());
    if(m_parent)
    {
        m_parent->takeChild(m_deletedPtr.get());
    }
}

void SAValueRemoveCommand::undo()
{
    saValueManager->__addData(m_deletedPtr);
    if(m_parent)
    {
        m_parent->insertChild(m_deletedPtr.get(),m_row);
    }
}


//=============================================================


SAValuesRemoveCommand::SAValuesRemoveCommand(const QString &cmdName, const QList<std::shared_ptr<SAAbstractDatas> > &ptrs)
    :SAValueOptCommand(cmdName)
{
    m_deletedPtrs = ptrs;
}

void SAValuesRemoveCommand::redo()
{
    saValueManager->__removeDatas(SAValueManager::fromSmartPtr( m_deletedPtrs));
}

void SAValuesRemoveCommand::undo()
{
    saValueManager->__addDatas(m_deletedPtrs);
}
//=============================================================



SAValueRenameCommand::SAValueRenameCommand(const QString &cmdName, const QString dataNewName, const std::shared_ptr<SAAbstractDatas> &ptr)
    :SAValueOptCommand(cmdName)
{
    m_dataPtr = ptr;
    m_newName = dataNewName;
    m_oldName = m_dataPtr->getName();
}

void SAValueRenameCommand::redo()
{
    saValueManager->__renameData(m_dataPtr.get(),m_newName);
}

void SAValueRenameCommand::undo()
{
    saValueManager->__renameData(m_dataPtr.get(),m_oldName);
}



