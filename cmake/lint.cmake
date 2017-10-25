set(LINT_COMMAND ${CMAKE_SOURCE_DIR}/scripts/cpplint.py)
set(SRC_FILE_EXTENSIONS h hpp c cpp cc)
set(LINT_DIRS include/base include/client include/configure include/data include/filesystem
  src/base src/client src/configure src/data src/filesystem test)

cmake_policy(SET CMP0009 NEW)  # suppress cmake warning

# find all files of interest
foreach(ext ${SRC_FILE_EXTENSIONS})
    foreach(dir ${LINT_DIRS})
        file(GLOB_RECURSE FOUND_FILES ${CMAKE_SOURCE_DIR}/${dir}/*.${ext})
        set(LINT_SOURCES ${LINT_SOURCES} ${FOUND_FILES})
    endforeach()
endforeach()

# find all files that should be excluded
set(EXCLUDE_FILE_NAMES HelpText.cpp QSError.cpp)
foreach(file ${EXCLUDE_FILE_NAMES})
    file(GLOB_RECURSE FOUND_FILES ${CMAKE_SOURCE_DIR}/${file})
    set(EXCLUDED_FILES ${EXCLUDED_FILES} ${FOUND_FILES})
endforeach()

# exclude generated pb files
list(REMOVE_ITEM LINT_SOURCES ${EXCLUDED_FILES})

execute_process(
    COMMAND ${LINT_COMMAND} ${LINT_SOURCES}
    ERROR_VARIABLE LINT_OUTPUT
    ERROR_STRIP_TRAILING_WHITESPACE
)

#message(STATUS "cpplint output: ${LINT_OUTPUT}")

string(REPLACE "\n" ";" LINT_OUTPUT ${LINT_OUTPUT})

list(GET LINT_OUTPUT -1 LINT_RESULT)
list(REMOVE_AT LINT_OUTPUT -1)
string(REPLACE " " ";" LINT_RESULT ${LINT_RESULT})
list(GET LINT_RESULT -1 NUM_ERRORS)
if(NUM_ERRORS GREATER 0)
    foreach(msg ${LINT_OUTPUT})
        string(FIND ${msg} "Done" result)
        if(result LESS 0)
            message(STATUS ${msg})
        endif()
    endforeach()
    message(FATAL_ERROR "Lint found ${NUM_ERRORS} errors!")
else()
    message(STATUS "Lint did not find any errors!")
endif()