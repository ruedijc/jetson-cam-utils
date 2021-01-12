#ifndef XIPICBUF_H
#define XIPICBUF_H

#include <xiCoreGlobal.h>
#include <PicBuf.h>
#include <QRect>

//! Padding of image buffers.
#define XICORE_IMG_PADDING 256
//! Padding of image buffers used by video endoders.
#define XICORE_VIDEO_PADDING 4

#define XICORE_ALIGN_TO(length, padding) ((((length)+padding-1)/padding)*padding)

//! Align picture data row size with our internal padding (\ref XICORE_IMG_PADDING)
#define XICORE_ALIGN_IMG_STRIDE(rowLength) XICORE_ALIGN_TO(rowLength, XICORE_IMG_PADDING)

//! Check whether the two images have the same size including the number of channels and bytes per pixel.
#define CHECK_PICBUFS_SAME_SIZE(pic1, pic2) \
    (((pic1).m_uiWidth == (pic2).m_uiWidth) && \
    ((pic1).m_uiHeight == (pic2).m_uiHeight) && \
    ((pic1).m_uiChannels == (pic2).m_uiChannels) && \
    ((pic1).m_eDataType == (pic2).m_eDataType) && \
    ((pic1).bytesPerPixel() == (pic2).bytesPerPixel()))

//! Checks whether the two images have the same width and height.
#define CHECK_PIC_SAME_WxH(pic1, pic2)\
    (((pic1).m_uiWidth)==((pic2).m_uiWidth)&&((pic1).m_uiHeight)==((pic2).m_uiHeight))

//! Checks whether the two images have the same size and format.
#define CHECK_PIC_SAME(pic1, pic2)\
    (((pic1).m_uiWidth == (pic2).m_uiWidth) && \
    ((pic1).m_uiHeight == (pic2).m_uiHeight) && \
    ((pic1).m_uiChannels == (pic2).m_uiChannels) && \
    ((pic1).m_uiBpc == (pic2).m_uiBpc) && \
    ((pic1).m_eDataType == (pic2).m_eDataType) && \
    ((pic1).m_eFormat == (pic2).m_eFormat) && \
    ((pic1).m_eColorFilterArray == (pic2).m_eColorFilterArray))

//! Checks whether the two images have the same size and format. The color filter array is not checked.
#define CHECK_PIC_SAME_NOFILTER(pic1, pic2)\
    (((pic1).m_uiWidth == (pic2).m_uiWidth) && \
    ((pic1).m_uiHeight == (pic2).m_uiHeight) && \
    ((pic1).m_uiChannels == (pic2).m_uiChannels) && \
    ((pic1).m_uiBpc == (pic2).m_uiBpc) && \
    ((pic1).m_eDataType == (pic2).m_eDataType) && \
    ((pic1).m_eFormat == (pic2).m_eFormat))

#define CHECK_PIC_MONO(pic)\
    (((pic).m_uiChannels==1) && ((pic).m_eColorFilterArray==excfaNone))

#define CHECK_PIC_MONO8(pic)\
    (((pic).m_uiChannels==1)&&((pic).bytesPerPixel()==1) && ((pic).m_eColorFilterArray==excfaNone))

#define CHECK_PIC_MONO16(pic)\
    (((pic).m_uiChannels==1)&&((pic).bytesPerPixel()==2)&&((pic).m_eColorFilterArray==excfaNone))

#define CHECK_PIC_GRAY(pic)\
    ((pic).m_uiChannels==1)

#define CHECK_PIC_GRAY8(pic)\
    (((pic).m_uiChannels==1)&&((pic).bytesPerPixel()==1))

#define CHECK_PIC_GRAY16(pic)\
    (((pic).m_uiChannels==1)&&((pic).bytesPerPixel()==2))

#define CHECK_PIC_GRAY32(pic)\
    (((pic).m_uiChannels==1)&&((pic).m_eDataType==extypeUInt32))

#define CHECK_PIC_RGB32(pic)\
    (((pic).m_uiChannels==4)&&((pic).bytesPerPixel()==4)&&((pic).m_eFormat==exdataStoragePixel)&&((pic).m_bIsRgb))

#define CHECK_PIC_RGB24(pic)\
    (((pic).m_uiChannels==3)&&((pic).bytesPerPixel()==3)&&((pic).m_eFormat==exdataStoragePixel)&&((pic).m_bIsRgb))

