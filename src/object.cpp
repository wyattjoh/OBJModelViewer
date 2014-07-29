#include "object.h"

// ModelObject * ModelObject::current_object;

ModelObject::ModelObject()
{
    object = new obj_data;

    object->vert_count = 0;
    object->face_count = 0;
    object->obj_name = 0;
}

ModelObject::~ModelObject ()
{
    for (unsigned int i = 0; i < object->vertices.size(); i++)
        delete object->vertices[i];

    for (unsigned int i = 0; i < object->faces.size(); i++)
        delete object->faces[i];

    delete [] object->obj_name;

    delete object;
}

void ModelObject::read_obj(const char * filename)
{
    int vert_id = 1;
    std::string temp;
    std::string obj_line;
    std::ifstream obj_file (filename);

    if (obj_file.is_open()) {
        while (getline(obj_file, obj_line)) {
            if (obj_line[0] == 'v') {
                std::istringstream vert_string (obj_line);
                coord * new_coord = new coord;

                new_coord->id = vert_id;

                for (int i = 0; i < 4; i++) {
                    if (getline(vert_string, temp, ' ')) {
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
                object->vert_count++;
                vert_id++;
            }
            else if (obj_line[0] == 'f') {
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
            else if (obj_line[0] == 'o' && object->obj_name == 0) {
                std::string name_string (obj_line);

                if (name_string.length() > 2) {
                    object->obj_name = new char[name_string.length() - 1];
                    
                    for (int i = 2; i < name_string.length(); i++)
                        object->obj_name[i - 2] = name_string[i];
                }
            }
        }
        obj_file.close();

        process_object();
    }
}

const char * ModelObject::get_name(char ** argv)
{
    if (object->obj_name == 0)
        return argv[1];
    return object->obj_name;
}

void ModelObject::print_data()
{

    std::cout.precision(9);
    for (unsigned int i = 0; i < object->vertices.size(); i++)
    {
        std::cout << "\tV: ";
        print_coord(object->vertices[i]);
        std::cout << std::endl;
    }

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

    // std::cout.precision(9);

    output_file << std::fixed;

    if (output_file.is_open())
    {
        output_file << "o output\n";

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
                output_file << object->faces[i]->coords[h]->id << " ";

                // if (h < object->faces[i]->coords.size() - 1)
                //     output_file << " ";
            }

            output_file << "\n";
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

void ModelObject::process_object()
{
    float scaling_factor;
    coord object_min;
    coord object_max;

    // Initilize min/max
    object_min.x = object->vertices[0]->x;
    object_min.y = object->vertices[0]->y;
    object_min.z = object->vertices[0]->z;

    object_max.x = object->vertices[0]->x;
    object_max.y = object->vertices[0]->y;
    object_max.z = object->vertices[0]->z;

    // Find the min max
    for (unsigned int i = 1; i < object->vertices.size(); i++) {
        if (object->vertices[i]->x < object_min.x)
            object_min.x = object->vertices[i]->x;
        else if (object->vertices[i]->x > object_max.x)
            object_max.x = object->vertices[i]->x;

        if (object->vertices[i]->y < object_min.y)
            object_min.y = object->vertices[i]->y;
        else if (object->vertices[i]->y > object_max.y)
            object_max.y = object->vertices[i]->y;

        if (object->vertices[i]->z < object_min.z)
            object_min.z = object->vertices[i]->z;
        else if (object->vertices[i]->z > object_max.z)
            object_max.z = object->vertices[i]->z;
    }

    // Start finding the scaling factor
    scaling_factor = object_max.x - object_min.x;

    if (scaling_factor < object_max.y - object_min.y)
        scaling_factor = object_max.y - object_min.y;
    else if (scaling_factor < object_max.z - object_min.z)
        scaling_factor = object_max.z - object_min.z;

    // Apply the scaling factor
    for (unsigned int i = 0; i < object->vertices.size(); i++) {
        object->vertices[i]->x /= scaling_factor;
        object->vertices[i]->y /= scaling_factor;
        object->vertices[i]->z /= scaling_factor;
    }

    object->center.x = (object_max.x + object_min.x)/2.0;
    object->center.y = (object_max.y + object_min.y)/2.0;
    object->center.z = (object_max.z + object_min.z)/2.0;
}