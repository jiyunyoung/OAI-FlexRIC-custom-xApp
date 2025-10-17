cmake_minimum_required(VERSION 3.15)
project(array_sm)

# === Source files for your Service Model ===
set(ARRAY_SM_SRC
    array_sm_agent.c
    array_sm_ric.c
    array_sm_msg.c
)

# === Shared library (.so) ===
add_library(array_sm SHARED ${ARRAY_SM_SRC})
target_compile_options(array_sm PRIVATE -fPIC -fvisibility=hidden)

# === Static library (.a) ===
add_library(array_sm_static STATIC ${ARRAY_SM_SRC})

# === Include directories ===
target_include_directories(array_sm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(array_sm_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# === Compiler definitions (match global build) ===
target_compile_definitions(array_sm PUBLIC ${SM_ENCODING_SLICE} ${E2AP_VERSION}   ${KPM_VERSION}  )
target_compile_definitions(array_sm_static PUBLIC ${SM_ENCODING_SLICE} ${E2AP_VERSION}  ${KPM_VERSION}  )

# Install to FlexRIC plugin directory
install(TARGETS array_sm DESTINATION lib/flexric)

