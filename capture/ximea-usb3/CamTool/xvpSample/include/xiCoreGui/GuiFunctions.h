#ifndef CXFGFG_H
#define CXFGFG_H

#include <xiCoreGuiGlobal.h>

#include <QIcon>
#include <QMessageBox>

class QToolBar;

class XICOREGUI_API CxGuiFunctions
{
public:
    static void makeToolbarButtonsSameHeight(QToolBar *pToolbar);   //!< Toolbar buttons without icon may be smaller then those that have one - looks horrible

    static QMessageBox::StandardButton warningMsgBox(bool &bShowNextTime, QWidget *parent, const QString &title, const QString &text,
                                                     QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Ok),
                                                     QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);//!< Displays a warning message box with "Do not show next time this message" checkbox.

    static QMessageBox::StandardButton informationMsgBox(bool &bShowNextTime, QWidget *parent, const QString &title, const QString &text,
                                                         QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Ok),
                                                         QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);//!< Displays an information message box with "Do not show next time this message" checkbox.

    static QMessageBox::StandardButton criticalMsgBox(bool &bShowNextTime, QWidget *parent, const QString &title, const QString &text,
                                                         QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Ok),
                                                         QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);//!< Displays a critical message box with "Do not show next time this message" checkbox.

public:
    //static double dpiScaleFactor();     //!< Scale that should be applied to icons (deprecated, remove the usage, GUI is scaled automatically)
    static QPixmap dpiScaledPixmap(const QString &sPath, int iOrigWidth = 0); //!< Image scaled to current DPI
};

#endif // CXFGFG_H
