#ifndef PIECHARTNODE_H
#define PIECHARTNODE_H

#include <QSGGeometryNode>

class QRectF;
class PieChartMaterial;

/**
 * @todo write docs
 */
class PieChartNode : public QSGGeometryNode
{
public:
    PieChartNode();

    /**
     * Default constructor
     */
    explicit PieChartNode(const QRectF &rect);

    /**
     * Destructor
     */
    ~PieChartNode();

    void setRect(const QRectF &rect);
    void setBorderWidth(qreal width);
    void setSections(const QVector<qreal> &sections);
    void setColors(const QVector<QColor> &colors);

private:
    void updateTriangles();

    QRectF m_rect;
    qreal m_borderWidth = 0.0;

    QVector<qreal> m_sections;
    QVector<QColor> m_colors;

    QSGGeometry *m_geometry = nullptr;
    PieChartMaterial *m_material = nullptr;
};

#endif // PIECHARTNODE_H
