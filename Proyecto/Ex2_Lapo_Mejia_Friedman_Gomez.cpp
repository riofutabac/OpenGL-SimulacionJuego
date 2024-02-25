#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>

#include <vector>

// Estructura para almacenar las transformaciones de cada modelo


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void setFlashlightProperties(Shader& shader, bool enabled);


unsigned int loadTexture(const char* path);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Límites para el movimiento de la cámara en el mapa 100x100
const float X_MIN_LIMIT = 0.0f;
const float X_MAX_LIMIT = 100.0f;
const float Z_MIN_LIMIT = 0.0f;
const float Z_MAX_LIMIT = 100.0f;
const float CAMERA_HEIGHT_LIMIT = 2.5f; // Ya definido, incluido aquí para referencia
bool flashlightEnabled = false; // La linterna está inicialmente apagada


Camera camera(glm::vec3(50.0f, 2.5f, 49.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Alexis Lapo CI:1723053151", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
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

    glEnable(GL_DEPTH_TEST);

    //MODELOS
    // load models
    Model estrellas("C:/model/cielo/scene.gltf");
    Model arbolViejo("C:/model/tree/scene.gltf");
    Model arbol("C:/model/oldtree/scene.gltf");
    Model linterna("C:/model/linterna/scene.gltf");
    Model casa("C:/model/casa/scene.gltf");

    // build and compile our shader program
    // ------------------------------------
    Shader lightingShader("shaders/vertexShader_materials.vs", "shaders/fragmentShader_materials.fs");
    Shader lightingShader1("shaders/vertexShader_materials.vs", "shaders/fragmentShader_materials1.fs");
    Shader lightCubeShader("shaders/vertexShader_lightcube.vs", "shaders/fragmentShader_lightcube.fs");
    Shader ourShader("shaders/shader_exercise16_mloading.vs", "shaders/shader_exercise16_mloading.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------


    float vertices[] = {
        // Coordenadas XYZ         // Normales XYZ          // Coordenadas de textura UV
       // Cara trasera
       -0.5f, -0.5f, -0.5f,       0.0f,  0.0f, -1.0f,       0.5343f, 0.5f,    // Esquina inferior izquierda
        0.5f, -0.5f, -0.5f,       0.0f,  0.0f, -1.0f,       0.7291f, 0.3264f,    // Esquina inferior derecha    
        0.5f,  0.5f, -0.5f,       0.0f,  0.0f, -1.0f,       0.7291f, 0.3264f, // Esquina superior derecha              
        0.5f,  0.5f, -0.5f,       0.0f,  0.0f, -1.0f,       0.7291f, 0.3264f, // Esquina superior derecha
       -0.5f,  0.5f, -0.5f,       0.0f,  0.0f, -1.0f,       0.5343f, 0.5f, // Esquina superior izquierda
       -0.5f, -0.5f, -0.5f,       0.0f,  0.0f, -1.0f,       0.5343f, 0.5f,    // Esquina inferior izquierda                

       // Cara frontal
       -0.5f, -0.5f,  0.5f,       0.0f,  0.0f,  1.0f,       0.5343f, 0.5f,    // Esquina inferior izquierda
        0.5f,  0.5f,  0.5f,       0.0f,  0.0f,  1.0f,       0.7291f, 0.3264f, // Esquina superior derecha
        0.5f, -0.5f,  0.5f,       0.0f,  0.0f,  1.0f,       0.7291f, 0.3264f,    // Esquina inferior derecha        
        0.5f,  0.5f,  0.5f,       0.0f,  0.0f,  1.0f,       0.7291f, 0.3264f, // Esquina superior derecha
       -0.5f, -0.5f,  0.5f,       0.0f,  0.0f,  1.0f,       0.5343f, 0.5f,    // Esquina inferior izquierda
       -0.5f,  0.5f,  0.5f,       0.0f,  0.0f,  1.0f,       0.5343f, 0.3264f, // Esquina superior izquierda        

       // Cara izquierda
       -0.5f,  0.5f,  0.5f,       -1.0f, 0.0f,  0.0f,       0.7291f, 0.3264f, // Esquina superior derecha
       -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f,  0.0f,       0.925f, 0.5f,      // Esquina inferior izquierda
       -0.5f,  0.5f, -0.5f,       -1.0f, 0.0f,  0.0f,       0.925f, 0.3264f,   // Esquina superior izquierda       
       -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f,  0.0f,       0.925f, 0.5f,      // Esquina inferior izquierda
       -0.5f,  0.5f,  0.5f,       -1.0f, 0.0f,  0.0f,       0.7291f, 0.3264f, // Esquina superior derecha
       -0.5f, -0.5f,  0.5f,       -1.0f, 0.0f,  0.0f,       0.7291f, 0.5f,    // Esquina inferior derecha

       // Cara derecha
        0.5f,  0.5f,  0.5f,       1.0f,  0.0f,  0.0f,       0.5343f, 0.3264f, // Esquina superior izquierda
        0.5f,  0.5f, -0.5f,       1.0f,  0.0f,  0.0f,       0.3385f, 0.3264f, // Esquina superior derecha      
        0.5f, -0.5f, -0.5f,       1.0f,  0.0f,  0.0f,       0.3385f, 0.5f,    // Esquina inferior derecha          
        0.5f, -0.5f, -0.5f,       1.0f,  0.0f,  0.0f,       0.3385f, 0.5f,    // Esquina inferior derecha
        0.5f, -0.5f,  0.5f,       1.0f,  0.0f,  0.0f,       0.5343f, 0.5f,    // Esquina inferior izquierda
        0.5f,  0.5f,  0.5f,       1.0f,  0.0f,  0.0f,       0.5343f, 0.3264f, // Esquina superior izquierda

        // Cara inferior          
        -0.5f, -0.5f, -0.5f,       0.0f, -1.0f,  0.0f,       0.5343f, 0.7435f, // Esquina superior derecha
         0.5f, -0.5f,  0.5f,       0.0f, -1.0f,  0.0f,       0.7291f, 0.5f,    // Esquina inferior izquierda
         0.5f, -0.5f, -0.5f,       0.0f, -1.0f,  0.0f,       0.7291f, 0.7435f, // Esquina superior izquierda        
         0.5f, -0.5f,  0.5f,       0.0f, -1.0f,  0.0f,       0.7291f, 0.5f,    // Esquina inferior izquierda
        -0.5f, -0.5f, -0.5f,       0.0f, -1.0f,  0.0f,       0.5343f, 0.7435f, // Esquina superior derecha
        -0.5f, -0.5f,  0.5f,       0.0f, -1.0f,  0.0f,       0.5343f, 0.5f,    // Esquina inferior derecha

        // Cara superior
        -0.5f,  0.5f, -0.5f,       0.0f,  1.0f,  0.0f,       0.5343f, 0.5f,  // Esquina superior izquierda
         0.5f,  0.5f, -0.5f,       0.0f,  1.0f,  0.0f,       0.925f, 0.5f,  // Esquina superior derecha
         0.5f,  0.5f,  0.5f,       0.0f,  1.0f,  0.0f,       0.7291f, 0.3264f, // Esquina inferior derecha                 
         0.5f,  0.5f,  0.5f,       0.0f,  1.0f,  0.0f,       0.7291f, 0.3264f, // Esquina inferior derecha
        -0.5f,  0.5f,  0.5f,       0.0f,  1.0f,  0.0f,       0.5343f, 0.5f,  // Esquina inferior izquierda  
        -0.5f,  0.5f, -0.5f,       0.0f,  1.0f,  0.0f,       0.5343f, 0.3264f  // Esquina superior izquierda  
    };

    //ARREGLO DE POSICIONES PARA RENDEREIZA EL SUELO
    // Ajustar el tamaño de este array para contener 10,000 posiciones (100 filas de 100)
    glm::vec3 cubePositions[10000];
    // Llenar el array con posiciones para crear 100 filas de 100 cubos cada una
    int index = 0; // Índice para llenar el arreglo
    for (int j = 0; j < 100; j++) { // 100 filas
        for (int i = 0; i < 100; i++) { // 100 cubos por fila
            cubePositions[index++] = glm::vec3(i, 0.0f, j); // Ajusta 'i' y 'j' para cambiar la columna y la fila, respectivamente
        }
    }


    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

   //texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int diffuseMap1 = loadTexture("textures/suelo.jpg");

    // shader configuration
    // --------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse1", 0); // Texture unit 0

    float lightSpeed = 1.0f; // Velocidad de movimiento de la luz
    glm::vec3 lightDirection(1.0f, 0.0f, 0.0f); // Dirección de movimiento de la luz


    //LLENAR ARREGLOS CON POSICIONES AL AZAR PARA LUCES Y ARBOLES
    // Antes del bucle de renderizado
    int numArboles = 50; // Número de árboles
    int numLuces = 7; // Para editar la cantidad de luces, se debe de igual editar el numero en el FS
    std::vector<glm::vec3> posicionesArboles;
    std::vector<glm::vec3> pointLightPositions;

    srand(static_cast<unsigned int>(glfwGetTime())); // Inicializa la semilla de aleatoriedad

    for (int i = 0; i < numArboles; ++i) {
        float x = static_cast<float>(rand() % 100); // Genera posición x aleatoria entre 0 y 99
        float z = static_cast<float>(rand() % 100); // Genera posición z aleatoria entre 0 y 99
        posicionesArboles.push_back(glm::vec3(x, 0.0f, z)); // Asume que el suelo está en y = 0
    }

    for (int i = 0; i < numLuces; ++i) {
        float x = static_cast<float>(rand() % 100); // Genera posición x aleatoria entre 0 y 99
        float z = static_cast<float>(rand() % 100); // Genera posición z aleatoria entre 0 y 99
        pointLightPositions.push_back(glm::vec3(x, 10.0f, z)); // La altura 'y' es constante para todas las luces
    }

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

          // render
          // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //PROPIEDADESDE LA LUZ PARA LA LINTERNA
        
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();

        //Exercise 15 Task 4
        lightingShader.setVec3("light.position", camera.Position);
        lightingShader.setVec3("light.direction", camera.Front);
        lightingShader.setVec3("viewPos", camera.Position);

        // light properties
        if (flashlightEnabled) {
            lightingShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
            // we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
            // each environment and lighting type requires some tweaking to get the best out of your environment.	 
            lightingShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
            lightingShader.setVec3("spotlight.specular", 0.0f, 0.0f, 0.0f);
        }
       else {
           lightingShader.setVec3("light.ambient", 0.0f, 0.0f, 0.0f);
           lightingShader.setVec3("light.diffuse", 0.0f, 0.0f, 0.0f); // Linterna apagada
           lightingShader.setVec3("light.specular", 0.0f, 0.0f, 0.0f); // Linterna apagada
        }

        //Exercise 15 Task 2
        lightingShader.setFloat("light.constant", 1.0f);
        lightingShader.setFloat("light.linear", 0.01f);
        lightingShader.setFloat("light.quadratic", 0.017f);
        lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(10.5f)));
        lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(25.5f)));


        // material properties
        lightingShader.setFloat("material.shininess", 32.0f);

    
        // LUCES
        glm::vec3 ambient(0.05f, 0.05f, 0.05f);
        glm::vec3 diffuse(0.8f, 0.8f, 0.8f);
        glm::vec3 specular(1.0f, 1.0f, 1.0f);
        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;

        for (size_t i = 0; i < numLuces; ++i) {
            std::string baseName = "pointLights[" + std::to_string(i) + "].";
            lightingShader.setVec3(baseName + "position", pointLightPositions[i]);
            lightingShader.setVec3(baseName + "ambient", ambient);
            lightingShader.setVec3(baseName + "diffuse", diffuse);
            lightingShader.setVec3(baseName + "specular", specular);
            lightingShader.setFloat(baseName + "constant", constant);
            lightingShader.setFloat(baseName + "linear", linear);
            lightingShader.setFloat(baseName + "quadratic", quadratic);
        }

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // Configuración del shader para los cubos
        glActiveTexture(GL_TEXTURE0); // Cambia a la unidad de textura de los cubos
        glBindTexture(GL_TEXTURE_2D, diffuseMap1);

        glBindVertexArray(cubeVAO);
        // Modificar el bucle de dibujo para iterar sobre los 10,000 cubos
        for (unsigned int i = 0; i < 10000; i++) {
            // Transformación del mundo para este cubo
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            lightingShader.setMat4("model", model);

            // Dibuja el cubo
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // RENDERIZAR LOS ARBOLES EN LA ESCENA
        for (int i = 0; i < numArboles; ++i) {
            model = glm::mat4(1.0f); // Matriz de modelo general para el árbol
            model = glm::translate(model, posicionesArboles[i]); // Posiciona el modelo en la ubicación aleatoria
            model = glm::scale(model, glm::vec3(0.2f)); // Ajusta el tamaño según sea necesario

            if (i % 2 == 0) {
                // Para índices pares, usa el modelo de arbolViejo
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Si es necesario rotar el modelo
                lightingShader.setMat4("model", model);
                arbolViejo.Draw(lightingShader);
            }
            else {
                // Para índices impares, usa el modelo de arbol
                // No se aplica rotación adicional, ajusta según sea necesario
                model = glm::scale(model, glm::vec3(10.0f)); // Ajusta el escalamiento si es diferente para este modelo
                lightingShader.setMat4("model", model);
                arbol.Draw(lightingShader);
            }
        }

        //MODELO DE LA LINTERNA
        // Configura la transformación de la linterna relativa a la cámara
        glm::mat4 modelLinterna = glm::mat4(1.0f);
        // Posición relativa a la cámara, ajusta estos valores según sea necesario
        modelLinterna = glm::translate(modelLinterna, glm::vec3(0.35f, -0.40f, -0.75f));
        modelLinterna = glm::rotate(modelLinterna, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelLinterna = glm::scale(modelLinterna, glm::vec3(0.15f));
        // Aplica la inversa de la matriz de vista para colocar la linterna en el espacio de la cámara
        modelLinterna = glm::inverse(view) * modelLinterna;
        lightingShader.setMat4("model", modelLinterna);
        linterna.Draw(lightingShader);

        //MODELO DE LA CASA
        glm::mat4 modelCasa = glm::mat4(1.0f);
        modelCasa = glm::translate(modelCasa, glm::vec3(30.0f, 0.35f, 30.0f)); // translate it down so it's at the center of the scene
        modelCasa = glm::rotate(modelCasa, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelCasa = glm::scale(modelCasa, glm::vec3(2.0f));	// it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", modelCasa);
        casa.Draw(lightingShader);

        // RENDERIZAR CUBOS DE LAS LUCES
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < numLuces; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //MODELO DE LAS ESTRELLAS
        ourShader.use();
        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100000.0f);
        //glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        glm::mat4 modelStar = glm::mat4(1.0f);
        modelStar = glm::translate(modelStar, glm::vec3(30.0f, 0.35f, 30.0f)); // translate it down so it's at the center of the scene
        //modelStar = glm::rotate(modelStar, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelStar = glm::scale(modelStar, glm::vec3(1000.0f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", modelStar);
        estrellas.Draw(ourShader);

        // Intercambio de buffers y eventos de GLFW
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 20.0f * deltaTime; // Ajusta esto según la velocidad deseada
    glm::vec3 newPosition = camera.Position;

    // Copia temporal del vector Front sin la componente y para el movimiento
    glm::vec3 frontHorizontal = glm::normalize(glm::vec3(camera.Front.x, 0.0f, camera.Front.z));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        newPosition += frontHorizontal * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        newPosition -= frontHorizontal * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        newPosition -= glm::normalize(glm::cross(frontHorizontal, camera.Up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        newPosition += glm::normalize(glm::cross(frontHorizontal, camera.Up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        flashlightEnabled = !flashlightEnabled; // Toggle estado de la linterna

    // Restricciones de movimiento en el eje Y ya aplicadas previamente
    // Restricciones en los ejes X y Z
    newPosition.x = glm::clamp(newPosition.x, X_MIN_LIMIT, X_MAX_LIMIT);
    newPosition.z = glm::clamp(newPosition.z, Z_MIN_LIMIT, Z_MAX_LIMIT);
    newPosition.y = glm::clamp(newPosition.y, CAMERA_HEIGHT_LIMIT, CAMERA_HEIGHT_LIMIT); // Mantener la altura constante

    // Aplicar la nueva posición
    camera.Position = newPosition;
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
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void setFlashlightProperties(Shader& shader, bool enabled) {
    if (enabled) {
        // Configura las propiedades de la linterna para que esté "encendida"
        shader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f); // luz difusa intensa
        shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f); // luz especular intensa
    }
    else {
        // Configura las propiedades de la linterna para simular que está "apagada"
        shader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f); // luz difusa apagada
        shader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f); // luz especular apagada
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

//Exercise 14 Task 2
// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
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