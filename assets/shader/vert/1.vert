#version 100
//static passthrough
void main()
{
    gl_Position = gl_Vertex;
    //contains the position of the current vertex
    //available since 1.10
}
