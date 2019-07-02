#ifndef XYCHART_H
#define XYCHART_H

#include "Chart.h"

class RangeGroup;

struct ComputedRange {
    int startX = 0;
    int endX = 0;
    int distanceX = 0;
    float startY = 0.0;
    float endY = 0.0;
    float distanceY = 0.0;
};

/**
 * @todo write docs
 */
class XYChart : public Chart
{
    Q_OBJECT
    Q_PROPERTY(RangeGroup* xRange READ xRange CONSTANT)
    Q_PROPERTY(RangeGroup* yRange READ yRange CONSTANT)
    Q_PROPERTY(Direction direction READ direction WRITE setDirection NOTIFY directionChanged)

public:
    enum class Direction {
        ZeroAtStart,
        ZeroAtEnd
    };
    Q_ENUM(Direction)

    /**
     * Constructor
     *
     * @param parent TODO
     */
    XYChart(QQuickItem *parent = nullptr);

    /**
     * Destructor
     */
    virtual ~XYChart();

    virtual RangeGroup *xRange() const;
    virtual RangeGroup *yRange() const;
    virtual XYChart::Direction direction() const;
    Q_SLOT virtual void setDirection(XYChart::Direction newDirection);
    Q_SIGNAL void directionChanged();

    const ComputedRange computedRange() const;
    Q_SIGNAL void computedRangeChanged();

protected:
    virtual void updateComputedRange();
    virtual void updateAutomaticXRange(ComputedRange &range) = 0;
    virtual void updateAutomaticYRange(ComputedRange &range) = 0;

private:
    RangeGroup* m_xRange = nullptr;
    RangeGroup* m_yRange = nullptr;
    Direction m_direction = Direction::ZeroAtStart;
    ComputedRange m_computedRange;
};

#endif // XYCHART_H
