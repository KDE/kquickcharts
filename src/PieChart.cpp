#include "PieChart.h"

#include <QAbstractItemModel>
#include <QDebug>

#include "scenegraph/PieChartNode.h"
#include "datasource/ChartDataSource.h"
#include "RangeGroup.h"

// class PieChart::Private
// {
// public:
//     RangeGroup *range = nullptr;
//     qreal borderWidth = -1.0;
//
//     ChartDataSource *valueSource = nullptr;
//     ChartDataSource *colorSource = nullptr;
//     ChartDataSource *nameSource = nullptr;
//
//     QVector<qreal> sections;
//     QVector<QColor> colors;
//
//     QColor backgroundColor = Qt::transparent;
// };

PieChart::PieChart(QQuickItem *parent)
    : Chart(parent)
{
    m_range = std::make_unique<RangeGroup>();
    connect(m_range.get(), &RangeGroup::rangeChanged, this, &PieChart::onDataChanged);
}

PieChart::~PieChart()
{
}

RangeGroup *PieChart::range() const
{
    return m_range.get();
}

bool PieChart::filled() const
{
    return m_filled;
}

void PieChart::setFilled(bool newFilled)
{
    if (newFilled == m_filled) {
        return;
    }

    m_filled = newFilled;
    update();
    Q_EMIT filledChanged();
}

qreal PieChart::thickness() const
{
    return m_thickness;
}

void PieChart::setThickness(qreal newThickness)
{
    if (newThickness == m_thickness) {
        return;
    }

    m_thickness = newThickness;
    update();
    Q_EMIT thicknessChanged();
}

qreal PieChart::spacing() const
{
    return m_spacing;
}

void PieChart::setSpacing(qreal newSpacing)
{
    if (newSpacing == m_spacing) {
        return;
    }

    m_spacing = newSpacing;
    update();
    Q_EMIT spacingChanged();
}


QColor PieChart::backgroundColor() const
{
    return m_backgroundColor;
}

void PieChart::setBackgroundColor(const QColor &color)
{
    if (color == m_backgroundColor) {
        return;
    }
    m_backgroundColor = color;
    update();
    Q_EMIT backgroundColorChanged();
}

bool PieChart::continueColors() const
{
    return m_continueColors;
}

void PieChart::setContinueColors(bool newContinueColors)
{
    if (newContinueColors == m_continueColors) {
        return;
    }

    m_continueColors = newContinueColors;
    onDataChanged();
    Q_EMIT continueColorsChanged();
}


QSGNode *PieChart::updatePaintNode(QSGNode *node, UpdatePaintNodeData *data)
{
    Q_UNUSED(data);
    if (!node) {
        node = new QSGNode{};
    }

    auto minDimension = std::min(width(), height());

    float innerRadius = m_filled ? 0.0 : minDimension - (m_sections.size() * m_thickness + (m_sections.size() - 1) * m_spacing);
    float outerRadius = minDimension - (m_sections.size() - 1) * (m_thickness + m_spacing);
    for (int i = 0; i < m_sections.size(); ++i) {
        if (node->childCount() <= i)
            node->appendChildNode(new PieChartNode{});

        auto pieNode = static_cast<PieChartNode*>(node->childAtIndex(i));
        pieNode->setRect(boundingRect());
        pieNode->setInnerRadius(innerRadius);
        pieNode->setOuterRadius(outerRadius);
        pieNode->setSections(m_sections.at(i));
        pieNode->setBackgroundColor(m_backgroundColor);
        pieNode->setColors(m_colors.at(i));

        innerRadius = outerRadius + m_spacing;
        outerRadius = innerRadius + m_thickness;
    }

    while (node->childCount() > m_sections.size()) {
        node->removeChildNode(node->childAtIndex(node->childCount() - 1));
    }

    return node;
}

void PieChart::onDataChanged()
{
    m_sections.clear();
    m_colors.clear();

    const auto sources = valueSources();

    if (!colorSource() || sources.isEmpty() || !m_range->isValid())
        return;

    const auto colors = colorSource();
    auto colorIndex = 0;

    for(auto source : sources) {
        qreal threshold = !m_range->automatic() ? m_range->from() : 0.0;
        qreal total = 0.0;

        QVector<qreal> sections;
        QVector<QColor> sectionColors;

        for (int i = 0; i < source->itemCount(); ++i) {
            auto value = source->item(i).toReal();
            auto limited = value - threshold;
            if (limited > 0.0) {
                sections << limited;
                total += limited;

                auto color = colors->item(colorIndex).value<QColor>();
                sectionColors << color;
            }
            threshold = qMax(0.0, threshold - value);
            colorIndex++;
        }

        if (qFuzzyCompare(total, 0.0))
            continue;

        qreal max = total;

        if (!m_range->automatic() && m_range->distance() >= total) {
            max = m_range->distance();
        }

        for (auto& value : sections) {
            value = value / max;
        }

        m_sections << sections;
        m_colors << sectionColors;
        colorIndex = m_continueColors ? colorIndex : 0;
    }

    update();
}
