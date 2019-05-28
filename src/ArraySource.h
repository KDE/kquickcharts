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
    Q_PROPERTY(bool wrap READ wrap WRITE setWrap NOTIFY dataChanged)

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
    QVariant minimum() const override;
    QVariant maximum() const override;

    QVariantList array() const;
    bool wrap() const;

public Q_SLOTS:
    void setArray(QVariantList array);
    void setWrap(bool wrap);

private:
    QVariantList m_array;
    bool m_wrap = false;
};

#endif // ARRAYSOURCE_H
