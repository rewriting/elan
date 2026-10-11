#!/usr/bin/env python3
"""elanc-go against REM on the bench (S7e task 1): per G case of
tests/legacy-bench (run_tests.discover()), the program is built by REM
(`elanc-rem` when installed, else `elanc`, then `make -f <prog>.make`) and by
elanc-go, and the table gives:

  compile J: elanc (REM: export, Java, C generation) and make (C build), s;
  compile G: a new program (runtime in the warm cache) and the same program
             built again (unchanged), s;
  run J/G:   best wall time of `./a.out -noInput -quiet`, ms (3 untimed runs
             each, then N runs interleaved J, G, J, G, ...);
  RSS J/G:   largest maximum resident set size of the timed runs, MB;
  steps J/G: the rewrite_step line; status J/G: the exit statuses.

--check fails (exit 1) when, for a case both compilers build, the steps or
the statuses differ, or G is slower than J by more than max(1 ms, 5 %) (the
noise of a process start: S7e plan, finding 2; `SLOWER <case>: ...`), or
when G fails where J builds, or a G compile of a new program takes more than
2 s (criterion 4 of the S7 spec). Cases that neither builds are counted.

Not part of `make check` (timings depend on the machine); run it, with
bench_perf.sh --check, after a change of the runtime or of the generated
code. The machine should be otherwise idle.

Usage: bench_jg.py PREFIX [--filter REGEX] [--runs N] [--check] [--tsv FILE]
The work directory tests/legacy-bench/work/jg (git-ignored) holds the shared
go build cache (gocache, warmed by one build first) and one case at a time.
"""
import argparse, os, re, shutil, subprocess, sys, time
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
BENCH = REPO / "tests" / "legacy-bench"
WORK = BENCH / "work" / "jg"
SLOW_ABS_MS, SLOW_REL, COMPILE_MAX_S = 1.0, 0.05, 2.0
RUN_TIMEOUT = 120


def parse(argv):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("prefix")
    ap.add_argument("--filter", default="")
    ap.add_argument("--runs", type=int, default=15)
    ap.add_argument("--check", action="store_true")
    ap.add_argument("--tsv")
    return ap.parse_args(argv)


