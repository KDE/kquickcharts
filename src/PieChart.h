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
    Q_PROPERTY(qreal innerDiameter READ innerDiameter WRITE setInnerDiameter NOTIFY innerDiameterChanged)
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

    qreal innerDiameter() const;
    DataSource* valueSource() const;
    DataSource* colorSource() const;

public Q_SLOTS:
    void setInnerDiameter(qreal diameter);
    void setValueSource(DataSource* value);
    void setColorSource(DataSource* color);

Q_SIGNALS:
    void innerDiameterChanged();
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
