#include "LineSegmentNode.h"

#include <QColor>
#include <QSGGeometry>
#include <cmath>

#include "LineChartMaterial.h"

LineSegmentNode::LineSegmentNode()
    : LineSegmentNode(QRectF {})
{
}

LineSegmentNode::LineSegmentNode(const QRectF &rect)
{
    m_geometry = new QSGGeometry { QSGGeometry::defaultAttributes_TexturedPoint2D(), 4 };
    QSGGeometry::updateTexturedRectGeometry(m_geometry, rect, QRectF { 0, 0, 1, 1 });
    setGeometry(m_geometry);

    m_material = new LineChartMaterial {};
    setMaterial(m_material);

    setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial);
}

LineSegmentNode::~LineSegmentNode()
{
}

void LineSegmentNode::setRect(const QRectF &rect)
{
    if (rect == m_rect)
        return;

    m_rect = rect;
    QSGGeometry::updateTexturedRectGeometry(m_geometry, m_rect, QRectF { 0.0, 0, m_xAspect, 1 });
    markDirty(QSGNode::DirtyGeometry);

    updatePoints();
}


void LineSegmentNode::setAspect(float xAspect, float yAspect)
{
    if(qFuzzyCompare(xAspect, m_xAspect) && qFuzzyCompare(yAspect, m_yAspect))
        return;

    m_yAspect = yAspect;
    m_material->setAspect(m_yAspect);
    markDirty(QSGNode::DirtyMaterial);

    m_xAspect = xAspect;
    QSGGeometry::updateTexturedRectGeometry(m_geometry, m_rect, QRectF { 0.0, 0, m_xAspect, 1 });
    markDirty(QSGNode::DirtyGeometry);
}

void LineSegmentNode::setLineWidth(float width)
{
    if(qFuzzyCompare(width, m_lineWidth))
        return;

    m_lineWidth = width;
    m_material->setLineWidth(width);
    markDirty(QSGNode::DirtyMaterial);
}

void LineSegmentNode::setLineColor(const QColor& color)
{
    if(m_material->lineColor() == color)
        return;

    m_material->setLineColor(color);
    markDirty(QSGNode::DirtyMaterial);
}

void LineSegmentNode::setFillColor(const QColor& color)
{
    if(m_material->fillColor() == color)
        return;

    m_material->setFillColor(color);
    markDirty(QSGNode::DirtyMaterial);
}

void LineSegmentNode::setValues(const QVector<QVector2D>& values)
{
    m_values = values;
}

void LineSegmentNode::setFarLeft(const QVector2D& value)
{
    m_farLeft = value;
}

void LineSegmentNode::setFarRight(const QVector2D& value)
{
    m_farRight = value;
}

void LineSegmentNode::updatePoints()
{
    if(m_values.isEmpty())
        return;

    QVector<QVector2D> points;
    points.reserve(m_values.size() + 6);

    points << QVector2D{0.0,  -0.5};
    points << QVector2D{-0.5, -0.5};

    if (!m_farLeft.isNull()) {
        points << QVector2D(-0.5, m_farLeft.y() * m_yAspect);
        points << QVector2D(((m_farLeft.x() - m_rect.left()) / m_rect.width()) * m_xAspect, m_farLeft.y() * m_yAspect);
    } else {
        points << QVector2D(-0.5, m_values[0].y() * m_yAspect);
    }

    for(auto value : qAsConst(m_values)) {
        auto x = ((value.x() - m_rect.left()) / m_rect.width()) * m_xAspect;
        points << QVector2D(x, value.y() * m_yAspect);
    }

    if (!m_farRight.isNull()) {
        points << QVector2D(((m_farRight.x() - m_rect.left()) / m_rect.width()) * m_xAspect, m_farRight.y() * m_yAspect);
        points << QVector2D(1.5, m_farRight.y() * m_yAspect);
    } else {
        points << QVector2D(1.5, points.last().y());
    }

    points << QVector2D{1.5, -0.5};
    points << QVector2D{0.0, -0.5};

    m_material->setPoints(points);
    markDirty(QSGNode::DirtyMaterial);
}
