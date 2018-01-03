
#include "damTriggerHandler.h"

#include "fee_reg.h"


#include <iostream>
#include <sys/types.h>
#include <stdint.h>

#include <unistd.h>

using namespace std;


int damTriggerHandler::wait_for_trigger( const int moreinfo)
{

  uint16_t reg5 = 0x8;
  int rc;
  
  if ( _self_trigger) 
    {
      usleep (_self_trigger*1000 );
      
      // we initiate a SAMPA read by setting bit 4 in reg 5
      rc = fee_reg_write(_dam_fd, 5, reg5);
      
      // and we are clearing it again
      reg5 = 0;
      rc = fee_reg_write(_dam_fd, 5, reg5);

      // now we poll until bit 1 comes on again
      int count = 0;
      
      rc = fee_reg_read(_dam_fd, 5, &reg5);
      while ( ! (reg5 & 0x1) )
	{
	  count++;
	  rc = fee_reg_read(_dam_fd, 5, &reg5);
	}
      
      cout << __LINE__ << "  " << __FILE__ << " trigger after " << count << " polls" << endl;
      return _etype;
    }


  // if we are not running in self-trigger mode
  
  int count = 0;
  const int pollcount = 10000;
  
  rc = fee_reg_read(_dam_fd, 5, &reg5);
  while ( ( ! (reg5 & 0x1) ) && count < pollcount )
    {
      count++;
      rc = fee_reg_read(_dam_fd, 5, &reg5);
    }

  if ( count >= pollcount) return 0;  // empty trigger
  cout << __LINE__ << "  " << __FILE__ << " trigger after " << count << " polls  reg 5 value = " << hex << reg5 << dec << endl;
  return _etype;

}

int damTriggerHandler::set_damfd( const int dam_fd)
{
  _dam_fd = dam_fd;
  return 0;
}
