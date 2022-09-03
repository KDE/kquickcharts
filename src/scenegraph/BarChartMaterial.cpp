/*
 * SPDX-FileCopyrightText: 2021 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "BarChartMaterial.h"

BarChartMaterial::BarChartMaterial()
{
    setFlag(QSGMaterial::Blending);
}

BarChartMaterial::~BarChartMaterial()
{
}

QSGMaterialType *BarChartMaterial::type() const
{
    static QSGMaterialType type;
    return &type;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QSGMaterialShader *BarChartMaterial::createShader() const
#else
QSGMaterialShader *BarChartMaterial::createShader(QSGRendererInterface::RenderMode) const
#endif
{
    return new BarChartShader();
}

int BarChartMaterial::compare(const QSGMaterial *other) const
{
    auto material = static_cast<const BarChartMaterial *>(other);

    /* clang-format off */
    if (material->aspect == aspect
        && qFuzzyCompare(material->radius, radius)
        && material->backgroundColor == backgroundColor) { /* clang-format on */
        return 0;
    }

    return QSGMaterial::compare(other);
}

BarChartShader::BarChartShader()
{
    setShaders(QStringLiteral("barchart.vert"), QStringLiteral("barchart.frag"));
}

BarChartShader::~BarChartShader()
{
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
const char *const *BarChartShader::attributeNames() const
{
    static const char *const names[] = {"in_vertex", "in_uv", "in_color", "in_value", nullptr};
    return names;
}

void BarChartShader::initialize()
{
    QSGMaterialShader::initialize();
    m_matrixLocation = program()->uniformLocation("matrix");
    m_opacityLocation = program()->uniformLocation("opacity");
    m_aspectLocation = program()->uniformLocation("aspect");
    m_backgroundColorLocation = program()->uniformLocation("backgroundColor");
    m_radiusLocation = program()->uniformLocation("radius");
}

void BarChartShader::updateState(const QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    if (state.isMatrixDirty()) {
        program()->setUniformValue(m_matrixLocation, state.combinedMatrix());
    }
    if (state.isOpacityDirty()) {
        program()->setUniformValue(m_opacityLocation, state.opacity());
    }

    if (!oldMaterial || newMaterial->compare(oldMaterial) != 0) {
        BarChartMaterial *material = static_cast<BarChartMaterial *>(newMaterial);
        program()->setUniformValue(m_aspectLocation, material->aspect);
        program()->setUniformValue(m_backgroundColorLocation, material->backgroundColor);
        program()->setUniformValue(m_radiusLocation, material->radius);
    }
}
#else
bool BarChartShader::updateUniformData(QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    bool changed = false;
    QByteArray *buf = state.uniformData();
    Q_ASSERT(buf->size() >= 96);

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
        const auto material = static_cast<BarChartMaterial *>(newMaterial);
        memcpy(buf->data() + 64, &material->aspect, 8);
        memcpy(buf->data() + 76, &material->radius, 4);
        float c[4];
        material->backgroundColor.getRgbF(&c[0], &c[1], &c[2], &c[3]);
        memcpy(buf->data() + 80, c, 16);
        changed = true;
    }

    return changed;
}
#endif
