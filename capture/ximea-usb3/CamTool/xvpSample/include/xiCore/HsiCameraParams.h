#ifndef CXHSICAMERAPARAMS_H
#define CXHSICAMERAPARAMS_H

#include <xiCoreGlobal.h>

#include <QRect>
#include <QVector>

class QDomDocument;
class QDomElement;

struct _tagXI_HSI_CAMERA_PARAMS;
typedef struct _tagXI_HSI_CAMERA_PARAMS XI_HSI_CAMERA_PARAMS;

class XICORE_API CxHsiCameraParams
{
public:
    enum ExPattern
    {
        epNone,
        epMosaic,   // Mosaic (4x4, 5x5)
        epTiled,    // Tiled (MxN)
        epWedge,    // LineScan
    };
    enum ExCorrectionMatrixType
    {
        ecmtHyperspectral = 0, //!< matrix generates corrected hyperspectral samples
        ecmtRgb           = 1, //!< matrix generates true color RGB samples
        ecmtPanchromatic  = 2, //!< matrix generates panchromatic samples
    };
    enum ExCorrectionMatrixAlgorithm
    {
        ecmaPitchfork     = 0, // ?
        ecmaTacktile      = 1, //!< correction matrix must be applied prior to reflectance calculation
        ecmaBcls          = 2, //!< correction matrix must be applied after reflectance calculation
    };

    enum ExOpticalComponentType
    {
        eoctShortpassFilter     = 0, // shortpass_filter
        eoctLongpassFilter      = 1, // longpass_filter
        eoctBandpassFilter      = 2, // bandpass_filter
        eoctNotchFilter         = 3, // notch_filter
        eoctLinearPolarizer     = 4, // linear_polarizer
        eoctCircularPolarizer   = 5, // circular_polarizer
        eoctSource              = 6, // source
    };

    int m_iCalibrationRangeStartNm, m_iCalibrationRangeEndNm;   //!< Range of band calibration data (optional, zero when N/A)
    int m_iCalibrationResNm;                //!< Calibration data resolution (is usually 1 nm) (optional, zero when N/A)
    int m_iFilterRangeStartNm, m_iFilterRangeEndNm;     //!< Range of the active filter on camera (matches the filename, not read from XML)

    class XICORE_API CxBand
    {
    public:
        bool m_bSelected;                       //!< Flags if the signal of this band is within specifications (true) or not (false). Usage of non-selected bands will result in wrong spectra.
        QString m_sName;                        //!< Name in case it was written in XML. Is probably empty. Use name() method to get the caption.
        QVector<double> m_vecPeakWaveLengths;   //!< Central wavelength of peaks [nm]. At leask one peak should be defined.
        QVector<double> m_vecCalibData;         //!< Band response data, tag <response_composition> (not required for images)

    public:
        CxBand();
        bool isValid() const;
        double peakInRange(int iMinNm, int iMaxNm) const;   //!< Returns peak value in a specified range
        QString name() const;       //!< Returns band name as "123 nm"
    };

    class XICORE_API CxFilterZone
    {
    public:
        ExPattern m_eFilterPattern;             //!< Mosaic?
        int m_iPatternWidth, m_iPatternHeight;  //!< Pattern size (4x4, 5x5 for mosaic, or 1*h for line scan)
        int m_iFilterWidth, m_iFilterHeight;    //!< Filter size (1x1 for mosaic, w*X for line scan) - repetition of same band inside pattern
        QRect m_rcActiveArea;                   //!< Filter active area (optional, zero when N/A)
        int m_iSpectralRangeStartNm, m_iSpectralRangeEndNm;         //!< Active sensor range in nm (optional, zero when N/A)

        QVector<CxBand> m_vecBands;

    public:
        CxFilterZone();
        bool isValid() const;
        CxBand* bandAt(int xPos, int yPos);     //!< For xPos < m_iPatternWidth and yPos < m_iPatternHeight
        const CxBand* bandAt(int xPos, int yPos, int *piIdx = NULL) const;      //!< For xPos < m_iPatternWidth and yPos < m_iPatternHeight
        const CxBand* bandAtPicPos(int xPos, int yPos, int *piIdx = NULL) const; //!< Use \ref m_rcActiveArea to get the pattern pos
        bool isPicPosInZone(int xPos, int yPos) const;                          //!< Use \ref m_rcActiveArea to determine if picture position belongs into this zone
        QVector<int> sortedBandIdxs(bool bOnlySelected = false) const;          //!< Indexes to band vector sorted by band peak wavelength

        int selectedBandsCount() const;             //!< Number of selected bands in the spectrum.
        QVector<int> selectedBandsMapping() const;  //!< Mapping of band index to selected band index. If a band is not selected then the mapping index is -1.
    };

