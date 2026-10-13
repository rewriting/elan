# Façade over CMake, the reference build and the test bench.
#   make                 build the interpreter (build/)
#   make install         install into $(PREFIX)
#   make check           unit tests of the runner + golden tests + interpreter tests (I, A)
#                        of the bench + compiled tests (J, JO) + check-rust + check-go
#   make check-rust      tests of the Rust compiler elanc-rs, only in a build with
#                        -DELAN_RUST=ON (OFF by default since S7e), e.g.
#                        make BUILD=build-rust CMAKE_FLAGS=-DELAN_RUST=ON check
#   make check-go        tests of the Go compiler elanc-go and its runtime (when go >= 1.24
#                        exists): go vet/test, .ref reader, normalisation, strategies, AC,
#                        depth, bench G, query input, BenchThesis G
#   make check-front     tests of the Go front end (src/compiler-go/front, spec S8,
#                        when go >= 1.24 exists): go vet/test, the harness
#                        elan-lextrace, lexem streams, module trees, error corpus
#   make check-case      no two tracked paths differ only by case (CASE-RENAMES.tsv)
#   make check-golden    golden tests: .ref files, -d dump, statistics, errors
#   make check-compiler  compiled tests (J, JO) only: elanc-rem, REM, runtime libraries
#   make check-elanc     what bin/elanc is (ELAN_ELANC: go by default, rem), CMake's fallback
#   make check-elanc-rem the same test in a build with -DELAN_ELANC=rem (build-elanc-rem/,
#                        removed afterwards)
#   make reference       build the 2004 reference system (REFERENCE_PREFIX, default
#                        reference/install; on a case-sensitive volume, see reference/README.md)
#   make check-reference full bench (778 tests) against the reference
BUILD   ?= build
PREFIX  ?= $(CURDIR)/$(BUILD)/install
BENCH    = tests/legacy-bench/run_tests.py
REFERENCE_PREFIX ?= $(CURDIR)/reference/install
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

.PHONY: check-case front-tables check-front manual manual-update all configure install check check-rust check-go check-compiler check-elanc check-elanc-rem elanc-kind check-golden check-sanitize check-arch check-unit test-runner smoke reference check-reference clean toolchain

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
# ELAN_CONFIG_EPOCH changes when a default of CMakeLists.txt changes, so that
# existing build directories (whose CMake cache keeps the old default) are
# recreated: 2 = ELAN_RUST OFF by default (S7e).
ELAN_CONFIG_EPOCH = 2
ELAN_CONFIG = $(ELAN_CONFIG_EPOCH)|$(ELAN_CC)|$(ELAN_CXX)|$(PREFIX)|$(CMAKE_FLAGS)
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
# compiler's classes, its runtime headers, elanc-rem, elanc-go and the
# symlink elanc (to elanc-go or elanc-rem, ELAN_ELANC) are reinstalled from
# scratch.
install: all
	rm -rf "$(PREFIX)/share/elanlib" "$(PREFIX)/classes" "$(PREFIX)/include/elan-compiler" "$(PREFIX)/bin/elanc-rem" \
	       "$(PREFIX)/bin/elanc-rs" "$(PREFIX)/share/elan-rs" "$(PREFIX)/bin/elanc-go" "$(PREFIX)/share/elan-go"
	rm -f "$(PREFIX)/bin/elanc"  # bare-elanc-ok: the install removes what bin/elanc was
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

# Module dependency rules (tests/architecture, S3a spec D2), the
# compiler's copies of the interpreter's limits and codes (REF contract), and
# no script calling the bare elanc (REM is elanc-rem, S7e task 9)
# No two tracked paths may differ only by case (CASE-RENAMES.tsv).
check-case:
	tests/check_case.sh

check-arch:
	cd tests/architecture && python3 test_check_deps.py && python3 test_check_limits.py && python3 test_check_rem_name.py
	python3 tests/architecture/check_deps.py
	python3 tests/architecture/check_limits.py
	python3 tests/architecture/check_rem_name.py

# C++ unit tests of the interpreter modules (tests/unit, CTest)
check-unit: all
	cd $(BUILD) && ctest --output-on-failure --no-tests=error

