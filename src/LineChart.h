/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef LINECHART_H
#define LINECHART_H

#include <memory>

#include "XYChart.h"

class LineChartNode;

/**
 * An attached property that is exposed to point delegates created in line charts.
 *
 * \sa LineChart::pointDelegate
 */
class LineChartAttached : public QObject
{
    Q_OBJECT
    /**
     * The value at the current point.
     */
    Q_PROPERTY(QVariant value READ value NOTIFY valueChanged)
    /**
     * The color at the current point.
     */
    Q_PROPERTY(QColor color READ color NOTIFY colorChanged)
    /**
     * The name at the current point.
     */
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    /**
     * The short name at the current point.
     */
    Q_PROPERTY(QString shortName READ shortName NOTIFY shortNameChanged)

public:
    LineChartAttached(QObject *parent = nullptr);

    QVariant value() const;
    void setValue(const QVariant &value);
    Q_SIGNAL void valueChanged();

    QColor color() const;
    void setColor(const QColor &color);
    Q_SIGNAL void colorChanged();

    QString name() const;
    void setName(const QString &newName);
    Q_SIGNAL void nameChanged();

    QString shortName() const;
    void setShortName(const QString &newShortName);
    Q_SIGNAL void shortNameChanged();

private:
    QVariant m_value;
    QColor m_color;
    QString m_name;
    QString m_shortName;
};

/**
 * A line chart.
 *
 * ## Usage example
 *
 * \snippet snippets/linechart.qml example
 *
 * \image html linechart.png "The resulting line chart."
 */
class LineChart : public XYChart
{
    Q_OBJECT

    /**
     * Smooth the lines in the chart instead of making hard corners.
     */
    Q_PROPERTY(bool smooth READ smooth WRITE setSmooth NOTIFY smoothChanged)
    /**
     * The width of a line in the chart.
     */
    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    /**
     * The opacity of the area below a line.
     *
     * The default is 0.0. Note that if fillColorSource is set, this value is
     * ignored.
     */
    Q_PROPERTY(qreal fillOpacity READ fillOpacity WRITE setFillOpacity NOTIFY fillOpacityChanged)
    /**
     * A data source that supplies color values for the line charts' fill area.
     *
     * If this is not set (the default), the normal color source will be used,
     * with the fillOpacity used as its opacity.
     */
    Q_PROPERTY(ChartDataSource *fillColorSource READ fillColorSource WRITE setFillColorSource NOTIFY fillColorSourceChanged)
    /**
     * A delegate that will be placed at each line chart point.
     *
     * When this is not null, the specified component will be used to
     * instantiate an object for each point in the chart. These objects will
     * then be placed centered at positions corresponding to the points on the
     * chart. Each instance will have access to the attached properties of
     * LineChartAttached through LineChart attached object.
     *
     * \note The component assigned to this property is expected to create a
     *       QQuickItem, since the created object needs to be positioned.
     */
    Q_PROPERTY(QQmlComponent *pointDelegate READ pointDelegate WRITE setPointDelegate NOTIFY pointDelegateChanged)

public:
    explicit LineChart(QQuickItem *parent = nullptr);

    bool smooth() const;
    void setSmooth(bool smooth);
    Q_SIGNAL void smoothChanged();

    qreal lineWidth() const;
    void setLineWidth(qreal width);
    Q_SIGNAL void lineWidthChanged();

    qreal fillOpacity() const;
    void setFillOpacity(qreal opacity);
    Q_SIGNAL void fillOpacityChanged();

    ChartDataSource *fillColorSource() const;
    void setFillColorSource(ChartDataSource *newFillColorSource);
    Q_SIGNAL void fillColorSourceChanged();

    QQmlComponent *pointDelegate() const;
    void setPointDelegate(QQmlComponent *newPointDelegate);
    Q_SIGNAL void pointDelegateChanged();

    static LineChartAttached *qmlAttachedProperties(QObject *object)
    {
        return new LineChartAttached(object);
    }

protected:
    void updatePolish() override;
    QSGNode *updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data) override;
    void onDataChanged() override;
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    void updateLineNode(LineChartNode *node, const QColor &lineColor, const QColor &fillColor, ChartDataSource *valueSource);
    void createPointDelegates(const QVector<QVector2D> &values, int sourceIndex);
    void updatePointDelegate(QQuickItem *delegate, const QVector2D &position, const QVariant &value, int sourceIndex);

    bool m_smooth = false;
    qreal m_lineWidth = 1.0;
    qreal m_fillOpacity = 0.0;
    bool m_rangeInvalid = true;
    ChartDataSource *m_fillColorSource = nullptr;
    QHash<ChartDataSource *, QVector<QVector2D>> m_values;
    QQmlComponent *m_pointDelegate = nullptr;
    QHash<ChartDataSource *, QVector<QQuickItem *>> m_pointDelegates;
};

QML_DECLARE_TYPEINFO(LineChart, QML_HAS_ATTACHED_PROPERTIES)

#endif // LINECHART_H
