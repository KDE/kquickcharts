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

class ChartDataSource;
class ItemBuilder;

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
    ~AxisLabels() override;

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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
#else
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
#endif

private:
    void scheduleLayout();
    bool isHorizontal();
    void updateLabels();
    void layout();
    void onBeginCreate(int index, QQuickItem* item);

    Direction m_direction = Direction::HorizontalLeftRight;
    ChartDataSource *m_source = nullptr;
    Qt::Alignment m_alignment = Qt::AlignHCenter | Qt::AlignVCenter;
    bool m_constrainToBounds = true;

    std::unique_ptr<ItemBuilder> m_itemBuilder;
    bool m_layoutScheduled = false;
};

QML_DECLARE_TYPEINFO(AxisLabels, QML_HAS_ATTACHED_PROPERTIES)

#endif // AXISLABELS_H
