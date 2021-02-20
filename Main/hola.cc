#include <iostream>
#include <fstream>
using namespace std;
#include "../Objects/Parser.cc"



int main(){
  Parser p;
  string s;
  cout<<"---------------MENU PRINCIPAL---------------"<<endl;
  do{
    cout<<"Ingrese el comando EXEC"<<endl;
    getline(cin,s);
    vector<Instruction> in =  p.Exec(s);
    
  }while(p.getNumError()>0);
  
}
/*
int getNumeroInodos(int size, int super, int journal, int inodos, int bloque){
  //Size en bytes, multiplicar por 1024 respectivamente
    return ((size-super)/(journal+inodos+(3*bloque)+4));
}*/
/*for(int i =0; i<in.size();i++){
      cout<<"--------------------"<<endl;
      cout<<in[i].comando<<endl;
      cout<<in[i].type<<endl;
      cout<<in[i].delete_<<endl;
      cout<<in[i].name<<endl;
      cout<<in[i].add<<endl;
      cout<<in[i].size<<endl;
      cout<<in[i].f_<<endl;
      cout<<in[i].u_<<endl;
      cout<<in[i].path<<endl;
      cout<<in[i].id<<endl;
      cout<<in[i].fs<<endl;
    }*/