#include "ModelSource.h"

#include <QAbstractItemModel>

#include <QDebug>

ModelSource::ModelSource(QObject *parent)
    : ChartDataSource(parent)
{
    connect(this, &ModelSource::modelChanged, this, &ModelSource::dataChanged);
    connect(this, &ModelSource::columnChanged, this, &ModelSource::dataChanged);
    connect(this, &ModelSource::roleChanged, this, &ModelSource::dataChanged);
}

ModelSource::~ModelSource()
{
}

int ModelSource::role() const
{
    return m_role;
}

QString ModelSource::roleName() const
{
    return m_roleName;
}

int ModelSource::column() const
{
    return m_column;
}

QAbstractItemModel *ModelSource::model() const
{
    return m_model;
}

int ModelSource::itemCount() const
{
    return m_model ? m_model->rowCount() : 0;
}

QVariant ModelSource::item(int index) const
{
    if (!m_model)
        return QVariant {};

    // For certain model (QML ListModel for example), the roleNames() are more
    // dynamic and may only be valid when this method gets called. So try and
    // lookup the role first before anything else.
    if (m_role < 0 && !m_roleName.isEmpty())
        m_role = m_model->roleNames().key(m_roleName.toLatin1(), -1);

    if (m_role < 0) {
        qWarning() << "ModelSource: Invalid role " << m_role << m_roleName;
        return QVariant {};
    }

    if(m_column < 0 || m_column > m_model->columnCount()) {
        qWarning() << "ModelSource: Invalid column" << m_column;
        return QVariant {};
    }

    return m_model->data(m_model->index(index, m_column), m_role);
}

void ModelSource::setRole(int role)
{
    if(role == m_role)
        return;

    m_role = role;
    if(m_model) {
        m_roleName = QString::fromLatin1(m_model->roleNames().value(role));
        emit roleNameChanged();
    }
    emit roleChanged();
}

void ModelSource::setRoleName(const QString& name)
{
    if(name == m_roleName)
        return;

    m_roleName = name;
    if(m_model) {
        m_role = m_model->roleNames().key(m_roleName.toLatin1(), -1);
        emit roleChanged();
    }
    emit roleNameChanged();
}

void ModelSource::setColumn(int column)
{
    if(column == m_column)
        return;

    m_column = column;
    emit columnChanged();
}

void ModelSource::setModel(QAbstractItemModel *model)
{
    if (m_model == model) {
        return;
    }

    if (m_model)
        m_model->disconnect(this);

    m_model = model;
    if (m_model) {
        connect(m_model, &QAbstractItemModel::rowsInserted, this, &ModelSource::dataChanged);
        connect(m_model, &QAbstractItemModel::rowsRemoved, this, &ModelSource::dataChanged);
        connect(m_model, &QAbstractItemModel::rowsMoved, this, &ModelSource::dataChanged);
        connect(m_model, &QAbstractItemModel::modelReset, this, &ModelSource::dataChanged);
        connect(m_model, &QAbstractItemModel::dataChanged, this, &ModelSource::dataChanged);
        connect(m_model, &QAbstractItemModel::layoutChanged, this, &ModelSource::dataChanged);
    }

    emit modelChanged();
}
