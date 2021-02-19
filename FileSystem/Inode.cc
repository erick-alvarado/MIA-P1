#include <iostream>
using namespace std;

class Inode{
   public:
    int i_uid;
    int i_gid;
    int i_size;
    time_t i_atime;
    time_t i_ctime;
    time_t i_mtime;
    int i_block;
    char i_type;
    int i_perm;
};
