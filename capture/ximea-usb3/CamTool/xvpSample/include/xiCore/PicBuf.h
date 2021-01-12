#ifndef PICBUF_H
#define PICBUF_H


#include "xiCoreGlobal.h"
#include <QtEndian>

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN

#define XICORE_RGBA_ALPHA 3
#define XICORE_RGBA_RED   2
#define XICORE_RGBA_GREEN 1
#define XICORE_RGBA_BLUE  0
#define XICORE_RGBA_RED_SHIFT       16
#define XICORE_RGBA_GREEN_SHIFT     8
#define XICORE_RGBA_BLUE_SHIFT      0
#define XICORE_RGBA_ALPHA_SHIFT     24
#define XICORE_RGBA_RED_MASK        0x00FF0000
#define XICORE_RGBA_GREEN_MASK      0x0000FF00
#define XICORE_RGBA_BLUE_MASK       0x000000FF
#define XICORE_RGBA_ALPHA_MASK      0xFF000000

#elif Q_BYTE_ORDER == Q_BIG_ENDIAN

#define XICORE_RGBA_ALPHA 3
#define XICORE_RGBA_RED   0
#define XICORE_RGBA_GREEN 1
#define XICORE_RGBA_BLUE  2
#define XICORE_RGBA_RED_SHIFT       0
#define XICORE_RGBA_GREEN_SHIFT     8
#define XICORE_RGBA_BLUE_SHIFT      16
#define XICORE_RGBA_ALPHA_SHIFT     24
#define XICORE_RGBA_RED_MASK        0x000000FF
#define XICORE_RGBA_GREEN_MASK      0x0000FF00
#define XICORE_RGBA_BLUE_MASK       0x00FF0000
#define XICORE_RGBA_ALPHA_MASK      0xFF000000

#else
#error Byte order is not defined.
#endif

#define DX_FLOATMINMAX_INVALID  -1.0f

class QObject;

/*!
 \brief Values that represents the data types of image pixels.

 \enum ExImageDataType
*/
enum ExImageDataType
{
    extypeUnknown,   //!< Unknown type
    extypeInt8,      //!< qint8
    extypeUInt8,     //!< quint8
    extypeInt16,     //!< qint16
    extypeUInt16,    //!< quint16
    extypeInt32,     //!< qint32
    extypeUInt32,    //!< qint32
    extypeFloat,     //!< float
    extypeDouble,    //!< double
    extypePacked565, //!< for RGB565 - 5 bits for red and blue, 6 for green
};


/*!
 \brief Values that represents how the image data are stored.

 \enum ExDataStorageFormat
*/
enum ExDataStorageFormat
{
    exdataStorageUnknown, //!< Unknown format.
    exdataStoragePixel,   //!< All channel values for each pixel are clustered and stored consecutively (e.g. RGB, RGBA)
    exdataStoragePlanar,  //!< Each channel is stored in separated plane.
    exdataStoragePixelPacked //!< The data are packed in transport data format.
};

enum ExColorFilterArray
{
    excfaNone    = 0,       // do not change these constants, they are saved to file as int
    excfaRG      = 1,
    excfaGR      = 2,
    excfaGB      = 3,
    excfaBG      = 4,
    excfaCMYG    = 5,   // AK Sony sens
    excfaRGR     = 6,   // 2R+G readout
    excfaHyperSpectral = 7, //!< HSI sensor, use \ref CxHsiCameraParams to get the pattern dimensions (4x4, 5x5)
};

