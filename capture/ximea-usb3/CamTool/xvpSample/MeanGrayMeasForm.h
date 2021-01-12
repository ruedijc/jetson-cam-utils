#ifndef MEANGRAYMEASFORM_H
#define MEANGRAYMEASFORM_H

#include <QTime>
#include <QWidget>
#include <VectorObject.h>

class QMainWindow;
class CxImageData;

namespace Ui {
class CxMeanGrayMeasForm;
}

class CxMeanGrayMeasForm : public QWidget
{
    Q_OBJECT

public:
    explicit CxMeanGrayMeasForm(QWidget *parent = 0);
    ~CxMeanGrayMeasForm();

    // events from the application
public:
    void activeViewChanged(int iViewId);
    void viewGotNewImage(int iViewId, const CxImageData *pImageData);
    bool viewMouseBtnPressEvent(int iViewId, QGraphicsSceneMouseEvent *event);
    bool viewMouseMoveEvent(int iViewId, QGraphicsSceneMouseEvent *event);
    bool viewMouseBtnReleaseEvent(int iViewId, QGraphicsSceneMouseEvent *event);

private:
    Ui::CxMeanGrayMeasForm *ui;
    bool m_bDrawingNewRect;
    TxVecObjID m_idRectDrawing;
    QPointF m_ptStartRectDrawing;

    // measured data
private:
    class CxMeasRecord
    {
        // identification
    public:
        int m_iViewId;
        TxVecObjID m_idObj;
        int m_iRecordIdx;

        // data
    public:
        double m_dMeanGray;
        double m_dArea;
        double m_dWidth;
        double m_dHeight;
    };
    QVector<CxMeasRecord> m_vecData;  // all measured data, shown in the table
    int m_iLastRecIdx;
    QMap<int, QTime> m_mapLastMeasureTime; // last time we measured in each view (skip frames when acq. running)

    bool measureData(CxMeasRecord &aData, const CxImageData *pImageData, const QRectF &rc);
    void updateaTableRow(int iRow, CxMeasRecord &aData);
    void enableTableButtons();

private slots:
    void on_rectChanging(const QRectF &rc);
    void on_btnClear_clicked();
    void on_btnRemove_clicked();
    void on_tableResults_itemSelectionChanged();
};

#endif // MEANGRAYMEASFORM_H
