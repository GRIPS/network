GRIPS Network Interface
=======================

This C++ code implements and tests the specification of the GRIPS network interface:

https://docs.google.com/document/d/1DubfOC0UKNMLMgQmaWAg3FIBkPa_e8ORo-74ypTPjdE/edit?usp=sharing

To use these classes code in your actual software, you need to be able to incorporate C++ code (as opposed to just C code, for example).

How to build
------------

* `make` builds `listen_telemetry` and `listen_command`
* `make all` also builds `fake_telemetry` and `fake_command`

Listening
---------
You can use the `listen_*` programs to test whether you are sending out valid packets.

* `listen_telemetry` will listen on port 60501 for telemetry packets
* `listen_command` will listen on port 50501 for command packets

For each packet received, the program will check if the packet has a valid checksum, and if it does, print out some basic information.

Faked packets
-------------
You can use the `fake_*` programs to test whether you are able to validate packets and parse the headers.
Since the specifications of the contents of packets has not been decided, for now these are basically for testing the header.

* `fake_telemetry` will send fake telemetry packets to port 60501 on localhost
* `fake_command` will send fake command packets to port 50501 on localhost
