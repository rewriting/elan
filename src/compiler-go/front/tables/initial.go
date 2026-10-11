package tables

import "elan/front/diag"

// NewTypeT is typet (load/module.cc): the sort table, NTypes, with its seven
// built-in sorts.
func NewTypeT(d *diag.D) *StringTab {
	return NewStringTab(d, NTypes, "bool", "ident", "builtinInt",
		"builtinString", "intern string", "intern ident", "intern int")
}

// NewImport is import (load/module.cc): the module table, MaxNofImports.
func NewImport(d *diag.D) *StringTab { return NewStringTab(d, MaxNofImports) }

// NewBuiltinModules is builtinmodules (load/module.cc): the 33 modules
// allowed to use `code`, in a table of 64.
func NewBuiltinModules(d *diag.D) *StringTab {
	return NewStringTab(d, 64, "bool", "builtinInt", "ident", "double",
		"doubleConstants", "common",
		"cmp", "replace", "occur", "builtinSyntacticMatching",
		"builtinIO", "builtinStdio",
		"Query",
		"builtinString",
		"test", "pem1", "pem2", "fac",
		"Meta_apply", "Meta_capply", "Meta_strat",
		"strass",
		"REF",
		"refstring2string",
		"ref2string",
		"Meta_Apply",
		"divers2string",
		"strsig",
		"strat",
		"strconc",
		"builtinArray",
		"builtinHashTerm",
		"builtinEqMolecule")
}
