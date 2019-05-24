#include "LineChartNode.h"

#include <QColor>
#include <QSGGeometry>
#include <cmath>

#include "LineChartMaterial.h"

LineChartNode::LineChartNode()
    : LineChartNode(QRectF {})
{
}

LineChartNode::LineChartNode(const QRectF &rect)
{
    m_geometry = new QSGGeometry { QSGGeometry::defaultAttributes_TexturedPoint2D(), 4 };
    QSGGeometry::updateTexturedRectGeometry(m_geometry, rect, QRectF { 0, 0, 1, 1 });
    setGeometry(m_geometry);

    m_material = new LineChartMaterial {};
    setMaterial(m_material);

    setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial);
}

LineChartNode::~LineChartNode()
{
}

void LineChartNode::setRect(const QRectF &rect)
{
    if (rect == m_rect)
        return;

    m_rect = rect;
    QSGGeometry::updateTexturedRectGeometry(m_geometry, m_rect, QRectF { 0, 0, 1, 1 });
    markDirty(QSGNode::DirtyGeometry);

    m_material->setLineWidth(m_lineWidth / m_rect.height());
    markDirty(QSGNode::DirtyMaterial);

    updatePoints();
}

void LineChartNode::setLineWidth(float width)
{
    if(qFuzzyCompare(width, m_lineWidth))
        return;

    m_lineWidth = width;
    m_material->setLineWidth(width / m_rect.height());
    markDirty(QSGNode::DirtyMaterial);
}

void LineChartNode::setLineColor(const QColor& color)
{
    if(m_material->lineColor() == color)
        return;

    m_material->setLineColor(color);
    markDirty(QSGNode::DirtyMaterial);
}

void LineChartNode::setFillColor(const QColor& color)
{
    if(m_material->fillColor() == color)
        return;

    m_material->setFillColor(color);
    markDirty(QSGNode::DirtyMaterial);
}

void LineChartNode::setValues(const QVector<qreal>& values)
{
    m_values = values;
    updatePoints();
}

void LineChartNode::updatePoints()
{
    if(m_values.isEmpty())
        return;

    qreal valueStep = 1.0 / (m_values.count() - 1);

    QVector<QVector2D> points;
    qreal currentX = 0.0;

    points << QVector2D{-0.01, -0.01};
    points << QVector2D(-0.01, 1.0 - m_values[0]);

    for(auto value : qAsConst(m_values)) {
        points << QVector2D(currentX, 1.0 - value);
        currentX += valueStep;
    }

    points << QVector2D(1.01, points.last().y());
    points << QVector2D{1.01, -0.01};

    m_material->setPoints(points);
    markDirty(QSGNode::DirtyMaterial);
}
