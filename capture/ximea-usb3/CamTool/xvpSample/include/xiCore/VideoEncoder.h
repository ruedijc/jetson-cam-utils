#ifndef VIDEOENCODER_H
#define VIDEOENCODER_H

#include <xiCoreGlobal.h>
#include <PicBuf.h>
#include "ImageFormat.h"

#define XI_VIDEO_KEYFRAME_FACTOR    10

/*!
 * \brief The base class of all video encoders.
 * \remarks The implementations of this class should be registered by CxRtti so that
 * the application could use them.
 */
class XICORE_API IxVideoEncoder
{
public:
    virtual ~IxVideoEncoder(){}
    virtual QString name() = 0;     //!< Localized name name visible in GUI.
    virtual TxFourCC fourCC() = 0;  //!< The FourCC identifier of the codec.

    /*!
     * \brief Encodes next image (rPic) in the stream, output is stored to pEncoded buffer with size ruiSize.
     *
     *  \param pbKeyFrame is optional. When not NULL and set to true on input, then the encoder should force this image to be a key frame.
     * On the output it gives the information whether image was encoded as the key frame.
     */
    virtual bool encode(const SxPicBuf &rPic, void* pEncoded, size_t &ruiSize, bool *pbKeyFrame = NULL) = 0;
    virtual size_t maxEncodedSize(const SxPicBufInfo &rPicInfo) = 0;
    virtual void queryOuputFormat(const SxPicBufInfo &rInput, SxPicBufInfo &rOutput) = 0; //!< For the given input format it sets the output format

    virtual void setEncoderOptions(const TxImageFormatOptions &aOpt);
public:
    virtual QWidget* widgetForSaveDlg(QWidget *pParent);
    virtual TxImageFormatOptions optionsFromWidgetInSaveDlg(QWidget *pWidget);
};

#endif // ENCODER_H
