/*
 * SPDX-FileCopyrightText: 2020 Arjen Hiemstra <ahiemstra@heimr.nl>
 * 
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "Legend.h"

#include <cmath>

#include "Chart.h"
#include "ItemBuilder.h"
#include "datasource/ChartDataSource.h"

qreal sizeWithSpacing(int count, qreal size, qreal spacing)
{
    return size * count + spacing * (count - 1);
}

LegendAttached::LegendAttached(QObject* parent)
    : QObject(parent)
{
}

int LegendAttached::index() const
{
    return m_index;
}

void LegendAttached::setIndex(int newIndex)
{
    if (newIndex == m_index) {
        return;
    }

    m_index = newIndex;
}

QString LegendAttached::name() const
{
    return m_name;
}

void LegendAttached::setName(const QString &newName)
{
    if (newName == m_name) {
        return;
    }

    m_name = newName;
    Q_EMIT nameChanged();
}

QString LegendAttached::shortName() const
{
    if (m_shortName.isEmpty()) {
        return m_name;
    } else {
        return m_shortName;
    }
}

void LegendAttached::setShortName(const QString &newShortName)
{
    if (newShortName == m_shortName) {
        return;
    }

    m_shortName = newShortName;
    Q_EMIT shortNameChanged();
}

QVariant LegendAttached::value() const
{
    return m_value;
}

void LegendAttached::setValue(const QVariant &newValue)
{
    if (newValue == m_value) {
        return;
    }

    m_value = newValue;
    Q_EMIT valueChanged();
}


QColor LegendAttached::color() const
{
    return m_color;
}

void LegendAttached::setColor(const QColor &newColor)
{
    if (newColor == m_color) {
        return;
    }

    m_color = newColor;
    Q_EMIT colorChanged();
}

qreal LegendAttached::minimumWidth() const
{
    return m_minimumWidth;
}

void LegendAttached::setMinimumWidth(qreal newMinimumWidth)
{
    if (newMinimumWidth == m_minimumWidth) {
        return;
    }

    m_minimumWidth = newMinimumWidth;
    Q_EMIT minimumWidthChanged();
}

qreal LegendAttached::maximumWidth() const
{
    return m_maximumWidth;
}

void LegendAttached::setMaximumWidth(qreal newMaximumWidth)
{
    if (newMaximumWidth == m_maximumWidth) {
        return;
    }

    m_maximumWidth = newMaximumWidth;
    Q_EMIT maximumWidthChanged();
}

Legend::Legend(QQuickItem *parent)
    : QQuickItem(parent)
{
    m_itemBuilder = std::make_unique<ItemBuilder>();
    connect(m_itemBuilder.get(), &ItemBuilder::finished, this, &Legend::polish);
    connect(m_itemBuilder.get(), &ItemBuilder::beginCreate, this, &Legend::updateItem);
}

Legend::~Legend() = default;

Chart * Legend::chart() const
{
    return m_chart;
}

void Legend::setChart(Chart * newChart)
{
    if (newChart == m_chart) {
        return;
    }

    if (m_chart) {
        disconnect(m_chart, &Chart::dataChanged, this, &Legend::updateItems);
    }

    m_chart = newChart;

    if (m_chart) {
        connect(m_chart, &Chart::dataChanged, this, &Legend::updateItems);
        updateItems();
    }

    Q_EMIT chartChanged();
}

QQmlComponent *Legend::delegate() const
{
    return m_itemBuilder->component();
}

void Legend::setDelegate(QQmlComponent *newDelegate)
{
    if (newDelegate == m_itemBuilder->component()) {
        return;
    }

    m_itemBuilder->setComponent(newDelegate);
    updateItems();
    Q_EMIT delegateChanged();
}

qreal Legend::horizontalSpacing() const
{
    return m_horizontalSpacing;
}

void Legend::setHorizontalSpacing(qreal newHorizontalSpacing)
{
    if (newHorizontalSpacing == m_horizontalSpacing) {
        return;
    }

    m_horizontalSpacing = newHorizontalSpacing;
    polish();
    Q_EMIT horizontalSpacingChanged();
}

qreal Legend::verticalSpacing() const
{
    return m_verticalSpacing;
}

void Legend::setVerticalSpacing(qreal newVerticalSpacing)
{
    if (newVerticalSpacing == m_verticalSpacing) {
        return;
    }

    m_verticalSpacing = newVerticalSpacing;
    polish();
    Q_EMIT verticalSpacingChanged();
}

void Legend::componentComplete()
{
    QQuickItem::componentComplete();

    m_completed = true;
    updateItems();
}

void Legend::updatePolish()
{
    if (!m_completed || !m_itemBuilder->isFinished()) {
        return;
    }

    int columns = 0;
    int rows = 0;
    qreal itemWidth = 0.0;
    qreal itemHeight = 0.0;

    std::tie(columns, rows, itemWidth, itemHeight) = determineColumns();

    auto items = m_itemBuilder->items();

    auto column = 0;
    auto row = 0;

    for (auto item : items) {
        auto attached = static_cast<LegendAttached*>(qmlAttachedPropertiesObject<Legend>(item.get(), true));

        auto x = (itemWidth + m_horizontalSpacing) * column;
        auto y = (itemHeight + m_verticalSpacing) * row;

        item->setPosition(QPointF{x, y});
        item->setWidth(std::max(attached->minimumWidth(), std::min(itemWidth, attached->maximumWidth())));

        column++;
        if (column >= columns) {
            row++;
            column = 0;
        }
    }

    setImplicitSize(sizeWithSpacing(columns, itemWidth, m_horizontalSpacing),
                    sizeWithSpacing(rows, itemHeight, m_verticalSpacing));
}

void Legend::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    if (newGeometry != oldGeometry) {
        polish();
    }
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
}

void Legend::itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData& data)
{
    if (change == QQuickItem::ItemVisibleHasChanged || change == QQuickItem::ItemSceneChange) {
        polish();
    }

    QQuickItem::itemChange(change, data);
}

void Legend::updateItems()
{
    if (!m_completed) {
        return;
    }

    auto count = itemCount();

    if (count != m_itemBuilder->count()) {
        m_itemBuilder->clear();
        m_itemBuilder->setCount(count);
        m_itemBuilder->build(this);
        return;
    }

    const auto items = m_itemBuilder->items();
    for (std::size_t i = 0; i < items.size(); ++i) {
        updateItem(i, items.at(i).get());
    }

    polish();
}

void Legend::updateItem(int index, QQuickItem* item)
{
    if (!item || !m_completed) {
        return;
    }

    auto attached = static_cast<LegendAttached *>(qmlAttachedPropertiesObject<Legend>(item, true));

    if (attached->index() < 0) {
        attached->setIndex(index);
    }

    connect(attached, &LegendAttached::minimumWidthChanged, this, &Legend::polish, Qt::UniqueConnection);
    connect(attached, &LegendAttached::maximumWidthChanged, this, &Legend::polish, Qt::UniqueConnection);

    attached->setName(m_chart->nameSource() ? m_chart->nameSource()->item(index).toString() : QString{});
    attached->setShortName(m_chart->shortNameSource() ? m_chart->shortNameSource()->item(index).toString() : QString{});
    attached->setColor(m_chart->colorSource() ? m_chart->colorSource()->item(index).value<QColor>() : QColor{});

    if (m_chart->indexingMode() == Chart::IndexEachSource) {
        attached->setValue(m_chart->valueSources().at(index)->item(0));
    } else {
        const auto sources = m_chart->valueSources();
        for (auto source : sources) {
            if (index >= source->itemCount()) {
                index -= source->itemCount();
                continue;
            } else {
                attached->setValue(source->item(index));
                break;
            }
        }
    }
}

int Legend::itemCount()
{
    if (!m_chart) {
        return 0;
    }

    if (m_chart->indexingMode() == Chart::IndexEachSource) {
        return m_chart->valueSources().count();
    } else {
        const auto sources = m_chart->valueSources();
        return std::accumulate(sources.begin(), sources.end(), 0, [](int value, ChartDataSource *source) {
            return value + source->itemCount();
        });
    }
}

std::tuple<int, int, qreal, qreal> Legend::determineColumns()
{
    // Determine how many columns should be used for layouting the items.

    auto minWidth = std::numeric_limits<qreal>::min();
    auto maxWidth = std::numeric_limits<qreal>::max();
    auto maxHeight = std::numeric_limits<qreal>::min();

    const auto items = m_itemBuilder->items();

    // First, we determine the minimum and maximum width of all items. These are
    // determined from the attached object, or implicitWidth for minimum size if
    // minimumWidth has not been set.
    //
    // We also determine the maximum height of items so we do not need to do
    // that later.
    for (auto item : items) {
        auto attached = static_cast<LegendAttached*>(qmlAttachedPropertiesObject<Legend>(item.get(), true));

        if (attached->minimumWidth() > 0.0) {
            minWidth = std::max(minWidth, attached->minimumWidth());
        } else {
            minWidth = std::max(minWidth, item->implicitWidth());
        }

        if (attached->maximumWidth() > 0.0) {
            maxWidth = std::min(maxWidth, attached->maximumWidth());
        }

        maxHeight = std::max(maxHeight, item->height());
    }

    // If none of the items have a maximum width set, default to fillings all
    // available space.
    if (maxWidth <= 0.0) {
        maxWidth = width();
    }

    // Ensure we don't try to size things below their minimum size.
    if (maxWidth < minWidth) {
        maxWidth = minWidth;
    }

    auto availableWidth = width();
    auto columns = 1;
    auto rows = items.size();
    auto tries = items.size();

    // Calculate the actual number of rows and columns by trying to fit items
    // until we find the right number. In certain cases this may end up not
    // finding a correct solution, so we limit the amount of attempts we make to
    // the number of items.
    while (tries > 0) {
        auto minTotalWidth = sizeWithSpacing(columns, minWidth, m_horizontalSpacing);
        auto maxTotalWidth = sizeWithSpacing(columns, maxWidth, m_horizontalSpacing);

        if (minTotalWidth < availableWidth && maxTotalWidth > availableWidth) {
            // We found a solution that lies within bounds, so abort the loop.
            break;
        } else if (maxTotalWidth < availableWidth) {
            rows--;
            if (rows >= 1) {
                columns = std::ceil(items.size() / float(rows));
            } else {
                break;
            }
        } else if (minTotalWidth > availableWidth) {
            rows += 1;
            columns = std::ceil(items.size() / float(rows));
            break;
        }

        tries--;
    }

    auto itemWidth = (width() - m_horizontalSpacing * (columns - 1)) / columns;

    rows = std::ceil(items.size() / float(columns));

    return std::make_tuple(columns, rows, itemWidth, maxHeight);
}
