#include "CameraTriggerForm.h"
#include "ui_CameraTriggerForm.h"

#include <CameraManager.h>
#include <CameraParameters.h>
#include <Units.h>
#include <xiApi.h>

CxCameraTriggerForm::CxCameraTriggerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CxCameraTriggerForm)
{
    ui->setupUi(this);
    QIcon aIcon;
    aIcon.addFile(":/images/stop.png", QSize(16,16), QIcon::Normal, QIcon::On);
    aIcon.addFile(":/images/start.png", QSize(16,16), QIcon::Normal, QIcon::Off);
    ui->btStart->setIcon(aIcon);

    int iCnt = CxUnits::countInGroup(CxUnits::eugTime);
    for (int i = 0; i < iCnt; i++)
    {
        if (i != (int)CxUnits::etMicrosecond)
            ui->cbUnit->addItem(CxUnits::text(CxUnits::eugTime, i), QVariant(i));
    }
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(doTrigger()));

    CxCameraManager *pCm = CxCameraManager::instance();
    //we need to handle whether a camera was closed or new camera was opened
    connect(pCm, SIGNAL(cameraOpened(XICORE_HANDLE)), this, SLOT(onCameraOpened(XICORE_HANDLE)));
    connect(pCm, SIGNAL(cameraClosed(XICORE_HANDLE)), this, SLOT(onCameraClosed(XICORE_HANDLE)));

    adjustSize();
}

//-----------------------------------------------------------------
CxCameraTriggerForm::~CxCameraTriggerForm()
{
    delete ui;
}

//-----------------------------------------------------------------
void CxCameraTriggerForm::onCameraOpened(XICORE_HANDLE hCamera)
{
    Q_UNUSED(hCamera);
    updateDlg();
}

//-----------------------------------------------------------------
void CxCameraTriggerForm::onCameraClosed(XICORE_HANDLE hCamera)
{
    Q_UNUSED(hCamera);
    updateDlg();
}

//-----------------------------------------------------------------
void CxCameraTriggerForm::showEvent(QShowEvent *)
{
    updateDlg();
}

//-----------------------------------------------------------------
void CxCameraTriggerForm::on_btStart_clicked()
{
    if (m_timer.isActive())
    {
        m_timer.stop();
    }
    else
    {
        m_timer.start(spinValueMsec());
    }
}

//-----------------------------------------------------------------
void CxCameraTriggerForm::on_spinInterval_valueChanged(double val)
{
    Q_UNUSED(val);
    m_timer.setInterval(spinValueMsec());
}


//-----------------------------------------------------------------
void CxCameraTriggerForm::on_cbUnit_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    m_timer.setInterval(spinValueMsec());
}

//-----------------------------------------------------------------
void CxCameraTriggerForm::loadSettings(QSettings *pSettings)
{
    //load the last interval from settings
    double dVal = pSettings->value("CameraTriggerInterval", 1.).toDouble();
    int iUnit = pSettings->value("CameraTriggerUnit", (int)CxUnits::etSecond).toInt();
    ui->spinInterval->setValue(dVal);
    ui->cbUnit->setCurrentIndex(ui->cbUnit->findData(iUnit));
}

//-----------------------------------------------------------------
void CxCameraTriggerForm::saveSettings(QSettings *pSettings)
{
    //save the interval to application settings
    pSettings->setValue("CameraTriggerInterval", ui->spinInterval->value());
    pSettings->setValue("CameraTriggerUnit", ui->cbUnit->currentData().toInt());
}

//-----------------------------------------------------------------
void CxCameraTriggerForm::doTrigger()
{
    // invoke the software trigger on all running cameras
    CxCameraManager *pCm = CxCameraManager::instance();
    QList<XICORE_HANDLE> lstCams = pCm->runningCameras();
    foreach(XICORE_HANDLE hCam, lstCams)
    {
        //Never call the xiAPI's xiSetParam() or xiGetParam() directly!! Always use the CxCameraParameters.
        CxCameraParameters *pParams = pCm->cameraParameters(hCam);
        //This command succeedes only when software trigger is set on camera setting dialog.
        pParams->setParamDirect(XI_PRM_TRG_SOFTWARE, CxValue(1));
    }
}

//-----------------------------------------------------------------
void CxCameraTriggerForm::updateDlg()
{
    if (CxCameraManager::instance()->openedCameraCount() > 0)
    {
        ui->btStart->setEnabled(true);
    }
    else
    {
        ui->btStart->setChecked(false);
        ui->btStart->setEnabled(false);
        m_timer.stop();
    }
}

//-----------------------------------------------------------------
int CxCameraTriggerForm::spinValueMsec()
{
    double dVal = ui->spinInterval->value();
    int iUnits = ui->cbUnit->currentData().toInt();
    dVal = CxUnits::convertValue(dVal, CxUnits::eugTime, iUnits, CxUnits::etMillisecond);
    return (int)dVal;
}
