AUTOMAKE_OPTIONS = foreign


AM_LDFLAGS = -L$(libdir)

INCLUDES = -I$(includedir) 


include_HEADERS = 

lib_LTLIBRARIES =  libdam_util.la librcdaqplugin_dam.la


libdam_util_la_SOURCES = \
	pl_lib.c

librcdaqplugin_dam_la_SOURCES = \
	daq_device_dam.cc \
	damTriggerHandler.cc \
	dam_plugin.cc


librcdaqplugin_dam_la_LIBADD = libdam_util.la


#bin_PROGRAMS = triggerSampa
#triggerSampa_SOURCES = triggerSampa.cc
#triggerSampa_LDADD = libdam_util.la


clean-local:
	rm -f $(BUILT_SOURCES)
