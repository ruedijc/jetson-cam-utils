#ifndef CXFLIPIMAGECNBL_H
#define CXFLIPIMAGECNBL_H


#include <Chainable.h>
#include <QToolButton>

/*!
 * \brief The class flips the actual image according to the user input.
 * \remarks The purpose of this class is to demonstrate an implementation of chainable object
 * with configuration dialog.
 */
class CxFlipImageCnbl : public CxImageProvider
{
    Q_OBJECT
    Q_CLASSINFO("CustomName", tr("Flip"))
    Q_CLASSINFO("UniqueID", "com.ximea.CamTool.CxFlipImageCnbl")
public:
    Q_INVOKABLE CxFlipImageCnbl();
public:
    virtual QString title() const;
    virtual bool acceptsDataFrom(CxChainable *pPrecedessor);
    virtual int buffersCountInMemoryPool() const;
    virtual CxChainable* clone();
    virtual QWidget* configurationWidget();
    virtual bool hasConfigurationWidget() const;
    virtual void simpleStateConfigButtons(TxSimpleStateConfigButtonList &lstButtons, bool &bMutuallyExclusive);
    virtual void simpleStateConfigButtonPressed(int iBtnData);
    virtual bool saveSettings(QDomDocument &aDoc, QDomElement &aElm) const;
    virtual bool loadSettings(const QDomElement &aDom);
    virtual bool hasHelp() const;
    virtual QString help(QString &sImagesPath);
protected:
    virtual IxChainData* processData(IxChainData *pReceivedData);//performs the flip of the image

    // CxImageProvider
public:
    virtual bool queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput,
                                      const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput = NULL);

//own
public:
    bool m_bFlipHoriz;  //!< Flip by vertical axis.
    bool m_bFlipVert;   //!< Flip by horizontal axis.

    void setTransTypes(bool bHorz, bool bVert);
};


#endif // CXFLIPIMAGECNBL_H
