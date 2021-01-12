#include <qmath.h>

#include "BayerPseudoColorsChnbl.h"

#include <ImageData.h>
#include <PicBufAPI.h>
#include <PicBufMacros.h>

namespace {
	//! Precalculated channel values for conversion from 16bit image to 8bit image
	static QMap<uint, quint8* > s_mapPrecalc16to8;
	static QMutex s_lock;

	//auto release the s_mapPrecalc16to8
	struct free_precalc
	{
		~free_precalc()
		{
			QMap<uint, quint8* >::iterator it;
			for(it = s_mapPrecalc16to8.begin(); it != s_mapPrecalc16to8.end(); it++)
			{
				delete [] it.value();
			}
		}
	};
	free_precalc _fp;

	//-----------------------------------------------------------------
	//Precalculation of 16bit pixel values to 8bit for given color depth.
	static const quint8* GetPrecalc16to8(uint uiColorDepth)
	{
		QMutexLocker lock(&s_lock);
		if(uiColorDepth > 16)
		{
			return NULL;
		}
		if(s_mapPrecalc16to8.contains(uiColorDepth))
		{
			return s_mapPrecalc16to8[uiColorDepth];
		}
		else
		{
			int iMax16 = (int)qPow(2., (double)uiColorDepth);
			double dMax8 = 255.;
			double dK = 1./(double)(iMax16-1);
			quint8* puiPrc = new quint8[iMax16];
			for(int i = 0; i < iMax16; i++)
			{
				puiPrc[i] = ((quint8)qRound(dMax8 * ((double)i) * dK));
			}
			s_mapPrecalc16to8.insert(uiColorDepth, puiPrc);
			return puiPrc;
		}
	}
}

//------------------------------------------------------------------
//The object needs own memory pool because it's output format differs
//from the input format.
CxBayerPseudoColorsCnbl::CxBayerPseudoColorsCnbl()
    : CxImageProvider(ExChainableFlags(CxChainable::eHasOwnMemoryPool | CxChainable::eCanChangeEnableState))
{
}

//-----------------------------------------------------------------
QString CxBayerPseudoColorsCnbl::title() const
{
    //The title displayed in Chain Editor.
    return tr("RAW Coloring");
}

//-----------------------------------------------------------------
bool CxBayerPseudoColorsCnbl::acceptsDataFrom(CxChainable *pPrecedessor)
{
    //The object accepts images (coming from CxImageProvider).
    return qobject_cast<CxImageProvider*>(pPrecedessor) != NULL;
}

//-----------------------------------------------------------------
int CxBayerPseudoColorsCnbl::buffersCountInMemoryPool() const
{
    //The object needs one buffer for output image.
    return 1;
}

//-----------------------------------------------------------------
CxChainable* CxBayerPseudoColorsCnbl::clone()
{
    return new CxBayerPseudoColorsCnbl();
}

//-----------------------------------------------------------------
void colorize_raw8(const SxPicBuf &srcPic, SxPicBuf &dstPic, int shiftFirstOdd, int shiftSecondOdd,
                   int shiftFirstEven, int shiftSecondEven)
{
    const quint8 *pSrcPix;
    quint32 *pDstPix;
    uint uiWidth = srcPic.m_uiWidth;
    uint uiHeight = srcPic.m_uiHeight;
    uint uiStrideSrc = srcPic.m_uiStride;
    uint uiStrideDst = dstPic.m_uiStride;
    for(uint row = 0; row < uiHeight; row+=2)
    {
        for(uint col = 0; col < uiWidth; col+=2)
        {
            pSrcPix = PIXEL_U8(srcPic, col, row);
            pDstPix = (quint32*)PIXEL_U8(dstPic, col, row);
            *pDstPix = (((quint32)(*pSrcPix))<<shiftFirstOdd) | XICORE_RGBA_ALPHA_MASK;
            *(pDstPix+1) = (((quint32)(*(pSrcPix+1)))<<shiftSecondOdd) |  XICORE_RGBA_ALPHA_MASK;
            pSrcPix = NEXT_ROW_U8(pSrcPix, uiStrideSrc);
            pDstPix = (quint32*)NEXT_ROW_U8(((quint8*)pDstPix), uiStrideDst);
            *pDstPix = (((quint32)(*pSrcPix))<<shiftFirstEven) | XICORE_RGBA_ALPHA_MASK;
            *(pDstPix+1) = (((quint32)(*(pSrcPix+1)))<<shiftSecondEven) | XICORE_RGBA_ALPHA_MASK;
        }
    }
}

