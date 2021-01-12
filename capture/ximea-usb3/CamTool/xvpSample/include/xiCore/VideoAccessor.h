#ifndef VIDEOACCESSOR_H
#define VIDEOACCESSOR_H

#include "qglobal.h"


/*!
 * \brief The class for accessing the raw video data buffers.
 * \remarks This class should implemented by video loaders. It is passed to IxVideoDecoder instances
 * to access the video data.
 */
class IxVideoAccessor
{
public:    
    //! Number of frames of the video sequence.
    virtual qint32 framesCount() = 0;

    //! Size of the frame with the given index in bytes.
    virtual size_t frameDataSize(qint32 iSeqIndex) = 0;

    //! Copies the raw frame data with given index to the preallocated \p pData.
    virtual bool frameData(qint32 iSeqIndex, void *pData, size_t &ruiSize) = 0;

    //! Returns the index of last processed (decoded) frame.
    virtual qint32 lastProcessedFrame() = 0;

    //! Returns a list of keyframes of the video sequence.
    virtual void keyFrames(QList<qint32> &rKeyFrames) = 0;
};

#endif // VIDEOACCESSOR_H
