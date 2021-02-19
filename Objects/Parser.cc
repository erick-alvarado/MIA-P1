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

                    cout<<"este es el texto:"+txt;
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
                    errores.push_back("La operacion MKDISK neceista el parametro path");
                    break;
                }
                mkdisk();
                ResetParams();
                continue;
            }
            if(tokens.at(index)=="rmdisk"){
                continue;
            }
            if(tokens.at(index)=="fdisk"){
                continue;
            }
            if(tokens.at(index)=="mount"){
                continue;
            }
            if(tokens.at(index)=="unmount"){
                continue;
            }
            if(tokens.at(index)=="mkfs"){
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
        cout<<"Se creara un disco con los siguientes parametros:"+to_string(size) + "|"+f_+ "|"+u_+ "|"+path<<endl;
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


};