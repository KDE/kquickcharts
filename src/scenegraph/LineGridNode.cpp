#include "LineGridNode.h"

#include <QSGFlatColorMaterial>

LineGridNode::LineGridNode()
{
    m_geometry = new QSGGeometry{QSGGeometry::defaultAttributes_Point2D(), 0};
    m_geometry->setDrawingMode(QSGGeometry::DrawLines);
    m_geometry->setLineWidth(m_lineWidth);
    setGeometry(m_geometry);

    m_material = new QSGFlatColorMaterial{};
    m_material->setColor(QColor(255,0,0,255));
    setMaterial(m_material);

    setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial | QSGNode::UsePreprocess);
}

LineGridNode::~LineGridNode()
{
}

void LineGridNode::setVisible(bool visible)
{
    if (visible == m_visible)
        return;

    m_visible = visible;
    markDirty(QSGNode::DirtySubtreeBlocked);
}

void LineGridNode::setVertical(bool vertical)
{
    if (vertical == m_vertical)
        return;

    m_vertical = vertical;
//     update();
//     setFlag(QSGNode::UsePreprocess, true);
}

void LineGridNode::setRect(const QRectF& rect)
{
    if (rect == m_rect)
        return;

    m_rect = rect;
//     update();
//     setFlag(QSGNode::UsePreprocess, true);
}

void LineGridNode::setColor(const QColor& color)
{
    if (color == m_material->color())
        return;

    m_material->setColor(color);
    markDirty(QSGNode::DirtyMaterial);
}

void LineGridNode::setSpacing(float spacing)
{
    if (qFuzzyCompare(spacing, m_spacing))
        return;

    m_spacing = spacing;
//     update();
//     setFlag(QSGNode::UsePreprocess, true);
}

void LineGridNode::setLineWidth(float lineWidth)
{
    if (qFuzzyCompare(lineWidth, m_lineWidth)) {
        return;
    }

    m_lineWidth = lineWidth;
    m_geometry->setLineWidth(lineWidth);
    markDirty(QSGNode::DirtyGeometry);
}

bool LineGridNode::isSubtreeBlocked() const
{
    return !m_visible;
}

void LineGridNode::update()
{
    if(!m_rect.isValid())
        return;

    int totalVertices = 0;
    if (!m_vertical) {
        totalVertices = int(m_rect.width() / m_spacing - 1) * 2;
    } else {
        totalVertices = int(m_rect.height() / m_spacing) * 2;
    }

    qDebug() << "verts" << totalVertices << m_rect.width() << m_spacing;

    if (totalVertices != m_geometry->vertexCount()) {
        m_geometry->allocate(totalVertices, totalVertices);
    }

    auto vertices = m_geometry->vertexDataAsPoint2D();
    auto indices = m_geometry->indexDataAsUShort();

    int index = 0;
    if (m_vertical) {
        for (auto i = m_rect.height() - m_spacing; i > 0.0; i -= m_spacing) {
            line(vertices, indices, index, m_rect.left(), i, m_rect.right(), i);
        }
    } else {
        for (auto i = 0.0; i < m_rect.width(); i += m_spacing) {
            line(vertices, indices, index, i, m_rect.top(), i, m_rect.bottom());
        }
    }

    m_geometry->markVertexDataDirty();
    m_geometry->markIndexDataDirty();
    markDirty(QSGNode::DirtyGeometry);
//     setFlag(QSGNode::UsePreprocess, false);
}

void LineGridNode::line(QSGGeometry::Point2D* vertices, quint16 *indices, int &index, qreal fromX, qreal fromY, qreal toX, qreal toY)
{
    indices[index] = index;
    vertices[index++].set(fromX, fromY);
    indices[index] = index;
    vertices[index++].set(toX, toY);
}
