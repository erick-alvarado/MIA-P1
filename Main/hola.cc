#include <iostream>
#include <fstream>

#include "../Objects/Parser.cc"
#include "../Objects/Disk.cc"
#include "../Objects/Report.cc"

using namespace std;
Report rep;
Parser p;
string s;
vector<Instruction> instrucciones;
Disk disco;

void Exec(vector<Instruction> ins)
{
  for (int i = 0; i < ins.size(); i++)
  {
    if (ins[i].comando == "mkdisk")
    {
      disco.CreateDisk(ins[i].size, ins[i].f_, ins[i].u_, ins[i].path);
      continue;
    }
    if (ins[i].comando == "fdisk")
    {
      Instruction asdf = ins[i];
      disco.CreatePartition(ins[i].size, ins[i].u_, ins[i].path, ins[i].type, ins[i].f_, ins[i].delete_, ins[i].name, ins[i].add);
      cout << "--------------------" << endl;
      Mbr m = disco.getMbr(ins[i].path);
      
      Ebr e = disco.getEbr(ins[i].path, m.particiones[0].part_start);
      while (e.part_next != -1)
      {
        cout << e.part_name << endl;
        cout << e.part_start << endl;
        cout << e.part_size << endl;
        cout << e.part_next << endl;
        cout << e.part_fit << endl;
        e = disco.getEbr(ins[i].path, e.part_next);
      }
      cout << e.part_name << endl;
      cout << e.part_start << endl;
      cout << e.part_size << endl;
      cout << e.part_next << endl;
      cout << e.part_fit << endl;
      continue;
    }
    if (ins[i].comando == "rmdisk")
    {
      disco.DeleteDisk(ins[i].path);
      continue;
    }
    if (ins[i].comando == "mount")
    {
      disco.mountPartition(ins[i].name, ins[i].path);
      continue;
    }
    if (ins[i].comando == "unmount")
    {
      disco.unmountPartition(ins[i].id);
      continue;
    }
    if (ins[i].comando == "rep")
    {
      string path = disco.getPathFromId(ins[i].id);
      if (path == "")
      {
        cout << "No se encuentra montada la particion:" << ins[i].id << endl;
        continue;
      }
      if (ins[i].name == "mbr")
      {
        rep.getMbr(disco.getMbr(path), ins[i].path);
        continue;
      }
      if (ins[i].name == "disk")
      {
        rep.getDsk(disco.getMbr(path), ins[i].path);
      }
    }
  }
}

int main(int argc, char *argv[])
{

  //Get args
  for (int i = 1; i < argc; i++)
  {
    if (argv[i][0] == '.')
    {
      s += argv[i];
    }
    else
    {
      s += " ";
      s += argv[i];
    }
  }
  //Pa pruebas windows
  s = "exec -path= \\Users\\alexa\\OneDrive\\Desktop\\pruebas.txt";

  //Parse
  instrucciones = p.Parse(s);

  if (instrucciones.size() > 0)
  {
    Exec(instrucciones);
  }

  /*
  cout<<"---------------MENU PRINCIPAL---------------"<<endl;
  do{
    cout<<"Ingrese el comando EXEC"<<endl;
    getline(cin,s);
    instrucciones =  p.Exec(s);
    
  }while(p.getNumError()>0);
  if(instrucciones.size()>0){
    Exec(instrucciones);
  }*/
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
