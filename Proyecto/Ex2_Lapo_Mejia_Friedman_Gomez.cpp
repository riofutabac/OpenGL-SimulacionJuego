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


glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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



    // configure global opengl state
    //Exercise 11 Task 3
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    //MODELOS
    // build and compile shaders
    // -------------------------
    Shader ourShader("shaders/shader_exercise16_mloading.vs", "shaders/shader_exercise16_mloading.fs");

    // load models
 
    Model arbolViejo("C:/Users/Alexis Lapo/Documents/ProyectoCompuGrafica/Proyecto/Proyecto/model/tree/scene.gltf");
    Model arbol("C:/Users/Alexis Lapo/Documents/ProyectoCompuGrafica/Proyecto/Proyecto/model/oldtree/scene.gltf");
    Model linterna("C:/Users/Alexis Lapo/Documents/ProyectoCompuGrafica/Proyecto/Proyecto/model/linterna/scene.gltf");
    Model mano("C:/Users/Alexis Lapo/Documents/ProyectoCompuGrafica/Proyecto/Proyecto/model/mano/scene.gltf");
    Model casa("C:/Users/Alexis Lapo/Documents/ProyectoCompuGrafica/Proyecto/Proyecto/model/casa/scene.gltf");
  

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("shaders/vertexShader_materials.vs", "shaders/fragmentShader_materials.fs");
    Shader lightCubeShader("shaders/vertexShader_lightcube.vs", "shaders/fragmentShader_lightcube.fs");

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

    //SUELOOOOOOOOOOOOOOO
    // Ajustar el tamaño de este array para contener 10,000 posiciones (100 filas de 100)

    glm::vec3 cubePositions[10000];

    // Llenar el array con posiciones para crear 100 filas de 100 cubos cada una
    int index = 0; // Índice para llenar el arreglo
    for (int j = 0; j < 100; j++) { // 100 filas
        for (int i = 0; i < 100; i++) { // 100 cubos por fila
            cubePositions[index++] = glm::vec3(i, 0.0f, j); // Ajusta 'i' y 'j' para cambiar la columna y la fila, respectivamente

        }
    }
    std::cout << "Contenido de la posición 4999: (" << cubePositions[4999].x << ", " << cubePositions[4999].y << ", " << cubePositions[4999].z << ")" << std::endl;


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

    //Exerice 14 Task 2
   //texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Exercise 14 Task 2
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Exercise 14 Task 2
   // load textures (we now use a utility function to keep the code more organized)
       // -----------------------------------------------------------------------------

    unsigned int diffuseMap1 = loadTexture("textures/suelo.jpg");

    // shader configuration
    // --------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse1", 0); // Texture unit 0




    float lightSpeed = 1.0f; // Velocidad de movimiento de la luz
    glm::vec3 lightDirection(1.0f, 0.0f, 0.0f); // Dirección de movimiento de la luz


    // Antes del bucle de renderizado
    int numArboles = 50; // Número de árboles
    std::vector<glm::vec3> posicionesArboles;

    srand(static_cast<unsigned int>(glfwGetTime())); // Inicializa la semilla de aleatoriedad

    for (int i = 0; i < numArboles; ++i) {
        float x = static_cast<float>(rand() % 100); // Genera posición x aleatoria entre 0 y 99
        float z = static_cast<float>(rand() % 100); // Genera posición z aleatoria entre 0 y 99
        posicionesArboles.push_back(glm::vec3(x, 0.0f, z)); // Asume que el suelo está en y = 0
    }

    // Imprimir posiciones de los árboles en la consola
    std::cout << "Posiciones de los árboles:" << std::endl;
    for (int i = 0; i < numArboles; ++i) {
        std::cout << "Árbol " << i << ": (" << posicionesArboles[i].x << ", " << posicionesArboles[i].y << ", " << posicionesArboles[i].z << ")" << std::endl;
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


        //Fuente de luz que se mueve a lo largo del tiempo
        float time = glfwGetTime();
        float radius = 7.5; // Incrementa el radio para abarcar todos los cubos
        float lightX = sin(time * lightSpeed) * radius;
        float lightZ = cos(time * lightSpeed) * radius;
        // Ajusta el centro del círculo si es necesario, por ejemplo, moviéndolo a la mitad entre el primer y el último cubo
        float centerX = 4.0f; // Aproximadamente el punto medio entre el primer y el último cubo
        lightPos = glm::vec3(lightX + centerX, 1.0f, lightZ);


        // input
        // -----
        processInput(window);
        //  std::cout << "Posición de la cámara: (" << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z << ")" << std::endl;

          // render
          // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Activación del shader para el modelo y configuración de matrices de transformación

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();



        // Configuración del shader para los cubos

        lightingShader.use();
        glActiveTexture(GL_TEXTURE0); // Cambia a la unidad de textura de los cubos
        glBindTexture(GL_TEXTURE_2D, diffuseMap1);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        //Exercise 15 Task 4
        lightingShader.setVec3("light.position", camera.Position);
        lightingShader.setVec3("light.direction", camera.Front);
        lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(10.5f)));
        lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(25.5f)));//radio linterna
        lightingShader.setVec3("viewPos", camera.Position);



        // light properties
        lightingShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
        // we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
        // each environment and lighting type requires some tweaking to get the best out of your environment.	 
        lightingShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        //Exercise 15 Task 2
        lightingShader.setFloat("light.constant", 1.0f);
        lightingShader.setFloat("light.linear", 0.05f);//alcance linterna, para mayor alcance reducimos valores
        lightingShader.setFloat("light.quadratic", 0.009f);//alcance linterna, para mayor alcance reducimos valores



        // material properties
        lightingShader.setFloat("material.shininess", 32.0f);

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


        // Transformación y dibujo del modelo
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // Dentro del bucle de renderización
        for (int i = 0; i < numArboles; ++i) {
            glm::mat4 model = glm::mat4(1.0f); // Matriz de modelo general para el árbol
            model = glm::translate(model, posicionesArboles[i]); // Posiciona el modelo en la ubicación aleatoria
            model = glm::scale(model, glm::vec3(0.2f)); // Ajusta el tamaño según sea necesario

            if (i % 2 == 0) {
                // Para índices pares, usa el modelo de arbolViejo
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Si es necesario rotar el modelo
                ourShader.setMat4("model", model);
                arbolViejo.Draw(ourShader);
            }
            else {
                // Para índices impares, usa el modelo de arbol
                // No se aplica rotación adicional, ajusta según sea necesario
                model = glm::scale(model, glm::vec3(10.0f)); // Ajusta el escalamiento si es diferente para este modelo
                ourShader.setMat4("model", model);
                arbol.Draw(ourShader);
            }
        }



        // Configura la transformación de la mano relativa a la cámara

        glm::mat4 modelLinterna = glm::mat4(1.0f);
        // Posición relativa a la cámara, ajusta estos valores según sea necesario
        modelLinterna = glm::translate(modelLinterna, glm::vec3(0.35f, -0.40f, -0.75f));
        modelLinterna = glm::rotate(modelLinterna, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelLinterna = glm::scale(modelLinterna, glm::vec3(0.15f));

        // Aplica la inversa de la matriz de vista para colocar la mano en el espacio de la cámara
        modelLinterna = glm::inverse(view) * modelLinterna;

        // Configura el shader y dibuja la mano
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view); // Aquí estás pasando la matriz de vista, pero la transformación real ya ha sido aplicada a modelMano
        ourShader.setMat4("model", modelLinterna);
        linterna.Draw(ourShader);

        //CASA
        glm::mat4 modelCasa = glm::mat4(1.0f);
        modelCasa = glm::translate(modelCasa, glm::vec3(30.0f, 0.35f, 30.0f)); // translate it down so it's at the center of the scene
        modelCasa = glm::rotate(modelCasa, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelCasa = glm::scale(modelCasa, glm::vec3(2.0f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", modelCasa);
        casa.Draw(ourShader);



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