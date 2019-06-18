#include "LineChartNode.h"

#include <QColor>
#include <QtMath>

#include "LineChartMaterial.h"
#include "LineSegmentNode.h"

static const int MaxPointsInSegment = 100;

LineChartNode::LineChartNode()
{
}

LineChartNode::~LineChartNode()
{
}

void LineChartNode::setRect(const QRectF &rect)
{
    if (rect == m_rect)
        return;

    m_rect = rect;
    m_aspect = m_rect.height() / m_rect.width();
    std::for_each(m_segments.begin(), m_segments.end(), [this](LineSegmentNode* node) { node->setAspect(m_aspect); } );
    updatePoints();
}

void LineChartNode::setLineWidth(float width)
{
    if(qFuzzyCompare(width, m_lineWidth))
        return;

    m_lineWidth = width;
    std::for_each(m_segments.begin(), m_segments.end(), [this](LineSegmentNode* node) { node->setLineWidth(m_lineWidth / m_rect.width()); });
}

void LineChartNode::setLineColor(const QColor& color)
{
    if(m_lineColor == color)
        return;

    m_lineColor = color;
    std::for_each(m_segments.begin(), m_segments.end(), [color](LineSegmentNode* node) { node->setLineColor(color); });
}

void LineChartNode::setFillColor(const QColor& color)
{
    if(m_fillColor == color)
        return;

    m_fillColor = color;
    std::for_each(m_segments.begin(), m_segments.end(), [color](LineSegmentNode* node) { node->setFillColor(color); });
}

void LineChartNode::setValues(const QVector<QVector2D> &values)
{
    m_values = values;
    updatePoints();
}

void LineChartNode::updatePoints()
{
    if(m_values.isEmpty())
        return;

    auto segmentCount = qCeil(qreal(m_values.count()) / MaxPointsInSegment);
    if(segmentCount != m_segments.count()) {
        removeAllChildNodes();
        m_segments.clear();

        for(int i = 0; i < segmentCount; ++i) {
            auto node = new LineSegmentNode{};
            m_segments << node;
            appendChildNode(node);
        }
    }

    auto currentX = m_rect.left();
    auto pointStart = 0;
    auto pointsPerSegment = m_values.count() / m_segments.count();

    for(auto segment : qAsConst(m_segments)) {
        auto segmentPoints = m_values.mid(pointStart, pointsPerSegment);
        pointStart += pointsPerSegment;

        if (pointStart < m_values.count()) {
            segmentPoints.append(m_values[pointStart]);
        }

        auto segmentWidth = segmentPoints.last().x() - currentX;
        auto rect = QRectF(currentX, m_rect.top(), segmentWidth, m_rect.height());

        currentX += segmentWidth;
        segment->setRect(rect);
        segment->setAspect(m_aspect);
        segment->setLineWidth(m_lineWidth / m_rect.width());
        segment->setLineColor(m_lineColor);
        segment->setFillColor(m_fillColor);
        segment->setValues(segmentPoints);
    }
}
