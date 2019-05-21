#ifndef PIECHARTMATERIAL_H
#define PIECHARTMATERIAL_H

#include <QSGMaterial>
#include <QSGMaterialShader>

class PieChartMaterial : public QSGMaterial
{
public:
    PieChartMaterial();
    ~PieChartMaterial();

    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader() const override;

    QVector2D aspectRatio() const;
    float innerDimension() const;

    QVector<QVector2D> triangles() const;
    QVector<QVector4D> colors() const;

    void setAspectRatio(const QVector2D &ratio);
    void setInnerDimension(float dimension);
    void setTriangles(const QVector<QVector2D> &triangles);
    void setColors(const QVector<QVector4D> &colors);

private:
    QVector2D m_aspectRatio;
    float m_innerDimension = 0.0f;
    QVector<QVector2D> m_triangles;
    QVector<QVector4D> m_colors;
};

class PieChartShader : public QSGMaterialShader
{
public:
    PieChartShader();
    ~PieChartShader();

    char const *const *attributeNames() const override;

    void initialize() override;
    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;

private:
    int m_matrixLocation = 0;
    int m_opacityLocation = 0;
    int m_innerDimensionLocation = 0;
    int m_aspectLocation = 0;
    int m_colorsLocation = 0;
    int m_trianglesLocation = 0;
    int m_triangleCountLocation = 0;
};

#endif // PIECHARTMATERIAL_H
