# ELAN 3.6 user manual

LaTeX sources of the ELAN user manual (V3.6, 2003), copied from
`legacy/elan3/doc`, and the rebuilt `manual.pdf`. Rebuild with `make manual` (output in `build-manual/manual.pdf`; `make manual-update` refreshes this copy)
(needs `latex` and `dvipdf`, e.g. MacTeX or TeX Live + Ghostscript).

The text is unchanged. Only the tooling that no longer exists was replaced:

* `fancyhead.sty` — shim loading `fancyhdr` (its modern name);
* `lgrind.sty` — removed from TeX Live; shim loading `lgrind-ck.sty`, the copy
  of the package that came with the 2003 doc;
* listings — the `lgrind` *program* that turned `ElanExamples/*.eln` into
  `ElanTeX/*.tex` no longer exists: `\insertCode` (macros.tex) now includes the
  original sources verbatim (`fancyvrb`);
* the title page and footers carry a fixed date ("V3.6 (2003), rebuilt 2026")
  instead of `\today`, so that rebuilding gives the same document.

The 2003 build tree held a truncated `manual.dvi` (5 pages); this rebuild has
99 pages. The examples of the manual are runnable in `examples/`.