def main(argv):
    a = parse(argv)
    prefix = Path(a.prefix).resolve()
    os.environ["ELAN_PREFIX"] = str(prefix)   # read by run_tests at import
    sys.path.insert(0, str(BENCH))
    import run_tests as R

    env = R.env()
    env["LC_ALL"] = "C"
    env["ELANC_GO_CACHE"] = str(WORK / "gocache")
    rem = "elanc-rem" if (prefix / "bin" / "elanc-rem").exists() else "elanc"
    WORK.mkdir(parents=True, exist_ok=True)

    def sh(cmd, cwd, timeout=300):
        t0 = time.perf_counter()
        try:
            p = subprocess.run(cmd, cwd=cwd, stdin=subprocess.DEVNULL, capture_output=True,
                               env=env, timeout=timeout)
            return p.returncode, time.perf_counter() - t0, (p.stdout + p.stderr).decode("latin-1")
        except subprocess.TimeoutExpired:
            return -9, time.perf_counter() - t0, "TIMEOUT"

    def run_rss(wd):
        """(status, wall s, max RSS MB, stdout) with the child's own rusage."""
        t0 = time.perf_counter()
        r, w = os.pipe()
        pid = os.fork()
        if pid == 0:
            try:
                os.chdir(wd)
                os.dup2(os.open(os.devnull, os.O_RDONLY), 0)
                os.dup2(w, 1)
                os.dup2(os.open(os.devnull, os.O_WRONLY), 2)
                os.close(r)
                os.execve("./a.out", ["./a.out", "-noInput", "-quiet"], env)
            finally:
                os._exit(127)
        os.close(w)
        chunks = []
        while True:
            b = os.read(r, 1 << 16)
            if not b:
                break
            chunks.append(b)
        os.close(r)
        _, st, ru = os.wait4(pid, 0)
        dt = time.perf_counter() - t0
        rss = ru.ru_maxrss / (1048576 if sys.platform == "darwin" else 1024)
        return os.waitstatus_to_exitcode(st), dt, rss, b"".join(chunks).decode("latin-1")

    def build_j(t, wd, flags, spc):
        rc, d1, o = sh([rem, "-b", "-nosplit", "-quiet", *flags, t["lgi"], *spc], wd)
        mk = wd / f"{t['lgi']}.make"
        if not mk.exists():
            return f"ERROR-{rem}", d1, None
        rc, d2, o = sh(["make", "-f", mk.name], wd)
        if rc != 0 or not (wd / "a.out").exists():
            return "ERROR-make", d1, d2
        return None, d1, d2

    def build_g(t, wd, flags, spc):
        rc, d1, o = sh(["elanc-go", "-b", *flags, t["lgi"], *spc], wd)
        if rc != 0 or not (wd / "a.out").exists():
            return f"ERROR-elanc-go-{rc}", d1, None
        (wd / "a.out").unlink()
        rc, d2, o = sh(["elanc-go", "-b", *flags, t["lgi"], *spc], wd)
        if rc != 0 or not (wd / "a.out").exists():
            return f"ERROR-elanc-go-{rc}", d1, d2
        return None, d1, d2

    def copy(t, dst):
        shutil.rmtree(dst, ignore_errors=True)
        shutil.copytree(t["dir"], dst, symlinks=True,
                        ignore=shutil.ignore_patterns("CVS", "*.o", "a.out", ".elan.*", ".elan-go.*"))

    tests = [t for t in R.discover() if t["kind"] == "G" and re.search(a.filter, t["id"])
             and (t["dir"] / f"{t['lgi']}.lgi").exists()]
    if not tests:
        print("bench_jg.py: no case", file=sys.stderr)
        return 2

    # warm the go build cache (the runtime compiled once) with the first case
    t0 = tests[0]
    warm = WORK / "warm" / t0["dir"].name
    copy(t0, warm)
    sh(["elanc-go", "-b", *[x for x in t0["flags"] if x], t0["lgi"],
        *([] if t0["spc"] == "no" else [f"{t0['spc']}.spc"])], warm)
    shutil.rmtree(WORK / "warm", ignore_errors=True)

    cols = ["case", "J_status", "J_elanc_s", "J_make_s", "J_run_ms", "J_rss_MB", "J_steps",
            "G_status", "G_new_s", "G_again_s", "G_run_ms", "G_rss_MB", "G_steps", "check"]
    tsv = open(a.tsv, "w") if a.tsv else None
    if tsv:
        tsv.write("\t".join(cols) + "\n")
    print(f"{'case':58s} {'Jelanc':>6s} {'Jmake':>6s} {'Gnew':>6s} {'Gagain':>6s} "
          f"{'Jms':>8s} {'Gms':>8s} {'JMB':>6s} {'GMB':>6s} {'steps':>10s} st")
    problems, both_fail, compared = [], 0, 0
    for t in tests:
        cid = t["id"].replace("::G::", "::")
        spc = [] if t["spc"] == "no" else [f"{t['spc']}.spc"]
        flags = [x for x in t["flags"] if x]
        base = WORK / "case"
        shutil.rmtree(base, ignore_errors=True)
        wdj, wdg = base / "J" / t["dir"].name, base / "G" / t["dir"].name
        copy(t, wdj)
        copy(t, wdg)
        ej, j1, j2 = build_j(t, wdj, flags, spc)
        eg, g1, g2 = build_g(t, wdg, flags, spc)
        row = dict(case=cid, J_elanc_s=f"{j1:.2f}", J_make_s="-" if j2 is None else f"{j2:.2f}",
                   G_new_s=f"{g1:.2f}", G_again_s="-" if g2 is None else f"{g2:.2f}")
        mark = []
        res = {}
        for k, wd, err in (("J", wdj, ej), ("G", wdg, eg)):
            if err:
                res[k] = [err, None, 0.0, "-"]
                continue
            st, dt, rss, out = run_rss(wd)          # untimed: status, output
            res[k] = [st, dt, rss, out]
            if dt <= 10:
                for _ in range(2):
                    run_rss(wd)
                res[k][1] = float("inf")
        if not ej and not eg and res["J"][1] == float("inf") and res["G"][1] == float("inf"):
            for _ in range(a.runs):           # interleaved J, G
                for k, wd in (("J", wdj), ("G", wdg)):
                    st, dt, rss, out = run_rss(wd)
                    r = res[k]
                    r[1], r[2] = min(r[1], dt), max(r[2], rss)
        else:                                 # long or failing: one more run each
            for k, wd, err in (("J", wdj, ej), ("G", wdg, eg)):
                if not err and res[k][1] == float("inf"):
                    st, dt, rss, out = run_rss(wd)
                    res[k][1], res[k][2] = dt, max(res[k][2], rss)
        for k in ("J", "G"):
            st, best, rss, out = res[k]
            m = re.search(r"rewrite_step = (\d+)", out)
            row[f"{k}_status"] = str(st)
            row[f"{k}_run_ms"] = "-" if best is None else f"{best * 1000:.2f}"
            row[f"{k}_rss_MB"] = f"{rss:.1f}"
            row[f"{k}_steps"] = m.group(1) if m else "-"
        if ej and eg:
            both_fail += 1
            mark.append("both fail")
        elif eg:
            mark.append(f"G FAILS: {eg}")
        elif ej:
            mark.append(f"J fails ({ej})")
        else:
            compared += 1
            if row["J_status"] != row["G_status"]:
                mark.append(f"STATUS {row['J_status']} != {row['G_status']}")
            if row["J_steps"] != row["G_steps"]:
                mark.append(f"STEPS {row['J_steps']} != {row['G_steps']}")
            jms, gms = res["J"][1] * 1000, res["G"][1] * 1000
            if gms - jms > max(SLOW_ABS_MS, SLOW_REL * jms):
                mark.append(f"SLOWER {gms:.2f} ms > {jms:.2f} ms")
        if g1 > COMPILE_MAX_S and not eg:
            mark.append(f"COMPILE {g1:.2f} s > {COMPILE_MAX_S} s")
        row["check"] = "; ".join(mark) or "ok"
        bad = [x for x in mark if not x.startswith(("both fail", "J fails"))]
        if bad:
            problems.append(f"{cid}: {'; '.join(bad)}")
        print(f"{cid[:58]:58s} {row['J_elanc_s']:>6s} {row['J_make_s']:>6s} {row['G_new_s']:>6s} "
              f"{row['G_again_s']:>6s} {row['J_run_ms']:>8s} {row['G_run_ms']:>8s} "
              f"{row['J_rss_MB']:>6s} {row['G_rss_MB']:>6s} {row['G_steps']:>10s} {row['check']}",
              flush=True)
        if tsv:
            tsv.write("\t".join(row[c] for c in cols) + "\n")
            tsv.flush()
        shutil.rmtree(base, ignore_errors=True)
    if tsv:
        tsv.close()
    print(f"{len(tests)} cases: {compared} built by both, {both_fail} by neither, "
          f"{len(problems)} with a problem")
    for p in problems:
        print(p)
    return 1 if a.check and problems else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
