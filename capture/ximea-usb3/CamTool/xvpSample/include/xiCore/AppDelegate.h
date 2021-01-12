#ifndef APPDELEGATE_H
#define APPDELEGATE_H

#include <xiCoreGlobal.h>
#include <QtGui/QIcon>
#include "../../xiGuiApp/AppOptions.h"

class QCursor;
class QWidget;
class QMainWindow;
class QSettings;
class QToolBar;
class CxChain;
class CxImageData;
class CxImageMetadata;
class CxVectorObject;
class QCustomPlot;
class QMenu;
class CxBinaryLayer;
class CxChainable;
class CxMdiView;
class CxImageSource;
struct SxPicBuf;
class CxAppOptions;

enum ExMainMenu
{
    exMenuFile,
    exMenuEdit,
    exMenuTools,
    exMenuPlugins,
    exMenuWindow,
    exMenuHelp
};

/**
 * @brief Class for plugins to communicate with the application. Use IxAppDelegate::instance()
 *        to get the pointer.
 * \warning This class should always be used in the main (GUI) thread. You should never
 * use it in CxChainable::processData(). In this case the application behaviour is undefined.
 */

class XICORE_API IxAppDelegate
{
public:
    static IxAppDelegate* instance();       //!< Returns the pointer to actual application delegate

public:
    virtual QMainWindow* mainWindow() = 0;  //!< For adding something to main menu, or anything
    virtual QToolBar* mainToolbar() = 0;    //!< For adding toolbar buttons
    virtual QMenu *mainMenu(ExMainMenu eMenu) = 0; //!< For adding actions to main menu
	virtual CxAppOptions* appOptions() = 0;			//!< For accessing application options
    virtual QSettings* settings() = 0;      //!< Use for storing plugin settings
    virtual QString appDataDirectory() = 0; //!< Store there your internal data, preserved for furute sessions
    virtual QString lastOpenedDir() = 0;    //!< Directory use last used
    virtual void setLastOpenedDir(const QString &sDir) = 0; //!< Set directory use last used

public:
    virtual void addDockedWidget(QWidget *pWidget, Qt::DockWidgetArea eDockArea, Qt::DockWidgetAreas aAllowedAreas, bool bShow) = 0; //!< Add new widget to main window. QDockWidget is created around it and docked in correct place.
    virtual void showNotification(const QString &sMessage) = 0; //!< Popup notification in top right corner

    // views
public:
    virtual CxChain* activeChain() = 0;     //!< Chain of the active view
    virtual int activeViewId() = 0;         //!< iViewId of the current view
    virtual void activateView(int iViewId) = 0;     //!< Let the view become selected
    virtual void tileViews() = 0;                   //!< Tiles all open views
    virtual void switchFullScreen(bool bActivate) = 0;  //!< Switches full screen viewing mode on or off
    virtual void enableGUI(bool bEnable) = 0;       //!< Disables or enables application GUI
	virtual void updateViewImage(int iViewId) = 0;

    /*!
     * \brief Loads chain from XML, attaches all views and starts it.
     * If \p pImageSource is not NULL then the chain will be appended to it and the image source from the XML will be omitted.
     */
    virtual CxChain* loadChainFromXML(const QString &sXml, bool bStart = true, CxImageSource *pImageSource = NULL,  QString *pErrMsg = NULL) = 0;
    virtual void showChainableConfiguration(CxChainable *pChainable, QWidget *pParent) = 0;//!< Displays a modal configuration dialog for given widget if it exists.
    virtual CxMdiView* viewPtr(int iViewId) = 0;//!< Pointer to view by its id.
    virtual CxChain* viewChain(int iViewId);    //!< Pointer to chain the view is belonging to
    virtual bool captureImageInView(int iViewId = -1) = 0;//!< Duplicates the image in active view (as in menu File - Capture)