void colorize_raw16(const SxPicBuf &srcPic, SxPicBuf &dstPic, const quint8 *puiPrecalc16to8,
                    int shiftFirstOdd, int shiftSecondOdd, int shiftFirstEven, int shiftSecondEven)
{
    const quint16 *pSrcPix;
    quint32 *pDstPix;
    uint uiWidth = srcPic.m_uiWidth;
    uint uiHeight = srcPic.m_uiHeight;
    uint uiStrideSrc = srcPic.m_uiStride;
    uint uiStrideDst = dstPic.m_uiStride;
    for(uint row = 0; row < uiHeight; row+=2)
    {
        for(uint col = 0; col < uiWidth; col+=2)
        {
            pSrcPix = PIXEL_U16(srcPic, col, row);
            pDstPix = (quint32*)PIXEL_U8(dstPic, col, row);
            *pDstPix = (((quint32)*(puiPrecalc16to8 + (*pSrcPix)))<<shiftFirstOdd) | XICORE_RGBA_ALPHA_MASK;
            *(pDstPix+1) = (((quint32)*(puiPrecalc16to8+(*(pSrcPix+1))))<<shiftSecondOdd) |  XICORE_RGBA_ALPHA_MASK;
            pSrcPix = NEXT_ROW_U16(pSrcPix, uiStrideSrc);
            pDstPix = (quint32*)NEXT_ROW_U8(pDstPix, uiStrideDst);
            *pDstPix = (((quint32)*(puiPrecalc16to8+(*pSrcPix)))<<shiftFirstEven) | XICORE_RGBA_ALPHA_MASK;
            *(pDstPix+1) = (((quint32)*(puiPrecalc16to8+(*(pSrcPix+1))))<<shiftSecondEven) | XICORE_RGBA_ALPHA_MASK;
        }
    }
}

void colorize_raw32(const SxPicBuf &srcPic, SxPicBuf &dstPic,
                    int shiftFirstOdd, int shiftSecondOdd, int shiftFirstEven, int shiftSecondEven)
{
    const quint32 *pSrcPix;
    quint32 *pDstPix;
    uint uiWidth = srcPic.m_uiWidth;
    uint uiHeight = srcPic.m_uiHeight;
    uint uiStrideSrc = srcPic.m_uiStride;
    uint uiStrideDst = dstPic.m_uiStride;
    uint shift = srcPic.m_uiBpc - dstPic.m_uiBpc;
    for(uint row = 0; row < uiHeight; row+=2)
    {
        for(uint col = 0; col < uiWidth; col+=2)
        {
            pSrcPix = PIXEL_U32(srcPic, col, row);
            pDstPix = (quint32*)PIXEL_U8(dstPic, col, row);
            *pDstPix = (((*pSrcPix)>>shift)<<shiftFirstOdd) | XICORE_RGBA_ALPHA_MASK;
            *(pDstPix+1) = (((*(pSrcPix+1))>>shift)<<shiftSecondOdd) |  XICORE_RGBA_ALPHA_MASK;
            pSrcPix = NEXT_ROW_U32(pSrcPix, uiStrideSrc);
            pDstPix = (quint32*)NEXT_ROW_U8(pDstPix, uiStrideDst);
            *pDstPix = (((*pSrcPix)>>shift)<<shiftFirstEven) | XICORE_RGBA_ALPHA_MASK;
            *(pDstPix+1) = (((*(pSrcPix+1))>>shift)<<shiftSecondEven) | XICORE_RGBA_ALPHA_MASK;
        }
    }
}

