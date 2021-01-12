#ifndef ICHAINABLE_H
#define ICHAINABLE_H

//for doxygen to be able to generate a macros documenation
/*! \file */

#include <QRunnable>
#include <QObject>
#include <QVector>
#include <QMap>
#include <QSet>
#include <QSemaphore>
#include <QMutex>
#include <QQueue>
#include <QDomDocument>
#undef min
#include <QTime>


#include "xiCoreLog.h"
#include "Chain.h"

//!< Internally used constant.
#define CHAINABLE_MAX_LAST_TIMES 120

class IxChainData;
class CxImageMetadata;

typedef QList<int> TxIntList;

/*!
 \brief A base class of all chainable objects.

 \remarks The \ref CxChainable class together with the \ref CxChain is the key stone of the
 \XICORE architecture. It is recommended to read the \ref CxChain documentation
 before you start to study this class.

 \section chainable_impl Implementation of chainable object
 The CxChainable class is an abstract class that implements:
 \li starting and stopping the object (\ref start(), \ref stop());
 \li receiving a data from its precedessors in the chain (\ref onDataAvailable(), \ref data(), \ref discardData());
 \li notifying its successors when it has a processed data ready (\ref dataAvailable()).

 This class generally can process any kind of data (\ref IxChainData). When implementing a custom CxChainable class
then in most cases the \ref CxImageProvider is the class you should inherit from. Currently the \ref CxImageProvider
is the only class that can be registered by the application and thus that can be added to a processing chain by the
chain editor (see \ref plugins).

  \par Memory management
  Any chainable object can use its own memory pool (\ref CxMemoryManager). This is specified by the
  CxChainable::eHasOwnMemoryPool flag passed to the constructor. The memory pool should be initialized
  in the pure virtual method \ref init(). This method is implemented by the \ref CxImageProvider so that
  when you inherit from it you do not need to implement this method as well as the \ref initialized()
  method.\n
  Each chainable object in the chain has its \ref onFreePoolBuffers() slot connected to the
  CxMemoryManager::freePoolBuffers(). When the signal is emitted then the chainable object \b allways \b must
  immediatelly release all buffers allocated from the given memory pool otherwise the application
  crashes on assertation!

  \par Data ownership
  There are simple rules describing the data ownership:
  1. If the object created (allocated) a data (\ref IxChainData) then it is the owner of the data until some of its successors
  take the data by calling \ref data() method.
  2. If the object takes the data by calling \ref data() method on its precedessor then it become the owner of the data
  until some of its successors take the data by calling \ref data() method.

  When the object is the owner of the data then there are two scenarios how the data can be managed:
  1. The chainable object processes the data directly. It means that the format and the size of the data is unchanged
 and the data are processed in-place. After the processing the data are passed to the object's successors.
In this case the chainable object cannot delete the data until the successors pick up the data by calling the
  \ref data() method.
  2. The chainable object changes the data size, format etc. so that it needs to create (alloc) new data
  which are passed to the object's successors. In this case the object must delete the original (received)
  data after the processing finished.

  Sometimes it can happen that the chainable object cannot accept a data from its precedessor. It means that its
  precedessor notified the object by \ref dataAvailable() signal but the object cannot process them i.e. it cannot call
  the \ref data() method. In this case the object should call the \ref discardData() on the precedessor instead of the \ref data().

*/
class XICORE_API CxChainable : public QObject
{
    Q_OBJECT

    friend class CxChain;
    friend class CxChainManager;
public:
    //! Value representing a state of the CxChainable object.
    enum ExChainableState
    {
        eUndefined = 0,
        eStopped,
        eRunning
    };

    //! Values representing an data source type.
    enum ExDataSourceType
    {
        //! The object is not a data source.
        eSourceTypeNone = 0,
        //! Data come from camera (\ref CxCameraSource).
        eSourceTypeCamera,
        //! Data come from static image (\ref CxStaticImageSource).
        eSourceTypeStaticImage,
        //! Data come from video (\ref CxVideoSource).
        eSourceTypeVideo
    };

    //! Flags of the chainable object.
    enum ExChainableFlag
    {        
       eHasOwnMemoryPool =      0x1,    /*!< Object uses own memory pool. */
       eRunIsInfiniteLoop =     0x2,    /*!< The \ref run() method is implemented as an infinite loop. */       
       eSupportsFpsRegulation = 0x4,    /*!< The object allows to regulate a framerate by discarding input data. */
       eCanChangeEnableState  = 0x8,    /*!< The object can be disabled so that it will not process a data and the data flow is bypassed */
       eTakeLatestImage       = 0x10    /*!< The object always gets the latest image from its precedessor's output queue.*/
    };

