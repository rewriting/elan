//! Rust identifiers for ELAN sorts and operators.
//!
//! ELAN names may contain any character (`list[int]`, `@ + @`); the
//! generated names keep the letters and digits and name the other
//! characters (`+` is `plus`), so that the generated code stays readable.
//! Uniqueness is ensured by the callers ([`unique`]); function names carry
//! the symbol code.

use std::collections::HashSet;

/// Names that the generated code uses for other things (runtime items,
/// Rust prelude, keywords that are capitalised).
const RESERVED: &[&str] = &[
    "H",
    "Builtin",
    "Int",
    "Str",
    "Print",
    "Writer",
    "Sort",
    "Session",
    "Option",
    "Some",
    "None",
    "Result",
    "Ok",
    "Err",
    "String",
    "Vec",
    "Box",
    "Rc",
    "Self",
    "Fn",
    "FnMut",
    "FnOnce",
    "Copy",
    "Clone",
    "Send",
    "Sync",
    "Sized",
    "Drop",
    "Default",
    "Eq",
    "PartialEq",
    "Hash",
    "Debug",
    "Ord",
    "PartialOrd",
    "Iterator",
];

/// A name for a character of an operator's syntax.
pub fn char_name(c: char) -> Option<&'static str> {
    Some(match c {
        '+' => "plus",
        '-' => "minus",
        '*' => "star",
        '/' => "slash",
        '%' => "percent",
        '&' => "amp",
        '|' => "bar",
        '.' => "dot",
        '<' => "lt",
        '>' => "gt",
        '=' => "eq",
        '!' => "bang",
        '~' => "tilde",
        '^' => "caret",
        ':' => "colon",
        ';' => "semi",
        '#' => "hash",
        '$' => "dollar",
        '?' => "q",
        '\'' => "quote",
        '"' => "dquote",
        '\\' => "bslash",
        '`' => "bquote",
        '[' => "lb",
        ']' => "rb",
        '{' => "lc",
        '}' => "rc",
        '@' => "at",
        // parentheses and commas are the usual prefix syntax: no name
        '(' | ')' | ',' => return None,
        _ => "c",
    })
}

/// Letters, digits and `_` of `s`, other characters as `_`.
fn sanitise(s: &str) -> String {
    s.chars()
        .map(|c| if c.is_ascii_alphanumeric() { c } else { '_' })
        .collect()
}

/// The base name of an operator from the texts of its syntax tokens
/// (identifiers and characters, arguments left out): `fact(@)` gives
/// `fact`, `@ + @` gives `plus`, `@` (an injection) gives `inj`.
pub fn op_base_name(tokens: &[String]) -> String {
    let mut parts: Vec<String> = Vec::new();
    for t in tokens {
        let mut cs = t.chars();
        match (cs.next(), cs.next()) {
            (Some(c), None) if !c.is_ascii_alphanumeric() => {
                if let Some(n) = char_name(c) {
                    parts.push(n.to_string())
                }
            }
            _ => parts.push(sanitise(t)),
        }
    }
    let s = parts.join("_");
    let s = s.trim_matches('_').to_string();
    if s.is_empty() {
        "inj".to_string()
    } else if s.starts_with(|c: char| c.is_ascii_digit()) {
        format!("n{s}")
    } else {
        s
    }
}

fn capitalise(s: &str) -> String {
    let mut out = String::new();
    let mut up = true;
    for c in s.chars() {
        if c == '_' {
            up = true;
            continue;
        }
        if up {
            out.extend(c.to_uppercase());
            up = false;
        } else {
            out.push(c);
        }
    }
    out
}

/// The Rust type name of a sort: `list[int]` gives `ListInt`.
pub fn type_name(sort: &str) -> String {
    let n = capitalise(&sanitise(sort));
    let n = if n.is_empty() || n.starts_with(|c: char| c.is_ascii_digit()) {
        format!("S{n}")
    } else {
        n
    };
    if RESERVED.contains(&n.as_str()) {
        format!("{n}Sort")
    } else {
        n
    }
}

/// A variant name for an operator: `plus` gives `Plus`.
pub fn variant_name(base: &str, symbol: i32) -> String {
    let n = capitalise(base);
    if n.is_empty() {
        format!("Op{symbol}")
    } else if RESERVED.contains(&n.as_str()) {
        format!("{n}{symbol}")
    } else {
        n
    }
}

/// `name`, or `name_2`, `name_3`... if already used; records the result.
pub fn unique(used: &mut HashSet<String>, name: String) -> String {
    let mut n = name.clone();
    let mut i = 2;
    while used.contains(&n) {
        n = format!("{name}_{i}");
        i += 1;
    }
    used.insert(n.clone());
    n
}

#[cfg(test)]
mod tests {
    use super::*;

    fn toks(v: &[&str]) -> Vec<String> {
        v.iter().map(|s| s.to_string()).collect()
    }

    #[test]
    fn operator_names() {
        assert_eq!(op_base_name(&toks(&["fact", "(", ")"])), "fact");
        assert_eq!(op_base_name(&toks(&["+"])), "plus");
        assert_eq!(op_base_name(&toks(&[])), "inj");
        assert_eq!(op_base_name(&toks(&["[", "]"])), "lb_rb");
        assert_eq!(
            op_base_name(&toks(&["string_strlen", "(", ")"])),
            "string_strlen"
        );
        assert_eq!(op_base_name(&toks(&["1st"])), "n1st");
    }

    #[test]
    fn type_and_variant_names() {
        assert_eq!(type_name("nat"), "Nat");
        assert_eq!(type_name("list[int]"), "ListInt");
        assert_eq!(type_name("builtinInt"), "BuiltinInt");
        assert_eq!(type_name("int"), "IntSort");
        assert_eq!(type_name("string"), "StringSort");
        assert_eq!(variant_name("plus", 3), "Plus");
        assert_eq!(variant_name("self", 400), "Self400");
        let mut used = HashSet::new();
        assert_eq!(unique(&mut used, "a".into()), "a");
        assert_eq!(unique(&mut used, "a".into()), "a_2");
    }
}
