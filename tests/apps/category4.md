# S9b: differences found while testing (spec S9 §5, category 4)

## Modern interpreter vs the 2004 reference (category 4)

Programs whose modern interpreter output differs from the reference on the
query written for them: their test is not added and they stay in `todo.tsv`.

(none so far)

## elanc-go vs the reference REM (gtest lines not added)

The `itest`/`aitest` lines of these programs pass; their `gtest` line was
not added because the output of the program compiled by elanc-go differs
from the one compiled by the reference's REM (`make_expected.py`).

Most come from the start term: a program compiled by REM that reads a query
applies the start strategy to the query and ignores the start term of the
`.lgi` (`start with (s) f(query)` behaves as `(s) query`), whereas elanc-go
evaluates the start term as the interpreter does.

| program | query | REM | elanc-go |
|---|---|---|---|
| Features/i2str | `a` | `result = a` (start term `(ident2string(query),...)` ignored) | `result = (string_ident2string(a),string_ident2string(a))` (not evaluated, unlike the interpreter: `("a","a")`) |
| Features/longnames | `3` | no result (`doit(query)` ignored, `allsols` fails on 3) | `result = 3` three times (as the interpreter) |
| Features/mc | `(la la la (2 ((3 2) 1)) la la 1)` | `result = la la(21)` (the query normalised) | the start term normalised, as the interpreter |
| Features/nqueens | `4` | no result (`queens(query)` ignored) | the two solutions, as the interpreter |
| Features/tri, Features/trii | `a` | `result = a` | `result = f(a)` (start term `f(a)`, as the interpreter) |
