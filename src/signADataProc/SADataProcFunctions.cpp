#include "SADataProcFunctions.h"
#include "SAMath.h"
#include "SAServerDefine.h"
#include "SACRC.h"
#include "SATcpDataProcessClient.h"
#include "SAServerDefine.h"
// 简化的XMLDataPtr的创建
SASession::XMLDataPtr createXMLDataPtr(int funid);
SASession::XMLDataPtr createXMLDataPtr(int funid,SASession::XMLDataPtr reqxml);

SASession::XMLDataPtr createXMLDataPtr(int funid)
{
    SASession::XMLDataPtr res = SASession::makeXMLDataPtr();
    res->setClassID(SA::ProtocolTypeXml);
    res->setFunctionID(funid);
    return res;
}
SASession::XMLDataPtr createXMLDataPtr(int funid,SASession::XMLDataPtr reqxml)
{
    SASession::XMLDataPtr res = SASession::makeXMLDataPtr();
    res->setClassID(SA::ProtocolTypeXml);
    res->setFunctionID(funid);
    QVariant key = reqxml->getValue("key");
    res->setValue("key",key);
    return res;
}

/**
 * @brief 根据请求头创建回复头
 * @param requestHeader
 * @param data
 * @param Funid
 * @return
 */
SAProtocolHeader createXMLReplyHeader(const SAProtocolHeader &requestHeader,const QByteArray& data,int Funid)
{
    SAProtocolHeader rep = requestHeader;
    rep.dataSize = data.size();
    rep.dataCrc32 = SACRC::crc32(data);
    rep.protocolFunID = Funid;
    rep.protocolTypeID = SA::ProtocolTypeXml;
    rep.magic_start = SA_PROTOCOL_HEADER_MAGIC_START;
    rep.magic_end = SA_PROTOCOL_HEADER_MAGIC_END;
    return rep;
}
