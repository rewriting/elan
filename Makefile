# Façade over CMake, the reference build and the test bench.
#   make                 build the interpreter (build/)
#   make install         install into $(PREFIX)
#   make check           unit tests of the runner + interpreter tests (I, A) of the bench
#   make reference       build the 2004 reference system (reference/install)
#   make check-reference full bench (778 tests) against the reference
BUILD   ?= build
PREFIX  ?= $(CURDIR)/$(BUILD)/install
BENCH    = tests/legacy-bench/run_tests.py

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
  ELAN_CC  ?= gcc-16
  ELAN_CXX ?= g++-16
else
  ELAN_CC  ?= gcc
  ELAN_CXX ?= g++
endif
export ELAN_CC ELAN_CXX

.PHONY: all configure install check test-runner reference check-reference clean toolchain

all: configure
	cmake --build $(BUILD) -j

toolchain:
	@command -v $(ELAN_CC)  >/dev/null || { echo "ERROR: C compiler '$(ELAN_CC)' not found (set ELAN_CC, see README)"; exit 1; }
	@command -v $(ELAN_CXX) >/dev/null || { echo "ERROR: C++ compiler '$(ELAN_CXX)' not found (set ELAN_CXX, see README)"; exit 1; }
	@command -v cmake >/dev/null || { echo "ERROR: cmake not found (see README)"; exit 1; }

configure: toolchain
	@test -f $(BUILD)/CMakeCache.txt || \
	  cmake -S . -B $(BUILD) -DCMAKE_C_COMPILER=$(ELAN_CC) -DCMAKE_CXX_COMPILER=$(ELAN_CXX) \
	        -DCMAKE_INSTALL_PREFIX=$(PREFIX)

install: all
	cmake --install $(BUILD) --prefix $(PREFIX)

test-runner:
	cd tests/legacy-bench && python3 test_run_tests.py

check: install test-runner
	$(BENCH) --prefix $(PREFIX) --kinds I,A

reference:
	reference/build.sh

check-reference:
	$(BENCH) --prefix reference/install

clean:
	rm -rf $(BUILD)
