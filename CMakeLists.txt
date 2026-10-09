cmake_minimum_required(VERSION 3.20)
project(elan VERSION 3.6 LANGUAGES C CXX)

# S1: the 1998-2004 sources are built unchanged, in pre-standard modes.
# Removing these options is sub-project S2.
set(ELAN_C_COMPAT_OPTIONS   -std=gnu89 -w -fcommon)
set(ELAN_CXX_COMPAT_OPTIONS -std=gnu++98 -fpermissive -w)
set(ELAN_DEFINITIONS
    PACKAGE="elan-interpreter" VERSION="3.6g" YYTEXT_POINTER=1)

add_subdirectory(src/interpreter)
