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
        renderer.blendMode(agl::BLEND);
        renderer.loadTexture("particle", "../textures/dollar.png", 0);
        renderer.loadTexture("coin", "../textures/coin.png", 0);
    }

    void createConfetti(int size)
    {
        
        for (int i = 0; i < size; i++)
        {
            Particle particle;
            particle.color = vec4(.22f, 0.86f, 0.11f, 1);
            particle.t = glfwGetTime();
            particle.size = 0.25;
            particle.rot = 0.0;
            particle.pos.x = clamp(agl::randomUnitCube().x * 3.0f, -3.0f, 3.0f);
            particle.vel = vec3(0, clamp(agl::randomUnitCube().x, -1.7f, -0.8f), 0);
            mParticles.push_back(particle);
        }
    }

    void createCoins(int size)
    {

        for (int i = 0; i < size; i++)
        {
            Particle particle;
            particle.color = vec4(.98f, 0.83f, 0.22f, 1); 
            particle.t = glfwGetTime();
            particle.size = 0.0025;
            particle.rot = 0.0;
            particle.pos.x = clamp(agl::randomUnitCube().x * 3.0f, -3.0f, 3.0f);
            particle.vel = vec3(0, clamp(agl::randomUnitCube().x, -1.7f, -0.8f), 0);
            mCoins.push_back(particle);
        }
    }

    void updateConfetti()
    {
        for (int i = 0; i < mParticles.size(); i++) {
            mParticles[i].pos = mParticles[i].pos + (mParticles[i].vel * dt());
            if (mParticles[i].pos.y <= -1.5f) {
                mParticles[i].t = glfwGetTime();
                mParticles[i].size = 0.25;
                mParticles[i].rot = 0.0;
                mParticles[i].pos = position;
                mParticles[i].vel.y = clamp(agl::randomUnitCube().x, -1.8f, -0.7f);

            }
        }
    }

    void updateCoins()
    {
        for (int i = 0; i < mCoins.size(); i++) {
            mCoins[i].pos = mCoins[i].pos + (mCoins[i].vel * dt());
            mCoins[i].rot += 0.1f;
            if (mCoins[i].pos.y <= -1.5f) {
                mCoins[i].t = glfwGetTime();
                mCoins[i].size = 0.075;
                mCoins[i].rot = 0.0;
                mCoins[i].pos = position;
                mCoins[i].vel.y = clamp(agl::randomUnitCube().x, -2.1f, -1.0f);

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

    void drawCoins() {
        renderer.texture("image", "coin");
        for (int i = 0; i < mCoins.size(); i++)
        {
            Particle particle = mCoins[i];
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
        position.x = clamp(agl::randomUnitCube().x * 3.0f, -3.0f, 3.0f);
        position.y = 3.0f;
        if (mParticles.size() < 1000) {
            createConfetti(1);
            createCoins(1);
        }
        float aspect = ((float)width()) / height();
        renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
        renderer.lookAt(eyePos, lookPos, up);
        updateConfetti();
        updateCoins();
        drawConfetti();
        drawCoins();
        renderer.endShader();
    }

protected:

    vec3 eyePos = vec3(0, 0, 3);
    vec3 lookPos = vec3(0, 0, 0);
    vec3 up = vec3(0, 1, 0);
    vec3 position = vec3(1, 2, 0);

    std::vector<Particle> mParticles;
    std::vector<Particle> mCoins;
};

int main(int argc, char** argv)
{
    Viewer viewer;
    viewer.run();
    return 0;
}
