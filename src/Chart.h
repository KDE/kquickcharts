/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef CHART_H
#define CHART_H

#include <QQuickItem>
#include <qqmlregistration.h>

#include "datasource/ChartDataSource.h"

#include "quickcharts_export.h"

/*!
 * \qmltype Chart
 * \inqmlmodule org.kde.quickcharts
 *
 * \brief Abstract base class for all charts.
 */
class QUICKCHARTS_EXPORT Chart : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Base class")

public:
    using DataSourcesProperty = QQmlListProperty<ChartDataSource>;

    /*!
     * \enum Chart::IndexingMode
     *
     * How to index color and name sources relative to the different value sources.
     *
     * \value IndexSourceValues
     *        Index each value, restart indexing for each value source.
     * \value IndexEachSource
     *        Index each value source, never index individual values.
     * \value IndexAllValues
     *        Index each value, continuing with the index for each value source.
     */
    enum IndexingMode {
        IndexSourceValues = 1,
        IndexEachSource,
        IndexAllValues
    };
    Q_ENUM(IndexingMode)

    explicit Chart(QQuickItem *parent = nullptr);
    ~Chart() override = default;

    /*!
     * \qmlproperty ChartDataSource Chart::nameSource
     * \brief The data source to use for names of chart items.
     */
    Q_PROPERTY(ChartDataSource *nameSource READ nameSource WRITE setNameSource NOTIFY nameSourceChanged)
    ChartDataSource *nameSource() const;
    void setNameSource(ChartDataSource *nameSource);
    Q_SIGNAL void nameSourceChanged();

    /*!
     * \qmlproperty ChartDataSource Chart::shortNameSource
     * \brief The data source to use for short names of chart items.
     */
    Q_PROPERTY(ChartDataSource *shortNameSource READ shortNameSource WRITE setShortNameSource NOTIFY shortNameSourceChanged)
    ChartDataSource *shortNameSource() const;
    void setShortNameSource(ChartDataSource *shortNameSource);
    Q_SIGNAL void shortNameSourceChanged();

    /*!
     * \qmlproperty ChartDataSource Chart::colorSource
     * \brief The data source to use for colors of chart items.
     */
    Q_PROPERTY(ChartDataSource *colorSource READ colorSource WRITE setColorSource NOTIFY colorSourceChanged)
    ChartDataSource *colorSource() const;
    void setColorSource(ChartDataSource *colorSource);
    Q_SIGNAL void colorSourceChanged();

    /*!
     * \qmlproperty list<ChartDataSource> Chart::valueSources
     * \brief The data sources providing the data this chart needs to render.
     */
    Q_PROPERTY(QQmlListProperty<ChartDataSource> valueSources READ valueSourcesProperty NOTIFY valueSourcesChanged)
    DataSourcesProperty valueSourcesProperty();
    QList<ChartDataSource *> valueSources() const;
    Q_SIGNAL void valueSourcesChanged();
    Q_INVOKABLE void insertValueSource(int index, ChartDataSource *source);
    Q_INVOKABLE void removeValueSource(int index);
    Q_INVOKABLE void removeValueSource(QObject *source);

    /*!
     * \qmlproperty enumeration Chart::indexingMode
     * \qmlenumeratorsfrom Chart::IndexingMode
     * \brief The indexing mode used for indexing colors and names.
     *
     * The default value is \c{Chart.IndexEachSource}.
     */
    Q_PROPERTY(IndexingMode indexingMode READ indexingMode WRITE setIndexingMode NOTIFY indexingModeChanged)
    IndexingMode indexingMode() const;
    void setIndexingMode(IndexingMode newIndexingMode);
    Q_SIGNAL void indexingModeChanged();

    /*!
     * \qmlproperty int Chart::highlight
     * \brief The index of a value source to highlight.
     *
     * Highlighting is dependant on Chart type, but will generally mean that
     * other value sources are rendered with lower opacity.
     *
     * By default, this is -1 which means nothing is highlighted.
     */
    Q_PROPERTY(int highlight READ highlight WRITE setHighlight NOTIFY highlightChanged RESET resetHighlight)
    int highlight() const;
    void setHighlight(int highlight);
    void resetHighlight();
    Q_SIGNAL void highlightChanged();

    Q_SIGNAL void dataChanged();

protected:
    /*!
     * \brief Called when the data of a value source changes.
     *
     * This method should be reimplemented by subclasses. It is called whenever
     * the data of one of the value sources changes. Subclasses should use this
     * to make sure that they update whatever internal state they use for
     * rendering, then call update() to schedule rendering the item.
     */
    virtual void onDataChanged() = 0;

    void componentComplete() override;

    /*!
     * \brief Desaturate and de-emphasise a color.
     *
     * Mainly intended as a standard for ensuring everything but the highlighted
     * item is desaturated.
     */
    QColor desaturate(const QColor &input);

private:
    static void appendSource(DataSourcesProperty *list, ChartDataSource *source);
    static qsizetype sourceCount(DataSourcesProperty *list);
    static ChartDataSource *source(DataSourcesProperty *list, qsizetype index);
    static void clearSources(DataSourcesProperty *list);
    static void replaceSource(DataSourcesProperty *list, qsizetype index, ChartDataSource *source);
    static void removeLastSource(DataSourcesProperty *list);

    ChartDataSource *m_nameSource = nullptr;
    ChartDataSource *m_shortNameSource = nullptr;
    ChartDataSource *m_colorSource = nullptr;
    QList<ChartDataSource *> m_valueSources;
    IndexingMode m_indexingMode = IndexEachSource;
    int m_highlight = -1;
};

#endif // CHART_H
