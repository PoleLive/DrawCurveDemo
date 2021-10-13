#include "CurveWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QPolygonF>

CurveWidget::CurveWidget(QWidget *parent)
{

    m_listPoint.append(QPointF(300, 180));
    m_listPoint.append(QPointF(350, 350));
    m_listPoint.append(QPointF(500, 400));
    m_listPoint.append(QPointF(450, 500));
    m_listPoint.append(QPointF(350, 550));
    m_listPoint.append(QPointF(150, 400));
    m_listPoint.append(QPointF(200, 250));
}
bool CurveWidget::GetAreaRoundCurve(const std::vector<QPointF> &pts, std::vector<QPointF> &curves)
{
    const double Pi = 3.1415926;
    int nSize = pts.size();
    std::vector<QPointF> allCurves;
    curves.clear();

    for (int i = 0; i < pts.size(); i++)
    {
        int preIndex = (i + nSize - 1) % nSize;
        int nextIndex = (i + 1) % nSize;
        double x1 = pts[preIndex].x(), x2 = pts[i].x(), x3 = pts[nextIndex].x();
        double y1 = pts[preIndex].y(), y2 = pts[i].y(), y3 = pts[nextIndex].y();

        double a = x1 - x2;
        double b = y1 - y2;
        double c = x1 - x3;
        double d = y1 - y3;
        double e = ((x1 * x1 - x2 * x2) + (y1 * y1 - y2 * y2)) / 2.0;
        double f = ((x1 * x1 - x3 * x3) + (y1 * y1 - y3 * y3)) / 2.0;
        double det = b * c - a * d;

        //三点一线
        if (fabs(det) <= 1)
        {
            return false;
        }

        //圆心坐标
        double x0 = -(d * e - b * f) / det;
        double y0 = -(a * f - c * e) / det;

        double ddd = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);

        //计算三点的对应的角度
        double angle1 = atan2((y1 - y0), (x1 - x0));
        double angle2 = atan2((y2 - y0), (x2 - x0));
        double angle3 = atan2((y3 - y0), (x3 - x0));

        //将角度限制到0-2 * Pi
        if (angle1 < 0)
        {
            angle1 += 2.0 * Pi;
        }
        if (angle2 < 0)
        {
            angle2 += 2.0 * Pi;
        }
        if (angle3 < 0)
        {
            angle3 += 2.0 * Pi;
        }

        int flag = 0;

        if (angle1 > angle2 && ddd >= 0)
        {
            angle2 += 2.0 * Pi;
            flag = 1;
        }
        else if (angle1 < angle2 && ddd < 0)
        {
            angle1 += 2.0 * Pi;
        }
        double nInsertPointNumber = 200;
        double dStep1 = (angle2 - angle1) / nInsertPointNumber;
        //if(nSize < 4 || i == 1)
        {
        for (int j = 0; j < nInsertPointNumber; j++)
        {
            double thet = dStep1 * j;
            double nX = (x1 - x0) * cos(thet) - (y1 - y0) * sin(thet) + x0;
            double nY = (x1 - x0) * sin(thet) + (y1 - y0) * cos(thet) + y0;

            if (j + i * nInsertPointNumber >= allCurves.size() /*&& nSize < 4*/)
            {
                allCurves.push_back(QPointF(nX, nY));
            }
            else
            {
                int nNextIndex = j + i * nInsertPointNumber;
                if(nSize > 4)
                {
                    //nNextIndex = j;
                }
                double dPosX = (allCurves[nNextIndex].x() * (nInsertPointNumber - j) + nX * j) / nInsertPointNumber;
                double dPosY = (allCurves[nNextIndex].y() * (nInsertPointNumber - j) + nY * j) / nInsertPointNumber;
                allCurves[nNextIndex].setX(dPosX);
                allCurves[nNextIndex].setY(dPosY);
            }
        }
        }
        if (flag)
        {
            angle2 -= 2.0 * Pi;
        }

        if (angle2 > angle3 && ddd >= 0)
        {
            angle3 += 2.0 * Pi;
        }
        else if (angle2 < angle3 && ddd < 0)
        {
            angle2 += 2.0 * Pi;
        }
        double dStep2 = (angle3 - angle2) / nInsertPointNumber;
        //if(nSize < 4 || i == 0)
        {
        for (int j = 0; j < nInsertPointNumber; j++)
        {
            double thet = dStep2 * j;
            double nX = (x2 - x0) * cos(thet) - (y2 - y0) * sin(thet) + x0;
            double nY = (x2 - x0) * sin(thet) + (y2 - y0) * cos(thet) + y0;

            int nNextIndex = j + i * nInsertPointNumber + nInsertPointNumber;
            if (nNextIndex >= allCurves.size() && i != nSize - 1)
            {
                allCurves.push_back(QPointF(nX, nY));
            }
            else
            {
                if (i == nSize - 1)
                {
                    double dPosX = (allCurves[j].x() * j + nX * (nInsertPointNumber - j)) / nInsertPointNumber;
                    double dPosY = (allCurves[j].y() * j + nY * (nInsertPointNumber - j)) / nInsertPointNumber;
                    allCurves[j].setX(dPosX);
                    allCurves[j].setY(dPosY);
                }
                else
                {
                    double dPosX = (allCurves[nNextIndex].x() * j + nX * (nInsertPointNumber - j)) / nInsertPointNumber;
                    double dPosY = (allCurves[nNextIndex].y() * j + nY * (nInsertPointNumber - j)) / nInsertPointNumber;
                    allCurves[nNextIndex].setX(dPosX);
                    allCurves[nNextIndex].setY(dPosY);
                }
            }
        }
        }
        /*
        if(nSize > 4 && i >= 1)
            break;
        */

    }

    curves.push_back(allCurves[0]);
    for (int i = 1; i < allCurves.size(); i++)
    {
        if (allCurves[i] != allCurves[i - 1])
        {
            curves.push_back(allCurves[i]);
        }
    }
    return true;
}

