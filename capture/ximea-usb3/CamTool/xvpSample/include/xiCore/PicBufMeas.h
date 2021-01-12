#ifndef CXPICBUFMEAS_H
#define CXPICBUFMEAS_H

#include <QColor>
#include <QLine>
#include <QRect>
#include <QVector>

#include "PicBuf.h"

class CxImageData;

//---------------------------------------------------------------------------
//! \brief Class holding measured histogram and pixel statistics data
class XICORE_API CxHistoData
{
public:
    enum ExHistoDataFlag
    {
        eMeasureHisto       = 0x01, //!< measure histogram (fill m_vecChannelData)
        eMeasureStats       = 0x02, //!< measure pixel statistics (fill m_vecChannelStats)
        eSkipRows           = 0x04, //!< skip rows in large images
        eTreatRawAsMono     = 0x08, //!< RAW image are usually split into channels, keep as mono
    };
    Q_DECLARE_FLAGS(ExHistoDataFlags, ExHistoDataFlag)

    //! \brief Class for measurement of data in the ROI/image per channel
    class CxPixelStats
    {
    public:
        double m_dMin, m_dMax;      //!< min, max
        double m_dMean, m_dStDev;   //!< mean, standard deviation
        double m_dRms;      //!< root mean square: sqrt(sum(x*x)/n)
    };

public:
    quint32 m_uiMaxVal;         //!< max value according to bpc (i.e. 1024 for 10 bit images)
    ExColorFilterArray m_eCfa;  //!< RAW format of the image
    QVector<quint32*> m_vecChannelData; //!< measured histogram per channel, arrays size equal m_uiMaxVal
    QVector<QColor> m_vecChannelColor;  //!< suggested channel colors
    QVector<QString> m_vecChannelName;  //!< suggested channel names
    QVector<CxPixelStats> m_vecChannelStats;    //!< measured pixel statistics
    int m_iDataShift;   //!< histogram data are shifted for bpc > 16bit
    float m_fFloatDataMin, m_fFloatDataMax;  //!< in case of float image, histogram is between data min and max
    bool m_bImgIsRgb;   //!< image was RGB
    QRect m_rc;         //!< rectangle that was measured

public:
    CxHistoData();
    ~CxHistoData();
    CxHistoData& operator=(const CxHistoData &rhs);

    void allocChannels(int iTotalChannels, quint32 uiMaxVal, bool bIncludeStats);
    void freeChannelData();
    void resetChannelData();
    bool isChannelDataCompatible(const CxHistoData &other) const;
    bool isForFloatData() const;
    void calcPixelStats(bool bEnableAlloc = false);

public:
    void sumWithData(const CxHistoData &other);
    void maxWithData(const CxHistoData &other);
    void divideBy(quint32 uiVal);
};

//---------------------------------------------------------------------------
//! \brief Class holding measured line profile data
class XICORE_API CxProfileData
{
public:
    quint32 m_uiMaxVal;         //!< max value according to bpc (i.e. 1024 for 10 bit images)
    int m_iLength;              //!< size of arrays in m_vecChannelData and m_vecChannelStats
    QVector<float*> m_vecChannelData;   //!< measured profile per channel (float because of averaging)
    QVector<QColor> m_vecChannelColor;  //!< suggested channel colors
    QVector<QString> m_vecChannelName;  //!< suggested channel names

public:
    QVector<CxHistoData::CxPixelStats*> m_vecChannelStats; //!< pixel stats for each row/column. Measured only in measureRowsColumnsProfileStats function

public:
    ~CxProfileData();
    void freeChannelData();
};

//---------------------------------------------------------------------------
class XICORE_API CxPicBufMeas
{
public:
    static bool measureHistogram(CxHistoData &rData, const CxImageData *pImageData,
                                 const QRect &rc, CxHistoData::ExHistoDataFlags aFlags);
    static bool measureHistogram(CxHistoData &rData, const SxPicBuf &aPic,
                                 const QRect &rc, CxHistoData::ExHistoDataFlags aFlags);

    static bool measureLineProfile(CxProfileData &rData, const CxImageData *pImageData,
                                   const QLine &line, int iBandSize);

    static bool measureRowsColumnsProfileStats(bool bRows, CxProfileData &rData,
                                               const CxImageData *pImageData, const QRect &rc);

private:
    static void fillMeasChannels(const SxPicBuf &rPic, QVector<QColor> &rvecChannelColor,
                                 QVector<QString> &rvecChannelName, bool bTreatRawAsMono);
};

#endif // CXPICBUFMEAS_H
