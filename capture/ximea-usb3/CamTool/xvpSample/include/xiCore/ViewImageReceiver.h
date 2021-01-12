#ifndef _VIEWIMAGERECEIVER_H_
#define _VIEWIMAGERECEIVER_H_

#include <xiCoreGlobal.h>
#include <Rgb32Convertor.h>

#include <QWidget>

/*!
 * \brief This class is primarily used by images views to receive a image data
 * from a chain.
 * \remarks It converts the incoming data to RGB32 and creates an instance
 * of \ref CxViewImageData for the view.
 * \warning It is not recommended to use this class in plugins or chainables because the usage of this
 * classe somewhere in the middle of the chain can have a lot of unwanted side effects! Just forget this
 * class exists and always work with CxImageData.
 */
class XICORE_API CxViewImageReceiver : public CxRgb32Convertor
{
    Q_OBJECT

public:
    Q_INVOKABLE CxViewImageReceiver();

    void setRenderingEnabled(bool bEnable); //!< Enable rendering in view. Can be disabled when wanting to get higest possible FPS on camera.
    bool isRenderingEnabled();                //!< Returns true when rendering is enabled
    void setForceFpsLimit(bool bForce, int iFpsLimit = -1); //!< User-defined fps limit. Limit should be above zero.
    bool isFpsLimitForced(int *piFpsLimit = NULL);

protected:
    bool m_bRenderingEnabled;   //!< Enable rendering in view
    bool m_bForceFpsLimit;      //!< User-set fps limit (when false, our automatics code is used)
    int m_iFpsLimitForced;     //!< User-set fps limit
    double m_dFpsLimitAutomatic;  //!< Fps limit as defined by CamTool (from monitor frequency)

    //CxChainable
public:
    virtual QString title() const;
    virtual CxChainable* clone();
    virtual int buffersCountInMemoryPool() const;
    virtual int queryImagesCountInMemoryPool(const SxPicBufInfo &picInfo, TxMemPoolHandle hPool);
    virtual QWidget* configurationWidget();
    virtual bool hasConfigurationWidget() const;
    virtual void setMaxFps(double dFps);

protected:
    virtual IxChainData* processData(IxChainData *pReceivedData);
};

class CxViewReceiverConfWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CxViewReceiverConfWidget(CxViewImageReceiver *pReceiver, QWidget *parent = 0);
    ~CxViewReceiverConfWidget();

private slots:
    void on_chkRendering_clicked(bool bChecked);
    void on_rbFpsAuto_clicked(bool bChecked);
    void on_rbFpsForced_clicked(bool bChecked);
    void on_spinFps_edited(int iValue);

private:
    CxViewImageReceiver *m_pReceiver;
};

#endif //_VIEWIMAGERECEIVER_H_

