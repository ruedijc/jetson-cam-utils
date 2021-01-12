#ifndef CAMERAPARAMETERS_H
#define CAMERAPARAMETERS_H

#include <QMap>
#include <QSize>
#include <QStringList>
#include <QRectF>
#include <QVector>
#include <QObject>
#include <QMutex>

#include <ValueTypes.h>
#include <ImageMetadata.h>
#include <PicBuf.h>


//! \brief Information about camera device.
struct SxCameraInfo
{
    QString m_sName;        //!< Camera model name (XI_PRM_DEVICE_NAME)
    QString m_sType;        //!< Device type (XI_PRM_DEVICE_TYPE)
    int m_iDeviceModelId;   //!< Camera model ID (XI_PRM_DEVICE_MODEL_ID)
    QString m_sSerial;      //!< Serial number. We expect it to be unique. (XI_PRM_DEVICE_SN)
    QString m_sSensorSerial;//!< Serial number of the sensor. We expect it to be unique. (XI_PRM_DEVICE_SENS_SN)
    QString m_sUserId;      //!< Camera user id (as set by user, and loaded later opening the camera)
    QString m_sInstancePath;    //!< Instance path of the device. (XI_PRM_DEVICE_INSTANCE_PATH)
    ExColorFilterArray m_eColorFilterArray; //!< Color filter array type of RAW data (XI_PRM_COLOR_FILTER_ARRAY)
};

//---------------------------------------------------------------------------
//! \brief Class describing invalidator connection
class XICORE_API CxValueRegInvalidator
{
public:
    QString m_sXiApiName;   //!< name of the parameter that invalidates this register
    bool m_bReSet;          //!< whe set to true, we should set the read value back to API again

    CxValueRegInvalidator(const QString& sXiApiName, bool bReSet = false);
    CxValueRegInvalidator(const char* szXiApiName);
};

typedef QList<CxValueRegInvalidator> TxValueRegInvalidatorList;

//---------------------------------------------------------------------------
//! \brief This class is responsible for the communitaion with the camera
class XICORE_API CxValueRegister
{
public:
    QString m_sXiApiName;           //!< which parameter should we call (read from XML, e.g. "exposure" or "exposure:min". If empty, no update possible
    TxValueRegInvalidatorList m_lstInvalidators;  //!< paramaters that invalidate this register when they change
    bool m_bIsPath;                 //!< Tells if (string) parameter is file name (send to xiApi in fileSystemRepresentation, not UTF-8)

    enum EStringValEncoding
    {
        esveUtf8,           //!< string value encoded in UTF-8 (default)
        esveFileSystem,     //!< string represents file in disk
    };

public:
    CxValueRegister();
    bool readValue(XICORE_HANDLE hCamera, CxValue &vValue, int *piRetVal = NULL) const;   //!< Note: vValue.m_eType must be correctly filler
    bool storeValue(XICORE_HANDLE hCamera, const CxValue &vValue, int *piRetVal = NULL) const;
    bool isInvalidatedByParam(const QString &sInvalidator) const;
    bool readIfInvalidated(XICORE_HANDLE hCamera, CxValue &vValue, const QString &sInvalidator) const;

    // generic communication with xiApi
public:
    static bool readValue(XICORE_HANDLE hCamera, const QString& sXiApiParamName, CxValue &vValue,
                          int *piRetVal = NULL, EStringValEncoding eStringEncoding = esveUtf8);
    static bool storeValue(XICORE_HANDLE hCamera, const QString& sXiApiParamName, const CxValue &vValue,
                           int *piRetVal = NULL, EStringValEncoding eStringEncoding = esveUtf8);
};

//---------------------------------------------------------------------------
class XICORE_API CxCameraParam
{
    friend class CxCameraCapsLoader;
    friend class CxCameraParameters;
public:

    enum ExType
    {
        ecpInvalid,
        ecpInt,
        ecpFloat,
        ecpBool,
        ecpString,
        ecpEnum,
        ecpCommand,
    };

    enum ExRepresentation
    {
        erepDefault,
        erepLogarithmic,
    };

