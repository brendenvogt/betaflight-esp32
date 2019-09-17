#
# Main component makefile.
#
# This Makefile can be left empty. By default, it will take the sources in the 
# src/ directory, compile them and link them into lib(subdirectory_name).a 
# in the build directory. This behaviour is entirely configurable,
# please read the ESP-IDF documents if you need to do this.

COMPONENT_SRCDIRS += ./esp32
COMPONENT_SRCDIRS += ./ArduinoJson

COMPONENT_ADD_INCLUDEDIRS += ./esp32
COMPONENT_ADD_INCLUDEDIRS += ./ArduinoJson

## Uncomment the following line to enable exception handling 
#CXXFLAGS+=-fexceptions
#CXXFLAGS+=-std=c++11