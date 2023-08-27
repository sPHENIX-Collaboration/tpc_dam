#ifndef __DAQ_DEVICE_DAM__
#define __DAQ_DEVICE_DAM__


#include <daq_device.h>
#include <stdio.h>
#include <damTriggerHandler.h>

class Fee;

class daq_device_dam: public  daq_device {


public:

  daq_device_dam(const int eventtype
		 , const int subeventid0
		 , const int subeventid1 = 0
		 , const int nunits = 0
		 , const int trigger = 1);


  ~daq_device_dam();


  void identify(std::ostream& os = std::cout) const;

  int max_length(const int etype) const;

  // functions to do the work

  int put_data(const int etype, int * adr, const int length);

  int init();

  int rearm( const int etype);
  
  int endrun();

  
 protected:

  int add_packet(int fd, int * adr, const int length, const int ipacket );

  
  int _subeventid0;
  int _subeventid1;
  
  int _dam_fd0;
  int _dam_fd1;
  
  int _broken;

  int _trigger;

  int _nunits;
  int _npackets;   // we keep track if we have 1 or 2 packets
  int _desired_data_length;   // data length per packet
  
  damTriggerHandler *_th;


};


#endif
