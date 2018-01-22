#include "SALocalServeBaseProtocol.h"
#include <QTextCodec>
#include <QDebug>
SALocalServeBaseProtocol::SALocalServeBaseProtocol()
{

}

SALocalServeBaseProtocol::~SALocalServeBaseProtocol()
{

}

bool SALocalServeBaseProtocol::isValid() const
{
    return m_header.isValid();
}

SALocalServeBaseHeader &SALocalServeBaseProtocol::header()
{
    return m_header;
}

const SALocalServeBaseHeader &SALocalServeBaseProtocol::header() const
{
    return m_header;
}

//
//========================================================
//

SALocalServeShakeHandProtocol::SALocalServeShakeHandProtocol():SALocalServeBaseProtocol()
{
    memset(m_raw,0,sizeof(unsigned char)*SA_PROTOCOL_SHAKE_HAND_DATA_LEN);
}

void SALocalServeShakeHandProtocol::serializeIn(QDataStream &st)
{
    //TODO 考虑加校验位
    st >> m_header;
    st.readRawData((char*)m_raw,SA_PROTOCOL_SHAKE_HAND_DATA_LEN);
}

void SALocalServeShakeHandProtocol::serializeOut(QDataStream &st) const
{
    SALocalServeBaseHeader header;
    header.init();
    header.type = (int)TypeShakeHand;
    header.dataSize = SA_PROTOCOL_SHAKE_HAND_DATA_LEN;
    st << header;
    st.writeRawData((char*)m_raw,header.dataSize);
}


SALocalServeBaseProtocol::Type SALocalServeShakeHandProtocol::type() const
{
    return TypeShakeHand;
}

//===================
SALocalServeStringProtocol::SALocalServeStringProtocol():SALocalServeBaseProtocol()
{

}

void SALocalServeStringProtocol::serializeIn(QDataStream &st)
{
    st >> m_header;
    QByteArray encodedString;
    encodedString.resize(m_header.dataSize);
    st.readRawData(encodedString.data(),m_header.dataSize);
    m_str = QString::fromUtf8(encodedString);

//    qDebug() << "SALocalServeStringProtocol::serializeInData:encodedString byte size"<<encodedString.size()
//             << " hex:" << QString(encodedString.toHex())
//             <<" fromUtf8:"<<m_str

//               ;
}

void SALocalServeStringProtocol::serializeOut(QDataStream &st) const
{
    QByteArray encodedString = m_str.toUtf8();
//qDebug() << "SALocalServeStringProtocol::serializeOutAll:"
//         << m_str
//         << " toUtf8 size:"<<encodedString.size()
//            << " hex:" << QString(encodedString.toHex())
//            ;
    SALocalServeBaseHeader header;
    header.init();
    header.type = (int)TypeString;
    header.dataSize = encodedString.size();
    st << header;
    st.writeRawData(encodedString.data(),header.dataSize);
}


SALocalServeBaseProtocol::Type SALocalServeStringProtocol::type() const
{
    return TypeString;
}

const QString &SALocalServeStringProtocol::string() const
{
    return m_str;
}

QString &SALocalServeStringProtocol::string()
{
    return m_str;
}

void SALocalServeStringProtocol::setString(const QString &str)
{
    m_str = str;
}

//===================

SALocalServeVectorPointProtocol::SALocalServeVectorPointProtocol():SALocalServeBaseProtocol()
  ,m_winId(0)
  ,m_figId(0)
  ,m_itemId(0)
{

}

void SALocalServeVectorPointProtocol::serializeIn(QDataStream &st)
{
    st >> m_header;
    QByteArray buffer;
    buffer.resize(m_header.dataSize);
    st.readRawData(buffer.data(),m_header.dataSize);
    QDataStream io(buffer);
    io >> m_points >> m_winId >> m_figId >> m_itemId;
}

void SALocalServeVectorPointProtocol::serializeOut(QDataStream &st) const
{
    QByteArray buffer;
    QDataStream io(buffer);
    io << m_points << m_winId << m_figId << m_itemId;
    SALocalServeBaseHeader header;
    header.init();
    header.type = (int)TypeVectorPointFData;
    header.dataSize = buffer.size();
    st << header;
    st.writeRawData(buffer.data(),header.dataSize);
}


SALocalServeBaseProtocol::Type SALocalServeVectorPointProtocol::type() const
{
    return TypeVectorPointFData;
}

void SALocalServeVectorPointProtocol::setPoints(const QVector<QPointF> &points)
{
    m_points = points;
}

const QVector<QPointF> &SALocalServeVectorPointProtocol::getPoints() const
{
    return m_points;
}

QVector<QPointF> &SALocalServeVectorPointProtocol::getPoints()
{
    return m_points;
}

void SALocalServeVectorPointProtocol::setIDs(qintptr wndId, qintptr figId, qintptr itemId)
{
    m_winId = wndId;
    m_figId = figId;
    m_itemId = itemId;
}

void SALocalServeVectorPointProtocol::getIDs(qintptr &wndId, qintptr &figId, qintptr &itemId) const
{
    wndId = m_winId;
    figId = m_figId;
    itemId = m_itemId;
}

//===================
QDataStream &operator <<(QDataStream &st, const SALocalServeShakeHandProtocol &d)
{
    d.serializeOut(st);
    return st;
}

QDataStream &operator >>(QDataStream &st, SALocalServeShakeHandProtocol &d)
{
    d.serializeIn(st);
    return st;
}


QDataStream &operator <<(QDataStream &st, const SALocalServeStringProtocol &d)
{
    d.serializeOut(st);
    return st;
}

QDataStream &operator >>(QDataStream &st, SALocalServeStringProtocol &d)
{
    d.serializeIn(st);
    return st;
}


QDataStream &operator <<(QDataStream &st, const SALocalServeVectorPointProtocol &d)
{
    d.serializeOut(st);
    return st;
}

QDataStream &operator >>(QDataStream &st, SALocalServeVectorPointProtocol &d)
{
    d.serializeIn(st);
    return st;
}
