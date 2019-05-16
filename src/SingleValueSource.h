#ifndef SINGLEVALUESOURCE_H
#define SINGLEVALUESOURCE_H

#include <QVariant>

#include "ChartDataSource.h"

class SingleValueSource : public ChartDataSource
{
    Q_OBJECT
    Q_PROPERTY ( QVariant value READ value WRITE setValue NOTIFY dataChanged )

public:
    SingleValueSource ( QObject* parent = nullptr );
    ~SingleValueSource();

    virtual int itemCount() const override;
    virtual QVariant item ( int index ) const override;

    QVariant value() const;

public Q_SLOTS:
    void setValue ( const QVariant& value );

private:
    QVariant m_value;
};

#endif // SINGLEVALUESOURCE_H
