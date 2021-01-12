#ifndef CXFFT_H
#define CXFFT_H

#include <xiCoreGlobal.h>
#include <PicBuf.h>

//! The class covers several 1D and 2D FFT functions.
class XICORE_API CxFFT
{
public:

    /*!
     * \brief Preallocates a buffers needed for 2D FFT. The buffers must be freed using \ref Free2D()
     */
    static bool Alloc2D(const SxPicBufInfo &picSrc, void *& pCfg, void *& pFFTin, void *& pFFTout);

    /*!
      * \brief Frees the temporary buffers allocated by \ref Alloc2D()
    */
    static void Free2D(void *pCfg, void *pFFTin, void *pFFTout);

    /*!
     * \brief Performs the 2D FFT on the \p picSrcMono image.
     * \remarks The \p picRe and \p picIm are images of the same size as \p picSrc. The image type of these images must be
     * \ref extypeInt32. The \p pCfg, \p pFFTin and \p pFFTout are buffers preallocated by \ref Alloc2D().
     */
    static bool Calc2D(const SxPicBuf &picSrcMono, SxPicBuf &picRe, SxPicBuf &picIm, void *pCfg, void *pFFTin, void *pFFTout);

    /*!
     * \brief Performs the 2D FFT on the \p picSrcMono image.
     * \remarks The \p picRe and \p picIm are images of the same size as \p picSrc. The image type of these images must be
     * \ref extypeInt32.
     * \warning This function allocates the temporary buffers internally for each call so that it is less effective. It is suitable for
     * static images.
     */
    static bool Calc2D(const SxPicBuf &picSrcMono, SxPicBuf &picRe, SxPicBuf &picIm);

    /*!
     * \brief Preallocates a buffers needed for 1D FFT. The buffers must be freed using \ref Free1D()
     */
    static bool Alloc1D(int iLength, void *&pCfg, void *&pFFTin, void *&pFFTout);

    /*!
      * \brief Frees the temporary buffers allocated by \ref Alloc1D()
    */
    static void Free1D(void *pCfg, void *pFFTin, void *pFFTout);

    /*!
     * \brief Performs the 1D FFT on the \p pInput array.
     * \remarks The \p pReal and \p pImaginary are array of the same lenght as \p pInput and the length must be the same as passed
     * to the \ref Alloc1D(). The \p pCfg, \p pFFTin and \p pFFTout are buffers preallocated by \ref Alloc1D().
     */
    static bool Calc1D(float *pInput, float *pReal, float *pImaginary, void *pCfg, void *pFFTin, void *pFFTout);

    /*!
     * \brief Performs the 1D FFT on the \p pInput array.
     * \remarks The \p pReal and \p pImaginary are array of the same length as \p pInput given by \p iLength.
     * \warning This function allocates the temporary buffers internally for each call so that it is less effective.
     */
    static bool Calc1D(float *pInput, float *pReal, float *pImaginary, int iLength);
};

#endif // CXFFT_H
