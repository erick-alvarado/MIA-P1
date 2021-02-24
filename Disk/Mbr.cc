#include <iostream>
using namespace std;
#include "Partition.cc"


class Mbr{
  public:
    int mbr_tamano=0;
    time_t mbr_fecha_creacion;
    int mbr_disk_signature;
    char disk_fit;
    Partition particiones[4];
};
