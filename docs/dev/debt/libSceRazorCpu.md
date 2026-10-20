# libSceRazorCpu technical debt

### Functional

- [libSceRazorCpu](../../../core/libs/prx/libSceRazorCpu/Export.cpp): capture-controller registration, thread-local capture storage, marker serialization and flushing are not implemented. The host implements only the confirmed uninitialized-profiler path: markers return zero without reading labels, and `sceRazorCpuFlushOccurred` returns false without writing its optional output.

### Unknown function info

- [sceRazorCpuJobManagerDispatch, sceRazorCpuJobManagerJob and sceRazorCpuJobManagerSequence](../../../core/libs/prx/libSceRazorCpu/Export.cpp): argument positions and serialized widths are recovered from machine instructions. Dispatch takes seven arguments, including two 64-bit payloads whose upper 16 bits are replaced by the count fields. The payloads are not dereferenced, so their original pointer-versus-integer types and the semantic names of the event identifiers remain unresolved. Job and Sequence serialize 16-bit identifiers and an event selector; their source enum names are unavailable.
- [sceRazorCpuGetDataTagStorageSize](../../../core/libs/prx/libSceRazorCpu/Export.cpp) (libSceRazorCpu) - throws: [SharpProspero](https://github.com/SvenGDK/SharpProspero/blob/5dfdb4b0b166e83dc9ae275001afb695f12c7174/src/SharpProspero/Interop/Debug/RazorCpu.cs#L142-L144) documents it as the byte size a tag table needs for a count of entries, but not the size of an entry
