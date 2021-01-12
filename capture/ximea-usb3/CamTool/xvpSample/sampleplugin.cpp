#include "sampleplugin.h"

#include <xiRtti.h>
#include <AppDelegate.h>
#include <xiCoreVersion.h>

#include "NegativeChnbl.h"
#include "BayerPseudoColorsChnbl.h"
#include "MeanGrayMeasForm.h"
#include "FlipImageCnbl.h"
#include "CameraTriggerForm.h"

//---------------------------------------------------------------------------
bool CxSamplePlugin::init()
{
    // register plugins that work as simple chainables - are added to chain upon request
    xiRTTI_REGISTER(this, CxNegativeChnbl, "CxChainable");
    xiRTTI_REGISTER(this, CxBayerPseudoColorsCnbl, "CxChainable");
    xiRTTI_REGISTER(this, CxFlipImageCnbl, "CxChainable");

    // create plugin for measurement of mean gray on active image
    m_pMeanGrayMeasurement = new CxMeanGrayMeasForm;
    IxAppDelegate::instance()->addDockedWidget(m_pMeanGrayMeasurement, Qt::BottomDockWidgetArea,
                                               Qt::TopDockWidgetArea|Qt::BottomDockWidgetArea, false);

    m_pSoftwareTrigger = new CxCameraTriggerForm;
    IxAppDelegate::instance()->addDockedWidget(m_pSoftwareTrigger, Qt::RightDockWidgetArea,
                                               Qt::AllDockWidgetAreas, false);

    return true;
}

//---------------------------------------------------------------------------
QString CxSamplePlugin::name() const
{
    return tr("Sample plugin");
}

//---------------------------------------------------------------------------
QString CxSamplePlugin::author() const
{
    return QString("XIMEA GmbH");
}

//---------------------------------------------------------------------------
QString CxSamplePlugin::description() const
{
    return tr("This is a plugin for educational purpose. On simple examples it demonstrates"\
              "a plugin development, interface registration, chainable object implementation,"\
              "communication of the plugin with the application etc.");
}

//---------------------------------------------------------------------------
void CxSamplePlugin::version(int &major, int &minor, int &build) const
{
    xiCoreVersion(major, minor, build);
}

//---------------------------------------------------------------------------
QString CxSamplePlugin::website() const
{
    return QString("http://www.ximea.com");
}

//---------------------------------------------------------------------------
void CxSamplePlugin::loadSettings(QSettings *pSettings)
{
    m_pSoftwareTrigger->loadSettings(pSettings);
}

//---------------------------------------------------------------------------
void CxSamplePlugin::saveSettings(QSettings *pSettings)
{
    m_pSoftwareTrigger->saveSettings(pSettings);
}

//---------------------------------------------------------------------------
void CxSamplePlugin::activeViewChanged(int iViewId)
{
    m_pMeanGrayMeasurement->activeViewChanged(iViewId);
}

//---------------------------------------------------------------------------
void CxSamplePlugin::viewGotNewImage(int iViewId, const CxImageData *pImageData)
{
    m_pMeanGrayMeasurement->viewGotNewImage(iViewId, pImageData);
}

//---------------------------------------------------------------------------
bool CxSamplePlugin::viewMouseBtnPressEvent(int iViewId, QGraphicsSceneMouseEvent *event)
{
    return m_pMeanGrayMeasurement->viewMouseBtnPressEvent(iViewId, event);
}

//---------------------------------------------------------------------------
bool CxSamplePlugin::viewMouseMoveEvent(int iViewId, QGraphicsSceneMouseEvent *event)
{
    return m_pMeanGrayMeasurement->viewMouseMoveEvent(iViewId, event);
}

//---------------------------------------------------------------------------
bool CxSamplePlugin::viewMouseBtnReleaseEvent(int iViewId, QGraphicsSceneMouseEvent *event)
{
    return m_pMeanGrayMeasurement->viewMouseBtnReleaseEvent(iViewId, event);
}
