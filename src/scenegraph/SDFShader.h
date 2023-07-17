/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef SDF_SHADER_H
#define SDF_SHADER_H

#include <QSGMaterialShader>

struct UniformDataStream {
    inline UniformDataStream(QSGMaterialShader::RenderState &state) noexcept
        : bytes(state.uniformData()->data())
    {
    }

    ~UniformDataStream()
    {
    }

    template<typename Data>
    friend inline UniformDataStream &operator<<(UniformDataStream &stream, const Data &data)
    {
        stream.applyPadding<Data>();
        memcpy(stream.bytes, &data, sizeof(Data));
        stream.bytes += sizeof(Data);
        stream.updatePadding<Data>();
        return stream;
    }

    template<typename Data>
    inline void skip(const Data &data = {})
    {
        applyPadding<Data>();
        Q_UNUSED(data);
        bytes += sizeof(Data);
        updatePadding<Data>();
    }

    inline void skipComponents(uint count)
    {
        if (padding < 16) {
            bytes += padding;
            padding = 16;
        }
        bytes += (4 * count);
    }

    friend inline UniformDataStream &operator<<(UniformDataStream &stream, const QMatrix4x4 &m)
    {
        stream.applyPadding<QMatrix4x4>();
        memcpy(stream.bytes, m.constData(), 4 * 4 * 4);
        stream.bytes += 4 * 4 * 4;
        return stream;
    }

    template<typename T>
    friend inline UniformDataStream &operator<<(UniformDataStream &stream, const QVector<T> &v)
    {
        for (const auto &item : v) {
            stream.applyPadding<T>();
            memcpy(stream.bytes, &item, sizeof(T));
            stream.bytes += sizeof(T);
        }
        return stream;
    }

    char *bytes;
    size_t padding = 16;

private:
    template<typename Data>
    inline void applyPadding()
    {
        if (padding < sizeof(Data) && padding < 16) {
            bytes += padding;
            padding = 16;
        }
    }
    template<typename Data>
    inline void updatePadding()
    {
        if constexpr (sizeof(Data) % 16 != 0)
            padding -= sizeof(Data) % 16;
    }
};

class SDFShader : public QSGMaterialShader
{
public:
    SDFShader();
    virtual ~SDFShader();

    void setShaders(const QString &vertex, const QString &fragment);
};

#endif // SDF_SHADER_H
