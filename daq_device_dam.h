#ifndef __DAQ_DEVICE_DAM__
#define __DAQ_DEVICE_DAM__


#include <daq_device.h>
#include <stdio.h>
#include <damTriggerHandler.h>


class daq_device_dam: public  daq_device {


public:

  daq_device_dam(const int eventtype
		 , const int subeventid
		 , const int trigger = 1
		 , const int self_trigger = 0);

    ~daq_device_dam();


  void identify(std::ostream& os = std::cout) const;

  int max_length(const int etype) const;

  // functions to do the work

  int put_data(const int etype, int * adr, const int length);

  int init();

  int rearm( const int etype);
  
  int endrun();

  
 protected:


  int _broken;

  int _trigger;
  int _self_trigger;
  
  int  _dam_fd;


  damTriggerHandler *_th;

};


#endif
