#include <Chain.h>

#include "FlipConfigurationWgt.h"
#include "ui_FlipConfigurationWgt.h"
#include "FlipImageCnbl.h"

//-------------------------------------------------------------
CxFlipConfigurationWgt::CxFlipConfigurationWgt(CxFlipImageCnbl *pFlip, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CxFlipConfigurationWgt),
    m_pFlip(pFlip)
{
    ui->setupUi(this);
    ui->chkHoriz->setChecked(m_pFlip->m_bFlipHoriz);
    ui->chkVert->setChecked(m_pFlip->m_bFlipVert);
}

//-------------------------------------------------------------
CxFlipConfigurationWgt::~CxFlipConfigurationWgt()
{
    delete ui;
}

//-------------------------------------------------------------
void CxFlipConfigurationWgt::on_chkHoriz_clicked(bool bChecked)
{
    setFlip(bChecked, m_pFlip->m_bFlipVert);
}

//-------------------------------------------------------------
void CxFlipConfigurationWgt::on_chkVert_clicked(bool bChecked)
{
    setFlip(m_pFlip->m_bFlipHoriz, bChecked);
}

//-------------------------------------------------------------
void CxFlipConfigurationWgt::setFlip(bool bHorz, bool bVert)
{
    bool bStopped = m_pFlip->chain()->stopped();
    if (!bStopped)
        m_pFlip->chain()->stop();

    m_pFlip->setTransTypes(bHorz, bVert);

    if (!bStopped)
        m_pFlip->chain()->start();
}