QPointF CurveWidget::GetCenter(const std::vector<QPointF> &pts)
{
    const double Pi = 3.1415926;
    int nSize = pts.size();
    std::vector<QPointF> allCurves;
    double x0;
    double y0;
    for (int i = 0; i < pts.size(); i++)
    {
        int preIndex = (i + nSize - 1) % nSize;
        int nextIndex = (i + 1) % nSize;
        double x1 = pts[preIndex].x(), x2 = pts[i].x(), x3 = pts[nextIndex].x();
        double y1 = pts[preIndex].y(), y2 = pts[i].y(), y3 = pts[nextIndex].y();

        double a = x1 - x2;
        double b = y1 - y2;
        double c = x1 - x3;
        double d = y1 - y3;
        double e = ((x1 * x1 - x2 * x2) + (y1 * y1 - y2 * y2)) / 2.0;
        double f = ((x1 * x1 - x3 * x3) + (y1 * y1 - y3 * y3)) / 2.0;
        double det = b * c - a * d;

        //圆心坐标
        x0 = -(d * e - b * f) / det;
        y0 = -(a * f - c * e) / det;
        break;
    }
    return QPointF(x0, y0);
}
void CurveWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QFont textFont = painter.font();
    textFont.setPointSize(12);
    painter.setFont(textFont);
    painter.setRenderHints(QPainter::Antialiasing
                           | QPainter::SmoothPixmapTransform
                           | QPainter::Qt4CompatiblePainting);
    std::vector<QPointF> pts;
    std::vector<QPointF> curves;
    for(int i = 0; i < m_listPoint.size(); i++)
    {
        pts.push_back(m_listPoint[i]);
    }

    GetAreaRoundCurve(pts, curves);
    QPolygonF polygonf;
    for(int i = 0; i < curves.size(); i++)
    {
        polygonf.append(curves[i]);
    }
    QPainterPath path;
    path.addPolygon(polygonf);
    path.closeSubpath();
    painter.setPen(Qt::red);
    //painter.drawPath(path);

    pts.clear();
    curves.clear();
    for(int i = 0; i < 3; i++)
    {
        pts.push_back(m_listPoint[i]);
    }

    GetAreaRoundCurve(pts, curves);
    polygonf.clear();
    for(int i = 0; i < curves.size(); i++)
    {
        polygonf.append(curves[i]);
    }
    QPainterPath path1;
    path1.addPolygon(polygonf);
    path1.closeSubpath();
    QPen pen1;
    pen1.setColor(Qt::blue);
    pen1.setStyle(Qt::DashLine);
    painter.setPen(pen1);
    painter.setBrush(Qt::NoBrush);
    //painter.drawPath(path1);

    painter.setBrush(Qt::blue);
    QPointF o1 = GetCenter(pts);
    //painter.drawEllipse(GetCenter(pts), 2, 2);
    //painter.drawLine(GetCenter(pts), m_listPoint[1]);
    //painter.drawLine(GetCenter(pts), m_listPoint[0]);
    o1.setX(o1.x() - 15);
    o1.setY(o1.y() + 15);
    //painter.drawText(o1, "O2");

    pts.clear();
    curves.clear();
    pts.push_back(m_listPoint[m_listPoint.size() - 1]);
    for(int i = 0; i < 2; i++)
    {
        pts.push_back(m_listPoint[i]);
    }

    GetAreaRoundCurve(pts, curves);
    polygonf.clear();
    for(int i = 0; i < curves.size(); i++)
    {
        polygonf.append(curves[i]);
    }
    QPainterPath path2;
    path2.addPolygon(polygonf);
    path2.closeSubpath();
    QPen pen2;
    pen2.setColor(Qt::black);
    pen2.setStyle(Qt::DashLine);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(pen2);
    //painter.drawPath(path2);
    painter.setBrush(Qt::black);
    QPointF o2 = GetCenter(pts);
    //painter.drawEllipse(o2 , 2, 2);
    //painter.drawLine(o2, m_listPoint[1]);
    //painter.drawLine(o2, m_listPoint[0]);
    o2.setX(o2.x() + 3);
    o2.setY(o2.y() + 13);
    //painter.drawText(o2, "O1");
    painter.setPen(Qt::red);
    painter.setBrush(Qt::red);
    for(int i = 0; i < m_listPoint.size(); i++)
    {
        painter.drawEllipse(m_listPoint[i], 2, 2);
        QPointF pos = m_listPoint[i];
        pos.setX(pos.x() + 3);
        pos.setY(pos.y() + 10);
        painter.drawText(pos, QString::number(i));
    }

}
