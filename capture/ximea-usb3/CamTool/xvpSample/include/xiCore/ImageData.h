#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <xiCoreGlobal.h>
#include <Data.h>
#include <ImageMetadata.h>


/*!
 \brief The class implements an image data object exchanged by \ref CxChainable objects.
*/
class XICORE_API CxImageData : public IxChainData
{
    Q_OBJECT
public:
    //! Constructor.
    CxImageData();

    //! Destructor.
    virtual ~CxImageData();

    //IxChainData
public:
    virtual qint32 frameNo() const;
    virtual IxChainData* clone(TxMemPoolHandle hMemPool, QObject *pMemPoolClient) const;
    virtual IxChainData* clone(QObject *pMemPoolClient) const;
    virtual bool isFromMemoryPool(TxMemPoolHandle hMemPool) const;

    //own members
public:
    virtual SxPicBuf& picBuf();                 //!< Reference to the picture. Can be modified
    virtual const SxPicBuf& picBuf() const;     //!< Const variant should not change the returned buffer
    virtual void setPicBuf(const SxPicBuf &picBuf);

    virtual const CxImageMetadata& imageMetadata() const;
    virtual void setImageMetadata(const CxImageMetadata &aMetadata);

protected:
    SxPicBuf m_aPic;  /*!< The image. */
    CxImageMetadata m_aMetadata;
};

/*!
 * \brief Implementation of \ref CxImageData intended for displaying in views.
 * \remarks The class keeps pointer to the original data and to the data converted to RGB32
 * which can be displayed by QGraphicsView.
 */
class XICORE_API CxViewImageData : public CxImageData
{
    Q_OBJECT
public:

    /*!
     \brief Constructor.

     \remarks The object takes ownership of the \p pOrigData and \p pViewPicBuf.
     If the \p pViewPicBuf is NULL then the object internally uses pOrigData->picBuf()
     as a view picbuf.

    */
    CxViewImageData(CxImageData *pOrigData, SxPicBuf *pViewPicBufRgb32);
    virtual ~CxViewImageData();

    //IxData
public:    
    virtual IxChainData* clone(TxMemPoolHandle hMemPool, QObject *pMemPoolClient) const;
    //! Returns true if the original picbuf (\ref origImageData()->picBuf()) or the current picbuf is allocated from the given memory pool.
    virtual bool isFromMemoryPool(TxMemPoolHandle hMemPool) const;

    //own members
public:
    const CxImageData *origImageData() const;   //!< Returns the original image data.

private:
    CxImageData *m_pOrigData;  /*!< Original image data. */    
};

/*!
 * \brief Implementation of \ref CxImageData that can keep the FFT on an image. It consists of real and imaginary parts both of type
 * extypeInt32.
 * \remarks The \ref setPicBuf() and \ref picBuf() methods sets/gets the real part of the FFT.
 * \warning This class can keep only one-channel images!
 */
class XICORE_API CxFFTImageData : public CxImageData
{
    Q_OBJECT
public:
    CxFFTImageData();
    virtual ~CxFFTImageData();

    //IxChainData
public:
    virtual IxChainData* clone(TxMemPoolHandle hMemPool, QObject *pMemPoolClient) const;

    virtual void setPicBuf(const SxPicBuf &picBuf); //!< Set she real part the the FFT image. It must be one-channel image of type extypeInt32.
    //own members

public:

    bool setPicBufIm(const SxPicBuf &picIm); //!< Sets the imaginary part of the FFT image. It must be one-channel image of type extypeInt32.
    SxPicBuf &picBufIm();               //!< Reference to the imaginary parth of the FFT image. Can be modified.
    const SxPicBuf &picBufIm() const;   //!< Const variant that should not change the returned buffer.

protected:
    SxPicBuf m_aPicIm;//! The imaginary part of the FFT.

};

#endif // IMAGEDATA_H
