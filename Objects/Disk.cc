#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <ctime>
#include "../Disk/Mbr.cc"
#include "../Disk/Ebr.cc"
#include "../Disk/Space.cc"
#include "Code.cc"
using namespace std;


class Disk{
    int disk_signature=0;
    vector<Code> codes;
    //int mbr_size=136;
  public:
  //Metodos de mount y unmount
    char intToAlphabet( int i )
    {
        return static_cast<char>('a' - 1 + i);
    }
    string getPathFromId(string id){
        for(Code c: codes){
            if(id=="46"+to_string(c.disk_code)+intToAlphabet(c.partition_code)){
                return c.path;
            }
        }
        return "";
        
    }
    void mountPartition(string name, string path){
        Mbr m = getMbr(path);
        if(m.mbr_disk_signature!=0){
            if(verifyMountedPartition(name)){
                cout<<"Esta particion ya se encuentra montada:"<<name<<endl;
                return;
            }
            Partition p = verifyPartitionExists(m.particiones,name);
            if(p.part_fit=='-'){
                cout<<"No se encuentra la particion:"<<name<<endl;
                return;
            }
            int diskcode = m.mbr_disk_signature;
            int partCount = getPartitionCount(diskcode);
            Code c(diskcode,partCount,name,path,p);
            codes.push_back(c);
            printMounted();
        }
        
    }
    void printMounted(){
        cout<<"Particiones montadas:"<<endl;
        for(Code c:codes){
            string id = "46"+to_string(c.disk_code)+intToAlphabet(c.partition_code);
            cout<<c.path<<"  |  "<<c.name<<"  |  "<<id<<endl;
        }
    }
    void unmountPartition(string id){
        for(int i=0; i<codes.size();i++){
            Code c = codes[i];
            string id_ = "46"+to_string(c.disk_code)+intToAlphabet(c.partition_code);
            if(id_==id){
                cout<<"Partition unmounted:"<<id<<endl;
                codes.erase(codes.begin()+i);
                printMounted();
                return;
            }
        }
        cout<<"No se encontro montada la particion:"<<id<<endl;
    }
    bool verifyMountedPartition(string name){
        for(Code c:codes){
            if(c.name==name){
                return true;
            }
        }
        return false;
    }
    Partition verifyPartitionExists(Partition prts[4], string name){
        for(int i = 0; i<4; i++){
            if(prts[i].part_name==name){
                return prts[i];
            }
        }
        Partition p;
        return p;
    }
    int getPartitionCount(int diskcode){
        int num =1;
        for(int i = 0; i<codes.size(); i++){
            if(codes[i].disk_code==diskcode){
                num++;
            }
        }
        return num;
    }


