#ifndef SDF_SHADER_H
#define SDF_SHADER_H

#include <QSGMaterialShader>

class SDFShader : public QSGMaterialShader
{
public:
    SDFShader();
    virtual ~SDFShader();

    void setShaders(const QString &vertex, const QString &fragment);
};

#endif // SDF_SHADER_H
