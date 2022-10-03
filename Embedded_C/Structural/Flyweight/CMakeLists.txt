set (Target flyweight)

add_executable(${Target} ${SOURCE_FILES})

target_sources(${Target}
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/${Target}.c
  )

target_include_directories(${Target}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../../_DataStructure/Dictionary
	${CMAKE_CURRENT_LIST_DIR}/../../_DataStructure/LinkedList
	${CMAKE_CURRENT_LIST_DIR}/../../_DataStructure/Vector
  )

target_link_libraries(${Target}
  PRIVATE
    dictionary
	linkedlist
    vector	
  )
