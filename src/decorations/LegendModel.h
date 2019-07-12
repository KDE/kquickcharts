#ifndef LEGENDMODEL_H
#define LEGENDMODEL_H

#include <vector>

#include <QAbstractListModel>
#include <QColor>

class Chart;
class ChartDataSource;

/**
 * @todo write docs
 */
class LegendModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Chart *chart READ chart WRITE setChart NOTIFY chartChanged)
    Q_PROPERTY(int sourceIndex READ sourceIndex WRITE setSourceIndex NOTIFY sourceIndexChanged)

public:
    enum Roles {
        NameRole = Qt::UserRole,
        ColorRole,
        ValueRole
    };

    enum SourceIndex {
        UseSourceCount = -2
    };
    Q_ENUM(SourceIndex)

    LegendModel(QObject* parent = nullptr);
    ~LegendModel();

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    Chart *chart() const;
    Q_SLOT void setChart(Chart *newChart);
    Q_SIGNAL void chartChanged();

    int sourceIndex() const;
    Q_SLOT void setSourceIndex(int index);
    Q_SIGNAL void sourceIndexChanged();

private:
    void queueUpdate();
    void update();
    void updateData();

    struct LegendItem;

    Chart *m_chart = nullptr;
    int m_sourceIndex = UseSourceCount;
    bool m_updateQueued = false;
    std::vector<QMetaObject::Connection> m_connections;
    std::vector<LegendItem> m_items;
};

#endif // LEGENDMODEL_H
