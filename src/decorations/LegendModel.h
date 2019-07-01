#ifndef LEGENDMODEL_H
#define LEGENDMODEL_H

#include <vector>

#include <QAbstractListModel>
#include <QColor>

class QQuickItem;

class ChartDataSource;

/**
 * @todo write docs
 */
class LegendModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem* chart READ chart WRITE setChart NOTIFY chartChanged)

public:
    enum Roles {
        NameRole = Qt::UserRole,
        ColorRole
    };

    LegendModel(QObject* parent = nullptr);
    ~LegendModel();

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    QQuickItem * chart() const;
    Q_SLOT void setChart(QQuickItem * newChart);
    Q_SIGNAL void chartChanged();

private:
    void update();

    QQuickItem *m_chart = nullptr;
    std::vector<QMetaObject::Connection> m_connections;
    std::vector<std::pair<QString, QColor>> m_items;
};

#endif // LEGENDMODEL_H
