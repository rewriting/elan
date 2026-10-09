# --- macc: parser-table generator (C, host tool) --------------------------
add_executable(macc
  parser/globdef.c parser/rtglobde.c parser/mallo.c parser/tiderr.c parser/mlexan.c
  parser/dump.c parser/envir.c parser/gen.c parser/rwtab.c parser/ingram.c
  parser/init.c parser/macc.c parser/trapsa.c parser/mh.c)
target_compile_options(macc PRIVATE ${ELAN_C_COMPAT_OPTIONS} -g)
target_compile_definitions(macc PRIVATE ${ELAN_DEFINITIONS})

# --- libmatch: AC matcher (C) ----------------------------------------------
add_library(match STATIC
  acmatcher/matcher/build_free.c acmatcher/matcher/solve_free.c
  acmatcher/matcher/build_match.c acmatcher/matcher/flatten.c
  acmatcher/matcher/solve_pure.c acmatcher/matcher/tools.c
  acmatcher/matcher/build_pure.c acmatcher/matcher/gccmatch.c)
target_compile_options(match PRIVATE ${ELAN_C_COMPAT_OPTIONS} -g)
target_compile_definitions(match PRIVATE ${ELAN_DEFINITIONS})

# --- options shared by every C++ target of src/ ------------------------------
set(ELAN_INTERP_DEFINITIONS ${ELAN_DEFINITIONS}
    PREFIX="${CMAKE_INSTALL_PREFIX}")
set(ELAN_COMMON_SOURCES
  src/commondefs.cc src/ochstream.cc src/ichstream.cc src/lstream.cc
  src/stringtab.cc src/mitab.cc src/mallo.cc src/misc.cc)

function(elan_cxx_target target)
  target_compile_options(${target} PRIVATE ${ELAN_CXX_COMPAT_OPTIONS} -pipe -O2)
  target_compile_definitions(${target} PRIVATE ${ELAN_INTERP_DEFINITIONS})
  target_include_directories(${target} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/compat ${CMAKE_CURRENT_SOURCE_DIR}/src)
endfunction()

# --- mtokdef / mabident: keyword-table generators (host tools) ------------
add_executable(mtokdef src/mtokdefmain.cc src/specials.cc ${ELAN_COMMON_SOURCES})
elan_cxx_target(mtokdef)
add_executable(mabident src/mtokmain.cc src/specials.cc ${ELAN_COMMON_SOURCES})
elan_cxx_target(mabident)

# --- generated parser sources ----------------------------------------------
set(ELAN_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(ELAN_GENERATED
  ${ELAN_GEN_DIR}/ppexparser.cc ${ELAN_GEN_DIR}/mparser.cc ${ELAN_GEN_DIR}/atermparser.cc
  ${ELAN_GEN_DIR}/reduceparser.cc ${ELAN_GEN_DIR}/ldparser.cc ${ELAN_GEN_DIR}/tabofident.cc
  ${ELAN_GEN_DIR}/atabofident.cc ${ELAN_GEN_DIR}/rtabofident.cc ${ELAN_GEN_DIR}/commtokens.h)
set(S ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_custom_command(
  OUTPUT ${ELAN_GENERATED}
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/gen-parsers.sh
          $<TARGET_FILE:macc> $<TARGET_FILE:mtokdef> $<TARGET_FILE:mabident> ${S} ${ELAN_GEN_DIR}
  DEPENDS macc mtokdef mabident ${CMAKE_CURRENT_SOURCE_DIR}/gen-parsers.sh
          ${S}/ppexgram.t ${S}/modgram.t ${S}/ldmodgram.t ${S}/aterm.orig ${S}/aterm.ref
          ${S}/aterm.reduce ${S}/include.parser ${S}/toaterm ${S}/ppexparser.h ${S}/mparser.h
          ${S}/atermparser.h ${S}/reduceparser.h ${S}/ldparser.h
  COMMENT "Generating ELAN parser sources with macc")
add_custom_target(elan_parsers DEPENDS ${ELAN_GENERATED})

# --- elan: the interpreter ---------------------------------------------------
add_executable(elan
  src/ldmain.cc src/msemact.cc src/msemact3.cc src/termcompile.cc src/matchcompile.cc
  src/compilemisc.cc src/strategy.cc src/grammar.cc src/earley.cc src/esemact.cc src/term.cc
  src/module.cc src/tmisc.cc src/rtmisc.cc src/match.cc src/trsystem.cc src/normalise.cc
  src/aterm.cc src/partial.cc src/stateofexecution.cc src/meta.cc src/lbuffer.cc
  src/mlstream.cc src/command.cc
  ${ELAN_COMMON_SOURCES}
  ${ELAN_GEN_DIR}/tabofident.cc ${ELAN_GEN_DIR}/ppexparser.cc ${ELAN_GEN_DIR}/mparser.cc
  ${ELAN_GEN_DIR}/atermparser.cc ${ELAN_GEN_DIR}/reduceparser.cc ${ELAN_GEN_DIR}/ldparser.cc
  ${ELAN_GEN_DIR}/atabofident.cc)
elan_cxx_target(elan)
target_include_directories(elan PRIVATE ${ELAN_GEN_DIR})
add_dependencies(elan elan_parsers)
target_link_libraries(elan PRIVATE match m)

install(TARGETS elan RUNTIME DESTINATION bin)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/lib/elanlib/
        DESTINATION share/elanlib)
