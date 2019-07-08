#ifndef LINECHART_H
#define LINECHART_H

#include <memory>

#include "XYChart.h"

class LineChartNode;

/**
 * @todo write docs
 */
class LineChart : public XYChart
{
    Q_OBJECT

    Q_PROPERTY(bool smooth READ smooth WRITE setSmooth NOTIFY smoothChanged)
    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    Q_PROPERTY(qreal fillOpacity READ fillOpacity WRITE setFillOpacity NOTIFY fillOpacityChanged)

public:
    LineChart(QQuickItem *parent = nullptr);
    ~LineChart();

    bool smooth() const;
    qreal lineWidth() const;
    qreal fillOpacity() const;

public Q_SLOTS:
    void setSmooth(bool smooth);
    void setLineWidth(qreal width);
    void setFillOpacity(qreal opacity);

Q_SIGNALS:
    void smoothChanged();
    void lineWidthChanged();
    void fillOpacityChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data) override;
    void onDataChanged() override;

private:
    void updateLineNode(LineChartNode *node, const QColor& lineColor, ChartDataSource *valueSource);

    bool m_smooth = false;
    qreal m_lineWidth = 1.0;
    qreal m_fillOpacity = 0.0;
    bool m_rangeInvalid = true;
    QVector<QVector2D> m_previousValues;
};

#endif // LINECHART_H
