# The REF contract

`elan --export F.ref` (and `--cexport`, which the compiler driver `elanc`
uses) writes a loaded program as text: its tables, grammars, rules,
strategies and query. Three readers depend on it:

* `elan --import F.ref` (interpreter, `src/interpreter/ref/ref.cc`);
* REM, the Java compiler front end (`src/compiler/rem`), which turns it into C;
* the compiled runtime and its Earley parser (`src/compiler/runtime`,
  `src/compiler/earley`), which parse queries and print terms with the
  exported tables.

A `.ref` file contains **numbers, not names**, wherever a name is used, so
the numbering is part of the contract. The bench compares standard output
only and would not notice a renumbering: the golden tests
(`tests/golden`, `make check-golden`) pin the `.ref` files of 27 programs
byte for byte.

## What the numbers are

| in the file | number | where it comes from |
|---|---|---|
| `Identifiers` section, `Ident(n)`, `IDENT(n)` | position of the identifier in `tabofident` | hash position in a table of `MAXNOFIDENT` = 3000 entries |
| `Sorts` section, `Type(n)`, `GrammarForSort n`, sort fields of `RULE`, `VAR(i,n)`, `STRATEGY`, `QUERY` | position of the sort in `typet` | hash position, `NTYPES` = 500 entries (built-in sorts first: `bool`, `ident`, `builtinInt`, ...) |
| `Modules` section, module fields of `RULE` and `STRATEGY` | position of the module in `import` | hash position, `MAXNOFIMPORTS` = 200 |
| `RuleNames` section, first field of `RULE` | position of the rule name | hash position, `MAXNOFTRN` = 2000 |
| `StrategyNames` section, `STRATEGY(n,...)` | position of the strategy name | hash position, `MAXNOFSTRAT` = 500 |
| second field of a grammar rule, last field of `FSYM(...)` | function symbol code | built-ins below `FSYMCODESBEG` = 300 (`term/codes.h`), user symbols from 300, `RULECONSTRULE`.. = `MAXNFSYM`+1..+4 |
| seventh field of a grammar rule (`defstrat`) | packed word | `FSYM_FLAG`/`LAB_FLAG`/`DSTR_FLAG`/`APPLY_FLAG` (`term/codes.h`): a flag in bits 24-27 and two symbol codes in 12 bits each |
| `Char(c)`, `Num(n)`, `INT(n)`, `STRING(c.c.nil)` | character codes, values | |

A hash position is computed from the characters of the name and the size of
the table (`base/stringtab.cc`: sum of the characters modulo the size,
collisions probed by steps of 211). Changing a table size renumbers every
name; the order of insertion decides the collisions.

## Constants that must not change

Changing any of these renumbers the exported programs (and breaks the copies
in the compiler):

* hashed table sizes: `MAXNOFIDENT`, `NTYPES`, `MAXNOFIMPORTS`, `MAXNOFTRN`,
  `MAXNOFSTRAT`, `MRWTSIZE`, `MAXNOFMAC` (`base/constants.h`);
* `FSYMCODESBEG` and the built-in symbol codes (`term/codes.h`);
* the `defstrat` packing: every symbol code (up to `MAXNFSYM`+4) must fit in
  12 bits;
* `IDLEN` (identifiers are truncated to 49 characters before hashing);
* the `MAXLENNTERM` default (printed by `elan --help`, can be changed with
  `--MAXLENNTERM n`).

`base/constants.h` checks them with `static_assert`s. The lexem encoding
(the ranges of characters, identifiers, strings, numbers and sorts, also in
`base/constants.h`) does not appear in `.ref` files, but the Earley parser of
the compiled runtime keeps a copy of it.

## Copies in the compiler

| copy | must equal |
|---|---|
| `src/compiler/runtime/termIn.h`: `TABOFIDENT_SIZE`, `TABOFSORT_SIZE` | `MAXNOFIDENT`, `NTYPES` |
| `src/compiler/earley/runtimeInit.cc`: `stringtab tabofident(...)`, `atabofident(...)`, `typet(...)` | `MAXNOFIDENT`, `MAXNOFIDENT`, `NTYPES` |
| `src/compiler/earley/commondefs.h`: `MAXNOFIDENT`, `NTYPES`, `MAXNFSYM`, `IDLEN`, the lexem ranges | the interpreter's |
| `src/compiler/runtime/codes.h`, `src/compiler/earley/codes.h` | the built-in codes of `term/codes.h` |

`tests/architecture/check_limits.py` (run by `make check-arch`) checks them.
`src/compiler/earley/commondefs.h` also defines `FSYMCODESBEG` as 200, a stale
value from an older interpreter: it is unused, and the check fails if code in
`src/compiler` starts using it.

## Layout

Sections, in order (`driver/ldmain.cc`, `ref/ref.cc`):

1. `Identifiers`, `Sorts`, `Modules`, `RuleNames`, `StrategyNames`: one
   `n:"name".` line per entry, in table order, then `nil end`;
2. three grammar dumps, each `GrammarForSort s:b:` blocks (`b` = built-in
   sort) of rules `flag:symbol:priority:printable+assoc:semantic:infos:defstrat:rhs-lexems.nil:nil.`,
   then `EndDef end`;
3. `RULE(name,sort,module,infos,whichmatch,nvars,lhs,rhs,wheres)` (and
   `SWRULE` for rules with a strategy right-hand side), then `EndDef end`;
4. `STRATEGY(name,sort,module,body)`, then `EndDef end`;
5. `QUERY(sort,resultsort,strategy,startwith,checkwith) end`.
