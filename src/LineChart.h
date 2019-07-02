#ifndef LINECHART_H
#define LINECHART_H

#include <memory>

#include "XYChart.h"

class RangeGroup;
class ChartDataSource;

/**
 * @todo write docs
 */
class LineChart : public XYChart
{
    Q_OBJECT

    Q_PROPERTY(bool stacked READ stacked WRITE setStacked NOTIFY stackedChanged)
    Q_PROPERTY(bool smooth READ smooth WRITE setSmooth NOTIFY smoothChanged)
    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    Q_PROPERTY(qreal fillOpacity READ fillOpacity WRITE setFillOpacity NOTIFY fillOpacityChanged)

public:



    /**
     * Default constructor
     */
    LineChart(QQuickItem *parent = nullptr);

    /**
     * Destructor
     */
    ~LineChart();

    bool stacked() const;
    bool smooth() const;
    qreal lineWidth() const;
    qreal fillOpacity() const;


public Q_SLOTS:
    void setStacked(bool stacked);
    void setSmooth(bool smooth);
    void setLineWidth(qreal width);
    void setFillOpacity(qreal opacity);

Q_SIGNALS:
    void stackedChanged();
    void smoothChanged();
    void lineWidthChanged();
    void fillOpacityChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data) override;
    void onDataChanged() override;
    void updateAutomaticXRange(ComputedRange &range) override;
    void updateAutomaticYRange(ComputedRange &range) override;

private:

    class Private;
    const std::unique_ptr<Private> d;
};

#endif // LINECHART_H
