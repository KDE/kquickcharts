#include "LegendModel.h"

#include "Chart.h"
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
        QMetaObject::invokeMethod(this, &LegendModel::update);
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

    m_connections.push_back(connect(m_chart, &PieChart::colorSourceChanged, this, &LegendModel::queueUpdate, Qt::UniqueConnection));
    m_connections.push_back(connect(m_chart, &PieChart::nameSourceChanged, this, &LegendModel::queueUpdate, Qt::UniqueConnection));

    int itemCount = m_chart->valueSources().count();
    if (m_sourceIndex > 0) {
        auto sources = m_chart->valueSources();
        if (m_sourceIndex < sources.size()) {
            itemCount = sources.at(m_sourceIndex)->itemCount();
            m_connections.push_back(connect(sources.at(m_sourceIndex), &ChartDataSource::dataChanged, this, &LegendModel::queueUpdate, Qt::UniqueConnection));
        }
    }
    m_connections.push_back(connect(m_chart, &Chart::valueSourcesChanged, this, &LegendModel::queueUpdate, Qt::UniqueConnection));

    if ((!colorSource && !nameSource) || itemCount <= 0) {
        endResetModel();
        return;
    }

    if (colorSource)
        m_connections.push_back(connect(colorSource, &ChartDataSource::dataChanged, this, &LegendModel::queueUpdate, Qt::UniqueConnection));

    if (nameSource)
        m_connections.push_back(connect(nameSource, &ChartDataSource::dataChanged, this, &LegendModel::queueUpdate, Qt::UniqueConnection));

    for (int i = 0; i < itemCount; ++i) {
        auto name = nameSource ? nameSource->item(i).toString() : QString();
        auto color = colorSource ? colorSource->item(i).value<QColor>() : QColor();
        m_items.push_back(std::make_pair(name, color));
    }
}

