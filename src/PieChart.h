#ifndef PIECHART_H
#define PIECHART_H

#include <QQuickItem>
#include <memory>

class QAbstractItemModel;
class ChartDataSource;
class RangeGroup;

/**
 * @todo write docs
 */
class PieChart : public QQuickItem
{
    Q_OBJECT

    /**
     * The range of values to display in this PieChart. When set to "automatic", the values will be divide across the
     * entire chart. .
     */
    Q_PROPERTY(RangeGroup *range READ range CONSTANT)
    /**
     * The width of the PieChart in border rendering mode. If this is -1, the PieChart will use a completely filled circle.
     */
    Q_PROPERTY(qreal borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)
    /**
     * The source of the PieChart's values.
     */
    Q_PROPERTY(ChartDataSource *valueSource READ valueSource WRITE setValueSource NOTIFY valueSourceChanged)
    /**
     * The source of the PieChart's colors.
     */
    Q_PROPERTY(ChartDataSource *colorSource READ colorSource WRITE setColorSource NOTIFY colorSourceChanged)

public:
    /**
     * Default constructor
     */
    PieChart(QQuickItem *parent = nullptr);

    /**
     * Destructor
     */
    ~PieChart();

    RangeGroup *range() const;
    qreal borderWidth() const;
    ChartDataSource *valueSource() const;
    ChartDataSource *colorSource() const;

public Q_SLOTS:
    void setBorderWidth(qreal width);
    void setValueSource(ChartDataSource *value);
    void setColorSource(ChartDataSource *color);

Q_SIGNALS:
    void borderWidthChanged();
    void valueSourceChanged();
    void colorSourceChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data) override;

private:
    void updateData();

    class Private;
    const std::unique_ptr<Private> d;
};

#endif // PIECHART_H
