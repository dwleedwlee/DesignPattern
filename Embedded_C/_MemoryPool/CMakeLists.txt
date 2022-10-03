add_library(mempool ${SOURCE_FILES})

target_sources(mempool
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/mempool.c
  )
  