# newline_to_null.py
# by Eliot Baez
#
# This script takes the sorted newline-separated wordlist as an input file, and
# converts it into a null-separated word table in a single binary file. In
# addition, it generates a special header file for internal use that defines
# important sizes used in generating the source files for libcobalt.
#
# Two files are generated by this script:
#   ../src/globals/sizes.h
#       This file will contain #define directives with all the important lengths
#       necessary for generating the machine-specific source files. 
#   ./wordtable.bin
#       Stores the contents of the INPUT_FILE, with all newlines replaced with
#       null characters.

# TODO:
# make the in-header value of NUMBER_OF_WORDS depend on the definition of the
# NUMBER_OF_WORDS variable in this file 

NUMBER_OF_WORDS = 50000
INPUT_FILE = "50k-newline-separated-sorted.txt"

if __name__ == "__main__":
    length = 0
    # First write the null-separated and null-terminated file that will be used
    # to generate the wordtable.c source file
    with open(INPUT_FILE, "rt") as i, open("wordtable.bin", "wb") as o:
        for count in range(NUMBER_OF_WORDS):
            line = bytes(i.readline().replace('\n', '\0'), encoding="utf-8")
            length += len(line)
            o.write(line)

    # Then, based on the size of the wordtable.bin file and the number of words
    # defined in this script, define the relevant information in sizes.c. The
    # only information that goes in sizes.c is information that will not be
    # generated by the c_hexdump program.
    with open("../src/globals/sizes.c", "wt") as sizes:
        sizes.write(
            "#include <stdint.h>\n"
            "#include <stddef.h>\n"
            "#include \"cobalt.h\"\n\n"
            "const uint16_t NUMBER_OF_WORDS = 50000;\n"
            "const size_t WORDTABLE_STRLEN = %d;"
            % (length - 1))
