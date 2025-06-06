/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef AXISLABELS_H
#define AXISLABELS_H

#include <memory>

#include <QQuickItem>
#include <Qt>
#include <qqmlregistration.h>

class ChartDataSource;
class ItemBuilder;

class AxisLabels;

class AxisLabelsAttached : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS

public:
    explicit AxisLabelsAttached(QObject *parent = nullptr);

    /*!
     * \qmlattachedproperty int AxisLabels::index
     */
    Q_PROPERTY(int index READ index NOTIFY indexChanged)
    int index() const;
    void setIndex(int newIndex);
    Q_SIGNAL void indexChanged();

    /*!
     * \qmlattachedproperty string AxisLabels::label
     */
    Q_PROPERTY(QString label READ label NOTIFY labelChanged)
    QString label() const;
    void setLabel(const QString &newLabel);
    Q_SIGNAL void labelChanged();

private:
    int m_index = -1;
    QString m_label;
};

/*!
 * \qmltype AxisLabels
 * \inqmlmodule org.kde.quickcharts.controls
 * \brief An item that uses a delegate to place axis labels on a chart.
 * \inherits Item
 */
class AxisLabels : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    QML_ATTACHED(AxisLabelsAttached)

public:
    /*!
     * \value HorizontalLeftRight
     * \value HorizontalRightLeft
     * \value VerticalTopBottom
     * \value VerticalBottomTop
     */
    enum class Direction { HorizontalLeftRight, HorizontalRightLeft, VerticalTopBottom, VerticalBottomTop };
    Q_ENUM(Direction)

    explicit AxisLabels(QQuickItem *parent = nullptr);
    ~AxisLabels() override;

    /*!
     * \qmlproperty enumeration AxisLabels::direction
     * \qmlenumeratorsfrom AxisLabels::Direction
     */
    Q_PROPERTY(AxisLabels::Direction direction READ direction WRITE setDirection NOTIFY directionChanged)
    AxisLabels::Direction direction() const;
    Q_SLOT void setDirection(AxisLabels::Direction newDirection);
    Q_SIGNAL void directionChanged();

    /*!
     * \qmlproperty Component AxisLabels::delegate
     */
    Q_PROPERTY(QQmlComponent *delegate READ delegate WRITE setDelegate NOTIFY delegateChanged)
    QQmlComponent *delegate() const;
    Q_SLOT void setDelegate(QQmlComponent *newDelegate);
    Q_SIGNAL void delegateChanged();

    /*!
     * \qmlproperty ChartDataSource AxisLabels::source
     */
    Q_PROPERTY(ChartDataSource *source READ source WRITE setSource NOTIFY sourceChanged)
    ChartDataSource *source() const;
    Q_SLOT void setSource(ChartDataSource *newSource);
    Q_SIGNAL void sourceChanged();

    /*!
     * \qmlproperty Qt.Alignment AxisLabels::alignment
     */
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment NOTIFY alignmentChanged)
    Qt::Alignment alignment() const;
    Q_SLOT void setAlignment(Qt::Alignment newAlignment);
    Q_SIGNAL void alignmentChanged();

    /*!
     * \qmlproperty bool AxisLabels::constrainToBounds
     */
    Q_PROPERTY(bool constrainToBounds READ constrainToBounds WRITE setConstrainToBounds NOTIFY constrainToBoundsChanged)
    bool constrainToBounds() const;
    Q_SLOT void setConstrainToBounds(bool newConstrainToBounds);
    Q_SIGNAL void constrainToBoundsChanged();

    static AxisLabelsAttached *qmlAttachedProperties(QObject *object)
    {
        return new AxisLabelsAttached(object);
    }

protected:
    void updatePolish() override;
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    bool isHorizontal();
    void updateLabels();
    void onBeginCreate(int index, QQuickItem *item);

    Direction m_direction = Direction::HorizontalLeftRight;
    ChartDataSource *m_source = nullptr;
    Qt::Alignment m_alignment = Qt::AlignHCenter | Qt::AlignVCenter;
    bool m_constrainToBounds = true;

    std::unique_ptr<ItemBuilder> m_itemBuilder;
    bool m_layoutScheduled = false;
};

#endif // AXISLABELS_H