#define CHECK_PIC_RGB64(pic)\
    (((pic).m_uiChannels==4)&&((pic).bytesPerPixel()==8)&&((pic).m_eFormat==exdataStoragePixel)&&((pic).m_bIsRgb))

#define CHECK_PIC_RGB48(pic)\
    (((pic).m_uiChannels==3)&&((pic).bytesPerPixel()==6)&&((pic).m_eFormat==exdataStoragePixel)&&((pic).m_bIsRgb))

#define CHECK_PIC_RGB565(pic)\
    (((pic).m_uiChannels==1)&&((pic).m_eDataType==extypePacked565))

#define CHECK_PIC_RGBPLANAR(pic) \
    (((pic).m_uiChannels==3)&&((pic).bytesPerPixel()==3)&&((pic).m_eFormat==exdataStoragePlanar))

#define CHECK_PIC_RGB16PLANAR(pic) \
    (((pic).m_uiChannels==3)&&((pic).bytesPerPixel()==6)&&((pic).m_eFormat==exdataStoragePlanar))

#define CHECK_PIC_RAW8(pic) \
    (((pic).m_uiChannels==1) && ((pic).bytesPerPixel()==1) && ((pic).m_eColorFilterArray!=excfaNone))

#define CHECK_PIC_RAW16(pic) \
    (((pic).m_uiChannels==1) && ((pic).bytesPerPixel()==2) && ((pic).m_eColorFilterArray!=excfaNone))

#define CHECK_PIC_RAW(pic) \
    (((pic).m_uiChannels==1) && ((pic).m_eColorFilterArray!=excfaNone) && ((pic).m_eFormat!=exdataStoragePixelPacked))

#define CHECK_PIC_RAW8X2(pic) \
    (((pic).m_uiChannels==2) && ((pic).bytesPerPixel()==2) && !((pic).m_bIsRgb))

#define CHECK_PIC_RAW8X4(pic) \
    (((pic).m_uiChannels==4) && ((pic).bytesPerPixel()==4) && !((pic).m_bIsRgb))

#define CHECK_PIC_RAW16X2(pic) \
    (((pic).m_uiChannels==2) && ((pic).bytesPerPixel()==4) && !((pic).m_bIsRgb))

#define CHECK_PIC_RAW16X4(pic) \
    (((pic).m_uiChannels==4) && ((pic).bytesPerPixel()==8) && !((pic).m_bIsRgb))

#define CHECK_PIC_RAW32(pic) \
    (((pic).m_uiChannels==1) && ((pic).m_eDataType==extypeUInt32) && ((pic).m_eColorFilterArray!=excfaNone))

#define CHECK_PIC_FLOAT(pic) \
    ((pic).m_eDataType == extypeFloat)

#define CHECK_PIC_RAW32FLOAT(pic) \
    (((pic).m_uiChannels==1) && ((pic).m_eDataType==extypeFloat) && ((pic).m_eColorFilterArray!=excfaNone))

#define CHECK_PIC_PACKED(pic) \
    ((pic).m_eFormat == exdataStoragePixelPacked)

#define SET_PIC_RGB32(pic) \
    (pic).m_uiChannels=4;\
    (pic).m_uiBpc=8;\
    (pic).m_eDataType=extypeUInt8;\
    (pic).m_eFormat=exdataStoragePixel;\
    (pic).m_eColorFilterArray=excfaNone;\
    (pic).m_bIsRgb=true;


//---------------------------------------------------------------------------
// RGB macros
#define DX_RGB_ALPHA_MASK      0xFF000000
#define DX_RGB(r,g,b)          ((quint32)(((quint8)(r)|((quint16)((quint8)(g))<<8))|(((quint16)(quint8)(b))<<16)|DX_RGB_ALPHA_MASK))
#define DX_GetRValue(rgb)      ((quint8)(rgb))
#define DX_GetGValue(rgb)      ((quint8)(((quint16)(rgb)) >> 8))
#define DX_GetBValue(rgb)      ((quint8)((rgb)>>16))

