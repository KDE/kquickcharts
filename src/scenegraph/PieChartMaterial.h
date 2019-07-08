#ifndef PIECHARTMATERIAL_H
#define PIECHARTMATERIAL_H

#include <QSGMaterial>
#include <QSGMaterialShader>
#include <QColor>

#include "SDFShader.h"

class PieChartMaterial : public QSGMaterial
{
public:
    PieChartMaterial();
    ~PieChartMaterial();

    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader() const override;

    QVector2D aspectRatio() const;
    float innerRadius() const;
    float outerRadius() const;
    QColor backgroundColor() const;

    QVector<QVector2D> triangles() const;
    QVector<QVector4D> colors() const;
    QVector<int> segments() const;

    void setAspectRatio(const QVector2D &aspect);
    void setInnerRadius(float radius);
    void setOuterRadius(float radius);
    void setBackgroundColor(const QColor &color);

    void setTriangles(const QVector<QVector2D> &triangles);
    void setColors(const QVector<QVector4D> &colors);
    void setSegments(const QVector<int> &segments);

private:
    QVector2D m_aspectRatio;
    float m_innerRadius = 0.0f;
    float m_outerRadius = 0.0f;
    QColor m_backgroundColor;

    QVector<QVector2D> m_triangles;
    QVector<QVector4D> m_colors;
    QVector<int> m_segments;
};

class PieChartShader : public SDFShader
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
    int m_innerRadiusLocation = 0;
    int m_outerRadiusLocation = 0;
    int m_aspectLocation = 0;
    int m_backgroundColorLocation = 0;
    int m_trianglesLocation = 0;
    int m_colorsLocation = 0;
    int m_segmentsLocation = 0;
    int m_segmentCountLocation = 0;
};

#endif // PIECHARTMATERIAL_H
