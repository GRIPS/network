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

Example output
--------------
If you run `fake_telemetry` in one shell and `listen_telemetry` in another shell, you will see output that looks like below.

`fake_telemetry` shell:
```
90eb6c1f00100c0001001e08e7785634000000000000f03f0000803f
Packet size: 28
90eb3c380010080002005e53337956340000000000000040
Packet size: 24
90eb08a100100c0003009e9e7f795634000000000000084000004040
Packet size: 28
90eb37ff001008000400dee9cb7956340000000000001040
Packet size: 24
```

`listen_telemetry` shell:
```
Packet received, 28 bytes
  Raw: 90eb6c1f00100c0001001e08e7785634000000000000f03f0000803f
  SystemID: 0x00
  TmType: 0x10
  SystemTime: 0x345678e7081e
Packet received, 24 bytes
  Raw: 90eb3c380010080002005e53337956340000000000000040
  SystemID: 0x00
  TmType: 0x10
  SystemTime: 0x34567933535e
Packet received, 28 bytes
  Raw: 90eb08a100100c0003009e9e7f795634000000000000084000004040
  SystemID: 0x00
  TmType: 0x10
  SystemTime: 0x3456797f9e9e
Packet received, 24 bytes
  Raw: 90eb37ff001008000400dee9cb7956340000000000001040
  SystemID: 0x00
  TmType: 0x10
  SystemTime: 0x345679cbe9de
```
