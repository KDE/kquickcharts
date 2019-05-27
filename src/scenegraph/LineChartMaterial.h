#ifndef LINECHARTMATERIAL_H
#define LINECHARTMATERIAL_H

#include <QSGMaterial>
#include <QSGMaterialShader>
#include <QColor>

class LineChartMaterial : public QSGMaterial
{
public:
    LineChartMaterial();
    ~LineChartMaterial();

    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader() const override;

    QColor lineColor() const;
    QColor fillColor() const;
    float lineWidth() const;
    QVector<QVector2D> points() const;

    void setLineColor(const QColor &color);
    void setFillColor(const QColor &color);
    void setLineWidth(float width);
    void setPoints(const QVector<QVector2D> &points);

private:
    QColor m_lineColor;
    QColor m_fillColor;
    float m_lineWidth = 0.0;
    QVector<QVector2D> m_points;
};

class LineChartShader : public QSGMaterialShader
{
public:
    LineChartShader();
    ~LineChartShader();

    char const *const *attributeNames() const override;

    void initialize() override;
    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;

private:
    int m_matrixLocation = 0;
    int m_opacityLocation = 0;
    int m_lineColorLocation = 0;
    int m_fillColorLocation = 0;
    int m_lineWidthLocation = 0;
    int m_pointsLocation = 0;
    int m_pointCountLocation = 0;
};

#endif // LINECHARTMATERIAL_H
