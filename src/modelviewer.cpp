#include "opengl.h"

int main(int argc, char **argv) 
{  
    OpenGL * gl_object = new OpenGL;

    gl_object->read_obj(argv[1]);

    gl_object->gl_init(argc, argv);

    delete gl_object;

    return 0;  
}