    Q_DECLARE_FLAGS(ExChainableFlags, ExChainableFlag)

public:    
    CxChainable(ExChainableFlags eFlags);
    virtual ~CxChainable();

    //--------------------
    //pure virtual methods
    //--------------------

public:

    /*!
     \brief Gets a title of this chainable object. The title is usually used
     to display a window title in the GUI etc. It should be a localized string.
    */
    virtual QString title() const = 0;

    //! Query if this object can accept data from the given chainable object.
    virtual bool acceptsDataFrom(CxChainable *pPrecedessor) = 0;


    /*!
     \brief Gets a number of buffers which \b this object needs to have reserved in memory pool.
     If this object allocates no buffers then the method should return 0. See the \ref CxChainable
    */
    virtual int buffersCountInMemoryPool() const = 0;

    //! Creates a deep copy of this object.
    virtual CxChainable* clone() = 0;

protected:
    /*!
     \brief Initializes the object. When calling this method, the object MUST be stopped.

     \remarks Methods \ref init() and \ref initialized() are internally used by the \ref start()
     method. It should initialize the chainable object so that it is ready to start (\ref start()) .
     If this object is the \ref CxImageProvider then the method initializes internal
     memory pool.
    */
    virtual bool init(QString *pErrMsg = NULL) = 0;
    virtual bool initialized() = 0;

    //--------------------
    // virtual methods
    //--------------------

public:

    /*!
     \brief Returns true if the object is data consumer, i.e.
     the object has no successors and all the received data are "consumed" here e.g. they are displayed
    in image view, saved to disk etc. In other words this object is the a leaf in the chain.
    */
    virtual bool isDataConsumer() const;

    virtual ExChainableState state();

    //! Gets the pointer to data processed by this object. \sa discardData() \sa CxChainable
    virtual IxChainData* data(QObject *pReceiver, qint32 iFrameNo);

    //! This method discards data for the given receiver.\sa data() \sa CxChainable
    virtual void discardData(QObject *pReceiver, qint32 iFrameNo);


    //! Gets the latest data from the output queue. It is called automatically if ExChainableFlag::eTakeLatestImage is set.
    virtual IxChainData* latestData(QObject *pReceiver);

    //! Discards the latest data for the given receiver. It is called automatically if ExChainableFlag::eTakeLatestImage is set.
    virtual void discardLatestData(QObject *pReceiver);

    /*!
     * \brief Displays the object's configuration dialog.
     * \remarks The configuration widget should not contain the Ok and Cancel button as they will be
     * added automatically by the application.
     * \warning When this chainable object changes the image format, don't forget to stop the current chain before
     * applying new value from the dialog!
     * \warning This method MUST be called from GUI (main) thread only!!
     * \return The default value is NULL. When overriden then it should return the pointer to the configuration widget.
     */
    virtual QWidget* configurationWidget();

    //! Returns true when the the object has configuration widget (ie. the \ref configurationWidget() returns not NULL).
    virtual bool hasConfigurationWidget() const;

    class XICORE_API CxSimpleStateConfigButton
    {
    public:
        QString m_sCaption;
        int m_iBtnData;
        bool m_bChecked;
        CxSimpleStateConfigButton(const QString &sCaption, int iBtnData, bool bChecked = false);
    };
    typedef QList<CxSimpleStateConfigButton> TxSimpleStateConfigButtonList;

    /*! Fills state names to be shown on small push-like checkboxes on the box in the Chain Editor.
     *  Set mutuallyExclusive to true in case buttons should serve as radio button group.
     */
    virtual void simpleStateConfigButtons(TxSimpleStateConfigButtonList &lstButtons, bool &bMutuallyExclusive);

    //! Tells the object that one of simple state buttons has been pressed.
    virtual void simpleStateConfigButtonPressed(int iBtnData);

    /*!
     * \brief The method saves the settings of this object to the DOM element.
     * \remarks The element \p aElm is already named by a caller, do not change the name (ie. do not call the aElm.setTagName()).
     * You can add attributes and child elements.
     */
    virtual bool saveSettings(QDomDocument &aDoc, QDomElement &aElm) const;

