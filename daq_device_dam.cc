
#include <iostream>

#include <daq_device_dam.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <fcntl.h>

#define DEVNAME0 "/dev/dam0"
#define DEVNAME1 "/dev/dam1"

// this is nice for testing - we can read off named pipes where we control what gets there  
//#define DEVNAME0 "/home/purschke/damtest/f_dam0"
//#define DEVNAME1 "/home/purschke/damtest/f_dam1"

#define DATA_LENGTH          256*256 // 256KB


using namespace std;

int readn (int fd, char *ptr, const int nbytes);


daq_device_dam::daq_device_dam(const int eventtype
			       , const int subeventid0
			       , const int subeventid1
			       , const int nunits
			       , const int trigger)
{

  m_eventType  = eventtype;
  m_subeventid = 0;
  _subeventid0 = subeventid0;
  _subeventid1 = subeventid1;
  _nunits = nunits;
  if ( _nunits <= 0 ) _nunits = 1;  // make sure we read at least one unit
  
  _trigger = trigger;
    
  _dam_fd0 = -1;  // want them to be a non-valid file descriptor initially
  _dam_fd1 = -1;

  _broken = 0;

  _npackets = 0;   // we need this to calculate the max data size we can get
  if ( subeventid0 ) _npackets++;
  if ( subeventid1 ) _npackets++;

  if ( ! _npackets)  // if we don't read anything, what's the point...
    {
      _broken = -3;
    }
  
  _desired_data_length =  _nunits * DATA_LENGTH; // this is per packet
  
  if ( _desired_data_length > 1.5 * 1024 * 1024 * 1024)
    {
      _broken = -4;
    }

  
  if ( _trigger )
    {
      //  cout << __LINE__ << "  " << __FILE__ << " registering triggerhandler " << endl;
      _th  = new damTriggerHandler (m_eventType);
      
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
  if ( _dam_fd0 > 0) close (_dam_fd0);
  if ( _dam_fd1 > 0) close (_dam_fd1);
  
}


int  daq_device_dam::init()
{
  
  if ( _broken ) 
    {
      //      cout << __LINE__ << "  " << __FILE__ << " broken ";
      //      identify();
      return 0; //  we had a catastrophic failure
    }
  
  if ( _subeventid0) // we have the first dam to read
    {
      if ( (_dam_fd0 = open(DEVNAME0, O_RDWR)) <= 0 )
	{
	  perror(__func__);
	  _broken = 2;
	  return 1;
	}
    }
  
  if ( _subeventid1) // we have a second dam to read
    {
      if ( (_dam_fd1 = open(DEVNAME1, O_RDWR)) <= 0 )
	{
	  close(_dam_fd0);  // we already got fd0
	  perror(__func__);
	  _broken = 2;
	  return 1;
	}
    }
  
  if ( _trigger )
    {
      _th->set_damfd( _dam_fd0, _dam_fd1);
    }
  
  return 0;
}
  
// the put_data function
  
int daq_device_dam::put_data(const int etype, int * adr, const int length )
{

  if ( _broken ) 
    {
      cout << __LINE__ << "  " << __FILE__ << " broken ";
      //      identify();
      return 0; //  we had a catastrophic failure
    }


  int len = 0;

  if (etype != m_eventType )  // not our id
    {
      return 0;
    }

  int l = 0;

  if ( _th)  // we cannot do without being a trigger device for now
    {
      fd_set readflags = _th->get_readflags();

      if (_subeventid0 && FD_ISSET(_dam_fd0, &readflags))
	{
	  //cout << __LINE__ << "  " << __FILE__ << " data on dam0 "  << endl;
	  l = add_packet ( _dam_fd0, adr, length, _subeventid0);
	}
  
      // see if we have a 2nd endpoint, and data on it
      adr += l;
      if ( _subeventid1 && FD_ISSET(_dam_fd1, &readflags) )
	{
	  // cout << __LINE__ << "  " << __FILE__ << " data on dam1 "  << endl;
	  l+= add_packet (_dam_fd1, adr, length -l, _subeventid1);
	}
    }
  return l;
  
}
 
 int daq_device_dam::add_packet(int fd, int * adr, const int length, const int ipacket )
 {
  subevtdata_ptr sevt =  (subevtdata_ptr) adr;
  // set the initial subevent length
  sevt->sub_length =  SEVTHEADERLENGTH;

  // update id's etc
  sevt->sub_id = ipacket;
  sevt->sub_type=2;
  sevt->sub_decoding = 120; // IDTPCFEEV3 
  sevt->reserved[0] = 0;
  sevt->reserved[1] = 0;
  
  uint16_t *dest = (uint16_t *) &sevt->data;
  
  int ret;
  
  ret = read(fd, dest, _desired_data_length);
  
  //cout << __LINE__ << "  " << __FILE__ << " wanted " << _desired_data_length << " and read  "  << ret << " words " << endl;
  
  //      sevt->sub_padding = ret%2 ;
  sevt->sub_padding = 0;  // we can never have an odd number of uint16s
  
  sevt->sub_length += (ret + sevt->sub_padding);
  // cout << __LINE__ << "  " << __FILE__ << " returning "  << sevt->sub_length << endl;
  return sevt->sub_length;

}

  
void daq_device_dam::identify(std::ostream& os) const
{
  if ( _broken) 
    {
      os << "FELIX DAM Card  Event Type: " << m_eventType 
	 << " Subevent ids: " << _subeventid0 << " " << _subeventid1 
	 << " units " << _nunits 
	 << " ** not functional ** " << _broken << endl;
    }
  else
    {
      os << "FELIX DAM Card  Event Type: " << m_eventType 
	 << " Subevent id: " << _subeventid0 << " " << _subeventid1 
	 << " units " << _nunits ;
      if (_trigger) os << " *Trigger*" ;
      os << endl;

    }
}

int daq_device_dam::max_length(const int etype) const
{
  if (etype != m_eventType) return 0;
  return  ( _npackets*_desired_data_length + _npackets *SEVTHEADERLENGTH) +16; // I think we got the desired length right, +16 is just for safety

}


// the rearm() function
int  daq_device_dam::rearm(const int etype)
{
  return 0;
}

int daq_device_dam::endrun() 
{
  
  if (_dam_fd0 > 0)  close (_dam_fd0);
  if (_dam_fd1 > 0)  close (_dam_fd1);

  _dam_fd0 = -1;
  _dam_fd1 = -1;
  
  return 0;
}


