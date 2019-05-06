
#include <iostream>

#include <daq_device_dam.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>

#include "pl_lib.h"
#include "fee_reg.h"


using namespace std;


daq_device_dam::daq_device_dam(const int eventtype
			       , const int subeventid
			       , const int trigger
			       , const int self_trigger
			       )
{

  m_eventType  = eventtype;
  m_subeventid = subeventid;

  _trigger = trigger;
  _self_trigger = self_trigger;
  _broken = 0;

 
  if ( _trigger )
    {
      //  cout << __LINE__ << "  " << __FILE__ << " registering triggerhandler " << endl;
      _th  = new damTriggerHandler (m_eventType);
      _th->set_damfd( _dam_fd);
      if (_self_trigger ) _th->set_selftrigger(_self_trigger);
      
      registerTriggerHandler ( _th);
    }
  else
    {
      _th = 0;
    }

}

daq_device_dam::~daq_device_dam()
{

  if (_th)
    {
      clearTriggerHandler();
      delete _th;
      _th = 0;
    }
}


int  daq_device_dam::init()
{

  if ( _broken ) 
    {
      //      cout << __LINE__ << "  " << __FILE__ << " broken ";
      //      identify();
      return 0; //  we had a catastrophic failure
    }

  _broken = 0;
  pl_open(&_dam_fd);

  if ( _dam_fd < 0)
    {
      _broken = 1;
      return -1;
    }

  if ( _trigger )
    {
      _th->set_damfd( _dam_fd);
    }


  uint16_t reg4 =4;
  int rc = fee_reg_write(_dam_fd, reg4, 1);
  rc = fee_reg_write(_dam_fd, reg4, 0);

  
  return 0;

}

// the put_data function

int daq_device_dam::put_data(const int etype, int * adr, const int length )
{


  cout << __LINE__ << "  " << __FILE__ << " starting put_data "  << endl;


  if ( _broken ) 
    {
      //      cout << __LINE__ << "  " << __FILE__ << " broken ";
      //      identify();
      return 0; //  we had a catastrophic failure
    }

  int len = 0;

  if (etype != m_eventType )  // not our id
    {
      return 0;
    }

  
  subevtdata_ptr sevt =  (subevtdata_ptr) adr;
  // set the initial subevent length
  sevt->sub_length =  SEVTHEADERLENGTH;

  // update id's etc
  sevt->sub_id =  m_subeventid;
  sevt->sub_type=4;    //++++
  sevt->sub_decoding = 99;  /++++++
  sevt->reserved[0] = 0;
  sevt->reserved[1] = 0;


  uint16_t reg5;

  uint16_t *dest = (uint16_t *) &sevt->data;
  //++++++ here starts the magic code

  
  int wordcount = 0;
  
  int max = max_length(etype) - SEVTHEADERLENGTH - 4; // safety belt for the length

  
  int rc = fee_reg_read(_dam_fd, 5, &reg5);

  //  cout << __LINE__ << "  " << __FILE__ << " reg5  "  << hex << reg5 << dec << endl;

  while ( ( reg5 & 0x1) && wordcount < max )
    {

      uint8_t a = 0x6;
      rc = fee_reg_read(_dam_fd, a, dest++);

      if (rc != 0) 
	{
	  break;
	}
      wordcount++;

      rc = fee_reg_read(_dam_fd, 5, &reg5);
    }

  //cout << __LINE__ << "  " << __FILE__ << " done reading data  "  << wordcount << endl;

  // since we are a type 2 packer, we align in units of "shorts"
  sevt->sub_padding = 8-(wordcount % 8);
  if ( sevt->sub_padding == 8) sevt->sub_padding = 0;
  
  sevt->sub_length += (wordcount + sevt->sub_padding) /2;
  // cout << __LINE__ << "  " << __FILE__ << " returning "  << sevt->sub_length << endl;

  return  sevt->sub_length;
}


void daq_device_dam::identify(std::ostream& os) const
{
  if ( _broken) 
    {
      os << "FELIX DAM Card  Event Type: " << m_eventType 
	 << " Subevent id: " << m_subeventid 
	 << " ** not functional ** " << endl;
    }
  else
    {
      os << "FELIX DAM Card  Event Type: " << m_eventType 
	 << " Subevent id: " << m_subeventid; 
      if (_trigger) os << " *Trigger*" ;
      if (_self_trigger) os << "  self-triggering every " << _self_trigger << " ms";
      os << endl;

    }
}

int daq_device_dam::max_length(const int etype) const
{
  if (etype != m_eventType) return 0;
  return  (128*1024 + SEVTHEADERLENGTH + 4);
}


// the rearm() function
int  daq_device_dam::rearm(const int etype)
{
  //  if ( _broken ) 
  //  {
      //      cout << __LINE__ << "  " << __FILE__ << " broken ";
      //      identify();
  //    return 0; //  we had a catastrophic failure
  //  }

  //  if (etype != m_eventType) return 0;


  return 0;
}

int daq_device_dam::endrun() 
{

  close (_dam_fd);
  
  return 0;
}
