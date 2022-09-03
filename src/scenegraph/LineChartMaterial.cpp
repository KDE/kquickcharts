/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QSGMaterialShader *LineChartMaterial::createShader() const
#else
QSGMaterialShader *LineChartMaterial::createShader(QSGRendererInterface::RenderMode) const
#endif
{
    return new LineChartShader();
}

int LineChartMaterial::compare(const QSGMaterial *other) const
{
    auto material = static_cast<const LineChartMaterial *>(other);

    /* clang-format off */
    if (qFuzzyCompare(material->aspect, aspect)
        && qFuzzyCompare(material->lineWidth, lineWidth)
        && qFuzzyCompare(material->smoothing, smoothing)) { /* clang-format on */
        return 0;
    }

    return QSGMaterial::compare(other);
}

LineChartShader::LineChartShader()
{
    setShaders(QStringLiteral("linechart.vert"), QStringLiteral("linechart.frag"));
}

LineChartShader::~LineChartShader()
{
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
const char *const *LineChartShader::attributeNames() const
{
    /* clang-format off */
    static char const *const names[] = {
        "in_vertex",
        "in_uv",
        "in_lineColor",
        "in_fillColor",
        "in_bounds",
        "in_count",
        "in_points_0",
        "in_points_1",
        "in_points_2",
        "in_points_3",
        "in_points_4",
        "in_points_5",
        "in_points_6",
        "in_points_7",
        "in_points_8",
        nullptr
    }; /* clang-format on */
    return names;
}

void LineChartShader::initialize()
{
    QSGMaterialShader::initialize();
    m_matrixLocation = program()->uniformLocation("matrix");
    m_opacityLocation = program()->uniformLocation("opacity");
    m_lineWidthLocation = program()->uniformLocation("lineWidth");
    m_aspectLocation = program()->uniformLocation("aspect");
    m_smoothingLocation = program()->uniformLocation("smoothing");
}

void LineChartShader::updateState(const QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    if (state.isMatrixDirty()) {
        program()->setUniformValue(m_matrixLocation, state.combinedMatrix());
    }
    if (state.isOpacityDirty()) {
        program()->setUniformValue(m_opacityLocation, state.opacity());
    }

    if (!oldMaterial || newMaterial->compare(oldMaterial) != 0) {
        LineChartMaterial *material = static_cast<LineChartMaterial *>(newMaterial);
        program()->setUniformValue(m_lineWidthLocation, material->lineWidth);
        program()->setUniformValue(m_aspectLocation, material->aspect);
        program()->setUniformValue(m_smoothingLocation, material->smoothing);
    }
}
#else
bool LineChartShader::updateUniformData(QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    bool changed = false;
    QByteArray *buf = state.uniformData();
    Q_ASSERT(buf->size() >= 80);

    if (state.isMatrixDirty()) {
        const QMatrix4x4 m = state.combinedMatrix();
        memcpy(buf->data(), m.constData(), 64);
        changed = true;
    }

    if (state.isOpacityDirty()) {
        const float opacity = state.opacity();
        memcpy(buf->data() + 72, &opacity, 4);
        changed = true;
    }

    if (!oldMaterial || newMaterial->compare(oldMaterial) != 0) {
        const auto material = static_cast<LineChartMaterial *>(newMaterial);
        memcpy(buf->data() + 64, &material->lineWidth, 4);
        memcpy(buf->data() + 68, &material->aspect, 4);
        memcpy(buf->data() + 76, &material->smoothing, 4);
        changed = true;
    }

    return changed;
}
#endif
