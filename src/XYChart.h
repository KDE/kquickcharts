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

bool operator==(const ComputedRange &first, const ComputedRange &second);

/**
 * @todo write docs
 */
class XYChart : public Chart
{
    Q_OBJECT
    Q_PROPERTY(RangeGroup* xRange READ xRange CONSTANT)
    Q_PROPERTY(RangeGroup* yRange READ yRange CONSTANT)
    Q_PROPERTY(Direction direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(bool stacked READ stacked WRITE setStacked NOTIFY stackedChanged)

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

    bool stacked() const;
    Q_SLOT void setStacked(bool newStacked);
    Q_SIGNAL void stackedChanged();

    const ComputedRange computedRange() const;
    Q_SIGNAL void computedRangeChanged();

protected:
    virtual void updateComputedRange();
    virtual void updateAutomaticXRange(ComputedRange &range);
    virtual void updateAutomaticYRange(ComputedRange &range);

private:
    RangeGroup* m_xRange = nullptr;
    RangeGroup* m_yRange = nullptr;
    Direction m_direction = Direction::ZeroAtStart;
    bool m_stacked = false;
    ComputedRange m_computedRange;
};

QDebug operator<<(QDebug debug, const ComputedRange &range);

#endif // XYCHART_H