    //! Camera parameter visibility level (selected in app GUI)
    enum ExVisibility
    {
        evisInvisible = 0,
        evisBeginner  = 1,  // integer values are used for comparing the level
        evisExpert    = 2,
        evisGuru      = 3,

        evisMaxLevel  = evisGuru,
    };

    //! Struct holding all info for each item of enumerated parameter (ecpEnum)
    struct XICORE_API SxEnumItem
    {
        CxValue m_vValue;
        QString m_sCaption;

        SxEnumItem();
        SxEnumItem(const CxValue& vVal, const QString& sCaption);
    };

public:
    QString m_sXiApiName;   //!< Parameter name as in xiApi (e.g. XI_PRM_EXPOSURE)
    QString m_sCaption;     //!< User-friendly name of this parameter
    QString m_sTooltip;     //!< Some longer text about this parameter
    QString m_sUIGroup;     //!< Name of the section in camera settings this parameter should belong to
    bool m_bIsPath;         //!< Tells if (string) parameter is file name (should display Browse dialog)
    bool m_bReadOnly;       //!< If we can write this parameter to camera
	bool m_bNotStreamable;       //!< denotes that the corresponding feature is prepared to be stored to and loaded from a file via the GenApi node tree.
    ExVisibility m_eVisible;        //!< Tells that this camera parameter should be present in GUI
    bool m_bNeedsRegularRefresh;    //!< We should poll this parameter to get new updates automatically (even if there is no invalidator)
    bool m_bRequiresAcqStop;        //!< Acquisition loop has to be stopped before settings this parameter
    bool m_bInvalidatesAll;         //!< Invalidates all parameters - all need refresh
    ExRepresentation m_eRepresentation;  //!< How the parameter should look like in GUI
    QStringList m_lstSelectedParams;     //!< List of parameter names selected by this parameter (i.e. this parameter is "selector")

    ExType m_eType;         //!< Type of the parameter: int, float, bool, string, enum, command...

    CxValue m_vValue;       //!< Current value of the parameter, type is selected accordnig to m_eType
    CxValue m_vMin;         //!< Minimum value as read from xiApi or defined in XML
    CxValue m_vMax;         //!< Maximum value as read from xiApi or defined in XML
    CxValue m_vIncrement;   //!< Value increment as read from xiApi or defined in XML

    QVector<SxEnumItem> m_vecEnumValues;   //!< Special case for enumerated parameter, selection stored in m_vValue.intVal. Filtered using XI_PRM_INFO_SETTABLE.

    CxValue m_vAppDefault;  //!< Default value as defined in XML (may be undefined - type is etInvalid)

    QStringList m_lstFileFormats;   //!< File formats defined for this (string) parameter

    // communitation with camera
private:
    CxValueRegister m_aValueRegister;
    CxValueRegister m_aMinRegister;
    CxValueRegister m_aMaxRegister;
    CxValueRegister m_aIncRegister;

    QVector<SxEnumItem> m_vecEnumValuesUnfiltered;   //!< All enum values as defined in XML. Filtered values according to camera caps are in m_vecEnumValues

public:
    bool m_bCameraError;    //!< There was an error when getting the value of this parameter. Probably not supported by camera

public:
    CxCameraParam(ExType eType, const QString &sXiApiName, const QString &sCaption, const QString &sUIGroup, ExVisibility eVisibility);
    CxCameraParam();        //!< This default constructor should not be used

    void readInfo(XICORE_HANDLE hCamera);            //!< Reads m_vMin, m_vMax and m_vIncrement from xiApi, should be called only once
    bool readFromCamera(XICORE_HANDLE hCamera, int *piRetVal = NULL);      //!< Reads current value from the camera. iRetVal contains xiApi return value.
    bool writeToCamera(XICORE_HANDLE hCamera, int *piRetVal = NULL) const; //!< Sends the current value to camera. iRetVal contains xiApi return value.
    void filterEnumValues(XICORE_HANDLE hCamera);    //!< Filter out enum values
    void limitEnumsWorkaround(XICORE_HANDLE hCamera);//!< Filter out enum values (old hack)

