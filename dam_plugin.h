#ifndef __TSPM_PLUGIN_H__
#define __TSPM_PLUGIN_H__

#include <rcdaq_plugin.h>

#include <iostream>


class dam_plugin : public RCDAQPlugin {

 public:
  int  create_device(deviceblock *db);

  void identify(std::ostream& os = std::cout, const int flag=0) const;

};


#endif
