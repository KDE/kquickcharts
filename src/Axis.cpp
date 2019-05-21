#include "Axis.h"

#include "RangeGroup.h"
#include "ChartDataSource.h"

Axis::Axis(QObject* parent)
    : QObject(parent)
{
    m_range = new RangeGroup{this};
}

Axis::~Axis()
{
}

QString Axis::title() const
{
    return m_title;
}

RangeGroup* Axis::range() const
{
    return m_range;
}

qreal Axis::stepSize() const
{
    return m_stepSize;
}

bool Axis::drawLines() const
{
    return m_drawLines;
}

ChartDataSource *Axis::labelSource() const
{
    return m_labelSource;
}

void Axis::setTitle(const QString& title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    emit titleChanged();
}

void Axis::setStepSize(qreal stepSize)
{
    if (m_stepSize == stepSize) {
        return;
    }

    m_stepSize = stepSize;
    emit stepSizeChanged();
}

void Axis::setDrawLines(bool drawLines)
{
    if (m_drawLines == drawLines) {
        return;
    }

    m_drawLines = drawLines;
    emit drawLinesChanged();
}

void Axis::setLabelSource(ChartDataSource *labelSource)
{
    if (m_labelSource == labelSource) {
        return;
    }

    m_labelSource = labelSource;
    emit labelSourceChanged();
}
