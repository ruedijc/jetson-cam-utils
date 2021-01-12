#include "NegativeChnbl.h"

#include <ImageData.h>
#include <PicBufAPI.h>
#include <PicBufMacros.h>

CxNegativeChnbl::CxNegativeChnbl() : CxImageProvider(ExChainableFlags(CxChainable::eCanChangeEnableState))
{
}

//-----------------------------------------------------------------
QString CxNegativeChnbl::title() const
{
    return tr("Negative");
}

//-----------------------------------------------------------------
bool CxNegativeChnbl::acceptsDataFrom(CxChainable *pPrecedessor)
{
    return qobject_cast<CxImageProvider*>(pPrecedessor) != NULL;
}

//-----------------------------------------------------------------
int CxNegativeChnbl::buffersCountInMemoryPool() const
{
    return 0;
}

//-----------------------------------------------------------------
CxChainable* CxNegativeChnbl::clone()
{
    return new CxNegativeChnbl();
}

//-----------------------------------------------------------------
IxChainData* CxNegativeChnbl::processData(IxChainData *pReceivedData)
{
    CxImageData *pImage = qobject_cast<CxImageData*>(pReceivedData);
    if (pImage == NULL)
        return pReceivedData;

    SxPicBuf &rPic = pImage->picBuf();
    uint uiWidth = rPic.m_uiWidth;
    uint uiHeight = rPic.m_uiHeight;
    uint uiChannels = rPic.m_uiChannels;

    if (rPic.m_eDataType == extypeUInt8)
    {
        quint8 *pLine;
        if (CHECK_PIC_RGB32(rPic))  // must skip alpha channel
        {
            for (uint row = 0; row < uiHeight; row++)
            {
                pLine = ROW_U8(rPic, row);
                for (quint32 col = 0; col < uiWidth; col++)
                {
                    pLine[XICORE_RGBA_RED] = 255-pLine[XICORE_RGBA_RED];
                    pLine[XICORE_RGBA_GREEN] = 255-pLine[XICORE_RGBA_GREEN];
                    pLine[XICORE_RGBA_BLUE] = 255-pLine[XICORE_RGBA_BLUE];
                    pLine+=4;
                }
            }
        }
        else
        {
            for (uint row = 0; row < uiHeight; row++)
            {
                pLine = ROW_U8(rPic, row);
                for (quint32 col = 0; col < uiWidth; col++)
                {
                    for (quint32 chan = 0; chan < uiChannels; chan++)
                    {
                        *pLine = 255-*pLine;
                        pLine++;
                    }
                }
            }
        }
    }
    else if (rPic.m_eDataType == extypeUInt16)
    {
        quint16 uiMaxVal = (quint16)((1<<rPic.m_uiBpc) - 1);
        quint16 *pLine;
        if (CHECK_PIC_RGB64(rPic))  // must skip alpha channel
        {
            for (uint row = 0; row < uiHeight; row++)
            {
                pLine = ROW_U16(rPic, row);
                for (quint32 col = 0; col < uiWidth; col++)
                {
                    pLine[XICORE_RGBA_RED] = uiMaxVal-pLine[XICORE_RGBA_RED];
                    pLine[XICORE_RGBA_GREEN] = uiMaxVal-pLine[XICORE_RGBA_GREEN];
                    pLine[XICORE_RGBA_BLUE] = uiMaxVal-pLine[XICORE_RGBA_BLUE];
                    pLine+=4;
                }
            }
        }
        else
        {
            for (uint row = 0; row < uiHeight; row++)
            {
                pLine = ROW_U16(rPic, row);
                for (quint32 col = 0; col < uiWidth; col++)
                {
                    for (quint32 chan = 0; chan < uiChannels; chan++)
                    {
                        *pLine = uiMaxVal-*pLine;
                        pLine++;
                    }
                }
            }
        }
    }
    else if (rPic.m_eDataType == extypeUInt32)
    {
        quint32 uiMaxVal = (1<<rPic.m_uiBpc) - 1;
        if (rPic.m_uiBpc == 32)
            uiMaxVal = 0xFFFFFF;

        quint32 *pLine;
        for (uint row = 0; row < uiHeight; row++)
        {
            pLine = ROW_U32(rPic, row);
            for (quint32 col = 0; col < uiWidth; col++)
            {
                for (quint32 chan = 0; chan < uiChannels; chan++)
                {
                    *pLine = uiMaxVal-*pLine;
                    pLine++;
                }
            }
        }
    }
    else if (rPic.m_eDataType == extypeFloat)
    {
        float fOldMin, fOldMax;
        CxPicBufAPI::CalcFloatDataMinMax(rPic, fOldMin, fOldMax);

        float *pLine;
        for (uint row = 0; row < uiHeight; row++)
        {
            pLine = ROW_F(rPic, row);
            for (quint32 col = 0; col < uiWidth; col++)
            {
                for (quint32 chan = 0; chan < uiChannels; chan++)
                {
                    *pLine = -*pLine;
                    pLine++;
                }
            }
        }
        rPic.m_fFloatDataMin = -fOldMax;
        rPic.m_fFloatDataMax = -fOldMin;
    }
    return pReceivedData;
}

//--------------------------------------------------------
bool CxNegativeChnbl::hasHelp() const
{
    return true;
}

//--------------------------------------------------------
QString CxNegativeChnbl::help(QString &sImagesPath)
{
    Q_UNUSED(sImagesPath);
    return tr("Inverts the colors of the current image.");
}

//-----------------------------------------------------------------
bool CxNegativeChnbl::queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput, const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput)
{
    picInfoOutput = picInfoInput;
    if (pMetadataOutput != NULL)
        *pMetadataOutput = *pMetadataInput;
    return true;
}
