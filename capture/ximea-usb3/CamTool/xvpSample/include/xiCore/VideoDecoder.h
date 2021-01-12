#ifndef VIDEODECODER_H
#define VIDEODECODER_H

#include <xiCoreGlobal.h>
#include <PicBuf.h>
#include <VideoAccessor.h>

/*!
 * \brief The base class of all video decoders.
 * \remarks The implementations of this class should be registered by CxRtti so that
 * the application could use them.
 */
class XICORE_API IxVideoDecoder
{
public:
    virtual ~IxVideoDecoder(){}
    virtual QString name() = 0; //!< Localized name of the decoder visible in GUI.
    virtual TxFourCC fourCC() = 0; //! The FourCC identifier of the codec.
    /*!
     * \brief The methed decodes a video frame given by \p iFrameIndex.
     * \remarks If the <tt>rDecodedPicBuf.m_pData==NULL</tt>, the decoder will allocate it but the image
     * must be initialized correctly (width, height, channels, datatype...)
     */
    virtual bool decode(qint32 iFrameIndex, IxVideoAccessor *pVideo, SxPicBuf &rDecodedPic) = 0;
};


#endif // VIDEODECODER_H
