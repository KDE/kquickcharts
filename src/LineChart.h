#ifndef LINECHART_H
#define LINECHART_H

#include <memory>

#include <QQuickItem>

class Axis;
class ChartDataSource;

/**
 * @todo write docs
 */
class LineChart : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(Axis* xAxis READ xAxis WRITE setXAxis NOTIFY xAxisChanged)
    Q_PROPERTY(Axis* yAxis READ yAxis WRITE setYAxis NOTIFY yAxisChanged)
    Q_PROPERTY(QQmlListProperty<ChartDataSource> valueSources READ valueSources)
    Q_PROPERTY(ChartDataSource* lineColorSource READ lineColorSource WRITE setLineColorSource NOTIFY lineColorSourceChanged)
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

    Axis *xAxis() const;
    Axis *yAxis() const;
    DataSourcesProperty valueSources();
    ChartDataSource *lineColorSource() const;
    bool stacked() const;
    bool smooth() const;
    qreal lineWidth() const;
    qreal fillOpacity() const;

public Q_SLOTS:
    void setXAxis(Axis *axis);
    void setYAxis(Axis *axis);
    void setLineColorSource(ChartDataSource *source);
    void setStacked(bool stacked);
    void setSmooth(bool smooth);
    void setLineWidth(qreal width);
    void setFillOpacity(qreal opacity);

Q_SIGNALS:
    void xAxisChanged();
    void yAxisChanged();
    void lineColorSourceChanged();
    void stackedChanged();
    void smoothChanged();
    void lineWidthChanged();
    void fillOpacityChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data) override;

private:
    class Private;
    const std::unique_ptr<Private> d;
};

#endif // LINECHART_H
