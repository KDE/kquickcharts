/*
 * SPDX-FileCopyrightText: 2020 Arjen Hiemstra <ahiemstra@heimr.nl>
 * 
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef LEGENDLAYOUT_H
#define LEGENDLAYOUT_H

#include <QQuickItem>

class LegendLayoutAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal minimumWidth READ minimumWidth WRITE setMinimumWidth NOTIFY minimumWidthChanged)
    Q_PROPERTY(qreal preferredWidth READ preferredWidth WRITE setPreferredWidth NOTIFY preferredWidthChanged)
    Q_PROPERTY(qreal maximumWidth READ maximumWidth WRITE setMaximumWidth NOTIFY maximumWidthChanged)

public:
    explicit LegendLayoutAttached(QObject *parent = nullptr);

    qreal minimumWidth() const;
    void setMinimumWidth(qreal newMinimumWidth);
    Q_SIGNAL void minimumWidthChanged();

    qreal preferredWidth() const;
    void setPreferredWidth(qreal newPreferredWidth);
    Q_SIGNAL void preferredWidthChanged();

    qreal maximumWidth() const;
    void setMaximumWidth(qreal newMaximumWidth);
    Q_SIGNAL void maximumWidthChanged();

private:
    qreal m_minimumWidth = -1.0;
    qreal m_preferredWidth = -1.0;
    qreal m_maximumWidth = -1.0;
};

class LegendLayout : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal horizontalSpacing READ horizontalSpacing WRITE setHorizontalSpacing NOTIFY horizontalSpacingChanged)
    Q_PROPERTY(qreal verticalSpacing READ verticalSpacing WRITE setVerticalSpacing NOTIFY verticalSpacingChanged)
    Q_PROPERTY(qreal preferredWidth READ preferredWidth NOTIFY preferredWidthChanged)

public:
    explicit LegendLayout(QQuickItem *parent = nullptr);

    qreal horizontalSpacing() const;
    void setHorizontalSpacing(qreal newHorizontalSpacing);
    Q_SIGNAL void horizontalSpacingChanged();

    qreal verticalSpacing() const;
    void setVerticalSpacing(qreal newVerticalSpacing);
    Q_SIGNAL void verticalSpacingChanged();

    qreal preferredWidth() const;
    Q_SIGNAL void preferredWidthChanged();

    static LegendLayoutAttached *qmlAttachedProperties(QObject *object)
    {
        return new LegendLayoutAttached(object);
    }

protected:
    void componentComplete() override;
    void updatePolish() override;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
#else
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
#endif
    void itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData &data) override;

private:
    void updateItem(int index, QQuickItem *item);
    std::tuple<int, int, qreal, qreal> determineColumns();

    qreal m_horizontalSpacing = 0.0;
    qreal m_verticalSpacing = 0.0;
    qreal m_preferredWidth = 0.0;

    bool m_completed = false;
};

QML_DECLARE_TYPEINFO(LegendLayout, QML_HAS_ATTACHED_PROPERTIES)

#endif // LEGENDLAYOUT_H
