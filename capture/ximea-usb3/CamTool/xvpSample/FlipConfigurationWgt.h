#ifndef FLIPCONFIGURATIONDLG_H
#define FLIPCONFIGURATIONDLG_H

#include <QWidget>
#include "FlipImageCnbl.h"
namespace Ui {
class CxFlipConfigurationWgt;
}

class CxFlipImageCnbl;

class CxFlipConfigurationWgt : public QWidget
{
    Q_OBJECT

public:
    explicit CxFlipConfigurationWgt(CxFlipImageCnbl *pFlip, QWidget *parent = 0);
    ~CxFlipConfigurationWgt();

private slots:
    void on_chkHoriz_clicked(bool bChecked);
    void on_chkVert_clicked(bool bChecked);
private:
    void setFlip(bool bHorz, bool bVert);
private:
    Ui::CxFlipConfigurationWgt *ui;
    CxFlipImageCnbl *m_pFlip;
};

#endif // FLIPCONFIGURATIONDLG_H
