#include "object.h"

ModelObject::ModelObject()
{
    object = new obj_data;
}

ModelObject::~ModelObject ()
{
    for (unsigned int i = 0; i < object->vertices.size(); i++)
    {
        delete object->vertices[i];
    }

    for (unsigned int i = 0; i < object->faces.size(); i++)
    {
        delete object->faces[i];
    }

    delete object;
}

void ModelObject::generate_gl()
{
    for (unsigned int i = 0; i < object->faces.size(); i++)
    {
        glBegin(GL_POLYGON);
        for (unsigned int h = 0; h < object->faces[i]->coords.size(); h++)
        {
            float x = object->faces[i]->coords[h]->x;
            float y = object->faces[i]->coords[h]->y;
            float z = object->faces[i]->coords[h]->z;

            glVertex3f(x, y, z);

        }
        glEnd();
    }
}

void ModelObject::read_obj(const char * filename)
{
    int vert_id = 0;
    std::string temp;
    std::string obj_line;
    std::ifstream obj_file (filename);

    object->vert_count = 0;
    object->face_count = 0;

    if (obj_file.is_open())
    {
        while (getline(obj_file, obj_line))
        {
            if (obj_line[0] == 'o')
                continue;
            else if (obj_line[0] == 'v')
            {
                object->vert_count++;

                std::istringstream vert_string (obj_line);
                coord * new_coord = new coord;

                new_coord->id = object->vert_count;

                for (int i = 0; i < 4; i++)
                {
                    if (getline(vert_string, temp, ' '))
                    {
                        if (i == 1) {
                            new_coord->x = (float)atof(temp.c_str());
                        }
                        else if (i == 2) {
                            new_coord->y = (float)atof(temp.c_str());
                        }
                        else if (i == 3) {
                            new_coord->z = (float)atof(temp.c_str());
                        }
                    }
                }

                object->vertices.push_back(new_coord);
            }
            else if (obj_line[0] == 'f')
            {
                object->face_count++;

                std::istringstream face_string (obj_line);
                face * new_face = new face;
                int coord_id;

                getline(face_string, temp, ' ');

                while (getline(face_string, temp, ' '))
                {
                    coord_id = (int)atoi(temp.c_str());

                    new_face->coords.push_back(object->vertices[coord_id - 1]);
                }

                object->faces.push_back(new_face);

            }
        }
        obj_file.close();

        std::cout << "Object has " << object->vert_count << " verticies and " << object->face_count << " faces." << std::endl;
    }
}

void ModelObject::print_data()
{
    std::cout << "Vertices." << std::endl;

    std::cout.precision(9);
    for (unsigned int i = 0; i < object->vertices.size(); i++)
    {
        std::cout << "\tV: ";
        print_coord(object->vertices[i]);
        std::cout << std::endl;
    }

    std::cout << "Faces." << std::endl;

    std::cout.precision(9);
    for (unsigned int i = 0; i < object->faces.size(); i++)
    {
        std::cout << "\tF: ";
        print_face(object->faces[i]);
        std::cout << std::endl;
    }
}

void ModelObject::save_obj()
{
    std::ofstream output_file ("output.obj", std::ios::out | std::ios::trunc);

    std::cout.precision(9);

    output_file << std::fixed;

    if (output_file.is_open())
    {
        for (unsigned int i = 0; i < object->vertices.size(); i++)
        {
            output_file << "v" << " ";
            output_file << object->vertices[i]->x << " ";
            output_file << object->vertices[i]->y << " ";
            output_file << object->vertices[i]->z << "\n";
        }

        for (unsigned int i = 0; i < object->faces.size(); i++)
        {
            output_file << "f" << " ";

            for (unsigned int h = 0; h < object->faces[i]->coords.size(); h++)
            {
                output_file << object->faces[i]->coords[h]->id;

                if (h < object->faces[i]->coords.size() - 1)
                    output_file << " ";
            }
            // output_file << "\n";
        }

        output_file.close();
    }
}

void ModelObject::print_coord(const coord * my_coord)
{
    std::cout << "(";

    std::cout << my_coord->x;
    std::cout << ", ";
    std::cout << my_coord->y;
    std::cout << ", ";
    std::cout << my_coord->z;

    std::cout << ")";
}

void ModelObject::print_face(const face * my_face)
{
    for (unsigned int i = 0; i < my_face->coords.size(); i++)
    {
        print_coord(my_face->coords[i]);

        if (i < my_face->coords.size() - 1)
            std::cout << ", ";
    }
}