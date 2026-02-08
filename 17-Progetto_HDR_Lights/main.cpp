/**
  Questo codice mostra come sfruttare la libreria esterna assimp per impofrtare dei
  modelli 3D e renderizzarli nella scena.
  Al momento, il codice è in grado di renderizzare modelli costituiti da una
  singola mesh e con una singola Texture associata.

  I modelli 3D sono memorizzato nella cartella models (nella root di questo codice).
  Il formato dei file è Wavefront (https://en.wikipedia.org/wiki/Wavefront_.obj_file), 
  un semplice file di testo con la lista delle coordinate dei vertici, facce e 
  normali dei triangoli, e la/le texture associata/e.  

  E' stata creata una classe Mesh che gestisce il caricamento e il rendering dei
  modelli. Gli oggetti in scena sono incapsulati in classi che permettono
  l'inizializzazione del modello e il suo rendering.

  Per comodità, sono state istanziate delle variabili globali che
  contengono diversi modelli 3D:

  Modelli singoli:
  'teapot': Una teiera (visualizzabile premendo 't')
  'skull' : Una teschio (visualizzabile premendo 'k')
  'dragon': Una drago (visualizzabile premendo 'g')
  'boot'  : Uno scarpone (visualizzabile premendo 'b')
  'flower': Un fiore (visualizzabile premendo 'f')
  
  Modello composto
  'marius': Un volto (visualizzabile premendo 'm'). 
  Questo modello è composto da diverse mesh (6) ciascuna definita in un suo 
  file separato. Una volta caricate le singole mesh, queste sono renderizzate 
  di seguito all'interno dello stesso ciclo di rendering per avere il volto 
  completo. Inoltre, per questo modello, alcune texture hanno delle trasparenze
  Per poterle usare in modo corretto è necessario impostare in modo appropriato 
  OpenGL. Vedere la funzione marius.render().
*/


#include <iostream>
#include <sstream>
#include "GL/glew.h" // prima di freeglut
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include <glm/gtc/random.hpp>

#include "common.h"
#include "transform.h"
#include "camera.h"

#include "myshaderclass.h"
//NEWCODE
#include "myhdrshaderclass.h"
//ENDCODE
#include "teapot.h"
#include "skull.h"
#include "flower.h"
#include "boot.h"
#include "dragon.h"
#include "marius.h"

Teapot teapot;
Skull skull;
Flower flower;
Boot boot;
Dragon dragon;
Marius marius;

unsigned char MODEL_TO_RENDER = 'b';

GLint MODE = GL_FILL;

/**
  Struttura di comodo dove sono memorizzate tutte le variabili globali
*/
struct global_struct {

  int WINDOW_WIDTH  = 1024; // Larghezza della finestra 
  int WINDOW_HEIGHT = 768; // Altezza della finestra

  Camera camera;

  AmbientLight     ambient_light;
  DirectionalLight directional_light;
  DiffusiveLight   diffusive_light;
  SpecularLight    specular_light;

  PointLight       point_lights[MAX_POINT_LIGHTS];
  int num_point_lights = 0;
  int selected_point_light = 0;

  void next_point_light(){
    selected_point_light++;
    selected_point_light = selected_point_light % num_point_lights;
  }

  bool addPointLight(const PointLight& pl) {
    if (num_point_lights < MAX_POINT_LIGHTS) {
        point_lights[num_point_lights] = pl;
        num_point_lights++;
        return true;
    }
    return false; // Array is full
  }

  MyShaderClass myshaders;
  // NEWCODE
  MyHDRShaderClass hdrshaders;

  float exposure;
  // ENDCODE
  const float SPEED = 1;
  float gradX;
  float gradY; 

  global_struct() : gradX(0.0f), gradY(0.0f) {}

  // NEWCODE
  unsigned int hdrFBO;
  unsigned int colorBuffer;
  unsigned int rboDepth;
  // ENDCODE

} global;

/**
Prototipi della nostre funzioni di callback. 
Sono definite più avanti nel codice.
*/
void MyRenderScene(void);
void MyIdle(void);
void MyKeyboard(unsigned char key, int x, int y);
void MyClose(void);
void MySpecialKeyboard(int Key, int x, int y);
void MyMouse(int x, int y);

