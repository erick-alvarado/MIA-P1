#include <iostream>
#include <vector>
#include <cctype>
#include <string>
#include <fstream>
#include <streambuf>
#include <algorithm>
using namespace std;

class Parser{
    vector<string> tokens;
    vector<string> errores;
    vector<string> lineas;
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
    
    
    void Exec(){
        Split();
        getTokens();
        if(tokens.size()>0 && tokens[0]=="exec"){
            Params();
            if(path==""){
                errores.push_back("Para ejecutar exec se necesita el parametro path completo");

            }
            else{
                ifstream File(path);
                string str((std::istreambuf_iterator<char>(File)),
                istreambuf_iterator<char>());
                txt="";
                if(File){
                    txt=str;
                    Reset();
                    Parse();
                }
                else{
                    cout<<"Direccion invalida"<<endl;
                } 
            }
        }
        else{
            errores.push_back("Se esperaba exec y se obtuvo:"+tokens[0]);
        }   
    }

    void Parse(){
        Split();
        getTokens();
        
        for(index = 0; index < tokens.size() ; index ++)
        {
            cout<<tokens[index]<<endl;
            if(tokens.at(index)=="mkdisk"){
                Params();
                if(size==-1){
                    errores.push_back("La operacion MKDISK neceista el parametro size");
                    break;
                }
                if(f_==""){
                    f_="ff";
                }
                if(u_==""){
                    u_="m";
                }
                if (path == ""){
                    errores.push_back("La operacion MKDISK necesita el parametro path");
                    break;
                }
                mkdisk();
                ResetParams();
                continue;
            }
            if(tokens.at(index)=="rmdisk"){
                Params();
                if (path == ""){
                    errores.push_back("La operacion rmdisk necesita el parametro path");
                    break;
                }
                rmdisk();
                ResetParams();
                continue;
            }
            if(tokens.at(index)=="fdisk"){
                Params();
                if(size==-1 && delete_=="" && add==-1){
                    //OBLIGATORIO AL CREAR
                    errores.push_back("La operacion fdisk neceista el parametro size");
                    break;
                }
                if(u_==""){
                    u_="k";
                }
                if (path == ""){
                    errores.push_back("La operacion fdisk necesita el parametro path");
                    break;
                }
                if (type == ""){
                    type="p";
                }
                if(f_==""){
                    f_="wf";
                }
                if (name == ""){
                    errores.push_back("La operacion fdisk necesita el parametro name");
                    break;
                }
                fdisk();
                ResetParams();
                continue;
            }
            if(tokens.at(index)=="mount"){
                Params();
                if (path == ""){
                    errores.push_back("La operacion mount necesita el parametro path");
                    break;
                }
                if (name == ""){
                    errores.push_back("La operacion mount necesita el parametro name");
                    break;
                }
                mount();
                ResetParams();
                continue;
            }
            if(tokens.at(index)=="unmount"){
                Params();
                if (id == ""){
                    errores.push_back("La operacion unmount necesita el parametro id");
                    break;
                }
                unmount();
                ResetParams();
                continue;
            }
            if(tokens.at(index)=="mkfs"){
                Params();
                if (id == ""){
                    errores.push_back("La operacion mkfs necesita el parametro id");
                    break;
                }
                if (type == ""){
                    type="full";
                }
                if (fs == ""){
                    fs="2fs";
                }
                mkfs();
                ResetParams();
                continue;
            }
            if(tokens.at(index)=="@SALTO"){
                continue;
            }
            else{
                errores.push_back("Se esperaba MKDISK,RMDISK,FDISK,MOUNT,UNMOUNT o MKFS y se obtuvo:"+tokens[index]);
                break;
            } 
        }
        getError();
    }
    void Params(){
        while(true){
            //cout<<"==="+tokens[index]<<endl;
            if(index==tokens.size()-1){
                break;
            }
            index++;
            if(tokens[index]=="@SALTO"){
                break;
            }
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

        for(int i= 0; i<txt.size(); i++){
            txt[i]= tolower(txt[i]);
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
    
    void mkdisk(){
        cout<<"Se creara un mkdisk con los siguientes parametros:"+to_string(size) + "|"+f_+ "|"+u_+ "|"+path<<endl;
    }
    void rmdisk(){
        cout<<"Se eliminara un rmdisk con los siguientes parametros:"+path;

    }
    void fdisk(){
        cout<<"Se creara un fdisk con los siguientes parametros:"+to_string(size)+ "|" +u_+ "|"+path+ "|"+type+ "|"+f_+ "|"+delete_+ "|"+name+ "|"+to_string(add)<<endl;
    }
    void mount(){
        cout<<"Se creara un mount con los siguientes parametros:"+path+"|"+name<<endl;
    }
    void unmount(){
        cout<<"Se creara un unmount con los siguientes parametros:"+id<<endl;
    }
    void mkfs(){
        cout<<"Se creara un mkfs con los siguientes parametros:"+id+"|"+type+"|"+fs<<endl;
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

};