#ifndef PLUGIN_H
#define PLUGIN_H

#include <QtPlugin>
#include <xiCoreGlobal.h>

class QGraphicsSceneMouseEvent;
class QSettings;
class CxImageData;

/*!
 * \brief Base class of all plugins. For more informations about plugin development see the \ref plugins.
 */
class XICORE_API CxPlugin
{
public:
    virtual ~CxPlugin();

public:
    virtual bool init();                        //!< Called on application start
    virtual void finish();                      //!< Called on application exit
    virtual void loadSettings(QSettings *pSettings); //!< Called right after init(), and then while loading program state
    virtual void saveSettings(QSettings *pSettings); //!< Called before finish(), and when program state is saved

    virtual QString copyrightNotice() const;    //!< In case plugin needs to write some legal info to Help - About - Legal...
    virtual QString name() const;               //!< Plugin name displayed in the Plugin Manager view.
    virtual QString author() const;             //!< Name of the author (company) of this plugin.
    virtual QString description() const;        //!< Short description of the plugin, notes, warnings etc.
    virtual void version(int &major, int &minor, int &build) const; //!< Version of the plugin.
    virtual QString version() const;            //!< Returns a version string. The default implementation returns a string formatted as M.m.b (i.e. Major.minor.build).
    virtual QString website() const;            //!< The author's website adress.

public:
    virtual void activeViewChanged(int iViewId);            //!< When new MDI view activated (or created)
    virtual void viewAboutToClose(int iViewId);             //!< The view will closed and deleted immediatelly.
    virtual void newCameraConnected(XICORE_HANDLE hCamera); //!< After new camera connected after app start
    virtual void cameraDisconnected(XICORE_HANDLE hCamera); //!< After new camera disconnected during app session (not when exiting)

public:
    virtual void viewGotNewImage(int iViewId, const CxImageData *pImageData); //!< New image received when acqusition running
    virtual bool viewMouseBtnPressEvent(int iViewId, QGraphicsSceneMouseEvent *event);   //!< Return true to prevent further processing
    virtual bool viewMouseMoveEvent(int iViewId, QGraphicsSceneMouseEvent *event);       //!< Return true to prevent further processing
    virtual bool viewMouseBtnReleaseEvent(int iViewId, QGraphicsSceneMouseEvent *event); //!< Return true to prevent further processing
    virtual bool viewMouseDoubleClickEvent(int iViewId, QGraphicsSceneMouseEvent *event);//!< Return true to prevent further processing

public:
    virtual bool customCommand(const QString &sCmdName, int iParam1, int iParam2, void *pParam1, void *pParam2);    //!< for internal use.
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(CxPlugin, "com.ximea.CxPlugin")
QT_END_NAMESPACE

#endif // PLUGIN_H
