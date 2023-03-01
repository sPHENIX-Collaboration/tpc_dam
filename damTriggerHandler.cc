
#include "damTriggerHandler.h"


#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>

#define DATA_LENGTH          137*256 // 137 words * 256 channels
#define FEE_SAMPA_CTRL       0x5
#define DAM_DMA_FIFO         0x5
#define DAM_DMA_CTRL         0x4004
#define DAM_DMA_BURST_LENGTH 0x4002
#define DAM_DMA_WRITE_PTR    0x4003

#include "pl_lib.h"

using namespace std;

int damTriggerHandler::enable()
{
  return 0;
}


int damTriggerHandler::wait_for_trigger( const int moreinfo)
{

  fd_set read_flags;
  struct timeval timeout;
  int sel;

  timeout.tv_sec = 4;
  timeout.tv_usec = 0;
  FD_ZERO(&read_flags);
  FD_SET(_dam_fd, &read_flags);

  sel = select(_dam_fd+1, &read_flags, NULL, (fd_set*)0, &timeout);
  if (sel < 0)
    {
      perror("select()");
    }

  _poll_count++;
  
  if (FD_ISSET(_dam_fd, &read_flags))
    {
      cout << __LINE__ << "  " << __FILE__ << " trigger after " << _poll_count << " polls" << endl;
      _poll_count = 0;
      return _etype;
    }
  
  return 0;

}

int damTriggerHandler::set_damfd( const int dam_fd)
{
  _dam_fd = dam_fd;
  return 0;
}
