#include "SADataProcFunctions.h"
#include "SAMath.h"
#include "SAServerDefine.h"
#include "SACRC.h"
#include "SATcpDataProcessSocket.h"
#include "SAServerDefine.h"


/**
 * @brief 根据请求头创建回复头
 * @param requestHeader
 * @param data
 * @param Funid
 * @return
 */
SAProtocolHeader createXMLReplyHeader(const SAProtocolHeader& requestHeader, const QByteArray& data, int Funid)
{
    SAProtocolHeader rep;

    rep.init();
    rep.dataSize = data.size();
    rep.dataCrc32 = SACRC::crc32(data);
    rep.protocolFunID = Funid;
    rep.protocolTypeID = SA::ProtocolTypeXml;
    rep.sequenceID = requestHeader.sequenceID;
    rep.extendValue = requestHeader.extendValue;
    return (rep);
}


/**
 * @brief 生成一个应答xml
 * @param funid
 * @param reqxml
 * @return
 */
SAXMLProtocolParser xmlProtocol(int funid, const SAXMLProtocolParser& reqxml)
{
    SAXMLProtocolParser res;

    res.setClassID(SA::ProtocolTypeXml);
    res.setFunctionID(funid);
    QVariant key = reqxml.getDefaultGroupValue("key");

    res.setValue("key", key);
    return (res);
}
