#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
// #include <gl/glew.h>
// #include <glfw/glfw3.h>

#define NUM 100000000

namespace {

const char* compute_shader_source = R"(
#version 430
uniform uint element_size;

uint hash3(uint x, uint y, uint z) {
    x += x >> 11;
    x ^= x << 7;
    x += y;
    x ^= x << 3;
    x += z ^ (x >> 14);
    x ^= x << 6;
    x += x >> 15;
    x ^= x << 5;
    x += x >> 12;
    x ^= x << 9;
    return x;
}

float random(vec3 f) {
    uint mantissaMask = 0x007FFFFFu;
    uint one = 0x3F800000u;
    uvec3 u = floatBitsToUint(f);
    uint h = hash3(u.x, u.y, u.z);
    return uintBitsToFloat((h & mantissaMask) | one) - 1.0;
}

layout(std430, binding = 3) buffer layout_dst
{
    int dst[];
};

layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

void main() {
    uint index = gl_GlobalInvocationID.x;
    // if (index >= element_size) { return; }
    float x = random(gl_GlobalInvocationID);
    float y = random(gl_GlobalInvocationID.yzx);
    if ((x*x + y*y) <= 1.0) {
        dst[index] = 1;
    }
}
)";

void initOpenGL() {
    auto inits_glfw = glfwInit();
    if (inits_glfw != GLFW_TRUE) {
        throw std::runtime_error("error occurred: glfwInit!");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(1, 1, "invisible window", nullptr, nullptr);
    if (window == nullptr) {
        throw std::runtime_error("error occurred: glfwCreateWindow!");
    }
    glfwMakeContextCurrent(window);

    auto inits_glew = glewInit();
    if (inits_glew != GLEW_OK) {
        throw std::runtime_error("error occurred: glewInit!");
    }
}

void terminateOpenGL() {
    glfwTerminate();
}


GLuint createComputeShaderProgram(const char* shader_src) {
    GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &shader_src, nullptr);
    glCompileShader(shader);

    GLint compiles = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiles);
    if (compiles == GL_FALSE) {
        GLint log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<GLchar> info_log(log_length);
        glGetShaderInfoLog(shader, log_length, &log_length, info_log.data());

        glDeleteShader(shader);

        std::string error_msg = "error occurred in compiling shader: ";
        throw std::runtime_error(error_msg + info_log.data());
    }

    GLuint program = glCreateProgram();

    glAttachShader(program, shader);
    glLinkProgram(program);

    GLint links = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &links);
    if (links == GL_FALSE) {
        GLint log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<GLchar> info_log(log_length);
        glGetProgramInfoLog(program, log_length, &log_length, info_log.data());

        glDeleteProgram(program);
        glDeleteShader(shader);

        std::string error_msg = "error occurred in linking shader: ";
        throw std::runtime_error(error_msg + info_log.data());
    }

    glDetachShader(program, shader);
    glDeleteShader(shader);

    return program;
}

void deleteComputeShaderProgram(GLuint program) {
    glDeleteProgram(program);
}


void compute() {
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    uint32_t num = NUM;

    GLuint shader_program = createComputeShaderProgram(compute_shader_source);

    // create buffer
    GLuint uniform_element_size = glGetUniformLocation(shader_program, "element_size");
    GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, num * sizeof(int), nullptr, GL_DYNAMIC_COPY);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glUseProgram(shader_program);

    glUniform1ui(uniform_element_size, num);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo);

    glDispatchCompute(num / 256 + 1, 1, 1);

    glUseProgram(0);

    std::vector<int> data(num);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, num * sizeof(int), data.data());
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // for (auto v : data) { std::cout << v << '\n'; }

    glDeleteBuffers(1, &ssbo);

    deleteComputeShaderProgram(shader_program);
    double stime = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() / 1000.0);
    std::cout << "gpu loop time : " << stime << " ms" << std::endl;
    // calc pi
    size_t sum = std::accumulate(data.begin(), data.end(), 0LL);
    std::cout << "sum : " << sum << std::endl;
    std::cout << "pi : " << 4.0 * sum / num << std::endl;
}

}

int main(int argc, char* argv[]) {
    std::cout << "number of elements: " << NUM << std::endl;
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    try {
        initOpenGL();
        compute();
        terminateOpenGL();
    }
    catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
    double stime = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() / 1000.0);
    std::cout << "all time : " << stime << " ms" << std::endl;
}