#ifndef SAVALUEOPTCOMMAND_H
#define SAVALUEOPTCOMMAND_H
#include <QUndoCommand>
#include "SAAbstractDatas.h"
#include <memory>
///
/// \brief 用于支持变量管理的命令操作
/// \date 2017-05-30
/// \author czy
///
class SAValueOptCommand : public QUndoCommand
{
public:
    SAValueOptCommand(const QString &cmdName);
};

///
/// \brief 变量的添加命令
///
class SAValueAddCommand : public SAValueOptCommand
{
public:
    SAValueAddCommand(const QString &cmdName,const std::shared_ptr<SAAbstractDatas>& ptr);
    void redo();
    void undo();
private:
    std::shared_ptr<SAAbstractDatas> m_addedPtr;
};
///
/// \brief 变量的添加命令
///
class SAValuesAddCommand : public SAValueOptCommand
{
public:
    SAValuesAddCommand(const QString &cmdName,const QList<std::shared_ptr<SAAbstractDatas> >& ptrs);
    void redo();
    void undo();
private:
    QList<std::shared_ptr<SAAbstractDatas> > m_addedPtrs;
};
///
/// \brief 变量的删除命令
///
class SAValueRemoveCommand : public SAValueOptCommand
{
public:
    SAValueRemoveCommand(const QString &cmdName,const std::shared_ptr<SAAbstractDatas>& ptr);
    void redo();
    void undo();
private:
    std::shared_ptr<SAAbstractDatas> m_deletedPtr;
    SAItem* m_parent;
    int m_row;
};
///
/// \brief 变量的删除命令
///
class SAValuesRemoveCommand : public SAValueOptCommand
{
public:
    SAValuesRemoveCommand(const QString &cmdName,const QList<std::shared_ptr<SAAbstractDatas> >& ptrs);
    void redo();
    void undo();
private:
    QList<std::shared_ptr<SAAbstractDatas> > m_deletedPtrs;
};
///
/// \brief 变量的改名命令
///
class SAValueRenameCommand : public SAValueOptCommand
{
public:
    SAValueRenameCommand(const QString &cmdName,const QString dataNewName,const std::shared_ptr<SAAbstractDatas>& ptr);
    void redo();
    void undo();
private:
    QString m_oldName;
    QString m_newName;
    std::shared_ptr<SAAbstractDatas> m_dataPtr;
};
#endif // SAVALUEOPTCOMMAND_H