    //! Loads settings from DOM.
    virtual bool loadSettings(const QDomElement &aDom);

    //! Returns true if the object has help. The default is false.
    virtual bool hasHelp() const;

    /*!
     * \brief Returns a string with help.
     * \remarks The text can be plain text or HTML. If the HTML help contains images then \p sImagesPath
     * should contain an path to the images. The images path must be either absolute or relative to the
     * application path (qApp->applicationDirPath()) and it also can be a prefix to embedded resource
     * images (for example ":/images").
     */
    virtual QString help(QString &sImagesPath);

    //! Sets max. FPS (when zero or negative, then the FPS will not be regulated)
    virtual void setMaxFps(double dFps);

    //! Returns max. FPS (zero means that the max FPS is not set).
    virtual double maxFps() const;

protected:

    /*!
     \brief Sets up the object before the object is started.

     \remarks The method is internally used by the \ref start() method immediately before
     the \ref run() method is invoked.  For example, the \ref CxCameraSource sets up internal frame
    counter to zero and starts camera acquisition. The default implementation does nothing.
    */
    virtual void setupStart();


    /*!
     \brief Sets up the object after the object is stopped.

     \remarks The method is internally used by the \ref stop() method immediatelly after
     the \ref run() method is finished. For example, the \ref CxCameraSource stops camera acquisition
     here. The default implementation does nothing.
    */
    virtual void setupStop();

    /*!
     \brief Returns true if this object requires memory pool.
     The default implementation just tests the \ref m_eFlags, but for example the camera source
    should take into account the actual buffer policy of the camera.
    */
    bool isMemoryPoolNeeded();

    /*!
     \brief Recommended additional number of buffers that should be present in memory pool, but not
     reserved for this chainable only. Used in CameraSource to add more then minimum number
     of frames to pool.
    */
    virtual int recommendedAdditionalBuffersInMemoryPool() const;

    /*!
     \brief The method processes the received data. It is called automatically
     in the default implementation of the \ref onDataAvailable() slot.

     \remarks If the implementation creates (allocates) a new data here
    then the original (received) data should be deleted using the delete operator.
    The default implementation of this method returns a pointer to the received
    data without any processing.
    \warning If the method returns NULL, then the \p pReceivedData will be deleted automatically!
    */
    virtual IxChainData* processData(IxChainData *pReceivedData);

    /*!
     \brief Adds new successor to this object and connects the \ref dataAvailable()
     signal to the successor's \ref onDataAvailable() slot. It is internally used by \ref CxChain class.
    */
    virtual bool addSuccessor(CxChainable *pSuccessor);

    //! Adds new precedessor to this object. It is internally used by \ref CxChain class.
    virtual bool addPrecedessor(CxChainable *pPrecedessor);

    /*!
     \brief Removes the successor from this object and discards the connection of the
     \ref dataAvailable() signal from the successor's \ref onDataAvailable() slot. It is internally used by \ref CxChain class.
    */
    virtual bool removeSuccessor(CxChainable *pSuccessor);

    //! Removes the precedessor from this object. It is internally used by \ref CxChain class.
    virtual bool removePrecedessor(CxChainable *pPrecedessor);

    //! Removes and disconnect all the successors of this object. It internally calls the \ref removeSuccessor().
    virtual bool clearSuccesors();

    //! Removes all the precedessors of this object. It internally calls the \ref removePrecedessor().
    virtual bool clearPrecedessors();

    //! Adds the given data to output data and emits the \ref dataAvailable() signal if necessary.
    virtual bool addToOutputData(IxChainData *pData);

    //! Deletes the output data. The method is not thread safe, it is neccassary to lock the m_lockOutputData!
    void deleteOldestOutputDataNoLock();

protected slots:
    /*!
     \brief Releases all buffers allocated from the given memory pool.

     \remarks This slot is always connected to CxMemoryManager::freePoolBuffers() signal.
     The chainable object \b MUST release all buffers allocated from the given memory pool.
     If the buffers will not be released then the application's behaviour is undefined
     (the app. most probably crashes or freezes). The default implementation of the method checks the internal output data
     (CxChainable::m_mapOutputData) and releases all the data from the given memory pool.
    */
    virtual void onFreePoolBuffers(TxMemPoolHandle hMemoryPool);

