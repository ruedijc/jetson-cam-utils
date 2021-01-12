#ifndef DATASAVER_H
#define DATASAVER_H

#include <xiCoreGlobal.h>
#include <PicBuf.h>
#include <QObject>
#include "ImageFormat.h"

class QWidget;
class IxChainData;
class CxImageData;

/*!
 *\brief The base class for any kind of class for saving \ref IxChainData
*/
class XICORE_API IxChainDataSaver
{
public:
    virtual ~IxChainDataSaver();

    //! In case the source has multiple data items, we let saver know the number of items.
    virtual void setSequenceSize(qint32 iSeqSize);

    //! Saves the current data. May be part of sequence.
    virtual bool save(const IxChainData *pData, qint32 iSeqIdx = 0) = 0;
};


/*!
 \brief The base class for any kind of image data saver.
  \remarks The implementations of this class should be registered by CxRtti so that
  the application could use them.
*/
class XICORE_API IxImageDataSaver : public IxChainDataSaver
{
public:
    virtual TxImageFormatList supportedImageFormats() = 0;      //!< Image formats capable of saving.
    virtual void setOutputFile(const QString &sFileName) = 0;
    virtual bool querySavedImageFormat(const SxPicBufInfo &aInput, SxPicBufInfo &aOutput);
    virtual void setSaverOptions(const TxImageFormatOptions &aOpt);

public:
    virtual QWidget* widgetForSaveDlg(QWidget *pParent);
    virtual TxImageFormatOptions optionsFromWidgetInSaveDlg(QWidget *pWidget);
    virtual TxImageFormatOptions codecOptionsFromWigetInSaveDlg(QWidget *pWidget, TxFourCC &fccCodec);
};

#endif // DATASAVER_H
