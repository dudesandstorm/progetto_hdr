#ifndef MYHDRSHADERCLASS_H
#define MYHDRSHADERCLASS_H

#include "shaderclass.h"

/**
    Classe specifica per la gestione dello shader HDR.
    Gestisce il tone mapping e la correzione gamma per il rendering post-processing.
*/
class MyHDRShaderClass : public ShaderClass {
public:
    /**
        Setta il valore di esposizione per il tone mapping
        @param exposure valore di esposizione
    */
    void set_exposure(float exposure);

private:
    /**
        Carica i file hdr.vert e hdr.frag
    */
    virtual bool load_shaders() override;

    /**
        Recupera le location per hdrBuffer e exposure
    */
    virtual bool load_done() override;

    GLint _hdr_buffer_location;    ///<< Location del sampler2D hdrBuffer
    GLint _exposure_location;      ///<< Location della variabile exposure
};

#endif