    bool updateIfInvalidatedWithParam(XICORE_HANDLE hCamera, const QString &sInvalidator); //!< Returns true if it has sInvalidator among its invalidators

public:
    bool isVisibleInGUI() const;                        //!< Parameter should be visible in Camera settings
    bool isVisibleForLevel(ExVisibility eLevel) const;  //!< Same as isVisibleInGUI, but checks visibility level too
    bool belongsToGroup(const QString &sUIGroup, ExVisibility eLevel = evisMaxLevel) const; //!< Same as isVisibleForLevel, but verifies the group membership too
    bool isInvalidatedByParam(const QString &sParam) const; //!< Returns true if this parameter is invalidated by another parameter
    bool isSelector() const;        //!< This parameter is selector for other parameters

public:
    CxValue paramValue() const;   //!< returns current value. When enum, sends the value, not selection index.
    int indexOfEnumValue(const CxValue &vValue) const;   //!< Returns index if value is among this param's enum values
    bool setParamValue(const CxValue &vValue);   //!< sets parameter value. When enum, sets the correct selection index.
    QString paramValueDisplayString() const;     //!< returns current value formatted for display (useful for enums)
};

//---------------------------------------------------------------------------
typedef QMap<int, CxValue> TxSelectorToValueMap;   // maps selector enum int value to the actual value

//---------------------------------------------------------------------------
enum ExHsiCorrection
{
    ehcOff = 0,
    ehcShading,
    ehcReflectance
};

//---------------------------------------------------------------------------
class XICORE_API CxHsiCorrectionImage
{
public:
    CxHsiCorrectionImage();
    ~CxHsiCorrectionImage();
    void reset();
    bool isEmpty();
    bool setPicbuf(const SxPicBuf &aPic, const CxImageMetadata &aMetadata, bool isWhiteImage); //!< The ownership of the \p aPic is passed to this object.
    bool initDefault(const SxPicBufInfo &aPicInfo, const CxImageMetadata &aMetadata);
    CxImageMetadata metadata();
    bool load(const QString &sPath, bool bIsWhite);
    bool save(const QString &sPath);
    const double *meansPerBand();
    void lock();
    void unlock();

    static bool apply(CxHsiCorrectionImage &aWhite, CxHsiCorrectionImage &aDark, CxHsiCorrectionImage &aDarkBias,
                      SxPicBuf &aPic, const CxImageMetadata &aPicMetadata, ExHsiCorrection eCorrType, QString *pErrMsg = NULL);
    static bool validate(CxHsiCorrectionImage &aWhite, CxHsiCorrectionImage &aDark, CxHsiCorrectionImage &aDarkBias,
                         bool &bDarkValid, bool &bBiasValid, QString *pErrMsg = NULL);
    static bool validate(CxHsiCorrectionImage &aWhite, CxHsiCorrectionImage &aDark, CxHsiCorrectionImage &aDarkBias,
                         SxPicBufInfo &aPicInfo, const CxImageMetadata &aPicMetadata, bool &bDarkValid, bool &bBiasValid, QString *pErrMsg = NULL);

private:
    void calculateMeansPerBand();
    static bool validateNoLock(CxHsiCorrectionImage &aWhite, CxHsiCorrectionImage &aDark, CxHsiCorrectionImage &aDarkBias,
                         bool &bDarkValid, bool &bBiasValid, QString *pErrMsg = NULL);

    static bool validateNoLock(CxHsiCorrectionImage &aWhite, CxHsiCorrectionImage &aDark, CxHsiCorrectionImage &aDarkBias,
                         SxPicBufInfo &aPicInfo, const CxImageMetadata &aPicMetadata, bool &bDarkValid, bool &bBiasValid, QString *pErrMsg = NULL);
private:
    SxPicBuf m_pic;
    CxImageMetadata m_mdata;
    double *m_pMeansPerBand; //!< Mean values per each band in the center of the WHITE image. For dark images it is NULL.
    QMutex m_lock;
};

//---------------------------------------------------------------------------
class XICORE_API CxShadingPreset
{
public:
    QString m_sName;            //!< Name of the preset
    class CxItem
    {
    public:
        QString m_sXiApiName;       //!< Parameter name
        CxValue m_vVal;             //!< Parameter value
    };
    QList<CxItem> m_lstParamValues; //!< Important camera parameters (defined in definingParamList())
    QRect m_rcRoi;                  //!< ROI is always stored
    QString m_sDarkFilename;        //!< File with dark frame for shaindg correction
    QString m_sMidFilename;         //!< File with light frame for shaindg correction

public:
    CxShadingPreset();

