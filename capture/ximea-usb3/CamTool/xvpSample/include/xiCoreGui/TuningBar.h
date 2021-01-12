#ifndef TUNINGBAR_H
#define TUNINGBAR_H

#include <QWidget>
#include <xiCoreGuiGlobal.h>
#include <ValueTypes.h>

class CxTuningBar;

//! \brief Our own implementation of slides, working with CxValue types etInt and etFloat
class IxTuningBarDelegate
{
public:
    virtual bool tuningBarEnableScroll(CxTuningBar* pTuningBar) = 0;
};

//---------------------------------------------------------------------------
//! \brief Our own implementation of slides, working with CxValue types etInt and etFloat
class XICOREGUI_API CxTuningBar : public QWidget
{
    Q_OBJECT
public:
    explicit CxTuningBar(QWidget *parent = 0);

public:
    enum ExBarColorScale
    {
        ebcsNone,
        ebcsGray,
        ebcsRed,
        ebcsGreen,
        ebcsBlue,
        ebcsColorful,
    };
    enum ExBarTransform
    {
        ebtLinear,      // default
        ebtLogarithmic,  // for exposure
    };

    bool m_bShowTickMarks;
    bool m_bSnapToTickMarks;
    ExBarColorScale m_eColorScale;
    ExBarTransform m_eTransform;

    void setRange(const CxValue &vMin, const CxValue &vMax, const CxValue &vIncrement);
    const CxValue& rangeMin() const;
    const CxValue& rangeMax() const;
    const CxValue& increment() const;
    const CxValue& value() const;

protected:
    CxValue m_vRangeMin, m_vRangeMax, m_vIncrement;
    CxValue m_vValue;
    bool m_bReadOnly;
    IxTuningBarDelegate *m_pDelegate;
    static QPixmap *m_pPicArrow;// arrow image, loaded only once
    static QPixmap *m_pPicArrowDis; // arrow image, loaded only once
    static QPixmap *m_pPicBarColorful;  // colorful color scale, loaded only once
    bool m_bMovingArrow;
    int calcPaintXPos(const CxValue &vValue);  // value to view coordinate
    CxValue calcValueFromPos(int nXPos);
    bool setValueAndEmitSignal(CxValue vNewVal);

    CxValue smallStep(int iRangeFraction, bool bUp);

    // geometry helpers
protected:
    int m_iBorder_X, m_iSnapDist;

protected:
    virtual QSize sizeHint() const;
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
#ifndef QT_NO_WHEELEVENT
    virtual void wheelEvent(QWheelEvent *event);
#endif
    virtual void keyPressEvent(QKeyEvent *event);

signals:
    void valueChanged(const CxValue &vValue);

public slots:
    void setValue(const CxValue &vValue);
    void setReadOnly(bool bReadOnly);
    void setDelegate(IxTuningBarDelegate *pDelegate);
    void processExternalWheelEvent(QWheelEvent *event);
};

#endif // TUNINGBAR_H
