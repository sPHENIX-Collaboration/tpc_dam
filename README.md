# tpc_dam
The RCDAQ plugin for the TPC Felix card-based readout

DAM refers to the "Data Aggregation Module" that formats and aggregates the 
data from the TPC.

This is a plugin to the RCDAQ system. You will need RCDAQ installed in order
to build it.

This version is aware of multiple (well, the max of 2)
endpoints. Since RCDAQ can only have one trigger device, all endpoints
need to be dealt with with one device.

The first device refers to /dev/dam0, and the second to /dev/dam1. It
is hard-coded and cannot be overwritten.

The parameters are

rcdaq_client create_device device_dam <event type> <packetid0> <packetid2> <units> <trigger>
for example

rcdaq_client create_device device_dam 1 4001 4002 1 1
- evt type 1 = data event,
- packet ids 4001 (/dev/dam0) and 4002 (/dev/dam1),
- reading one "unit of data",
- and we are the trigger device

The "units of data" allows us to coarsely control the maximum amount
of words that we want to read in one packet. The unit here is 64KB, so
with the parameter=1 as shown above, each packet reads at most 64KB.

In high-volume data scenarios one can decrease the overhead slightly
by making the packets larger (= fewer packets/events and less
overhead) by specifying, for example, units=64 to get 4MB packets.
(Note that in dlist you are shown the packet size in 32-bit units, and
it is still made larger by the packet header).

The defaults are units=1 and trigger=1.

As it is the case for many sPHENIX plugins, at this point in time this
device MUST be the trigger device, that is, the full parameter
list is in place mainly for future use. It is recommended that you
only give the parameters that are actually not the default.

So instead of 

rcdaq_client create_device device_dam 1 4001 4002 1 1

as shown above, you should use

rcdaq_client create_device device_dam 1 4001 4002

Here are some use cases:

rcdaq_client create_device device_dam 1 4001 4002 64

(make 4MB packets)

rcdaq_client create_device device_dam 1 4001 0 64

(do not read the 2nd endpoint - expert level)

rcdaq_client create_device device_dam 1 0 4002 64

(do not read the first endpoint - expert level)

Note that setting both packet ids to 0 results in an error.

