ifstream obj_file_data (filename);

   if (obj_file_data.is_open())
   {
      while (getline(obj_file_data, obj_line))
      {
         if (obj_line[0] == 'o')
            continue;
         else if (obj_line[0] == 'v')
         {
            object->vert_count++;
         }
         else if (obj_line[0] == 'f')
         {
            object->face_count++;
         //    vert_id++;
         //    istringstream vert (obj_line);
         //    while (getline(vert, temp, ' ')) {
         //       if (temp == "v")
         //          continue;

         //       cout << temp << ",";
         //    }
         //    cout << endl;
         }
      }
      obj_file_data.close();
   }