# Compiled tests of the bench (spec S5a, D4): elanc-rem -> make -> a.out, against
# the installed compiler (the generated programs are built with the
# compilers of this build, see src/compiler/CMakeLists.txt), after the tests
# of elanc-rem, of the strategies evaluated at run time and of the BenchThesis
# programs ported to ELAN 3 (tests/compiler).
# The compiled tests run only when the compiler is installed (not with
# -DELAN_COMPILER=OFF).
CHECK_COMPILER = if [ -x "$(PREFIX)/bin/elanc-rem" ]; then \
	  tests/compiler/test_elanc.sh "$(PREFIX)" && tests/compiler/test_runtime_strategies.sh "$(PREFIX)" \
	  && tests/compiler/test_start_term.sh "$(PREFIX)" \
	  && tests/compiler/test_benchthesis.sh "$(PREFIX)" \
	  && $(BENCH) --prefix $(PREFIX) --kinds J,JO; \
	else echo "compiler not built (-DELAN_COMPILER=OFF): compiled tests (J, JO) skipped"; fi
check-compiler: install
	$(CHECK_COMPILER)

# What bin/elanc is (S7e task 11): a symlink to elanc-go (ELAN_ELANC=go, the
# default; elanc-rem when elanc-go is not built) or to elanc-rem
# (CMAKE_FLAGS=-DELAN_ELANC=rem); CMAKE_CHECK=1 also configures scratch build
# directories for the fallback and the errors of ELAN_ELANC.
# (the forms CMake accepts: -DELAN_ELANC=rem, -DELAN_ELANC:STRING=rem,
# -D ELAN_ELANC=rem; `make elanc-kind` prints it)
EMPTY :=
SPACE := $(EMPTY) $(EMPTY)
ELANC_KIND = $(if $(filter -DELAN_ELANC=rem -DELAN_ELANC:STRING=rem,$(subst -D$(SPACE),-D,$(strip $(CMAKE_FLAGS)))),rem,go)
elanc-kind:
	@echo $(ELANC_KIND)
check-elanc: install
	CMAKE_CHECK=1 tests/compiler/test_elanc_switch.sh "$(PREFIX)" $(ELANC_KIND)
# the rem variant, in its own build directory (a full build), removed after
check-elanc-rem:
	$(MAKE) BUILD=build-elanc-rem CMAKE_FLAGS=-DELAN_ELANC=rem install \
	  && tests/compiler/test_elanc_switch.sh "$(CURDIR)/build-elanc-rem/install" rem; \
	st=$$?; rm -rf build-elanc-rem; exit $$st

# Golden tests (tests/golden): what the bench does not compare, byte for
# byte (.ref files, the -d dump, the statistics, stderr and exit status of
# errors), against the installed interpreter.
check-golden: install
	cd tests/golden && python3 test_run_golden.py
	python3 tests/golden/run_golden.py --prefix $(PREFIX)

# Tests of the Rust compiler (src/compiler-rs, spec S6): optional since S7e
# (spec G7, elanc-go has reached parity), they run only when elanc-rs is
# installed, i.e. in a build configured with -DELAN_RUST=ON (and cargo):
#   make BUILD=build-rust CMAKE_FLAGS=-DELAN_RUST=ON check-rust   (or check)
# (a separate BUILD keeps build/: changing CMAKE_FLAGS recreates the build
# directory).
check-rust: install
	@if [ -x "$(PREFIX)/bin/elanc-rs" ]; then \
	  (cd src/compiler-rs && CARGO_TARGET_DIR=$(CURDIR)/$(BUILD)/src/compiler-rs/target cargo test --offline --quiet) \
	  && CARGO_TARGET_DIR=$(CURDIR)/$(BUILD)/src/compiler-rs/target tests/compiler-rs/test_ref_roundtrip.sh "$(PREFIX)" rust \
	  && ELANC=elanc-rs tests/compiler-rs/test_normalise.sh "$(PREFIX)" \
	  && ELANC=elanc-rs tests/compiler-rs/test_strategies.sh "$(PREFIX)" \
	  && ELANC=elanc-rs tests/compiler-rs/test_strategies.sh "$(PREFIX)" -d strategy_terms \
	  && ELANC=elanc-rs tests/compiler-rs/test_depth.sh "$(PREFIX)" \
	  && tests/compiler-rs/bench_parity.py "$(PREFIX)" --kind R \
	  && ELANC=elanc-rs tests/compiler-rs/test_query.sh "$(PREFIX)"; \
	else echo "elanc-rs not built (-DELAN_RUST=OFF, the default since S7e): Rust tests skipped"; fi

