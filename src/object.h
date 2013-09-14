#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

struct coord
{
    int id;

    float x;
    float y;
    float z;
};

struct face
{
    std::vector <coord *> coords;
};

struct obj_data
{
    std::vector <coord *> vertices;
    int vert_count;

    std::vector <face *> faces;
    int face_count;
};

class ModelObject {
	obj_data * object;

	public:
		ModelObject  ();
        ~ModelObject ();
		void read_obj(const char *);
        void save_obj();
        void print_data();
        void generate_gl();

    private:
        void print_coord(const coord *);
        void print_face(const face *);
};