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
        //Spaces particiones MBR
        vector<Space> sp = d.getSpaces(mbr.mbr_tamano,mbr.particiones,1);
        for(Partition p: mbr.particiones){
            if(p.part_status=='o'){
                Space space;
                if(p.part_type=='e'){
                    space.name="ext$";
                }
                else{
                    space.name="Primary";
                }
                space.start=p.part_start;
                space.size= p.part_size;
                sp.push_back(space);
            }
            
        }        
        sort(sp.begin(), sp.end(), [](Space const & a, Space const & b) -> bool{ return a.start< b.start; } );
        

        for(Space s:sp){
            if(s.name=="ext$"){
                int colspan = 2;
                Ebr e = d.getEbr(path_mbr,s.start);
                vector<Space> sp_ebr = d.getSpaces(e, s.size, path_mbr, 1);
                
                //Obtener espacios y ebrs
                while(e.part_next!=-1){
                    if(e.part_status!='-'){
                        Space space;
                        space.name="Logic";
                        space.size=e.part_size;
                        space.start= e.part_start;
                        sp_ebr.push_back(space);

                        colspan+=2;
                    }
                    e= d.getEbr(path_mbr,e.part_next);
                }
                Space space;
                space.name = "Logic";
                space.size = e.part_size;
                space.start = e.part_start;
                sp_ebr.push_back(space);

                sort(sp_ebr.begin(), sp_ebr.end(), [](Space const & a, Space const & b) -> bool{ return a.start< b.start; } );


                ebr+="<tr>";
                for(Space x: sp_ebr){
                    if(x.name=="Logic"){
                        ebr+="<td rowspan ='2'>Ebr</td>\n";
                    }
                    else if(x.name==""){
                        x.name = "Unoccupied";
                    }
                    if(x.name=="Unoccupied"){
                        colspan++;
                    }
                    ebr+="<td>"+x.name+"</td>\n";
                    values+="<td>"+to_string(x.size)+"</td>\n";
                }
                ebr+="</tr>";

                titles += "<td colspan='" + to_string(colspan) + "'> EXT </td>\n";
            }
            else{
                if(s.name==""){
                    s.name="Unoccupied";
                }
                titles+="<td rowspan='1'>"+s.name+"</td>\n";
                values+="<td>"+to_string(s.size)+"</td>\n";

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