// for converting to RGB16
#define DX_RGB16_GREEN_MASK 0x7e0 // 0000011111100000
#define DX_RGB16_RED_MASK 0xf800  // 1111100000000000
#define DX_RGB16_BLUE_MASK 0x1f   // 0000000000011111
#define DX_CONVERT_888RGB_TO_565RGB(r, g, b) (quint16)((b >> 3) | (((quint16)g << 3) & DX_RGB16_GREEN_MASK) | (((quint16)r << 8) & DX_RGB16_RED_MASK))
#define DX_RGB565_GetRValue(rgb565) ((quint8)(((rgb565)&DX_RGB16_RED_MASK)>>11))
#define DX_RGB565_GetGValue(rgb565) ((quint8)(((rgb565)&DX_RGB16_GREEN_MASK)>>5))
#define DX_RGB565_GetBValue(rgb565) ((quint8)((rgb565)&DX_RGB16_BLUE_MASK))

// big endian to little endian or back
#define DX_SWAP16(num) (((num & 0xff00) >> 8) | ((num & 0xff) << 8))
#define DX_SWAP32(num) (((num & 0xff000000) >> 24) | ((num & 0x00ff0000) >> 8) | ((num & 0x0000ff00) << 8) | ((num & 0x000000ff) << 24))
#define DX_SWAP64(num) ((((num) & 0xff00000000000000ull) >> 56) |\
                        (((num) & 0x00ff000000000000ull) >> 40) |\
                        (((num) & 0x0000ff0000000000ull) >> 24) |\
                        (((num) & 0x000000ff00000000ull) >> 8 ) |\
                        (((num) & 0x00000000ff000000ull) << 8 ) |\
                        (((num) & 0x0000000000ff0000ull) << 24) |\
                        (((num) & 0x000000000000ff00ull) << 40) |\
                        (((num) & 0x00000000000000ffull) << 56))

//---------------------------------------------------------------------------
class XICORE_API CxPicBufAPI
{
public:

    //! Returns number of bytes occupied by given image data type.
    static size_t ImageTypeSize(ExImageDataType eDataType);

/*!
 \brief Allocates and initializes the \ref SxPicBuf structure.
 \remarks If the \p uiStride is zero then it will be calculated. If the \p bResetData
 is true then the allocated data will be set to zero. The allocation is preformed using the malloc() function from heap.
*/
    static bool AllocPicBufFromHeap(SxPicBuf &pic,
        quint32 uiWidth,
        quint32 uiHeight,
        quint32 uiChannels,
        ExImageDataType eDataType,
        quint32  uiStride = 0,
        quint32 uiColorBitDepth = 0,
        ExDataStorageFormat eFormat = exdataStoragePixel,
        ExColorFilterArray eCfa = excfaNone,
        bool bIsRgb = false,
        bool bResetData = false);


/*!
 \brief Allocates and initializes the \ref SxPicBuf using the initialized \ref SxPicBufInfo structure.
The allocation is preformed using the malloc() function from heap.
*/
    static bool AllocPicBufFromHeap(SxPicBuf &aPic, const SxPicBufInfo &aInfo, bool bResetData = false);


/*!
 \brief Allocates the \p aPic from the memory pool given by \p hPool handle using the
 initialized \p aInfo structure.
 \remarks If the \p hPool is NULL, then the image will be allocated using \ref AllocPicBufFromHeap()
*/
    static bool AllocPicBufFromPool(TxMemPoolHandle hPool, QObject *pMemPoolClient, SxPicBuf &aPic,
                                    const SxPicBufInfo &aInfo, bool bResetData = false);

/*!
 \brief Deallocates the given image depending on SxPicBuf::m_bDataOwner.

 \remarks If the SxPicBuf::m_eDataOwner member is not SxPicBuf::edoThisPicBuf then the function
 tries to release the buffer calling CxMemoryManager::freePicBuf().
 The member SxPicBuf::m_pData is always set to NULL.
*/
    static void FreePicBuf(SxPicBuf &pic, bool bReset = true);

/*!
 \brief Copies the \p srcPic to \p dstPic. The destination image must be correctly
 preallocated otherwise the function fails.
*/
    static bool CopyPicBuf(SxPicBuf &dstPic, const SxPicBuf &srcPic, bool bIncludeMetadata = true);

/*!
 \brief Copies the rectangle from \p srcPic to \p dstPic.
*/
    static bool CopyRect(SxPicBuf &dstPic, quint32 dx, quint32 dy, const SxPicBuf &srcPic, quint32 sx, quint32 sy, quint32 w, quint32 h);

/*!
 \brief Checks if the data in N-bit image are really N-bit. E.g. returns false when value
 1234 is stored in 10-bit image, where max value is 1023.
*/
    static bool CheckPicValidData(const SxPicBuf &pic);

/*!
 \brief Attepmt to convert source image to RGB, depending on the src and dst format.
*/
    static bool ConvertPicToRGB(const SxPicBuf &picSrc, SxPicBuf &picDst);

/*!
 \brief Converts the source MONO8 or RAW8 picbuf to picbuf in RGB32 format (0x00RRGGBB).
*/
    static bool ConvertGray8ToRGB32(const SxPicBuf &picMono8, SxPicBuf &picRgb32);

/*!
 \brief Converts the source MONO8 or RAW8 picbuf to picbuf in RGB24 format (0xRRGGBB).
*/
    static bool ConvertGray8ToRGB24(const SxPicBuf &picMono8, SxPicBuf &picRgb24);

/*!
 \brief Converts the source MONO8 or RAW8 picbuf to picbuf in RGB565 format.
*/
    static bool ConvertGray8ToRGB565(const SxPicBuf &picMono8, SxPicBuf &picRgb565);

/*!
 \brief Converts the source MONO16 or RAW16 picbuf to picbuf in RGB32 format (0x00RRGGBB).
*/
    static bool ConvertGray16ToRGB32(const SxPicBuf &picMono16, SxPicBuf &picRgb32);

/*!
 \brief Converts the source MONO16 or RAW16 picbuf to picbuf in RGB24 format (0xRRGGBB).
*/
    static bool ConvertGray16ToRGB24(const SxPicBuf &picMono16, SxPicBuf &picRgb24);

