#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <QObject>
#include <QMap>
#include <QSet>
#include <QMutex>
#include <QSettings>
#include <xiCoreGlobal.h>
#include <PicBuf.h>
#include <CameraParameters.h>


/*!
 \brief The class that provides all necessary methods to access cameras and
 to set up camera parameters.
*/
class XICORE_API CxCameraManager : public QObject
{
    Q_OBJECT

private:
    //! Private constructor (the class is singleton).
    CxCameraManager();

public:

    //! Gets the instance if this object.
    static CxCameraManager* instance();

    //! Destructor.
    ~CxCameraManager();

    //! Sets API parameters even before any camera is opened
    void setGeneralApiParameters() const;

    //! Gets the number of available cameras connected to PC.
    uint cameraCount() const;

    /*!
     \brief Gets the info about camera with given index \p uiCameraIdx.

     \remarks The index \p uiCameraIdx is related to cameras count returned
     by the \ref cameraCount() method (NOT by the \ref openedCameraCount() method!).
    */
    bool cameraInfo(uint uiCameraIdx, SxCameraInfo &aInfo) const;
    bool cameraSerialNumber(uint uiCameraIdx, QString &sSerialNumber) const;
    bool overrideCameraSNto(uint uiCameraIdx, QString &sNewSerialNumber);

    //!Opens a camera with given index and returns its handle.
    XICORE_HANDLE openCamera(uint uiCameraIdx, SxCameraInfo *pPreloadedInfo = NULL);

    //! Gets the number of opened cameras.
    uint openedCameraCount();

    //! Gets a list of handles to the opened cameras.
    QList<XICORE_HANDLE> openedCameras();

    bool isCameraOpened(XICORE_HANDLE hCamera);

    bool closeCamera(XICORE_HANDLE hCamera);
    bool startAcquisition(XICORE_HANDLE hCamera);
    bool stopAcquisition(XICORE_HANDLE hCamera);

    uint runningCameraCount();
    bool isCameraRunning(XICORE_HANDLE hCamera);
    QList<XICORE_HANDLE> runningCameras();

    //!  Gets the camera parameters.
    CxCameraParameters* cameraParameters(XICORE_HANDLE hCamera);

    //! Reads HSI calibration XML from camera
    bool readCameraHsiParams(XICORE_HANDLE hCamera, const SxCameraInfo &info, CxHsiCameraParams *pHsiParams) const;

    /*!
     \brief Call this method to notify the rest of the application that the camera
     format (image dimensions, components, ...) are to be changed.
    */
    void notifyCameraFormatChanging(XICORE_HANDLE hCamera);

    /*!
     \brief Call this method to notify rest of the application that the camera format
     (image dimensions, components, ...) has been changed.
    */
    void notifyCameraFormatChanged(XICORE_HANDLE hCamera);


    //! \brief Call this method to notify rest of the application that the camera just disconnected    */
    void notifyCameraLost(XICORE_HANDLE hCamera);

    //! \brief Call this method to notify rest of the application that the camera acquisition is failing  */
    void notifyCameraAcqError(XICORE_HANDLE hCamera, int iErrorCode, const QString &sInFunction);

    //! Gets the picbuf info currently produced by the given camera.
    bool cameraPicbufInfo(XICORE_HANDLE hCamera, SxPicBufInfo &picInfo);

    //! Gets the camera handle by the serial id of the camera.
    XICORE_HANDLE cameraHandleBySerialId(const QString &sSerialId);

    //! State of XI_PRM_DEBUG_LEVEL
    int apiDebugLevel() const;
    void setApiDebugLevel(int iLevel);
    //! State of XI_PRM_AUTO_BANDWIDTH_CALCULATION
    bool autoBandwidthCalculationEnabled() const;
    void enableAutoBandwidthCalculation(bool bEnable);
    //! State of XI_PRM_NEW_PROCESS_CHAIN_ENABLE parameter
    bool newProcessChainEnabled() const;
    void setNewProcessChainEnabled(bool bEnable);
    bool newProcessChainSupported() const;
	//! State of XI_PRM_CAM_SENSOR_INIT_DISABLED
	bool sensorInitDisabled() const;
	void setSensorInitDisabled(bool bSensDisable);
	//! State of XI_PRM_TEST_PATTERN
	bool testPatternEnabled() const;
	void setTestPattern(bool bEnable);
	void enableTestPatternAll();
	void enableTestPattern(XICORE_HANDLE hCamera);

    void saveSettings(QSettings *pSettings);    //!< store some options to app settings
    void loadSettings(QSettings *pSettings);    //!< load some options from app settings

public:
    void createCameraSimulator();
    void createCameraSimulator(int iResWidth, int iResHeight);
    void disconnectCameraSimulator();

    void createHsiCameraSimulator();
    void createRgbNirCameraSimulator();

public slots:
    /*!
     \brief Stops all cameras and closes them. This method should be called
     on appliction exit.
    */
    void finishAll();

signals:
    //! The signal is emitted when the acquistion on the given camera was started.
    void acquisitionStarted(XICORE_HANDLE hCamera);

    //! The signal is emitted when the acquistion on the given camera was finished.
    void acquisitionFinished(XICORE_HANDLE hCamera);

    //! The signal is emitted when camera is opened.
    void cameraOpened(XICORE_HANDLE hCamera);

    /*!
     \brief The signal is emitted when the camera was closed.

     \remarks The camera handle \p hCamera is invalid in the moment of emitting of this signal,
     so any attempt to communitation with the camera fails.
    */
    void cameraClosed(XICORE_HANDLE hCamera);

    //! The signal is emitted when camera disconnected during app session.
    void cameraLost(XICORE_HANDLE hCamera);

    //! The signal is emitted when camera acquisition fails.
    void cameraAcqError(XICORE_HANDLE hCamera, int iErrorCode, const QString &sInFunction);

    //! The signal is emitted when format of camera image (dimension, components..) is to be changed.
    void imageFormatChanging(XICORE_HANDLE hCamera);

    //! The signal is emitted when format of the camera image (dimension, komponents..) changed.
    void imageFormatChanged(XICORE_HANDLE hCamera);

private:
    QMap<XICORE_HANDLE, CxCameraParameters*> m_mapCameras;   /*!< Camera parameters by the camera handle */
    QSet<XICORE_HANDLE> m_setRunningCameras;                 /*!< Set of camera where the acquistion is running. */
    QMutex m_lock;                                           /*!< The lock of the manager. */
    XICORE_HANDLE m_hLastCameraLost;                         /*!< Do not emit the cameraLost signal more then once */
    XICORE_HANDLE m_hLastCameraAcqError;                     /*!< Do not emit the cameraAcqError signal more then once */
    QString m_sLastCameraAcqErrorFunc;

    // xiAPI options
    bool m_bEnableNewProcessChain;
    bool m_bNewProcessingChainSupported;
	bool m_bSensorInitDisabled;
	bool m_bEnableTestPattern;
    int m_iXiApiAutoBandwidthCalculation;   //!< Set XI_PRM_AUTO_BANDWIDTH_CALCULATION to ON
    int m_iXiApiDebugLevel;                 //!< Settings for XI_PRM_DEBUG_LEVEL
};

#endif // CAMERAMANAGER_H
