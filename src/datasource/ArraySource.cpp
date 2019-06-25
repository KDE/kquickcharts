#include "ArraySource.h"

ArraySource::ArraySource(QObject* parent)
    : ChartDataSource(parent)
{
}

ArraySource::~ArraySource()
{
}

int ArraySource::itemCount() const
{
    return m_array.count();
}

QVariant ArraySource::item(int index) const
{
    if(!m_wrap && (index < 0 || index > m_array.count() - 1))
        return QVariant{};

    return m_array.at(index % m_array.count());
}

QVariant ArraySource::minimum() const
{
    return *std::min_element(m_array.begin(), m_array.end());
}

QVariant ArraySource::maximum() const
{
    return *std::max_element(m_array.begin(), m_array.end());
}

QVariantList ArraySource::array() const
{
    return m_array;
}

bool ArraySource::wrap() const
{
    return m_wrap;
}

void ArraySource::setArray(const QVariantList &array)
{
    if (m_array == array) {
        return;
    }

    m_array = array;
    Q_EMIT dataChanged();
}

void ArraySource::setWrap(bool wrap)
{
    if (m_wrap == wrap) {
        return;
    }

    m_wrap = wrap;
    Q_EMIT dataChanged();
}