    //TODO allow to convert RAW16 to RAW8?
/*!
 \brief Converts the source MONO16 or RAW16 picbuf to MONO8 or RAW8 picbuf.
*/
    static bool ConvertGray16ToGray8(const SxPicBuf &picMono16, SxPicBuf &picMono8);

/*!
 \brief Converts the source MONO16 or RAW16 picbuf to picbuf in RGB565 format.
*/
    static bool ConvertGray16ToRGB565(const SxPicBuf &picMono16, SxPicBuf &picRgb565);

/*!
 \brief Converts the source RGB24 (0xRRGGBB) picbuf to picbuf in RGB32 format (0x00RRGGBB).
*/
    static bool ConvertRGB24ToRGB32(const SxPicBuf &picRgb24, SxPicBuf &picRgb32);

/*!
 \brief Converts the source RGB24 (0xRRGGBB) picbuf to picbuf in RGB565 format.
*/
    static bool ConvertRGB24ToRGB565(const SxPicBuf &picRgb24, SxPicBuf &picRgb565);

/*!
 \brief Converts the source RGB32 (0x00RRGGBB) picbuf to picbuf in RGB24 format (0xRRGGBB).
*/
    static bool ConvertRGB32ToRGB24(const SxPicBuf &picRgb32, SxPicBuf &picRgb24);

/*!
 \brief Converts the source RGB32 (0x00RRGGBB) picbuf to picbuf in RGB565 format.
*/
    static bool ConvertRGB32ToRGB565(const SxPicBuf &picRgb32, SxPicBuf &picRgb565);

/*!
 \brief Converts the source RGB Planar picbuf to picbuf in RGB32 format (0x00RRGGBB).
*/
    static bool ConvertRGBPlanarToRGB32(const SxPicBuf &picRgbPlanar, SxPicBuf &picRgb32);

/*!
 \brief Converts the source RGB Planar picbuf to picbuf in RGB24 format (0xRRGGBB).
*/
    static bool ConvertRGBPlanarToRGB24(const SxPicBuf &picRgbPlanar, SxPicBuf &picRgb24);

/*!
 \brief Converts the source RGB Planar picbuf to picbuf in RGB565 format
*/
    static bool ConvertRGBPlanarToRGB565(const SxPicBuf &picRgbPlanar, SxPicBuf &picRgb565);

