#include "LegendModel.h"

#include "Chart.h"
#include "datasource/ChartDataSource.h"

struct LegendModel::LegendItem
{
    QString name;
    QColor color;
    QVariant value;
};

LegendModel::LegendModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

LegendModel::~LegendModel()
{
}

QHash<int, QByteArray> LegendModel::roleNames() const
{
    static QHash<int, QByteArray> names = {
        { NameRole, "name" },
        { ColorRole, "color" },
        { ValueRole, "value" },
    };

    return names;
}

int LegendModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_items.size();
}

QVariant LegendModel::data(const QModelIndex& index, int role) const
{
    if (!checkIndex(index, CheckIndexOption::ParentIsInvalid | CheckIndexOption::IndexIsValid))
        return QVariant{};

    switch (role) {
        case NameRole:
            return m_items.at(index.row()).name;
        case ColorRole:
            return m_items.at(index.row()).color;
        case ValueRole:
            return m_items.at(index.row()).value;
    }

    return QVariant{};
}

Chart *LegendModel::chart() const
{
    return m_chart;
}

void LegendModel::setChart(Chart *newChart)
{
    if (newChart == m_chart) {
        return;
    }

    if (m_chart) {
        for (auto connection : m_connections) {
            disconnect(connection);
        }
        m_connections.clear();
    }

    m_chart = newChart;
    queueUpdate();
    Q_EMIT chartChanged();
}

int LegendModel::sourceIndex() const
{
    return m_sourceIndex;
}

void LegendModel::setSourceIndex(int index)
{
    if (index == m_sourceIndex) {
        return;
    }

    m_sourceIndex = index;
    queueUpdate();
    Q_EMIT sourceIndexChanged();
}

void LegendModel::queueUpdate()
{
    if (!m_updateQueued) {
        m_updateQueued = true;
        QMetaObject::invokeMethod(this, &LegendModel::update, Qt::QueuedConnection);
    }
}

void LegendModel::update()
{
    m_updateQueued = false;

    if (!m_chart)
        return;

    beginResetModel();
    m_items.clear();

    ChartDataSource *colorSource = m_chart->colorSource();
    ChartDataSource *nameSource = m_chart->nameSource();
    ChartDataSource *valueSource = nullptr;

    m_connections.push_back(connect(m_chart, &Chart::colorSourceChanged, this, &LegendModel::queueUpdate, Qt::UniqueConnection));
    m_connections.push_back(connect(m_chart, &Chart::nameSourceChanged, this, &LegendModel::queueUpdate, Qt::UniqueConnection));

    auto sources = m_chart->valueSources();
    int itemCount = sources.count();
    if (m_sourceIndex >= 0 && m_sourceIndex < sources.size()) {
        valueSource = sources.at(m_sourceIndex);
        itemCount = valueSource->itemCount();
        m_connections.push_back(connect(valueSource, &ChartDataSource::dataChanged, this, &LegendModel::updateData, Qt::UniqueConnection));
    } else {
        std::transform(sources.cbegin(), sources.cend(), std::back_inserter(m_connections), [this](ChartDataSource *source) {
            return connect(source, &ChartDataSource::dataChanged, this, &LegendModel::updateData, Qt::UniqueConnection);
        });
    }
    m_connections.push_back(connect(m_chart, &Chart::valueSourcesChanged, this, &LegendModel::queueUpdate, Qt::UniqueConnection));

    if ((!colorSource && !nameSource) || itemCount <= 0) {
        endResetModel();
        return;
    }

    if (colorSource)
        m_connections.push_back(connect(colorSource, &ChartDataSource::dataChanged, this, &LegendModel::updateData, Qt::UniqueConnection));

    if (nameSource)
        m_connections.push_back(connect(nameSource, &ChartDataSource::dataChanged, this, &LegendModel::updateData, Qt::UniqueConnection));

    for (int i = 0; i < itemCount; ++i) {
        LegendItem item;
        item.name = nameSource ? nameSource->item(i).toString() : QString();
        item.color = colorSource ? colorSource->item(i).value<QColor>() : QColor();

        if (m_sourceIndex < 0) {
            item.value = m_chart->valueSources().at(i)->item(0);
        } else {
            item.value = valueSource ? valueSource->item(i) : QVariant{};
        }

        m_items.push_back(item);
    }

    endResetModel();
}

void LegendModel::updateData()
{
    ChartDataSource *colorSource = m_chart->colorSource();
    ChartDataSource *nameSource = m_chart->nameSource();
    ChartDataSource *valueSource = nullptr;

    auto itemCount = m_chart->valueSources().count();
    if (m_sourceIndex >= 0) {
        auto sources = m_chart->valueSources();
        if (m_sourceIndex < sources.size()) {
            valueSource = sources.at(m_sourceIndex);
            itemCount = valueSource->itemCount();
        }
    }

    if (itemCount != int(m_items.size())) {
        // Number of items changed, so trigger a full update
        queueUpdate();
        return;
    }

    std::for_each(m_items.begin(), m_items.end(), [&, i = 0](LegendItem &item) mutable {
        item.name = nameSource ? nameSource->item(i).toString() : QString();
        item.color = colorSource ? colorSource->item(i).value<QColor>() : QColor();

        if (m_sourceIndex < 0) {
            item.value = m_chart->valueSources().at(i)->item(0);
        } else {
            item.value = valueSource ? valueSource->item(i) : QVariant{};
        }

        i++;
    });

    Q_EMIT dataChanged(index(0, 0), index(itemCount - 1, 0), { NameRole, ColorRole, ValueRole });
}