void colorize_raw_float(const SxPicBuf &srcPic, SxPicBuf &dstPic,
                    int shiftFirstOdd, int shiftSecondOdd, int shiftFirstEven, int shiftSecondEven)
{
    const float *pSrcPix;
    quint32 *pDstPix;
    uint uiWidth = srcPic.m_uiWidth;
    uint uiHeight = srcPic.m_uiHeight;
    uint uiStrideSrc = srcPic.m_uiStride;
    uint uiStrideDst = dstPic.m_uiStride;

    float fMin, fMax;
    CxPicBufAPI::CalcFloatDataMinMax(srcPic, fMin, fMax);
    float fRange = fMax - fMin;
    if (fRange < 0.000001) fRange = 1.0;
    float fScale = 255.0f / fRange;
    quint32 val1, val2;

    for(uint row = 0; row < uiHeight; row+=2)
    {
        for(uint col = 0; col < uiWidth; col+=2)
        {
            pSrcPix = PIXEL_F(srcPic, col, row);
            pDstPix = (quint32*)PIXEL_U8(dstPic, col, row);

            val1 = quint8((*pSrcPix - fMin) * fScale);
            val2 = quint8((*(pSrcPix+1) - fMin) * fScale);
            *pDstPix = (val1 << shiftFirstOdd) | XICORE_RGBA_ALPHA_MASK;
            *(pDstPix+1) = (val2 << shiftSecondOdd) |  XICORE_RGBA_ALPHA_MASK;

            pSrcPix = NEXT_ROW_F(pSrcPix, uiStrideSrc);
            pDstPix = (quint32*)NEXT_ROW_U8(pDstPix, uiStrideDst);
            val1 = quint8((*pSrcPix - fMin) * fScale);
            val2 = quint8((*(pSrcPix+1) - fMin) * fScale);
            *pDstPix = (val1 << shiftFirstEven) | XICORE_RGBA_ALPHA_MASK;
            *(pDstPix+1) = (val2 << shiftSecondEven) | XICORE_RGBA_ALPHA_MASK;
        }
    }
}

//-----------------------------------------------------------------
// Helper cuntions for HSI coloring
void colhsi_to_gray(const SxPicBuf &rRaw, SxPicBuf &aPicOut, const quint8 *puiPrecalc16to8, int x, int y, int w, int h)
{
    quint32 arrMap[256];
    for (int i = 0; i < 256; i++)
        arrMap[i] = DX_RGB(i, i, i);

    if (rRaw.m_eDataType == extypeUInt8)
    {
        for (int row = 0; row < h; row++)
        {
            const quint8* pSrcLine = ROW_U8(rRaw, y + row) + x;
            quint32* pDstLine = ROW_U32(aPicOut, y + row) + x;
            for (int col = 0; col < w; col++)
            {
                *pDstLine = arrMap[*pSrcLine];
                pSrcLine++;
                pDstLine++;
            }
        }
    }
    else if (rRaw.m_eDataType == extypeUInt16 && puiPrecalc16to8 != NULL)
    {
        for (int row = 0; row < h; row++)
        {
            const quint16* pSrcLine = ROW_U16(rRaw, y + row) + x;
            quint32* pDstLine = ROW_U32(aPicOut, y + row) + x;
            for (int col = 0; col < w; col++)
            {
                *pDstLine = arrMap[puiPrecalc16to8[*pSrcLine]];
                pSrcLine++;
                pDstLine++;
            }
        }
    }
    else if (rRaw.m_eDataType == extypeUInt32)
    {
        quint32 shift = rRaw.m_uiBpc - 8;
        for (int row = 0; row < h; row++)
        {
            const quint32* pSrcLine = ROW_U32(rRaw, y + row) + x;
            quint32* pDstLine = ROW_U32(aPicOut, y + row) + x;
            for (int col = 0; col < w; col++)
            {
                *pDstLine = arrMap[quint8((*pSrcLine)>>shift)];
                pSrcLine++;
                pDstLine++;
            }
        }
    }
    else if (rRaw.m_eDataType == extypeFloat)
    {
        float fMin, fMax;
        CxPicBufAPI::CalcFloatDataMinMax(rRaw, fMin, fMax);
        float fRange = fMax - fMin;
        if (fRange < 0.000001) fRange = 1.0;
        float fScale = 255.0f / fRange;

        for (int row = 0; row < h; row++)
        {
            const float* pSrcLine = ROW_F(rRaw, y + row) + x;
            quint32* pDstLine = ROW_U32(aPicOut, y + row) + x;
            for (int col = 0; col < w; col++)
            {
                quint8 val = quint8((*pSrcLine - fMin) * fScale);
                *pDstLine = arrMap[val];
                pSrcLine++;
                pDstLine++;
            }
        }
    }
}

