#include <xiCoreGlobal.h>
#include <Chainable.h>

#ifndef DEMOSAIC_H
#define DEMOSAIC_H


/*!
 * \brief The class performs a debayering on images in RAW format.
 */
class XICORE_API CxDemosaic : public CxImageProvider
{
    Q_OBJECT
    Q_CLASSINFO("CustomName", tr("Offline Processing"))
    Q_CLASSINFO("UniqueID", "com.ximea.CamTool.CxDemosaic")
public:
    Q_INVOKABLE CxDemosaic();
    CxDemosaic(int iPredefOutputXiFormat);

    //CxChainable
public:
    virtual QString title() const;
    virtual bool acceptsDataFrom(CxChainable *pPrecedessor);
    virtual int buffersCountInMemoryPool() const;
    virtual CxChainable* clone();
    virtual bool hasHelp() const;
    virtual QString help(QString &sImagesPath);
protected:
    virtual void setupStart();
    virtual void setupStop();
    virtual IxChainData* processData(IxChainData *pReceivedData);

    //CxImageProvider
public:
    virtual bool queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput,
                                      const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput = NULL);
    //Own members
private:
    enum ExOfflineProcessing
    {
        eopNone,
        eopUnpacking,
        eopDebayering
    };

    int m_iPredefOutputXiFormat;
    void *m_pProc;//offline processing handle
    ExOfflineProcessing m_eProcType;
};


#endif // DEMOSAIC_H
