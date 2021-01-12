#include "MeanGrayMeasForm.h"
#include "ui_MeanGrayMeasForm.h"

#include <QDockWidget>
#include <QMainWindow>
#include <QGraphicsSceneMouseEvent>

#include <AppDelegate.h>
#include <ImageData.h>
#include <PicBufMacros.h>

#define KEY_VIEWID  1           // user data at vector object

CxMeanGrayMeasForm::CxMeanGrayMeasForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CxMeanGrayMeasForm)
{
    ui->setupUi(this);

    m_bDrawingNewRect = false;
    m_iLastRecIdx = 0;

    ui->btnNewMeas->setEnabled(false); // enable after some view activated
    ui->tableResults->verticalHeader()->setDefaultSectionSize(ui->tableResults->fontMetrics().height()+5);  // set row height
    enableTableButtons();
}

//---------------------------------------------------------------------------
CxMeanGrayMeasForm::~CxMeanGrayMeasForm()
{
    delete ui;
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasForm::activeViewChanged(int iViewId)
{
    // when no view, disable drawing new measurement rectangle
    ui->btnNewMeas->setEnabled(iViewId != -1);

    // also when user clicked on the button and then closed all views, uncheck the button
    if (iViewId == -1 && ui->btnNewMeas->isChecked())
        ui->btnNewMeas->setChecked(false);
}

//---------------------------------------------------------------------------
bool CxMeanGrayMeasForm::viewMouseBtnPressEvent(int iViewId, QGraphicsSceneMouseEvent *event)
{
    if (ui->btnNewMeas->isChecked() && event->button() == Qt::LeftButton)
    {
        IxAppDelegate *pAppDelegate = IxAppDelegate::instance();
        QPointF pt = event->scenePos();

        // start drawing new measurement rectangle
        CxVectorObjectRect *pRect = new CxVectorObjectRect;
        pRect->setPenColor(QColor(128, 255, 128));
        pRect->setCaptionColor(QColor(128, 255, 128));
        pRect->setCoordIncrement(1, 1);     // only integer numbers
        pRect->setSizeIncrement(1, 1);      // width and height as well
        pRect->setKeepInScene(true);        // rectangle needs to stay in the image
        pRect->setRect(pt.x(), pt.y(), 1, 1);   // initial rectangle
        pRect->setData(KEY_VIEWID, iViewId);    // needed later in our rectChanging event
        m_idRectDrawing = pAppDelegate->addVectorObject(iViewId, pRect);
        m_bDrawingNewRect = true;
        m_ptStartRectDrawing = pt;
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool CxMeanGrayMeasForm::viewMouseMoveEvent(int iViewId, QGraphicsSceneMouseEvent *event)
{
    if (m_bDrawingNewRect)
    {
        // continue drawing rectangle
        IxAppDelegate *pAppDelegate = IxAppDelegate::instance();
        CxVectorObjectRect *pRect = qobject_cast<CxVectorObjectRect*>(pAppDelegate->vectorObjectPtr(iViewId, m_idRectDrawing));
        if (pRect != NULL)
        {
            QPointF pt = event->scenePos();
            QRectF rc = QRectF(m_ptStartRectDrawing, pt).normalized();
            pRect->setRect(rc);
        }
        return true;
    }
    else if (ui->btnNewMeas->isChecked())
    {
        IxAppDelegate *pAppDelegate = IxAppDelegate::instance();
        QCursor aCur(Qt::CrossCursor);
        pAppDelegate->overrideViewCursor(iViewId, &aCur);
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool CxMeanGrayMeasForm::viewMouseBtnReleaseEvent(int iViewId, QGraphicsSceneMouseEvent* /*event*/)
{
    if (m_bDrawingNewRect)
    {
        // see the size - when too small, delete it
        IxAppDelegate *pAppDelegate = IxAppDelegate::instance();
        CxVectorObjectRect *pRect = qobject_cast<CxVectorObjectRect*>(pAppDelegate->vectorObjectPtr(iViewId, m_idRectDrawing));
        if (pRect != NULL)
        {
            QRectF rc = pRect->rect();
            if (rc.isEmpty())
                pAppDelegate->removeVectorObject(iViewId, m_idRectDrawing); // too small
            else
            {
                //  measure this rectangle, and add the result to our table
                CxMeasRecord aData;
                if (measureData(aData, pAppDelegate->viewCurrentImage(iViewId), rc))
                {
                    aData.m_iViewId = iViewId;
                    aData.m_idObj = pRect->id();
                    aData.m_iRecordIdx = ++m_iLastRecIdx;
                    m_vecData.push_back(aData);

                    // add to table
                    updateaTableRow(m_vecData.count()-1, aData);

                    // listen to further changes to be able to remeasure when rectangle changed
                    connect(pRect, SIGNAL(rectChanging(QRectF)), this, SLOT(on_rectChanging(QRectF)));

                    // show the record idx as the label of the rectangle
                    pRect->setCaption(QString::number(aData.m_iRecordIdx));
                }
            }
        }

        m_bDrawingNewRect = false;
        m_idRectDrawing = 0;
        ui->btnNewMeas->setChecked(false);
        pAppDelegate->overrideViewCursor(iViewId, NULL);
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
// make sum for all supported data types.
template<typename T, typename TSUM>
TSUM measuredata_sum_rect_T(const SxPicBuf &aPic, int iRcLeft, int iRcTop, int iRcWidth, int iRcHeight)
{
    TSUM uiSum = 0;
    if (aPic.m_uiChannels == 1)
    {
        for (int y = 0; y < iRcHeight; y++)
        {
            T *pLine = ROW(T, aPic, y + iRcTop) + iRcLeft;
            for (int x = 0; x < iRcWidth; x++)
                uiSum += *pLine++;
        }
    }
    else if (aPic.m_uiChannels == 3)
    {
        for (int y = 0; y < iRcHeight; y++)
        {
            T *pLine = ROW(T, aPic, y + iRcTop) + 3*iRcLeft;
            for (int x = 0; x < iRcWidth; x++)
            {
                uiSum += ((TSUM)pLine[0] + pLine[1] + pLine[2])/3;
                pLine += 3;
            }
        }
    }
    else if (aPic.m_uiChannels == 4 && aPic.m_bIsRgb)    // RGBA (skip A)
    {
        for (int y = 0; y < iRcHeight; y++)
        {
            T *pLine = ROW(T, aPic, y + iRcTop) + 4*iRcLeft;
            for (int x = 0; x < iRcWidth; x++)
            {
                uiSum += ((TSUM)pLine[0] + pLine[1] + pLine[2])/3;
                pLine += 4;
            }
        }
    }
    else    // generic number of channels
    {
        int ch, uiChannels = (int)aPic.m_uiChannels;
        TSUM uiPixSum;
        for (int y = 0; y < iRcHeight; y++)
        {
            T *pLine = ROW(T, aPic, y + iRcTop) + 4*iRcLeft;
            for (int x = 0; x < iRcWidth; x++)
            {
                uiPixSum = 0;
                for (ch = 0; ch < uiChannels; ch++)
                    uiPixSum += pLine[ch];
                uiSum += uiPixSum/uiChannels;
                pLine += uiChannels;
            }
        }
    }
    return uiSum;
}

//---------------------------------------------------------------------------
bool CxMeanGrayMeasForm::measureData(CxMeasRecord &aData, const CxImageData *pImageData, const QRectF &rc)
{
    int iRcWidth = (int)rc.width(), iRcHeight = (int)rc.height();
    int iRcTop = (int)rc.top(), iRcLeft = (int)rc.left();
    aData.m_dWidth = iRcWidth;
    aData.m_dHeight = iRcHeight;
    aData.m_dArea = aData.m_dWidth * aData.m_dHeight;

    aData.m_dMeanGray = 0;
    if (pImageData != NULL)
    {
        double dSum = 0;
        const SxPicBuf &aPic = pImageData->picBuf();

        if (aPic.m_eDataType == extypeUInt8)
            dSum = measuredata_sum_rect_T<quint8, quint64>(aPic, iRcLeft, iRcTop, iRcWidth, iRcHeight);
        else if (aPic.m_eDataType == extypeUInt16)
            dSum = measuredata_sum_rect_T<quint16, quint64>(aPic, iRcLeft, iRcTop, iRcWidth, iRcHeight);
        else if (aPic.m_eDataType == extypeUInt32)
            dSum = measuredata_sum_rect_T<quint32, quint64>(aPic, iRcLeft, iRcTop, iRcWidth, iRcHeight);
        else if (aPic.m_eDataType == extypeFloat)
            dSum = measuredata_sum_rect_T<float, float>(aPic, iRcLeft, iRcTop, iRcWidth, iRcHeight);

        aData.m_dMeanGray = dSum / (double)(iRcWidth * iRcHeight);
    }
    return true;
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasForm::updateaTableRow(int iRow, CxMeasRecord &aData)
{
    if (iRow >= ui->tableResults->rowCount())
    {
        // add new row and create TableWidgetItems
        ui->tableResults->setRowCount(iRow+1);
        for (int iCol = 0; iCol < ui->tableResults->columnCount(); iCol++)
        {
            QTableWidgetItem *pItem = new QTableWidgetItem("");
            pItem->setTextAlignment(Qt::AlignRight);
            ui->tableResults->setItem(iRow, iCol, pItem);
        }
    }

    // RecordIdx | MeanGray | Area | Width | Height |

    ui->tableResults->item(iRow, 0)->setText(QString::number(aData.m_iRecordIdx));
    ui->tableResults->item(iRow, 1)->setText(QString::number(aData.m_dMeanGray, 'f', 2));
    ui->tableResults->item(iRow, 2)->setText(QString::number(aData.m_dArea));
    ui->tableResults->item(iRow, 3)->setText(QString::number(aData.m_dWidth));
    ui->tableResults->item(iRow, 4)->setText(QString::number(aData.m_dHeight));
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasForm::on_rectChanging(const QRectF &rc)
{
    CxVectorObject *pRect = qobject_cast<CxVectorObject*>(sender());
    if (pRect == NULL)
        return;

    // remeasure this rectangle
    IxAppDelegate *pAppDelegate = IxAppDelegate::instance();

    // find the record
    int iViewId = pRect->data(KEY_VIEWID).toInt();
    for (int iRow = 0; iRow < m_vecData.size(); iRow++)
    {
        CxMeasRecord &aData = m_vecData[iRow];
        if (aData.m_iViewId != iViewId || aData.m_idObj != pRect->id())
            continue;

        if (measureData(aData, pAppDelegate->viewCurrentImage(iViewId), rc))
            updateaTableRow(iRow, aData);
    }
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasForm::viewGotNewImage(int iViewId, const CxImageData *pImageData)
{
	if (isVisible())   // if plugin widget is open
	{
		// check the last time we measured in that view. Because if we measured on each live frame,
		// we would slow it down a lot
		if (m_mapLastMeasureTime.contains(iViewId))
		{
			if (m_mapLastMeasureTime[iViewId].elapsed() < 250)  // measure every 250 ms, not sooner
				return;
			m_mapLastMeasureTime[iViewId].restart();
		}
		else
		{
			// view not found, create new items for it
			m_mapLastMeasureTime[iViewId] = QTime();
			m_mapLastMeasureTime[iViewId].start();
		}

		// find lines with objects from this view, and re-measure all values
		IxAppDelegate *pAppDelegate = IxAppDelegate::instance();

		for (int iRow = 0; iRow < m_vecData.size(); iRow++)
		{
			CxMeasRecord &aData = m_vecData[iRow];
			if (aData.m_iViewId != iViewId)
				continue;

			CxVectorObjectRect *pRect = qobject_cast<CxVectorObjectRect*>(pAppDelegate->vectorObjectPtr(iViewId, aData.m_idObj));
			if (pRect == NULL)  // object does not exist anymore ?!
				continue;

			if (measureData(aData, pImageData, pRect->rect()))
				updateaTableRow(iRow, aData);
		}
	}
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasForm::enableTableButtons()
{
    int iSelCount = 0;
    for (int iRow = 0; iRow < ui->tableResults->rowCount(); iRow++)
    {
        if (ui->tableResults->item(iRow, 0)->isSelected())
            iSelCount++;
    }

    ui->btnRemove->setEnabled(iSelCount > 0);
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasForm::on_tableResults_itemSelectionChanged()
{
    enableTableButtons();
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasForm::on_btnClear_clicked()
{
    // delete all records, and also all rectangles from views
    IxAppDelegate *pAppDelegate = IxAppDelegate::instance();

    QVector<CxMeasRecord>::iterator it;
    for (it = m_vecData.begin(); it != m_vecData.end(); it++)
        pAppDelegate->removeVectorObject(it->m_iViewId, it->m_idObj);

    m_vecData.clear();
    m_iLastRecIdx = 0;

    ui->tableResults->clearContents();
    ui->tableResults->setRowCount(0);
}

//---------------------------------------------------------------------------
void CxMeanGrayMeasForm::on_btnRemove_clicked()
{
    // delete seleceted records
    IxAppDelegate *pAppDelegate = IxAppDelegate::instance();

    QVector<int> vecRowsToDelete;
    QVector<CxMeasRecord> vecNewData;
    for (int iRow = 0; iRow < ui->tableResults->rowCount(); iRow++)
    {
        CxMeasRecord &aData = m_vecData[iRow];
        if (ui->tableResults->item(iRow, 0)->isSelected())
        {
            pAppDelegate->removeVectorObject(aData.m_iViewId, aData.m_idObj);
            vecRowsToDelete.push_back(iRow);
        }
        else
            vecNewData.push_back(aData);
    }
    m_vecData = vecNewData;

    // delete in reverse order
    for (int i = vecRowsToDelete.size()-1; i >= 0; i--)
        ui->tableResults->removeRow(vecRowsToDelete[i]);

    ui->tableResults->clearSelection();
}
