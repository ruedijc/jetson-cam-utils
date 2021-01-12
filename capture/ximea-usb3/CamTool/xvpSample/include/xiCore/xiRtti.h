#ifndef XIRTTI_H
#define XIRTTI_H

#include <xiCoreGlobal.h>
#include <QObject>
#include <QStringList>
#include <QSettings>

class CxPlugin;
typedef void* TxRttiItem;


/*! \brief Our RTTI builds upon Qt meta object system, and just adds class discovery

     Registered interfaces are: CxChainable, IxImageDataLoader, IxImageDataSaver,
                                IxVideoEncoder, IxVideoDecoder
*/
class XICORE_API CxRTTI : public QObject
{
    Q_OBJECT
public:
    //! This method is called from xiRTTI_REGISTER macro, do not use directly
    void registerClass(CxPlugin *pPlugin, const QMetaObject& aClassMetaObj, const QString& sInterface);

    //! Queries available classes - get all classes that implement given interface
    QStringList classesForInterface(const QString& sInterface, bool bEnabledOnly = true);

    //! Returns an interface for the given registered class name. It returns empty string when the class is not registered.
    QString interfaceForClass(const QString &sClass);

    //! Title that should be presented to user when enumerating the class
    QString customNameForClass(const QString& sClass);

    //! UniqueID of the class (it may not exist and the method may return empty string in that case).
    QString uniqueIdForClass(const QString& sClass);

    //! Returns a classname for given UniqueID of chainable (if it exists).
    QString classForUniqueId(const QString &sUniqueId);

    /*! Creates instance of the class and casts for requected interface.
        Important: the default constructor of that class must be declared as Q_INVOKABLE! */
    template<class T> T createInstanceAndCast(const QString& sClass)
    {
        return dynamic_cast<T>(createInstanceOfClass(sClass));
    }

    //! Checks whether the corresponding Rtti item is enabled (used by plugin manager).
    bool itemEnabled(TxRttiItem hItem);

    //! Sets the corresponding Rtti item enabled/disabled.
    void setItemEnabled(TxRttiItem hItem, bool bEnable);

    //! Gets the "CustomName" of the corresponding Rtti item from metadata of the object.
    QString itemName(TxRttiItem hItem);

    //! Returns Rtti items grouped by interface.
    QMap<QString, QList<TxRttiItem> > itemsByInterface();

    //! Returns a plugin from which this item was registered.
    const CxPlugin* pluginForItem(TxRttiItem hItem) const;

    //! Returns a list of rtti items registered by the plugin.
    QList<TxRttiItem> itemsForPlugin(const CxPlugin *pPlugin) const;

    //! Returns the interface which this item implements.
    QString interfaceForItem(TxRttiItem hItem);

    //! Saves the current state of the Rtti items (their enable state etc.)
    void saveSettings(QSettings *pSettings);

    //! Loads the state of Rtti items.
    void loadSettings(const QSettings *pSettings);

protected:
    //! Helper function. Use createInstanceAndCast instead.
    QObject* createInstanceOfClass(const QString& sClass);

    //! If the Rtti item class has not the CustomName then this function generates a default name.
    QString defaultNameForInterfaceClass(const QString &interfaceClass);

    //! Returns the name from metadata for given class and metadata tag.
    QString nameFromMetadataForClass(const QString &sClass, const QString &sMetadataTag);

signals:
    //! The signal is emitted from \ref setItemEnabled() method every time the item's enable state changes.
    void itemEnableStateChanged(TxRttiItem hItem, bool bEnabled);
private:
    CxRTTI();
public:
    static CxRTTI* instance();
};

//------------------------------------------------------------
/*! The following macro will register the class cl into our simple RTTI.
    Use in some main library function cpp file (only once).
*/
#define xiRTTI_REGISTER(plugin, cl, interf) CxRTTI::instance()->registerClass(plugin, cl::staticMetaObject, interf);

#endif // XIRTTI_H
