#include <QToolBar>
#include <QHBoxLayout>

#include <ImageData.h>
#include <PicBufAPI.h>
#include <AppDelegate.h>
#include <Chain.h>
#include "FlipImageCnbl.h"
#include "FlipConfigurationWgt.h"

//-----------------------------------------------------------
CxFlipImageCnbl::CxFlipImageCnbl()
    : CxImageProvider(ExChainableFlags(CxChainable::eCanChangeEnableState)),
      m_bFlipHoriz(true), m_bFlipVert(false)
{

}

//-----------------------------------------------------------
QString CxFlipImageCnbl::title() const
{
    QString sFlip;
    if (m_bFlipHoriz || m_bFlipVert)
    {
        sFlip = " ";
        if (m_bFlipHoriz)
        {
            sFlip += "H";
        }
        if (m_bFlipVert)
        {
            if (m_bFlipHoriz)
                sFlip += "+";
            sFlip += "V";
        }
    }
    return tr("Flip") + sFlip;
}

//-----------------------------------------------------------
bool CxFlipImageCnbl::acceptsDataFrom(CxChainable *pPrecedessor)
{
    //The object accepts images (coming from CxImageProvider).
    return qobject_cast<CxImageProvider*>(pPrecedessor) != NULL;
}

//-----------------------------------------------------------
int CxFlipImageCnbl::buffersCountInMemoryPool() const
{
    //The object needs one buffer for output image.
    return 1;
}

//-----------------------------------------------------------
CxChainable* CxFlipImageCnbl::clone()
{
    CxFlipImageCnbl *pClone = new CxFlipImageCnbl();
    pClone->m_bFlipHoriz = m_bFlipHoriz;
    pClone->m_bFlipVert = m_bFlipVert;
    return pClone;
}

//-----------------------------------------------------------
QWidget *CxFlipImageCnbl::configurationWidget()
{
    return new CxFlipConfigurationWgt(this);
}

//-----------------------------------------------------------
bool CxFlipImageCnbl::hasConfigurationWidget() const
{
    return true;
}

//--------------------------------------------------
void CxFlipImageCnbl::simpleStateConfigButtons(TxSimpleStateConfigButtonList &lstButtons, bool &bMutuallyExclusive)
{
    lstButtons.push_back(CxSimpleStateConfigButton("H", 0, m_bFlipHoriz));
    lstButtons.push_back(CxSimpleStateConfigButton("V", 1, m_bFlipVert));
    bMutuallyExclusive = false;
}

//--------------------------------------------------
void CxFlipImageCnbl::simpleStateConfigButtonPressed(int iBtnData)
{
    bool bHorz = m_bFlipHoriz, bVert = m_bFlipVert;
    if (iBtnData == 0) bHorz = !bHorz;
    if (iBtnData == 1) bVert = !bVert;

    bool bStopped = chain()->stopped();
    if (!bStopped)
        chain()->stop();

    setTransTypes(bHorz, bVert);

    if (!bStopped)
        chain()->start();
}

//-----------------------------------------------------------
bool CxFlipImageCnbl::saveSettings(QDomDocument &aDoc, QDomElement &aElm) const
{
    //do not forget to save settings on parent!
    bool bOk = CxChainable::saveSettings(aDoc, aElm);
    aElm.setAttribute("flipHoriz", m_bFlipHoriz ? 1 : 0);
    aElm.setAttribute("flipVert", m_bFlipVert ? 1 : 0);
    return bOk;
}

//-----------------------------------------------------------
bool CxFlipImageCnbl::loadSettings(const QDomElement &aDom)
{
    //do not forget to load settings on parent!
    bool bOk = CxChainable::loadSettings(aDom);
    if (aDom.hasAttribute("flipHoriz"))
        m_bFlipHoriz = (aDom.attribute("flipHoriz").toInt() > 0);
    if (aDom.hasAttribute("flipVert"))
        m_bFlipVert = (aDom.attribute("flipVert").toInt() > 0);
    return bOk;
}

//--------------------------------------------------------
bool CxFlipImageCnbl::hasHelp() const
{
    return true;
}