// http://stackoverflow.com/questions/1472514/convert-light-frequency-to-rgb
void spectral_color(double &r, double &g, double &b, double l) // RGB <0,1> <- lambda l <400,700> [nm]
{
    if((l >= 380) && (l<440)){
        r = -(l - 440) / (440 - 380);
        g = 0.0;
        b = 1.0;
    }else if((l >= 440) && (l<490)){
        r = 0.0;
        g = (l - 440) / (490 - 440);
        b = 1.0;
    }else if((l >= 490) && (l<510)){
        r = 0.0;
        g = 1.0;
        b = -(l - 510) / (510 - 490);
    }else if((l >= 510) && (l<580)){
        r = (l - 510) / (580 - 510);
        g = 1.0;
        b = 0.0;
    }else if((l >= 580) && (l<645)){
        r = 1.0;
        g = -(l - 645) / (645 - 580);
        b = 0.0;
    }else if((l >= 645) && (l<700)){
        r = 1.0;
        g = 0.0;
        b = 0.0;
    }else{
        r = 0.0;
        g = 0.0;
        b = 0.0;
    };

    // Let the intensity fall off near the vision limits
    double factor;
    if((l >= 380) && (l<420)){
        factor = 0.3 + 0.7*(l - 380) / (420 - 380);
    }else if((l >= 420) && (l<701)){
        factor = 1.0;
    }else if((l >= 701) && (l<781)){
        factor = 0.3 + 0.7*(780 - l) / (780 - 700);
    }else{
        factor = 0.0;
    };

    // OUR extension to wavelengths beyond 701 nm (and remove fall off at the end)
    if (l >= 651 && l < 750)
    {
        r = 1.0;
        g = 0.5*(l-651)/(750-651);
        b = (l-651)/(750-651);
        factor = 1.0;
    }
    else if (l >= 751 && l < 850)
    {
        r = 1.0;
        g = 0.5*(850-l)/(850-751);
        b = 1.0;
        factor = 1.0;
    }
    else if (l >= 851)
    {
        r = (1050-l)/(1050-851);
        g = (l-851)/(1050-851);
        b = 1.0;
        factor = 1.0;
    }

    r *= factor;
    g *= factor;
    b *= factor;
}