    // camera
public:
    virtual bool runAcquisition(bool bStart, XICORE_HANDLE hCamera = NULL) = 0;   //!< Start or stop acquisition. Camera can be NULL in case of one camera connected.
    virtual XICORE_HANDLE cameraHandleFromView(int iViewId) = 0;        //!< Camera used in the given view (returns NULL when no camera invoved)
    virtual QList<int> viewIdsForCameraHandle(XICORE_HANDLE hCamera) = 0; //!< All views associated with given camera handle.
    virtual void enableCameraGUI(XICORE_HANDLE hCamera, bool bEnable) = 0; //!< Disables or enables user interaction with camera
    virtual void resetCameraSettings(XICORE_HANDLE hCamera) = 0;        //!< Resets the camera settings and disconnects it

    /*! Show dialog that lets user to capture a image from camera, returning the picture.
     * Caller must free and delete the picture). If you leave pParent NULL, mainWnd is used as parent.
     */
    virtual SxPicBuf* capturePictureWithGUI(QWidget* pParent, XICORE_HANDLE hCamera,
                                            const QString &sWndTitle, const QString &sInstructions,
                                            bool bEnableAveraging, int &iAverageCount,
                                            CxImageMetadata *pDstMetadata = NULL) = 0;

    // view contents
public:
    virtual bool openFile(const QString& sFilename, bool bAddToMRU = true) = 0; //!< Open file as new view. Optionally add the file name to recently used list.
    virtual bool createViewWithImage(CxImageData *pImgData, const QString &sTitle) = 0;        //!< Creates new view using the image. View takes ownership of the data and will free it when needed.
    virtual bool replaceViewImage(int iViewId, CxImageData *pImgData) = 0;      //!< Sets the new view image contents. View takes ownership of the data and will free it when needed.
    virtual const CxImageData* viewCurrentImage(int iViewId) = 0;       //!< Image currently displayed in view
    virtual TxVecObjID addVectorObject(int iViewId, CxVectorObject *pObj) = 0;  //!< Adds vector object to view. You should store the returned ID instead of object pointer.
    virtual CxVectorObject* vectorObjectPtr(int iViewId, TxVecObjID idObj) = 0; //!< In case you want to manipulate the object directly.
    virtual void removeVectorObject(int iViewId, TxVecObjID idObj) = 0; //!< Deletes the object
    virtual QVector<TxVecObjID> listAllVectorObjects(int iViewId) = 0;  //!< Returns the list of all objects in the scene
    virtual void overrideViewCursor(int iViewId, QCursor *pCursor) = 0; //!< Pass NULL to stop overriding
    virtual bool mapPositionToImage(int iViewId, const QPoint &ptScreenPos, QPointF *pPtInImage) = 0; //!< Recalculate to position inside the image. Returns true when inside.

    //binary layers
public:
    virtual TxBinaryLayerID addBinaryLayer(int iViewId, CxBinaryLayer *pLayer, bool bAutoColor = true) = 0; //!< Add new binary layer.
    virtual CxBinaryLayer* binaryLayerPtr(int iViewid, TxBinaryLayerID idLayer) = 0; //!< Returns a pointer to the binary layer with given id.
    virtual void removeBinaryLayer(int iViewId, TxBinaryLayerID idLayer) = 0; //!< Deletes the binary layers.
    virtual QList<TxBinaryLayerID> listAllBinaryLayers(int iViewId) = 0; //!< Returns a list of all binary layers in the view.
    virtual QList<TxBinaryLayerID> listVisibleBinaryLayers(int iViewId) = 0; //!< Returns a list of visible binary layers in the view.

    // GUI helpers
public:
    virtual void setCustomPlotColors(QCustomPlot *pPlot) = 0;   //!< Sets app-style colors to graph grawing component

    // sounds
    enum ExSound
    {
        eSoundCapture, //!< When image is captured or autosaved (instant save).
        eSoundRecord //!< When instant video recordint is started or stopped.
    };

    virtual void playSound(ExSound eSound) = 0; //!< Plays the application predefined sound.
    virtual void playSound(const QString &sFilename) = 0;//!< Plays a sound in WAV format.

    // generic helpers
public:
    static QByteArray fileSystemRepresentation(const QString &sFilename);
    static QString stringFromFileSystemRepresentation(const QByteArray &sEncodedFilename);

    // helpers for this class
public:
    static void setInstance(IxAppDelegate *pInstance);  //!< Reserved for internal use
    virtual ~IxAppDelegate();
};

#endif // APPDELEGATE_H
