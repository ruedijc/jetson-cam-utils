#ifndef CHAIN_H
#define CHAIN_H


#include <QObject>
#include <QSet>
#include <QMutex>
#include <QDomDocument>

#include "xiCoreGlobal.h"
#include "PicBuf.h"

class CxChainable;
class CxImageSource;
class CxViewImageReceiver;
class CxMdiView;
class CxCameraSource;

/**
 * \brief This class implements a processing chain which is together with the \ref CxChainable class (chainable objects) the keystones
 * of the \XV.
 * \section chains Chains and chainable objects
 *
 * Processing chain (a pipeline) defines a data flow and processing of the data.
 * \note Currently the chains are implemented as a tree with single root (data source).
 *
 * In the \XV the chain consists of these chainables:
 * - Data source - currently implemented by \ref CxImageSource.
 * - Processing elements - mostly implemented by \ref CxImageProvider.
 * - Data consumers - a chainables where the chain ends i.e. consumers are leafs in the chain tree. The \XICORE implements
 * these consumers: \ref CxViewImageReceiver, \ref CxCircularBuffer, \ref CxVideoToFile.
 *
 * The data flow and interaction between chainables is shown on the image below.
 *\image html ChainSequenceDiagramExample.jpg "Interaction of chainables in the chain"
 *
 * The chain on the image consists of:
 * - \p CameraSource (\ref CxCameraSource) which is the implementation of the \ref CxImageSource.
 * - \p RAW coloring (\ref CxBayerPseudoColorsCnbl) - the implementation of the \ref CxImageProvider.
 * - \p VideoToFile (\ref CxVideoToFile) - the implementation of \ref CxChainable as a data consumer. The \p VideoToFile
 * has pointer to \p ImageSaver (\ref IxImageDataSaver).
 *
 * The data that flows through the chain must inherit the \ref IxChainData interface. Currently the \ref CxImageData and \ref CxViewImageData classes based
 * on that interface are implemented.
 *
 * Any chain can consist of an arbitrary number of branches. Each branch gets its own copy of the data coming from data source. The copy of
 * data is created automatically in the CxChainable::data() method.
 *
 * The CxChain class implements several methods for adding/removing the \ref CxChainable to/from the chain. These methods are private. Chains
 * are managed by \ref CxChainManager which is a friend class of the CxChain.
 *
 * \subsection chain_nonchainable Connecting non-chainble object to the chain
 * The current implementation of chains allows to connect an arbitrary object to any chainable elemement. This object then receives its own
 * copy of chain data. The arbitrary object can be connected
 * to the chainable when it satisfies these conditions:
 * - It must be a descendant of the QObject.
 * - It must implement a slot to be able to listen the CxChainable::dataAvailable() signal.
 * - It must implement a slot to be able to listen the CxMemoryManager::freePoolBuffers() signal.
 *
 * The object can be connected to the chainable in the chain by CxChainManager::connectNonChainable() method. When the object
 * is connected to the chain then it behaves similar to CxChainable. It means that it recevies the CxChainable::dataAvailable() signal
 * and can call CxChainable::data() or CxChainable::discardData() etc.
 *
 * The mechanism of connected non-chainable objects is used by views (\ref CxMdiView) in the GUI frontend. So that the
 * chain architecture allows to connect any number of views to chainables in the chain and it is guaranteed that each of the connected
 * objects (views) receives its own copy of the data.
 *
 */
class XICORE_API CxChain : public QObject
{
    Q_OBJECT
friend class CxChainManager;

public:
    enum ExChainState
    {
        eStateStopped,
        eStateStopping,
        eStateStarted,
        eStateStarting
    };
    Q_ENUM(ExChainState)

    enum ExChainErrorState
    {
        eErrStateNoErr,
        eErrStateWarning,
        eErrStateError
    };
    Q_ENUM(ExChainErrorState)

private:
    //! Constructor.
    CxChain();

public:
    //! Destructor.
    ~CxChain();


    /*!
     \brief Starts processing on the chain.

     \return The method return false if the chain is locked or already started.
    */
    bool start();

    /*!
     \brief Stops the chain. Each chainable stops its
     task immediately, all unprocessed data will be lost.  The method leaves its
     execution when all chainables are stopped (it's a blocking operation).
    */
    void stop();

    //! Closes (but not deletes) the chain. It deletes all chainables and views in the chain.
    bool close();