//-----------------------------------------------------------------
void colorize_hsi(const SxPicBuf &rRaw, SxPicBuf &aPicOut, const CxHsiCameraParams *pHsiParams, const quint8 *puiPrecalc16to8)
{
    int iPicWidth = rRaw.m_uiWidth;
    int iPicHeight = rRaw.m_uiHeight;

    for (int iZone = 0; iZone < pHsiParams->m_vecFilterZones.size(); iZone++)
    {
        const CxHsiCameraParams::CxFilterZone &rZone = pHsiParams->m_vecFilterZones[iZone];
        QRect rcActive = rZone.m_rcActiveArea;
        if (rcActive.isEmpty())
            rcActive.setRect(0, 0, iPicWidth, iPicHeight);
        else if (iZone == 0)
        {
            // convert everything to gray outside active area
            if (rcActive.top() > 0)
                colhsi_to_gray(rRaw, aPicOut, puiPrecalc16to8, 0, 0, iPicWidth, rcActive.top());
            if (rcActive.bottom()+1 < iPicHeight)
                colhsi_to_gray(rRaw, aPicOut, puiPrecalc16to8, 0, rcActive.bottom()+1, iPicWidth, iPicHeight - rcActive.bottom()-1);
            if (rcActive.left() > 0)
                colhsi_to_gray(rRaw, aPicOut, puiPrecalc16to8, 0, rcActive.top(), rcActive.left(), rcActive.height());
            if (rcActive.right()+1 < iPicWidth)
                colhsi_to_gray(rRaw, aPicOut, puiPrecalc16to8, rcActive.right()+1, rcActive.top(), iPicWidth - rcActive.right()-1, rcActive.height());
        }

        // prepare band colors
        int iBandCount = rZone.m_vecBands.size();
        quint32 *pBandMultColors = new quint32[256*iBandCount];
        for (int iBand = 0; iBand < iBandCount; iBand++)
        {
            double dWavelength = rZone.m_vecBands[iBand].m_vecPeakWaveLengths[0];
            double r, g, b;
            spectral_color(r, g, b, dWavelength);
            for (int i = 0; i < 256; i++)
                pBandMultColors[256*iBand + i] = DX_RGB(b*i, g*i, r*i);
        }

        // colorize active area
        int iBandIdx;
        if (rRaw.m_eDataType == extypeUInt8)
        {
            for (int y = rcActive.top(); y <= rcActive.bottom(); y++)
            {
                const quint8* pSrcLine = ROW_U8(rRaw, y) + rcActive.left();
                quint32* pDstLine = ROW_U32(aPicOut, y) + rcActive.left();
                for (int x = rcActive.left(); x <= rcActive.right(); x++)
                {
                    if (rZone.bandAtPicPos(x, y, &iBandIdx) != NULL)
                        *pDstLine = pBandMultColors[256*iBandIdx + *pSrcLine];
                    else
                        *pDstLine = 0;
                    pSrcLine++;
                    pDstLine++;
                }
            }
        }
        else if (rRaw.m_eDataType == extypeUInt16)
        {
            for (int y = rcActive.top(); y <= rcActive.bottom(); y++)
            {
                const quint16* pSrcLine = ROW_U16(rRaw, y) + rcActive.left();
                quint32* pDstLine = ROW_U32(aPicOut, y) + rcActive.left();
                for (int x = rcActive.left(); x <= rcActive.right(); x++)
                {
                    if (rZone.bandAtPicPos(x, y, &iBandIdx) != NULL)
                        *pDstLine = pBandMultColors[256*iBandIdx + puiPrecalc16to8[*pSrcLine]];
                    else
                        *pDstLine = 0;
                    pSrcLine++;
                    pDstLine++;
                }
            }
        }
        delete[] pBandMultColors;
    }
}

