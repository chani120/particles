// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    renderer.loadShader("simple-texture",
      "../shaders/simple-texture.vs",
      "../shaders/simple-texture.fs");

    img.load("../textures/tree.png", true);
    renderer.loadTexture("tree", img, 0);
    // TODO: Use the width and the height of the image to scale the billboard

    renderer.loadTexture("grass", "../textures/grass.png", 0);
    renderer.blendMode(agl::BLEND);
  }


  void mouseMotion(int x, int y, int dx, int dy) {
      if (selected) {
          azimuth_var = azimuth_var - dx * 0.02;
          elevation_var = elevation_var + dy * 0.02;
          float x = rad * sin(azimuth_var) * cos(elevation_var);
          float y = rad * sin(elevation_var);
          float z = rad * cos(azimuth_var) * cos(elevation_var);
          eyePos = vec3(x, y, z);
      }
  }

  void mouseDown(int button, int mods) {
      selected = true;
  }

  void mouseUp(int button, int mods) {
      selected = false;
  }

  void scroll(float dx, float dy) {
  }

  void draw() {
    renderer.beginShader("simple-texture");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    renderer.lookAt(eyePos, lookPos, up);

    // draw plane
    renderer.texture("Image", "grass");
    renderer.push();
    renderer.translate(vec3(0.0, -0.5, 0));
    renderer.scale(vec3(2.0f));
    renderer.plane();
    renderer.pop();

    vec3 direction = normalize(eyePos - lookPos);
    float theta = atan2(direction.x, direction.z);
    // draw tree
    renderer.texture("Image", "tree");
    renderer.push();
    renderer.rotate(theta, vec3(0.0, 1.0, 0.0));
    renderer.translate(vec3(-0.5, -0.5, 0));
    renderer.scale(vec3((float)img.width() / (float)img.height(), 1.0, 1.0));
    renderer.quad(); // vertices span from (0,0,0) to (1,1,0)
    renderer.pop();

    renderer.endShader();
  }

protected:
  float elevation_var = 0;
  float azimuth_var = 0;
  float rad = 10;
  bool selected = false;
  Image img;
  vec3 eyePos = vec3(0, 0, 2);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
