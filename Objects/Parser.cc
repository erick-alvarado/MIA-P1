#include <iostream>
#include <vector>
#include <cctype>
#include <string>
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
                string myText;
                ifstream File(path);
                if(File){
                    while (getline (File, myText)) {
                        cout << myText;
                    }
                    File.close();
                }
                else{
                    cout<<"Direccion invalida"<<endl;
                }
                
                txt=myText;
                cout<<txt;
                //Reset();
                //Parse();      
            }
        }
        else{
            errores.push_back("Se esperaba exec y se obtuvo:"+tokens[0]);
        }   
        getError();
    }

    void Parse(){
        Split();
        for(index = 0; index < tokens.size() ; index ++)
        {
            if(tokens.at(index)=="MKDISK"){
            }
            if(tokens.at(index)=="RMDISK"){
            }
            if(tokens.at(index)=="FDISK"){
            }
            if(tokens.at(index)=="MOUNT"){
            }
            if(tokens.at(index)=="UNMOUNT"){
            }
            if(tokens.at(index)=="MKFS"){
            }

            cout<<tokens[index]+"|";

        }
    }
    void Params(){
        while(tokens[index]!="$"){
            index++;
            if(tokens.size()-1==index||tokens.size()==index){
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
            }
        }
    }
    
    void Split(){
        replace(txt.begin(), txt.end(), '\t', ' ');
        replace(txt.begin(), txt.end(), '\r', ' ');
        replace(txt.begin(), txt.end(), '\n', '$');
        txt.push_back(' ');

        for(int i= 0; i<txt.size(); i++){
            txt[i]= tolower(txt[i]);
            linea+=txt[i];
            if(txt[i]=='$'){
                lineas.push_back(linea);
                Match();
                tokens.push_back("$");
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
    void getTokens(){
        cout<<"==========Lista Tokens ============"<<endl;
        for(int i=0; i<tokens.size();i++){
            cout<<tokens[i]<<endl;;
        }
    }
    void getError(){
        cout<<"==========Lista Errores ============"<<endl;
        for(int i=0; i<errores.size();i++){
            cout<<errores[i]<<endl;
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
        txt;
    }
    int getNumError(){
        return errores.size();
    }
    void setTxt(string text){
        txt = text;
    }


};