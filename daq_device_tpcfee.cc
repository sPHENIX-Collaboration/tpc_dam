
#include <iostream>

#include <daq_device_tpcfee.h>
#include <fee.h>

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>


using namespace std;


daq_device_tpcfee::daq_device_tpcfee(const int eventtype
			       , const int subeventid
			       , const int trigger)
{

  m_eventType  = eventtype;
  m_subeventid = subeventid;

  _trigger = trigger;
  _broken = 0;

  _fee = 0;  // get a clean start, nothing set
  
  if ( _trigger )
    {
      //  cout << __LINE__ << "  " << __FILE__ << " registering triggerhandler " << endl;
      _th  = new tpcfeeTriggerHandler (m_eventType);
      
      registerTriggerHandler ( _th);
    }
  else
    {
      _th = 0;
    }

}

daq_device_tpcfee::~daq_device_tpcfee()
{

  if ( _fee) delete _fee;
  
  if (_th)
    {
      clearTriggerHandler();
      delete _th;
      _th = 0;
    }
}


int  daq_device_tpcfee::init()
{

  if ( _broken ) 
    {
      //      cout << __LINE__ << "  " << __FILE__ << " broken ";
      //      identify();
      return 0; //  we had a catastrophic failure
    }

  _broken = 0;

  if ( ! _fee) _fee = new Fee(0);
  
  if ( _trigger && _th)
    {
      _th->set_fee ( _fee);
    }

  _fee->setupSampa();

  return 0;

}

// the put_data function

int daq_device_tpcfee::put_data(const int etype, int * adr, const int length )
{


  //  cout << __LINE__ << "  " << __FILE__ << " starting put_data "  << endl;


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
  sevt->sub_type=4;
  sevt->sub_decoding = 97;
  sevt->reserved[0] = 0;
  sevt->reserved[1] = 0;


  int fee_length = _fee->getDataLength();
  uint32_t *data = _fee->getRawData();
  
  if ( fee_length >= length) 
    {
      cout << __LINE__ << "  " << __FILE__ << " wrong data length, max is  " << length
	   << " got " << fee_length << endl;
      return 0;
    }
  
  memcpy ( &sevt->data, data, fee_length*sizeof(uint32_t));
	   
  sevt->sub_padding = 4-(fee_length % 4);
  if ( sevt->sub_padding == 4) sevt->sub_padding = 0;
  
  sevt->sub_length += fee_length + sevt->sub_padding;
  //cout << __LINE__ << "  " << __FILE__ << " returning "  << sevt->sub_length << endl;
  
  return  sevt->sub_length;
}


void daq_device_tpcfee::identify(std::ostream& os) const
{
  if ( _broken) 
    {
      os << "TPC 2-SAMPA FEE Board  Event Type: " << m_eventType 
	 << " Subevent id: " << m_subeventid 
	 << " ** not functional ** " << endl;
    }
  else
    {
      os << "TPC 2-SAMPA FEE Board  Event Type: " << m_eventType 
	 << " Subevent id: " << m_subeventid; 
      if (_trigger) os << " *Trigger*" ;
      os << endl;

    }
}

int daq_device_tpcfee::max_length(const int etype) const
{
  if (etype != m_eventType) return 0;
  return  (64*1024 + SEVTHEADERLENGTH + 4);
}


// the rearm() function
int  daq_device_tpcfee::rearm(const int etype)
{
  return 0;
}

int daq_device_tpcfee::endrun() 
{

  delete _fee;
  _fee = 0;
  
  if ( _trigger && _th)
    {
      _th->clear_fee();
    }

  return 0;
}
