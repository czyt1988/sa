#include "SACsvFileTableModel.h"
#include <QFile>
#include <QTextStream>
SACsvFileTableModel::SACsvFileTableModel(const QString& fileName,QObject * parent)
    :SAVariantHashTableModel(parent)
    ,m_enableEdit(false)
{
    setHashTableData (&m_t);
    setFile(fileName);
}

bool SACsvFileTableModel::setFile(const QString& fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }
    QTextStream inStream(&file);
    QString lineStr;
    size_t row = 0;
    while (!inStream.atEnd())
    {
        lineStr = inStream.readLine();
        splitCsvLine(lineStr,row);
        ++row;
    }
    return true;
}

void SACsvFileTableModel::splitCsvLine(const QString& lineStr,size_t rows)
{
    QString str;
    int i, j=0;
    int col = 0;
    i = 0;
    do {
        if (i < lineStr.length() && lineStr[i] == '\"')
            j = advquoted(lineStr, str, ++i); // skip quote
        else
            j = advplain(lineStr, str, i);
        m_t.setData (rows,col,str);
        ++col;
        i = j + 1;
    } while (j < lineStr.length());
}

int SACsvFileTableModel::advplain(const QString& s, QString& fld, int i)
{
    int j;

    j = s.indexOf(',', i); // look for separator
    if (j < 0) // none found
        j = s.length();
    fld = s.mid (i,j-i);//string(s, i, j-i);
    return j;
}

int SACsvFileTableModel::advquoted(const QString& s, QString& fld, int i)
{
    int j;

    fld = "";
    for (j = i; j < s.length()-1; j++)
    {
        if (s[j] == '\"' && s[++j] != '\"')
        {
            int k = s.indexOf (',', j);
            if (k < 0 ) // no separator found
                k = s.length();
            for (k -= j; k-- > 0; )
                fld += s[j++];
            break;
        }
        fld += s[j];
    }
    return j;
}

QVariant SACsvFileTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if(Qt::Horizontal == orientation)
    {//说明是水平表头
        return section+1;
    }
    else if(Qt::Vertical == orientation)
    {
        return section+1;
    }
    return QVariant();
}

Qt::ItemFlags SACsvFileTableModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    if(m_enableEdit)
        return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

bool SACsvFileTableModel::getCzyHashTable(const QString& fileName, SAHashTable<QVariant>& data)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }
    QTextStream inStream(&file);
    QString lineStr;
    size_t row = 0;
    while (!inStream.atEnd())
    {
        lineStr = inStream.readLine();
        QString str;
        int i, j=0;
        int col = 0;
        i = 0;
        do {
            if (i < lineStr.length() && lineStr[i] == '\"')
                j = advquoted(lineStr, str, ++i); // skip quote
            else
                j = advplain(lineStr, str, i);
            data.setData (row,col,str);
            ++col;
            i = j + 1;
        } while (j < lineStr.length());
        ++row;
    }
    return true;
}
