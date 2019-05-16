#include "PieChart.h"

#include <QDebug>
#include <QAbstractItemModel>

#include "PieChartNode_p.h"

class PieChart::Private
{
public:
    QAbstractItemModel* model = nullptr;
    QString dataRole;
    QString colorRole;
    qreal innerDiameter = 0.0;

    QVector<qreal> sections;
    QVector<QColor> colors;
};

PieChart::PieChart(QQuickItem* parent)
    : QQuickItem(parent), d(new Private)
{
    setFlag(QQuickItem::ItemHasContents, true);
}

PieChart::~PieChart()
{
}

QAbstractItemModel* PieChart::model() const
{
    return d->model;
}

QString PieChart::dataRole() const
{
    return d->dataRole;
}

QString PieChart::colorRole() const
{
    return d->colorRole;
}

qreal PieChart::innerDiameter() const
{
    return d->innerDiameter;
}

void PieChart::setModel(QAbstractItemModel* model)
{
    if (d->model == model) {
        return;
    }

    if(d->model)
        d->model->disconnect(this);

    d->model = model;
    if(model) {
        QObject::connect(model, &QAbstractItemModel::rowsInserted, this, &PieChart::updateData);
        QObject::connect(model, &QAbstractItemModel::rowsRemoved, this, &PieChart::updateData);
        QObject::connect(model, &QAbstractItemModel::rowsMoved, this, &PieChart::updateData);
        QObject::connect(model, &QAbstractItemModel::modelReset, this, &PieChart::updateData);
        QObject::connect(model, &QAbstractItemModel::dataChanged, this, &PieChart::updateData);
        QObject::connect(model, &QAbstractItemModel::layoutChanged, this, &PieChart::updateData);
    }

    updateData();
    emit modelChanged();
}

void PieChart::setDataRole(const QString& dataRole)
{
    if (d->dataRole == dataRole) {
        return;
    }

    d->dataRole = dataRole;
    updateData();
    emit dataRoleChanged();
}

void PieChart::setColorRole(const QString& colorRole)
{
    if (d->colorRole == colorRole)
        return;

    d->colorRole = colorRole;
    updateData();
    emit colorRoleChanged();
}

void PieChart::setInnerDiameter(qreal diameter)
{
    if(qFuzzyCompare(d->innerDiameter, diameter)) {
        return;
    }

    d->innerDiameter = diameter;
    update();
    emit innerDiameterChanged();
}

QSGNode *PieChart::updatePaintNode(QSGNode *node, UpdatePaintNodeData *data)
{
    Q_UNUSED(data);
    PieChartNode *n = static_cast<PieChartNode *>(node);
    if(!n) {
        n = new PieChartNode();
    }
    n->setRect(boundingRect());
    n->setInnerDiameter(d->innerDiameter);
    n->setSections(d->sections);
    n->setColors(d->colors);

    return n;
}

void PieChart::updateData()
{
    d->sections.clear();
    d->colors.clear();

    if(!d->model)
        return;

    auto dataRole = d->model->roleNames().key(d->dataRole.toUtf8(), -1);
    auto colorRole = d->model->roleNames().key(d->colorRole.toUtf8(), -1);
    if(dataRole == -1 || colorRole == -1) {
        qWarning() << "Model does not provide the" << d->dataRole << "or" << d->colorRole << "role";
        return;
    }

    qreal total = 0.0;
    QVector<qreal> data;
    for(int i = 0; i < d->model->rowCount(); ++i) {
        auto value = d->model->data(d->model->index(i, 0), dataRole).toReal();
        data << value;
        total += value;

        d->colors << d->model->data(d->model->index(i, 0), colorRole).value<QColor>();
    }

    for(auto value : data) {
        d->sections << (value / total);
    }

    update();
}
