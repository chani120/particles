// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

struct Particle {
  glm::vec3 pos;
  glm::vec3 vel;
  glm::vec4 color;
  float rot;
  float size;
  float t;
};

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }

  void createConfetti(int size)
  {
    renderer.loadTexture("particle", "../textures/star4.png", 0);
    for (int i = 0; i < size; i++)
    {
      Particle particle;
      particle.color = vec4(agl::randomUnitCube(), 1);
      particle.t = glfwGetTime();
      particle.size = 0.25;
      particle.rot = 0.0;
      particle.pos = position;
      particle.vel = vec3(-position.y, position.x, 0) + agl::randomUnitCube();
      mParticles.push_back(particle);
    }
  }

  void updateConfetti()
  {
      for (int i = 0; i < mParticles.size(); i++) {
          mParticles[i].pos = mParticles[i].pos + (mParticles[i].vel * dt());
          mParticles[i].color.a = mParticles[i].color.a * 0.97;
          if (glfwGetTime() - mParticles[i].t > 1.5) {
              mParticles[i].color = vec4(agl::randomUnitCube(), 1);
              mParticles[i].t = glfwGetTime();
              mParticles[i].size = 0.25;
              mParticles[i].rot = 0.0;
              mParticles[i].pos = position;
              mParticles[i].vel = vec3(-position.y, position.x, 0) + agl::randomUnitCube();
          }
      }
  }

  void drawConfetti()
  {
    renderer.texture("image", "particle");
    for (int i = 0; i < mParticles.size(); i++)
    {
      Particle particle = mParticles[i];
      renderer.sprite(particle.pos, particle.color, particle.size, particle.rot);
    }
  }

  void mouseMotion(int x, int y, int dx, int dy) {
  }

  void mouseDown(int button, int mods) {
  }

  void mouseUp(int button, int mods) {
  }

  void scroll(float dx, float dy) {
    eyePos.z += dy;
  }

  void keyUp(int key, int mods) {
  }

  void draw() {
    renderer.beginShader("sprite");
    position.x = cos(glfwGetTime());
    position.y = sin(-glfwGetTime());
    if (mParticles.size() < 1000) {
        createConfetti(1);
    }
    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    renderer.lookAt(eyePos, lookPos, up);
    renderer.sprite(position, vec4(1.0f), 0.25f);
    updateConfetti();
    drawConfetti();
    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 3);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 position = vec3(1, 0, 0);

  std::vector<Particle> mParticles;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
