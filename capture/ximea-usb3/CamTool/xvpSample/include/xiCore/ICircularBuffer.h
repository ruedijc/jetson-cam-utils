#ifndef ICIRCULARBUFFER_H
#define ICIRCULARBUFFER_H

#include <xiCoreGlobal.h>
#include <PicBuf.h>
#include <QVariant>

class CxImageData;

enum ExCircularBufferType
{
	ecbtNone = 0,
	ecbtRAM,
	ecbtRAM_HS,			// high speed recording to RAM using xiAPI buffer
	ecbtHDD,
};

#define CIRC_STORAGE_SEL    "Select"
#define CIRC_STORAGE_RAM	"RAM"
#define CIRC_STORAGE_RAM_HS	"RAM HS"
#define CIRC_STORAGE_HDD	"HDD"
#define CIRC_STORAGE_BROWSE	"Path..."

#define DEFAULT_ROW_HEIGHT 25

enum ExCircularBufferFileOperation
{
	Available = 0,
	Write,
	Read
};

//-----------------------------------------------------
// camera table
#define CAMERA_TABLE_COL_COUNT			10

// column definitions
enum CameraTableCols
{
	IsEnabledCol = 0,			//!< checkbox to mark whether camera will be used
	CamModelCol,				//!< camera model string column
	WriteSpeedCol,				//!? data write speed
	AcqTransferredCol,			//!< frame transfer counter column
	WriteFailCol,				//!< number of failed write operations
	TransportSkippedCol,		//!< transport lost counter column
	TriggerOverlapCol,			//!< trigger overlap counter column
	TriggerFrameBuffOvrCol,		//!< trigger frame buffer overflow counter column
	BufferOverflowCol			//!< frame buffer overflow counter column
};

//-----------------------------------------------------
// storage table
#define STORAGE_COL_COUNT			3
#define STORAGE_COL_DEFAULT_PATH	"..."
#define STORAGE_COL_NO_PATH			"-"
#define STORE_LOC_SEPARATOR         '#'

#define CACHE_MULTI_SELECT_VAL "*"

// column definitions
enum ExStorageTableCols
{
	StorageColErase = 0,		//!< column with erase button
	StorageColType,				//!< column with storage type selection checkbox
	StorageColPath,				//!< column with path for storage
};

//-----------------------------------------------------

#define CIRCBUFFER_PARAM_FILEPATH  "filepath"
#define CIRCBUFFER_PARAM_CACHESIZE "chunk_size"
#define CIRCBUFFER_PARAM_IS_TEST   "is_test"

//!The base class for circular buffers.
class XICORE_API IxCircularBuffer
{
public:
	IxCircularBuffer();
	virtual ~IxCircularBuffer();

	virtual ExCircularBufferType bufferType() const = 0;
	virtual void setBufferType(ExCircularBufferType type) = 0;
	virtual void setCircleSize(qint32 iSize) = 0;								//!< Sets the number of frames in the circular buffer.
	virtual qint32 circleSize() const = 0;										//!< Number of frames in the circular buffer.
	virtual void resetInternals() = 0;											//!< Reset internal variable state of circular buffer
	virtual bool initializeBuffer(const SxPicBufInfo &aPicFormat) = 0;			//!< Initializes (allocates) the circular buffer.
	virtual bool isBufferInitialized() const = 0;								//!< Returns true if the circular buffer is initialized.
	virtual qint32 framesCount() const = 0;										//!< Number of frames captured to the circular buffer.
	virtual qint32 lastFrame() const = 0;										//!< Returns an index of the last captured frame.
	virtual qint32 framesLost() const = 0;										//!< Returns the number of frames lost during the acquistion.
	virtual void framesLostIncrement(int cnt) = 0;								//!? Increments the internal frame lost counter in case chain is too fast.
	virtual bool pushFrame(const CxImageData *pData) = 0;						//!< Pushes new frame to the circular buffer.
	virtual CxImageData* frame(int iIndex, QObject *pMemPoolClient = NULL) = 0; //!< Returns a a copy of the frame with the given index which must be lesser than \ref framesCount().
	virtual bool bufferPicFormat(SxPicBufInfo &aPicFormat) const = 0;			//!< Gets the current image format of the circular buffer.
	virtual quint64 timestamp(qint32 iIndex) const = 0;						    //!< Return a timestamp of a frame with the given index.
	virtual void finishBuffer() = 0;											//!< Finish (deallocate) the circular bufer.
	virtual quint64 bytesAvailable() = 0;										//!< Returns the maximum size of the circular buffer in bytes.
	virtual quint64 frameSize() const = 0;										//!< Size of single frame saved to the circular buffer.
	virtual void finishWriting() = 0;											//!< This method must finish all async. writing operations.
	virtual quint64 currentDataFlow() = 0;										//!< Data flow of the circular buffer in bytes per second (the speed of the data storing to the ciruclar buffer).
	virtual quint64 averageDataFlow() = 0;										//!< The average data flow of the circ. buffer in bytes per second (it makes sense to call this method after the chain was stopped).
	virtual quint64 minDataFlow() = 0;											//!< The minimum data flow in bytes per second detected in a single frame.

	virtual bool checkCircBufferCompatible(const SxPicBufInfo &aPicFormat);		//!< Returns true if the given \p aPicFormat is compatible with the circular buffer image format.
	virtual bool setParam(const QString &paramName, const QVariant &value);
	virtual QVariant param(const QString &paramName);
};

#endif // ICIRCULARBUFFER_H
