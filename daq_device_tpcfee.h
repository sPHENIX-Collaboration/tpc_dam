#ifndef __DAQ_DEVICE_TPCFEE__
#define __DAQ_DEVICE_TPCFEE__


#include <daq_device.h>
#include <stdio.h>
#include <tpcfeeTriggerHandler.h>

class Fee;

class daq_device_tpcfee: public  daq_device {


public:

  daq_device_tpcfee(const int eventtype
		    , const int subeventid
		    , const int trigger = 1);

  ~daq_device_tpcfee();


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
  
  Fee *_fee;

  tpcfeeTriggerHandler *_th;

};


#endif
