#include "SimpleFramework.h"
#include <memory>

int main(void)
{ 
    // std::unique_ptr<Demo> demo(new BallCollisionDemo(1024, 768, "Ball collision demo"));
    std::unique_ptr<Demo> demo(new NBallsCollisionDemo(1024, 768, "N-Ball collisions demo", 1000));
    demo->run();

    return 0;
}