#ifndef RANGEGROUP_H
#define RANGEGROUP_H

#include <QObject>

/**
 * @todo write docs
 */
class RangeGroup : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal from READ from WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(qreal to READ to WRITE setTo NOTIFY toChanged)
    Q_PROPERTY(bool automatic READ automatic WRITE setAutomatic NOTIFY automaticChanged)
    Q_PROPERTY(qreal distance READ distance NOTIFY rangeChanged)

public:
    RangeGroup(QObject* parent = nullptr);
    ~RangeGroup();

    qreal from() const;
    qreal to() const;
    bool automatic() const;

    bool isValid() const;
    qreal distance() const;

public Q_SLOTS:
    void setFrom(qreal from);
    void setTo(qreal to);
    void setAutomatic(bool automatic);

Q_SIGNALS:
    void fromChanged();
    void toChanged();
    void automaticChanged();
    void rangeChanged();
    void distanceChanged();

private:
    qreal m_from = 0.0;
    qreal m_to = 100.0;
    bool m_automatic = true;
};

#endif // RANGEGROUP_H
