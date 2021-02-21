#include <iostream>
#include <fstream>
using namespace std;
#include "../Objects/Parser.cc"
#include "../Objects/Disk.cc"
Parser p;
string s;
vector<Instruction> instrucciones;
Disk disco;
vector<Mbr> mounts;
void Exec(vector<Instruction> ins){
  for(int i =0; i<ins.size();i++){
    if(ins[i].comando=="mkdisk"){
      disco.CreateDisk(ins[i].size,ins[i].f_,ins[i].u_,ins[i].path);  
      continue;
    }
    if(ins[i].comando=="rmdisk"){
      disco.DeleteDisk(ins[i].path);
      continue;
    }
    if(ins[i].comando=="mount"){
      mounts.push_back(disco.getMbr(ins[i].path));  
      continue;
    }
  }
  
  
}

int main(){
  
  cout<<"---------------MENU PRINCIPAL---------------"<<endl;
  do{
    cout<<"Ingrese el comando EXEC"<<endl;
    getline(cin,s);
    instrucciones =  p.Exec(s);
    
  }while(p.getNumError()>0);
  if(instrucciones.size()>0){
    Exec(instrucciones);
  }
}

/*
int getNumeroInodos(int size, int super, int journal, int inodos, int bloque){
  //Size en bytes, multiplicar por 1024 respectivamente
    return ((size-super)/(journal+inodos+(3*bloque)+4));
    cout<<"--------------------"<<endl;
      cout<<ins[i].comando<<endl;
      cout<<ins[i].type<<endl;
      cout<<ins[i].delete_<<endl;
      cout<<ins[i].name<<endl;
      cout<<ins[i].add<<endl;
      cout<<ins[i].size<<endl;
      cout<<ins[i].f_<<endl;
      cout<<ins[i].u_<<endl;
      cout<<ins[i].path<<endl;
      cout<<ins[i].id<<endl;
      cout<<ins[i].fs<<endl;
}



if(b.mbr_tamano!=0){
    cout<<b.disk_fit<<endl;
    cout<<b.mbr_disk_signature<<endl;
    cout<<b.mbr_fecha_creacion<<endl;
    cout<<b.mbr_tamano<<endl;
  }
  
  
  */
