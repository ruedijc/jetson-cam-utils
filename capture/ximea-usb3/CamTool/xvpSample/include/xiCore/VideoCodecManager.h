#ifndef VIDEOCODECMANAGER_H
#define VIDEOCODECMANAGER_H

#include <QPair>
#include <QString>
#include <QList>
#include <xiCoreGlobal.h>
#include <VideoEncoder.h>
#include <VideoDecoder.h>
#include <xiRtti.h>

/*!
 * \brief The class that manages video codecs used by video loaders and savers.
 */
class XICORE_API CxVideoCodecManager : public QObject
{
    Q_OBJECT
public:
    static CxVideoCodecManager* instance(); //!< Singleton class, this is the way to obtain the pointer

private:
    CxVideoCodecManager();

public:
    IxVideoEncoder* encoderForFourCC(TxFourCC fcc);
    IxVideoDecoder* decoderForFourCC(TxFourCC fcc);

    QList<QPair<TxFourCC, QString> > encoders(); //!< Returns a list of encoders with their FourCCs and names.
    QList<QPair<TxFourCC, QString> > decoders(); //!< Returns a list of decoders with their FourCCs and names.

    void setCodecOptions(TxFourCC fccCodec, const TxImageFormatOptions &aOpt);   //!< set defaults codec
    void saveSettings(QSettings *pSettings);    //!< store format options to app settings
    void loadSettings(QSettings *pSettings);    //!< load format options from app settings

private slots:
    void onRttiItemEnableStateChanged(TxRttiItem hItem, bool bEnabled); //!< Handle the CxRTTI changes.

private:
    struct SxCodecItem
    {
        QString m_sRttiClass;
        QString m_sName; //!< Localized name of the codec.
        TxFourCC m_fcc;
    };

private:
    void resetEncoders();
    void resetDecoders();
    void loadAvailableEncoders();
    void loadAvailableDecoders();

    static QString findClassForFourCC(const QList<SxCodecItem>& lstCodecs, TxFourCC fourCC);


private:
    bool m_bEncodersLoaded;
    bool m_bDecodersLoaded;
    QList<SxCodecItem> m_lstEncoders;
    QList<SxCodecItem> m_lstDecoders;
    QMap<TxFourCC, TxImageFormatOptions> m_mapCodecOptions; //!< maps FourCC of a codec to its options
};

#endif // VIDEOCODECMANAGER_H
