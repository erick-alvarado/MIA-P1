#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <ctime>
#include "../Disk/Mbr.cc"

using namespace std;


class Disk{
    int disk_signature=0;
  public:
    void CrearDisco(int size, string f, string u,string path){
        int size_=1024;
        if(u=="m"){
            size_=1024*1024;
        }
        size_=size_*size;

        //write object into the file
        fstream file=getFile(path);
        
        if (!file){
            cout << "Error al crear el archivo:"+path<<endl;
            return;
        }
        disk_signature++;
        Mbr mbr;
        mbr.mbr_tamano=size_;
        mbr.mbr_fecha_creacion=time(0);
        mbr.mbr_disk_signature= disk_signature;
        char c[1];
        strcpy(c , f.c_str()); 
        mbr.disk_fit=c[0];
        Partition p1;
        Partition p2;
        Partition p3;
        Partition p4;
        mbr.mbr_partition_1= p1;
        mbr.mbr_partition_2=p2;
        mbr.mbr_partition_3=p3;
        mbr.mbr_partition_4=p4;

        file.write((char *)&mbr, sizeof(mbr));
        file.close();
        cout << "Archivo creado:"+path<<endl;
        /*
        //open file again
        file.open(FILE_NAME, ios::in | ios::binary);
        if (!file)
        {
            cout << "Error in opening file...\n";
            return -1;
        }

        if (file.read((char *)&emp, sizeof(emp)))
        {
            cout << endl
                 << endl;
            cout << "Data extracted from file..\n";
            //print the object
            emp.displayEmployee();
        }
        else
        {
            cout << "Error in reading data from file...\n";
            return -1;
        }

        file.close();
        return 0;*/
    }
	fstream getFile(string path){
        fstream file;
        file.open(path, ios::out | ios::binary);
        return file;
        
    }
	
};