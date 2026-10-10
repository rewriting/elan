#!/bin/sh
# Builds bin/nqueens, bin/efib, bin/propc (Go default build, std only, offline).
set -e
cd "$(dirname "$0")"
export GOTOOLCHAIN=local GOFLAGS=-mod=mod
mkdir -p bin
for p in nqueens efib propc; do
  go build -o bin/$p ./$p
done
