 #ifndef _MEMORYMANAGER_H_
#define _MEMORYMANAGER_H_

#include <QObject>
#include <QSet>
#include <QMap>
#include <QMutex>
#include <xiCoreGlobal.h>
#include <PicBuf.h>
#include <ValueTypes.h>
#include <QWaitCondition>

class CxCameraParam;
class CxValue;

/*!
 \brief The singleton class for management of memory pools.
*/
class XICORE_API CxMemoryManager : public QObject
{
    Q_OBJECT
private:
    //! Private constructor.
    CxMemoryManager();

public:
    //! Gets the singleton instance of the class.
    static CxMemoryManager* instance();

    //! Destructor.
    ~CxMemoryManager();


    /*!
     \brief Creates new empty memory pool.
     \sa deletePool()
     \sa initPool()
     \sa freePool()
    */
    TxMemPoolHandle createPool();

    /*!
     \brief Initialization of the existing empty pool.

     \remarks The parameter \p picInfo must have set these members correctly:
     \li SxPicBufInfo::m_uiWidth
     \li SxPicBufInfo::m_uiHeight
     \li SxPicBufInfo::m_uiStride
     \li SxPicBufInfo::m_uiChannels.
    */
    bool initPool(TxMemPoolHandle hPool, const SxPicBufInfo &picInfo, int iCount, int &rAllocatedCount);

    /*!
     \brief Releases the pool's internal buffers. It means that the memory
     pool itself will not be deleted. After calling this method the memory
     poll will be empty and it won't be possible to allocate any buffer from it.
    The method succeedes if the pool has all buffers free.

     \remarks The method does not delete the pool itself. The handle stays valid. It should be called
     before inicialization of the already initialized pool.
     \sa deletePool()
     \sa initPool()
    */
    void freePool(TxMemPoolHandle hPool);


    /*!
     \brief Tries to delete the memory pool.

     \remarks The method internally calls \ref freePool() and if it succeeds then the pool is deleted and the
     handle \p hPool became invalid.
    */
    bool deletePool(TxMemPoolHandle hPool);

    /*!
     \brief Gets total number of buffers kept by this pool.
     \return If the method fails then it return -1.
    */
    int buffersCount(TxMemPoolHandle hPool);


    /*!
     \brief Gets free buffers kept by this pool.
     \return If the method faise then it returns -1.
    */
    int freeBuffersCount(TxMemPoolHandle hPool);


    /*!
     \brief Gets to total allocated size of the pool..
    */
    bool poolSize(TxMemPoolHandle hPool, size_t &uiSize);


    /*!
     \brief Gets buffer info for which the pool was created.
     \sa createPool()
    */
    bool bufferInfo(TxMemPoolHandle hPool, SxPicBufInfo &picInfo);


    //! Objects (chainables) can reserve some buffer places for themselves, so that cloning imageData would not fail.
    void reserveBuffersInPool(TxMemPoolHandle hPool, QObject *pClient, int iRequestCount);

    //! Clear all reservations made in pool.
    void invalidateBufferReservations(TxMemPoolHandle hPool);

    /*!
     \brief Allocates a buffer from given pool.
     \remarks The \p picBuf parameter must fit the \p picInfo parameter passed
     to \ref createPool(). The method actualizes these members of the \p picBuf:
     \li SxPicBuf::m_pData
     \li SxPicBuf::m_uiAllocSize
     \li SxPicBuf::m_bDataOwner
    */
    bool allocPicBuf(TxMemPoolHandle hPool, QObject *pClient, SxPicBuf &picBuf, bool bReset);


    /*!
     \brief Tries to releases the image buffer.
     \remarks The function releases the buffer only when the SxPicBuf::m_hDataOwner contains
     a valid handle to memory pool.
    */
    bool freePicBuf(SxPicBuf &picBuf);


    /*!
     \brief The method releases all buffers in the memory pool but will not
     dealocate the memory pool itself. If the pool
     is non empty then it emits the \ref freePoolBuffers() signal.
    */
    void freeAllBuffers(TxMemPoolHandle hPool);


    /*!
     \brief Checks whether the given image info is compatible with the memory pool.
     It means whether it is possible to allocate the a buffer with given image info
     from the memory pool.
    */
    bool isImageCompatibleWithPool(TxMemPoolHandle hMemPool, const SxPicBufInfo &picInfo);

    //memory threshold checking
public:
    enum ExAppOperation
    {
        eaopStartProgram,
        eaopStartAcquisition,
        eaopStartVideoRecord
    };
    Q_ENUM(ExAppOperation)

    //! Loads the limits from XML file
    bool loadLimits();

    /*!
     * \brief Check a resources for given operation and camera.
     * \remarks If it is neccessary to check an parameter and its value that is not set on the given camera yer, pass it via \p pParamName and \p pParValue.
     * It is suitable for checking resources before parameter change.
     */
    bool checkLimits(ExAppOperation eOperation, XICORE_HANDLE hCam, QString &sMessage, const QString *pParamName = NULL, const CxValue *pParValue = NULL);

private:
    struct SxLimitDef
    {
        QString m_sOS;
        QString m_sBits;
        QMap<QString, QString> m_mapParams;
        quint64 m_uiLimitBytes;

        bool operator<(const SxLimitDef &other) const;
    };

    QMap<ExAppOperation, QList<SxLimitDef> > m_mapLimitDefs;

public slots:

    /*!
     \brief Destroys all memory pools.

     \remarks The method forces the memory pools destruction without checking if the pools contain
     only free (nonoccupied) buffers so that the method should be used very carefully.
    It should be called when the application is closing.
    */
    void finishAll();

private slots:
    //! Reaction on signal CxMemoryPool::freeBuffersRequest()
    void onFreePoolBuffers(TxMemPoolHandle hMemPool);

signals:

    /*!
     \brief The signal is emitted when the memory pool is freed and it still contains
     some allocated (non-released) buffers. The receiver of this signal MUST immediately
     releaese these buffers by calling \ref freePicBuf(). It is very important to
     connect this signal with direct connection (see QObject::connect() and Qt::DirectConnection).
    */
    void freePoolBuffers(TxMemPoolHandle hMemPool);

public:
    //! Returns the amount of available RAM in bytes.
    static quint64 availableMemory();

    //! Returns the size of L2 cache in bytes
    static quint32 L2CacheSize();

private:    
    QSet<TxMemPoolHandle> m_setMemPools;    /*!< Map of memory pools and its threads. */
    QMutex m_lock;                          /*!< Lock of this object. */

};

#endif //_MEMORYMANAGER_H_
