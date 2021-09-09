#include <string>
#include <sstream>
#include "Graphviz.cc"
using namespace std;

class Report {  
    private:
    Graphviz g;
    Disk d;
    public:
    

    void getMbr(string path_mbr,string path_file){
        Mbr mbr = d.getMbr(path_mbr);
        if(mbr.mbr_tamano!=0){
            return;
        }
        string dot = "digraph{ \n";
        dot+= "tbl[ \n shape=plaintext \n label=< \n <table border='0' cellborder='1' color='black' cellspacing='0'> \n";
        dot+="<tr> <td bgcolor='grey' colspan='2'> MBR </td> </tr>\n";
        dot+="<tr><td>Nombre</td><td>Valor</td></tr>\n";

        string time_str = std::ctime(&mbr.mbr_fecha_creacion);
        time_str.resize(time_str.size() - 1);

        dot+="<tr><td>mbr_tamano</td><td>"+to_string(mbr.mbr_tamano)+ "</td></tr>\n";
        dot+="<tr><td>mbr_fecha_creacion</td><td>"+time_str+ "</td></tr>\n";
        dot+="<tr><td>mbr_disk_signature</td><td>"+to_string(mbr.mbr_disk_signature)+ "</td></tr>\n";

        string s;
          s.push_back(mbr.disk_fit);
        dot+="<tr><td>disk_fit</td><td>"+s+"</td></tr>\n";
        
        Ebr ebr;
        for(int i=0; i<4;i++){
            Partition p = mbr.particiones[i];
            
            if(p.part_type!='-'){
                dot+="<tr><td>part_name"+to_string(i+1)+"</td><td>"+p.part_name+ "</td></tr>\n";
                dot+="<tr><td>part_status"+to_string(i+1)+"</td><td>"+p.part_status+ "</td></tr>\n";
                dot+="<tr><td>part_type"+to_string(i+1)+"</td><td>"+p.part_type+ "</td></tr>\n";
                dot+="<tr><td>part_fit"+to_string(i+1)+"</td><td>"+p.part_fit+ "</td></tr>\n";
                dot+="<tr><td>part_start"+to_string(i+1)+"</td><td>"+to_string(p.part_start)+ "</td></tr>\n";
                dot+="<tr><td>part_size"+to_string(i+1)+"</td><td>"+to_string(p.part_size)+ "</td></tr>\n";

            }
            if(p.part_type=='e'){
                ebr = d.getEbr(path_mbr,p.part_start);
            }
        }
        dot+="\n </table> \n >]; \n ";
        if(ebr.part_status!='-'){
            int i =1;
            while(ebr.part_next!=-1){
                dot+= "tbl"+to_string(i)+"[ \n shape=plaintext \n label=< \n <table border='0' cellborder='1' color='black' cellspacing='0'> \n";
                dot+="<tr> <td bgcolor='grey' colspan='2'> EBR_"+to_string(i)+"</td> </tr>\n";
                dot+="<tr><td>Nombre</td><td>Valor</td></tr>\n";

                dot+="<tr><td>part_name"+to_string(i)+"</td><td>"+ebr.part_name+ "</td></tr>\n";
                dot+="<tr><td>part_status"+to_string(i)+"</td><td>"+ebr.part_status+ "</td></tr>\n";
                dot+="<tr><td>part_fit"+to_string(i)+"</td><td>"+ebr.part_fit+ "</td></tr>\n";
                dot+="<tr><td>part_start"+to_string(i)+"</td><td>"+to_string(ebr.part_start)+ "</td></tr>\n";
                dot+="<tr><td>part_size"+to_string(i)+"</td><td>"+to_string(ebr.part_size)+ "</td></tr>\n";
                dot+="<tr><td>part_next"+to_string(i)+"</td><td>"+to_string(ebr.part_next)+ "</td></tr>\n";
                dot+="\n </table> \n >]; \n ";
                ebr = d.getEbr(path_mbr,ebr.part_next);
                i++;
            }
            dot+= "tbl"+to_string(i)+"[ \n shape=plaintext \n label=< \n <table border='0' cellborder='1' color='black' cellspacing='0'> \n";
            dot+="<tr> <td bgcolor='grey' colspan='2'> EBR_"+to_string(i)+"</td> </tr>\n";
            dot+="<tr><td>Nombre</td><td>Valor</td></tr>\n";
            dot+="<tr><td>part_name"+to_string(i)+"</td><td>"+ebr.part_name+ "</td></tr>\n";
            dot+="<tr><td>part_status"+to_string(i)+"</td><td>"+ebr.part_status+ "</td></tr>\n";
            dot+="<tr><td>part_fit"+to_string(i)+"</td><td>"+ebr.part_fit+ "</td></tr>\n";
            dot+="<tr><td>part_start"+to_string(i)+"</td><td>"+to_string(ebr.part_start)+ "</td></tr>\n";
            dot+="<tr><td>part_size"+to_string(i)+"</td><td>"+to_string(ebr.part_size)+ "</td></tr>\n";
            dot+="<tr><td>part_next"+to_string(i)+"</td><td>"+to_string(ebr.part_next)+ "</td></tr>\n";
            dot+="\n </table> \n >]; \n";
            
        }
        dot+="}";
        g.GenerateGraph("MBR",path_file,dot);
    }
    void getDsk(string path_mbr,string path_file){
        Mbr mbr = d.getMbr(path_mbr);
        
        if(mbr.mbr_tamano==0){
            return;
        }


        string dot = "digraph test {\n parent [ \n shape=plaintext \n label=< \n <table color='grey' border='1' cellborder='1' cellspacing='5'> \n ";
        string titles ="<tr>\n";
        string ebr = "";
        string values = "<tr>";

        titles+="<td rowspan='3'>MBR</td>\n";

        for(int i =0; i<4; i++){
            Partition p = mbr.particiones[i];
            if(p.part_type=='e'){
                ebr+="<tr>\n";
                int colspan =0;
                Ebr e = d.getEbr(path_mbr,p.part_start);
                while(e.part_next!=-1){
                    ebr+="<td rowspan ='2'>Ebr</td>\n";
                    double size = e.part_size;
                    if(e.part_status!='-'){
                        ebr+="<td>Logica</td>\n";
                        values+="<td>"+to_string(size)+"</td>\n";
                        if(e.part_start+e.part_size!=e.part_next ){
                            ebr+="<td>Libre</td>\n";
                            if(e.part_next==-1){
                                size =sizeof(Mbr)+  p.part_size-(e.part_start+e.part_size);
                            }
                            else{
                                size = e.part_next-(e.part_start+e.part_size);
                            }
                            values+="<td>"+to_string(size)+"</td>\n";
                            colspan++;
                        }
                    }
                    else{
                        ebr+="<td>Libre</td>\n";
                        size = e.part_next-e.part_start;
                        values+="<td>"+to_string(size)+"</td>\n";
                    }
                    colspan+=2;
                    e = d.getEbr(path_mbr,e.part_next);
                }
                ebr+="<td rowspan ='2'>Ebr</td>\n";
                    double size = e.part_size;
                    if(e.part_status!='-'){
                        ebr+="<td>Logica</td>\n";
                        values+="<td>"+to_string(size)+"</td>\n";
                        if(e.part_start+e.part_size!=e.part_next ){
                            ebr+="<td>Libre</td>\n";
                            if(e.part_next==-1){
                                size =sizeof(Mbr)+ p.part_size-(e.part_start+e.part_size);
                            }
                            else{
                                size = e.part_next-(e.part_start+e.part_size);
                            }
                            values+="<td>"+to_string(size)+"</td>\n";
                            colspan++;
                        }
                    }
                    else{
                        ebr+="<td>Libre</td>\n";
                        size = e.part_next-e.part_start;
                        values+="<td>"+to_string(size)+"</td>\n";
                    }
                    colspan+=2;
                titles+="<td colspan='"+to_string(colspan)+"'>Extended</td>\n";
                ebr+="</tr>\n";
            }
            else{
                double size = p.part_size;
                if(p.part_status=='o'){
                    titles+="<td rowspan='2'>Primaria</td>\n";

                }
                else{
                    titles+="<td rowspan='2'>Libre</td>\n";
                    if(i==3){
                        size =sizeof(Mbr)+ mbr.mbr_tamano-(mbr.particiones[2].part_start+mbr.particiones[2].part_size);
                    }
                }
                values+="<td>"+to_string(size)+"</td>\n";
            }
            
        }




        titles+="</tr>\n";
        values+="</tr>\n";

        dot+=titles;
        dot+=ebr;
        dot+=values;
        dot+="</table>\n >]; \n }";
        g.GenerateGraph("DSK",path_file,dot);

    }
};