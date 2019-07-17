#ifndef PIECHART_H
#define PIECHART_H

#include <memory>

#include "Chart.h"

class ChartDataSource;
class RangeGroup;

/**
 * @todo write docs
 */
class PieChart : public Chart
{
    Q_OBJECT

    /**
     * The range of values to display in this PieChart. When set to "automatic", the values will be divide across the
     * entire chart. .
     */
    Q_PROPERTY(RangeGroup *range READ range CONSTANT)
    /**
     * Whether to use a filled Pie or not. If true, the first Pie rendered will be rendered as a filled circle.
     */
    Q_PROPERTY(bool filled READ filled WRITE setFilled NOTIFY filledChanged)
    /**
     * The thickness of an individual pie. If filled is set, this is ignored for the first pie.
     */
    Q_PROPERTY(qreal thickness READ thickness WRITE setThickness NOTIFY thicknessChanged)
    /**
     * The amount of spacing between stacked pies.
     */
    Q_PROPERTY(qreal spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    /**
     * Sets a colour to use to fill remaining space on the arc.
     */
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    /**
     * Whether to continue indexing colors when drawing other pies instead of starting from 0 again.
     */
    Q_PROPERTY(bool continueColors READ continueColors WRITE setContinueColors NOTIFY continueColorsChanged)

    Q_PROPERTY(qreal fromAngle READ fromAngle WRITE setFromAngle NOTIFY fromAngleChanged)

    Q_PROPERTY(qreal toAngle READ toAngle WRITE setToAngle NOTIFY toAngleChanged)

    Q_PROPERTY(bool smoothEnds READ smoothEnds WRITE setSmoothEnds NOTIFY smoothEndsChanged)

public:
    /**
     * Default constructor
     */
    PieChart(QQuickItem *parent = nullptr);

    /**
     * Destructor
     */
    ~PieChart();

    RangeGroup *range() const;

    bool filled() const;
    Q_SLOT void setFilled(bool newFilled);
    Q_SIGNAL void filledChanged();

    qreal thickness() const;
    Q_SLOT void setThickness(qreal newThickness);
    Q_SIGNAL void thicknessChanged();

    qreal spacing() const;
    Q_SLOT void setSpacing(qreal newSpacing);
    Q_SIGNAL void spacingChanged();

    QColor backgroundColor() const;
    Q_SLOT void setBackgroundColor(const QColor &color);
    Q_SIGNAL void backgroundColorChanged();

    bool continueColors() const;
    Q_SLOT void setContinueColors(bool newContinueColors);
    Q_SIGNAL void continueColorsChanged();

    qreal fromAngle() const;
    Q_SLOT void setFromAngle(qreal newFromAngle);
    Q_SIGNAL void fromAngleChanged();

    qreal toAngle() const;
    Q_SLOT void setToAngle(qreal newToAngle);
    Q_SIGNAL void toAngleChanged();

    bool smoothEnds() const;
    Q_SLOT void setSmoothEnds(bool newSmoothEnds);
    Q_SIGNAL void smoothEndsChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data) override;
    void onDataChanged() override;

private:
    std::unique_ptr<RangeGroup> m_range;
    bool m_filled = false;
    qreal m_thickness = 10.0;
    qreal m_spacing = 0.0;
    QColor m_backgroundColor = Qt::transparent;
    bool m_continueColors = false;
    qreal m_fromAngle = 0.0;
    qreal m_toAngle = 360.0;
    bool m_smoothEnds = false;

    QVector<QVector<qreal>> m_sections;
    QVector<QVector<QColor>> m_colors;
};

#endif // PIECHART_H
