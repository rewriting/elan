#!/bin/sh
# Clean release build (cargo, offline, std only) of bin/nqueens, bin/propc
# (and bin/droptest). Only this directory's ./target is cleaned.
set -e
cd "$(dirname "$0")"
CARGO=${CARGO:-$HOME/.cargo/bin/cargo}
"$CARGO" clean --offline --manifest-path ./Cargo.toml
/usr/bin/time -p "$CARGO" build --release --offline --manifest-path ./Cargo.toml
mkdir -p bin
cp target/release/nqueens target/release/propc target/release/droptest bin/
