#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <fstream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <thread>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//Global variables
//Camara
glm::vec3 camPos = glm::vec3(-8.0f, 0.0f, -10.0f); //-8, 0, 4
glm::vec3 camFront = glm::vec3(1.0f, 0.0f, 0.0f); //f1.0f, f1.0, f-1.0
glm::vec3 camUp = glm::vec3(0.0f, 2.5f, 0.f); //-1.f, -2.5f, 0.f 
glm::vec3* lSource = &camPos; //pointer to the current light source
float yaw = -90.f;
float pitch = 90.0f;
float fov = 45.f;
float with = 800;
float Heihgt = 600;
bool up = false;
bool right = false;
bool left = false;
bool down = false;

void framebuffer_resize(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void console(float &shininess);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


int main() {
    glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f); //Ortho limits

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* windowo = glfwCreateWindow(800, 600, "vic3", NULL, NULL);
    if (windowo == NULL) {
        std::cout << "Window error" << std::endl;
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(windowo);
    glfwSetFramebufferSizeCallback(windowo, framebuffer_resize);
    glfwSetScrollCallback(windowo, scroll_callback);
    glfwSetCursorPosCallback(windowo, mouse_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);

    glClearColor(0.2f, 0.6f, 0.3f, 3.0f);

    glEnable(GL_DEPTH_TEST);



    float vecTr[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glm::vec3 chatterPositions[] = {
glm::vec3(0.0f,  0.0f,  0.0f),
glm::vec3(2.0f,  5.0f, -15.0f),
glm::vec3(-1.5f, -2.2f, -2.5f),
glm::vec3(-3.8f, -2.0f, -12.3f),
glm::vec3(2.4f, -0.4f, -3.5f),
glm::vec3(-1.7f,  3.0f, -7.5f),
glm::vec3(1.3f, -2.0f, -2.5f),
glm::vec3(1.5f,  2.0f, -2.5f),

    };



    //What will be graphed
    unsigned int vertexArrayObject[2];
    glGenVertexArrays(2, vertexArrayObject);
    unsigned int vertexBufferObject[1];
    glGenBuffers(1, vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vecTr), vecTr, GL_STATIC_DRAW);

    glBindVertexArray(vertexArrayObject[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glBindVertexArray(vertexArrayObject[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Textuwure shit
//Loading the actual texture
    int width, height, numChannels;
    unsigned int map;
    glGenTextures(1, &map);
    glBindTexture(GL_TEXTURE_2D, map);
    //Setting up some texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Sending data to the texture (actualy making it)
    unsigned char* texdata = stbi_load("map.jpg", &width, &height, &numChannels, 0); //data for the texture
    if (texdata) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texdata);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "The texture data didn't load properly :(" << std::endl;
    }
    stbi_image_free(texdata);


    unsigned int mapSpec; //the texture is called sillySpec (Specular of the last texture)
    glGenTextures(1, &mapSpec);
    glBindTexture(GL_TEXTURE_2D, mapSpec);
    //Setting up some texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Sending data to the texture (actualy making it)
    texdata = stbi_load("map_specular.jpg", &width, &height, &numChannels, 0); //data for the texture
    if (texdata) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texdata);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "The texture data didn't load properly :(" << std::endl;
    }
    stbi_image_free(texdata);

    //Shader program for main object
    //Didn't want to take this from a file to prevent issues

    const char* Shadershit = "#version 330 core\n" //compiles on run  time
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 2) in vec2 aTexCoords;\n"
        "layout (location = 1) in vec3 aNormal;"
        "out vec2 stexture;\n"//stexture to prevent issues
        "out vec3 normal;\n"
        "out vec3 fragPos;\n"
        "uniform mat4 modelo;\n"
        "uniform mat4 vista;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "gl_Position =  projection * vista * modelo * vec4(aPos, 1.0);\n"
        "stexture = aTexCoords;\n"
        "normal = mat3(transpose(inverse(modelo)))* aNormal;\n"
        "fragPos = vec3(modelo * vec4(aPos, 1.0));\n"
        "}\0";

    unsigned int Vershad = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(Vershad, 1, &Shadershit, NULL);
    glCompileShader(Vershad);
    int errorFree; //Checking if the compilation is fine
    char infolog[512];
    glGetShaderiv(Vershad, GL_COMPILE_STATUS, &errorFree);
    if (!errorFree) {
        glGetShaderInfoLog(Vershad, 512, NULL, infolog);
        std::cout << "Error_compilation_shader_vertex_bad" << infolog << std::endl;
    }
    const char* fragmentshaderR = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 normal;\n"
        "in vec3 fragPos;\n"
        "in vec2 stexture;\n"
        "struct Material {\n" //The structs are only here for organisations
        "sampler2D diffuse;\n"
        "sampler2D specular;\n"
        "float shininess;\n"
        "};\n"

        "struct Light {\n"
        //values for different light values
        "vec3 position;\n"
        "vec3 ambient;\n"
        "vec3 diffuse;\n"
        "vec3 specular;\n"
        //Light distance stuff
        "float constant;\n"
        "float linear;\n"
        "float quadratic;\n"
        "};\n"

        "uniform vec3 viewPos;\n"
        "uniform Material material;\n"
        "uniform Light light;\n"
        "void main()\n"
        "{\n"
        //ambient
        "vec3 ambient = light.ambient * texture(material.diffuse,  stexture).rgb;\n"

        // diffuse 
        "vec3 norm = normalize(normal);\n"
        "vec3 lightDir = normalize(light.position - fragPos);\n"
        "float diff = max(dot(norm, lightDir), 0.0);\n"
        "vec3 diffuse = light.diffuse * diff * texture(material.diffuse,  stexture).rgb;\n"

        // specular
        "vec3 viewDir = normalize(viewPos - fragPos);\n"
        "vec3 reflectDir = reflect(-lightDir, norm);\n"
        "float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
        "vec3 specular = light.specular * spec * texture(material.specular,  stexture).rgb;\n"

        // attenuation
        "float distance = length(light.position - fragPos);\n"
        "float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n"

        "ambient *= attenuation;\n"
        "diffuse *= attenuation;\n"
        "specular *= attenuation;\n"

        "vec3 result = ambient + diffuse + specular;\n"
        "FragColor = vec4(result, 1.0); \n"
        " }\0";

    unsigned int fradshadR = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fradshadR, 1, &fragmentshaderR, NULL);
    glCompileShader(fradshadR);
    glGetShaderiv(fradshadR, GL_COMPILE_STATUS, &errorFree);
    if (!errorFree) {
        glGetShaderInfoLog(fradshadR, 512, NULL, infolog);
        std::cout << "Error_compilation_shader_fragment_bad" << infolog << std::endl;
    }


    //shader programs
    unsigned int shadpromR = glCreateProgram();
    glAttachShader(shadpromR, fradshadR);
    glAttachShader(shadpromR, Vershad);
    glLinkProgram(shadpromR);
    glGetProgramiv(shadpromR, GL_LINK_STATUS, &errorFree);
    if (!errorFree) {
        glGetProgramInfoLog(shadpromR, 512, NULL, infolog);

        std::cout << "Error_shad_program_bad" << infolog << std::endl;
    }


    glDeleteShader(fradshadR);
    glDeleteShader(Vershad);    


    float shine = 300.f;
    std::thread f(console, std::ref(shine));
    bool inputControle;
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(windowo)) {//Render loop
        glm::vec3 lightPosition(sin(glfwGetTime()/2) * 12,0., cos(glfwGetTime() / 2) * 12);//position of the lightsource, moves in circles to simulate the sun
        //calculate delta time
        float deltaTime = glfwGetTime() - lastFrame;
        lastFrame = glfwGetTime();
        //view matrix
        glm::mat4 vista = glm::lookAt(camPos, camPos + camFront, camUp); //I realised I named shit in spanish after I had already done stuff with it.
        //model
        glm::mat4 modelo = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
        modelo = glm::rotate(modelo, glm::radians(270.f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelo = glm::rotate(modelo, glm::radians(270.f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelo = glm::scale(modelo, glm::vec3(10.f));

        { //input block, most people make it a funtion, but I prefer to keep it here for easy acces when chaging rendering and input.
            const float camSpeed = 2.6f * deltaTime;
            
            if (glfwGetKey(windowo, GLFW_KEY_A) == GLFW_PRESS || left ) { //Left
                camPos -= camSpeed * glm::normalize(glm::cross(camFront, camUp));
            }
            if (glfwGetKey(windowo, GLFW_KEY_D) == GLFW_PRESS || right) { //Right
                camPos += camSpeed * glm::normalize(glm::cross(camFront, camUp));
            }
            if (glfwGetKey(windowo, GLFW_KEY_W) == GLFW_PRESS || up) { //Upwards
                camPos += camSpeed * camUp;
            }
            if (glfwGetKey(windowo, GLFW_KEY_S) == GLFW_PRESS || down) { //Downwards
                camPos -= camSpeed * camUp;
            }
            else if (glfwGetKey(windowo, GLFW_KEY_B) == GLFW_PRESS) { //center
                camPos = glm::vec3(-8.0f, 0.0f, 3.0f);
            }
            else if (glfwGetKey(windowo, GLFW_KEY_X) == GLFW_PRESS) { //Change light
                if (lSource == &camPos && inputControle)
                {
                    lSource = &lightPosition;
                    inputControle = false;
                }
                else if (inputControle){
                    lSource = &camPos;
                    inputControle = false;
                }
            }
            else {
                inputControle = true;
            }
            if (camPos.y > 3.28131) {
                camPos.y = 3.28131;
            }
            else if (camPos.y < -3.14865) {
                camPos.y = -3.14865;
            }
            if (camPos.z < -15) {
                camPos.z = -5;
            }
            else if (camPos.z > -3) {
                camPos.z = -13.f;
            }

        }

        //Actual rendering starts here.
        glClearColor(0.2f, 0.3f, 1.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shadpromR);
        glUniform3fv(glGetUniformLocation(shadpromR, "light.position"), 1, glm::value_ptr(*lSource));
        glUniform3fv(glGetUniformLocation(shadpromR, "viewPos"), 1, glm::value_ptr(camPos));
        glUniform1f(glGetUniformLocation(shadpromR, "material.shininess"), 300.f);
        glm::vec3 lightColour(1.f, 1.f, 1.f);
        glUniform3fv(glGetUniformLocation(shadpromR, "light.ambient"), 1, glm::value_ptr(lightColour * glm::vec3(0.1f)));
        glUniform3fv(glGetUniformLocation(shadpromR, "light.diffuse"), 1, glm::value_ptr(lightColour * glm::vec3(0.5f)));
        glUniform3f(glGetUniformLocation(shadpromR, "light.specular"), 1.0f, 1.0f, 1.0f);
        //point light
        glUniform1f(glGetUniformLocation(shadpromR, "light.constant"), 1.f);
        glUniform1f(glGetUniformLocation(shadpromR, "light.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(shadpromR, "light.quadratic"), 0.032f);
        glUniform1f(glGetUniformLocation(shadpromR, "material.shininess"), shine);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, map);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mapSpec);



        //World matrices
        glUniformMatrix4fv(glGetUniformLocation(shadpromR, "vista"), 1, GL_FALSE, glm::value_ptr(vista));
        glUniformMatrix4fv(glGetUniformLocation(shadpromR, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shadpromR, "modelo"), 1, GL_FALSE, glm::value_ptr(modelo));


        //Assigning textures.
        glBindVertexArray(vertexArrayObject[0]);
        glUniform1i(glGetUniformLocation(shadpromR, "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(shadpromR, "material.specular"), 1);

        //make the Cube (a cube cuz my other vertex plan kinda failed)
        for (unsigned char i = 0; i < 3;i++) {
            glDrawArrays(GL_TRIANGLES, 0, 36); //36 for cube
            modelo = glm::translate(modelo, glm::vec3(-1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(glGetUniformLocation(shadpromR, "modelo"), 1, GL_FALSE, glm::value_ptr(modelo));
        }

        //window shit
        glfwSwapBuffers(windowo);
        glfwPollEvents();
        //Debug shit
        //std::cout << camPos.x << std::endl;
    }
    glfwTerminate();    

    return 0;

}

void framebuffer_resize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width - 5, height - 5);
    with = width;
    Heihgt = height;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    else if (fov > 90.0f)
        fov = 90.0f;
}

void console(float &shininess) {
    std::cout << "---------------\nControls:\nmovement:WASD\nZoom: scroll wheel\nReturn to origen: B\nTurn day-night cicle on/oof: X\n---------------\nType the (float) you want the water shininess to have the bigger the number, the smaller the shininess\ncurrrent shininess " << shininess << std::endl;
    while (true) {
        std::cin >> shininess;
        std::cout << std::endl;
    }

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {


    if (xpos > with*0.95) {
        right = true;
    }
    else {
        right = false;
    }
    if (xpos < with/20) {
        left = true;
    }
    else {
        left = false;
    }
    if (ypos > Heihgt * 0.95) {
        down = true;
    }
    else {
        down = false;
    }
    if (ypos < Heihgt/20) {
        up = true;
    }
    else {
        up = false;
    }
}