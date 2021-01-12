#ifndef IMAGEFORMATITEM_H
#define IMAGEFORMATITEM_H

#include <QString>
#include <QList>
#include <QMap>
#include <QVariant>
#include <xiCoreGlobal.h>

class CxImageFormat
{
public:
    enum EType
    {
        eSingle     = 0x01, //!< Format can be used for saving single image
        eSequence   = 0x02, //!< Format can be used for saving sequence of images
    };

    QString m_sFormat;      //!< Should be in format "Name (*.ext)"
    int m_iType;            //!< Combination of EType flags

public:
    CxImageFormat(const QString& sFormat, int iType) : m_sFormat(sFormat), m_iType(iType) {}
};

typedef QList<CxImageFormat> TxImageFormatList;         //!< List of supported formats by DataLoades and DataSavers
typedef QMap<QString, QVariant> TxImageFormatOptions;   //!< VariantMap to store settings. Keys must not start with number.

#endif // IMAGEFORMATITEM_H
