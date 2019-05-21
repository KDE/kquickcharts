#ifndef LINEGRIDNODE_H
#define LINEGRIDNODE_H

#include <QSGGeometryNode>
#include <QColor>

/**
 * @todo write docs
 */
class LineGridNode : public QSGGeometryNode
{
public:
    LineGridNode();
    ~LineGridNode();

    void setRect(const QRectF& rect);
    void setDrawX(bool drawX);
    void setDrawY(bool drawY);
    void setXSpacing(qreal xSpacing);
    void setYSpacing(qreal ySpacing);
    void setXColor(const QColor& xColor);
    void setYColor(const QColor& yColor);
    void update();

private:
    void line(QSGGeometry::ColoredPoint2D *vertices, quint16 *indices, int &index, qreal fromX, qreal fromY, qreal toX, qreal toY, const QColor& color);

    QSGGeometry *m_geometry = nullptr;
    QRectF m_rect;
    bool m_drawBorder = true;
    bool m_drawX = true;
    bool m_drawY = true;
    qreal m_xSpacing = 1.0;
    qreal m_ySpacing = 1.0;
    QColor m_xColor;
    QColor m_yColor;
    QColor m_borderColor;
};

#endif // LINEGRIDNODE_H
