#include <string>
#include <iostream>
#include <cstdint>
#include <stdexcept>
#include <glad/glad.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "fundamentum/Logger.h"

#include "ShaderCompiler.h"
#include "ShaderProgram.h"
#include "AppWindowPrivate.h"
#include "Renderer.h"
#include "Input.h"
#include "KeyInput.h"
#include "KeyEvent.h"

namespace
{
    template <typename T>
    void glBufferVector(GLenum target, const std::vector<T> &data, GLenum usage)
    {
        glBufferData(target, data.size() * sizeof(T), data.data(), usage);
    }

    int toPrdKey(int glfwKey)
    {
        switch (glfwKey)
        {
        case GLFW_KEY_SPACE:
            return PRD_KEY_SPACE;
        case GLFW_KEY_APOSTROPHE:
            return PRD_KEY_APOSTROPHE;
        case GLFW_KEY_COMMA:
            return PRD_KEY_COMMA;
        case GLFW_KEY_MINUS:
            return PRD_KEY_MINUS;
        case GLFW_KEY_PERIOD:
            return PRD_KEY_PERIOD;
        case GLFW_KEY_SLASH:
            return PRD_KEY_SLASH;
        case GLFW_KEY_0:
            return PRD_KEY_0;
        case GLFW_KEY_1:
            return PRD_KEY_1;
        case GLFW_KEY_2:
            return PRD_KEY_2;
        case GLFW_KEY_3:
            return PRD_KEY_3;
        case GLFW_KEY_4:
            return PRD_KEY_4;
        case GLFW_KEY_5:
            return PRD_KEY_5;
        case GLFW_KEY_6:
            return PRD_KEY_6;
        case GLFW_KEY_7:
            return PRD_KEY_7;
        case GLFW_KEY_8:
            return PRD_KEY_8;
        case GLFW_KEY_9:
            return PRD_KEY_9;
        case GLFW_KEY_SEMICOLON:
            return PRD_KEY_SEMICOLON;
        case GLFW_KEY_EQUAL:
            return PRD_KEY_EQUAL;
        case GLFW_KEY_A:
            return PRD_KEY_A;
        case GLFW_KEY_B:
            return PRD_KEY_B;
        case GLFW_KEY_C:
            return PRD_KEY_C;
        case GLFW_KEY_D:
            return PRD_KEY_D;
        case GLFW_KEY_E:
            return PRD_KEY_E;
        case GLFW_KEY_F:
            return PRD_KEY_F;
        case GLFW_KEY_G:
            return PRD_KEY_G;
        case GLFW_KEY_H:
            return PRD_KEY_H;
        case GLFW_KEY_I:
            return PRD_KEY_I;
        case GLFW_KEY_J:
            return PRD_KEY_J;
        case GLFW_KEY_K:
            return PRD_KEY_K;
        case GLFW_KEY_L:
            return PRD_KEY_L;
        case GLFW_KEY_M:
            return PRD_KEY_M;
        case GLFW_KEY_N:
            return PRD_KEY_N;
        case GLFW_KEY_O:
            return PRD_KEY_O;
        case GLFW_KEY_P:
            return PRD_KEY_P;
        case GLFW_KEY_Q:
            return PRD_KEY_Q;
        case GLFW_KEY_R:
            return PRD_KEY_R;
        case GLFW_KEY_S:
            return PRD_KEY_S;
        case GLFW_KEY_T:
            return PRD_KEY_T;
        case GLFW_KEY_U:
            return PRD_KEY_U;
        case GLFW_KEY_V:
            return PRD_KEY_V;
        case GLFW_KEY_W:
            return PRD_KEY_W;
        case GLFW_KEY_X:
            return PRD_KEY_X;
        case GLFW_KEY_Y:
            return PRD_KEY_Y;
        case GLFW_KEY_Z:
            return PRD_KEY_Z;
        case GLFW_KEY_LEFT_BRACKET:
            return PRD_KEY_LEFT_BRACKET;
        case GLFW_KEY_BACKSLASH:
            return PRD_KEY_BACKSLASH;
        case GLFW_KEY_RIGHT_BRACKET:
            return PRD_KEY_RIGHT_BRACKET;
        case GLFW_KEY_GRAVE_ACCENT:
            return PRD_KEY_GRAVE_ACCENT;
        case GLFW_KEY_WORLD_1:
            return PRD_KEY_WORLD_1;
        case GLFW_KEY_WORLD_2:
            return PRD_KEY_WORLD_2;
        case GLFW_KEY_ESCAPE:
            return PRD_KEY_ESCAPE;
        case GLFW_KEY_ENTER:
            return PRD_KEY_ENTER;
        case GLFW_KEY_TAB:
            return PRD_KEY_TAB;
        case GLFW_KEY_BACKSPACE:
            return PRD_KEY_BACKSPACE;
        case GLFW_KEY_INSERT:
            return PRD_KEY_INSERT;
        case GLFW_KEY_DELETE:
            return PRD_KEY_DELETE;
        case GLFW_KEY_RIGHT:
            return PRD_KEY_RIGHT;
        case GLFW_KEY_LEFT:
            return PRD_KEY_LEFT;
        case GLFW_KEY_DOWN:
            return PRD_KEY_DOWN;
        case GLFW_KEY_UP:
            return PRD_KEY_UP;
        case GLFW_KEY_PAGE_UP:
            return PRD_KEY_PAGE_UP;
        case GLFW_KEY_PAGE_DOWN:
            return PRD_KEY_PAGE_DOWN;
        case GLFW_KEY_HOME:
            return PRD_KEY_HOME;
        case GLFW_KEY_END:
            return PRD_KEY_END;
        case GLFW_KEY_CAPS_LOCK:
            return PRD_KEY_CAPS_LOCK;
        case GLFW_KEY_SCROLL_LOCK:
            return PRD_KEY_SCROLL_LOCK;
        case GLFW_KEY_NUM_LOCK:
            return PRD_KEY_NUM_LOCK;
        case GLFW_KEY_PRINT_SCREEN:
            return PRD_KEY_PRINT_SCREEN;
        case GLFW_KEY_PAUSE:
            return PRD_KEY_PAUSE;
        case GLFW_KEY_F1:
            return PRD_KEY_F1;
        case GLFW_KEY_F2:
            return PRD_KEY_F2;
        case GLFW_KEY_F3:
            return PRD_KEY_F3;
        case GLFW_KEY_F4:
            return PRD_KEY_F4;
        case GLFW_KEY_F5:
            return PRD_KEY_F5;
        case GLFW_KEY_F6:
            return PRD_KEY_F6;
        case GLFW_KEY_F7:
            return PRD_KEY_F7;
        case GLFW_KEY_F8:
            return PRD_KEY_F8;
        case GLFW_KEY_F9:
            return PRD_KEY_F9;
        case GLFW_KEY_F10:
            return PRD_KEY_F10;
        case GLFW_KEY_F11:
            return PRD_KEY_F11;
        case GLFW_KEY_F12:
            return PRD_KEY_F12;
        case GLFW_KEY_F13:
            return PRD_KEY_F13;
        case GLFW_KEY_F14:
            return PRD_KEY_F14;
        case GLFW_KEY_F15:
            return PRD_KEY_F15;
        case GLFW_KEY_F16:
            return PRD_KEY_F16;
        case GLFW_KEY_F17:
            return PRD_KEY_F17;
        case GLFW_KEY_F18:
            return PRD_KEY_F18;
        case GLFW_KEY_F19:
            return PRD_KEY_F19;
        case GLFW_KEY_F20:
            return PRD_KEY_F20;
        case GLFW_KEY_F21:
            return PRD_KEY_F21;
        case GLFW_KEY_F22:
            return PRD_KEY_F22;
        case GLFW_KEY_F23:
            return PRD_KEY_F23;
        case GLFW_KEY_F24:
            return PRD_KEY_F24;
        case GLFW_KEY_F25:
            return PRD_KEY_F25;
        case GLFW_KEY_KP_0:
            return PRD_KEY_KP_0;
        case GLFW_KEY_KP_1:
            return PRD_KEY_KP_1;
        case GLFW_KEY_KP_2:
            return PRD_KEY_KP_2;
        case GLFW_KEY_KP_3:
            return PRD_KEY_KP_3;
        case GLFW_KEY_KP_4:
            return PRD_KEY_KP_4;
        case GLFW_KEY_KP_5:
            return PRD_KEY_KP_5;
        case GLFW_KEY_KP_6:
            return PRD_KEY_KP_6;
        case GLFW_KEY_KP_7:
            return PRD_KEY_KP_7;
        case GLFW_KEY_KP_8:
            return PRD_KEY_KP_8;
        case GLFW_KEY_KP_9:
            return PRD_KEY_KP_9;
        case GLFW_KEY_KP_DECIMAL:
            return PRD_KEY_KP_DECIMAL;
        case GLFW_KEY_KP_DIVIDE:
            return PRD_KEY_KP_DIVIDE;
        case GLFW_KEY_KP_MULTIPLY:
            return PRD_KEY_KP_MULTIPLY;
        case GLFW_KEY_KP_SUBTRACT:
            return PRD_KEY_KP_SUBTRACT;
        case GLFW_KEY_KP_ADD:
            return PRD_KEY_KP_ADD;
        case GLFW_KEY_KP_ENTER:
            return PRD_KEY_KP_ENTER;
        case GLFW_KEY_KP_EQUAL:
            return PRD_KEY_KP_EQUAL;
        case GLFW_KEY_LEFT_SHIFT:
            return PRD_KEY_LEFT_SHIFT;
        case GLFW_KEY_LEFT_CONTROL:
            return PRD_KEY_LEFT_CONTROL;
        case GLFW_KEY_LEFT_ALT:
            return PRD_KEY_LEFT_ALT;
        case GLFW_KEY_LEFT_SUPER:
            return PRD_KEY_LEFT_SUPER;
        case GLFW_KEY_RIGHT_SHIFT:
            return PRD_KEY_RIGHT_SHIFT;
        case GLFW_KEY_RIGHT_CONTROL:
            return PRD_KEY_RIGHT_CONTROL;
        case GLFW_KEY_RIGHT_ALT:
            return PRD_KEY_RIGHT_ALT;
        case GLFW_KEY_RIGHT_SUPER:
            return PRD_KEY_RIGHT_SUPER;
        case GLFW_KEY_MENU:
            return PRD_KEY_MENU;
        case GLFW_KEY_UNKNOWN:
            return PRD_KEY_UNKNOWN;
        }
        throw std::runtime_error("Unhandled GLFW Key");
    }

