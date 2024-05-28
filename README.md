
# Proyecto Grupo6 de OpenGL

Este proyecto es una demostración de cómo utilizar OpenGL para crear escenas interactivas en 3D con iluminación y múltiples modelos. Incluye la implementación de una cámara móvil, modelos de árboles, una casa, una linterna, zombies, y un cementerio con un cielo estrellado.

## Características

- **Iluminación**: Implementación de luces puntuales y una linterna que puede ser encendida y apagada.
- **Modelos 3D**: Carga y renderizado de modelos 3D usando la librería `Assimp`.
- **Interacción**: Control de la cámara mediante el teclado y el mouse para explorar la escena.
- **Texturas**: Uso de texturas para añadir detalle a los modelos y el terreno.
- **Colisiones**: Detección básica de colisiones para evitar que la cámara atraviese objetos como árboles.

## Dependencias

Para ejecutar este proyecto, necesitarás las siguientes bibliotecas:
- [GLFW](https://www.glfw.org/) para la creación de la ventana y el manejo de la entrada/salida.
- [Glad](https://glad.dav1d.de/) para la gestión de extensiones OpenGL.
- [GLM](https://glm.g-truc.net/0.9.9/index.html) para operaciones matemáticas.
- [Assimp](http://www.assimp.org/) para la carga de modelos.
- [stb_image.h](https://github.com/nothings/stb) para la carga de texturas.

## Configuración y compilación

### Clonar el repositorio

```bash
git clone 
cd proyecto_grupo6_opengl
```

### Compilación con CMake

```bash
mkdir build
cd build
cmake ..
make
```

### Ejecutar

Una vez compilado el proyecto, puedes ejecutar el binario generado:

```bash
./Grupo6
```

## Controles

- **WASD**: Movimiento de la cámara.
- **Mouse**: Controlar la dirección de la cámara.
- **F**: Encender/apagar la linterna.
- **Escape**: Cerrar la aplicación.

## Créditos

- Modelos 3D obtenidos de [Modelos3D.com](https://modelos3d.com).
- Texturas de [Texturas.com](https://texturas.com).
- Desarrollado por los estudiantes de Escuela Politecnica Nacional.


