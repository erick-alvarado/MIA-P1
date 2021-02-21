#include <iostream>
using namespace std;
#include "Partition.cc"


class Mbr{
  public:
    int mbr_tamano=0;
    time_t mbr_fecha_creacion;
    int mbr_disk_signature;
    char disk_fit;
    Partition mbr_partition_1;
    Partition mbr_partition_2;
    Partition mbr_partition_3;
    Partition mbr_partition_4;
};
