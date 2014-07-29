#include "opengl.h"

OpenGL * OpenGL::current_object;
int OpenGL::error_count = 0;

OpenGL::OpenGL()
{
  perspective_mode = 0;

  window.width = 500.0;
  window.height = 500.0;

  // Reset origins
  object_origin = new origin(0.0, 0.0, 0.0);
  object_angle = new origin(0.0, 0.0, 0.0);
  camera_origin = new origin(0.0, 0.0, 0.0);
  camera_angle = new origin(0.0, 0.0, 0.0);

  // Setup static current object
  current_object = this;
}

OpenGL::~OpenGL()
{
  // Clean up origins
  delete object_origin;
  delete object_angle;
  delete camera_origin;
  delete camera_angle;
}

void OpenGL::gl_init(int argc, char **argv)
{
   glutInitWindowSize(window.width, window.height);
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutCreateWindow(get_name(argv));

   // Wireframe mode
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   // Generate the display list from the saved data
   gl_generate_dl();
   // Center the model in the world
   gl_center();

   // Display and run the main loop
   glutDisplayFunc(OpenGL::gl_display);
   glutReshapeFunc(OpenGL::gl_reshape);
   glutKeyboardFunc(OpenGL::gl_keyboard);
   glutSpecialFunc(OpenGL::gl_special_keyboard);
   glutMainLoop();
}

void OpenGL::gl_generate_dl()
{
	const obj_data * object_data = get_object();

  object_list = glGenLists(1);
  glNewList(object_list, GL_COMPILE);

  // Step through the verticies/faces generating the display list
  for (unsigned int i = 0; i < object_data->faces.size(); i++)
  {
      glBegin(GL_POLYGON);
      for (unsigned int h = 0; h < object_data->faces[i]->coords.size(); h++)
      {
          float x = object_data->faces[i]->coords[h]->x;
          float y = object_data->faces[i]->coords[h]->y;
          float z = object_data->faces[i]->coords[h]->z;

          glVertex3f(x, y, z);
      }
      glEnd();
  }

  glEndList();
  glShadeModel(GL_FLAT);

  // Make it black clear color
  glClearColor(0.0, 0.0, 0.0, 0.0);
}

void OpenGL::check_error()
{
  int count = 0;
  GLenum error;

  error_count++;

  while ((error = glGetError()) != GL_NO_ERROR) {
    count++;

    std::cout << error_count <<  ":ERROR: ";

    if (error == GL_INVALID_VALUE)
      std::cout << "GL_INVALID_VALUE";

     std::cout << std::endl;
  }

  if (!count) {
    std::cout << error_count <<  ":NO ERRORS." << std::endl;
  }
}

void OpenGL::gl_camera_view()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (current_object->perspective_mode) {
    gluPerspective(45.0, (GLfloat)current_object->window.width / (GLfloat)current_object->window.height, 1.0, 100.0);
  }
  else {
    glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(0, 0, -2, 0, 0, 2, 0, 1, 0);
}

void OpenGL::gl_reshape(const int w, const int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);

  current_object->window.width = w;
  current_object->window.height = h;

  gl_camera_view();
}

