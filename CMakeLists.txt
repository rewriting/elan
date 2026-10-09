cmake_minimum_required(VERSION 3.20)
project(elan VERSION 3.6 LANGUAGES C CXX)

# S1: the 1998-2004 sources are built unchanged, in pre-standard modes.
# Removing these options is sub-project S2.
# -fsigned-char: the code stores -1 in `char` (e.g. module visibilities); char
# is unsigned on Linux/aarch64, where local rules then become visible.
set(ELAN_C_COMPAT_OPTIONS   -std=gnu89 -w -fcommon -fsigned-char)
set(ELAN_CXX_COMPAT_OPTIONS -std=gnu++98 -fpermissive -w -fsigned-char)
set(ELAN_DEFINITIONS
    PACKAGE="elan-interpreter" VERSION="3.6g" YYTEXT_POINTER=1)

add_subdirectory(src/interpreter)
