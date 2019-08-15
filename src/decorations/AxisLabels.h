/*
 * This file is part of Quick Charts.
 * Copyright 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef AXISLABELS_H
#define AXISLABELS_H

#include <memory>

#include <QQuickItem>
#include <Qt>

#include "../namespace.h"

BEGIN_NAMESPACE

class ChartDataSource;

class AxisLabels;

class AxisLabelsAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ index NOTIFY indexChanged)
    Q_PROPERTY(QString label READ label NOTIFY labelChanged)

public:
    explicit AxisLabelsAttached(QObject *parent = nullptr);

    int index() const;
    void setIndex(int newIndex);
    Q_SIGNAL void indexChanged();

    QString label() const;
    void setLabel(const QString &newLabel);
    Q_SIGNAL void labelChanged();

private:
    int m_index = -1;
    QString m_label;
};

/**
 * An item that uses a delegate to place axis labels on a chart.
 */
class AxisLabels : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(AxisLabels::Direction direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QQmlComponent *delegate READ delegate WRITE setDelegate NOTIFY delegateChanged)
    Q_PROPERTY(ChartDataSource *source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment NOTIFY alignmentChanged)
    Q_PROPERTY(bool constrainToBounds READ constrainToBounds WRITE setConstrainToBounds NOTIFY constrainToBoundsChanged)

public:
    enum class Direction { HorizontalLeftRight, HorizontalRightLeft, VerticalTopBottom, VerticalBottomTop };
    Q_ENUM(Direction)

    explicit AxisLabels(QQuickItem *parent = nullptr);

    AxisLabels::Direction direction() const;
    Q_SLOT void setDirection(AxisLabels::Direction newDirection);
    Q_SIGNAL void directionChanged();

    QQmlComponent *delegate() const;
    Q_SLOT void setDelegate(QQmlComponent *newDelegate);
    Q_SIGNAL void delegateChanged();

    ChartDataSource *source() const;
    Q_SLOT void setSource(ChartDataSource *newSource);
    Q_SIGNAL void sourceChanged();

    Qt::Alignment alignment() const;
    Q_SLOT void setAlignment(Qt::Alignment newAlignment);
    Q_SIGNAL void alignmentChanged();

    bool constrainToBounds() const;
    Q_SLOT void setConstrainToBounds(bool newConstrainToBounds);
    Q_SIGNAL void constrainToBoundsChanged();

    static AxisLabelsAttached *qmlAttachedProperties(QObject *object)
    {
        return new AxisLabelsAttached(object);
    }

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    void scheduleLayout();
    bool isHorizontal();
    void updateLabels();
    void layout();

    Direction m_direction = Direction::HorizontalLeftRight;
    QQmlComponent *m_delegate = nullptr;
    ChartDataSource *m_source = nullptr;
    Qt::Alignment m_alignment = Qt::AlignHCenter | Qt::AlignVCenter;
    bool m_constrainToBounds = true;
    QVector<QQuickItem *> m_labels;
    bool m_layoutScheduled = false;
};

END_NAMESPACE

QML_DECLARE_TYPEINFO(AxisLabels, QML_HAS_ATTACHED_PROPERTIES)

#endif // AXISLABELS_H
