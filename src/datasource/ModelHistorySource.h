#ifndef MODELHISTORYSOURCE_H
#define MODELHISTORYSOURCE_H

#include "ModelSource.h"

class ModelHistorySource : public ModelSource
{
    Q_OBJECT
    Q_PROPERTY(int row READ row WRITE setRow NOTIFY rowChanged)
    Q_PROPERTY(int maximumHistory READ maximumHistory WRITE setMaximumHistory NOTIFY maximumHistoryChanged)

public:
    ModelHistorySource(QObject* parent = nullptr);
    ~ModelHistorySource();

    virtual int itemCount() const override;
    virtual QVariant item(int index) const override;
    virtual QVariant minimum() const override;
    virtual QVariant maximum() const override;

    int row() const;
    int maximumHistory() const;

public Q_SLOTS:
    void setRow(int row);
    void setMaximumHistory(int maximumHistory);

Q_SIGNALS:
    void rowChanged();
    void maximumHistoryChanged();

private:
    void onModelChanged();
    void onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);

    int m_row;
    int m_maximumHistory;
    QVariantList m_history;
};

#endif // MODELHISTORYSOURCE_H
