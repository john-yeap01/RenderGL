#ifndef RENDERGL_VIEWMODEL_INPUT_STATE_H
#define RENDERGL_VIEWMODEL_INPUT_STATE_H

struct InputState
{
    bool closeRequested = false;
    bool moveForward = false;
    bool moveBackward = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;
    bool boost = false;
    bool lookActive = false;
    bool captureCameraInput = true;

    int windowWidth = 1;
    int windowHeight = 1;
    int framebufferWidth = 1;
    int framebufferHeight = 1;

    double mouseX = 0.0;
    double mouseY = 0.0;
};

#endif