    int toPrdAction(int glfwAction)
    {
        switch (glfwAction)
        {
        case GLFW_PRESS:
            return PRD_PRESS;
        case GLFW_REPEAT:
            return PRD_REPEAT;
        case GLFW_RELEASE:
            return PRD_RELEASE;
        default:
            return PRD_KEY_UNKNOWN;
        }
    }

    int toPrdMods(int glfwMods)
    {
        int prdMods = 0;
        prdMods |= glfwMods & GLFW_MOD_SHIFT ? PRD_MOD_SHIFT : 0;
        prdMods |= glfwMods & GLFW_MOD_CONTROL ? PRD_MOD_CONTROL : 0;
        prdMods |= glfwMods & GLFW_MOD_ALT ? PRD_MOD_ALT : 0;
        prdMods |= glfwMods & GLFW_MOD_SUPER ? PRD_MOD_SUPER : 0;
        prdMods |= glfwMods & GLFW_MOD_CAPS_LOCK ? PRD_MOD_CAPS_LOCK : 0;
        prdMods |= glfwMods & GLFW_MOD_NUM_LOCK ? PRD_MOD_NUM_LOCK : 0;
        return prdMods;
    }

    std::vector<KeyEvent> keyEvents;
    double xpos, ypos, prevX, prevY = 0.f;

    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        keyEvents.push_back({toPrdKey(key), scancode, toPrdAction(action), toPrdMods(mods)});
    }

    void cursor_position_callback(GLFWwindow *window, double newXpos, double newYpos)
    {
        xpos = newXpos;
        ypos = newYpos;
    }
}

int createShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
{
    int vertexShader = ShaderCompiler::compileShader(vertexShaderSource, ShaderCompiler::ShaderType::Vertex);
    Logger::Log("Created vertex shader " + std::to_string(vertexShader));
    int fragmentShader = ShaderCompiler::compileShader(fragmentShaderSource, ShaderCompiler::ShaderType::Fragment);
    Logger::Log("Created fragment shader " + std::to_string(fragmentShader));

    auto program = Program::createProgram({vertexShader, fragmentShader});
    Logger::Log("Created program " + std::to_string(program));

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void useShaderProgram(int program)
{
    glUseProgram(program);
}

void setBool(int shaderProgram, const std::string &name, bool value)
{
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}

void setInt(int shaderProgram, const std::string &name, int value)
{
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void setFloat(int shaderProgram, const std::string &name, float value)
{
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void setVec3(int shaderProgram, const std::string &name, glm::vec3 value)
{
    glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), value.x, value.y, value.z);
}

void setMat4(int shaderProgram, const std::string &name, glm::mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void loadTexture(Texture *texture)
{
    uint32_t textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto width = texture->getWidth();
    auto height = texture->getHeight();
    auto channels = texture->getChannels();
    auto data = texture->getData();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    auto colorSpace = channels == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, colorSpace, texture->getWidth(), texture->getHeight(), 0, colorSpace, GL_UNSIGNED_BYTE, texture->getData());
    glGenerateMipmap(GL_TEXTURE_2D);
    texture->setId(textureId);
}

void loadMesh(Mesh *mesh)
{
    uint32_t VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferVector(GL_ARRAY_BUFFER, mesh->getVertexBuffer(), GL_STATIC_DRAW);
    auto &materialsMap = mesh->getMaterials();
    for (auto &subMesh : mesh->getSubMeshes())
    {
        uint32_t VAO;
        glGenVertexArrays(1, &VAO);
        subMesh.vao = VAO;
        uint32_t EBO;
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, subMesh.indexCount * sizeof(uint32_t), mesh->getIndices().data() + subMesh.startIndex, GL_STATIC_DRAW);

        // Vertex
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // UV
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // Normal
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
        // Tangents
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)(8 * sizeof(float)));
        glEnableVertexAttribArray(3);
        // Bitangents
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)(11 * sizeof(float)));
        glEnableVertexAttribArray(4);

        auto material = materialsMap[subMesh.materialName];
        assert(material != nullptr);
        for (auto textureProperty = material->texturesBegin(); textureProperty != material->texturesEnd(); textureProperty++)
        {
            loadTexture(textureProperty->second.get());
        }
    }
}

