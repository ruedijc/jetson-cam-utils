#ifndef BAYERPSEUDOCOLORSCHNBL_H
#define BAYERPSEUDOCOLORSCHNBL_H


#include <Chainable.h>


/*!
 \brief The class converts any valid RAW image to RGB32 image, where the pixels are colored according
 to the Bayer's mask of the input image.
*/
class CxBayerPseudoColorsCnbl : public CxImageProvider
{
    Q_OBJECT
    Q_CLASSINFO("CustomName", tr("RAW Coloring"))
    Q_CLASSINFO("UniqueID", "com.ximea.CamTool.CxBayerPseudoColorsCnbl")
public:
    Q_INVOKABLE CxBayerPseudoColorsCnbl();
    //CxChainable
public:
    virtual QString title() const;
    virtual bool acceptsDataFrom(CxChainable *pPrecedessor);
    virtual int buffersCountInMemoryPool() const;
    virtual CxChainable* clone();
    virtual bool hasHelp() const;
    virtual QString help(QString &sImagesPath);
protected:
    virtual IxChainData* processData(IxChainData *pReceivedData);//performs the RAW coloring

    // CxImageProvider
public:
    virtual bool queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput,
                                      const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput = NULL);

    //own members
private:
    bool isSupportedRawFormat(const SxPicBufInfo &picInfoInput, const CxImageMetadata *pMetadataInput);
};


#endif // BAYERPSEUDOCOLORSCHNBL_H
