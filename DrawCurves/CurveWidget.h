#ifndef CURVEWIDGET_H
#define CURVEWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPointF>
#include <QRectF>
#include <QList>
#include <vector>
class CurveWidget : public QWidget
{
    Q_OBJECT
public:
    CurveWidget(QWidget *parent = nullptr);
    bool GetAreaRoundCurve(const std::vector<QPointF> &pts, std::vector<QPointF> &curves);
    QPointF GetCenter(const std::vector<QPointF> &pts);
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<QPointF> m_listPoint;
};

#endif // CURVEWIDGET_H
