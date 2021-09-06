#include <string>
#include <sstream>
#include "Graphviz.cc"
using namespace std;

class Report {  
    private:
    Graphviz g;
    public:
    

    void getMbr(Mbr mbr,string path){
        string dot = "digraph{ \n tbl[ \n shape=plaintext \n label=< \n <table border='0' cellborder='1' color='orange' cellspacing='0'> \n";
        
        dot+="<tr><td>Nombre</td><td>Valor</td></tr>\n";


        string time_str = std::ctime(&mbr.mbr_fecha_creacion);
        time_str.resize(time_str.size() - 1);

        dot+="<tr><td>mbr_tamano</td><td>"+to_string(mbr.mbr_tamano)+ "</td></tr>\n";
        dot+="<tr><td>mbr_fecha_creacion</td><td>"+time_str+ "</td></tr>\n";
        dot+="<tr><td>mbr_disk_signature</td><td>"+to_string(mbr.mbr_disk_signature)+ "</td></tr>\n";

        string s;
          s.push_back(mbr.disk_fit);
        dot+="<tr><td>disk_fit</td><td>"+s+"</td></tr>\n";
        
        for(int i=0; i<4;i++){
            Partition p = mbr.particiones[i];
            if(p.part_type!='-'){
                dot+="<tr><td>part_status"+to_string(i+1)+"</td><td>"+p.part_status+ "</td></tr>\n";
                dot+="<tr><td>part_type"+to_string(i+1)+"</td><td>"+p.part_type+ "</td></tr>\n";
                dot+="<tr><td>part_fit"+to_string(i+1)+"</td><td>"+p.part_fit+ "</td></tr>\n";
                dot+="<tr><td>part_start"+to_string(i+1)+"</td><td>"+to_string(p.part_start)+ "</td></tr>\n";
                dot+="<tr><td>part_size"+to_string(i+1)+"</td><td>"+to_string(p.part_size)+ "</td></tr>\n";
                dot+="<tr><td>part_name"+to_string(i+1)+"</td><td>"+p.part_name+ "</td></tr>\n";
            }
            
        }

        dot+="\n </table> \n >]; \n }";
        g.GenerateGraph("MBR",path,dot);
    }
    void getDsk(Mbr mbr, string path){
        string dot = "";

    }
};