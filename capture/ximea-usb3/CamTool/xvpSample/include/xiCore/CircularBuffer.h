#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <xiCoreGlobal.h>

#include "Chainable.h"
#include "DataLoader.h"
#include <ICircularBuffer.h>

#include <QElapsedTimer>

/*! Circular buffer has two modes:
    1. as a CxChainable, serves as a storage for frames (primary task)
    2. as a IxImageDataLoader, serves as a in-memory storage for CxVideoSource
*/
class XICORE_API CxCircularBufferCnbl : public CxImageProvider, public IxImageDataLoader, public IxCircularBuffer
{
    Q_OBJECT
public:
    CxCircularBufferCnbl();
    virtual ~CxCircularBufferCnbl();

    //CxChainable
public:
    virtual QString title() const;
    virtual bool acceptsDataFrom(CxChainable *pPrecedessor);
    virtual int buffersCountInMemoryPool() const;
    virtual CxChainable* clone();
protected:
    virtual bool init(QString *pErrMsg = NULL);
    virtual bool initialized();
    virtual void setupStart();
    virtual void setupStop();

protected slots:
    virtual void onDataAvailable(CxChainable *pSender, int iFrameNo);
    virtual void onFreePoolBuffers(TxMemPoolHandle hMemoryPool);
    //CxImageProvider
public:
    virtual bool currentOutputImageInfo(SxPicBufInfo &picInfo, CxImageMetadata *pMetadata = NULL);
    virtual bool queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput,
                                      const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput = NULL);

public:
    virtual qint32 sequenceSize();
    virtual IxChainData *loadedData(qint32 iSeqIdx = 0);
    virtual bool load();
    virtual QString inputFile();
    virtual void setInputFile(const QString &sFileName);

    // IxImageDataLoader interface
public:
    virtual double loadedFps();
    virtual TxImageFormatList supportedImageFormats();

    // IxCircularBuffer
public:
    virtual ExCircularBufferType bufferType() const;
	virtual void setBufferType(ExCircularBufferType type);
    virtual void setCircleSize(qint32 iSize);
    virtual qint32 circleSize() const;
	virtual void resetInternals();
    virtual bool initializeBuffer(const SxPicBufInfo &aPicFormat);
    virtual bool isBufferInitialized() const;
    virtual qint32 framesCount() const;
    virtual qint32 lastFrame() const;
    virtual qint32 framesLost() const;
	virtual void framesLostIncrement(int cnt);
    virtual bool pushFrame(const CxImageData *pData);
    virtual CxImageData* frame(int iIndex, QObject *pMemPoolClient = NULL);
    virtual bool bufferPicFormat(SxPicBufInfo &aPicFormat) const;
    virtual quint64 timestamp(qint32 iIndex) const;
    virtual void finishBuffer();
    virtual quint64 bytesAvailable();
    virtual quint64 frameSize() const;
    virtual void finishWriting();
    virtual bool setParam(const QString &paramName, const QVariant &value);
    virtual QVariant param(const QString &paramName);
    virtual quint64 currentDataFlow();
    virtual quint64 averageDataFlow();
    virtual quint64 minDataFlow();

    //Own members
public:
    void setCircularBufferType(ExCircularBufferType eBufType);

signals:
    void frameReceived(qint32 iCirclePosition);

protected:
    double m_dFps;         //!< fps of the incoming images. Need to tell when providing the data further
    int m_iPrevFrameNo;    //!< For lost frames detection.
    int m_iFramesCountOK;  //!< For reseting the error state.
    IxCircularBuffer *m_pCircBuffer;
    int m_iFramesCaptured; //!< The total number of frames captured by the circ. buffer.
    qint64 m_iPushFrameMaxTimeNsec; //!< The max. time in nanoseconds needed by \p pushFrame() method.
    QElapsedTimer m_timer;
    qint64 m_iStartCaptureNsecs;
    qint64 m_iStopCaptureNsecs;
    quint64 m_uiFrameSize; //!< Cached frame size for internal use. Do not touch or use it!!
    QMutex m_lockWrite;
};

#endif // CIRCULARBUFFER_H
