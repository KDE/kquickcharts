#ifndef AXISGRID_H
#define AXISGRID_H

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
    LinePropertiesGroup( GridLines *parent);

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
 * @todo write docs
 */
class GridLines : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(GridLines::Direction direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(XYChart * chart READ chart WRITE setChart NOTIFY chartChanged)
    Q_PROPERTY(float spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    Q_PROPERTY(LinePropertiesGroup *major READ major CONSTANT)
    Q_PROPERTY(LinePropertiesGroup *minor READ minor CONSTANT)

public:
    enum class Direction {
        Horizontal,
        Vertical
    };
    Q_ENUM(Direction)
    /**
     * Default constructor
     */
    explicit GridLines (QQuickItem *parent = nullptr);

    /**
     * Destructor
     */
    ~GridLines();

    Direction direction() const;
    Q_SLOT void setDirection(GridLines::Direction newDirection);
    Q_SIGNAL void directionChanged();

    XYChart * chart() const;
    Q_SLOT void setChart(XYChart * newChart);
    Q_SIGNAL void chartChanged();

    float spacing() const;
    Q_SLOT void setSpacing(float newSpacing);
    Q_SIGNAL void spacingChanged();

    LinePropertiesGroup *major() const;
    LinePropertiesGroup *minor() const;

private:
    QSGNode *updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *) override;
    void updateLines(LineGridNode *node, LinePropertiesGroup *properties);

    class Private;
    const std::unique_ptr<Private> d;
};

#endif // AXISGRID_H
