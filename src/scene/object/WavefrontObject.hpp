#ifndef WAVEFRONTOBJECT_HPP
#define WAVEFRONTOBJECT_HPP

#include <scene/Scene.hpp>
#include <resource/StaticModel.hpp>

#include <QString>

class WavefrontObject : public AObject
{
public:
    WavefrontObject( Scene * scene, QString filename, float scale = 1.0f );
    virtual ~WavefrontObject();

    virtual void updateSelf( const double & delta );
    virtual void drawSelf();

private:
    StaticModel * mModel;
    Scene * mScene;
    QString mFilename;
    float mScale;
};

#endif // WAVEFRONTOBJECT_HPP
