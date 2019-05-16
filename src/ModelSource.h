#ifndef MODELSOURCE_H
#define MODELSOURCE_H

#include "DataSource.h"

class QAbstractItemModel;

/**
 * ModelSource allows reading data from a QAbstractItemModel.
 *
 *
 */
class ModelSource : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(int role READ role WRITE setRole NOTIFY roleChanged)
    Q_PROPERTY(QString roleName READ roleName WRITE setRoleName NOTIFY roleNameChanged)
    Q_PROPERTY(int column READ column WRITE setColumn NOTIFY columnChanged)
    Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged)

public:
    ModelSource(QObject *parent = nullptr);
    ~ModelSource();

    int role() const;
    QString roleName() const;
    int column() const;
    QAbstractItemModel *model() const;

    int itemCount() const override;
    QVariant item(int index) const override;

public Q_SLOTS:
    void setRole(int role);
    void setRoleName(const QString &name);
    void setColumn(int column);
    void setModel(QAbstractItemModel *model);

Q_SIGNALS:
    void roleChanged();
    void roleNameChanged();
    void columnChanged();
    void modelChanged();

private:
    mutable int m_role = -1;
    QString m_roleName;
    int m_column = 0;
    QAbstractItemModel *m_model = nullptr;
};

#endif // MODELSOURCE_H