    static bool ConvertRGB48ToRGB24(const SxPicBuf &picRgb48, SxPicBuf &picRgb24);    //! \brief Converts the source RGB48 picbuf to picbuf in RGB24 format.
    static bool ConvertRGB48ToRGB32(const SxPicBuf &picRgb48, SxPicBuf &picRgb32);    //! \brief Converts the source RGB48 picbuf to picbuf in RGB32 format.
    static bool ConvertRGB48ToRGB565(const SxPicBuf &picRgb48, SxPicBuf &picRgb565);    //! \brief Converts the source RGB48 picbuf to picbuf in RGB565 format.
    static bool ConvertRGB64ToRGB24(const SxPicBuf &picRgb64, SxPicBuf &picRgb24);    //! \brief Converts the source RGB64 picbuf to picbuf in RGB24 format.
    static bool ConvertRGB64ToRGB32(const SxPicBuf &picRgb64, SxPicBuf &picRgb32);    //! \brief Converts the source RGB64 picbuf to picbuf in RGB32 format.
    static bool ConvertRGB64ToRGB565(const SxPicBuf &picRgb64, SxPicBuf &picRgb565);    //! \brief Converts the source RGB64 picbuf to picbuf in RGB565 format.
    static bool ConvertRGB64ToRGB48(const SxPicBuf &picRgb64, SxPicBuf &picRgb48);      //! \brief Converts the source RGB64 picbuf to picbuf in RGB48 format.
    static bool ConvertRGB16PlanarToRGB24(const SxPicBuf &picRgbPlanar, SxPicBuf &picRgb24);    //! \brief Converts the source RGB16 Planar picbuf to picbuf in RGB24 format.
    static bool ConvertRGB16PlanarToRGB32(const SxPicBuf &picRgbPlanar, SxPicBuf &picRgb32);    //! \brief Converts the source RGB16 Planar picbuf to picbuf in RGB32 format.
    static bool ConvertRGB16PlanarToRGB565(const SxPicBuf &picRgbPlanar, SxPicBuf &picRgb565);    //! \brief Converts the source RGB16 Planar picbuf to picbuf in RGB565 format.
    static bool ConvertMultiChannelToRGB24(const SxPicBuf &picMCh, SxPicBuf &picRgb24);      //! \brief Converts the source RAW8/16X2/4 picbuf to picbuf in RGB24 format.
    static bool ConvertMultiChannelToRGB32(const SxPicBuf &picMCh, SxPicBuf &picRgb32);      //! \brief Converts the source RAW8/16X2/4 picbuf to picbuf in RGB32 format.
    static bool ConvertMultiChannelToRGB565(const SxPicBuf &picMCh, SxPicBuf &picRgb565);      //! \brief Converts the source RAW8/16X2/4 picbuf to picbuf in RGB565 format.

