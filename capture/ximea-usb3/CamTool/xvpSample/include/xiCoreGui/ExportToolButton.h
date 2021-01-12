#ifndef CXEXPORTTOOLBUTTON_H
#define CXEXPORTTOOLBUTTON_H

#include <QToolButton>
#include <xiCoreGuiGlobal.h>

class XICOREGUI_API CxExportToolButton : public QToolButton
{
    Q_OBJECT
public:
    enum ExExportType
    {
        extImageToClipboard = 0x01,
        extImageToFile      = 0x02,
        extDataToClipboard  = 0x04,
        extDataToTextFile   = 0x08,
    };
    Q_DECLARE_FLAGS(ExExportTypes, ExExportType)

public:
    explicit CxExportToolButton(QWidget *parent = 0);
    explicit CxExportToolButton(ExExportTypes aSupportedExportTypes, QWidget *parent = 0);
    void addSupportedExportType(ExExportType eType);
    void setExportType(ExExportType eType);
    ExExportType exportType();

    static QString textForType(ExExportType eType);
    static QIcon iconForType(ExExportType eType);

private:
    QMenu *m_pPopupMenu;
    ExExportType m_eSelExportType;

private:
    void init();

private slots:
    void on_exportMenuClicked();
    void on_selfClicked();

signals:
    void exportClicked(int /*ExExportType*/ eType);
};

#endif // CXEXPORTTOOLBUTTON_H
