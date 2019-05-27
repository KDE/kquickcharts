#ifndef LINECHARTNODE_H
#define LINECHARTNODE_H

#include <QSGGeometryNode>
#include <QColor>

class QRectF;
class LineChartMaterial;

/**
 * @todo write docs
 */
class LineChartNode : public QSGGeometryNode
{
public:
    LineChartNode();

    /**
     * Default constructor
     */
    explicit LineChartNode(const QRectF &rect);

    /**
     * Destructor
     */
    ~LineChartNode();

    void setRect(const QRectF &rect);
    void setLineWidth(float width);
    void setLineColor(const QColor& color);
    void setFillColor(const QColor& color);
    void setValues(const QVector<qreal>& values);

private:
    void updatePoints();

    QRectF m_rect;
    float m_lineWidth = 0.0;
    QVector<qreal> m_values;
    QSGGeometry *m_geometry = nullptr;
    LineChartMaterial *m_material = nullptr;
};

#endif // LINECHARTNODE_H