// TODO: view and project can be done separate?
void drawMesh(Mesh *mesh, const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection)
{
    auto &materialsMap = mesh->getMaterials();
    for (auto &subMesh : mesh->getSubMeshes())
    {
        auto *material = materialsMap[subMesh.materialName];
        assert(material != nullptr);
        auto shader = material->shader();
        useShaderProgram(shader);
        int textureId = 0;
        for (auto textureProperty = material->texturesBegin(); textureProperty != material->texturesEnd(); textureProperty++)
        {
            setInt(shader, textureProperty->first, textureId);
            glActiveTexture(GL_TEXTURE0 + textureId);
            glBindTexture(GL_TEXTURE_2D, textureProperty->second->getId());
            textureId++;
        }
        for (auto floatProperty = material->floatsBegin(); floatProperty != material->floatsEnd(); floatProperty++)
        {
            setFloat(shader, floatProperty->first, floatProperty->second);
        }
        for (auto vec3Property = material->vec3sBegin(); vec3Property != material->vec3sEnd(); vec3Property++)
        {
            setVec3(shader, vec3Property->first, vec3Property->second);
        }
        // TODO setBool for useTexture

        // Globals, think about it
        setVec3(shader, "lightPos", glm::vec3(0, 0, 40));
        setVec3(shader, "lightColor", glm::vec3(1, 1, 1));
        setMat4(shader, "model", model);
        setMat4(shader, "view", view);
        setMat4(shader, "projection", projection);
        glBindVertexArray(subMesh.vao);
        glDrawElements(GL_TRIANGLES, subMesh.indexCount * sizeof(uint32_t), GL_UNSIGNED_INT, 0);
    }
}

void clearScreen(const glm::vec4 &color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

AppWindow *createWindow(int width, int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16);

    auto glfwWindow = glfwCreateWindow(width, height, "Fabricatio", NULL, NULL);
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowPos(glfwWindow, (1920 - width) / 2, 32);
    if (glfwWindow == NULL)
    {
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(glfwWindow);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return nullptr;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);

    // glEnable(GL_CULL_FACE);

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(glfwWindow, [](GLFWwindow *window, int width, int height)
                                   { glViewport(0, 0, width, height); });
    glfwSetKeyCallback(glfwWindow, key_callback);
    glfwSetCursorPosCallback(glfwWindow, cursor_position_callback);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);

    ImGui_ImplOpenGL3_Init("#version 130");

    return new AppWindow(glfwWindow);
}

void destroyWindow(AppWindow *window)
{
    delete window;
}

bool shouldClose(AppWindow *window)
{
    return glfwWindowShouldClose(**window);
}

void setShouldClose(AppWindow *window, bool shouldClose)
{
    glfwSetWindowShouldClose(**window, shouldClose);
}

Input processInput(AppWindow *window)
{
    Input result;
    result.keyEvents = keyEvents;
    result.deltaMouseX = xpos - prevX;
    result.deltaMouseY = ypos - prevY;
    prevX = xpos;
    prevY = ypos;
    keyEvents.clear();
    return result;
}

void swapBuffer(AppWindow *window)
{
    glfwSwapBuffers(**window);
}

void pollEvents(AppWindow *window)
{
    glfwPollEvents();
}

void newImGuiFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
}

void imGuiRenderDrawData(ImDrawData *drawData)
{
    ImGui_ImplOpenGL3_RenderDrawData(drawData);
}

ImGuiContext *imGuiCurrentContext()
{
    return ImGui::GetCurrentContext();
}

void enableCursor(AppWindow *window, bool showCursor)
{
    glfwSetInputMode(**window, GLFW_CURSOR, showCursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}
