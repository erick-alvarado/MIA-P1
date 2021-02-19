#include <iostream>
#include <fstream>
using namespace std;
#include "../Objects/Parser.cc"

/*
int getNumeroInodos(int size, int super, int journal, int inodos, int bloque){
  //Size en bytes, multiplicar por 1024 respectivamente
    return ((size-super)/(journal+inodos+(3*bloque)+4));
}*/

int main(){
  Parser p;
  string s;
  cout<<"---------------MENU PRINCIPAL---------------"<<endl;
  do{
    p.Reset();
    cout<<"Ingrese el comando EXEC"<<endl;
    getline(cin,s);
    p.setTxt(s);
    p.Exec();
  }while(p.getNumError()>0);
  
}