    //Metodos sobre el disco
    void CreateDisk(int size, string f, string u,string path){
        int size_=1024;
        if(u=="m"){
            size_=1024*1024;
        }
        size_=size_*size;
        
        disk_signature+=1;
        Mbr mbr;
        mbr.mbr_tamano=size_;
        mbr.mbr_fecha_creacion=time(0);
        mbr.mbr_disk_signature= disk_signature;
        mbr.disk_fit=f[0];
        Partition p1;
        Partition p2;
        Partition p3;
        Partition p4;
        mbr.particiones[0]= p1;
        mbr.particiones[1]=p2;
        mbr.particiones[2]=p3;
        mbr.particiones[3]=p4;
        if(createFile(path)){
            writeMbr(path,mbr);
            resetFile(path,sizeof(mbr),size_-sizeof(mbr));
            cout << "Archivo creado:"+path<<endl;
        }
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
        Partition temp;
        if(mbr.mbr_tamano==0){
            return;
        }
        
        if(u=="k"){
            size = size*1024;
        }
        if(u=="m"){
            size = size*1024*1024;
        }
        if(delete_!=""){
            Partition& p = verifyPartition(name,mbr,temp);
            if(p.part_size!=0){
                if(delete_=="full"){
                    resetFile(path,p.part_start,p.part_size);
                }
                p = temp;
                cout<<"Se elimino la particion:"+name<<endl;
                writeMbr(path,mbr);
            }
            return;
        }
        if(add!=-1){
            if(u=="k"){
                add = add*1024;
            }
            if(u=="m"){
                add = add*1024*1024;
            }
            cout<<to_string(add)<<endl;
            Partition& p = verifyPartition(name,mbr,temp);
            if(p.part_size!=0){
                if(add>=0){
                    vector<Space> v = getSpaces(mbr.mbr_tamano,mbr.particiones,add);
                    if(v.size()>0){
                        Space sp = getSpace(v,p.part_start+p.part_size);
                        if(sp.start!=-1){
                            p.part_size+=add;
                            cout<<"Se agrego a la particion "+name+": "+to_string(add)<<endl;
                            writeMbr(path,mbr);
                            return;
                        }
                        
                    }
                    cout<<"No existe espacio para agregarle a la particion:"+name<<endl;
                    return;
                }
                else{
                    if(p.part_size+add>0){
                        p.part_size=p.part_size+add;
                        writeMbr(path,mbr);
                        cout<<"Se quito a la particion "+name+": "+to_string(add)<<endl;
                        return;
                    }
                    cout<<"No se puede restar espacio a la particion:"+name<<endl;
                    return;
                }
            }
            cout<<"No existe la particion:"+name<<endl;
            return;
        }
        
        //Crear particion logica
        if(type[0]=='l'){
            //Obtenemos la particion extendida si existe
            Ebr ebr;
            Partition extended;
            for(int i =0;i<4;i++){
                if(mbr.particiones[i].part_type=='e'){
                    extended = mbr.particiones[i];
                    ebr= getEbr(path,extended.part_start);
                }
            }
            if(ebr.part_status=='-'){
                cout<<"No existe una particion extendida para almacenar las logica:"<<name<<endl;
                return;
            }

            //Obtenemos el ultimo ebr 
            //Guardar el aterior auxiliar para ponerle la del siguiente
            while(ebr.part_next!=-1){
                ebr = getEbr(path,ebr.part_next);
            }

            int inicio_anterior=0;
            if(ebr.part_start==0){
                ebr.part_start=extended.part_start;
            }
            else{
                inicio_anterior=ebr.part_start;
                ebr.part_start= ebr.part_start+sizeof(ebr)+ ebr.part_size;
            }
            ebr.part_next=-1;
            ebr.part_status='o';
            ebr.part_fit=f[0];
            ebr.part_size=size;
            strcpy (ebr.part_name, name.c_str());

            if(ebr.part_start+ebr.part_size+sizeof(ebr)-1<=extended.part_start+extended.part_size){
                writeEbr(path,ebr,ebr.part_start);
            }
            else{
                cout<<"El tamaño de la logica a crear sobrepasa el tamaño de la particion"<<endl;
            }
            if(inicio_anterior!=0){
                Ebr aux= getEbr(path,inicio_anterior);
                aux.part_next= ebr.part_start;
                writeEbr(path,aux,aux.part_start);
            }
            return;
        }    


        //Crear particion primaria
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
        p.part_start+=sizeof(mbr);
        strcpy (p.part_name, name.c_str());
        
        //Crear particion extendida
        if(p.part_type=='e'){
           //Verificar que no exista una particion extendida
            for(int i =0;i<4;i++){
                if(mbr.particiones[i].part_type=='e'){
                    cout<<"Ya existe una particion extendida";
                    return;
                }
            }
            Ebr ebr;
            ebr.part_status='o';
            string nn = "Inicial";
            strcpy (ebr.part_name, nn.c_str());
            writeEbr(path,ebr,p.part_start);
        }
       
        selectPartition(mbr,p);
        writeMbr(path,mbr);
        
    }

    
    
