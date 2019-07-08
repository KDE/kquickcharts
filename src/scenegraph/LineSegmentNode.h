#ifndef LINESEGMENTNODE_H
#define LINESEGMENTNODE_H

#include <QSGGeometryNode>
#include <QColor>
#include <QVector2D>

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
    void setAspect(float xAspect, float yAspect);
    void setLineWidth(float width);
    void setLineColor(const QColor& color);
    void setFillColor(const QColor& color);
    void setValues(const QVector<QVector2D>& values);
    void setFarLeft(const QVector2D &value);
    void setFarRight(const QVector2D &value);

    void updatePoints();

private:
    QRectF m_rect;
    float m_lineWidth = 0.0;
    float m_xAspect = 1.0;
    float m_yAspect = 1.0;
    QVector2D m_farLeft;
    QVector2D m_farRight;
    QVector<QVector2D> m_values;
    QSGGeometry *m_geometry = nullptr;
    LineChartMaterial *m_material = nullptr;
};

#endif // LINESEGMENTNODE_H
