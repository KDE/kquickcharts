#include "LegendModel.h"

#include "PieChart.h"
#include "LineChart.h"
#include "datasource/ChartDataSource.h"

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
        { ColorRole, "color" }
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
            return m_items.at(index.row()).first;
        case ColorRole:
            return m_items.at(index.row()).second;
    }

    return QVariant{};
}

QQuickItem * LegendModel::chart() const
{
    return m_chart;
}

void LegendModel::setChart(QQuickItem * newChart)
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
    update();
    Q_EMIT chartChanged();
}

void LegendModel::update()
{
    if (!m_chart)
        return;

    beginResetModel();
    m_items.clear();

    ChartDataSource *colorSource = nullptr;
    ChartDataSource *nameSource = nullptr;
    int itemCount = 0;

    auto pie = qobject_cast<PieChart*>(m_chart);
    if (pie) {
        if (!pie->valueSource()) {
            endResetModel();
            return;
        }

        m_connections.push_back(connect(pie, &PieChart::colorSourceChanged, this, &LegendModel::update));
        m_connections.push_back(connect(pie, &PieChart::nameSourceChanged, this, &LegendModel::update));
        m_connections.push_back(connect(pie->valueSource(), &ChartDataSource::dataChanged, this, &LegendModel::update));

        colorSource = pie->colorSource();
        nameSource = pie->nameSource();
        itemCount = pie->valueSource()->itemCount();
    }

    auto line = qobject_cast<LineChart*>(m_chart);
    if (line) {
        m_connections.push_back(connect(line, &LineChart::lineColorSourceChanged, this, &LegendModel::update));
        m_connections.push_back(connect(line, &LineChart::lineNameSourceChanged, this, &LegendModel::update));
        m_connections.push_back(connect(line, &LineChart::valueSourcesChanged, this, &LegendModel::update));

        colorSource = line->lineColorSource();
        nameSource = line->lineNameSource();
        itemCount = line->valueSourceCount();
    }

    if ((!colorSource && !nameSource) || itemCount <= 0) {
        endResetModel();
        return;
    }

    if (colorSource)
        m_connections.push_back(connect(colorSource, &ChartDataSource::dataChanged, this, &LegendModel::update));

    if (nameSource)
        m_connections.push_back(connect(nameSource, &ChartDataSource::dataChanged, this, &LegendModel::update));

    for (int i = 0; i < itemCount; ++i) {
        auto name = nameSource ? nameSource->item(i).toString() : QString();
        auto color = colorSource ? colorSource->item(i).value<QColor>() : QColor();
        m_items.push_back(std::make_pair(name, color));
    }
}

