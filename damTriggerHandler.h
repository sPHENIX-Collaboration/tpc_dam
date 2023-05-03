#ifndef __DAMTRIGGERHANDLER_H__
#define __DAMTRIGGERHANDLER_H__

#include "TriggerHandler.h"


class damTriggerHandler : public TriggerHandler {

public:

  damTriggerHandler(const int etype)
    {
      _etype = etype;
      //_poll_count = 0;
    };

  ~damTriggerHandler() {};

  int set_damfd ( const int dam_fd);
  //  int set_selftrigger(const int sleepinterval) { _self_trigger = sleepinterval;};

  int enable();

  // this is the virtual worker routine
  int wait_for_trigger( const int moreinfo=0);

 protected:
  
  int _dam_fd;
  int _etype;
  //int _poll_count;
};

#endif
