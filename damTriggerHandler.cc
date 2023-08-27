
#include "damTriggerHandler.h"


#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>

using namespace std;

int damTriggerHandler::enable()
{
  return 0;
}


int damTriggerHandler::wait_for_trigger( const int moreinfo)
{

  struct timeval timeout;
  int sel;

  timeout.tv_sec = 2;
  timeout.tv_usec = 0;

  FD_ZERO(&_read_flags);

  if ( _dam_fd0 > 0)
    {
      //cout << __LINE__ << "  " << __FILE__ << " setting 1st fd" << endl;
      FD_SET(_dam_fd0, &_read_flags);
    }
  if ( _dam_fd1 > 0)
    {
      //cout << __LINE__ << "  " << __FILE__ << " setting 2nd fd" << endl;
      FD_SET(_dam_fd1, &_read_flags);
    }
  
  sel = select(_maxfd, &_read_flags, NULL, (fd_set*)0, &timeout);
  if (sel < 0)
    {
      perror("select()");
    }

  // cout << __LINE__ << "  " << __FILE__ << " fds: "  << _dam_fd0  << " " << _dam_fd1
  //      << " " << FD_ISSET(_dam_fd0, &_read_flags) << "  " <<  FD_ISSET(_dam_fd1, &_read_flags) << endl;


  if (FD_ISSET(_dam_fd0, &_read_flags) || FD_ISSET(_dam_fd1, &_read_flags) )
    {
      //cout << __LINE__ << "  " << __FILE__ << " trigger after " << _poll_count << " polls" << endl;
      //_poll_count = 0;
      return _etype;
    }
  return 0;

}

int damTriggerHandler::set_damfd( const int dam_fd0, const int dam_fd1)
{
  _dam_fd0 = dam_fd0;
  _dam_fd1 = dam_fd1;
  _maxfd  = max (_dam_fd0, _dam_fd1)+1;
  //cout << __FILE__ << " " << __LINE__ << " fds, ndfs" << _dam_fd0 << " " << _dam_fd1 << " " << _maxfd << endl; 
  return 0;
}