    static bool ConvertGray32ToRGB24(const SxPicBuf &pic, SxPicBuf &picRgb24);      //! \brief Converts the source RAW32 picbuf to picbuf in RGB24 format.
    static bool ConvertGray32ToRGB32(const SxPicBuf &pic, SxPicBuf &picRgb32);      //! \brief Converts the source RAW32 picbuf to picbuf in RGB32 format.
    static bool ConvertGray32ToRGB565(const SxPicBuf &pic, SxPicBuf &picRgb565);      //! \brief Converts the source RAW32 picbuf to picbuf in RGB565 format.
    static bool ConvertFloatToRGB24(const SxPicBuf &pic, SxPicBuf &picRgb24);      //! \brief Converts the source float picbuf to picbuf in RGB24 format.
    static bool ConvertFloatToRGB32(const SxPicBuf &pic, SxPicBuf &picRgb32);      //! \brief Converts the source float picbuf to picbuf in RGB32 format.
    static bool ConvertFloatToRGB565(const SxPicBuf &pic, SxPicBuf &picRgb565);      //! \brief Converts the source float picbuf to picbuf in RGB565 format.

/*!
 \brief Converts the source FFT images to a picbuf in the format of RGB.
 \remarks In fact the function calculates an amplitued of the FFT and converts it to RGB.
 Internally it calls \ref ConvertFFTToRGB32() or \ref ConvertFFTToRGB24() or \ref ConvertFFTToRGB565().
*/
    static bool ConvertFFTToRGB(const SxPicBuf &picRe, const SxPicBuf &picIm, SxPicBuf &picRGB, SxPicBuf *pTmp = NULL);

/*!
 \brief Converts the source FFT images to a picbuf in the format of RGB32.
 \remarks In fact the function calculates an amplitued of the FFT and converts it to RGB32.
*/
    static bool ConvertFFTToRGB32(const SxPicBuf &picRe, const SxPicBuf &picIm, SxPicBuf &picRGB32, SxPicBuf *pTmp = NULL);

/*!
 \brief Converts the source FFT images to a picbuf in the format of RGB24.
 \remarks In fact the function calculates an amplitued of the FFT and converts it to RGB24.
*/
    static bool ConvertFFTToRGB24(const SxPicBuf &picRe, const SxPicBuf &picIm, SxPicBuf &picRGB24, SxPicBuf *pTmp = NULL);

/*!
 \brief Converts the source FFT images to a picbuf in the format of RGB565.
 \remarks In fact the function calculates an amplitued of the FFT and converts it to RGB565.
*/
    static bool ConvertFFTToRGB565(const SxPicBuf &picRe, const SxPicBuf &picIm, SxPicBuf &picRGB565, SxPicBuf *pTmp = NULL);

/*!
 \brief Converts the source picbuf to picbuf with uint32 or uin16 type and same number of components.
 \remarks Suitable for averaging, see \ref Add and \ref DivideByValue.
*/
    static bool ConvertPicToDifBpc(const SxPicBuf &picSrc, SxPicBuf &picDst, bool bStretchValues = false);

/*!
 \brief Swaps the red and green channels.
*/
    static void SwapRedAndBlue(SxPicBuf &picRgb);

/*!
 \brief Swaps all channels (i.e. ARGB -> BGRA).
*/
    static void SwapAllChannels(SxPicBuf &pic);

/*!
 \brief From little to big endian or back efficiently
*/
    static void SwapBytes(SxPicBuf &pic);

/*!
 \brief Sets color to one component
*/
    static void FillChannelWithValue(SxPicBuf &pic, quint32 uiChannel, quint16 uiValue);

/*!
 \brief Sets alpha byte to 0xFF, as this is required by Qt in format QImage::Format_RGB32
*/
    static void ResetRGB32AlphaByte(SxPicBuf &pic);

/*!
 \brief The function reverses the row order of the image.
 \warning The function does not change the eventual SxPicBuf::m_eColorFilterArray for RAW images!!!
*/
    static bool ReverseRowOrder(SxPicBuf &pic);


/*!
 \brief The function reverses the row order of the memory block.
*/
    static bool ReverseMemRowOrder(quint8 *pData, uint uiHeight, uint uiStride);

/*!
 \brief The function reverses the order of columns of the image.
 \warning The function does not change the eventual SxPicBuf::m_eColorFilterArray for RAW images!!!
*/
    static bool ReverseColOrder(SxPicBuf &pic);

/*!
 \brief The function detect the color bit depth of the image (per channel). Possible
 values that can be returned: 1, 8, 10, 12, 14, 16.

 \remarks The function find maximum value in each channel and from the total maximum
 will quess the color bit depth. The color bit depth will be detected on images with 1 or 2
 bytes per channel only.
*/
    static bool DetectColorBitDepth(const SxPicBuf &picBuf, uint &uiBitDepth);

/*!
 \brief Resizes the \p srcPic to the size given by \p dstPic. Both images must have
 the same image format. Resizing done using bilinear algorithm.
*/
    static bool Resize(const SxPicBuf &srcPic, SxPicBuf &dstPic);

/*!
 \brief Resizes the rectangular section within \p srcPic to the rectangle within \p dstPic.
 Both images must have the same image format. Resizing done using bilinear algorithm.
*/
    static bool ResizeRect(const SxPicBuf &srcPic, quint32 sx, quint32 sy, quint32 sw, quint32 sh,
                           SxPicBuf &dstPic, quint32 dx, quint32 dy, quint32 dw, quint32 dh);

/*!
 \brief Shift pixel bits right (\p iShift > 0) or left (\p iShift < 0).
*/
    static bool ShiftBits(SxPicBuf &srcPic, int iShift);

/*!
 \brief Subtracts values of pic2 from pic1, leaving the result in pic1
*/
    static bool Subtract(SxPicBuf &pic1, const SxPicBuf &pic2);

/*!
 \brief Adds values of picToAdd to pic1, leaving the result in pic1. Pic1 can be Uint32
*/
    static bool Add(SxPicBuf &pic1, const SxPicBuf &picToAdd);

/*!
 \brief Divides value in src picture by constant. picSrc can be Uint32
*/
    static bool DivideByValue(SxPicBuf &picSrc, int iDivideBy);

/*!
 \brief Divides value in src picture by constant, storing result to dst. picSrc can be Uint32
*/
    static bool DivideByValue(const SxPicBuf &picSrc, int iDivideBy, SxPicBuf &picDst);

/*!
 \brief Extracts one channel from source image. Dst image must be mono with same size as src.
*/
    static bool ExtractChannel(const SxPicBuf &picSrc, SxPicBuf &picDst, const quint32 uiComp);

/*!
 \brief Returns minimum and maximum float value found in the float picture
*/
    static bool CalcFloatDataMinMax(const SxPicBuf &pic, float &fMin, float &fMax, bool bUseCache = true);

/*!
 \brief Applies the "min" filter on the mosaic image (RAW or HSI).
 \remarks The output image can have either the size of:
  - [picSrc.m_uiWidth, picSrc.m_uiHeight] for interpolated image,
  - [rcSrc.width()/iMosaicWidth, rcSrc.height()/iMosaicHeight].
  For interpolated image the inactive area of the input image is copied to the output image.
*/
    static bool FilterMinOnMosaicPicBuf(const SxPicBuf &picSrc, const QRect &rcSrc, int iMosaicWidth, int iMosaicHeight, SxPicBuf &picDst);

/*!
 \brief Applies the "max" filter on the mosaic image (RAW or HSI).
 \remarks The output image can have either the size of:
  - [picSrc.m_uiWidth, picSrc.m_uiHeight] for interpolated image,
  - [rcSrc.width()/iMosaicWidth, rcSrc.height()/iMosaicHeight].
  For interpolated image the inactive area of the input image is copied to the output image.
*/
    static bool FilterMaxOnMosaicPicBuf(const SxPicBuf &picSrc, const QRect &rcSrc, int iMosaicWidth, int iMosaicHeight, SxPicBuf &picDst);
/*!
 \brief Applies the "average" filter on the mosaic image (RAW or HSI).
 \remarks The output image can have either the size of:
  - [picSrc.m_uiWidth, picSrc.m_uiHeight] for interpolated image,
  - [rcSrc.width()/iMosaicWidth, rcSrc.height()/iMosaicHeight].
  For interpolated image the inactive area of the input image is copied to the output image.
*/
    static bool FilterAvgOnMosaicPicBuf(const SxPicBuf &picSrc, const QRect &rcSrc, int iMosaicWidth, int iMosaicHeight, SxPicBuf &picDst);

/*!
 \brief Applies the "normalization" filter on the mosaic image (RAW or HSI). The output image must have the same size as input image.
 \remarks The normalization is performed per hyperpixel i.e. the hyperpixel's intensities are normalized to <0; maxIntensity>.
 An eventual inactive area of the input image is copied to the output image.
*/
    static bool FilterNormalizeOnMosaicPicBuf(const SxPicBuf &picSrc, const QRect &rcSrc, int iMosaicWidth, int iMosaicHeight, SxPicBuf &picDst);

/*!
  \brief Performs a median filterring on the \p picSrc with given \p kernelRadius.
  \remarks The kernel size used by the algorithm is (\p kernelRadius*2 + 1).
  The function operates with 8,10,12,14 and 16bit RGB or MONO images. The mosaic images (HSI, RAW) are not supported.
    <p><b>Temporary buffer \p pBuffer<b></p>
    If the \p pBuffer is NULL then the function allocates and frees it internally. For batch processing it is recommended to
    prealloc it before the batch starts. For allocation of the buffer use \ref MedianAllocBuffer()
  */
    static bool Median(const SxPicBuf &picSrc,
            SxPicBuf &picDst,
            int iKernelRadius,
            void *pBuffer = NULL);
/*!
 \brief Allocates a temporary buffer used by \ref Median() function.
 */
    static bool MedianAllocBuffer(
            const SxPicBufInfo &picSrc,
            int iKernelRadius,
            void **pBuffer);

/*!
 \brief Frees the buffer allocated by \ref MedianAllocBuffer().
 */
    static void MedianFreeBuffer(void *pBuffer);
};

#endif // XIPICBUF_H
