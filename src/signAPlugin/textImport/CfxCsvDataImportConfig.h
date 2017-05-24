#ifndef SACFXCSVDATAIMPORT_H
#define SACFXCSVDATAIMPORT_H
#include <memory>
#include <QVector>
#include <QObject>
class SAAbstractDatas;
class SACfxCsv2DDataParser;
class CfxCsvDataImportConfig: public QObject
{
    Q_OBJECT
public:
    CfxCsvDataImportConfig(QObject* par = nullptr);
    virtual ~CfxCsvDataImportConfig();
    //导入多个文件接口
    bool openFiles(const QList<QString>& path);
    //是否打开文件，执行done的前置操作
    virtual bool isOpenFile();
    //完成操作
    virtual QList<SAAbstractDatas*> createResultPtr();
    //解析
    bool parser();
    //获取parser
    SACfxCsv2DDataParser* getParser(int index) const;
    //获取文件数
    int getFileCount() const;
    //鉴定是否解析成功
    bool isSuccessParser(int index) const;
    //导出标志
    enum ExportWay{
        ToVectorDouble_1XnY///< 导出为线性数组，x只导出一组，y都导出，此对应x都是一样的情况下
        ,ToVectorDouble_nX1Y///< 导出为线性数组，y只导出一组，x都导出，此对应y都是一样的情况下
        ,ToVectorDouble_nXnY///< 导出线性数组，x，y都导出来
        ,toVectorPointF///< 导出为点序列
    };
    //设置导出方式
   void setExportWay(ExportWay way);
   //清空原来打开的
   void clear();
   //设置编码格式
   void setCodec(const QString& codex);
   QString getCodec() const;
private:
   void createBy1XnYWay(QList<SAAbstractDatas*>& res);
   void createBynX1YWay(QList<SAAbstractDatas*>& res);
   void createBynXnYWay(QList<SAAbstractDatas*>& res);
   void createByPointFWay(QList<SAAbstractDatas*>& res);
private:
    bool m_isOpenFile;
    typedef std::shared_ptr<SACfxCsv2DDataParser> PARSER;
    QVector<PARSER > m_pasers;
    QList<bool> m_successFile;///< 记录解析成功的标志
    ExportWay m_exportWay;
    QString m_codex;
};

#endif // SACFXCSVDATAIMPORT_H
