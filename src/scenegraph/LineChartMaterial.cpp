#include "LineChartMaterial.h"

LineChartMaterial::LineChartMaterial()
{
    setFlag(QSGMaterial::Blending);
}

LineChartMaterial::~LineChartMaterial()
{
}

QSGMaterialType *LineChartMaterial::type() const
{
    static QSGMaterialType type;
    return &type;
}

QSGMaterialShader *LineChartMaterial::createShader() const
{
    return new LineChartShader();
}

QColor LineChartMaterial::lineColor() const
{
    return m_lineColor;
}

QColor LineChartMaterial::fillColor() const
{
    return m_fillColor;
}

float LineChartMaterial::lineWidth() const
{
    return m_lineWidth;
}

QVector<QVector2D> LineChartMaterial::points() const
{
    return m_points;
}

void LineChartMaterial::setLineColor(const QColor& color)
{
    m_lineColor = color;
}

void LineChartMaterial::setFillColor(const QColor& color)
{
    m_fillColor = color;
}

void LineChartMaterial::setLineWidth(float width)
{
    m_lineWidth = width;
}

void LineChartMaterial::setPoints(const QVector<QVector2D>& points)
{
    m_points = points;
}

LineChartShader::LineChartShader()
{
    setShaderSourceFile(QOpenGLShader::Vertex, QStringLiteral(":/org.kde.quickcharts/uv.vert"));
    setShaderSourceFile(QOpenGLShader::Fragment, QStringLiteral(":/org.kde.quickcharts/linechart.frag"));
}

LineChartShader::~LineChartShader()
{
}

const char *const *LineChartShader::attributeNames() const
{
    static char const *const names[] = { "in_vertex", "in_uv", "\0" };
    return names;
}

void LineChartShader::initialize()
{
    QSGMaterialShader::initialize();
    m_matrixLocation = program()->uniformLocation("matrix");
    m_opacityLocation = program()->uniformLocation("opacity");
    m_lineColorLocation = program()->uniformLocation("lineColor");
    m_fillColorLocation = program()->uniformLocation("fillColor");
    m_lineWidthLocation = program()->uniformLocation("lineWidth");
    m_pointsLocation = program()->uniformLocation("points");
    m_pointCountLocation = program()->uniformLocation("pointCount");
}

void LineChartShader::updateState(const QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    if (state.isMatrixDirty())
        program()->setUniformValue(m_matrixLocation, state.combinedMatrix());
    if (state.isOpacityDirty())
        program()->setUniformValue(m_opacityLocation, state.opacity());

    if (!oldMaterial || newMaterial->compare(oldMaterial) != 0) {
        LineChartMaterial *material = static_cast<LineChartMaterial *>(newMaterial);
        program()->setUniformValue(m_lineColorLocation, material->lineColor());
        program()->setUniformValue(m_fillColorLocation, material->fillColor());
        program()->setUniformValue(m_lineWidthLocation, material->lineWidth());
        program()->setUniformValue(m_pointCountLocation, material->points().size());
        program()->setUniformValueArray(m_pointsLocation, material->points().constData(), material->points().size());
    }
}
