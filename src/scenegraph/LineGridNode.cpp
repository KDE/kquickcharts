#include "LineGridNode.h"

#include <QSGVertexColorMaterial>

LineGridNode::LineGridNode()
{
    m_geometry = new QSGGeometry{QSGGeometry::defaultAttributes_ColoredPoint2D(), 0};
    m_geometry->setDrawingMode(QSGGeometry::DrawLines);
    m_geometry->setLineWidth(1.0);
    setGeometry(m_geometry);

    m_borderColor = QColor(128, 128, 128, 255);
    m_xColor = QColor(0, 0, 0, 50);
    m_yColor = QColor(0, 0, 0, 50);

    setMaterial(new QSGVertexColorMaterial {});

    setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial);
}

LineGridNode::~LineGridNode()
{
}

void LineGridNode::setRect(const QRectF& rect)
{
    if (rect == m_rect)
        return;

    m_rect = rect;
}

void LineGridNode::setDrawX(bool drawX)
{
    if (m_drawX == drawX) {
        return;
    }

    m_drawX = drawX;
}

void LineGridNode::setDrawY(bool drawY)
{
    if (m_drawY == drawY) {
        return;
    }

    m_drawY = drawY;
}

void LineGridNode::setXSpacing(qreal xSpacing)
{
    if (m_xSpacing == xSpacing) {
        return;
    }

    m_xSpacing = xSpacing;
}

void LineGridNode::setYSpacing(qreal ySpacing)
{
    if (m_ySpacing == ySpacing) {
        return;
    }

    m_ySpacing = ySpacing;
}

void LineGridNode::setXColor(const QColor& xColor)
{
    if (m_xColor == xColor) {
        return;
    }

    m_xColor = xColor;
}

void LineGridNode::setYColor(const QColor& yColor)
{
    if (m_yColor == yColor) {
        return;
    }

    m_yColor = yColor;
}

void LineGridNode::update()
{
    if(!m_rect.isValid())
        return;

    int totalVertices = 0;

    if (m_drawX) {
        totalVertices += 2 * (m_rect.width() / m_xSpacing);
    }

    if (m_drawY) {
        totalVertices += 2 * (m_rect.height() / m_ySpacing);
    }

    if (totalVertices != m_geometry->vertexCount()) {
        m_geometry->allocate(totalVertices, totalVertices);
    }

    auto vertices = m_geometry->vertexDataAsColoredPoint2D();
    auto indices = m_geometry->indexDataAsUShort();

    int index = 0;

    if (m_drawX) {
        for (auto i = m_rect.width() - m_xSpacing; i > 0.0; i -= m_xSpacing) {
            line(vertices, indices, index, i, m_rect.top(), i, m_rect.bottom(), m_xColor);
        }
    }

    if (m_drawY) {
        for (auto i = m_rect.height() - m_ySpacing; i > 0.0; i -= m_ySpacing) {
            line(vertices, indices, index, m_rect.left(), i, m_rect.right(), i, m_yColor);
        }
    }

    m_geometry->markVertexDataDirty();
    m_geometry->markIndexDataDirty();
    markDirty(QSGNode::DirtyGeometry);
}

void LineGridNode::line(QSGGeometry::ColoredPoint2D* vertices, quint16 *indices, int &index, qreal fromX, qreal fromY, qreal toX, qreal toY, const QColor& color)
{
    indices[index] = index;
    vertices[index++].set(fromX, fromY, color.red(), color.green(), color.blue(), color.alpha());
    indices[index] = index;
    vertices[index++].set(toX, toY, color.red(), color.green(), color.blue(), color.alpha());
}
