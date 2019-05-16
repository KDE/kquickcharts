#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>

class DataSource : public QObject
{
    Q_OBJECT

public:
    DataSource(QObject* parent = nullptr);
    virtual ~DataSource();

    virtual int itemCount() const = 0;
    virtual QVariant item(int index) const = 0;
//     virtual QVector<QVariant> items() const = 0;

    Q_SIGNAL void dataChanged();
};

#endif // DATASOURCE_H
