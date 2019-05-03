#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../data_types/context.h"


// maxZoomFactor must be < 2
const double maxZoomFactor = 1.9;
//minZoomFactor must be > 0
const double minZoomFactor = 0.1;

const double zoomStartValue = 1.0;

void scrollCallback(GLFWwindow* window, double x_offset, double y_offset);

void mouseButtonPressed(Context* ctx, int button, int x, int y);

void mouseButtonReleased(Context* ctx, int button, int x, int y);

void resizeCallback(GLFWwindow* window, int width, int height);

void cursorPosCallback(GLFWwindow* window, double x, double y);

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);