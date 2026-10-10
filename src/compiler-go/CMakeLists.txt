# The ELAN compiler in Go (spec S7): elanc-go and the sources of its runtime
# module, built by the go tool (offline, standard library only) into the
# build tree.
set(ELAN_GO_BIN ${CMAKE_CURRENT_BINARY_DIR}/elanc-go)
add_custom_target(elanc_go ALL
  COMMAND ${CMAKE_COMMAND} -E env GOTOOLCHAIN=local GOFLAGS=-mod=mod
          GOCACHE=${CMAKE_CURRENT_BINARY_DIR}/gocache
          ${ELAN_GO_EXE} build -buildvcs=false -o ${ELAN_GO_BIN} ./cmd/elanc-go
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Building elanc-go (go)"
  USES_TERMINAL)
install(PROGRAMS ${ELAN_GO_BIN} DESTINATION bin)
# generated programs import the runtime module by a replace directive
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/elanrt/
        DESTINATION share/elan-go/elanrt
        PATTERN "*_test.go" EXCLUDE)
