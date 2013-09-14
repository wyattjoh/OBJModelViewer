#include "object.h"

int main(int argc, char **argv) 
{  
    ModelObject * object = new ModelObject();

    object->read_obj(argv[1]);

    object->save_obj();

    delete object;

    return 0;  
}