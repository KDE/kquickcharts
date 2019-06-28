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

    Q_PROPERTY(QQmlListProperty<ChartDataSource> valueSources READ valueSources NOTIFY valueSourcesChanged)
    Q_PROPERTY(ChartDataSource* lineColorSource READ lineColorSource WRITE setLineColorSource NOTIFY lineColorSourceChanged)
    Q_PROPERTY(ChartDataSource* lineNameSource READ lineNameSource WRITE setLineNameSource NOTIFY lineNameSourceChanged)
    Q_PROPERTY(bool stacked READ stacked WRITE setStacked NOTIFY stackedChanged)
    Q_PROPERTY(bool smooth READ smooth WRITE setSmooth NOTIFY smoothChanged)
    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    Q_PROPERTY(qreal fillOpacity READ fillOpacity WRITE setFillOpacity NOTIFY fillOpacityChanged)

public:
    using DataSourcesProperty = QQmlListProperty<ChartDataSource>;


    /**
     * Default constructor
     */
    LineChart(QQuickItem *parent = nullptr);

    /**
     * Destructor
     */
    ~LineChart();

    DataSourcesProperty valueSources();
    ChartDataSource *lineColorSource() const;
    ChartDataSource *lineNameSource() const;
    bool stacked() const;
    bool smooth() const;
    qreal lineWidth() const;
    qreal fillOpacity() const;

    int valueSourceCount() const;

public Q_SLOTS:
    void setLineColorSource(ChartDataSource *source);
    void setLineNameSource(ChartDataSource *source);
    void setStacked(bool stacked);
    void setSmooth(bool smooth);
    void setLineWidth(qreal width);
    void setFillOpacity(qreal opacity);
    void insertValueSource(int position, ChartDataSource *source);
    void removeValueSource(ChartDataSource *source);

Q_SIGNALS:
    void valueSourcesChanged();
    void lineColorSourceChanged();
    void lineNameSourceChanged();
    void stackedChanged();
    void smoothChanged();
    void lineWidthChanged();
    void fillOpacityChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data) override;
    void updateAutomaticXRange(ComputedRange &range) override;
    void updateAutomaticYRange(ComputedRange &range) override;

private:
    void onSourceDataChanged();

    class Private;
    const std::unique_ptr<Private> d;
};

#endif // LINECHART_H