    QVector<CxFilterZone> m_vecFilterZones;     //!< List of zones on the sensor with active areas, bands definitions (>= 1)

    class XICORE_API CxVirtualBand              //!< Band record inside <virtual_bands>
    {
    public:
        double m_dWaveLength;                   //!< Central wavelength of the virtual band in nm
        double m_dFwhm;                         //!< Full width of the virtual band at half the band’s maximum, in nm
        QVector<double> m_vecCorrectionData;    //!< Correction data, size is equal to vecBands.count

    public:
        CxVirtualBand();
        bool isValid() const;
        QString name() const;       //!< Returns virtual band name as "123 nm"
    };

    class XICORE_API CxOpticalComponent  //!< Calibration data for used rejection filter (on camera, may not be present)
    {
    public:
        ExOpticalComponentType m_eType;
        QString m_sManufacturer;         //!< Manufacturer of the optical component
        QString m_sPartId;               //!< Textual description of the component
        QString m_sDescription;          //!< Unique part identifier as given by the manufacturer
        QString m_sTag;                  //!< Custom tag uniquely identifying the component
        int m_iStartNm, m_iEndNm;   //!< Range of band calibration data
        int m_iResNm;               //!< Calibration data resolution (is usually 1 nm)
        QVector<double> m_vecCalibData;         //!< Band response data, tag <response_composition> (not required for images)

    public:
        CxOpticalComponent();
        bool isValid() const;
        void clear();
        QString typeString() const;
    };

    class XICORE_API CxCorrectionMatrix
    {
    public:
        QString m_sName;                        //!< Name of the correction matrix
        int m_iStartNm, m_iEndNm;               //!< Filter range
        ExCorrectionMatrixType m_eType;         //!< Type of correction matrix
        ExCorrectionMatrixAlgorithm m_eAlgorithm; //!< Algorithm used to create the matrix

        QVector<CxVirtualBand> m_vecVirtualBands; //!< size is equal or lower than vecBands.count, may be present even in images (correction of spectrum). tag <spectral_correction_info>
        QVector<CxOpticalComponent> m_vecOpticalComponents; //!< List of optical components used in this matrix.

    public:
        CxCorrectionMatrix();
    };
    QVector<CxCorrectionMatrix> m_vecCorrectionMatrices;        //!< Possible filter options for the camera sensor (as read from camera XMLs)

    struct SxBandIdx
    {
        int zone;
        int band;
    };

    typedef QVector<SxBandIdx> TxBandIdxVector;

public:
    CxHsiCameraParams();
    bool isValid() const;
    bool isSimpleMosaicFilter() const;          //!< Returns true if there is only one zone, and is mosaic
    int totalBandCountInAllZones() const;       //!< Returns the sum of number of bands in all zones
    const CxFilterZone* filterZoneWithTotalBandIdx(int iTotalBandIdx, SxBandIdx *pIdx = NULL) const; //!< Find the zone the band belongs to when iterating from 0 to \ref totalBandCountInAllZones()
    const CxFilterZone* filterZoneAtPicPos(int xPos, int yPos) const; //!< use \ref m_rcActiveArea to get the pattern pos
    const CxBand* bandAtPicPos(int xPos, int yPos, SxBandIdx *pIdx = NULL) const; //!< use \ref m_rcActiveArea to get the pattern pos
    TxBandIdxVector sortedBandIdxs(bool bOnlySelected = false) const;        //!< Indexes to band vector sorted by band peak wavelength across all filter zones
    void removeDataOutsideFilterRange();    //!< Removes band peaks and correction matrixes outside current filter range
    void removeBandCalibrationData();       //!< Clears m_vecCalibData for all bands when no longer needed
    bool isCorrectionMatrixPresent() const; //!< Returns true if the correction to virtual bands is available
    const CxCorrectionMatrix* currentCorrectionMatrix() const;  //!< Returns matrix if the correction to virtual bands for current filter range is available

public:
    bool loadFromCameraXmlFile(const QString &sFilename, QString *psErrorMsg = NULL);   //!< Load from camera XML file
    bool loadFromXml(QDomElement *pRootElement);            //!< Load from XML in metadata or camera XML file
    bool saveToXml(QDomDocument *pXmlDoc, QDomElement *pRootElement) const;   //!< Save to XML compatible with camera XML

    void loadFromHsiApiStruct(const XI_HSI_CAMERA_PARAMS *pParams);     //!< Helper function for reading from xiApi_hsi
    static bool isHsiCamera(XICORE_HANDLE hCamera);

    void generateRgbNirParams();        //!< Initialize the object with parameters defining RGB-NIR camera
    bool isRgbNirCamera() const;        //!< Returns true in case the parameters define RGB-NIR camera
};

#endif // CXHSICAMERAPARAMS_H
