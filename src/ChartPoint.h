/*
 * SPDX-FileCopyrightText: 2021 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef CHARTPOINT_H
#define CHARTPOINT_H

#include <QColor>
#include <QObject>
#include <QVariant>

class ChartDataSource;

/**
 * @todo write docs
 */
class ChartPoint
{
    Q_GADGET

public:
    ChartPoint();

    Q_PROPERTY(bool valid READ isValid)
    bool isValid() const;

    /**
     * TODO: Documentation
     */
    Q_PROPERTY(qreal x MEMBER x)
    qreal x = 0.0;

    /**
     * TODO: Documentation
     */
    Q_PROPERTY(qreal y MEMBER y)
    qreal y = 0.0;

    Q_PROPERTY(ChartDataSource *valueSource MEMBER valueSource)
    ChartDataSource *valueSource = nullptr;

    Q_PROPERTY(int item MEMBER item)
    int item = 0;

    Q_PROPERTY(QVariant value MEMBER value)
    QVariant value;

    Q_PROPERTY(QString name MEMBER name)
    QString name;

    Q_PROPERTY(QString shortName READ getShortName)
    QString shortName;

    QString getShortName() const
    {
        if (!shortName.isEmpty()) {
            return shortName;
        }
        return name;
    }

    Q_PROPERTY(QColor color MEMBER color)
    QColor color;
};

Q_DECLARE_METATYPE(ChartPoint)

#endif // CHARTPOINT_H
