#include "SingleValueSource.h"

SingleValueSource::SingleValueSource ( QObject* parent )
    : ChartDataSource(parent)
{

}

SingleValueSource::~SingleValueSource()
{
}

QVariant SingleValueSource::item ( int index ) const
{
    Q_UNUSED(index);
    return m_value;
}

int SingleValueSource::itemCount() const
{
    return 1;
}

QVariant SingleValueSource::minimum() const
{
    return m_value;
}

QVariant SingleValueSource::maximum() const
{
    return m_value;
}

QVariant SingleValueSource::value() const
{
    return m_value;
}

void SingleValueSource::setValue ( const QVariant& value )
{
    if ( m_value == value ) {
        return;
    }

    m_value = value;
    Q_EMIT dataChanged();
}
