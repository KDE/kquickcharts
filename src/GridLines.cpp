#include "GridLines.h"

#include "scenegraph/LineGridNode.h"

class GridLines::Private
{
public:
    GridLines::Direction direction = Direction::Horizontal;
    float spacing = 10.0;

    std::unique_ptr<LinePropertiesGroup> major;
    std::unique_ptr<LinePropertiesGroup> minor;
};

LinePropertiesGroup::LinePropertiesGroup(GridLines* parent)
    : QObject()
{
    m_parent = parent;
}

bool LinePropertiesGroup::visible() const
{
    return m_visible;
}

void LinePropertiesGroup::setVisible(bool newVisible)
{
    if (newVisible == m_visible) {
        return;
    }

    m_visible = newVisible;
    Q_EMIT propertiesChanged();
}


QColor LinePropertiesGroup::color() const
{
    return m_color;
}

void LinePropertiesGroup::setColor(const QColor & newColor)
{
    if (newColor == m_color) {
        return;
    }

    m_color = newColor;
    Q_EMIT propertiesChanged();
}

float LinePropertiesGroup::lineWidth() const
{
    return m_lineWidth;
}

void LinePropertiesGroup::setLineWidth(float newLineWidth)
{
    if (newLineWidth == m_lineWidth) {
        return;
    }

    m_lineWidth = newLineWidth;
    Q_EMIT propertiesChanged();
}

int LinePropertiesGroup::frequency() const
{
    return m_frequency;
}

void LinePropertiesGroup::setFrequency(int newFrequency)
{
    if (newFrequency == m_frequency) {
        return;
    }

    m_frequency = newFrequency;
    Q_EMIT propertiesChanged();
}

GridLines::GridLines(QQuickItem* parent)
    : QQuickItem(parent), d(new Private)
{
    setFlag(QQuickItem::ItemHasContents);

    d->major = std::make_unique<LinePropertiesGroup>(this);
    connect(d->major.get(), &LinePropertiesGroup::propertiesChanged, this, &GridLines::update);
    d->minor = std::make_unique<LinePropertiesGroup>(this);
    connect(d->minor.get(), &LinePropertiesGroup::propertiesChanged, this, &GridLines::update);
}

GridLines::~GridLines()
{
}

GridLines::Direction GridLines::direction() const
{
    return d->direction;
}

void GridLines::setDirection(GridLines::Direction newDirection)
{
    if (newDirection == d->direction) {
        return;
    }

    d->direction = newDirection;
    update();
    Q_EMIT directionChanged();
}

float GridLines::spacing() const
{
    return d->spacing;
}

void GridLines::setSpacing(float newSpacing)
{
    if (newSpacing == d->spacing) {
        return;
    }

    d->spacing = newSpacing;
    update();
    Q_EMIT spacingChanged();
}

LinePropertiesGroup * GridLines::major() const
{
    return d->major.get();
}

LinePropertiesGroup * GridLines::minor() const
{
    return d->minor.get();
}

QSGNode * GridLines::updatePaintNode(QSGNode* node, QQuickItem::UpdatePaintNodeData*)
{
    if (!node) {
        node = new QSGNode{};
        node->appendChildNode(new LineGridNode{});
        node->appendChildNode(new LineGridNode{});
    }

    updateMinorLines(static_cast<LineGridNode*>(node->childAtIndex(0)));
    updateMajorLines(static_cast<LineGridNode*>(node->childAtIndex(1)));

    return node;
}

void GridLines::updateMajorLines(LineGridNode *node)
{
    node->setVisible(d->major->visible());
    node->setRect(boundingRect());
    node->setVertical(d->direction == Direction::Vertical);
    node->setColor(d->major->color());
    node->setLineWidth(d->major->lineWidth());
    node->setSpacing(d->spacing * d->major->frequency());
    node->update();
}

void GridLines::updateMinorLines(LineGridNode *node)
{
    node->setVisible(d->minor->visible());
    node->setRect(boundingRect());
    node->setVertical(d->direction == Direction::Vertical);
    node->setColor(d->minor->color());
    node->setLineWidth(d->minor->lineWidth());
    node->setSpacing(d->spacing * d->minor->frequency());
    node->update();
}
