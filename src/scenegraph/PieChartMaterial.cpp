#include "PieChartMaterial_p.h"

PieChartMaterial::PieChartMaterial()
{
    setFlag(QSGMaterial::Blending);
}

PieChartMaterial::~PieChartMaterial()
{
}

QSGMaterialType *PieChartMaterial::type() const
{
    static QSGMaterialType type;
    return &type;
}

QSGMaterialShader *PieChartMaterial::createShader() const
{
    return new PieChartShader();
}

QVector2D PieChartMaterial::aspectRatio() const
{
    return m_aspectRatio;
}

float PieChartMaterial::innerDimension() const
{
    return m_innerDimension;
}

QVector<QVector2D> PieChartMaterial::triangles() const
{
    return m_triangles;
}

QVector<QVector4D> PieChartMaterial::colors() const
{
    return m_colors;
}

void PieChartMaterial::setAspectRatio(const QVector2D &aspect)
{
    m_aspectRatio = aspect;
}

void PieChartMaterial::setInnerDimension(float dimension)
{
    m_innerDimension = dimension;
}

void PieChartMaterial::setTriangles(const QVector<QVector2D> &triangles)
{
    m_triangles = triangles;
}

void PieChartMaterial::setColors(const QVector<QVector4D> &colors)
{
    m_colors = colors;
}

PieChartShader::PieChartShader()
{
    setShaderSourceFile(QOpenGLShader::Vertex, QStringLiteral(":/org.kde.quickcharts/uv.vert"));
    setShaderSourceFile(QOpenGLShader::Fragment, QStringLiteral(":/org.kde.quickcharts/piechart.frag"));
}

PieChartShader::~PieChartShader()
{
}

const char *const *PieChartShader::attributeNames() const
{
    static char const *const names[] = { "in_vertex", "in_uv", nullptr };
    return names;
}

void PieChartShader::initialize()
{
    QSGMaterialShader::initialize();
    m_matrixLocation = program()->uniformLocation("matrix");
    m_opacityLocation = program()->uniformLocation("opacity");
    m_innerDimensionLocation = program()->uniformLocation("innerDimension");
    m_aspectLocation = program()->uniformLocation("aspect");
    m_trianglesLocation = program()->uniformLocation("triangles");
    m_colorsLocation = program()->uniformLocation("colors");
    m_triangleCountLocation = program()->uniformLocation("triangleCount");
}

void PieChartShader::updateState(const QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    if (state.isMatrixDirty())
        program()->setUniformValue(m_matrixLocation, state.combinedMatrix());
    if (state.isOpacityDirty())
        program()->setUniformValue(m_opacityLocation, state.opacity());

    if (!oldMaterial || newMaterial->compare(oldMaterial) != 0) {
        PieChartMaterial *material = static_cast<PieChartMaterial *>(newMaterial);
        program()->setUniformValue(m_innerDimensionLocation, material->innerDimension());
        program()->setUniformValue(m_aspectLocation, material->aspectRatio());
        program()->setUniformValueArray(m_trianglesLocation, material->triangles().constData(), material->triangles().size());
        program()->setUniformValueArray(m_colorsLocation, material->colors().constData(), material->colors().size());
        program()->setUniformValue(m_triangleCountLocation, material->triangles().size());
    }
}
