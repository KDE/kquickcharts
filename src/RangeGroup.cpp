#include "RangeGroup.h"

RangeGroup::RangeGroup(QObject* parent)
    : QObject(parent)
{
    connect(this, &RangeGroup::fromChanged, this, &RangeGroup::rangeChanged);
    connect(this, &RangeGroup::toChanged, this, &RangeGroup::rangeChanged);
    connect(this, &RangeGroup::automaticChanged, this, &RangeGroup::rangeChanged);
}

RangeGroup::~RangeGroup()
{
}

qreal RangeGroup::from() const
{
    return m_from;
}

qreal RangeGroup::to() const
{
    return m_to;
}

bool RangeGroup::automatic() const
{
    return m_automatic;
}

bool RangeGroup::isValid() const
{
    return m_automatic || (m_to > m_from);
}

qreal RangeGroup::distance() const
{
    return m_to - m_from;
}

void RangeGroup::setFrom(qreal from)
{
    if (qFuzzyCompare(m_from, from)) {
        return;
    }

    m_from = from;
    Q_EMIT fromChanged();
}

void RangeGroup::setTo(qreal to)
{
    if (qFuzzyCompare(m_to, to)) {
        return;
    }

    m_to = to;
    Q_EMIT toChanged();
}

void RangeGroup::setAutomatic(bool automatic)
{
    if (m_automatic == automatic) {
        return;
    }

    m_automatic = automatic;
    Q_EMIT automaticChanged();
}
