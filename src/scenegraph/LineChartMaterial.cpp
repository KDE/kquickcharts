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

QSGMaterialShader *LineChartMaterial::createShader(QSGRendererInterface::RenderMode) const
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
