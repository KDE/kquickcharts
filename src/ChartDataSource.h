#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>

class ChartDataSource : public QObject
{
    Q_OBJECT

public:
    ChartDataSource(QObject *parent = nullptr);
    virtual ~ChartDataSource();

    virtual int itemCount() const = 0;
    virtual QVariant item(int index) const = 0;

    Q_SIGNAL void dataChanged();
};

#endif // DATASOURCE_H
