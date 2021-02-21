#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <ctime>
#include "../Disk/Mbr.cc"

using namespace std;


class Disk{
    int disk_signature=0;
    int mbr_size=136;
  public:
    void CreateDisk(int size, string f, string u,string path){
        int size_=1024;
        if(u=="m"){
            size_=1024*1024;
        }
        size_=size_*size;
        fstream file=getFile_toWrite(path);
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
        resetFile(path,sizeof(mbr),size_-sizeof(mbr));
        cout << "Archivo creado:"+path<<endl;
        
       
    }

    Mbr getMbr(string path){
        Mbr mbr;
        fstream file = getFile_toRead(path);
        if (!file){
            cout << "Error al abrir el archivo:"+path<<endl;
            return mbr;
        }

        if (file.read((char *)&mbr, sizeof(mbr))){
            cout << endl
                 << endl;
            cout << "Archivo cargado:"+path<<endl;
        }
        else{
            cout << "Error al abrir el archivo:"+path<<endl;
            return mbr;
        }

        file.close();
        return mbr;
    }

	fstream getFile_toWrite(string path){
        fstream file;
        file.open(path,  ios::app|ios::out |ios::binary);
        return file;
    }
    fstream getFile_toRead(string path){
        fstream file;
        file.open(path, ios::in |ios::binary);
        return file;
    }
    
    void resetFile(string path, int inicio, int fin){
        fstream file= getFile_toWrite(path);
        file.seekp(inicio);
        int t = file.tellg();
        cout<<t<<endl;
        cout<<"path:"+path<<endl;
        cout<<"inicio:"+to_string(inicio)<<endl;
        cout<<"size fin:"+to_string(fin)<<endl;
        for (int i = 0; i < fin; ++i){
            char zero = 0;
            file.write(&zero, sizeof(char));
        }
        t = file.tellg();
        cout<<t<<endl;
        file.close();
    }
	
};