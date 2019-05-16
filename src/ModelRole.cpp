#include "ModelRole.h"

#include <QAbstractItemModel>

ModelRole::ModelRole(QObject* parent)
    : DataSource(parent)
{
}

ModelRole::~ModelRole()
{
}

QAbstractItemModel * ModelRole::model() const
{
    return m_model;
}

QString ModelRole::name() const
{
    return m_name;
}

int ModelRole::itemCount() const
{
    return m_model ? m_model->rowCount() : 0;
}

QVariant ModelRole::item(int index) const
{
    if(!m_model)
        return QVariant{};

    auto role = m_model->roleNames().key(m_name.toUtf8(), -1);
    return m_model->data(m_model->index(index, 0), role);
}

void ModelRole::setModel(QAbstractItemModel* model)
{
    if (m_model == model) {
        return;
    }

    if(m_model)
        m_model->disconnect(this);

    m_model = model;
    if(m_model) {
        QObject::connect(m_model, &QAbstractItemModel::rowsInserted, this, &DataSource::dataChanged);
        QObject::connect(m_model, &QAbstractItemModel::rowsRemoved, this, &DataSource::dataChanged);
        QObject::connect(m_model, &QAbstractItemModel::rowsMoved, this, &DataSource::dataChanged);
        QObject::connect(m_model, &QAbstractItemModel::modelReset, this, &DataSource::dataChanged);
        QObject::connect(m_model, &QAbstractItemModel::dataChanged, this, &DataSource::dataChanged);
        QObject::connect(m_model, &QAbstractItemModel::layoutChanged, this, &DataSource::dataChanged);
    }

    emit modelChanged();
    emit dataChanged();
}

void ModelRole::setName ( const QString& name )
{
    if ( m_name == name ) {
        return;
    }

    m_name = name;
    emit nameChanged ();
}