    /*!
     \brief Reaction on the \ref dataAvailable() emitted by precedessors of this object. The slot is
     connected automatically by the \ref addSuccessor().
    */
    virtual void onDataAvailable(CxChainable *pSender, int iFrameNo);

    /*!
     \brief This method is invoked automatically by the \ref start() method. If the method is implemented
    as an infinite loop then it must use the \ref CHAINABLE_RUN_WHILE_TRUE and \ref CHAINABLE_RUN_END_WHILE
    macros to start and finish the infinite loop. For most of the chainables object it may not be implemented.
    It should be implemented for image sources (\ref CxImageSource).
    \sa CxChainable
    */
    virtual void run(){ }

    //! Handle the CxChain::chainStarted(). The default implementation does nothing.
    virtual void onChainStarted(CxChain *pChain){Q_UNUSED(pChain);}

    //! Handle the CxChain::chainStarted(). The default implementation does nothing.
    virtual void onChainStopped(CxChain *pChain){Q_UNUSED(pChain);}

    //--------------------
    // non-virtual methods
    //--------------------

public:

    //! Gets the set of precedessors of this objects in the chain.
    const QSet<CxChainable*>& precedessors();

    //! Gets the set of successors of this objects in the chain.
    const QSet<CxChainable*>& successors() const;

    //! Gets a chain in which this object is contained.
    CxChain* chain() const;

    //! Whether this object supports FPS regulation.
    bool supportsFpsRegulation();

    //! Returns a set of all non-CxChainable objects connected to this object.
    QSet<QObject*> nonChainableListeners();


    /*!
     \brief Returns a handle to the object's memory pool.
     \remarks If the object has no internal memory pool then the method returns NULL.
    */
    TxMemPoolHandle memoryPool();

    //! Whether the object is enabled. \sa CxChainable::eCanChangeEnableState \sa setEnabled()
    bool enabled() const;

    //! Can be applied when CxChainable::eCanChangeEnableState flag is set. The chain of this object must be stopped.
    void setEnabled(bool bEnabled);

    ExChainableFlags flags() const;

    //! Calculates the current framerate of this object.
    double calcFps();

private:
    /*!
     \brief Sets the 'parent' chain to this object.

     \remarks The method succeedes when this object is not assigned to other chain.
     If the \p pChain is NULL, then the method succeedes when the current object's chain
    does not have this chainable object connected i.e. the CxChain::isConnectedInChain()
    method returns false.
    */
    bool setChain(CxChain *pChain, bool bSetSuccessors = false);

    bool addToOutputDataNoLock(IxChainData *pData);
    IxChainData* dataNoLock(QObject *pReceiver, qint32 iFrameNo);
    void discardDataNoLock(QObject *pReceiver, qint32 iFrameNo);

public:

    /*!
     \brief Gets the type of data sources of this chainable object.
     \remarks The current implementation of the \ref CxChain supports only one data source so that
     the method return a list containing at most one element.
     \sa dataSources()
    */
    QList<ExDataSourceType> chainDataSourceTypes() const;


    /*!
     \brief Gets list of data sources of this chainable object.
     \remarks The current implementation of the \ref CxChain supports only one data source so that
     the method return a list containing at most one element.
     \sa chainDataSourceTypes()

    */
    QList<CxChainable*> dataSources();


    //! Error message of this object. \sa setErrorMessage()
    QString errorMessage();



protected:

    //! Set the error message and emits \ref errorStateChanged() signal.
    void setErrorMessage(const QString &sErrMsg);

    /*!
     \brief Starts this object and sets the correct state of the object.
     \remarks When the object is started then it can process and accept data.
    */
    bool start(CxChain::ExChainErrorState *pErrState = NULL, QString *pErrMsg = NULL);

    /*!
     \brief Stops running and resets this object (release appropriate buffers etc.).
     The method stops receiving input data and processing incoming data.

     \remarks When the chainable object is stopped then it must
     be guaranted that:
     \li the object cannot emit \ref dataAvailable(),
     \li when the \ref data() is invoked, then the method must return NULL,
     \li when the \ref onDataAvailable() slot is invoked then it cannot call the \ref data() on
     the sender. I.e. when the object is stopped it cannot provide or receive any data.
     The default implementation of the \ref CxChainable satisfies the conditions above.
    */
    bool stop();


    /*!
     \brief The method waits until the \ref run() method is started.
     \remarks The method is internally used by the \ref start() method.
     \warning Do not use it in your code!
    */
    void waitForStarted();

