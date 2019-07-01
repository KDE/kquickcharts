#ifndef AXISLABELS_H
#define AXISLABELS_H

#include <memory>

#include <Qt>
#include <QQuickItem>

class ChartDataSource;

class AxisLabels;

class AxisLabelsAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ index NOTIFY indexChanged)
    Q_PROPERTY(QString label READ label NOTIFY labelChanged)

public:
    explicit AxisLabelsAttached(QObject* parent = nullptr);

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
 * @todo write docs
 */
class AxisLabels : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(AxisLabels::Direction direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QQmlComponent *delegate READ delegate WRITE setDelegate NOTIFY delegateChanged)
    Q_PROPERTY(ChartDataSource *source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment NOTIFY alignmentChanged)

public:
    enum class Direction
    {
        HorizontalLeftRight,
        HorizontalRightLeft,
        VerticalTopBottom,
        VerticalBottomTop
    };
    Q_ENUM(Direction)
    /**
     * Default constructor
     */
    AxisLabels(QQuickItem *parent = nullptr);

    /**
     * Destructor
     */
    ~AxisLabels();

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

    static AxisLabelsAttached *qmlAttachedProperties(QObject *object)
    {
        return new AxisLabelsAttached(object);
    }

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    void scheduleLayout();

    class Private;
    const std::unique_ptr<Private> d;
};

QML_DECLARE_TYPEINFO(AxisLabels, QML_HAS_ATTACHED_PROPERTIES)

#endif // AXISLABELS_H
