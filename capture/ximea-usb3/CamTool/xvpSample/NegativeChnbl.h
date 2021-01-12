#ifndef NEGATIVECHNBL_H
#define NEGATIVECHNBL_H

#include <Chainable.h>

class CxNegativeChnbl : public CxImageProvider
{
    Q_OBJECT
    Q_CLASSINFO("CustomName", tr("Negative"))
    Q_CLASSINFO("UniqueID", "com.ximea.CamTool.CxNegativeChnbl")
public:
    Q_INVOKABLE CxNegativeChnbl();

    //CxChainable
public:
    virtual QString title() const;    
    virtual bool acceptsDataFrom(CxChainable *pPrecedessor);
    virtual int buffersCountInMemoryPool() const;
    virtual CxChainable* clone();
    virtual bool hasHelp() const;
    virtual QString help(QString &sImagesPath);
protected:
    virtual IxChainData* processData(IxChainData *pReceivedData);

    // CxImageProvider
public:
    virtual bool queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput,
                                      const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput = NULL);
};

#endif // NEGATIVECHNBL_H
