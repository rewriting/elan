# Façade over CMake, the reference build and the test bench.
#   make                 build the interpreter (build/)
#   make install         install into $(PREFIX)
#   make check           unit tests of the runner + golden tests + interpreter tests (I, A)
#                        of the bench + compiled tests (J, JO)
#   make check-golden    golden tests: .ref files, -d dump, statistics, errors
#   make check-compiler  compiled tests (J, JO) only: elanc, REM, runtime libraries
#   make reference       build the 2004 reference system (reference/install)
#   make check-reference full bench (778 tests) against the reference
BUILD   ?= build
PREFIX  ?= $(CURDIR)/$(BUILD)/install
BENCH    = tests/legacy-bench/run_tests.py
# parallel build jobs (a plain -j is unlimited with make and can exhaust memory)
JOBS    ?= $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)

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

.PHONY: manual manual-update all configure install check check-compiler check-golden check-sanitize check-arch check-unit test-runner smoke reference check-reference clean toolchain

all: configure
	cmake --build $(BUILD) -j $(JOBS)

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
	@case "$(PREFIX)" in ""|/|*" "*) echo "ERROR: refusing PREFIX='$(PREFIX)' (empty, / or containing spaces)"; exit 1;; esac
	@if [ "$$(cat $(BUILD)/.elan-configure 2>/dev/null)" != "$(ELAN_CONFIG)" ]; then \
	  rm -rf $(BUILD); \
	  cmake -S . -B $(BUILD) -DCMAKE_C_COMPILER=$(ELAN_CC) -DCMAKE_CXX_COMPILER=$(ELAN_CXX) \
	        -DCMAKE_INSTALL_PREFIX=$(PREFIX) $(CMAKE_FLAGS) && \
	  echo "$(ELAN_CONFIG)" > $(BUILD)/.elan-configure; \
	fi

# Files removed from the sources must not linger in PREFIX: the library, the
# compiler's classes, its runtime headers and elanc are reinstalled from
# scratch (the last three only when the compiler is built, -DELAN_COMPILER=ON,
# the default).
install: all
	rm -rf "$(PREFIX)/share/elanlib" "$(PREFIX)/classes" "$(PREFIX)/include/elan-compiler" "$(PREFIX)/bin/elanc"
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

# Module dependency rules (tests/architecture, S3a spec D2), and the
# compiler's copies of the interpreter's limits and codes (REF contract)
check-arch:
	cd tests/architecture && python3 test_check_deps.py && python3 test_check_limits.py
	python3 tests/architecture/check_deps.py
	python3 tests/architecture/check_limits.py

# C++ unit tests of the interpreter modules (tests/unit, CTest)
check-unit: all
	cd $(BUILD) && ctest --output-on-failure --no-tests=error

# Compiled tests of the bench (spec S5a, D4): elanc -> make -> a.out, against
# the installed compiler (the generated programs are built with the
# compilers of this build, see src/compiler/CMakeLists.txt), after the tests
# of elanc, of the strategies evaluated at run time and of the BenchThesis
# programs ported to ELAN 3 (tests/compiler).
# The compiled tests run only when the compiler is installed (not with
# -DELAN_COMPILER=OFF).
CHECK_COMPILER = if [ -x "$(PREFIX)/bin/elanc" ]; then \
	  tests/compiler/test_elanc.sh "$(PREFIX)" && tests/compiler/test_runtime_strategies.sh "$(PREFIX)" \
	  && tests/compiler/test_start_term.sh "$(PREFIX)" \
	  && tests/compiler/test_benchthesis.sh "$(PREFIX)" \
	  && $(BENCH) --prefix $(PREFIX) --kinds J,JO; \
	else echo "compiler not built (-DELAN_COMPILER=OFF): compiled tests (J, JO) skipped"; fi
check-compiler: install
	$(CHECK_COMPILER)

# Golden tests (tests/golden): what the bench does not compare, byte for
# byte (.ref files, the -d dump, the statistics, stderr and exit status of
# errors), against the installed interpreter.
check-golden: install
	cd tests/golden && python3 test_run_golden.py
	python3 tests/golden/run_golden.py --prefix $(PREFIX)

check: smoke test-runner check-arch check-unit check-golden
	$(BENCH) --prefix $(PREFIX) --kinds I,A
	@$(CHECK_COMPILER)

# Interpreter tests under sanitizers (build-san/): AddressSanitizer + UBSan.
# The compiled tests (J, JO) run too: `elan --cexport` runs under the
# sanitizers, the compiler libraries and the generated programs are built with
# UBSan only (libchoice copies the C stack, which ASan cannot check; elanc
# passes the options to the generated Makefiles).
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

# The ELAN 3.6 user manual (docs/manual, LaTeX from 2003): latex x3 + dvipdf
# (figures are MetaPost/EPS). Built in build-manual/manual.pdf; `make
# manual-update` copies it over the committed docs/manual/manual.pdf (the PDF
# bytes differ between builds: Ghostscript stamps the build time). The
# bibliography and index (manual.bbl, manual.ind) are the 2003 ones: bibtex
# and makeindex are not rerun, so edit them directly if needed.
manual:
	rm -rf build-manual && cp -R docs/manual build-manual
	cd build-manual && for i in 1 2 3; do latex -interaction=nonstopmode manual.tex > latex.log 2>&1 || true; done
	@if grep -q '^! ' build-manual/latex.log; then grep -A3 '^! ' build-manual/latex.log; echo "ERROR: LaTeX errors (build-manual/latex.log)"; exit 1; fi
	cd build-manual && dvipdf manual.dvi manual.pdf
	@grep -o 'Output written on [^(]*([0-9]* pages' build-manual/latex.log

manual-update: manual
	cp build-manual/manual.pdf docs/manual/manual.pdf

reference:
	reference/build.sh

check-reference:
	$(BENCH) --prefix reference/install

clean:
	rm -rf $(BUILD)
