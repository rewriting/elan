#!/bin/sh
# Builds every variant into bin/ (Go default build, std only, offline).
#   g-*   generic nodes, weak unique table            (rtg)
#   gs-*  generic nodes, strong table (build tag strong)
#   gn-*  generic, weak, no small-int cache (tag nosmallint), as the 1st spike
#   h-*   hybrid: generic nodes + native builtins in generated code (rtg)
#   u-*   hybrid with unique.Handle nodes                (rtu)
#   t-*   typed: one struct per operator, interface per sort (rtt)
#   depth recursion-depth experiment
set -e
cd "$(dirname "$0")"
export GOTOOLCHAIN=local GOFLAGS=-mod=mod
mkdir -p bin
for p in nqueens efib propc; do
  go build -o bin/g-$p ./gen/$p
  go build -tags strong -o bin/gs-$p ./gen/$p
  go build -tags nosmallint -o bin/gn-$p ./gen/$p
  go build -o bin/t-$p ./typed/$p
done
go build -o bin/h-nqueens ./hyb/nqueens
go build -o bin/u-nqueens ./uniq/nqueens
go build -o bin/depth ./depth
