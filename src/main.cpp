#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_SWIZZLE

// #include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::string root = std::filesystem::current_path();
std::string path = root + "/";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 1211;
const unsigned int SCR_HEIGHT = 700;

int capture = -1;
// camera
Camera camera(glm::vec3(0.0f, 3.0f, 4.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

class cube
{
    // Access specifier
    public:
    int r=0,g=0,b=0;
    glm::vec3 pos = glm::vec3( 0.0f,  0.0f,  0.0f);
    glm::vec4 color = glm::vec4( 0.0f,  0.0f,  0.0f, 0.0f);
};
       glm::vec3 prev_up = camera.Up;
       glm::vec3 prev_front = camera.Front; 


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "graphics render", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    const char* glsl_version = "#version 150";

    vector<cube> cube_list;
    for (int i = 0; i < 10; i++)
    {
    cube temp;

    cube_list.push_back( temp);
    }

    {
        cube_list[0].color = glm::vec4( 0.0f/255.0f,  71.0f/255.0f,  189.0f/255.0f, 1.0f);
        cube_list[1].color = glm::vec4( 7.0f/255.0f,  185.0f/255.0f,  217.0f/255.0f, 1.0f);
        cube_list[2].color = glm::vec4( 0.0f/255.0f,  171.0f/255.0f,  56.0f/255.0f, 1.0f);
        cube_list[3].color = glm::vec4( 154.0f/255.0f,  240.0f/255.0f,  0.0f/255.0f, 1.0f);
        cube_list[4].color = glm::vec4( 255.0f/255.0f,  179.0f/255.0f,  0.0f/255.0f, 1.0f);
        cube_list[5].color = glm::vec4( 255.0f/255.0f,  206.0f/255.0f,  0.0f/255.0f, 1.0f);
        cube_list[6].color = glm::vec4( 255.0f/255.0f,  230.0f/255.0f,  59.0f/255.0f, 1.0f);
        cube_list[7].color = glm::vec4( 234.0f/255.0f,  0.0f/255.0f,  52.0f/255.0f, 1.0f);
        cube_list[8].color = glm::vec4( 255.0f/255.0f,  130.0f/255.0f,  42.0f/255.0f, 1.0f);
        cube_list[9].color = glm::vec4( 182.0f/255.0f,  16.0f/255.0f,  245.0f/255.0f, 1.0f);
    }


    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    // bool show_demo_window = true;
    // bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    unsigned int textureID;
glGenTextures(1, &textureID);
glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

    Shader skyboxShader("6.1.skybox.vs", "6.1.skybox.fs");
    Shader shader("6.1.cubemaps.vs", "6.1.cubemaps.fs");

    // load models
    // -----------
    Model ourModel((path+"resources/objects/cyborg/cyborg.obj"));

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    int y_offset= -1;
    float cubeVertices[] = {
        // positions          // texture Coords
        -10.5f,y_offset -0.5f, -10.5f,  0.0f, 0.0f,
         10.5f,y_offset -0.5f, -10.5f,  1.0f, 0.0f,
         10.5f,y_offset +   0.5f, -10.5f,  1.0f, 1.0f,
         10.5f,y_offset +   0.5f, -10.5f,  1.0f, 1.0f,
        -10.5f,y_offset +   0.5f, -10.5f,  0.0f, 1.0f,
        -10.5f,y_offset -0.5f, -10.5f,  0.0f, 0.0f,

        -10.5f,y_offset -0.5f,  0.5f,  0.0f, 0.0f,
         10.5f,y_offset -0.5f,  0.5f,  1.0f, 0.0f,
         10.5f,y_offset +   0.5f,  0.5f,  1.0f, 1.0f,
         10.5f,y_offset +   0.5f,  0.5f,  1.0f, 1.0f,
        -10.5f,y_offset +   0.5f,  0.5f,  0.0f, 1.0f,
        -10.5f,y_offset -0.5f,  0.5f,  0.0f, 0.0f,

        -10.5f,y_offset +   0.5f,  10.5f,  1.0f, 0.0f,
        -10.5f,y_offset +   0.5f, -10.5f,  1.0f, 1.0f,
        -10.5f,y_offset -0.5f, -10.5f,  0.0f, 1.0f,
        -10.5f,y_offset -0.5f, -10.5f,  0.0f, 1.0f,
        -10.5f,y_offset -0.5f,  10.5f,  0.0f, 0.0f,
        -10.5f,y_offset +   0.5f,  10.5f,  1.0f, 0.0f,

         10.5f,y_offset +   0.5f,  10.5f,  1.0f, 0.0f,
         10.5f,y_offset +   0.5f, -10.5f,  1.0f, 1.0f,
         10.5f,y_offset -0.5f, -10.5f,  0.0f, 1.0f,
         10.5f,y_offset -0.5f, -10.5f,  0.0f, 1.0f,
         10.5f,y_offset -0.5f,  10.5f,  0.0f, 0.0f,
         10.5f,y_offset +   0.5f,  10.5f,  1.0f, 0.0f,

        -10.5f,y_offset -0.5f, -10.5f,  0.0f, 1.0f,
         10.5f,y_offset -0.5f, -10.5f,  1.0f, 1.0f,
         10.5f,y_offset -0.5f,  10.5f,  1.0f, 0.0f,
         10.5f,y_offset -0.5f,  10.5f,  1.0f, 0.0f,
        -10.5f,y_offset -0.5f,  10.5f,  0.0f, 0.0f,
        -10.5f,y_offset -0.5f, -10.5f,  0.0f, 1.0f,

        -10.5f,y_offset +  0.5f, -10.5f,  0.0f, 1.0f,
         10.5f,y_offset +   0.5f, -10.5f,  1.0f, 1.0f,
         10.5f,y_offset +   0.5f,  10.5f,  1.0f, 0.0f,
         10.5f,y_offset +   0.5f,  10.5f,  1.0f, 0.0f,
        -10.5f,y_offset +   0.5f,  10.5f,  0.0f, 0.0f,
        -10.5f,y_offset +   0.5f, -10.5f,  0.0f, 1.0f
    };


    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
 // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        unsigned int cubeTexture = loadTexture((path+"resources/textures/container.jpg").c_str());
    vector<std::string> faces
    {
        (path+"resources/textures/skybox/right.jpg"),
        (path+"resources/textures/skybox/left.jpg"),
        (path+"resources/textures/skybox/top.jpg"),
        (path+"resources/textures/skybox/bottom.jpg"),
        (path+"resources/textures/skybox/front.jpg"),
        (path+"resources/textures/skybox/back.jpg")
    };
    unsigned int cubemapTexture = loadCubemap(faces);
    
    shader.use();
    shader.setInt("texture1", 0);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);



    // build and compile our shader program
    // ------------------------------------
    Shader ourShader2("7.4.camera.vs", "7.4.camera.fs");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    glm::vec3 cubePositions[] = {
        glm::vec3( -3.0f,  4.0f,  -4.0f),
        glm::vec3( 3.0f,  5.0f, -15.0f),
        glm::vec3(-4.0f, 2.2f, -2.5f),
        glm::vec3(-3.0f, 2.0f, -12.3f),
        glm::vec3( 2.4f, 0.4f, -3.5f),
        glm::vec3(-3.0f,  3.0f, -7.5f),
        glm::vec3( 3.0f, 2.0f, -2.5f),
        glm::vec3( 5.0f,  2.0f, -2.5f),
        glm::vec3( 3.0f,  0.2f, -1.5f),
        glm::vec3(-4.0f,  1.0f, -1.5f)
    };

    unsigned int VBO2, VAO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); 

    unsigned char *data = stbi_load((path+"resources/textures/block_solid.png").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader2.use(); 
    

    ourShader2.setInt("texture1", 0);

    
    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            static float f = 0.1f;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
      // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        //move model using this
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);
        if(capture==1)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
             glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }






         // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // activate shader
        ourShader2.use();
       
        ourShader2.setMat4("projection", projection);
        ourShader2.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO2);
        for (unsigned int i = 0; i < 10; i++)
        {
            // float timeValue = glfwGetTime();
            // float greenValue = (sin(timeValue));
            ourShader2.setVec4("ourColor",cube_list[i].color);


            glm::mat4 cubes = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            cubes = glm::translate(cubes, cubePositions[i]);
            cubes = glm::translate(cubes, cube_list[i].pos);

            float angle = 20.0f * i;

            cubes = glm::rotate(cubes, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            cubes = glm::rotate(cubes, (float)glfwGetTime() * glm::radians(50.0f * (10 * f)) , glm::vec3(0.5f, 1.0f, 0.0f));  

            ourShader2.setMat4("model", cubes);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }




        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);


        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        // if (show_demo_window)
        //     ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static int counter = 0;

            ImGui::Begin("Editor Window");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Cube obbject selected");    // Edit bools storing our window open/close state
            // ImGui::Checkbox("Anti-aliasing", &show_another_window);

            ImGui::SliderFloat("Time acceleration", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
            
            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                {
                    counter++;
                    counter = counter%10;
                    if(counter == 0)
                    {
                        counter = 1;
                    }
                    camera.Up = glm::vec3(0.0f, 1.0f, 0.0f);
                    camera.Front = glm::vec3(0.0f, 0.0f, -1.0f);
                    prev_up = camera.Up;
                    prev_front = camera.Front;
                    camera.Position = cubePositions[counter]-glm::vec3(0.0f, 0.0f, -3.0f);
                    clear_color.x= cube_list[counter].color[0];
                    clear_color.y= cube_list[counter].color[1];
                    clear_color.z= cube_list[counter].color[2];
                }
            cube_list[counter].color = glm::vec4(clear_color.x, clear_color.y, clear_color.z, 0.0f) ;
            ImGui::SameLine();
            ImGui::Text("Cube %d", counter);

            ImGui::Text("Position = (%.1f,%.1f,%.1f)",camera.Position.x,camera.Position.y,camera.Position.z );
            ImGui::Text("Direction = (%.1f,%.1f,%.1f)",camera.Front.x,camera.Front.y,camera.Front.z );
            ImGui::Text("Application average %.0f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::End();
            
        }

        // // 3. Show another simple window.
        // if (show_another_window)
        // {
        //     ImGui::Begin("Comming soon!", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        //     ImGui::Text("Hello from another window!");
        //     if (ImGui::Button("Close Me"))
        //         show_another_window = false;
        //     ImGui::End();
        // }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        // glViewport(0, 0, display_w, display_h);
        // glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        // glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    	
        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

void processInput(GLFWwindow *window)
{
    int skip =0;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && capture == -1)
    {
        prev_up = camera.Up;
        prev_front = camera.Front;
        capture = -1 * capture;
        skip = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && capture == 1 && skip == 0)
    {
        camera.Up = prev_up ;
        camera.Front = prev_front ;
        capture = -1 * capture;
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
            {
                camera.Position = glm::vec3(6.3f, 4.1f, 8.5f);
                camera.Front = glm::vec3(-0.4f, -0.1f, -0.9f);
                prev_up = camera.Up;
                prev_front = camera.Front;
            }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.


        glViewport(0, 0, width, height);


}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if(capture==-1)
    {
    camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}