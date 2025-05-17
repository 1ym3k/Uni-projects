cmake_minimum_required(VERSION 3.10)

project(LatexPDFGenerator C CXX)

# Flagi kompilacji
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")

# Pliki źródłowe
add_executable(latex_pdf_generator main.c main_functions.c)

set(FILES_TO_COPY
    start.sh
    input.txt
    Readme.md
)

foreach(FILE ${FILES_TO_COPY})
    configure_file(${CMAKE_SOURCE_DIR}/${FILE} ${CMAKE_BINARY_DIR}/${FILE} COPYONLY)
endforeach()

# Ustawienie katalogu wyjściowego
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Włączenie testów
enable_testing()
add_subdirectory(tests)

