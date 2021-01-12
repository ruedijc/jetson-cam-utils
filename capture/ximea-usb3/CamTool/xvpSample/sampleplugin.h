#ifndef SAMPLEPLUGIN_H
#define SAMPLEPLUGIN_H

#include <Plugin.h>

class CxMeanGrayMeasForm;
class CxCameraTriggerForm;

class CxSamplePlugin : public QObject, public CxPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ximea.CxSamplePlugin")
    Q_INTERFACES(CxPlugin)

public:
    virtual bool init();
    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings);
    virtual QString name() const;
    virtual QString author() const;
    virtual QString description() const;
    virtual void version(int &major, int &minor, int &build) const;
    virtual QString website() const;
    virtual void activeViewChanged(int iViewId);
    virtual void viewGotNewImage(int iViewId, const CxImageData *pImageData);
    virtual bool viewMouseBtnPressEvent(int iViewId, QGraphicsSceneMouseEvent *event);
    virtual bool viewMouseMoveEvent(int iViewId, QGraphicsSceneMouseEvent *event);
    virtual bool viewMouseBtnReleaseEvent(int iViewId, QGraphicsSceneMouseEvent *event);

protected:
    CxMeanGrayMeasForm *m_pMeanGrayMeasurement;
    CxCameraTriggerForm *m_pSoftwareTrigger;
};

#endif // SAMPLEPLUGIN_H
