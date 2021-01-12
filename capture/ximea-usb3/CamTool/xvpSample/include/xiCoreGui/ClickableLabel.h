#ifndef CLICKABLELABELWIDGET_H
#define CLICKABLELABELWIDGET_H

#include <QLabel>
#include <xiCoreGuiGlobal.h>

//---------------------------------------------------------------------------
class XICOREGUI_API CxClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CxClickableLabel(QWidget *parent = 0);

protected:
    virtual void mouseReleaseEvent(QMouseEvent *event);

signals:
    void clicked();

public slots:

};

//---------------------------------------------------------------------------
class XICOREGUI_API CxColorPickerLabel : public CxClickableLabel
{
    Q_OBJECT
public:
    explicit CxColorPickerLabel(QWidget *parent = 0);
    static QCursor droppperCursor();

    unsigned pickerColor();
    void setPickerColor(unsigned clColor);

signals:
    void colorChanged();

protected:
    virtual void paintEvent(QPaintEvent*);

protected:
    unsigned m_color;  // keep the selected color here (used for simple drawing too)

private slots:
    void on_pickerClicked();
};


#endif // CLICKABLELABELWIDGET_H
