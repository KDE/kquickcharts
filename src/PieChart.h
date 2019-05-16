#ifndef PIECHART_H
#define PIECHART_H

#include <memory>
#include <QQuickItem>

class QAbstractItemModel;
class DataSource;

/**
 * @todo write docs
 */
class PieChart : public QQuickItem
{
    Q_OBJECT

    /**
     * The width of the PieChart in border rendering mode. If this is -1, the PieChart will use a completely filled circle.
     */
    Q_PROPERTY(qreal borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)
    /**
     * The source of the PieChart's values.
     */
    Q_PROPERTY(DataSource* valueSource READ valueSource WRITE setValueSource NOTIFY valueSourceChanged)
    /**
     * The source of the PieChart's colors.
     */
    Q_PROPERTY(DataSource* colorSource READ colorSource WRITE setColorSource NOTIFY colorSourceChanged)

public:
    /**
     * Default constructor
     */
    PieChart(QQuickItem* parent = nullptr);

    /**
     * Destructor
     */
    ~PieChart();

    qreal borderWidth() const;
    DataSource* valueSource() const;
    DataSource* colorSource() const;

public Q_SLOTS:
    void setBorderWidth(qreal width);
    void setValueSource(DataSource* value);
    void setColorSource(DataSource* color);

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
