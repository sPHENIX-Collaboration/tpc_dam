#include "dam_plugin.h"

#include "parseargument.h"

#include <daq_device_dam.h>


#include <strings.h>

using namespace std;

//parameters -
// evt type
// packetid 0
// packetid 1
// nunits  - control the size of the packet
// trigger

// eg 1 4001 4002 64 1  == read out 2 endpoints
// eg 1 4001    0 64 1  == read out 1 endpoint



int dam_plugin::create_device(deviceblock *db)
{

  //  std::cout << __LINE__ << "  " << __FILE__ << "  " << db->argv0 << "  " << db->npar << std::endl;

  int eventtype;
  int subid;

  if ( strcasecmp(db->argv0,"device_dam") == 0 ) 
    {
      //std::cout << __LINE__ << "  " << __FILE__ << "  " << db->argv0 << "  " << db->npar << std::endl;
      // we need at least 2 params
      if ( db->npar <3 ) return 1; // indicate wrong params
      
      eventtype  = get_value ( db->argv1); // event type
      subid = get_value ( db->argv2); // subevent id

      if ( db->npar == 3)
	{
	  add_readoutdevice ( new daq_device_dam( eventtype,
						  subid ));
	  return 0;  // say "we handled this request" 
	}

      else if ( db->npar == 4)
	{

	  int subid1 = get_value ( db->argv3);
	  cout << " subids " << subid << " " << subid1 << endl;
	  add_readoutdevice ( new daq_device_dam( eventtype,
						  subid,
						  subid1) );
	  return 0;  // say "we handled this request" 
	}

      else if ( db->npar == 5)
	{
	  int subid1 = get_value ( db->argv3);
	  int nunits = get_value ( db->argv4);

	  add_readoutdevice ( new daq_device_dam( eventtype,
						  subid,
						  subid1,
						  nunits ));  
	  return 0;  // say "we handled this request" 
	}

      else if ( db->npar == 6)
	{
	  int subid1 = get_value ( db->argv3);
	  int nunits = get_value ( db->argv4);
	  int trigger = get_value ( db->argv5);

	  add_readoutdevice ( new daq_device_dam( eventtype,
						  subid,
						  subid1,
						  nunits,
						  trigger ) );
	  return 0;  // say "we handled this request" 
	}

      else
	{
	  return 1; // say it is our device but the parameters are wrong 
	}
    } 


  return -1; // say " this is not our device"

}

void dam_plugin::identify(std::ostream& os, const int flag) const
{

  if ( flag <=2 )
    {
      os << " - DAM Plugin" << std::endl;
    }
  else
    {
      os << " - DAM Plugin, provides - " << std::endl;
      os << " -     device_dam (evttype, subid [, npackets, trigger] ) - DAM FELIX Board " << std::endl;
    }
      

}


dam_plugin _damp;
