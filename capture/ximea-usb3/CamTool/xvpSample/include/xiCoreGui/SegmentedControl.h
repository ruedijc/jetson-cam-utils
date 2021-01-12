#ifndef SEGMENTEDCONTROL_H
#define SEGMENTEDCONTROL_H

#include <QWidget>
#include <xiCoreGuiGlobal.h>

class QToolButton;

//! \brief Set of exclusively checkable buttons next to each other, inspired by OS X
class XICOREGUI_API CxSegmentedControl : public QWidget
{
    Q_OBJECT
public:
    explicit CxSegmentedControl(QWidget *parent = 0);

    void setSegmentCount(int nCount);
    void setSelecedSegment(int iIdx);
    int selectedIndex();
    void setSegmentText(int iIdx, const QString &sText, const QString &sSearchText = QString());
    void setSegmentIcon(int iIdx, const QIcon &rIcon, int iWidth = -1, int iHeight = -1);
    bool hasMatchingSegment(const QString &sFilter);

protected:
    int m_nSegmentCount;

    QToolButton* segmentBtn(int iIdx);

protected:
    virtual bool eventFilter(QObject *pObject, QEvent *pEvent);

signals:
    void selectedSegmentChanged();

private slots:
    void buttonPressed();

};

#endif // SEGMENTEDCONTROL_H
