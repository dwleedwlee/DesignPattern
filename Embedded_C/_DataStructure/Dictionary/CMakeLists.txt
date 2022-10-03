add_library(dictionary ${SOURCE_FILES})

target_sources(dictionary
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/dictionary.c
  )
  