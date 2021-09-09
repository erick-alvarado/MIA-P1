#include <iostream>
#include <vector>
#include <cctype>
#include <string>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include "Instruction.cc"
using namespace std;

class Parser{
    vector<string> tokens;
    vector<string> errores;
    vector<string> lineas;
    vector<Instruction> instrucciones;
    int index = 0;
    string cadena="";
    string linea="";
    

    int size=-1;
    string f_ = "";
    string u_ = "";
    string path = "";
    string type = "";
    string delete_="";
    string name = "";
    int add = -1;
    string id ="";
    string fs="";
    string txt;

  public:

    vector<Instruction> Parse(string text){
        txt = text;
        Split();
        for(index = 0; index < tokens.size() ; index ++)
        {
            ToLower(tokens[index]);
            ResetParams();
            if(tokens.at(index)=="exec"){
                Params();
                if(path==""){
                    errores.push_back("EXEC: falta path");
                    break;
                }
                ifstream File(path);
                string str((std::istreambuf_iterator<char>(File)),
                           istreambuf_iterator<char>());
                txt = "";
                if (File)
                {
                    txt = str;
                    Reset();
                    Parse(txt);
                    break;
                }
                else
                {
                    cout << "Direccion invalida" << endl;
                }
            }
            if(tokens.at(index)=="mkdisk"){
                Params();
                if(size==-1){
                    errores.push_back("MKDISK: falta size");
                    break;
                }
                if(f_==""){
                    f_="ff";
                }
                if(u_==""){
                    u_="m";
                }
                if (path == ""){
                    errores.push_back("MKDISK: falta path");
                    break;
                }
                mkdisk();
                continue;
            }
            if(tokens.at(index)=="rmdisk"){
                Params();
                if (path == ""){
                    errores.push_back("RMDISK: falta path");
                    break;
                }
                rmdisk();
                continue;
            }
            if(tokens.at(index)=="fdisk"){
                Params();
                if(size==-1 && delete_=="" && add==-1){
                    //OBLIGATORIO AL CREAR
                    errores.push_back("FDISK: falta size");
                    break;
                }
                if(u_==""){
                    u_="k";
                }
                if (path == ""){
                    errores.push_back("FDISK: falta path");
                    break;
                }
                if (type == ""){
                    type="p";
                }
                if(f_==""){
                    f_="wf";
                }
                if (name == ""){
                    errores.push_back("FDISK: falta name");
                    break;
                }
                fdisk();
                continue;
            }
            if(tokens.at(index)=="mount"){
                Params();
                if (path == ""){
                    errores.push_back("MOUNT: falta path");
                    break;
                }
                if (name == ""){
                    errores.push_back("MOUNT: falta name");
                    break;
                }
                mount();
                continue;
            }
            if(tokens.at(index)=="unmount"){
                Params();
                if (id == ""){
                    errores.push_back("UNMOUNT: falta id");
                    break;
                }
                unmount();
                continue;
            }
            if(tokens.at(index)=="mkfs"){
                Params();
                if (id == ""){
                    errores.push_back("MKFS: falta id");
                    break;
                }
                if (type == ""){
                    type="full";
                }
                if (fs == ""){
                    fs="2fs";
                }
                mkfs();
                
                continue;
            }
            
            if(tokens.at(index)=="rep"){
                Params();
                if (name == ""){
                    errores.push_back("REP: falta name");
                    break;
                }
                if (path == ""){
                    errores.push_back("REP: falta path");
                    break;
                }
                if (id == ""){
                    errores.push_back("REP: falta id");
                    break;
                }
                rep();
                
                continue;
            }
            if(tokens.at(index)=="pause"){
                Instruction i;
                i.comando="pause";
                instrucciones.push_back(i);
                continue;
            }
            if(tokens.at(index)=="@salto"){
                continue;
            }
            else{
                errores.push_back("Se esperaba MKDISK,RMDISK,FDISK,MOUNT,UNMOUNT o MKFS y se obtuvo:"+tokens[index]);
                break;
            } 
            ResetParams();
        }
        
        return instrucciones;
    }
    void Params(){
        while(true){
            //cout<<"==="+tokens[index]<<endl;
            if(index==tokens.size()-1){
                break;
            }
            index++;
            ToLower(tokens[index]);
            if(tokens[index]=="@salto"){
                break;
            }
            ToLower(tokens[index+1]);
            
            if(tokens.at(index)=="size"){
                index++;
                size = stoi(tokens.at(index));
                continue;
            }
            if(tokens.at(index)=="f"){
                index++;
                f_ = tokens.at(index);
                continue;
            }
            if(tokens.at(index)=="u"){
                index++;
                u_ = tokens.at(index);
                continue;
            }
            if(tokens.at(index)=="path"){
                index++;
                path = tokens.at(index);
                continue;
            }
            if(tokens.at(index)=="type"){
                index++;
                type = tokens.at(index);
                continue;
            }
            if(tokens.at(index)=="delete"){
                index++;
                delete_ = tokens.at(index);
                continue;
            }
            if(tokens.at(index)=="name"){
                index++;
                name = tokens.at(index);
                continue;
            }
            if(tokens.at(index)=="add"){
                index++;
                add = stoi(tokens.at(index));
                continue;
            }
            if(tokens.at(index)=="id"){
                index++;
                id = tokens.at(index);
                continue;
            }
            if(tokens.at(index)=="fs"){
                index++;
                fs = tokens.at(index);
                continue;
            }
            else{
                errores.push_back("Se esperaba parametro completo y se obtuvo:"+tokens[index]);
                break;
            }
        }
       // cout<<"SALIO DE ESTAAAAAAAAAAAA"<<endl;
    }

