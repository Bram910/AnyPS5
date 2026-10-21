# libSceSharePlay technical debt

### Functional

- [sceSharePlayInitialize and sceSharePlayTerminate](../../../core/libs/prx/libSceSharePlay/Export.cpp) - native heap validation and initialization state are recovered: a null heap ignores the supplied size, a supplied heap requires at least 0x1800 bytes, repeated initialization returns 0x810E0003 and termination before initialization returns 0x810E0004. The host does not allocate the native internal heap, connect to the Share Play service or reproduce its failures.
- [sceSharePlayGetCurrentConnectionInfoA](../../../core/libs/prx/libSceSharePlay/Export.cpp) - the native module confirms a 72-byte result, initial zero fields and both user IDs -1. Initialization is checked before the output pointer. The host retains this initial disconnected state; native event draining, cached updates and service reconnection are unimplemented.
- [sceSharePlayProhibitController](../../../core/libs/prx/libSceSharePlay/Export.cpp) - the argument is one byte, forwarded unchanged to the service. The host checks initialization and accepts 0/1 without a visitor controller; other byte values still throw because their handling belongs to the unimplemented service.
