#include <iostream>
using namespace std;


class Partition{
 public:
   char part_status='f';// f- free , o - occuppied 
   char part_type='-';
   char part_fit='-';
   int part_start=0;
   int part_size=0;
   char part_name[16];
};