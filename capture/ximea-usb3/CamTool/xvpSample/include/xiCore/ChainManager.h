#ifndef CHAINMANAGER_H
#define CHAINMANAGER_H

#include <QList>
#include <QMap>
#include "Chain.h"

class QSettings;
class CxChainable;
class CxImageProvider;
class CxImageSource;
class CxCameraSource;
class CxCameraManager;
class CxStaticImageSource;
class CxImageData;
class IxImageDataLoader;


/*!
 \brief The class that creates, manages and manipulates with chains.
*/
class XICORE_API CxChainManager : public QObject
{
    Q_OBJECT

private:
    //! Private constructor (the class is singleton).
    CxChainManager();

public:
    //! Gets the class instance.
    static CxChainManager* instance();

    //! Destructor.
    ~CxChainManager();

    //! Creates an instance of new chain.
    CxChain* createChain();

    /*!
     \brief Creates an instance of camera source.

     \remarks Each camera can have only one instance of the camera source. If the method
     is called more than once for the valid camera handle then it fails (it returns NULL).
    */
    CxCameraSource* createCameraSource(XICORE_HANDLE hCamera);


    /*!
     \brief Gets the pointer to the existing camera source.
     \return If the camera source exists then it returns pointer to it, otherwise it returns NULL.
    */
    CxCameraSource* cameraSource(XICORE_HANDLE hCamera);

    //! For internal use only. Keeps the existing cameraSource, but sets its handle to NULL.
    void cameraDisconnected(XICORE_HANDLE hCamera);

    //! Searches for disconnected cameraSources whether newly connected camera belongs to them.
    void checkReviveCameraSourceAfterReconnecting(XICORE_HANDLE hCamera);

    /*!
     \brief Creates an instance of an image source.
     \remarks The function take the ownership of the \p pLoader,
     so that it takes care about its deletion.
    */
    CxImageSource* createImageSource(IxImageDataLoader *pLoader);

    /*!
     \brief Creates an instance of static image source.

     \remarks If the function succedes then it takes the ownership of the \p pImageData,
     so that it takes care about its deletion.
    */
    CxStaticImageSource* createStaticImageSource(CxImageData *pImageData, const QString& sTitle);


    /*!
     \brief Closes the chain.
     \remarks If the chain is started it will be stopped. The method removes all the chainable object from the chain and
     then deletes the chain.
    */
    bool closeChain(CxChain *pChain);


    /*!
     \brief Removes and deletes the chainable from the manager. The chainable
     must not be connected to any chain.
    */
    bool deleteChainable(CxChainable *pChainable);

    /*!
     * \brief Deletes all branches (including views) of the given chainable \p pRoot.
     */
    bool deleteBranches(CxChain *pChain, CxChainable *pRoot);

    //!  Gets a camera's chain.
    CxChain* cameraChain(XICORE_HANDLE hCamera);

    /*!
     \brief Inserts the \p pChainable before the \p pBefore object in the chain given by \p pChain.
     \remarks The \p pBefore must already be connected in the \p pChain. All precedessors of the
     \p pBefore will be disconnected from it and reconnected to \p pChainable.
    */
    bool insertBefore(CxChain *pChain, CxChainable *pChainable, CxChainable *pBefore);

    /*!
     \brief Inserts the \p pChainable after the \p pAfter in the chain given by \p pChain as
     a new branch.
     \remarks The \p pAfter must already be connected in the \p pChain.
    */
    bool branchAfter(CxChain *pChain, CxChainable *pChainable, CxChainable *pAfter);

    /*!
     \brief Inserts the \p pChainable after the \p pAfter in the chain given by \p pChain.
     \remarks The \p pAfter must already be connected in the \p pChain. All successors of the
     \p pAfter will be disconnected fro it and reconnected to the \p pChainable.
    */
    bool insertAfter(CxChain *pChain, CxChainable *pChainable, CxChainable *pAfter);

    /*!
      \brief Inserts new element in place of the specified element in the chain given by \p pChain.
      \remarks The old element is removed, and all its sucessors and precedessors are connected to new element.
     */
    bool replace(CxChain *pChain, CxChainable *pNewChainable, CxChainable *pOldChainable);

    /*!
     \brief Removes the \p pChainable from the \p pChain.
     \remarks If the \p bDeleteBranchWhenNoListener and the branch from which the \p pChainable was removed
     has no listener then the branch will be removed from the chain. If the chain has only one branch then
     the chain will be removed and the \p pChain will be set to NULL.
    */
    bool remove(CxChain *&pChain, CxChainable *pChainable, bool bDeleteBranchWhenNoListener = false);

