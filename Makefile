# Façade over CMake, the reference build and the test bench.
#   make                 build the interpreter (build/)
#   make install         install into $(PREFIX)
#   make check           unit tests of the runner + interpreter tests (I, A) of the bench
#   make reference       build the 2004 reference system (reference/install)
#   make check-reference full bench (778 tests) against the reference
BUILD   ?= build
PREFIX  ?= $(CURDIR)/$(BUILD)/install
BENCH    = tests/legacy-bench/run_tests.py

# Compilers of the modern interpreter: the system Clang on macOS, GCC
# elsewhere.  They are not exported: reference/build.sh chooses its own
# defaults (Homebrew GCC on macOS) unless ELAN_CC/ELAN_CXX are set by the
# caller.
UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
  ELAN_CC  ?= cc
  ELAN_CXX ?= c++
else
  ELAN_CC  ?= gcc
  ELAN_CXX ?= g++
endif

.PHONY: all configure install check check-sanitize check-arch check-unit test-runner smoke reference check-reference clean toolchain

all: configure
	cmake --build $(BUILD) -j

toolchain:
	@command -v $(ELAN_CC)  >/dev/null || { echo "ERROR: C compiler '$(ELAN_CC)' not found (set ELAN_CC, see README)"; exit 1; }
	@command -v $(ELAN_CXX) >/dev/null || { echo "ERROR: C++ compiler '$(ELAN_CXX)' not found (set ELAN_CXX, see README)"; exit 1; }
	@command -v cmake >/dev/null || { echo "ERROR: cmake not found (see README)"; exit 1; }

# The configuration (compilers, install prefix compiled into elan, CMake
# options such as the sanitizers) is recorded in $(BUILD)/.elan-configure; when
# it changes, the build directory is recreated (CMake cannot switch compilers
# in place, and a stale sanitizer build would give misleading results).
ELAN_CONFIG = $(ELAN_CC)|$(ELAN_CXX)|$(PREFIX)|$(CMAKE_FLAGS)
configure: toolchain
	@case "$(BUILD)" in ""|.|..|/|"$(CURDIR)") echo "ERROR: refusing BUILD='$(BUILD)'"; exit 1;; esac
	@if [ "$$(cat $(BUILD)/.elan-configure 2>/dev/null)" != "$(ELAN_CONFIG)" ]; then \
	  rm -rf $(BUILD); \
	  cmake -S . -B $(BUILD) -DCMAKE_C_COMPILER=$(ELAN_CC) -DCMAKE_CXX_COMPILER=$(ELAN_CXX) \
	        -DCMAKE_INSTALL_PREFIX=$(PREFIX) $(CMAKE_FLAGS) && \
	  echo "$(ELAN_CONFIG)" > $(BUILD)/.elan-configure; \
	fi

# The library contains files differing only by case (strategy/any.eln, Any.eln).
install: all
	@mkdir -p $(PREFIX); p=$$(mktemp -d $(PREFIX)/.case.XXXX); touch $$p/a $$p/A; \
	n=$$(ls $$p | wc -l); rm -rf $$p; \
	if [ $$n -ne 2 ]; then echo "ERROR: $(PREFIX) is on a case-insensitive file system; the ELAN library has files differing only by case (see README)"; exit 1; fi
	cmake --install $(BUILD)

test-runner:
	cd tests/legacy-bench && python3 test_run_tests.py

# elan must find its library without ELANLIB (default = install prefix)
smoke: install
	@mkdir -p tests/legacy-bench/work; d=$$(mktemp -d tests/legacy-bench/work/smoke.XXXX); \
	cp legacy/elan/sources/Compiler.4.0/Test/enum.* $$d/; \
	echo 'enum(o,fac(s(s(o)))) end' | (cd $$d && env -u ELANLIB $(PREFIX)/bin/elan -b enum.lgi) > $$d/out 2>&1; \
	if grep -q 's(o)' $$d/out; then echo "smoke: elan finds its library without ELANLIB"; rm -rf $$d; \
	else cat $$d/out; rm -rf $$d; exit 1; fi

# Module dependency rules (tests/architecture, S3a spec D2)
check-arch:
	cd tests/architecture && python3 test_check_deps.py
	python3 tests/architecture/check_deps.py

# C++ unit tests of the interpreter modules (tests/unit, CTest)
check-unit: all
	cd $(BUILD) && ctest --output-on-failure --no-tests=error

check: smoke test-runner check-arch check-unit
	$(BENCH) --prefix $(PREFIX) --kinds I,A

# Interpreter tests under sanitizers (build-san/): AddressSanitizer + UBSan.
# On macOS 27 with Apple Clang 17, ASan hangs at startup even for an empty
# program, so the default there is UBSan only (ASan runs in CI on Linux and in
# ci/Dockerfile.linux). Leak detection is off: terms are never freed, by design.
# Stack-use-after-return detection is off: its fake stack makes the deeply
# recursive rewriting engine ~100 times slower (applications/Features m: 7 s
# instead of more than 11 min), so 29 tests would time out.
ifeq ($(UNAME),Darwin)
  SANITIZERS ?= undefined
else
  SANITIZERS ?= address,undefined
endif
# SAN_BUILD selects the build directory (e.g. one per compiler).
SAN_BUILD ?= build-san
check-sanitize:
	ASAN_OPTIONS=detect_leaks=0:detect_stack_use_after_return=0 UBSAN_OPTIONS=print_stacktrace=1 \
	$(MAKE) BUILD=$(SAN_BUILD) PREFIX=$(CURDIR)/$(SAN_BUILD)/install \
	  CMAKE_FLAGS="-DELAN_SANITIZE=ON -DELAN_SANITIZERS=$(SANITIZERS)" check

reference:
	reference/build.sh

check-reference:
	$(BENCH) --prefix reference/install

clean:
	rm -rf $(BUILD)
