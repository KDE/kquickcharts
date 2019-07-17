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

qreal PieChart::fromAngle() const
{
    return m_fromAngle;
}

void PieChart::setFromAngle(qreal newFromAngle)
{
    if (qFuzzyCompare(newFromAngle, m_fromAngle)) {
        return;
    }

    m_fromAngle = newFromAngle;
    update();
    Q_EMIT fromAngleChanged();
}

qreal PieChart::toAngle() const
{
    return m_toAngle;
}

void PieChart::setToAngle(qreal newToAngle)
{
    if (qFuzzyCompare(newToAngle, m_toAngle)) {
        return;
    }

    m_toAngle = newToAngle;
    update();
    Q_EMIT toAngleChanged();
}


bool PieChart::smoothEnds() const
{
    return m_smoothEnds;
}

void PieChart::setSmoothEnds(bool newSmoothEnds)
{
    if (newSmoothEnds == m_smoothEnds) {
        return;
    }

    m_smoothEnds = newSmoothEnds;
    update();
    Q_EMIT smoothEndsChanged();
}


QSGNode *PieChart::updatePaintNode(QSGNode *node, UpdatePaintNodeData *data)
{
    Q_UNUSED(data);
    if (!node) {
        node = new QSGNode{};
    }

    auto sourceCount = valueSources().size();

    auto minDimension = std::min(width(), height());

    float outerRadius = minDimension;
    for (int i = 0; i < sourceCount; ++i) {
        float innerRadius = i == sourceCount - 1 && m_filled ? 0.0 : outerRadius - m_thickness;

        if (node->childCount() <= i)
            node->appendChildNode(new PieChartNode{});

        auto pieNode = static_cast<PieChartNode*>(node->childAtIndex(i));
        pieNode->setRect(boundingRect());
        pieNode->setInnerRadius(innerRadius);
        pieNode->setOuterRadius(outerRadius);
        pieNode->setSections(m_sections.at(i));
        pieNode->setBackgroundColor(m_backgroundColor);
        pieNode->setColors(m_colors.at(i));
        pieNode->setFromAngle(m_fromAngle);
        pieNode->setToAngle(m_toAngle);
        pieNode->setSmoothEnds(m_smoothEnds);

        outerRadius = outerRadius - m_thickness - m_spacing;
    }

    while (node->childCount() > sourceCount) {
        node->removeChildNode(node->childAtIndex(node->childCount() - 1));
    }

    return node;
}

void PieChart::onDataChanged()
{
    m_sections.clear();
    m_colors.clear();

    const auto sources = valueSources();
    const auto colors = colorSource();

    if (!colors || sources.isEmpty() || !m_range->isValid())
        return;

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

        if (qFuzzyCompare(total, 0.0)) {
            m_sections << QVector<qreal>{ 0.0 };
            m_colors << QVector<QColor>{ colors->item(colorIndex++).value<QColor>() };
        }

        qreal max = total;

        if (!m_range->automatic() && m_range->distance() >= total) {
            max = m_range->distance();
        }

        for (auto& value : sections) {
            value = value / (qFuzzyCompare(max, 0.0) ? 1.0 : max);
        }

        m_sections << sections;
        m_colors << sectionColors;
        colorIndex = m_continueColors ? colorIndex : 0;
    }

    update();
}