    void saveCurrentValuesFrom(CxCameraParameters *pPars);      //!< Save values of defining camera parameters
    void saveShadingImages(CxCameraParameters *pPars, SxPicBuf *pPicDark, SxPicBuf *pPicMid, const QString &sStorageFolder);    //!< Save shading images
    void removeShadingImages();                                 //!< Delete stored images
    bool hasSameValuesAs(CxCameraParameters *pPars) const;      //!< Are camera parameters same?
    void activateValuesTo(CxCameraParameters *pPars) const;     //!< Activate shading correction

    static QStringList definingParamList();     //!< List of defining camera parameters (exposure, downsampling, etc...)

    static bool isShadingActive(CxCameraParameters *pPars);      //!< Returns true if shading is switched on on the camera (TODO: remove after parameter exists in xiApi)
    static void disableShading(CxCameraParameters *pPars);       //!< Switches off shading (TODO: remove after parameter exists in xiApi)
};

typedef QList<CxShadingPreset> TxShadingPresetList;

//---------------------------------------------------------------------------
class XICORE_API CxCameraParameters : public QObject
{
    Q_OBJECT
private:
    QMap<QString, CxCameraParam> m_mapParams;   //!< Map of all supported parameters by this camera, keys are their XiApiNames
    QStringList m_lstParamsOrder;               //!< Parameter names in the order they appeared in xml
    bool m_bParamsLoadedFromManifest;           //!< Parameter definition was loaded from device_manifest camera parameter

private:
    XICORE_HANDLE m_hCamera;                     //!< Handle to camera
    SxCameraInfo m_aCamInfo;                     //!< Basic camera informations
    CxHsiCameraParams m_aHsiParams;              //!< Hyperspectral information

    QSize m_sizeSensor;           //!< Physical resolution of the sensor
    QSize m_sizeDownsampledRes;   //!< Remember the maximum resolution we can get at the current downsampling mode. It will enalbe us reset ROI in one go.
    QRectF m_rcLastROI;           //!< Save ROI relatively to DownsampledRes as float vals (i.e. 1.0=full width)
    QMap<QString, CxValue> m_mapDefaultValues; //! The default values of some parameters immediatelly after the camera was opened.
    int m_iLastXiApiError;

public:
    CxCameraParameters(XICORE_HANDLE hCamera, const SxCameraInfo &aCamInfo);

    XICORE_HANDLE cameraHandle() const;             //!< Returns camera handle to which this object is connected
    static bool isValidPointer(CxCameraParameters* pPars);  //!< In case the caller wants to verify the camera was not disconnected in the meantime

    bool loadFromXMLDescription();                          //!< Clears everything and uses the definition stored in the external xml file for this camera
    void saveDescriptionToXML(const QString& sFile);        //!< For testing purposes, inverse to loadFromXMLDescription
    bool isXMLDescriptionFromCameraManifest();              //!< Parameter definition was loaded from device_manifest camera parameter

    const SxCameraInfo& cameraInfo() const;
    const CxHsiCameraParams& hsiParams() const;
    const QSize& sensorResolution() const;          //!< Physical resolution of the sensor
    const QSize& downsampledResolution() const;     //!< Maximum resolution at the current downsampling (when no ROI active)
    const QRectF& lastROI() const;                  //!< ROI relatively to resolution as float vals (i.e. 1.0=full width)
    void setLastROI(const QRectF &rc);              //!< Tells us to remember provided ROI geometry for next time
    bool setCameraUserId(const QString &sUserId);   //!< Write custom User ID to camera flash memory
	int  getCameraUserIdMaxLen();					//!< Read max. length of Used ID supported by camera