// NEWCODE
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad() {
    if (quadVAO == 0) {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
// ENDCODE

glm::vec3 randsphere(glm::vec3 center = glm::vec3(0.0f, 0.0f, -5.0f), float r = 5.0f)
{
  // Genera una direzione casuale normalizzata e moltiplicala per il raggio
  glm::vec3 random_point = glm::sphericalRand(r); 

  return center + random_point;
}

void init(int argc, char*argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);

  glutInitWindowSize(global.WINDOW_WIDTH, global.WINDOW_HEIGHT);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Informatica Grafica");

/*
  // Imposta la modalità a schemo intero e nasconde il cursore
  std::stringstream game_mode;
  game_mode << global.WINDOW_WIDTH << "x" << global.WINDOW_HEIGHT << ":32";
  glutGameModeString(game_mode.str().c_str());
  glutEnterGameMode();
*/
  glutSetCursor(GLUT_CURSOR_NONE);

  global.camera.set_mouse_init_position(global.WINDOW_WIDTH/2, global.WINDOW_HEIGHT/2);
  global.camera.lock_mouse_position(true);
  glutWarpPointer(global.WINDOW_WIDTH/2, global.WINDOW_HEIGHT/2);

 // Must be done after glut is initialized!
  GLenum res = glewInit();
  if (res != GLEW_OK) {
      std::cerr<<"Error : "<<glewGetErrorString(res)<<std::endl;
    exit(1);
  }

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glutDisplayFunc(MyRenderScene);

  glutKeyboardFunc(MyKeyboard);

  glutCloseFunc(MyClose);

  glutSpecialFunc(MySpecialKeyboard);

  glutPassiveMotionFunc(MyMouse);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_DEPTH_TEST);

}

void create_scene() {
  global.camera.set_camera(
          glm::vec3(0, 0, 0),
          glm::vec3(0, 0,-1),
          glm::vec3(0, 1, 0)
      );

  global.camera.set_perspective(
    30.0f,
    global.WINDOW_WIDTH,
    global.WINDOW_HEIGHT,
    0.1,
    100
  );

  global.ambient_light = AmbientLight(glm::vec3(1,1,1),1);
  global.directional_light = DirectionalLight(glm::vec3(1,1,1),glm::vec3(0,0,-1)); // 0.5
  global.diffusive_light = DiffusiveLight(0.5); // 0.5
  global.specular_light = SpecularLight(0.5,30);
  for (int i = 0; i < MAX_POINT_LIGHTS; i++)
  {
    global.addPointLight(PointLight(glm::vec3(1,1,1), randsphere(), 1, 100));
  }

  global.myshaders.init();
  global.myshaders.enable();

  // NEWCODE
  global.exposure = 0.2f;

  global.hdrshaders.init();
  global.hdrshaders.enable();

  glGenFramebuffers(1, &global.hdrFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, global.hdrFBO);

  glGenTextures(1, &global.colorBuffer);
  glBindTexture(GL_TEXTURE_2D, global.colorBuffer);
  // Use GL_RGBA16F to allow values > 1.0
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, global.WINDOW_WIDTH, global.WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Attach texture to framebuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, global.colorBuffer, 0);

  // Don't forget the depth buffer!
  glGenRenderbuffers(1, &global.rboDepth);
  glBindRenderbuffer(GL_RENDERBUFFER, global.rboDepth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, global.WINDOW_WIDTH, global.WINDOW_HEIGHT);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, global.rboDepth);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      std::cout << "Framebuffer not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // ENDCODE
}

void render_marius() {
  LocalTransform modelT;
  modelT.rotate(global.gradX, 180+global.gradY ,0.0f);
  modelT.translate(0,-1.7,-0.8);

  global.myshaders.set_model_transform(modelT.T());

  marius.render(); 
}

void render_teapot() {
  LocalTransform modelT;
  modelT.rotate(global.gradX, global.gradY ,0.0f);
  modelT.translate(0,-1.6,-10);

  global.myshaders.set_model_transform(modelT.T());

  teapot.render();  
}

void render_boot() {
  LocalTransform modelT;
  modelT.rotate(global.gradX, global.gradY ,0.0f);
  modelT.translate(0,-10,-70);

  global.myshaders.set_model_transform(modelT.T());

  boot.render();  
}

void render_flower() {
  LocalTransform modelT;
  modelT.rotate(-90+global.gradX, global.gradY ,0.0f);
  modelT.translate(0, -4,-15);

  global.myshaders.set_model_transform(modelT.T());

  flower.render();  
}

void render_dragon() {
  LocalTransform modelT;
  modelT.rotate(global.gradX, global.gradY ,0.0f);
  modelT.translate(0,0,-5);

  global.myshaders.set_model_transform(modelT.T());

  dragon.render();  
}

void render_skull() {
  LocalTransform modelT;
  modelT.rotate(global.gradX, global.gradY ,0.0f);
  modelT.translate(0,-5,-20);

  global.myshaders.set_model_transform(modelT.T());

  skull.render();  
}

