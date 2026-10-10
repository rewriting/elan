# The ELAN compiler in Rust (spec S6): elanc-rs and the sources of its
# runtime crate, built by cargo (offline) into the build tree.
set(ELAN_RS_TARGET_DIR ${CMAKE_CURRENT_BINARY_DIR}/target)
add_custom_target(elanc_rs ALL
  COMMAND ${CMAKE_COMMAND} -E env CARGO_TARGET_DIR=${ELAN_RS_TARGET_DIR}
          ${ELAN_CARGO} build --release --offline --quiet
          --manifest-path ${CMAKE_CURRENT_SOURCE_DIR}/Cargo.toml -p elanc-rs
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Building elanc-rs (cargo)"
  USES_TERMINAL)
install(PROGRAMS ${ELAN_RS_TARGET_DIR}/release/elanc-rs DESTINATION bin)
# generated programs depend on the runtime crate by path
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/elan-runtime/
        DESTINATION share/elan-rs/elan-runtime)
