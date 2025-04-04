find_package(Doxygen)

set(DOXYGEN_INPUT_DIRS "")
list(APPEND DOXYGEN_INPUT_DIRS ${CMAKE_SOURCE_DIR}/src)

doxygen_add_docs(doc ${DOXYGEN_INPUT_DIRS} COMMENT "Generate man pages" ALL)
