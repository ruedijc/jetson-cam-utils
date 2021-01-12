#ifndef IMAGEFORMATMANAGER_H
#define IMAGEFORMATMANAGER_H

#include <xiCoreGlobal.h>
#include <QStringList>
#include <DataLoader.h>
#include <DataSaver.h>
#include <xiRtti.h>

class QSettings;
class CxImageMetadata;

//! \brief Keeps list of supported file formats using CxRTTI, creates loader and saver objects.
class XICORE_API CxImageFormatManager : public QObject
{
    Q_OBJECT

public:
    static CxImageFormatManager* instance(); //!< Singleton class, this is the way to obtain the pointer

private:
    CxImageFormatManager();

public:
    //! All formats we are capable of loading, items as "Name (*.ext)"
    QStringList formatsForLoad(int iFormatTypes = CxImageFormat::eSingle);
    //! All formats we are capable of saving, items as "Name (*.ext)"
    QStringList formatsForSave(int iFormatTypes = CxImageFormat::eSingle);
    //! All formats we are capable of loading in format suitable for QFileDialog
    QString stringWithAllFormatForLoad(int iFormatTypes = CxImageFormat::eSingle, bool bIncludeAllImagesFilter = true);
    //! All formats we are capable of saving in format suitable for QFileDialog
    QString stringWithAllFormatForSave(int iFormatTypes = CxImageFormat::eSingle);

    IxImageDataLoader* loaderForFile(const QString& sFileName);  //!< Creates new image loader instance according to the file extension
    IxImageDataSaver* saverForFile(const QString& sFileName);    //!< Creates new image saver instance according to the file extension

    TxImageFormatOptions formatOptions(const QString &sExt);     //!< get defaults for file format
    void setFormatOptions(const QString &sExt, const TxImageFormatOptions &aOpt);   //!< set defaults for file format
    void saveSettings(QSettings *pSettings);    //!< store format options to app settings
    void loadSettings(QSettings *pSettings);    //!< load format options from app settings

    // helpers
public:
    static QString fileExtFromFormatSignature(const QString& sFormatItem);  //!< returns "ext" from "name (*.ext)"
    static bool loadPicBufFromFile(const QString& sFileName, SxPicBuf &rPic, CxImageMetadata *pMetadata = NULL);           //!< Helper function to load one picture. The result is stored to rPic, caller is responsible freeing it.
    static bool savePicBufToFile(const QString& sFileName, const SxPicBuf *pPic, const CxImageMetadata *pMetadata = NULL, const TxImageFormatOptions *pOpt = NULL); //!< Helper function to store one picutre

private slots:
    void onRttiItemEnableStateChanged(TxRttiItem hItem, bool bEnabled); //!< Handle the CxRTTI changes.

private:
    void resetFormatsForLoad();
    void resetFormatsForSave();
    void loadAvailableLoaders(); //!< Asks our CxRTTI to return all available loader classes
    void loadAvailableSavers(); //!< Asks our CxRTTI to return all available saver classes
private:
    class CxFormatItem
    {
    public:
        QString m_sRttiClass;               //!< CxRTTI class name
        TxImageFormatList m_lstFormats;     //!< list of supported formats
    };

    bool m_bLoaderFormatsLoaded; //!< Did we call loadAvailableLoaders()?
    bool m_bSaverFormatsLoaded; //!< Did we call loadAvailableSavers()?
    QList<CxFormatItem> m_lstFormatsForLoad;
    QList<CxFormatItem> m_lstFormatsForSave;
    QMap<QString, TxImageFormatOptions> m_mapFormatOptions; //!< maps "ext" to its options (not class!)

    static QString findClassForFormat(const QList<CxFormatItem>&lst, const QString& sExt);
};

#endif // IMAGEFORMATMANAGER_H
