#ifndef DATA_H
#define DATA_H

#include <xiCoreGlobal.h>
#include <PicBuf.h>
#include <QObject>


/*!
 \brief The base class for all data exchanged by CxChainable objects.
*/
class XICORE_API IxChainData : public QObject
{
    Q_OBJECT

public:
    virtual ~IxChainData(){}

public:

    //! Gets the frame number of the data. For "static" data (e.g. static image) it is allways zero.
    virtual qint32 frameNo() const = 0;


    /*!
     \brief Creates the deep copy of this object.
    If the \p hMemPool is not NULL and it is a valid memory pool, then the data will be allocated from
    the memory pool.
    If the \p hMemPool is NULL then the data will be allocated from heap (operator new).
    */
    virtual IxChainData* clone(TxMemPoolHandle hMemPool, QObject *pMemPoolClient) const = 0;


    /*!
     \brief Creates the deep copy of this object from the same memory source
     as the original data.
    */
    virtual IxChainData *clone(QObject *pMemPoolClient) const = 0;


    /*!
     \brief Checks if the data is from given memory pool.
    */
    virtual bool isFromMemoryPool(TxMemPoolHandle hMemPool) const = 0;
};


#endif // DATA_H