    /*!
     \brief The method registers a custom object as a data receiver of the \p pChainable.
     \remarks Allways use the SLOT macro to define the \p onDataAvailableSlot and \p onFreePoolBuffersSlot.
     These slots will be connected to CxChainable::dataAvailable() signal and CxMemoryManager::freePoolBuffers().
     \sa disconnectNonChainable()
    */
    bool connectNonChainable(CxChain *pChain, CxChainable *pChainable, QObject *pListener,
                             const char *onDataAvailableSlot, const char *onFreePoolBuffersSlot);


    /*!
     * \brief Discards the connection created by \ref connectNonChainable()
     */
    bool disconnectNonChainable(CxChain *&pChain, CxChainable *pChainable, QObject *pListener,
                                const char *onDataAvailableSlot, const char *onFreePoolBuffersSlot,
                                bool bDeleteWhenNoListener = false);

    /*!
     * \brief Duplicates the \p pChain starting from \p pRoot. All views (\ref CxMdiView) will be duplicated too.
     * \remarks All the duplicated members of the chain will be appended to the \p pNewRoot. If the \p bIncludeRoot
     * is true, then the duplicated \p pRoot will be included in the duplicated chain as well.
     */
    CxChain* duplicateChainWithViews(CxChain *pChain, CxChainable *pRoot, CxChainable *pNewRoot, bool bIncludeRoot);

    /*!
     * \brief Duplicates the branch of the \p pChain starting from \p pRoot to \p pLast. If the \p pLast
     * is an instance of \ref CxViewImageReceiver then the eventual image view attached to the receiver will
     * be duplitated as well.
     * \remarks All the duplicated members of the chain will be appended to the \p pNewRoot. If the \p bIncludeRoot
     * is true, then the duplicated \p pRoot will be included in the duplicated chain as well.
     */
    CxChain *duplicateChainBranch(CxChain *pChain, CxChainable *pRoot, CxChainable *pLast,
                                          CxChainable *pNewRoot, bool bIncludeRoot);

    //! Exports the chain to DOM document.
    static bool exportChain(CxChain *pChain, QDomDocument &aXml, QDomElement &aRoot);

    //! Export the chain to XML string.
    static bool exportChain(CxChain *pChain, QString &sXml);

    //! Saves the chain to XML file.
    static bool saveChain(CxChain *pChain, const QString &sFileName);

    //! Loads the chain settings from the given file to a DOM document.
    bool loadChainSettings(const QString &sFileName, QDomDocument &aDocChain, QString *pErrMsg=NULL);

    //! Saved the object settings to QSettings.
    static void saveChainableSettings(CxChainable *pObject, QSettings *pSettings);

    //! Loads the object settings from QSettings.
    static void loadChainableSettings(CxChainable *pObject, QSettings *pSettings);

    /*!
     * \brief Replaces the existing chain by the chain imported from the given DOM document.
     * \remarks. The chain being imported will be appended to the existing image source of the \p pChain.
     * The image source from the from the DOM will be excluded.
     */
    bool importToExistingChain(CxChain *pChain, const QDomDocument &aDocChain);

    //! Loads and creates a chain from the DOM document.
    CxChain* loadChain(const QDomDocument &aDocChain, QString *pErrorMsg = NULL);

    //! Gets all the camera sources.
    const QMap<XICORE_HANDLE, CxCameraSource*> cameraSources() const;

    //! Returns a chainable with given UniqueId. If it is not present in the \p pChain then it returns NULL.
    CxChainable* findChainableByUniqueID(CxChain *pChain, const QString &sUniqueID);

private slots:
    void onChainErrorState(CxChain::ExChainErrorState eState, QString sErrMsg);

signals:
    void chainErrorState(CxChain *pChain, CxChain::ExChainErrorState eState, QString sErrMsg);

private:
    //!Removes branches with no listeners. If the chain is empty, then it will be destroyed.
    void removeBlindBranches(CxChain *&pChain);
    //! Removes the blind branches the \p pChainable is connected to.
    void removeBlindBranches(CxChain *&pChain, CxChainable *pChainable);

private:
    QSet<CxChain*> m_setChains;  /*!< The set of chains. */    
    QMap<XICORE_HANDLE, CxCameraSource*> m_mapCameraSources;     /*!< Map of camera sources */    
    QSet<CxChainable*>  m_setChainables;                         /*!< Set of chainables. */
    QMutex m_lock;                                              /*!< The lock of this object. */
};


#endif // CHAINMANAGER_H
