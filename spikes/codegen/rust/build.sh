#!/bin/sh
# Clean release build (cargo, offline, std only) of bin/nqueens, bin/efib, bin/propc.
# Only this directory's ./target is cleaned.
set -e
cd "$(dirname "$0")"
CARGO=${CARGO:-$HOME/.cargo/bin/cargo}
"$CARGO" clean --offline --manifest-path ./Cargo.toml
start=$(date +%s)
/usr/bin/time -p "$CARGO" build --release --offline --manifest-path ./Cargo.toml
end=$(date +%s)
mkdir -p bin
cp target/release/nqueens target/release/efib target/release/propc bin/
echo "clean release build: $((end - start)) s (wall, see 'real' above)"
