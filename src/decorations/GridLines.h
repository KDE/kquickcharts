/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef GRIDLINES_H
#define GRIDLINES_H

#include <memory>

#include <QQuickItem>

class GridLines;
class LineGridNode;
class XYChart;

class LinePropertiesGroup : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY propertiesChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY propertiesChanged)
    Q_PROPERTY(float lineWidth READ lineWidth WRITE setLineWidth NOTIFY propertiesChanged)
    Q_PROPERTY(int frequency READ frequency WRITE setFrequency NOTIFY propertiesChanged)
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY propertiesChanged)

public:
    explicit LinePropertiesGroup(GridLines *parent);

    bool visible() const;
    void setVisible(bool newVisible);

    QColor color() const;
    void setColor(const QColor &newColor);

    float lineWidth() const;
    void setLineWidth(float newLineWidth);

    int frequency() const;
    void setFrequency(int newFrequency);

    int count() const;
    void setCount(int newCount);

    Q_SIGNAL void propertiesChanged();

private:
    GridLines *m_parent = nullptr;
    bool m_visible = true;
    QColor m_color = Qt::black;
    float m_lineWidth = 1.0;
    int m_frequency = 2;
    int m_count = -1;
};

/**
 * An item that renders a set of lines to make a grid for a chart.
 */
class GridLines : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(GridLines::Direction direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(XYChart *chart READ chart WRITE setChart NOTIFY chartChanged)
    Q_PROPERTY(float spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    Q_PROPERTY(LinePropertiesGroup *major READ majorGroup CONSTANT)
    Q_PROPERTY(LinePropertiesGroup *minor READ minorGroup CONSTANT)

public:
    enum class Direction { Horizontal, Vertical };
    Q_ENUM(Direction)
    /**
     * Default constructor
     */
    explicit GridLines(QQuickItem *parent = nullptr);

    Direction direction() const;
    void setDirection(GridLines::Direction newDirection);
    Q_SIGNAL void directionChanged();

    XYChart *chart() const;
    void setChart(XYChart *newChart);
    Q_SIGNAL void chartChanged();

    float spacing() const;
    void setSpacing(float newSpacing);
    Q_SIGNAL void spacingChanged();

    LinePropertiesGroup *majorGroup() const;
    LinePropertiesGroup *minorGroup() const;

private:
    QSGNode *updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *) override;
    void updateLines(LineGridNode *node, LinePropertiesGroup *properties);

    GridLines::Direction m_direction = Direction::Horizontal;
    XYChart *m_chart = nullptr;
    float m_spacing = 10.0;

    std::unique_ptr<LinePropertiesGroup> m_major;
    std::unique_ptr<LinePropertiesGroup> m_minor;
};

#endif // GRIDLINES_H