    //! Returns true if the chain is running.
    bool started();

    //! Returns true if the chain is stopped.
    bool stopped();

    //! Returns a state of the chain.
    ExChainState state();

    //! Get the chain's full title.
    QString fullChainTitle();

    //! Gets the chain's title for the given chainable.
    QString chainTitle(CxChainable *pChainable);

    //! Gets a set of all chainables in the chain. It does not matter if the chain contains branches.
    QSet<CxChainable*> allChainables();

    //! Gets a set of all image sources in the chain.
    QSet<CxImageSource*> imageSources();

    //! Gets a set of all data consumers in the chain.
    QSet<CxChainable*> consumers();

    //! Gets a set of all objects listening this chain.
    QSet<QObject*> nonChainableListeners();

    //! Gets all view receivers in the chain.
    QSet<CxViewImageReceiver*> viewReceivers();

    //! Gets all cameras associated with this chain. Internally it searches all image sources which are the CxCameraSource.
    QSet<XICORE_HANDLE> cameras();

signals:

    //! The signal is emitted when the chain was started
    void chainStarted(CxChain *pChain);

    //! The signal is emitted when the chain was stopped.
    void chainStopped(CxChain *pChain);

    //! The signal is emitted in destructor of the object.
    void aboutToDelete(CxChain *pSender);    

    //! The signal is emitted from CxChain::remove() just before the chainable object is removed from the chain.
    void aboutToRemove(CxChain *pSender, CxChainable *pRemoving);

    //! The signal is emitted from CxChain::start() if the chain start failed or some problem appeared while starting.
    void chainErrorState(CxChain::ExChainErrorState eState, QString sErrMsg);

private:
    /*!
     \brief Inserts new element before the specified element.

    \remarks If the \p pBefore has a precedessor, then it will be reconnected so that it became
     the precedessor of the \p pChainable.
      To be able to insert before the element, these conditions must be satisfied:
     \li both of the parameters must be at the state of CxChainable::eStopped,
     \li the current chain of the \p pChainable must be NULL (\ref chain()),
     \li the current chain of the \p pBefore must be this chain,
     \li the \p pBefore must not be a \ref CxImageSource,
     \li the \p pChainable must not be a data consumer (the \ref CxChainable::isDataConsumer() must
     return false.
     \li if this chain is empty then the \p pBefore must be NULL.
    */
    bool insertBefore(CxChainable *pChainable, CxChainable *pBefore);


    /*!
     \brief Inserts new element after the specified element as a new branch.

     \remarks If the \p pAfter has successors then the \p pChainable will be added as a new branch.
    To be able to insert after the element, these conditions must be satisfied:
     \li both of the parameters must be at the state of CxChainable::eStopped,
     \li the current chain of the \p pChainable must be NULL (\ref chain()),
     \li the current chain of the \p pAfter must be this chain,
     \li the \p pAfter must not be a data consumer (the \ref CxChainable::isDataConsumer() must
     return true,
     \li if this chain is empty then the \p pAfter must be NULL.
     \li if the \p pAfter is not NULL then the \p pChainable must not be \ref CxImageSource.
    */
    bool branchAfter(CxChainable *pChainable, CxChainable *pAfter);

    /*!
      \brief Insert new element after the specified element.
      \remarks The current sucessors of the \p pAfter will be reconnected to the \p pChainable and the \p pChainable
      becames the new successor of the \p pAfter.
     */
    bool insertAfter(CxChainable *pChainable, CxChainable *pAfter);

    /*!
      \brief Inserts new element in place of the specified element.
      \remarks The old element is removed, and all its sucessors and precedessors are connected to new element.
     */
    bool replace(CxChainable *pNewChainable, CxChainable *pOldChainable);

    //! The method removes given \p pChainable from this chain.
    bool remove(CxChainable *pChainable);

    /*!
     * \brief Connects an arbitrary object \p pListener to the \p pChainable.
     * \remarks the \p onDataAvailableSlot and \p onFreePoolBuffersSlot must be generated be the SLOT macro.
     * \sa disconnectNonChainable()
     */
    bool connectNonChainable(CxChainable *pChainable, QObject *pListener, const char *onDataAvailableSlot,
                             const char *onFreePoolBuffersSlot);

