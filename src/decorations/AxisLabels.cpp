#include "AxisLabels.h"

#include <QDebug>

#include "datasource/ChartDataSource.h"
#include <QQmlContext>

AxisLabelsAttached::AxisLabelsAttached(QObject* parent)
    : QObject(parent)
{
}

int AxisLabelsAttached::index() const
{
    return m_index;
}

void AxisLabelsAttached::setIndex(int newIndex)
{
    if (newIndex == m_index) {
        return;
    }

    m_index = newIndex;
    Q_EMIT indexChanged();
}

QString AxisLabelsAttached::label() const
{
    return m_label;
}

void AxisLabelsAttached::setLabel(const QString & newLabel)
{
    if (newLabel == m_label) {
        return;
    }

    m_label = newLabel;
    Q_EMIT labelChanged();
}



class AxisLabels::Private
{
public:
    Private(AxisLabels *qq) : q(qq) { }

    inline bool isHorizontal() { return direction == Direction::HorizontalLeftRight || direction == Direction::HorizontalRightLeft; }

    void update();
    void layout();

    AxisLabels *const q;

    Direction direction = Direction::HorizontalLeftRight;
    QQmlComponent *delegate = nullptr;
    ChartDataSource *source = nullptr;
    Qt::Alignment alignment = Qt::AlignHCenter | Qt::AlignVCenter;
    bool constrainToBounds = true;

    QVector<QQuickItem*> labels;
    bool layoutScheduled = false;
};

AxisLabels::AxisLabels(QQuickItem* parent)
    : QQuickItem(parent), d(new Private(this))
{
}

AxisLabels::~AxisLabels()
{
}

AxisLabels::Direction AxisLabels::direction() const
{
    return d->direction;
}

void AxisLabels::setDirection(AxisLabels::Direction newDirection)
{
    if (newDirection == d->direction) {
        return;
    }

    d->direction = newDirection;
    scheduleLayout();
    Q_EMIT directionChanged();
}


QQmlComponent *AxisLabels::delegate() const
{
    return d->delegate;
}

void AxisLabels::setDelegate(QQmlComponent *newDelegate)
{
    if (newDelegate == d->delegate) {
        return;
    }

    d->delegate = newDelegate;
    d->update();
    Q_EMIT delegateChanged();
}

ChartDataSource* AxisLabels::source() const
{
    return d->source;
}

void AxisLabels::setSource(ChartDataSource* newSource)
{
    if (newSource == d->source) {
        return;
    }

    if (d->source) {
        d->source->disconnect(this);
    }

    d->source = newSource;

    if (d->source) {
        connect(d->source, &ChartDataSource::dataChanged, this, [this]() { d->update(); });
    }

    d->update();
    Q_EMIT sourceChanged();
}

Qt::Alignment AxisLabels::alignment() const
{
    return d->alignment;
}

void AxisLabels::setAlignment(Qt::Alignment newAlignment)
{
    if (newAlignment == d->alignment) {
        return;
    }

    d->alignment = newAlignment;
    scheduleLayout();
    Q_EMIT alignmentChanged();
}

void AxisLabels::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);

    if (newGeometry != oldGeometry) {
        scheduleLayout();
    }
}

void AxisLabels::scheduleLayout()
{
    if (!d->layoutScheduled) {
        QMetaObject::invokeMethod(this, [this]() { d->layout(); d->layoutScheduled = false; }, Qt::QueuedConnection);
        d->layoutScheduled = true;
    }
}

void AxisLabels::Private::update()
{
    qDeleteAll(labels);
    labels.clear();

    if (!delegate || !source) {
        return;
    }

    for (int i = 0; i < source->itemCount(); ++i) {
        auto label = source->item(i).toString();

        auto context = new QQmlContext(qmlContext(q));
        auto item = qobject_cast<QQuickItem *>(delegate->beginCreate(context));
        if (!item) {
            qWarning() << "Failed to create label instance for label" << label;
            continue;
        }

        QObject::connect(item, &QQuickItem::xChanged, q, [this]() { q->scheduleLayout(); });
        QObject::connect(item, &QQuickItem::yChanged, q, [this]() { q->scheduleLayout(); });
        QObject::connect(item, &QQuickItem::widthChanged, q, [this]() { q->scheduleLayout(); });
        QObject::connect(item, &QQuickItem::heightChanged, q, [this]() { q->scheduleLayout(); });

        context->setParent(item);
        item->setParentItem(q);

        auto attached = static_cast<AxisLabelsAttached*>(qmlAttachedPropertiesObject<AxisLabels>(item, true));
        attached->setIndex(i);
        attached->setLabel(label);

        delegate->completeCreate();
        labels << item;
    }

    q->scheduleLayout();
}

void AxisLabels::Private::layout()
{
    auto maxWidth = 0.0;
    auto totalWidth = 0.0;
    auto maxHeight = 0.0;
    auto totalHeight = 0.0;

    for (auto label : qAsConst(labels)) {
        maxWidth = std::max(maxWidth, label->width());
        maxHeight = std::max(maxHeight, label->height());
        totalWidth += label->width();
        totalHeight += label->height();
    }

    auto impWidth = isHorizontal() ? totalWidth : maxWidth;
    auto impHeight = isHorizontal() ? maxHeight : totalHeight;

    if (qFuzzyCompare(impWidth, q->width()) && qFuzzyCompare(impHeight, q->height())) {
        return;
    }

    q->setImplicitWidth(impWidth);
    q->setImplicitHeight(impHeight);

    auto spacing = (isHorizontal() ? q->width() : q->height()) / (labels.size() - 1);
    auto i = 0;
    auto layoutWidth = isHorizontal() ? 0.0 : q->width();
    auto layoutHeight = isHorizontal() ? q->height() : 0.0;

    for (auto label : qAsConst(labels)) {
        auto x = 0.0;
        auto y = 0.0;

        switch(direction) {
            case Direction::HorizontalLeftRight:
                x = i * spacing;
                break;
            case Direction::HorizontalRightLeft:
                x = q->width() - i * spacing;
                break;
            case Direction::VerticalTopBottom:
                y = i * spacing;
                break;
            case Direction::VerticalBottomTop:
                y = q->height() - i * spacing;
                break;
        }

        if (alignment & Qt::AlignHCenter) {
            x += (layoutWidth - label->width()) / 2;
        } else if (alignment & Qt::AlignRight) {
            x += layoutWidth - label->width();
        }

        if (alignment & Qt::AlignVCenter) {
            y += (layoutHeight - label->height()) / 2;
        } else if (alignment & Qt::AlignBottom) {
            y += layoutHeight - label->height();
        }

        if (constrainToBounds) {
            x = std::max(x, 0.0);
            x = x + label->width() > q->width() ? q->width() - label->width() : x;
            y = std::max(y, 0.0);
            y = y + label->height() > q->height() ? q->height() - label->height() : y;
        }

        label->setX(x);
        label->setY(y);
        i++;
    }
}
