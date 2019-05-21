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
    if(index < 0 || index > m_array.count() - 1)
        return QVariant{};

    return m_array.at(index);
}

QVariantList ArraySource::array() const
{
    return m_array;
}

void ArraySource::setArray(QVariantList array)
{
    if (m_array == array) {
        return;
    }

    m_array = array;
    emit dataChanged();
}
