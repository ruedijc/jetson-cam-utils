#ifndef STATICIMAGESOURCE_H
#define STATICIMAGESOURCE_H

#include <xiCoreGlobal.h>
#include <Chainable.h>

class CxImageData;

/*!
 * \brief The image source of static image.
 */
class XICORE_API CxStaticImageSource : public CxImageSource
{
    Q_OBJECT

public:
    CxStaticImageSource();

    //! Creates the instance from the existing file name.
    CxStaticImageSource(const QString &sPath);

    //! Creates the instance from picbuf.
    CxStaticImageSource(const SxPicBuf &picBuf, const QString &sName);

    //! Creates the instance from image data.
    CxStaticImageSource(CxImageData *pImageData);

    //! Destructor.
    ~CxStaticImageSource();

    //CxChainable
public:
    virtual QString title() const;    
    virtual int buffersCountInMemoryPool() const;
    virtual CxChainable* clone();
    virtual IxChainData* data(QObject *pReceiver, qint32 iFrameNo);
    virtual IxChainData* latestData(QObject *pReceiver);
    virtual bool saveSettings(QDomDocument &aDoc, QDomElement &aElm) const;
    virtual bool loadSettings(const QDomElement &aDom);

    //CxImageProvider
public:
    virtual bool currentOutputImageInfo(SxPicBufInfo &picInfo, CxImageMetadata *pMetadata = NULL);
    //QRunnable
public:
    void run();

    //own methods
public:

    //! Whether the static image is loaded correctly.
    bool isLoaded() const;

    //! Reloads the image.
    bool reload();

    //! Get the image full path.
    QString path();

    /*!
     \brief Sets the image's path. The method will not
    reload  the image when the path differs from the original path.

     \fn setPath(const QString &path)
    */
    void setPath(const QString &path);

protected:
    bool loadFromFile(const QString &sPath);

private:
    CxImageData *m_pData;  //!<Pointer to image data
    QString m_sPath;       //!<Path of file
};

#endif // STATICIMAGESOURCE_H
