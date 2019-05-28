#ifndef MODELSOURCE_H
#define MODELSOURCE_H

#include "ChartDataSource.h"

class QAbstractItemModel;

/**
 * ModelSource allows reading data from a QAbstractItemModel.
 *
 *
 */
class ModelSource : public ChartDataSource
{
    Q_OBJECT
    Q_PROPERTY(int role READ role WRITE setRole NOTIFY roleChanged)
    Q_PROPERTY(QString roleName READ roleName WRITE setRoleName NOTIFY roleNameChanged)
    Q_PROPERTY(int column READ column WRITE setColumn NOTIFY columnChanged)
    Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(bool indexColumns READ indexColumns WRITE setIndexColumns NOTIFY indexColumnsChanged)

public:
    ModelSource(QObject *parent = nullptr);
    ~ModelSource();

    int role() const;
    QString roleName() const;
    int column() const;
    QAbstractItemModel *model() const;
    bool indexColumns() const;

    int itemCount() const override;
    QVariant item(int index) const override;
    QVariant minimum() const override;
    QVariant maximum() const override;

public Q_SLOTS:
    void setRole(int role);
    void setRoleName(const QString &name);
    void setColumn(int column);
    void setModel(QAbstractItemModel *model);
    void setIndexColumns(bool index);

Q_SIGNALS:
    void roleChanged();
    void roleNameChanged();
    void columnChanged();
    void modelChanged();
    void indexColumnsChanged();

private:
    mutable int m_role = -1;
    QString m_roleName;
    int m_column = 0;
    bool m_indexColumns = false;
    QAbstractItemModel *m_model = nullptr;
};

#endif // MODELSOURCE_H
