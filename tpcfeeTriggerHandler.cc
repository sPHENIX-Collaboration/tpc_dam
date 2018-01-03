
#include "tpcfeeTriggerHandler.h"

#include "fee_reg.h"
#include "fee.h"


#include <iostream>
#include <sys/types.h>
#include <stdint.h>

#include <unistd.h>

using namespace std;


int tpcfeeTriggerHandler::wait_for_trigger( const int moreinfo)
{

  if ( ! _fee) return 0;
  
  _fee->trigger();
  return _etype;

}

