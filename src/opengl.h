#ifndef OPENGL_H
#define OPENGL_H

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

#include "object.h"

struct box
{
    float width;
    float height;
};

struct origin
{
    float x, y, z;
    origin(float nx, float ny, float nz) { x = nx; y = ny; z = nz; }

    // Rotate
    // -------  Positive
    void rotate_x_cw() { x += 10.0; }
    void rotate_y_cw() { y += 10.0; }
    void rotate_z_cw() { z += 10.0; }
    // -------  Negative
    void rotate_x_ccw() { x -= 10.0; }
    void rotate_y_ccw() { y -= 10.0; }
    void rotate_z_ccw() { z -= 10.0; }

    // Translate
    // -------  Positive
    void translate_x_pos() { x += 0.1; }
    void translate_y_pos() { y += 0.1; }
    void translate_z_pos() { z += 0.1; }
    // -------  Negative
    void translate_x_neg() { x -= 0.1; }
    void translate_y_neg() { y -= 0.1; }
    void translate_z_neg() { z -= 0.1; }

    void zero() { x = y = z = 0; }
};

class OpenGL : public ModelObject
{
	public:
        OpenGL();
        ~OpenGL();

        // OpenGL Related Functions
        void gl_init(int, char **);

    private:
        GLuint object_list;

        bool perspective_mode;

        origin * object_origin;
        origin * object_angle;

        origin * camera_origin;
        origin * camera_angle;

        box window;

        void gl_generate_dl(); // Generates the display list
        void gl_center(); // Centers the object

        // Trampoline object
        static OpenGL * current_object;

        // Trampoline functions
        static void gl_reshape(int w, int h);
        static void gl_keyboard(unsigned char key, int x, int y);
        static void gl_special_keyboard(int key, int x, int y);
        static void gl_display();
        static void gl_refog(bool mode); // Apply or remove fog

        static int error_count; 
        static void check_error();

        static void gl_camera_view(); // Display the camera in default mode
};

#endif