void OpenGL::gl_keyboard(unsigned char key, int x, int y)
{
  switch (key) {
    case 'w':
      current_object->save_obj();
      return;
  }

  switch (key) {
      case 27:
      case 'q':
      case 'Q':
          exit(0);
          break;

      // Reset model and camera
      case 'x':
        gl_camera_view();
        break;

      // Switch to Perspective Mode
      case 'V':
        current_object->perspective_mode = 1;
        gl_camera_view();
        break;
      // Switch to Orthographic Mode
      case 'v':
        current_object->perspective_mode = 0;
        gl_camera_view();
        break;

      // Turn off fog
      case 'f':
        gl_refog(0);
        break;
      // Turn on fog
      case 'F':
        gl_refog(1);
        break;

      // TRANSLATE CAMERA
      // Translate X camera
      case 'd':
        current_object->camera_origin->translate_x_neg();
        break;
      case 'D':
        current_object->camera_origin->translate_x_pos();
        break;

      // Translate Y camera
      case 'c':
        current_object->camera_origin->translate_y_neg();
        break;
      case 'C':
        current_object->camera_origin->translate_y_pos();
        break;

      // Translate Z camera
      case 'i':
        current_object->camera_origin->translate_z_neg();
        break;
      case 'I':
        current_object->camera_origin->translate_z_pos();
        break;

      // ROTATE OBJECT
      // Rotate X object
      case 'p':
        current_object->object_angle->rotate_x_cw();
        break;
      case 'P':
        current_object->object_angle->rotate_x_ccw();
        break;

      // Rotate Y object
      case 'y':
        current_object->object_angle->rotate_y_cw();
        break;
      case 'Y':
        current_object->object_angle->rotate_y_ccw();
        break;

      // Rotate Z object
      case 'r':
        current_object->object_angle->rotate_z_cw();
        break;
      case 'R':
        current_object->object_angle->rotate_z_ccw();
        break;

      // ROTATE CAMERA
      // Rotate X camera
      case 't':
        current_object->camera_angle->rotate_x_cw();
        break;
      case 'T':
        current_object->camera_angle->rotate_x_ccw();
        break;

      // Rotate Y camera
      case 'a':
        current_object->camera_angle->rotate_y_cw();
        break;
      case 'A':
        current_object->camera_angle->rotate_y_ccw();
        break;

      // Rotate Z camera
      case 'l':
        current_object->camera_angle->rotate_z_cw();
        break;
      case 'L':
        current_object->camera_angle->rotate_z_ccw();
        break;

      // TRANSLATE OBJECT
      // Transform Z
      case 'n':
        current_object->object_origin->translate_z_neg();
        break;
      case 'N':
        current_object->object_origin->translate_z_pos();
        break;

  }
  glutPostRedisplay();
}

void OpenGL::gl_special_keyboard(int key, int x, int y)
{
  switch(key) {
    // TRANSLATE OBJECT
    // Translate object Y
    case GLUT_KEY_UP:
       current_object->object_origin->translate_y_pos();
       break;
    case GLUT_KEY_DOWN:
       current_object->object_origin->translate_y_neg();
       break;

    // Translate object X
    case GLUT_KEY_LEFT:
       current_object->object_origin->translate_x_pos();
       break;
    case GLUT_KEY_RIGHT:
       current_object->object_origin->translate_x_neg();
       break;
    }
    glutPostRedisplay();
}

void OpenGL::gl_display()
{
  // Introduce colors
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);

  // // Rotate the camera
  glMatrixMode(GL_PROJECTION);

  glRotatef(current_object->camera_angle->x, 1.0, 0.0, 0.0); // X
  glRotatef(current_object->camera_angle->y, 0.0, 1.0, 0.0); // Y
  glRotatef(current_object->camera_angle->z, 0.0, 0.0, 1.0); // Z

  glMatrixMode(GL_MODELVIEW);

  // Translate the object
  glTranslatef(current_object->object_origin->x, current_object->object_origin->y, current_object->object_origin->z);

  // Translate the camera
  glTranslatef(-current_object->camera_origin->x, -current_object->camera_origin->y, -current_object->camera_origin->z);

  // Rotate the object
  glRotatef(current_object->object_angle->x, 1.0, 0.0, 0.0); // X
  glRotatef(current_object->object_angle->y, 0.0, 1.0, 0.0); // Y
  glRotatef(current_object->object_angle->z, 0.0, 0.0, 1.0); // Z

  // Draw the object
  glCallList(current_object->object_list);
  glutSwapBuffers();

  // ZERO TRANSLATIONS
  // Zero Object
  current_object->object_angle->zero();
  current_object->object_origin->zero();

  // Zero Camera
  current_object->camera_angle->zero();
  current_object->camera_origin->zero();
}

void OpenGL::gl_center()
{
	coord object_center = get_center();

	glTranslatef(-1.0 * (object_center.x), -1.0 * (object_center.y), (-1.0 * (object_center.z)) - 2);
}

void OpenGL::gl_refog(bool mode)
{
  // Turn fog on
  if (mode) {
    glEnable(GL_FOG);

    glFogf(GL_FOG_START, 1.0);
    glFogf(GL_FOG_END, 5.0);
    glFogf(GL_FOG_DENSITY, 0.3);
    glFogi(GL_FOG_MODE, GL_EXP2);
  }
  // Turn fog off
  else {
    glDisable(GL_FOG);
  }
}