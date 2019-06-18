#ifndef LINESEGMENTNODE_H
#define LINESEGMENTNODE_H

#include <QSGGeometryNode>
#include <QColor>

class QRectF;
class LineChartMaterial;

/**
 * @todo write docs
 */
class LineSegmentNode : public QSGGeometryNode
{
public:
    LineSegmentNode();

    /**
     * Default constructor
     */
    explicit LineSegmentNode(const QRectF &rect);

    /**
     * Destructor
     */
    ~LineSegmentNode();

    void setRect(const QRectF &rect);
    void setLineWidth(float width);
    void setLineColor(const QColor& color);
    void setFillColor(const QColor& color);
    void setValues(const QVector<QVector2D>& values);

private:
    void updatePoints();

    QRectF m_rect;
    float m_lineWidth = 0.0;
    float m_aspect = 1.0;
    QVector<QVector2D> m_values;
    QSGGeometry *m_geometry = nullptr;
    LineChartMaterial *m_material = nullptr;
};

#endif // LINESEGMENTNODE_H
