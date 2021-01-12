#ifndef MDIVIEW_H
#define MDIVIEW_H

#include <xiCoreGlobal.h>
#include <QWidget>

class CxChainable;
class CxImageSource;

//!The base of all MDI subwindows in the application.
class XICORE_API CxMdiView : public QWidget
{
    Q_OBJECT
public:
    explicit CxMdiView(CxChainable *pDataProvider, CxChainable *pDataReceiver);
    virtual ~CxMdiView();

    void closeMdiView();

    int viewId();                   //!< Returns ID of this view
    void setViewId(int iViewId);    //!< Do not use, main window sets this automatically

    enum ExViewCap
    {
        evcCanZoomInOut     = 0x01, //!< Enable zooming using + / - toolbar buttons
        evcCanZoomFit       = 0x02, //!< Enable zooming using bestFit toolbar button
        evcCanSaveToFile    = 0x04, //!< This view can save some data to file, enable toolbar button, and calling
        evcCanCapture       = 0x10, //!< Is reasonable to make snaphots of the data inside the view
        evcCanRecordVideo   = 0x20, //!< This view can record video
    };
    Q_DECLARE_FLAGS(ExViewCaps, ExViewCap)

protected:    
    CxChainable *m_pDataReceiver;   //!< The chainable object as a data receiver.
    int m_iViewId;

signals:
    void receivedFrame(qint32 iFrame);                //!< New data arrived through chain to the view
    void requestAdjustWindowSize(CxMdiView *pSender); //!< Emit when parent MdiWindow should adjust its size to this view's "sizeHint() const"
    void requestCloseWindow(CxMdiView *pSender);      //!< For internal use only, called from \ref closeMdiView()

public slots:
    virtual void zoomIn();          //!< Change zoom as user clicked on the toolbar button
    virtual void zoomOut();         //!< Change zoom as user clicked on the toolbar button
    virtual void zoomFitToScreen(); //!< Change zoom as user clicked on the toolbar button
    virtual void updateViewTitle();

public:
    virtual ExViewCaps viewCaps();
    virtual bool containsData() = 0;        //!< Check whether view shows any data (maybe image view did not recieve any imageData)
    virtual bool canZoomIn();
    virtual bool canZoomOut();
    virtual bool save(QString& sLastUsedFolder);  //!< Called from toolbar button, you should show the SaveAs dialog, or save the data directly.
    virtual bool saveToFile(const QString& sFilename);  //!< Save contents to file
    virtual bool recordVideoToFile(const QString& sFilename);  //!< Start recording the video
    virtual bool stopVideoRecording();      //!< Stop video recording started with recordVideoToFile
    virtual CxMdiView* clone(CxChainable *pNewDataProvider, CxChainable *pNewDataReceiver) = 0;
    CxChainable* dataProvider() const;      //!< Returns chainable object view is connected to
    CxChainable* dataReceiver() const;      //!< Returns view's helper object (that is connected to dataProvider)
    CxImageSource* imageSource() const;     //!< Image source in the chain view is connected to
    virtual QString viewTitle() const;      //!< Returns the view title excluding additional decorations (i.e. w/ zoom factor, fps)
};

#endif // MDIVIEW_H
