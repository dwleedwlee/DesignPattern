add_library(linkedlist ${SOURCE_FILES})

target_sources(linkedlist
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/linkedlist.c
  )
  