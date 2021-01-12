#ifndef BINARYLAYER_H
#define BINARYLAYER_H

#include <QColor>
#include <QList>
#include <QObject>
#include <xiCoreGlobal.h>
#include <PicBuf.h>


class XICORE_API CxBinaryLayer : public QObject
{
    Q_OBJECT
public:
    CxBinaryLayer();
    CxBinaryLayer(uint uiWidth, uint uiHeight);
    ~CxBinaryLayer();

    TxBinaryLayerID id() const;         //!< ID of the layer.
    void setId(TxBinaryLayerID iId);    //!< Used by CxBinaryLayerContainer
    QString name() const;   //!< A custom name of the layer (defalt is "Layer #id").
    void setName(const QString &sName); //!< Sets the custom layer name.
    const SxPicBuf& picbuf() const;
    SxPicBuf& picbuf(); //!< Use with caution!! Change only the buffer's data (m_pData) but nothing more!!!!
    void reset();       //!< Resets the layer's data to zeros.
    void release();        //!< Releases the layer's data.
    bool alloc(uint uiWidth, uint uiHeight); //!< Allocs the layer's data. If the data exists then the allocation is not done and the method returns false.
    const QColor color() const;
    void setColor(const QColor &color);
    int alpha() const;
    void setAlpha(int iAlpha);
    bool visible() const;
    void setVisible(bool bVisible);
    void notifyChanged(); //!< When the layer's data is modified, call this method.

    //! Converts the binary layer to colored RGB32 image with premultiplied rgb values (see QImage::Format_ARGB32_Premultiplied)
    static bool MakeColoredBinary(const CxBinaryLayer *pLayer, SxPicBuf &picRgb32);

    //! Blends the given RGB image (RGB32, RGB24, RGB565) with the binary layer. The resulting \p picBlendedRgb is the image of the same format as \p picRgb.
    static bool BlendRGBWithBinary(const SxPicBuf &picRgb, const CxBinaryLayer *pLayer, SxPicBuf &picBlendedRgb);

protected:
    CxBinaryLayer& operator=(const CxBinaryLayer &rOther);  //!< Prevent to use the operator=.
    void setAsShallowCopyOf(const CxBinaryLayer &rOther);

signals:
    void changed(TxBinaryLayerID idLayer);
    void visibilityChanged(TxBinaryLayerID idLayer);
private:
    TxBinaryLayerID m_iId;
    QString m_sName;
    SxPicBuf m_binary;
    QColor m_color;
    bool m_bVisible;
    bool m_bChanged;
};


#endif // BINARYLAYER_H

