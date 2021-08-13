/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "LineChartNode.h"

#include <QtMath>

#include "LineChartMaterial.h"
#include "LineSegmentNode.h"

static const int MaxPointsInSegment = 10;

qreal calculateNormalizedLineWidth(qreal pixelWidth, const QRectF &rect)
{
    if (qFuzzyIsNull(pixelWidth)) {
        return 0.0;
    }

    qreal min = 0.6 / std::max(rect.width(), rect.height());
    return std::max(min, (pixelWidth - 1.0) / (std::min(rect.width(), rect.height()) * 4.0));
}

LineChartNode::LineChartNode()
{
}

LineChartNode::~LineChartNode()
{
}

void LineChartNode::setRect(const QRectF &rect, qreal devicePixelRatio)
{
    if (rect == m_rect) {
        return;
    }

    m_rect = rect;
    m_aspect = m_rect.height() / m_rect.width();

    auto nativeSize = QSizeF(m_rect.width() * devicePixelRatio, m_rect.height() * devicePixelRatio);
    auto diagonal = std::sqrt(nativeSize.width() * nativeSize.width() + nativeSize.height() * nativeSize.height());
    m_smoothing = 1.0 / diagonal;

    updatePoints();
}

void LineChartNode::setLineWidth(float width)
{
    if (qFuzzyCompare(width, m_lineWidth)) {
        return;
    }

    m_lineWidth = width;
    std::for_each(m_segments.cbegin(), m_segments.cend(), [this](LineSegmentNode *node) {
        node->setLineWidth(calculateNormalizedLineWidth(m_lineWidth, m_rect));
    });
}

void LineChartNode::setLineColor(const QColor &color)
{
    if (m_lineColor == color) {
        return;
    }

    m_lineColor = color;
    std::for_each(m_segments.cbegin(), m_segments.cend(), [color](LineSegmentNode *node) {
        node->setLineColor(color);
    });
}

void LineChartNode::setFillColor(const QColor &color)
{
    if (m_fillColor == color) {
        return;
    }

    m_fillColor = color;
    std::for_each(m_segments.cbegin(), m_segments.cend(), [color](LineSegmentNode *node) {
        node->setFillColor(color);
    });
}

void LineChartNode::setValues(const QVector<QVector2D> &values)
{
    m_values = values;
    updatePoints();
}

void LineChartNode::updatePoints()
{
    if (m_values.isEmpty()) {
        return;
    }

    auto segmentCount = qCeil(qreal(m_values.count()) / MaxPointsInSegment);
    if (segmentCount != m_segments.count()) {
        removeAllChildNodes();
        qDeleteAll(m_segments);
        m_segments.clear();

        for (int i = 0; i < segmentCount; ++i) {
            auto node = new LineSegmentNode{};
            m_segments << node;
            appendChildNode(node);
        }
    }

    auto currentX = m_rect.left();
    auto pointStart = 0;
    auto pointsPerSegment = MaxPointsInSegment;

    for (auto segment : qAsConst(m_segments)) {
        auto segmentPoints = m_values.mid(pointStart, pointsPerSegment);
        pointStart += pointsPerSegment;

        if (pointStart < m_values.count()) {
            segmentPoints.append(m_values[pointStart]);
        }

        auto segmentWidth = segmentPoints.last().x() - currentX;
        auto rect = QRectF(currentX, m_rect.top(), segmentWidth, m_rect.height());

        segment->setRect(rect);
        segment->setAspect(segmentWidth / m_rect.width(), m_aspect);
        segment->setSmoothing(m_smoothing);
        segment->setLineWidth(calculateNormalizedLineWidth(m_lineWidth, m_rect));
        segment->setLineColor(m_lineColor);
        segment->setFillColor(m_fillColor);
        segment->setValues(segmentPoints);
        segment->setFarLeft(m_values.at(std::max(0, pointStart - pointsPerSegment - 1)));
        segment->setFarRight(m_values.at(std::min(m_values.count() - 1, pointStart + 1)));
        segment->updatePoints();

        currentX += segmentWidth;
    }
}
