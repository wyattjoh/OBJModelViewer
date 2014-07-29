#ifndef OBJECT_H
#define OBJECT_H

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

    // Coordinate data
    coord center;

    char * obj_name;
};

class ModelObject {
	public:
        // static ModelObject * current_object;

		ModelObject  ();
        ~ModelObject ();
		void read_obj(const char *);
        void save_obj();

        coord get_center() { return object->center; }

        const obj_data * get_object() { return object; }
        const char * get_name(char ** argv);

    private:
        obj_data * object;

        void process_object(); // Find the min/max and center

        void print_coord(const coord *);
        void print_face(const face *);
        void print_data();
};

#endif