#ifndef IMAGEMETADATA_H
#define IMAGEMETADATA_H

#include <QDateTime>
#include <QString>
#include <xiCoreGlobal.h>
#include <PicBuf.h>
#include <HsiCameraParams.h>
#include <QMap>

class CxCameraParameters;
class QDomDocument;
class QDomElement;

//! \brief Metadata stored next to image pixel data
class XICORE_API CxImageMetadata
{
    // camera properties
public:
    QString m_sCameraModel;         //!< String got from XI_PRM_DEVICE_NAME
    int m_iCameraModelId;           //!< Integer got from XI_PRM_DEVICE_MODEL_ID
    QString m_sCameraSerialNumber;  //!< String got from XI_PRM_DEVICE_SN
    QString m_sSensorSerialNumber;  //!< String got from XI_PRM_DEVICE_SENS_SN
    QString m_sCameraUserName;      //!< String got from XI_PRM_DEVICE_USER_ID
    QString m_sApiContextList;      //!< String got from XI_PRM_API_CONTEXT_LIST

    // camera settings
public:
    bool m_bAutoExposure;   //!< Is auto exposuse switched on?
    int m_iExposure;        //!< In microseconds - as in xiApi
    float m_fGain;          //!< Gain in dB, as in xiApi
    bool m_bAutoWB;         //!< Is AWB switched on?
    float m_fWhiteR, m_fWhiteG, m_fWhiteB;  //!< White balance coefficients
    float m_fColorCorrMatrix[16];           //!< Current color correction matrix
    float m_fGammaY, m_fGammaC, m_fSharpness;   //!< Other color correction parameters
    int m_iSensorTaps;                      //!< Number of sensor taps used when capturing the image
    int m_iDownsamplingType;                //!< Value of XI_PRM_DOWNSAMPLING_TYPE
    float m_fLensAperture, m_fLensFocalLength;  //!< Lens parameters (when present, 0 for none)
    float m_fTempHousing, m_fTempSensor;    //!< Housing and sensor temperatures in deg Celsius
    ExColorFilterArray m_eColorFilterArray; //!< Type of Bayer matrix, excfaNone when not RAW image
    uint m_uTransportDataFormat;            //!< The GenTL image format on transport layer (see \ref XI_GenTL_Image_Format_e in \ref xiApi.h).
	uint m_uDataFormat;						//!< Current format of pixels. (see \ref XI_GenTL_Image_Format_e in \ref xiApi.h). 
	uint m_uImageFormat;					//!< XI_IMG image format
    CxHsiCameraParams m_aHsiParams;         //!< Hyper Spectral Image information
    QDateTime m_aAcqDateTime;               //!< Date and time of exposure start	
	
	bool isApiContextListSet(){ return (m_sApiContextList.length() > 0); };
	void clearApiContextList(){ m_sApiContextList.clear(); };

public:
    CxImageMetadata();
    bool isColorCorrMatrixEmpty() const;

    void fillFromCameraSettings(CxCameraParameters* pPars, bool bForceReadContextList = false);  //!< Reads metadata values from current (cached) camera parameters
    void updateFromPicBufInfo(const SxPicBufInfo *pInfo);
    void replaceContextListVariable(const QString &sVariable, quint32 uiNewValue);      //!< Replaces value of a given parameter (e.g. 'xiApiImg:ActiveAreaWidth') in \ref m_sApiContextList
    static bool genTLFormatFromParams(uint &uiGenTLFormat, ExColorFilterArray eCfa, quint32 uiBpc, bool bPacked);

    // storage
public:
    void saveToXml(QDomDocument *pXmlDoc, QDomElement *pRootElement) const;   //!< Saves all values to XML document
    bool loadFromXml(QDomElement *pRootElement);        //!< Loads all values from XML document

    QString saveToXmlString() const;                    //!< Calls saveToXml, and saves the XML doc to string
    bool loadFromXmlString(const QString &sString);     //!< Loads all values from XML doc stored in a string

    // XML helper
public:
    static void saveTag(QDomDocument *pXmlDoc, QDomElement *pRootElement, const QString &sTag, const QString &sVal);
};

#endif // IMAGEMETADATA_H
