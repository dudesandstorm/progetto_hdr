#include "myhdrshaderclass.h"
#include "common.h"

void MyHDRShaderClass::set_exposure(float exposure) {
    // Passa il valore di esposizione allo shader
    glUniform1f(_exposure_location, exposure);
}

bool MyHDRShaderClass::load_shaders() {
    // Caricamento degli shader specifici per l'HDR
    return add_shader(GL_VERTEX_SHADER, "hdr.vert") &&
           add_shader(GL_FRAGMENT_SHADER, "hdr.frag");
}

bool MyHDRShaderClass::load_done() {
    // Recupero delle location delle variabili uniform definite in hdr.frag
    _hdr_buffer_location = get_uniform_location("hdrBuffer");
    _exposure_location   = get_uniform_location("exposure");

    // Imposto il sampler per utilizzare il tunnel texture 0 (o quello configurato nell'app)
    // Di solito, per il post-processing, si usa TEXTURE_COLOR (0)
    glUniform1i(_hdr_buffer_location, 0);

    // Verifica che le uniform siano state trovate correttamente
    // Nota: Se 'exposure' non è usata attivamente nel calcolo del .frag, 
    // i driver potrebbero ottimizzarla via restituendo -1.
    return (_hdr_buffer_location != INVALID_UNIFORM_LOCATION) &&
           (_exposure_location != INVALID_UNIFORM_LOCATION);
}