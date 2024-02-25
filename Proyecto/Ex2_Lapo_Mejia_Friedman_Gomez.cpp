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

// L�mites para el movimiento de la c�mara en el mapa 100x100
const float X_MIN_LIMIT = 0.0f;
const float X_MAX_LIMIT = 100.0f;
const float Z_MIN_LIMIT = 0.0f;
const float Z_MAX_LIMIT = 100.0f;
const float CAMERA_HEIGHT_LIMIT = 2.5f; // Ya definido, incluido aqu� para referencia
bool flashlightEnabled = false; // La linterna est� inicialmente apagada

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
    Shader lightingShader("shaders/vertexShader_materials.vs", "shaders/fragmentShader_materials.fs");  //RENDERIZA ILUMINACION (CASTER LIGHTS) Y MODELOS AFECTADOS POR LA LUZ
    Shader lightCubeShader("shaders/vertexShader_lightcube.vs", "shaders/fragmentShader_lightcube.fs"); //RENDERIZA CUBOS BLANCOS DE LUZ (NO ESTAMOS USANDO)
    Shader ourShader("shaders/shader_exercise16_mloading.vs", "shaders/shader_exercise16_mloading.fs"); //RENDERIZA MODELOS SON AFECCION DE LUZ (SKYMAP)

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

    // coordenadas de los arboles
    glm::vec3 posicionesArboles[] = {

        //glm::vec3(35,0,43),// ARBOL DENTRO DE CASA
        glm::vec3(97,0,67),
        glm::vec3(71,0,95),
        glm::vec3(11,0,03),
        glm::vec3(70,0,53),
        glm::vec3(13,0,79),
        glm::vec3(66,0,53),
        glm::vec3(81,0,47),
        glm::vec3(42,0,69),
        glm::vec3(96,0,66),
        glm::vec3(02,0,87),
        glm::vec3(8,0,57),
        glm::vec3(41,0,74),
        //glm::vec3(33,0,19),// arbol dentro de casa
        glm::vec3(53,0,44),
        glm::vec3(85,0,32),
        glm::vec3(75,0,41),
        glm::vec3(89,0,63),
        glm::vec3(20,0,47),
        glm::vec3(98,0,68),
        glm::vec3(53,0,58),
        glm::vec3(12,0,95),
        glm::vec3(80,0,51),
        //glm::vec3(33,0,19),
        glm::vec3(64,0,70),
        glm::vec3(03,0,25),
        glm::vec3(05,0,78),
        glm::vec3(48,0,29),
        glm::vec3(54,0,01),
        glm::vec3(06,0,45),
        glm::vec3(59,0,30),
        glm::vec3(92,0,20),
        glm::vec3(80,0,80),
        glm::vec3(67,0,17),
        glm::vec3(24,0,79),
        glm::vec3(84,0,10),
        glm::vec3(03,0,45),
        glm::vec3(10,0,06),
        glm::vec3(16,0,12),
        glm::vec3(17,0,60),
        glm::vec3(53,0,85),
        glm::vec3(36,0,70),
        glm::vec3(98,0,53),
        glm::vec3(04,0,94),
        glm::vec3(48,0,46),
        glm::vec3(85,0,56),
        glm::vec3(35,0,63),
        glm::vec3(56,0,41),
        glm::vec3(74,0,39),
        glm::vec3(14,0,50),
    };

    //ARREGLO DE POSICIONES PARA RENDEREIZA EL SUELO
    // Ajustar el tama�o de este array para contener 10,000 posiciones (100 filas de 100)
    glm::vec3 cubePositions[10000];
    // Llenar el array con posiciones para crear 100 filas de 100 cubos cada una
    int index = 0; // �ndice para llenar el arreglo
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


    //VAO PARA LOS CUBOS USADOS EN EL SUELO
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

    //VAO CUBOS DE LUZ BLANCA
    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //CARGA DE TEXTURA
    unsigned int diffuseMap1 = loadTexture("textures/suelo.jpg"); //TEXTURA PARA EL SUELO

    // shader configuration
    // --------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse1", 0); // Texture unit 0

    //LLENAR ARREGLOS CON POSICIONES AL AZAR PARA LUCES Y ARBOLES
    // Antes del bucle de renderizado
    int numLuces = 7; // Para editar la cantidad de luces, se debe de igual editar el numero en el FS
    std::vector<glm::vec3> pointLightPositions;
    int tamanoPosicionesArboles = sizeof(posicionesArboles) / sizeof(posicionesArboles[0]);

    srand(static_cast<unsigned int>(glfwGetTime())); // Inicializa la semilla de aleatoriedad

    for (int i = 0; i < numLuces; ++i) {
        float x = static_cast<float>(rand() % 100); // Genera posici�n x aleatoria entre 0 y 99
        float z = static_cast<float>(rand() % 100); // Genera posici�n z aleatoria entre 0 y 99
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

        //PROPIEDADES DE LA LUZ 
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f);//PROPIEDADES DEL MATERIAL

        // 1.PROPIEDADES DE LA LINTERNA
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);

        if (flashlightEnabled) { //LOGICA DE ENCENDIDO Y APAGADO
            lightingShader.setVec3("spotLight.ambient", 0.1f, 0.1f, 0.1f);
            lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            lightingShader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
        }
       else {
           lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
           lightingShader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f); // Linterna apagada
           lightingShader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f); 
        }

        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.01f);
        lightingShader.setFloat("spotLight.quadratic", 0.017f);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(25.5f)));
        
        // 2. PROPIEDADES PARA LAS LUCES DIRECCIONALES
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

        //RENDERIZAR MODELOS DE LOS ARBOLES EN LA ESCENA
        //Configuraci�n del shader para los cubos
        glActiveTexture(GL_TEXTURE0); // Cambia a la unidad de textura de los cubos
        glBindTexture(GL_TEXTURE_2D, diffuseMap1);
        glBindVertexArray(cubeVAO);
        // Modificar el bucle de dibujo para iterar sobre los 10,000 cubos
        for (unsigned int i = 0; i < 10000; i++) {
            // Transformaci�n del mundo para este cubo
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            lightingShader.setMat4("model", model);
            // Dibuja el cubo
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //RENDERIZAR MODELOS DE LOS ARBOLES EN LA ESCENA
        for (int i = 0; i < tamanoPosicionesArboles; ++i) {
            model = glm::mat4(1.0f); // Matriz de modelo general para el �rbol
            model = glm::translate(model, posicionesArboles[i]); // Posiciona el modelo en la ubicaci�n aleatoria
            model = glm::scale(model, glm::vec3(0.2f)); // Ajusta el tama�o seg�n sea necesario

            if (i % 2 == 0) {
                // Para �ndices pares, usa el modelo de arbolViejo
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Si es necesario rotar el modelo
                lightingShader.setMat4("model", model);
                arbolViejo.Draw(lightingShader);
            }
            else {
                // Para �ndices impares, usa el modelo de arbol
                // No se aplica rotaci�n adicional, ajusta seg�n sea necesario
                model = glm::scale(model, glm::vec3(10.0f)); // Ajusta el escalamiento si es diferente para este modelo
                lightingShader.setMat4("model", model);
                arbol.Draw(lightingShader);
            }
        }
       

        // Imprimir posiciones de los �rboles en la consola
        std::cout << "Posiciones actual jugador" << std::endl;
        for (int i = 0; i < currentFrame; ++i) {
            std::cout << "Posicion : " << camera.Position.x << "," << camera.Position.y<<","<<camera.Position.z << std::endl;
        }

        //RENDERIZAR MODELO DE LA LINTERNA
        // Configura la transformaci�n de la linterna relativa a la c�mara
        glm::mat4 modelLinterna = glm::mat4(1.0f);
        // Posici�n relativa a la c�mara, ajusta estos valores seg�n sea necesario
        modelLinterna = glm::translate(modelLinterna, glm::vec3(0.35f, -0.40f, -0.75f));
        modelLinterna = glm::rotate(modelLinterna, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelLinterna = glm::scale(modelLinterna, glm::vec3(0.15f));
        // Aplica la inversa de la matriz de vista para colocar la linterna en el espacio de la c�mara
        modelLinterna = glm::inverse(view) * modelLinterna;
        lightingShader.setMat4("model", modelLinterna);
        linterna.Draw(lightingShader);

        //RENDERIZAR MODELO DE LA CASA
        glm::mat4 modelCasa = glm::mat4(1.0f);
        modelCasa = glm::translate(modelCasa, glm::vec3(30.0f, 0.35f, 30.0f)); // translate it down so it's at the center of the scene
        modelCasa = glm::rotate(modelCasa, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelCasa = glm::scale(modelCasa, glm::vec3(2.0f));	// it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", modelCasa);
        casa.Draw(lightingShader);

        // RENDERIZAR CUBOS DE LAS LUCES  ///NO HACE FALTA QUE SE VEAN LOS CUBOS DE LUZ
        /*lightCubeShader.use();
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
        }*/

        //RENDERIZAR MODELO DE LAS ESTRELLAS (SKYMAP)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        glm::mat4 modelStar = glm::mat4(1.0f);
        modelStar = glm::translate(modelStar, glm::vec3(30.0f, 0.35f, 30.0f)); // translate it down so it's at the center of the scene
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

    float cameraSpeed = 20.0f * deltaTime; // Ajusta esto seg�n la velocidad deseada
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

    // Aplicar la nueva posici�n
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
        // Configura las propiedades de la linterna para que est� "encendida"
        shader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f); // luz difusa intensa
        shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f); // luz especular intensa
    }
    else {
        // Configura las propiedades de la linterna para simular que est� "apagada"
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