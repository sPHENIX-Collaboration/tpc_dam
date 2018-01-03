#ifndef __TPCFEETRIGGERHANDLER_H__
#define __TPCFEETRIGGERHANDLER_H__

#include "TriggerHandler.h"

class Fee;

class tpcfeeTriggerHandler : public TriggerHandler {

public:

  tpcfeeTriggerHandler(const int etype)
    {
      _etype = etype;
    };

  int set_fee( Fee *f)
  {
    _fee = f;
    return 0;
  };

  int clear_fee()
  {
    _fee = 0;
    return 0;
  };

  ~tpcfeeTriggerHandler() {};

  // this is the virtual worker routine
  int wait_for_trigger( const int moreinfo=0);

 protected:
  
  Fee *_fee;
  int _etype;
};

#endif
