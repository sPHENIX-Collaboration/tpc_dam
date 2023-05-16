#ifndef __DAQ_DEVICE_DAM__
#define __DAQ_DEVICE_DAM__


#include <daq_device.h>
#include <stdio.h>
#include <damTriggerHandler.h>
#include <string>

class Fee;

class daq_device_dam: public  daq_device {


public:

  daq_device_dam(const int eventtype
		 , const int subeventid
		 , const int trigger = 1
		 , const int nunits=1
		 , const int npackets=1
     , const std::string & damDevName = "/dev/dam0"
     );

  ~daq_device_dam();


  void identify(std::ostream& os = std::cout) const;

  int max_length(const int etype) const;

  // functions to do the work

  int put_data(const int etype, int * adr, const int length);

  int init();

  int rearm( const int etype);
  
  int endrun();
  
 protected:

  int _dam_fd;
  
  int _broken;

  int _trigger;

  int _length;
  int _nunits;
  int _npackets;
  
  damTriggerHandler *_th;

  std::string m_deviceName;
};


#endif