//--------------------------------------------------------
QString CxFlipImageCnbl::help(QString &sImagesPath)
{
    //we add the path to resources
    sImagesPath = ":/images";
    QString sHelp= tr("Flips the current image:")
              +"<p>"
              +  "<img src=\"flipHoriz.png\" width=\"24\" height=\"24\"> "+ tr("flip horizontally")+" <br/>"
              +  "<img src=\"flipVert.png\" width=\"24\" height=\"24\"> "+ tr("flip vertically")+" <br/>"
              + "</p>";
    return sHelp;
}

//-----------------------------------------------------------
IxChainData* CxFlipImageCnbl::processData(IxChainData *pReceivedData)
{
    CxImageData *pTransformed = NULL;
    bool bOk = true;
    do
    {
        //When the object is stopped, do not process any data!
        if (pReceivedData == NULL || state() == eStopped)
        {
            break;
        }
        CxImageData *pOrig = qobject_cast<CxImageData*>(pReceivedData);
        if (pOrig == NULL)
        {
            break;
        }
        if (!m_bFlipHoriz && !m_bFlipVert)
        {
            pTransformed = pOrig;
            break;
        }

        SxPicBuf &rOrig = pOrig->picBuf();
        //Get current output image format of this object.
        SxPicBufInfo aOut;
        CxImageMetadata aDstMetadata;
        if (!queryOutputImageInfo(rOrig, aOut, &pOrig->imageMetadata(), &aDstMetadata))
        {
            bOk = false;
            break;
        }
        if (m_bFlipVert)
        {

            bOk = CxPicBufAPI::ReverseRowOrder(rOrig);
            if (!bOk)
            {
                break;
            }
        }

        if (m_bFlipHoriz)
        {
            bOk = CxPicBufAPI::ReverseColOrder(rOrig);
            if (!bOk)
                break;
        }

        rOrig.m_eColorFilterArray = aOut.m_eColorFilterArray;
        pOrig->setImageMetadata(aDstMetadata);
        pTransformed = pOrig;
    }while(false);

    return pTransformed;
}

//-----------------------------------------------------------
bool CxFlipImageCnbl::queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput, const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput)
{
    //The image size stays the same
    picInfoOutput = picInfoInput;
    if (pMetadataOutput != NULL)
        *pMetadataOutput = *pMetadataInput;

    //change the eventual color filter array for RAW images.
    ExColorFilterArray eCfa = picInfoInput.m_eColorFilterArray;
    if (m_bFlipHoriz)
    {
        if (picInfoOutput.m_eColorFilterArray == excfaRG)      picInfoOutput.m_eColorFilterArray = excfaGR;
        else if (picInfoOutput.m_eColorFilterArray == excfaGR) picInfoOutput.m_eColorFilterArray = excfaRG;
        else if (picInfoOutput.m_eColorFilterArray == excfaGB) picInfoOutput.m_eColorFilterArray = excfaBG;
        else if (picInfoOutput.m_eColorFilterArray == excfaBG) picInfoOutput.m_eColorFilterArray = excfaGB;
        eCfa = picInfoOutput.m_eColorFilterArray;
    }
    if (m_bFlipVert)
    {
        if (eCfa == excfaRG)      picInfoOutput.m_eColorFilterArray = excfaGB;
        else if (eCfa == excfaGB) picInfoOutput.m_eColorFilterArray = excfaRG;
        else if (eCfa == excfaGR) picInfoOutput.m_eColorFilterArray = excfaBG;
        else if (eCfa == excfaBG) picInfoOutput.m_eColorFilterArray = excfaGR;
    }

    if (pMetadataOutput != NULL)
    {
        pMetadataOutput->m_eColorFilterArray = picInfoOutput.m_eColorFilterArray;
        pMetadataOutput->updateFromPicBufInfo(&picInfoOutput);
    }

    return true;
}

//-----------------------------------------------------------
void CxFlipImageCnbl::setTransTypes(bool bHorz, bool bVert)
{
    //We do not have to stop the chain because the image format (width, height, number of channels..)
    //will not change. But we have to lock the object because the processing method could run in a different
    //thread (this method is called from configuration dialog which is running in the GUI thread).
    m_lock.lock();
    m_bFlipHoriz = bHorz;
    m_bFlipVert = bVert;
    m_lock.unlock();
}

