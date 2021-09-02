#include <iostream>
using namespace std;


class Code{
  
  public:
    int disk_code,partition_code;
    string name,path;
    Partition partition;
    Code(int dc, int pc,string n,string p,Partition pa){
      disk_code=dc;
      partition_code=pc;
      name = n;
      path = p;
      partition = pa;
    }
    
};