    /*!
     \brief The method waits until the \ref run() method is finished.
     \remarks The method is internally used by the \ref stop() method.
     \warning Do not use it in your code!
    */
    void waitForStopped();

    //! The method frees all output data CxChainable::m_mapOutputData.
    void freeOutputData();    

    /*!
     \brief This method registers an arbitrary non-chainable receiver to receive
     a data provided by this object.

     \remarks The method creates connection between its \ref dataAvailable() signal
     and the receiver's slot given by \p method parameter.
     \warning Allways use the SLOT macro to specify the method name given by \p method parameter.
     \warning Each object which wants to receive the data must be connected using
     this method otherwise the behaviour is undefined.

     \sa disconnect(const QObject *pReceiver, const char *method)
    */
    void connect(QObject *pReceiver, const char *method);


    /*!
     \brief Discards the connection between the \p pReceiver and this object created
     by \ref connect(const QObject *pReceiver, const char *method) method.

     \warning Allways use the SLOT macro to specify the method name given by the \p method parameter.
     \sa connect(const QObject *pReceiver, const char *method)
    */
    void disconnect(QObject *pReceiver, const char *method);


    /*!
     \brief Returns the number of buffers required by \b this object
 to have in memory pool.
    \remarks The method calls internally the \ref buffersCountInMemoryPool() and
 add a number of listeners of this object.

    */
    int objectBuffersInMemoryPool();

private:
    //! Gets the internal thread of this object.
    QThread *myThread() const;

signals:

    //! The signal is emitted when this objects has data available for next chainable objects. \sa CxChainable
    void dataAvailable(CxChainable *pSender, int iFrameNo);


    /*!
     \brief The signal is emitted when some error appears or when an error is cleared. It is emitted
     by method \ref setErrorMessage();
    */
    void errorStateChanged(CxChainable *pSender, bool bError);

    //! The signal is emitted from the destructor of this object.
    void aboutToDelete(CxChainable *pSender);

    /*!
     * \brief The signal is internally connected to the \ref run() slot.
     * \remarks It is internally emitted in the \ref start() method.
     * \warning Do not emit or receive the signal!
     */
    void startRun();

protected:
    QSet<CxChainable*> m_setPrecedessors; /*!< Set of precedessors of this object in the chain. */
    QSet<CxChainable*> m_setSuccessors;   /*!< Set of successors of this object in the chain. */    
    int m_iMaxOutputDataCount;               /*!< Max number of output data per successor. */
    ExChainableFlags m_eFlags;               /*!< Flags of this object. */
    double m_dMaxFps;                        /*!< Max. FPS processed by this object. (-1 for unlimited) */
    QQueue<int> m_queFpsCounter;              /*!< Queue of times when data was received - for FPS calculation. */
    QTime m_timerFps;                         /*!< Timer for FPS measurement */
    ExChainableState m_eState;               /*!< State of this object. */
    ExChainableState m_eReqState;            /*!< Required state of this object. It is used when run() is an infinite loop. Do not touch! */
    QMutex m_lock;                           /*!< Lock of this object. */
    QMutex m_lockOutputData;                 /*!< Lock of the output data. */
    QMutex m_lockErrorMessage;               /*!< Lock of the error message. */
    QMap<int, IxChainData*> m_mapOutputData; /*!< The output data, key ~ the frame number, value ~ the processed data. */
    QMap<QObject*, QSet<int> > m_mapObjectOutputData; /*!< The map of the output frame numbers. Key ~ the receiver, value ~ set of frame numbers */
    XICORE_HANDLE m_hMemoryPool;             /*!< Handle to memory pool. */
    QSemaphore m_semStartStop;               /*!< Semaphore signalizing that the run() started or stopped. \warning Do not touch! */
    bool m_bDataConsumer;                    /*!< Whether the object is data consumer. */    
#ifdef XICORE_ENABLE_PROFILE_LOG
    QTime m_timerDebug;                      /*!< A timer for debugging purpose (profiling) */
#endif //XICORE_ENABLE_PROFILE_LOG
private:
    CxChain *m_pMyChain;                     /*!< Chain which this object belongs to. */
    QThread *m_pThread;                      /*!< The thread in which this object lives (if NULL, then the object lives in the GUI thread) */
    QString m_sErrorMessage;                 /*!< The error message of this object. */    
    bool m_bLoosingFrames;                   /*!< It is internally used to signalizing the error state. \warning Do not touch! */
    int m_iFirstLostFrame;                   /*!< It is internally used to signalizing the error state. \warning Do not touch! */
    bool m_bEnabled;                         /*!< Enable state is valid when CxChainable::eCanChangeEnableState flag is set */
};

