#include "SAValueTableOptCommands.h"



//////SAValueTableOptCommandPrivateBase/////////////////////////////////////////////////////
bool SAAbstractValueTableOptCommandPrivate::checkVarList(const QList<QVariantList> &varTable, int row, int col)
{
    if(row < varTable.size())
    {
        if(col < varTable[row].size())
        {
            return true;
        }
    }
    return false;
}

