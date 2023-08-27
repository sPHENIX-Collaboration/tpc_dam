AUTOMAKE_OPTIONS = foreign


AM_LDFLAGS = -L$(libdir)

INCLUDES = -I$(includedir) 


include_HEADERS = 

lib_LTLIBRARIES =  librcdaqplugin_dam.la

librcdaqplugin_dam_la_SOURCES = \
	daq_device_dam.cc \
	damTriggerHandler.cc \
	dam_plugin.cc


clean-local:
	rm -f $(BUILT_SOURCES)
