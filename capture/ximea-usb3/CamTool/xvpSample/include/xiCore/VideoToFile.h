#ifndef VIDEOTOFILE_H
#define VIDEOTOFILE_H

#include <xiCoreGlobal.h>
#include <Chainable.h>
#include <DataSaver.h>


/*!
 * \brief The chainable class that allows to save images coming from camera
 * to a video file.
 */
class XICORE_API CxVideoToFile : public CxChainable
{
    Q_OBJECT

public:
    CxVideoToFile(IxImageDataSaver *pVideoSaver);
    ~CxVideoToFile();

    //CxChainable
public:
    virtual QString title() const;
    virtual bool acceptsDataFrom(CxChainable *pPrecedessor);
    virtual int buffersCountInMemoryPool() const;
    virtual CxChainable* clone();
protected:
    virtual bool init(QString *pErrMsg = NULL);
    virtual bool initialized();
protected slots:
    virtual void onDataAvailable(CxChainable *pSender, int iFrameNo);

    //Own members
signals:
    void frameSaved(int iSeqIndex, quint64 uiTimeStamp);
private:
    IxImageDataSaver *m_pVideoSaver;
    int m_iSeqIndex;
};

#endif // VIDEOTOFILE_H
