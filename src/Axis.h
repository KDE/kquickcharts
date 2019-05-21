#ifndef AXIS_H
#define AXIS_H

#include <QObject>

class RangeGroup;
class ChartDataSource;

/**
 * @todo write docs
 */
class Axis : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(RangeGroup* range READ range CONSTANT)
    Q_PROPERTY(qreal stepSize READ stepSize WRITE setStepSize NOTIFY stepSizeChanged)
    Q_PROPERTY(bool drawLines READ drawLines WRITE setDrawLines NOTIFY drawLinesChanged)
    Q_PROPERTY(ChartDataSource* labelSource READ labelSource WRITE setLabelSource NOTIFY labelSourceChanged)

public:
    /**
     * Constructor
     */
    Axis(QObject* parent = nullptr);
    /**
     * Destructor
     */
    ~Axis();

    QString title() const;
    RangeGroup* range() const;
    qreal stepSize() const;
    bool drawLines() const;
    ChartDataSource* labelSource() const;

public Q_SLOTS:
    void setTitle(const QString& title);
    void setStepSize(qreal stepSize);
    void setDrawLines(bool drawLines);
    void setLabelSource(ChartDataSource *labelSource);

Q_SIGNALS:
    void titleChanged();
    void stepSizeChanged();
    void drawLinesChanged();
    void labelSourceChanged();

private:
    QString m_title;
    RangeGroup* m_range;
    qreal m_stepSize;
    bool m_drawLines;
    ChartDataSource* m_labelSource;
};

#endif // AXIS_H
