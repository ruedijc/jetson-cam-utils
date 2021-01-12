#ifndef CXLUT_H
#define CXLUT_H

#include <xiCoreGlobal.h>
#include <QString>
#include <QVector>

struct SxPicBuf;
class CxImageMetadata;

class XICORE_API CxLut
{
    // picture format specification
public:
    quint32 m_uiSrcBpc;         //!< bits ber component on input
    quint32 m_uiSrcComps;       //!< number of input components
    bool m_bSrcIsRgb;           //!< source image is RGB (thus may skip 4th channel, known colors, etc)
    quint32 m_uiCompMask;       //!< binary mask of input comps idxs to map
    quint32 m_uiDstBpc;         //!< bits ber component on output
    quint32 m_uiDstComps;       //!< number of output components
    bool m_bSingleCompInColor;  //!< show masked channel in its color or in gray
    int m_iPseudoColor;         //!< in case one channel is mapped, use this pseodocolor map (-1 for none)
    quint32 m_clMonoCompColor;  //!< when mono image with m_bSingleCompInColor, use this color

    float m_fFloatDataMin, m_fFloatDataMax; //!< use these min and max for mapping float data instead of those in picture

    // contrast parameters
public:
    class XICORE_API CxCompContrast
    {
    public:
        qint64 m_iSrcMinVal, m_iSrcMaxVal; //!< increase contrast
        qint64 m_iDstMinVal, m_iDstMaxVal; //!< decrease contrast
        double m_dGamma;                //!< gamma during contrast
        CxCompContrast();
        bool isReset(quint32 uiSrcBpc) const;    //!< does it applies contast?
        QString saveToString() const;
        bool loadFromString(const QString &sString);
    };
    QVector<CxCompContrast> m_vecContrast;  //!< curve mapping per compoment. If size==1, same applied to all comps. Pre-created with size 1

    // over- and underexposed pixels
public:
    bool m_bMarkOverexposed, m_bMarkUnderexposed;   //!< mark over- or underexposed pixels?
    quint32 m_clOverexposed, m_clUnderexposed;      //!< color for over- and underexposed pixels (default: red, blue)

    // demosaic (for HyperSpectral images)
public:
    enum ExDemosaicMode
    {
        exdemNone = 0,
        exdemMin  = 1,
        exdemMax  = 2,
        exdemAvg  = 3,
    } m_eDemosaicMode;

protected:
    bool m_bWasAllocatedForRgb;

    enum ExMapMode
    {
        exmmUnset,
        exmmToRgb,              //!< try to convert the picture to RGB just using genering functions (no tables)
        exmmSeparateComps,      //!< map each component separately, each component has its own table
        exmmSeparate3Comps,     //!< like exmmSeparateComps, but use only first 3 components (for RGB32)
        exmmSeparateCompsSameTable,  //!< map each component separately, while using same table for all
        exmmSingleChannelToRgb, //!< color is table for one comp, producing RGB image
    } m_eMapMode;   // how to interpret the mapping table m_pLutVoidData
    bool m_bDoContrastOnFly;    //!< do not allocate arrays for large bpcs, apply contrast on the fly

    void *m_pLutVoidData;
    enum ExLutDataType
    {
        exldtNull,
        exldtUint8Ptr,      //!< mapping components to 8-bit component
        exldtUint16Ptr,     //!< typically mapping single component to RGB16
        exldtUint32Ptr,     //!< typically mapping single component to RGB32
    } m_eLutDataType;

    SxPicBuf *m_pPicDemosaicCache;

    QVector<quint32> m_vecCompColor;    //!< component color in case the image is multichannel (not RGB)

    // initialization
public:
    CxLut();
    CxLut(quint32 uiInBpc, quint32 uiInComps, bool bInIsRgb);
    ~CxLut();

    static quint32 fullMask(quint32 uiComps);
    static quint32 numCompsInMask(quint32 uiMask);
    static int firstCompInMask(quint32 uiMask);

    bool alloc();                       //!< allocate the mapping table
    bool allocRgb(quint32 uiRgbBits);   //!< allocate the mapping table to RGB24, RGB32 or RGB565 format

    CxLut* transformToFormat(quint32 uiInBpc, quint32 uiInComps, bool bInIsRgb) const; //!< create new Lut based on current setting

protected:
    void init();        //!< called from all constructors
    void freeLutData();

    // processing
public:
    bool isCompatibleWith(const SxPicBuf *pDstPic, const SxPicBuf *pSrcPic);
    bool apply(SxPicBuf *pDstPic, const SxPicBuf *pSrcPic, const CxImageMetadata *pSrcMetadata);

protected:
    bool applyOnOneCompToRgb(SxPicBuf *pDstPic, const SxPicBuf *pSrcPic, int iComp, const CxImageMetadata *pSrcMetadata);
    void markOverExposedRgb32(SxPicBuf *pPic);
    void markOverExposedRgb565(SxPicBuf *pPic);

    // pseudocolors
public:
    class XICORE_API CxPseudoColorTable
    {
    public:
        QString m_sName;
        quint32 m_arrValues[256];
        bool loadFromFile(const QString &sFile);
        bool saveToFile(const QString &sFile);
    };

    static bool addPseudoColorLut(const QString &sFile);
    static int pseudoColorLutCount();
    static QString pseudoColorLutName(int iIdx);
    static int indexOfPseudoColorLutName(const QString &sName);

    static void setCustomPseudoColorLutFolder(const QString &sFolderName);
    static QString customPseudoColorLutFolder();
    static void reloadCustomPseudoColorLuts();
};

#endif // CXLUT_H
