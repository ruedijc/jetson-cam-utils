#ifndef VIDEOSOURCE_H
#define VIDEOSOURCE_H

#include <xiCoreGlobal.h>

#include "Chainable.h"
#include "Data.h"
#include <ImageMetadata.h>
#include <QMutex>

class IxImageDataLoader;

/*!
 * \brief Provides image sequences.
 */
class XICORE_API CxVideoSource : public CxImageSource
{
    Q_OBJECT

public:
    CxVideoSource();
    virtual ~CxVideoSource();

    //CxChainable
public:
    virtual QString title() const;    
    virtual int buffersCountInMemoryPool() const;
    virtual CxChainable* clone();
    virtual void run();
    virtual bool saveSettings(QDomDocument &aDoc, QDomElement &aElm) const;
    virtual bool loadSettings(const QDomElement &aDom);

    //CxImageProvider
public:
    virtual bool currentOutputImageInfo(SxPicBufInfo &picInfo, CxImageMetadata *pMetadata = NULL);

    // additional functions for video
public:

    virtual int framesCount();
    virtual void requestFrame(int iFrameNo);
    virtual double frameRate();     //!< Fps, it defines the duration of the video.

    virtual void setDataLoader(IxImageDataLoader *pLoader);

protected:
    IxImageDataLoader *m_pLoader;   //!< Our source of the images
    SxPicBufInfo m_aImgFormat;
    CxImageMetadata m_aImgMetadata;
    qint32 m_iFrameCount;
    double m_dFps;
};

#endif // VIDEOSOURCE_H
