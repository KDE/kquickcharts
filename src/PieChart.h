#ifndef PIECHART_H
#define PIECHART_H

#include <memory>
#include <QQuickItem>

class QAbstractItemModel;

/**
 * @todo write docs
 */
class PieChart : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QString dataRole READ dataRole WRITE setDataRole NOTIFY dataRoleChanged)
    Q_PROPERTY(QString colorRole READ colorRole WRITE setColorRole NOTIFY colorRoleChanged)
    Q_PROPERTY(qreal innerDiameter READ innerDiameter WRITE setInnerDiameter NOTIFY innerDiameterChanged)

public:
    /**
     * Default constructor
     */
    PieChart(QQuickItem* parent = nullptr);

    /**
     * Destructor
     */
    ~PieChart();

    QAbstractItemModel* model() const;
    QString dataRole() const;
    QString colorRole() const;
    qreal innerDiameter() const;

public Q_SLOTS:
    void setModel(QAbstractItemModel* model);
    void setDataRole(const QString& dataRole);
    void setColorRole(const QString& colorRole);
    void setInnerDiameter(qreal diameter);

Q_SIGNALS:
    void modelChanged();
    void dataRoleChanged();
    void colorRoleChanged();
    void innerDiameterChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data) override;

private:
    void updateData();

    class Private;
    const std::unique_ptr<Private> d;
};

#endif // PIECHART_H
