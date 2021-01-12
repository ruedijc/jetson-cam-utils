#ifndef INTERNALPLUGIN_H
#define INTERNALPLUGIN_H

#include <Plugin.h>

/*!
 * \brief The core plugin that registers all core classes (image loaders/savers etc.)
 */
class XICORE_API CxCorePlugin : public QObject, public CxPlugin
{
    Q_OBJECT
    Q_INTERFACES(CxPlugin)

private:
    CxCorePlugin();
public:
    static CxPlugin* instance();

public:
    virtual bool init();
    virtual QString name() const;
    virtual QString author() const;
    virtual QString description() const;
    virtual void version(int &major, int &minor, int &build) const;
    virtual QString website() const;
    virtual QString copyrightNotice() const;
private:
    bool m_bInitialized;
};

#endif // INTERNALPLUGIN_H

