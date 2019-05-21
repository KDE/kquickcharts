#ifndef ARRAYSOURCE_H
#define ARRAYSOURCE_H

#include <QVariantList>

#include "ChartDataSource.h"

/**
 * @todo write docs
 */
class ArraySource : public ChartDataSource
{
    Q_OBJECT
    Q_PROPERTY(QVariantList array READ array WRITE setArray NOTIFY dataChanged)

public:
    /**
     * Constructor
     *
     * @param parent TODO
     */
    ArraySource(QObject* parent = nullptr);

    /**
     * Destructor
     */
    ~ArraySource();

    virtual int itemCount() const override;
    virtual QVariant item(int index) const override;

    QVariantList array() const;

public Q_SLOTS:
    void setArray(QVariantList array);

private:
    QVariantList m_array;
};

#endif // ARRAYSOURCE_H
