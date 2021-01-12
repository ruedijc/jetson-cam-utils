#ifndef DATALOADER_H
#define DATALOADER_H

#include <xiCoreGlobal.h>
#include <PicBuf.h>
#include <QObject>
#include "ImageFormat.h"

class IxChainData;
class CxImageData;

/*!
 \brief The base class for any kind of chain data loading.
*/
class XICORE_API IxChainDataLoader
{
public:
    virtual ~IxChainDataLoader() {}

    //! Loads data from some source.
    virtual bool load() = 0;

    //! In case the source has multiple data items, their count is returnes here.
    virtual qint32 sequenceSize() = 0;

    /*!
     * \brief Gets the pointer to the current data (after the \ref load() method was called).
     * \remarks The caller of this method is the owner of the returned data so that it has to
     * take care about its deletion using the \p delete operator.
     */
    virtual IxChainData *loadedData(qint32 iSeqIdx = 0) = 0;

    //! If the loader load data from file then this method returns the the file's name.
    virtual QString inputFile() = 0;
    virtual void setInputFile(const QString &sFileName) = 0;
};



/*!
 \brief The base class for image data loading.
 \remarks The implementations of this class should be registered by CxRtti so that
  the application could use them.
*/
class XICORE_API IxImageDataLoader : public IxChainDataLoader
{
public:
    //! In case the source is a sequence, this should get its fps. Return 0 if uncertain.
    virtual double loadedFps() = 0;

    //! Image formats capable of loading.
    virtual TxImageFormatList supportedImageFormats() = 0;
};


#endif // DATALOADER_H
