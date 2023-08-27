#ifndef __DAMTRIGGERHANDLER_H__
#define __DAMTRIGGERHANDLER_H__

#include "TriggerHandler.h"
#include <sys/types.h>
#include <unistd.h>


class damTriggerHandler : public TriggerHandler {

public:

  damTriggerHandler(const int etype)
    {
      _etype = etype;
      FD_ZERO(&_read_flags);
      //_poll_count = 0;
    };

  ~damTriggerHandler() {};

  int set_damfd ( const int dam_fd0, const int dam_fd1);
  
  //  int set_selftrigger(const int sleepinterval) { _self_trigger = sleepinterval;};

  int enable();

  // this is the virtual worker routine
  int wait_for_trigger( const int moreinfo=0);

  fd_set get_readflags() const { return _read_flags;};
  
 protected:
  
  int _dam_fd0;
  int _dam_fd1;
  int _maxfd;
  fd_set _read_flags;
  
  int _etype;
  //int _poll_count;
};

#endif