//-----------------------------------------------------------------
IxChainData* CxBayerPseudoColorsCnbl::processData(IxChainData *pReceivedData)
{
    CxImageData *pColored = NULL;
    SxPicBuf aPicOut;
    bool bOk = true;
    do
    {
        //When the object is stopped, do not process any data!
        if (pReceivedData == NULL || state() == eStopped)
        {
            break;
        }
        CxImageData *pRaw = qobject_cast<CxImageData*>(pReceivedData);
        if (pRaw == NULL)
        {
            break;
        }
        const SxPicBuf &rRaw = pRaw->picBuf();
        const CxImageMetadata &rRawMetadata = pRaw->imageMetadata();
        //if the image is not in the correct format, leave the data unprocessed
        if (!isSupportedRawFormat(rRaw, &rRawMetadata))
        {
            if (errorMessage().isEmpty())
                setErrorMessage(tr("The input image is not in supported RAW format."));
            return pRaw;
        }

        //Get current output image format of this object.
        CxImageMetadata aDstMetadata;
        if (!queryOutputImageInfo(rRaw, aPicOut, &rRawMetadata, &aDstMetadata))
        {
            bOk = false;
            break;
        }
        //alloc buffer for the colored image
        if(!CxPicBufAPI::AllocPicBufFromPool(m_hMemoryPool, this, aPicOut, aPicOut))
        {
            bOk = false;
            break;
        }

        if(rRaw.m_eDataType == extypeUInt8) // RAW8
        {
            switch(rRaw.m_eColorFilterArray)
            {
            case excfaRG:
                colorize_raw8(rRaw, aPicOut,
                              XICORE_RGBA_RED_SHIFT, XICORE_RGBA_GREEN_SHIFT,
                              XICORE_RGBA_GREEN_SHIFT, XICORE_RGBA_BLUE_SHIFT);
                break;
            case excfaGR:
                colorize_raw8(rRaw, aPicOut,
                              XICORE_RGBA_GREEN_SHIFT, XICORE_RGBA_RED_SHIFT,
                              XICORE_RGBA_BLUE_SHIFT, XICORE_RGBA_GREEN_SHIFT);
                break;
            case excfaGB:
                colorize_raw8(rRaw, aPicOut,
                              XICORE_RGBA_GREEN_SHIFT, XICORE_RGBA_BLUE_SHIFT,
                              XICORE_RGBA_RED_SHIFT, XICORE_RGBA_GREEN_SHIFT);
                break;
            case excfaBG:
                colorize_raw8(rRaw, aPicOut,
                              XICORE_RGBA_BLUE_SHIFT, XICORE_RGBA_GREEN_SHIFT,
                              XICORE_RGBA_GREEN_SHIFT, XICORE_RGBA_RED_SHIFT);
                break;
            case excfaHyperSpectral:
                colorize_hsi(rRaw, aPicOut, &rRawMetadata.m_aHsiParams, NULL);
                break;

            default: break;
            }
        }
        else if(rRaw.m_eDataType == extypeUInt16)   // RAW16
        {
            const quint8* puiPrecalc16to8 = GetPrecalc16to8(rRaw.m_uiBpc);
            switch(rRaw.m_eColorFilterArray)
            {
            case excfaRG:
                colorize_raw16(rRaw, aPicOut, puiPrecalc16to8,
                               XICORE_RGBA_RED_SHIFT, XICORE_RGBA_GREEN_SHIFT,
                               XICORE_RGBA_GREEN_SHIFT, XICORE_RGBA_BLUE_SHIFT);
                break;
            case excfaGR:
                colorize_raw16(rRaw, aPicOut, puiPrecalc16to8,
                               XICORE_RGBA_GREEN_SHIFT, XICORE_RGBA_RED_SHIFT,
                               XICORE_RGBA_BLUE_SHIFT, XICORE_RGBA_GREEN_SHIFT);
                break;
            case excfaGB:
                colorize_raw16(rRaw, aPicOut, puiPrecalc16to8,
                               XICORE_RGBA_GREEN_SHIFT, XICORE_RGBA_BLUE_SHIFT,
                               XICORE_RGBA_RED_SHIFT, XICORE_RGBA_GREEN_SHIFT);
                break;
            case excfaBG:
                colorize_raw16(rRaw, aPicOut, puiPrecalc16to8,
                               XICORE_RGBA_BLUE_SHIFT, XICORE_RGBA_GREEN_SHIFT,
                               XICORE_RGBA_GREEN_SHIFT, XICORE_RGBA_RED_SHIFT);
                break;
            case excfaHyperSpectral:
                colorize_hsi(rRaw, aPicOut, &rRawMetadata.m_aHsiParams, puiPrecalc16to8);
                break;

            default: break;
            }
        }
        else if(rRaw.m_eDataType == extypeUInt32)   // RAW32
        {
            switch(rRaw.m_eColorFilterArray)
            {
            case excfaRG:
                colorize_raw32(rRaw, aPicOut,
                               XICORE_RGBA_RED_SHIFT, XICORE_RGBA_GREEN_SHIFT,
                               XICORE_RGBA_GREEN_SHIFT, XICORE_RGBA_BLUE_SHIFT);
                break;
            case excfaGR:
                colorize_raw32(rRaw, aPicOut,
                               XICORE_RGBA_GREEN_SHIFT, XICORE_RGBA_RED_SHIFT,
                               XICORE_RGBA_BLUE_SHIFT, XICORE_RGBA_GREEN_SHIFT);
                break;
            case excfaGB:
                colorize_raw32(rRaw, aPicOut,
                               XICORE_RGBA_GREEN_SHIFT, XICORE_RGBA_BLUE_SHIFT,
                               XICORE_RGBA_RED_SHIFT, XICORE_RGBA_GREEN_SHIFT);
                break;
            case excfaBG:
                colorize_raw32(rRaw, aPicOut,
                               XICORE_RGBA_BLUE_SHIFT, XICORE_RGBA_GREEN_SHIFT,
                               XICORE_RGBA_GREEN_SHIFT, XICORE_RGBA_RED_SHIFT);
                break;
            case excfaHyperSpectral:
                colorize_hsi(rRaw, aPicOut, &rRawMetadata.m_aHsiParams, NULL);
                break;

            default: break;
            }
        }
        else if(rRaw.m_eDataType == extypeFloat)   // RAW32FLOAT
        {
            switch(rRaw.m_eColorFilterArray)
            {
            case excfaRG:
                colorize_raw_float(rRaw, aPicOut,
                               XICORE_RGBA_RED_SHIFT, XICORE_RGBA_GREEN_SHIFT,
                               XICORE_RGBA_GREEN_SHIFT, XICORE_RGBA_BLUE_SHIFT);
                break;
            case excfaGR:
                colorize_raw_float(rRaw, aPicOut,
                               XICORE_RGBA_GREEN_SHIFT, XICORE_RGBA_RED_SHIFT,
                               XICORE_RGBA_BLUE_SHIFT, XICORE_RGBA_GREEN_SHIFT);
                break;
            case excfaGB:
                colorize_raw_float(rRaw, aPicOut,
                               XICORE_RGBA_GREEN_SHIFT, XICORE_RGBA_BLUE_SHIFT,
                               XICORE_RGBA_RED_SHIFT, XICORE_RGBA_GREEN_SHIFT);
                break;
            case excfaBG:
                colorize_raw_float(rRaw, aPicOut,
                               XICORE_RGBA_BLUE_SHIFT, XICORE_RGBA_GREEN_SHIFT,
                               XICORE_RGBA_GREEN_SHIFT, XICORE_RGBA_RED_SHIFT);
                break;
            case excfaHyperSpectral:
                colorize_hsi(rRaw, aPicOut, &rRawMetadata.m_aHsiParams, NULL);
                break;

            default: break;
            }
        }

        aPicOut.copyPicAdditionalData(rRaw);

        pColored = new CxImageData();
        pColored->setPicBuf(aPicOut);
        pColored->setImageMetadata(aDstMetadata);

        if (!errorMessage().isEmpty())
            setErrorMessage("");

    } while(false);

    if(!bOk)
    {
        if (pColored != NULL)
        {
            delete pColored;//the aPicOut is deleted as well here
            pColored = NULL;
        }
        else
            CxPicBufAPI::FreePicBuf(aPicOut);//we must to free the buffer
    }
    else
    {
        delete pReceivedData;//everything is ok so we must to delete original data
    }

    return pColored;
}