/*!
 \brief Information about picture buffer.

 \class SxPicBufInfo PicBuf.h "xiCore/inc/PicBuf.h"
*/
struct SxPicBufInfo
{    
    quint32 m_uiWidth;              /*!< Width in pixels. */
    quint32 m_uiHeight;             /*!< Height in pixels. */
    quint32 m_uiStride;             /*!< Line width in bytes. */
    quint32 m_uiChannels;           /*!< Number of color channels. */
    quint32 m_uiBpc;                /*!< Bits per channel. It is valid to have m_uiBpc=8 for m_eDataType=extypeUInt16. For pixelPacked storage format, this is bits after unpacking. */
	quint32 m_uiBlackLevel;         /*!< Image data blacl level */
    ExImageDataType m_eDataType;    /*!< Data type of the image. For pixelPacked storage format, this is data type after unpacking. */
    ExDataStorageFormat m_eFormat;  /*!< Format of the image. */
    ExColorFilterArray m_eColorFilterArray;  //!< Bayer pattern
    bool m_bIsRgb;                  //!< treat multi-channel image as RGB (to distiguish between RAW4X and RGB32)
    //! Default constructor.
    SxPicBufInfo():m_uiWidth(0), m_uiHeight(0), m_uiStride(0),
        m_uiChannels(0), m_uiBpc(0),
        m_eDataType(extypeUnknown), m_eFormat(exdataStoragePixel),
        m_eColorFilterArray(excfaNone), m_bIsRgb(false)
    {}

    //! Returns the number of bytes per pixel.
    inline quint32 bytesPerPixel() const
    {
        switch (m_eDataType)
        {
        case extypeInt8:
        case extypeUInt8: return m_uiChannels;
        case extypeInt16:
        case extypeUInt16: return 2*m_uiChannels;
        case extypeInt32:
        case extypeUInt32:
        case extypeFloat:  return 4*m_uiChannels;
        case extypeDouble: return 8*m_uiChannels;
        case extypePacked565: return 2*m_uiChannels;
        default: break;
        }
        return m_uiChannels;
    }
};


/**
 * \brief Picture buffer.
 */
struct XICORE_API SxPicBuf : public SxPicBufInfo
{

    void *m_pData;          /*!< Pointer to pixel data. */
    size_t m_uiAllocSize;   /*!< The allocated size of data (i.e. available size) */

    enum ExDataOwner
    {
        edoNone,        /*!< You should delete the m_pData yourself */
        edoThisPicBuf,  /*!< Data belong to this picbuf. */
        edoMemoryPool,  /*!< Data were allocated inside memory pool, \ref m_hMemoryPoolOwner */
    } m_eDataOwner;

    TxMemPoolHandle m_hMemoryPoolOwner;  /*!< Data is part of this memory pool. Do not free. */
    QObject *m_pMemoryPoolClient;        /*!< Who allocated the buffer in the pool (may have requests). */

    qint32 m_iFrameNo;      /*!< Frame number is set when image comes from camera or an image sequence. Else zero.*/
    quint64 m_uiTimeStamp;  /*!< The timestamp in micro seconds.*/
    qint32 m_iXiApiFrameNo; /*!< Frame number as set by xiApi. May be different to our own counter m_iFrameNo. -1 for undefined. */
    float m_fFloatDataMin;  /*!< Cached minimum data value for float data. When both min and max equal DX_FLOATMINMAX_INVALID, they are not calculated yet. */
    float m_fFloatDataMax;  /*!< Cached maximum data value for float data. When both min and max equal DX_FLOATMINMAX_INVALID, they are not calculated yet. */

public:
    SxPicBuf();         //!< Default constructor, initialies all to zero.

    size_t dataSize() const;                        //!< Returns the size of pixel data (\ref m_pData).
    void setPicFormat(const SxPicBufInfo &rInfo);   //!< Sets the widht, height, format accoring to the rInfo parameter.
    bool isFromMemoryPool(TxMemPoolHandle hMemPool) const;  //!< Data comes from some memory pool.

    void setAsShallowCopyOf(const SxPicBuf &rOther);    //!< Use with caution while hacking. Should not be needed in 99% of cases
    void copyPicAdditionalData(const SxPicBuf &rOther);       //!< Copy frame number, time stamps and other info values

protected:
    SxPicBuf(const SxPicBuf &rOther);     //!< Copy constructor is forbidden to use. See \ref operator=.
    SxPicBuf& operator=(const SxPicBuf &rOther);     //!< operator= is forbidden to use. It is dangerous to copy pointer allocated data.
};

#endif // PICBUF_H
