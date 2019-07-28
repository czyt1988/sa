#include "SADataReference.h"
#include "SADataHeader.h"
SADataReference::SADataReference(SAAbstractDatas *origion)
    :SAAbstractDatas()
{
    m_linkData = origion;
    setProperty (getType(),SA_ROLE_DATA_TYPE);
}

SADataReference::SADataReference(SAAbstractDatas *origion, const QString &text):SAAbstractDatas(text)
{
    m_linkData = origion;
    setProperty (getType(),SA_ROLE_DATA_TYPE);
}




SADataReference::~SADataReference()
{
}

QString SADataReference::getTypeName() const
{
    if(m_linkData)
        return QString("ref - ") + m_linkData->getTypeName ();
    return QString("ref");
}

int SADataReference::getSize(int dim) const
{
    if(m_linkData)
        return m_linkData->getSize (dim);
    return 0;
}

int SADataReference::getDim() const
{
    if(m_linkData)
        return m_linkData->getDim ();
    return 0;
}

QVariant SADataReference::getAt(const std::initializer_list<size_t> &index) const
{
    if(m_linkData)
        return m_linkData->getAt (index);
    return QVariant();
}

QString SADataReference::displayAt(const std::initializer_list<size_t> &index) const
{
    if(m_linkData)
        return m_linkData->displayAt (index);
    return QString();
}


void SADataReference::disLink()
{
    m_linkData = nullptr;
    setName (QObject::tr("unknow ref"));
}

void SADataReference::write(QDataStream &out) const
{
    SADataHeader type(this);
    out << type;
    SAAbstractDatas::write(out);
}

bool SADataReference::isDirty() const
{
    if(m_linkData)
        return m_linkData->isDirty ();
    return false;
}

void SADataReference::setDirty(bool dirty)
{
    if(m_linkData)
        m_linkData->setDirty (dirty);
}

bool SADataReference::isEmpty() const
{
    if(m_linkData)
        return m_linkData->isEmpty ();
    return false;
}