    //Metodos para resetear y abrir archivos
	void getFile_toWrite(fstream& file,string path){
        file.open(path,  ios::in |ios::out |ios::binary);
    }
    void getFile_toRead(fstream& file,string path){
        file.open(path, ios::in |ios::binary);
    }
    bool createFile(string path){
        fstream f;
        getFile_toRead(f,path);
        if(f){
            cout<<"Este disco ya existe:"+path<<endl;
            return false;
        }
        ofstream file(path);
        if (!file){
            cout << "Error al crear el archivo:"+path<<endl;
            return false;
        }
        cout<<"Se creo el archivo:"+path<<endl;
        file.close();
        return true;
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
    //Metodos para obtener y escribir EBRs
    Ebr getEbr(string path, int inicio){
        Ebr ebr;
        fstream file;
        getFile_toRead(file,path);
        if (!file){
            cout << "Error al abrir el ebr:"+path<<endl;
            return ebr;
        }
        file.seekp(inicio);
        if (file.read((char *)&ebr, sizeof(ebr))){
            cout << endl
                 << endl;
            cout << "Ebr cargado:"+path<<endl;
        }
        else{
            cout << "Error al abrir el ebr:"+path<<endl;
            return ebr;
        }

        file.close();
        return ebr;
    }
    void writeEbr(string path, Ebr ebr, int inicio){
        fstream file;
        getFile_toWrite(file,path);
        if (!file){
            cout << "Error al crear el mbr:"+path<<endl;
            return;
        }
        file.seekp(inicio);
        file.write((char *)&ebr, sizeof(ebr));
        file.close();
        cout<<"Se escribio el mbr:"+path<<endl;
    }

    //Metodos para obtener y escribir MBRs esto deberia ir en mbr pero sepa si se puede
    Mbr getMbr(string path){
        Mbr mbr;
        fstream file;
        getFile_toRead(file,path);
        if (!file){
            cout << "Error al abrir el mbr:"+path<<endl;
            return mbr;
        }

        if (file.read((char *)&mbr, sizeof(mbr))){
            cout << endl
                 << endl;
            cout << "Mbr cargado:"+path<<endl;
        }
        else{
            cout << "Error al abrir el mbr:"+path<<endl;
            return mbr;
        }

        file.close();
        return mbr;
    }
    void writeMbr(string path, Mbr mbr){
        fstream file;
        getFile_toWrite(file,path);
        if (!file){
            cout << "Error al crear el mbr:"+path<<endl;
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
    Partition& verifyPartition(string name, Mbr &mbr, Partition &temp){
        if(mbr.particiones[0].part_name==name){
            return mbr.particiones[0];
        }
        if(mbr.particiones[1].part_name==name){
            return mbr.particiones[1];
        }
        if(mbr.particiones[2].part_name==name){
            return mbr.particiones[2];
        }
        if(mbr.particiones[3].part_name==name){
            return mbr.particiones[3];
        }
        cout<<"No se puede eliminar la particion:"+name+" ya que no existe"<<endl;
        return temp;
    }
    //Metodos para obtener espacios y organizar las particiones
    Partition* Sort(Partition particiones[4]){
        sort(particiones, particiones + 4, [](Partition const & a, Partition const & b) -> bool{ return a.part_start > b.part_start; } );
        return particiones;
    };
    vector<Space> getSpaces(int mbr_tamano, Partition particiones[4],int size){
        vector<Space> spaces;
        vector<Partition> v;
        Space temp;
        int space;
     
        for(int i = 0; i<3; i++){
            if(particiones[i].part_status=='o'){
                v.push_back(particiones[i]);
            }
        }
        if(v.size()>0){
            space = particiones[0].part_start;
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
            space=mbr_tamano-v[v.size()-1].part_start-v[v.size()-1].part_size;
            if(space>=size){
                temp.size=space;
                temp.start= v[v.size()-1].part_start+v[v.size()-1].part_size;
                spaces.push_back(temp);
            }
        }
        else{
            space = mbr_tamano;
            if(space>=size){
                temp.size=space;
                temp.start=0;
                spaces.push_back(temp);
            }
        }
        return spaces;
    }
	Space getSpace(vector<Space> spaces,int start){
        for(int i=0; i<spaces.size();i++){
            if(spaces[i].start==start){
                return spaces[i];
            }
        }
        Space p;
        p.start=-1;
        return p;
    }
};