    void saveParamValues(const QString &sStorageFolder, CxCameraParam::ExVisibility eFilterVisibleUpTo);   //!< Save values of all parameters to file, so that we can load it next time (called on app exit)
    void saveParamValuesToFile(const QString &sFilename, CxCameraParam::ExVisibility eFilterVisibleUpTo);  //!< Save values of all parameters to custom file
    QString saveParamValuesToString(CxCameraParam::ExVisibility eFilterVisibleUpTo);                       //!< Save values of all parameters to string
    bool loadParamValues(const QString &sStorageFolder, CxCameraParam::ExVisibility eFilterVisibleUpTo, QStringList *plstLoadedParams = NULL);   //!< Load saved values, called at the start of the application
    bool loadParamValuesFromFile(const QString &sFilename, CxCameraParam::ExVisibility eFilterVisibleUpTo, QStringList *plstLoadedParams = NULL, bool bNotifyGUI = false);     //!< Load saved value from custom file
    bool loadParamValuesFromString(const QString &sString, CxCameraParam::ExVisibility eFilterVisibleUpTo, QStringList *plstLoadedParams = NULL, bool bNotifyGUI = false);     //!< Load saved value from string
    static bool queryCameraStoredInParamFile(const QString &sFilename, QString *psCamera, QString *psSerial); //!< Returns camera model and serial stored in xml file with values
    static bool queryCameraStoredInParamString(const QString &sString, QString *psCamera, QString *psSerial); //!< Returns camera model and serial stored in xml string with values
    void removeParamValuesFile(const QString &sStorageFolder);

    void saveAdditionalSettingsToXml(QDomDocument *pXmlDoc, QDomElement *pRootElement);
    void loadAdditionalSettingsFromXml(QDomElement *pRootElement);

public:
    bool addParam(const CxCameraParam& aParam);     //!< Adds new parameter to our list
    CxCameraParam* findParam(const QString& sXiApiParamName, bool bOnlyWhenSupportedByCamera = false);  //!< Returns NULL when not found
    CxValue currentParamValue(const QString& sXiApiParamName); //!< Refresh current value from camera and return it

    const QStringList& paramsOrderList() const;          //!< Returns list with parameter names in the order they appeared in xml
    QString selectorParamNameFor(const QString& sXiApiParamName);  //!< Returns the selector for camera parameter. Empty string when none.

    /* Sending parameter values to camera. As we need to work with automatic
       invalidators, it is better to send modified parameters in a batch */
private:
    QStringList m_lstParamsToCommit;    //!< Parameter xiApiNames in current commit batch
public:
    enum ExCommitRefreshGuiMode { ecrgRefreshNormal, ecrgRefreshIncludeCommited, ecrgNoRefresh };

    bool commitSingleParamValue(const QString& sXiApiParamName, ExCommitRefreshGuiMode eGuiRefreshMode = ecrgRefreshNormal);    //!< Adds parameter to batch and commits it right away.
    bool commitSingleParamValue(const CxCameraParam *pParam, ExCommitRefreshGuiMode eGuiRefreshMode = ecrgRefreshNormal);       //!< Adds parameter to batch and commits it right away.
    void addParamCommitToBatch(const QString& sXiApiParamName);     //!< Marks the parameter to be sent to camera in this batch.
    void addParamCommitToBatch(const CxCameraParam *pParam);        //!< Marks the parameter to be sent to camera in this batch.
    bool endCommitBatch(ExCommitRefreshGuiMode eGuiRefreshMode = ecrgRefreshNormal);  //!< Sends all parameters in batch to camera, and checks invalidators
    int lastXiApiError();                   //!< Last xiApi error code from any commit function

    /* In an improbable event of direct communication with the camera, use the following functions.
       You can even use the parameters not in our list, use it as an API call. But we will cover
       refresing the GUI in case parameters are set to camera. */
public:
    bool setParamDirect(const QString& sXiApiParamName, const CxValue &vValue, int *piRetVal = NULL, ExCommitRefreshGuiMode eGuiRefreshMode = ecrgRefreshIncludeCommited);    //!< Use commitSingleParamValue instead of this. iRetVal contains xiApi return value.
    bool getParamDirect(const QString& sXiApiParamName, CxValue &vValue, int *piRetVal = NULL) const;    //!< Note: vValue.m_eType must be correctly filled. iRetVal contains xiApi return value.