    /*!
     * \brief Disconnects an arbitrary object \p pListener from the \p pChainable which was connected
     * using the connectChainable() method.
     * \remarks the \p onDataAvailableSlot and \p onFreePoolBuffersSlot must be generated be the SLOT macro.
     */
    bool disconnectNonChainable(CxChainable *pChainable, QObject *pListener, const char *onDataAvailableSlot,
                                const char *onFreePoolBuffersSlot);


    /*!
     * \brief Duplicates the chain starting from \p pRoot. All views (\ref CxMdiView) will be duplicated too.
     * \remarks All the duplicated members of the chain will be appended to the \p pNewRoot. If the \p bIncludeRoot
     * is true, then the duplicated \p pRoot will be included in the duplicated chain as well.
     */
    CxChain* duplicateChainWithViews(CxChainable *pRoot, CxChainable *pNewRoot, bool bIncludeRoot);

    /*!
     * \brief Duplicates the branch of the starting from \p pRoot to \p pLast. If the \p pLast
     * is an instance of \ref CxViewImageReceiver then the eventual image view attached to the receiver will
     * be duplitated as well.
     * \remarks All the duplicated members of the chain will be appended to the \p pNewRoot. If the \p bIncludeRoot
     * is true, then the duplicated \p pRoot will be included in the duplicated chain as well.
     */
    CxChain* duplicateBranch(CxChainable *pRoot, CxChainable *pLast, CxChainable *pNewRoot, bool bIncludeRoot);

    //! Internal method used by duplicateChainWithViews().
    bool addClonesWithViews(CxChain *pChain, CxChainable *pChainable, CxChainable *pPrevClone,
                            bool bAddChainableClone, bool bInsert);



    //! Returns a view connected the the chainable.
    CxMdiView* receiverView(CxViewImageReceiver *pChainable);

    //! Deletes the \p pChainable and all its successors.
    void deleteAll(CxChainable *pChainable);

    //! Returns a set leaves (chainables a the end of each branche).
    QSet<CxChainable*> leaves(CxChainable *pRoot = NULL);

    //! Removes a branch given by its leaf.
    void removeBranch(CxChainable *pLeaf);

    //! Exports the chain to DOM document.
    bool saveSettings(QDomDocument &aXml, QDomElement &aRoot);

    /*!
     * \brief Loads the chain from DOM.
     * \remarks If the chain is empty all the chain from DOM will be imported. If the current chain
     * is not empty (it contains a image source) then the image source will be kept and the rest of the
     * chain will be replaced by the chain from DOM and the image source from DOM will be excluded from import.
     */
    bool loadSettings(const QDomElement &aDom, QString *pErrorMsg = NULL);

    //! Returns UniqueID from metadata. If it is not present then it returns class name.
    //QString chainableExportName(const CxChainable *pChainable);

    //! Recursive method that saves a single chainable settings and all its successors.
    bool saveChainableSettings(const CxChainable *pChainable, QDomDocument &aXml, QDomElement &aDom);

    //! Recursive method that creates and loads a chainable and all successors from DOM.
    CxChainable* loadChainable(const QDomElement &aElm, CxChainable *pAfter);


    //! All chainables in the disconnected branches will be removed from chain (their chain will be set to NULL). For reconnecting use \ref connectBranch()
    QSet<CxChainable*> disconnectSuccessors(CxChainable *pChainable);

    //! Connect the \p pBranch to the \p pRoot. It is opposite method to \ref disconnectSuccessors()
    void connectBranch(CxChainable *pBranch, CxChainable *pRoot);

    void deleteSuccessors(CxChainable *pChainable);

    void deleteViewReceiver(CxViewImageReceiver *pReceiver);

    //! Recursive method that fills the \p setReceivers by all view receivers afther the \p pChainable
    void viewReceivers(CxChainable *pChainable, QSet<CxViewImageReceiver*> &setReceivers);

    CxCameraSource *getExistingNotOpenedCamera(const QString &sCamSerial, const QString &sCamName, const QString &sCamType, QString *pErrorMsg);

    static bool isImageSource(const QString &sClass);
private:
    //QSet<CxChainable*> m_setChainables;  /*!< Set of all chainables  */
    QMutex m_lock;                       /*!< Lock of this object */
    CxChainable *m_pRoot;                 /*!< The root of the chain */
    ExChainState m_eState;                 /*!< State of this chain. */
};


#endif // CHAIN_H
