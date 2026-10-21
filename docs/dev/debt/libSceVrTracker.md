# libSceVrTracker technical debt

### Unknown function info

- [VrTracker parameters](../../../core/libs/prx/libSceVrTracker/Export.cpp): argument slots are recovered. CPU processing and GPU waiting consume 32-byte records, result queries consume 56-byte records, and time queries write 64 bits. Initialization accepts 128-byte and 144-byte versions; the 144-byte layout includes three address/size/alignment records, but configuration selector semantics and the older field layout remain unresolved. GPU submission accepts 552-byte and 648-byte versions; submission and result records still require field recovery; result storage has 1520-byte and 3640-byte variants. CPU mode/argument semantics and the remaining query fields are unresolved. Protected bodies remain unchanged.
