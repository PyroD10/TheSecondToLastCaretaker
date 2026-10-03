set(TARGET GhostNet)

add_library(${TARGET} SHARED
    dllmain.cpp
    LocalStateReader.cpp
)

target_include_directories(${TARGET} PRIVATE .)
target_link_libraries(${TARGET} PUBLIC UE4SS)
target_link_libraries(${TARGET} PRIVATE Ws2_32)   # UDP transport (Phase 3)
target_compile_features(${TARGET} PRIVATE cxx_std_23)

# UE4SS loads Mods/<Name>/dlls/main.dll
set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME "main")
