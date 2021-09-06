#include <iostream>
using namespace std;

class Ebr{
  public:
    char part_status='-';
    char part_fit='-';
    int part_start=0;
    int part_size=0;
    int part_next=-1;
    char part_name[16];
};