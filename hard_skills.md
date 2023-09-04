# OpenGL - Hard Skills

Learning OpenGL is a rewarding but challenging experience. Here are some key points to focus on when diving into OpenGL:

## Basics of Graphics Programming

1. **Coordinate Systems**: Learn how the coordinate systems (object, world, screen, etc.) in OpenGL work.
2. **Vertices and Fragments**: Understand the fundamental building blocks of any shape in OpenGL.

## Shaders

1. **GLSL (OpenGL Shading Language)**: You'll need to learn GLSL to write shaders.
2. **Vertex Shaders**: These manipulate the vertex data of your shape.
3. **Fragment Shaders**: These handle pixel data and are crucial for texturing, lighting, and more.

## Texturing

1. **Texture Loading**: How to load 2D, 3D, and more complex textures into OpenGL.
2. **Texture Units**: How to use multiple textures in shaders.

## Buffer Objects

1. **VBO (Vertex Buffer Object)**: Stores vertex data.
2. **VAO (Vertex Array Object)**: Stores the format of vertex data.
3. **IBO/EBO (Index Buffer Object/Element Buffer Object)**: Stores indices of the vertices to draw.

## Transformations

1. **Model Matrix**: To transform the vertices of individual objects.
2. **View Matrix**: To simulate a camera.
3. **Projection Matrix**: To project the 3D world onto a 2D screen.
4. **Model-View-Projection Matrix (MVP)**: Often these are combined into a single matrix.

## Lighting

1. **Phong Lighting Model**: Understand the basics of ambient, diffuse, and specular lighting.
2. **Normal Mapping**: For more detailed lighting.

## Advanced Features

1. **Framebuffers**: For offscreen rendering, post-processing effects.
2. **Geometry and Tessellation Shaders**: For more advanced geometry manipulation.
3. **Instancing**: For efficient rendering of many objects.

## OpenGL Versions

1. **Compatibility vs Core Profile**: Learn about the differences and why you'd choose one over the other.
2. **Fixed-Function Pipeline vs Programmable Pipeline**: OpenGL has evolved a lot; make sure you're learning the modern approach.

## Debugging and Profiling

1. **OpenGL Errors**: Understand how to debug errors in OpenGL.
2. **Performance**: Use tools to profile and optimize your OpenGL code.

## Libraries and Additional Tools

1. **Windowing Libraries**: GLFW, SDL, etc.
2. **Loading Models**: Libraries like Assimp can load complex models for you.
3. **Image Loading**: Libraries like stb_image, SOIL, or DevIL can load textures.
4. **Math Libraries**: Libraries like GLM can help with mathematical operations.

Remember to always look at examples, and don't hesitate to get your hands dirty with actual coding—it's the best way to learn.
