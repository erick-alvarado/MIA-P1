#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <ctime>
#include "../Disk/Mbr.cc"
#include "../Disk/Space.cc"

using namespace std;


class Disk{
    int disk_signature=0;
    //int mbr_size=136;
  public:
    //Metodos sobre el disco
    void CreateDisk(int size, string f, string u,string path){
        int size_=1024;
        if(u=="m"){
            size_=1024*1024;
        }
        size_=size_*size;
        
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
        mbr.particiones[0]= p1;
        mbr.particiones[1]=p2;
        mbr.particiones[2]=p3;
        mbr.particiones[3]=p4;
        writeMbr(path,mbr);
        resetFile(path,sizeof(mbr),size_-sizeof(mbr));
        cout << "Archivo creado:"+path<<endl;
        
       
    }
    
    void DeleteDisk(string path){
        if (remove( path.c_str() ) != 0){
            cout<<"Error al borrar el archivo:"+path<<endl;
        }
        else{
            cout<<"Archivo eliminado:"+path<<endl;
        }
    }
    
    void CreatePartition(int size, string u, string path, string type, string f, string delete_, string name, int add){
        Mbr mbr = getMbr(path);
        if(mbr.mbr_tamano==0){
            return;
        }
        if(delete_!=""){
            cout<<"Eliminando particion"<<endl;
            //Eliminar particion
            return;
        }
        if(add!=-1){
            cout<<"Anadiendo particion"<<endl;

            //Añadir particion
            return;
        }
        if(u=="k"){
            size = size*1024;
        }
        if(u=="m"){
            size = size*1024*1024;
        }
        Partition p;
        vector<Space> sp = getSpaces(mbr.mbr_tamano,mbr.particiones,size);
        if(sp.size()==0){
            cout<<"Espacio insuficiente para crear la particion:"+name<<endl;
            return;
        }
        if(mbr.disk_fit=='f'){
           p.part_start= sp[0].start;
        }
        if(mbr.disk_fit =='w'){
           sort(sp.begin(), sp.end(), [](Space const & a, Space const & b) -> bool{ return a.size< b.size; } );
           p.part_start=sp[0].start;
        }
        if(mbr.disk_fit=='b'){
            sort(sp.begin(), sp.end(), [](Space const & a, Space const & b) -> bool{ return a.size> b.size; } );
            p.part_start=sp[0].start;
        }
        p.part_status='o';
        p.part_type=type[0];
        p.part_fit=f[0];
        p.part_size=size;
        
        strcpy (p.part_name, name.c_str());
        selectPartition(mbr,p);
        writeMbr(path,mbr);
        
    }

    //Metidis oara obtener y escribir MBRs
    Mbr getMbr(string path){
        Mbr mbr;
        fstream file;
        getFile_toRead(file,path);
        if (!file){
            cout << "Error al abrir el disco:"+path<<endl;
            return mbr;
        }

        if (file.read((char *)&mbr, sizeof(mbr))){
            cout << endl
                 << endl;
            cout << "Disco cargado:"+path<<endl;
        }
        else{
            cout << "Error al abrir el disco:"+path<<endl;
            return mbr;
        }

        file.close();
        return mbr;
    }
    void writeMbr(string path, Mbr mbr){
        fstream file;
        getFile_toWrite(file,path);
        if (!file){
            cout << "Error al crear el archivo:"+path<<endl;
            return;
        }
        file.seekp(0);
        file.write((char *)&mbr, sizeof(mbr));
        file.close();
        cout<<"Se escribio el mbr:"+path<<endl;
    }
    void selectPartition(Mbr& mbr, Partition p){
        if(mbr.particiones[0].part_status=='f'){
            mbr.particiones[0]=p;
            return;
        }
        if(mbr.particiones[1].part_status=='f'){
            mbr.particiones[1]=p;
            return;
        }
        if(mbr.particiones[2].part_status=='f'){
            mbr.particiones[2]=p;
            return;
        }
        if(mbr.particiones[3].part_status=='f'){
            mbr.particiones[3]=p;
            return;
        }
        cout<<"No se puede almacenar la particion:"+string(p.part_name)+" ya que el disco ya posee 4 particiones"<<endl;
    }
    //Metodos para resetear y abrir archivos
	void getFile_toWrite(fstream& file,string path){
        file.open(path,  ios::in |ios::out |ios::binary);
    }
    void getFile_toRead(fstream& file,string path){
        file.open(path, ios::in |ios::binary);
    }
    void resetFile(string path, int inicio, int fin){
        fstream file;
        getFile_toWrite(file,path);
        if(!file){
            cout<<"Error al abrir el archivo:"+path<<endl;
        }
        file.seekp(inicio);
        //int t = file.tellg();
        for (int i = 0; i < fin; ++i){
            char zero = 0;
            file.write(&zero, sizeof(char));
        }
        //t = file.tellg();
        file.close();
    }

    //Metodos para obtener espacios y organizar las particiones
    Partition* Sort(Partition particiones[4]){
        sort(particiones, particiones + 4, [](Partition const & a, Partition const & b) -> bool{ return a.part_start > b.part_start; } );
        return particiones;
    };
    vector<Space> getSpaces(int mbr_tamano, Partition particiones[4],int size){
      vector<Space> spaces;
      Space temp;
      int space = particiones[0].part_start;
      if(space>=size){
        temp.start=0;
        temp.size=space;
        spaces.push_back(temp);
      }
      for(int i = 0; i<3; i++){
        space = particiones[i+1].part_start-particiones[i].part_start-particiones[i].part_size;
        if(space>=size){
            temp.size=space;
            temp.start = particiones[i].part_start+particiones[i].part_size;
            spaces.push_back(temp);
        }
      }
      space = mbr_tamano-particiones[3].part_start+particiones[3].part_size;
      if(space>=size){
          temp.size=size;
          temp.start = particiones[3].part_start+particiones[3].part_size;
          spaces.push_back(temp);
      }
      return spaces;
    }
	
};