    TxSelectorToValueMap getAllSelectorValuePairs(const QString& sSelectorParamName, const QString& sValueParamName);
    void setAllSelectorValuePairs(const QString& sSelectorParamName, const QString& sValueParamName, const TxSelectorToValueMap &mapValues);

    /* Some default parameter's values need to be kept. Currently it is:
     *  - XI_PRM_ACQ_TRANSPORT_BUFFER_SIZE (default)
     *  - XI_PRM_ACQ_TRANSPORT_BUFFER_SIZE (minimum, increment)
     *  - XI_PRM_BUFFERS_QUEUE_SIZE (default)
     * These parameters are read immediatelly the camera was opened.
     * */
    CxValue defaultParamValue(const QString &sXiApiParamName);
private:
    void readDefaultParamValues();


    // ROI helpers
public:
    QSize calcCurrentImageSize(bool bForceRefresh = false);       //!<< Returns width and height of the image returned by the camera (may differ from PRM_WIDTH adn PRM_HEIGHT in Multi-ROI mode)
    void setROIRectParams(int x, int y, int w, int h, bool bCheckMinMax = false);   //!< Sends new ROI to camera, while checking min/max/inc optionally
    void setROIRectParams(const QRect &rc, bool bCheckMinMax = false);              //!< Sends new ROI to camera, while checking min/max/inc optionally
    QRect currentROIRectParams();                   //!< Returns current ROI as rectangle
    void setImageFormatKeepRoi(int eXiImgFormat);   //!< Switches XI_PRM_IMAGE_DATA_FORMAT and forces to keep current ROI

private:
    void addParamIfNotPresent(const QString& sXiApiParamName, CxCameraParam::ExType eType);

signals:
    void parametersNeedRefresh(const QStringList& lstParamNames);   //!< Signal for GUI that it should rehresh controls for these parameters
    void shadingChanged();                      //!< Signal for GUI that it should rehresh shading controls

private slots:
    void on_timerRefreshParams_update();        //!< Timer for auto-refreshed parameters (temperature,...)

//HSI related members
public:
    bool rereadHsiParamsForRange(int iStartNm, int iEndNm);     //!< switch to different filter range (when lens changed, e.g. for 5x5)
    bool setWhiteImage(const SxPicBuf &aPic, const CxImageMetadata &aMetadata);
    bool setDarkImage(const SxPicBuf &aPic, const CxImageMetadata &aMetadata);
    bool setDarkBiasImage(const SxPicBuf &aPic, const CxImageMetadata &aMetadata);
    CxHsiCorrectionImage& whiteImage();
    CxHsiCorrectionImage& darkImage();
    CxHsiCorrectionImage& darkBiasImage();
    ExHsiCorrection hsiCorrection();
    void setHsiCorrection(ExHsiCorrection eCorr);
    bool isCorrectionReady();
private:
    CxHsiCorrectionImage m_picWhite;//!< Exposure time T_ref
    CxHsiCorrectionImage m_picDark;//!< Exposure time T_ref
    CxHsiCorrectionImage m_picDarkBias;//!< Exposure time same as the current live image being corrected
    ExHsiCorrection m_eHsiCorrection;
    QMutex m_lock;

    // Shading presets
public:
    TxShadingPresetList m_lstShadingPresets;
    QString m_sLastActiveShadingPresetName;     // its here for persistance
    void notifyShadingChanged();
};

//---------------------------------------------------------------------------
// list of known parameter groups
// dockers
#define DX_G_BASIC      "Basic"
#define DX_G_IMAGE_FMT  "Image Format"
#define DX_G_PERFORMANCE "Performance"
#define DX_G_COOLING    "Temperature"
#define DX_G_COLOR_CORR "Color Correction"
#define DX_G_DEVICE_IO  "Device IO"
#define DX_G_LENS_CTRL  "Lens Control"
// dialogs
#define DX_G_AE_SETUP       "AE Setup"
#define DX_G_HDR_SETUP      "HDR Setup"
#define DX_G_GPIO_SETUP     "GPIO Setup"
#define DX_G_DEBOUNCE_SETUP "Debounce Setup"

//---------------------------------------------------------------------------
#endif // CAMERAPARAMETERS_H
