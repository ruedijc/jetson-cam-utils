#ifndef RGB32CONVERTOR_H
#define RGB32CONVERTOR_H

#include <Chainable.h>
#include <ImageData.h>

class CxLut;

/*!
 \brief The class converts all incoming image data to RGB32 format."
*/
class XICORE_API CxRgb32Convertor : public CxImageProvider
{
    Q_OBJECT
public:
    Q_INVOKABLE CxRgb32Convertor();
    virtual ~CxRgb32Convertor();

    //CxChainable
public:
    virtual QString title() const;    
    virtual bool acceptsDataFrom(CxChainable *pPrecedessor);
    virtual int buffersCountInMemoryPool() const;
    virtual CxChainable* clone();

protected:    
    virtual IxChainData* processData(IxChainData *pReceivedData);
    virtual void setupStop();

    //CxImageProvider
public:
    virtual bool queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput,
                                      const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput = NULL);

    //own members
public:
    enum ExConversionOutputFormat
    {
        ecofRGB32,
        ecofRGB24,
        ecofRGB565,
    };
    void setConversionOutputFormat(ExConversionOutputFormat eFmt);
    ExConversionOutputFormat conversionOutputFormat();

    void setLut(CxLut *pLut);   //!< set conversion LUTs. Objects takes ownership of the pointer.
    CxLut* lut();

protected:
    ExConversionOutputFormat m_eConvOutputFormat;
    CxLut *m_pLut;
    QMutex m_lockLuts;                      /*!< Lock when working with LUTs. */

    //bool createConvertedPicBuf(const SxPicBuf *pSrcPic, SxPicBuf *pDstPic,
    //                           const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput);

    bool createConvertedPicBuf(const CxImageData *pSrcData, SxPicBuf *pDstPic,
                               CxImageMetadata *pMetadataOutput);
    SxPicBuf m_picFFTtmp;//!<Preallocated temporary buffer for FFT conversion to RGB
};


#endif // RGB32CONVERTOR_H