//-----------------------------------------------------------------
bool CxBayerPseudoColorsCnbl::queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput, const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput)
{   
    picInfoOutput = picInfoInput;
    if (pMetadataOutput != NULL)
        *pMetadataOutput = *pMetadataInput;

    //supported raw images will be converted to RGB32
    if(isSupportedRawFormat(picInfoInput, pMetadataInput))
    {
        //RGB32
        SET_PIC_RGB32(picInfoOutput);
        picInfoOutput.m_uiStride = XICORE_ALIGN_IMG_STRIDE(picInfoOutput.m_uiWidth*picInfoOutput.bytesPerPixel());
        if (pMetadataOutput != NULL)
        {
            pMetadataOutput->m_eColorFilterArray = excfaNone;
            pMetadataOutput->m_aHsiParams = CxHsiCameraParams();
            pMetadataOutput->m_sApiContextList.clear();
        }
    }
    return true;
}

//--------------------------------------------------------
bool CxBayerPseudoColorsCnbl::hasHelp() const
{
    return true;
}

//--------------------------------------------------------
QString CxBayerPseudoColorsCnbl::help(QString &sImagesPath)
{
    Q_UNUSED(sImagesPath);
    return tr("Displays the current RAW image colored according to its Bayer grid.");
}

//-----------------------------------------------------------------
bool CxBayerPseudoColorsCnbl::isSupportedRawFormat(const SxPicBufInfo &picInfoInput,
                                                   const CxImageMetadata *pMetadataInput)
{
    // The input image must be a RAW
    if (!CHECK_PIC_RAW(picInfoInput) || pMetadataInput == NULL)
        return false;

    // All HSI images
    if (picInfoInput.m_eColorFilterArray == excfaHyperSpectral
            && pMetadataInput->m_aHsiParams.isValid())
        return true;

    // RGGB type mosaic needs to have image with even number rows and cols
    return (picInfoInput.m_eColorFilterArray == excfaRG ||
            picInfoInput.m_eColorFilterArray == excfaGR ||
            picInfoInput.m_eColorFilterArray == excfaGB ||
            picInfoInput.m_eColorFilterArray == excfaBG)
            && picInfoInput.m_uiWidth % 2 == 0 && picInfoInput.m_uiHeight % 2 == 0;
}