void MyRenderScene() {
  // NEWCODE
  glBindFramebuffer(GL_FRAMEBUFFER, global.hdrFBO);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  global.myshaders.enable();
  // ENDCODE

  global.myshaders.set_camera_transform(global.camera.CP());
  global.myshaders.set_ambient_light(global.ambient_light);
  global.myshaders.set_directional_light(global.directional_light);
  global.myshaders.set_diffusive_light(global.diffusive_light);
  global.myshaders.set_specular_light(global.specular_light);
  for (int i = 0; i < global.num_point_lights; i++)
  {
    global.myshaders.set_point_light(i, global.point_lights[i]);
  }
  global.myshaders.set_num_point_lights(global.num_point_lights);
  global.myshaders.set_camera_position(global.camera.position());

  switch (MODEL_TO_RENDER) {
    case 't': render_teapot(); break;
    case 'b': render_boot(); break;
    case 'g': render_skull(); break;
    case 'k': render_dragon(); break;
    case 'm': render_marius(); break;
    case 'f': render_flower(); break;
  }

  // NEWCODE
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  global.hdrshaders.enable();
  global.hdrshaders.set_exposure(global.exposure);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, global.colorBuffer); // The texture attached to hdrFBO
    
  renderQuad();
  //ENDCODE

  glutSwapBuffers();
}

// Funzione globale che si occupa di gestire l'input da tastiera.
void MyKeyboard(unsigned char key, int x, int y) {
  switch ( key )
  {
    case 27: // Escape key
      glutDestroyWindow(glutGetWindow());
      return;
    break;

    case 'a':
      global.gradY -= global.SPEED;
    break;
    case 'd':
      global.gradY += global.SPEED;
    break;
    case 'w':
      global.gradX -= global.SPEED;
    break;
    case 's':
      global.gradX += global.SPEED;
    break;

    // Variamo l'intensità di luce ambientale
    case '1':
      global.ambient_light.dec(0.01);
    break;

    // Variamo l'intensità di luce ambientale
    case '2':
      global.ambient_light.inc(0.01);
    break;

    // Variamo l'intensità di luce diffusiva
    case '3':
      global.diffusive_light.dec(0.01);
    break;

    // Variamo l'intensità di luce diffusiva
    case '4':
      global.diffusive_light.inc(0.01);
    break;

    // Variamo l'intensità di luce speculare
    case '5':
      global.specular_light.dec(0.01);
    break;

    // Variamo l'intensità di luce speculare
    case '6':
      global.specular_light.inc(0.01);
    break;

    // Variamo l'esponente della luce speculare
    case '7':
      global.specular_light.dec_shine(1);
    break;

    // Variamo l'esponente della luce speculare
    case '8':
      global.specular_light.inc_shine(1);
    break;

    // Variamo l'intensità della luce puntiforme
    case 'u':
      global.point_lights[global.selected_point_light].mod_int(-1);
    break;

    // Variamo l'intensità della luce puntiforme
    case 'i':
      global.point_lights[global.selected_point_light].mod_int(1);
    break;

    // Variamo il raggio della luce puntiforme
    case 'o':
      global.point_lights[global.selected_point_light].mod_range(-1);
    break;

    // Variamo il raggio della luce puntiforme
    case 'p':
      global.point_lights[global.selected_point_light].mod_range(1);
    break;

    // Variamo il colore della luce puntiforme
    case 'j':
      global.point_lights[global.selected_point_light].set_col(glm::linearRand(glm::vec3(0.0f), glm::vec3(1.0f)));
    break;

    // Spostiamo la luce puntiforme a una posizione casuale
    case 'l':
      {
          float r = glm::linearRand(0.0f, 10.0f);
          glm::vec3 center = glm::vec3(0.0f, 0.0f, -5.0f);

          glm::vec3 rp = randsphere(center, r);

          // Trasla il punto rispetto al centro desiderato
          //REMOVEDglobal.point_lights[global.selected_point_light].set_pos(rp);
      }
    break;

    case ' ': // Reimpostiamo la camera
      global.camera.set_camera(
          glm::vec3(0, 0, 0),
          glm::vec3(0, 0,-1),
          glm::vec3(0, 1, 0)
      );
      global.gradX = 0;
      global.gradY = 0;

    break;

    case 't':
    case 'b':
    case 'g': 
    case 'k':
    case 'm':
    case 'f':
      MODEL_TO_RENDER = key;
    break;

    case 'z':
      if(MODE == GL_FILL) {
        MODE = GL_LINE;
        glDisable(GL_CULL_FACE);
      }
      else {
        MODE = GL_FILL;
        glEnable(GL_CULL_FACE);
      }
      glPolygonMode(GL_FRONT_AND_BACK,MODE);
    break;
  }

  glutPostRedisplay();
}

void MySpecialKeyboard(int Key, int x, int y) {
  global.camera.onSpecialKeyboard(Key);
  glutPostRedisplay();
}


void MyMouse(int x, int y) {
  if (global.camera.onMouse(x,y)) {
    // Risposto il mouse al centro della finestra
    glutWarpPointer(global.WINDOW_WIDTH/2, global.WINDOW_HEIGHT/2);
  }
  glutPostRedisplay();
}


// Funzione globale che si occupa di gestire la chiusura della finestra.
void MyClose(void) {
  std::cout << "Tearing down the system..." << std::endl;
  // Clean up here

  // A schermo intero dobbiamo uccidere l'applicazione.
  exit(0);
}

int main(int argc, char* argv[])
{
  init(argc,argv);

  create_scene();

  glutMainLoop();
  
  return 0;
}