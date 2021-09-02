#include <string>
#include <fstream>
using namespace std;

class Graphviz {  
    public:
    void GenerateGraph(string file_name, string dot_text) { 
        ofstream MyFile(file_name+".dot");
        MyFile << dot_text;
        MyFile.close();

        string com = "dot -Tpng "+ file_name +".dot -o "+ file_name +".png";
        const char *cmd = com.c_str();
        system(cmd);
    }
};