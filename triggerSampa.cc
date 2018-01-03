#include "pl_lib.h"
#include "fee_reg.h"


#include <iostream>
#include <sys/types.h>
#include <stdint.h>

#include <unistd.h>

using namespace std;


int main()
{

  int _dam_fd;
  
  uint16_t reg5 = 0x8;
  int rc;

  pl_open(&_dam_fd);

  
  rc = fee_reg_write(_dam_fd, 5, reg5);
  
  // and we are clearing it again
  reg5 = 0;
  rc = fee_reg_write(_dam_fd, 5, reg5);
  return 0;
}
