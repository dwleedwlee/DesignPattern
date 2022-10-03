add_library(vector ${SOURCE_FILES})

target_sources(vector
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/vector.c
  )
  