# Tests of the Go compiler (src/compiler-go, spec S7), when go >= 1.24 exists.
GO_ENV = GOTOOLCHAIN=local GOFLAGS=-mod=mod GOPROXY=off GOWORK=off GOCACHE=$(CURDIR)/$(BUILD)/src/compiler-go/gocache
check-go: install
	@if [ -x "$(PREFIX)/bin/elanc-go" ]; then \
	  (cd src/compiler-go && $(GO_ENV) go vet ./... && $(GO_ENV) go test ./...) \
	  && (cd src/compiler-go/elanrt && $(GO_ENV) go vet ./... && $(GO_ENV) go test ./...) \
	  && $(GO_ENV) tests/compiler-rs/test_ref_roundtrip.sh "$(PREFIX)" go \
	  && ELANC=elanc-go tests/compiler-rs/test_normalise.sh "$(PREFIX)" \
	  && ELANC=elanc-go tests/compiler-rs/test_strategies.sh "$(PREFIX)" \
	  && ELANC=elanc-go tests/compiler-rs/test_strategies.sh "$(PREFIX)" -d strategy_terms \
	  && ELANC=elanc-go tests/compiler-rs/test_strategies.sh "$(PREFIX)" -d ac \
	  && ELANC=elanc-go tests/compiler-rs/test_strategies.sh "$(PREFIX)" -d io \
	  && ELANC=elanc-go tests/compiler-rs/test_strategies.sh "$(PREFIX)" -d arrays \
	  && ELANC=elanc-go tests/compiler-rs/test_depth.sh "$(PREFIX)" \
	  && tests/compiler-rs/bench_parity.py "$(PREFIX)" --kind G \
	  && ELANC=elanc-go tests/compiler-rs/test_query.sh "$(PREFIX)" \
	  && BENCHTHESIS_KINDS=G tests/compiler/test_benchthesis.sh "$(PREFIX)"; \
	else echo "elanc-go not built (go >= 1.24 missing or -DELAN_GO=OFF): Go tests skipped"; fi

# The MACC tables of the Go front end (spec S8, S8-4): regenerate
# src/compiler-go/front/lr/tables_gen.go and front/tables/keywords_gen.go from
# the build's generated parsers (checked in; ELAN_GENERATED_DIR=... go test
# ./front/lr/gen checks that they are up to date).
front-tables: all
	cd src/compiler-go && $(GO_ENV) go run ./front/lr/gen -in $(CURDIR)/$(BUILD)/src/interpreter/generated -out .

# Tests of the Go front end (src/compiler-go/front, spec S8, plan S8a), when
# go >= 1.24 exists: against the C++ through the harness elan-lextrace
# (tests/front/README.md).
check-front: install
	@if [ -x "$(PREFIX)/bin/elanc-go" ]; then \
	  (cd src/compiler-go && $(GO_ENV) go vet ./front/... \
	     && ELANLIB=$(PREFIX) ELAN_GENERATED_DIR=$(CURDIR)/$(BUILD)/src/interpreter/generated $(GO_ENV) go test ./front/...) \
	  && tests/front/check_lextrace_hooks.sh $(BUILD) \
	  && tests/front/test_check_lextrace_hooks.sh $(BUILD) \
	  && tests/front/test_lextrace_harness.sh $(BUILD) \
	  && python3 tests/front/test_tracecmp.py \
	  && tests/front/test_front_load.sh $(PREFIX) \
	  && tests/front/test_front_lex.sh $(PREFIX) \
	  && tests/front/test_front_errors.sh $(PREFIX); \
	else echo "elanc-go not built (go >= 1.24 missing or -DELAN_GO=OFF): front tests skipped"; fi

check: check-case smoke test-runner check-arch check-unit check-golden check-rust check-go check-elanc check-front
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
	PREFIX="$(REFERENCE_PREFIX)" reference/build.sh

check-reference:
	$(BENCH) --prefix "$(REFERENCE_PREFIX)"

clean:
	rm -rf $(BUILD)
