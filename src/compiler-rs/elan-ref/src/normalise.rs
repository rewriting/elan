//! Comparison of `.ref` files modulo white space and comments, for the
//! round-trip tests. Deliberately independent of [`crate::lexer`]: a token is
//! a quoted name (up to the `"` followed by `.`, see [`crate::lexer::tokenize`]),
//! a run of letters, digits, `_`, `*` and `-`, or any other single character.

/// The tokens of `src`, white space and `/* */`, `//` comments removed.
pub fn tokens(src: &[u8]) -> Vec<&[u8]> {
    let word = |b: u8| b.is_ascii_alphanumeric() || b == b'_' || b == b'*' || b == b'-';
    let mut v = Vec::new();
    let mut i = 0;
    while i < src.len() {
        let b = src[i];
        if b.is_ascii_whitespace() {
            i += 1;
        } else if src[i..].starts_with(b"/*") {
            i = match src[i + 2..].windows(2).position(|w| w == b"*/") {
                Some(p) => i + 2 + p + 2,
                None => src.len(),
            };
        } else if src[i..].starts_with(b"//") {
            while i < src.len() && src[i] != b'\n' {
                i += 1;
            }
        } else if b == b'"' {
            let mut j = i + 1;
            while j < src.len() && !(src[j] == b'"' && src.get(j + 1) == Some(&b'.')) {
                j += 1;
            }
            let end = (j + 1).min(src.len());
            v.push(&src[i..end]);
            i = end;
        } else if word(b) {
            let mut j = i;
            while j < src.len() && word(src[j]) {
                j += 1;
            }
            v.push(&src[i..j]);
            i = j;
        } else {
            v.push(&src[i..i + 1]);
            i += 1;
        }
    }
    v
}

/// `None` when `a` and `b` have the same tokens, else a description of the
/// first difference (token position and a few tokens of context).
pub fn first_difference(a: &[u8], b: &[u8]) -> Option<String> {
    let (ta, tb) = (tokens(a), tokens(b));
    let n = ta.iter().zip(&tb).take_while(|(x, y)| x == y).count();
    if n == ta.len() && n == tb.len() {
        return None;
    }
    let ctx = |t: &[&[u8]]| -> String {
        let lo = n.saturating_sub(8);
        let hi = (n + 8).min(t.len());
        t[lo..hi]
            .iter()
            .map(|x| String::from_utf8_lossy(x).into_owned())
            .collect::<Vec<_>>()
            .join(" ")
    };
    Some(format!(
        "token {n} of {}/{}:\n  original: {}\n  printed:  {}",
        ta.len(),
        tb.len(),
        ctx(&ta),
        ctx(&tb)
    ))
}
