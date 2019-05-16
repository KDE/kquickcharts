#ifndef MODELROLE_H
#define MODELROLE_H

#include "DataSource.h"

class QAbstractItemModel;

/**
 * @todo write docs
 */
class ModelRole : public DataSource
{
    Q_OBJECT
    Q_PROPERTY ( QString name READ name WRITE setName NOTIFY nameChanged )
    Q_PROPERTY (QAbstractItemModel* model READ model WRITE setModel NOTIFY modelChanged)

public:
    ModelRole(QObject* parent = nullptr);
    ~ModelRole();

    QString name() const;
    QAbstractItemModel* model() const;

    int itemCount() const override;
    QVariant item(int index) const override;

public Q_SLOTS:
    void setName(const QString& name );
    void setModel(QAbstractItemModel* model);

Q_SIGNALS:
    void nameChanged();
    void modelChanged();

private:
    QAbstractItemModel* m_model = nullptr;
    QString m_name;
};

#endif // MODELROLE_H