    void Split(){
        replace(txt.begin(), txt.end(), '\t', ' ');
        replace(txt.begin(), txt.end(), '\r', ' ');
        txt.push_back(' ');
        cadena="";
        for(int i= 0; i<txt.size(); i++){
            linea+=txt[i];
            if(txt[i]=='\n'){
                lineas.push_back(linea);
                Match();
                if(tokens[tokens.size()-1]!="@SALTO"){
                    tokens.push_back("@SALTO");
                }
                continue;
            }
            if(txt[i]==' '||txt[i]=='='){
                    Match();
                    continue;
            } 
            if(txt[i]=='"'){
                i++;
                while(txt[i]!='"'){
                    linea+=txt[i];
                    cadena+=txt[i];
                    i++;
                }
                Match();
                continue;
            }     
            if(txt[i]=='#'){
                while(txt[i]!='\n'){
                    if(i>txt.size()){
                        break;
                    }
                    i++;
                }
                i--;
            }
            else{
                if(txt[i]!='-'){
                    cadena+=txt[i];
                }
                if((txt[i]=='-'&&txt[i-1]=='=')||(txt[i]=='-'&&txt[i-2]=='=')){
                    cadena+=txt[i];
                }
            }
        }
    }
    void Match(){
        if(cadena!=""){
            tokens.push_back(cadena);
            cadena="";
        }
        linea="";
    }
    
    void rep(){
        Instruction ins;
        ins.comando="rep";
        ins.path = path;
        ins.id = id;
        ins.name = name;
        instrucciones.push_back(ins);
    }
    void mkdisk(){
        if(f_!="bf"&&f_!="ff"&& f_!="wf"){
            errores.push_back("MKDISK: valor de f incorrecto->"+f_);
        }
        if(u_!="m"&&u_!="k"){
            errores.push_back("MKDISK: valor de u incorrecto->"+u_);
        }
        if(errores.size()==0){
            Instruction ins;
            ins.comando="mkdisk";
            ins.size=size;
            ins.f_=f_;
            ins.u_=u_;
            ins.path=path;
            instrucciones.push_back(ins);
        }
        //errores.push_back("La operacion MKDISK neceista el parametro size");
    }
    void rmdisk(){
        Instruction ins;
        ins.comando="rmdisk";
        ins.path = path;
        instrucciones.push_back(ins);

    }
    void fdisk(){
    
        if(f_!="bf"&&f_!="ff"&& f_!="wf"){
            errores.push_back("FDISK: valor de f incorrecto->"+f_);
        }
        if(u_!="m"&& u_!="k"&&u_!="b"){
            errores.push_back("FDISK: valor de u incorrecto->"+u_);
        }
        if(type!="p"&&type!="e"&&type!="l"){
            errores.push_back("FDISK: valor de type incorrecto->"+type);
        }
        if(delete_!="fast"&&delete_!="full"&&size==-1&&add==-1){
            errores.push_back("FDISK: valor de delete incorrecto->"+delete_);
        }
        if(errores.size()==0){
            Instruction ins;
            ins.comando="fdisk";
            ins.size=size;
            ins.u_=u_;
            ins.path=path;
            ins.type=type;
            ins.f_=f_;
            ins.delete_=delete_;
            ins.name=name;
            ins.add=add;
            instrucciones.push_back(ins);
        }
    }
    void mount(){
        Instruction ins;
        ins.comando = "mount";
        ins.path = path;
        ins.name = name;
        instrucciones.push_back(ins);
    }
    void unmount(){
        Instruction ins;
        ins.comando = "unmount";
        ins.id = id;
        instrucciones.push_back(ins);
    }
    void mkfs(){
        
        if(fs!="2fs"&&fs!="3fs"){
            errores.push_back("MKFS: valor de fs incorrecto->"+fs);
        }
        if(type!="fast"&&type!="full"){
            errores.push_back("MKFS: valor de type incorrecto->"+type);
        }
        if(errores.size()==0){
            Instruction ins;
            ins.comando="mkfs";
            ins.id=id;
            ins.type=type;
            ins.fs=fs;
            instrucciones.push_back(ins);
        }
    }

    void getTokens(){
        cout<<"==========Lista Tokens ============"<<endl;
        for(int i=0; i<tokens.size();i++){
            //if(tokens[i]!="@SALTO"){
                cout<<tokens[i]<<endl;
            //}
        }
    }
    void getError(){
        cout<<"==========Lista Errores ============"<<endl;
        for(int i=0; i<errores.size();i++){
            cout<<errores[i]<<endl;
        }
        if(errores.size()==0){
            cout<<"*Sin errores*"<<endl;
        }
    }
    void Reset(){
        tokens.clear();
        errores.clear();
        lineas.clear();
        instrucciones.clear();
        index = 0;
        cadena="";
        linea="";
        ResetParams();

    }
    void ResetParams(){
        size=-1;
        f_ = "";
        u_ = "";
        path = "";
        type = "";
        delete_="";
        name = "";
        add = -1;
        id ="";
        fs="";
    }
    int getNumError(){
        return errores.size();
    }
    void setTxt(string text){
        txt = text;
    }
    void getParams(){
        cout<<"========== Params ============"<<endl;
        cout<<"size:"+to_string(size)<<endl;
        cout<<"f:"+f_<<endl;
        cout<<"u:"+u_<<endl;
        cout<<"path:"+path<<endl;
        cout<<"type:"+type<<endl;
        cout<<"delete:"+delete_<<endl;
        cout<<"name:"+name<<endl;
        cout<<"add:"+to_string(add)<<endl;
        cout<<"id:"+id<<endl;
        cout<<"fs:"+fs<<endl;



    }
    void ToLower(string &txt){
        for_each(txt.begin(), txt.end(), [](char & c) {
                c = ::tolower(c);
        });
    }
};