/*! \brief The macro must be used to start a loop in the CxChainable::run() in a case that the method
 * is implemented as an infinite loop (CxChainable::eRunIsInfiniteLoop). \sa CHAINABLE_RUN_END_WHILE
  */
#define CHAINABLE_RUN_WHILE_TRUE \
    Q_ASSERT(m_eFlags.testFlag(eRunIsInfiniteLoop));\
    LOG_TRACE(ERR_MSG(CxChainable::run(), "Releasing the m_semStartStop (started)."));\
    m_semStartStop.release();\
    bool __bStateChanged__ = false;\
    CxChainable::ExChainableState __eState__ = eUndefined;\
    while(true)\
    {\
        {\
           QMutexLocker lock(&m_lock);\
           if(m_eReqState == eStopped)\
           {\
               m_eState = eStopped;\
               __eState__ = m_eState;\
               __bStateChanged__ = true;\
               m_eReqState = eUndefined;\
               break;\
           }\
        }\
        if(chain()->stopped())\
        {\
            continue;\
        }

/*! \brief The macro must be used to finish a loop in the CxChainable::run() in a case that the method
 * is implemented as an infinite loop (CxChainable::eRunIsInfiniteLoop). \sa CHAINABLE_RUN_WHILE_TRUE
  */
#define CHAINABLE_RUN_END_WHILE \
    }\
    if(__bStateChanged__)\
    {\
       if(__eState__ == eStopped)\
       {\
          LOG_TRACE(ERR_MSG(CxChainable::run(), "Releasing the m_semStartStop (stopped)."));\
          m_semStartStop.release();\
       }\
    }



/*!
 \brief The base class for any chainable object which provides images.
*/
class XICORE_API CxImageProvider : public CxChainable
{
    Q_OBJECT
public:
    CxImageProvider(ExChainableFlags eFlags);
    virtual ~CxImageProvider();

    //------------
    //CxChainable
    //------------
protected:
    virtual bool init(QString *pErrMsg = NULL);
    virtual bool initialized();

    //--------------------------
    // Own pure virtual methods
    //--------------------------
public:


    /*!
     \brief Query the output image info (format) for given input image format.
    */
    virtual bool queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput,
                                      const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput = NULL) = 0;

    //-------------------------
    // Own non-virtual methods
    //-------------------------
public:

    /*!
     \brief Gets the current output image info (format) provided by this object.
     \remarks The method should call \ref getCurrentOutputImageInfo() on its precedessors
     and calculate the output image info provided by this object.
    */
    virtual bool currentOutputImageInfo(SxPicBufInfo &picInfo, CxImageMetadata *pMetadata = NULL);


    /*!
     \brief Gets the current input image info (format) which this object receives on its input.
    */
    virtual bool currentInputImageInfo(SxPicBufInfo &picInfo, CxImageMetadata *pMetadata = NULL);

    /*!
     \brief Gets the required number of images reserved in the memory pool by its successors.
     \remarks If this object uses its own memory pool then the method allways returns zero!
    */
    virtual int queryImagesCountInMemoryPool(const SxPicBufInfo &picInfo, TxMemPoolHandle hPool);

private:
    /*!
     \brief Returns the total buffers count required by this object and its successors.
     If this object does not have its own memory pool then it returns zero.
     */
    int totalBuffersCountInMemoryPool(TxMemPoolHandle hPool);

};


/*!
 \brief The base class of any image provider which is an image source, i.e. it has
 no precedessors and it provides images.
*/
class XICORE_API CxImageSource : public CxImageProvider
{
    Q_OBJECT

public:
    CxImageSource(ExChainableFlags eFlags);

    virtual ~CxImageSource(){}

    //-------------
    // CxChainable
    //-------------
public:
    virtual bool acceptsDataFrom(CxChainable *pPrecedessor);    
    virtual bool addPrecedessor(CxChainable *pPrecedessor);
protected:
    //-----------------
    // CxImageProvider
    //-----------------
public:
    virtual bool queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput,
                                      const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput = NULL);

};

Q_DECLARE_METATYPE(CxChainable::ExChainableState)

#endif // ICHAINABLE_H
