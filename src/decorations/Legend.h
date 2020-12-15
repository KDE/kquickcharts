/*
 * SPDX-FileCopyrightText: 2020 Arjen Hiemstra <ahiemstra@heimr.nl>
 * 
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef LEGEND_H
#define LEGEND_H

#include <QQuickItem>

class Chart;
class ItemBuilder;

class LegendAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ index CONSTANT)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString shortName READ shortName NOTIFY shortNameChanged)
    Q_PROPERTY(QVariant value READ value NOTIFY valueChanged)
    Q_PROPERTY(QColor color READ color NOTIFY colorChanged)

    Q_PROPERTY(qreal minimumWidth READ minimumWidth WRITE setMinimumWidth NOTIFY minimumWidthChanged)
    Q_PROPERTY(qreal maximumWidth READ maximumWidth WRITE setMaximumWidth NOTIFY maximumWidthChanged)

public:
    explicit LegendAttached(QObject *parent = nullptr);

    int index() const;
    void setIndex(int newIndex);
    Q_SIGNAL void indexChanged();

    QString name() const;
    void setName(const QString &newName);
    Q_SIGNAL void nameChanged();

    QString shortName() const;
    void setShortName(const QString &newShortName);
    Q_SIGNAL void shortNameChanged();

    QVariant value() const;
    void setValue(const QVariant &newValue);
    Q_SIGNAL void valueChanged();

    QColor color() const;
    void setColor(const QColor &newColor);
    Q_SIGNAL void colorChanged();

    qreal minimumWidth() const;
    void setMinimumWidth(qreal newMinimumWidth);
    Q_SIGNAL void minimumWidthChanged();

    qreal maximumWidth() const;
    void setMaximumWidth(qreal newMaximumWidth);
    Q_SIGNAL void maximumWidthChanged();

private:
    int m_index = -1;
    QString m_name;
    QString m_shortName;
    QVariant m_value;
    QColor m_color;

    qreal m_minimumWidth = -1.0;
    qreal m_maximumWidth = -1.0;
};

class Legend : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(Chart *chart READ chart WRITE setChart NOTIFY chartChanged)
    Q_PROPERTY(QQmlComponent *delegate READ delegate WRITE setDelegate NOTIFY delegateChanged)

    Q_PROPERTY(qreal horizontalSpacing READ horizontalSpacing WRITE setHorizontalSpacing NOTIFY horizontalSpacingChanged)
    Q_PROPERTY(qreal verticalSpacing READ verticalSpacing WRITE setVerticalSpacing NOTIFY verticalSpacingChanged)

public:
    explicit Legend(QQuickItem *parent = nullptr);
    ~Legend() override;

    Chart *chart() const;
    void setChart(Chart *newChart);
    Q_SIGNAL void chartChanged();

    QQmlComponent *delegate() const;
    void setDelegate(QQmlComponent *newDelegate);
    Q_SIGNAL void delegateChanged();

    qreal horizontalSpacing() const;
    void setHorizontalSpacing(qreal newHorizontalSpacing);
    Q_SIGNAL void horizontalSpacingChanged();

    qreal verticalSpacing() const;
    void setVerticalSpacing(qreal newVerticalSpacing);
    Q_SIGNAL void verticalSpacingChanged();

    static LegendAttached *qmlAttachedProperties(QObject *object)
    {
        return new LegendAttached(object);
    }

protected:
    void componentComplete() override;
    void updatePolish() override;
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData &data) override;

private:
    void updateItems();
    void updateItem(int index, QQuickItem *item);
    int itemCount();
    std::tuple<int, int, qreal, qreal> determineColumns();

    QPointer<Chart> m_chart;
    qreal m_horizontalSpacing = 0.0;
    qreal m_verticalSpacing = 0.0;

    bool m_completed = false;
    std::unique_ptr<ItemBuilder> m_itemBuilder;
};

QML_DECLARE_TYPEINFO(Legend, QML_HAS_ATTACHED_PROPERTIES)

#endif // LEGEND_H
