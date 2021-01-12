#ifndef SOFTWARETRIGGERFORM_H
#define SOFTWARETRIGGERFORM_H

#include <QWidget>
#include <QTimer>
#include <xiCoreGlobal.h>

class QSettings;

namespace Ui {
class CxCameraTriggerForm;
}

class CxCameraTriggerForm : public QWidget
{
    Q_OBJECT

public:
    explicit CxCameraTriggerForm(QWidget *parent = 0);
    ~CxCameraTriggerForm();

    void loadSettings(QSettings *pSettings);
    void saveSettings(QSettings *pSettings);

private slots:
    void on_btStart_clicked();
    void on_spinInterval_valueChanged(double val);
    void on_cbUnit_currentIndexChanged(int index);
    void onCameraOpened(XICORE_HANDLE hCamera);
    void onCameraClosed(XICORE_HANDLE hCamera);
    void doTrigger();
protected:
    virtual void showEvent(QShowEvent *);

private:
    void updateDlg();
    int spinValueMsec();//get value from spin box converted to msec
private:
    Ui::CxCameraTriggerForm *ui;
    QTimer m_timer;

};

#endif // SOFTWARETRIGGERFORM_H
