#ifndef LIGHT_H
#define LIGHT_H

#include "glm/glm.hpp"

/**
    Classe di supporto per gestire le informazioni di luce ambientale
*/
class AmbientLight {
    glm::vec3 _color; ///<< Colore della luce
    float _intensity; ///<< Intensità della luce

public:
    /**
        Setta la luce al colore bianco e massima intensità
    */
    AmbientLight();
    /**
        Setta la luce al colore e intensità dati
        @param col colore dela luce
        @param i intensità della luce
    */
    AmbientLight(const glm::vec3 &col, float i);

    /**
        Incrementa l'intensità della luce della quantità data
        @param value valore di incremento
    */
    void inc(float value);

    /**
        Decrementa l'intensità della luce della quantità data
        @param value valore di decremento
    */
    void dec(float value);

    /**
        Ritorna il colore della luce
    */
    glm::vec3 color() const;

    /**
        Ritorna l'intensità della luce
    */
    float intensity() const;
};

class DirectionalLight {
    glm::vec3 _color; ///<< Colore della luce
    glm::vec3 _direction; ///<< Direzione della sorgente
public:
    /**
        Setta la luce al colore bianco.
        La direzione di default è un vettore verticale verso il basso.
    */
    DirectionalLight();

    /**
        Setta la luce al colore e intensità dati
        @param col colore dela luce
        @param i intensità della luce
    */

    DirectionalLight(const glm::vec3 &col, const glm::vec3 &dir);

    /**
        Ritorna il colore della luce
    */
    glm::vec3 color() const;

    /**
        Ritorna la direzione della luce
    */
    glm::vec3 direction() const;

};

class DiffusiveLight {
    float _intensity; /// Intensità della luce

public:
    /**
        Setta la luce al colore bianco e massima intensità e posizionata in alto
        La direzione di irraggiamento di default è un vettore verticale verso il basso.
    */
    DiffusiveLight();

    /**
        Setta la luce al colore e intensità dati
        @param col colore dela luce
        @param i intensità della luce
    */
    DiffusiveLight(float i);

    /**
        Incrementa l'intensità della luce della quantità data
        @param value valore di incremento
    */
    void inc(float value);

    /**
        Decrementa l'intensità della luce della quantità data
        @param value valore di decremento
    */
    void dec(float value);

    /**
        Ritorna l'intensità della luce
    */
    float intensity() const;
};


class SpecularLight {
  float     _shininess; ///<< esponente di shininess
  float     _intensity; ///<< Intensità della luce

public:

  /**
    Setta la luce alla massima intensità. La posizione della camera è quella di 
    default. La shininess è settata a 30.
  */
  SpecularLight();

  /**
    Setta la luce speculare 
    @param col colore dela luce
    @param i intensità della luce
  */
  SpecularLight(float i,float s);

  /**
    Incrementa l'intensità della luce della quantità data
    @param value valore di incremento
  */
  void inc(float value);

  /**
    Decrementa l'intensità della luce della quantità data
    @param value valore di decremento
  */
  void dec(float value);

  /**
    Incrementa l'intensità della luce della quantità data
    @param value valore di incremento
  */
  void inc_shine(float value);

  /**
    Decrementa l'intensità della luce della quantità data
      @param value valore di decremento
  */
  void dec_shine(float value);

  /**
      Ritorna l'intensità della luce
  */
  float intensity() const;

  /**
      Ritorna il valore di shininess
  */
  float shininess() const;

};

class PointLight {
    glm::vec3 _color; ///<< Colore della luce
    glm::vec3 _position; ///<< Posizione della sorgente
    float _intensity; ///<< Intensità della luce
    float _range; ///<< Raggio massimo della luce
public:
    /**
        Setta la luce al colore bianco.
        La posizione di default è (0,0,0)).
    */
    PointLight();

    /**
        Setta la luce al colore e intensità dati
        @param col colore dela luce
        @param pos posizione della luce
        @param i intensità della luce
        @param r raggio della luce
    */

    PointLight(const glm::vec3 &col, const glm::vec3 &pos, float i, float r);

    /**
    Incrementa l'intensità della luce della quantità data
      @param value valore di incremento
    */
    void mod_int(float value);

    /**
    Incrementa il raggio della luce della quantità data
      @param value valore di incremento
    */
    void mod_range(float value);

    /**
    Imposta il colore della luce al colore dato
      @param col colore della luce
    */
    void set_col(glm::vec3 col);

    /**
    Sposta la luce alla posizione data
      @param pos valore di incremento
    */
    void set_pos(glm::vec3 pos);

    /**
        Ritorna il colore della luce
    */
    glm::vec3 color() const;

    /**
        Ritorna la posizione della luce
    */
    glm::vec3 position() const;

    /**
        Ritorna l'intensità della luce
    */
    float intensity() const;

    /**
        Ritorna il raggio della luce
    */
    float range() const;

};

#endif