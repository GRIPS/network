# Pattern matching in make files
#   $^ matches all the input dependencies
#   $< matches the first input dependency
#   $@ matches the output

CC=g++
GCC_VERSION_GE_43 := $(shell expr `g++ -dumpversion | cut -f2 -d.` \>= 3)

INCLUDE =

CFLAGS = -Wall $(INCLUDE) -Wno-unknown-pragmas
ifeq "$(GCC_VERSION_GE_43)" "1"
    CFLAGS += -std=gnu++0x
endif

THREAD = -lpthread

EXEC_CORE = listen_telemetry listen_command
EXEC_ALL = $(EXEC_CORE) fake_telemetry fake_command
PACKET = Packet.o lib_crc.o

default: $(EXEC_CORE)

all: $(EXEC_ALL)

listen_telemetry: listen_telemetry.cpp Telemetry.o $(PACKET) UDPReceiver.o types.o
	$(CC) $(CFLAGS) $^ -o $@ $(THREAD)

listen_command: listen_command.cpp Command.o $(PACKET) UDPReceiver.o types.o
	$(CC) $(CFLAGS) $^ -o $@ $(THREAD)

fake_telemetry: fake_telemetry.cpp Telemetry.o $(PACKET) UDPSender.o types.o
	$(CC) $(CFLAGS) $^ -o $@ $(THREAD)

fake_command: fake_command.cpp Command.o $(PACKET) UDPSender.o types.o
	$(CC) $(CFLAGS) $^ -o $@ $(THREAD)

#This pattern matching will catch all "simple" object dependencies
%.o: %.cpp %.hpp
	$(CC) -c $(CFLAGS) $< -o $@

lib_crc.o: lib_crc/lib_crc.c lib_crc/lib_crc.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *.out $(EXEC_ALL)
