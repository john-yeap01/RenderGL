#ifndef RENDERGL_PLATFORM_APPLICATION_H
#define RENDERGL_PLATFORM_APPLICATION_H

#include <filesystem>

struct GLFWwindow;

class Application
{
public:
    Application();
    int Run();

private:
    std::filesystem::path ResolveResourceRoot() const;
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};

#endif
