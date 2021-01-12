#ifndef CAMERASOURCE_H
#define CAMERASOURCE_H

#include <QMutex>
#include <QWaitCondition>
#include <QSemaphore>
#include <Chainable.h>
#include <CameraManager.h>
#include <CameraParameters.h>
#include <QQueue>

#ifdef min
#undef min
#endif
#include <QTime>

class CxImageData;


/**
 * \brief The implementation of image source for Ximea cameras.
 *
 */
class XICORE_API CxCameraSource : public CxImageSource
{
    Q_OBJECT

private:
    //! The constructor is private. Call the CxChainManager::createCameraSource() for new instance of the class.
    CxCameraSource(XICORE_HANDLE hCamera);
public:
    virtual ~CxCameraSource();

friend class CxChainManager;
    //-------------
    // CxChainable
    //-------------
public:
    virtual QString title() const;    
    virtual int buffersCountInMemoryPool() const;
    virtual CxChainable* clone();
    virtual bool saveSettings(QDomDocument &aDoc, QDomElement &aElm) const;
    virtual bool loadSettings(const QDomElement &aDom);

protected:
    virtual int recommendedAdditionalBuffersInMemoryPool() const;
    virtual void setupStart();
    virtual void setupStop();

public:

    virtual void run();    
    //-----------------
    // CxImageProvider
    //-----------------
    virtual bool currentOutputImageInfo(SxPicBufInfo &picInfo, CxImageMetadata *pMetadata = NULL);

//-----------------
// Own methods
//-----------------

public:
    CxCameraParameters* parameters();
    XICORE_HANDLE cameraHandle() const;
    void invalidateCameraHandle();
    void reviveWithCameraHandle(XICORE_HANDLE hCamera);
    QString cameraSerialNo() const;
    double cameraSourceFrameRate(); //!< The current receiving frame rate of the camera source (it is not the same as camera frame rate!).
    QVariant customParam(const QString &sParamName);
    bool setCustomParam(const QString &sParamName, const QVariant &value);    
private:
    //! Gets the current buffer policy.
    int bufferPolicy();
    bool createFakeTransportPicture(SxPicBuf *pPic, const SxPicBufInfo *pPicFormat, void* pSrcXiImg);
    void checkCameraSkippingFrames(QTime &timer, int &iApiCounter, int &iTransportCounter);
    bool isUnpackingInMyChain() const;

private slots:
    //! Connected to camera manager
    void onImageFormatChanged(XICORE_HANDLE hCamera);
private:
    XICORE_HANDLE m_hCamera;                    /*!< Handle to camera. */
    QString m_sCameraSerialNo;                     /*!< Camera serial number used when camera disconnected (handle=NULL) and connected again. */
    qint32 m_iFrameNo;                          /*!< Internal frame counter. */
    QQueue<int> m_queLastFrameTimes;            /*!< Acquisition times of the last frames. */
    QTime m_timerLastFrames;                    /*!< Measuring of the frame's acquisition times. */
    SxPicBufInfo m_aCurrentOutputFormat;        /*!< Cached current output format. */
    bool m_bCurrentOutputFormatValid;            /*!< Whether the cached output format is valid. */
    bool m_bFakeTransportFormat;                /*!< Camera is in Transport data format, generate and send fake images */
    int m_iPayloadSize;                         //!< The payload size of the current image
    bool m_bBypassGetImage;                     //!< Bypass the xiGetImage() -> the picbuf will contain random data.
    bool m_bLoopRecording;                      //!< If loop recording is running, set different queue size in setupStart()
	CxImageMetadata	aMetadata;					//!? Image metadata after acq. start
	bool m_bIsSimulator;						//!? True for camera simuator instances.
};

#endif // CAMERASOURCE_H
