# Source this file to use the reference ELAN build:  source reference/env.sh
_elan_ref="$(cd "$(dirname "${BASH_SOURCE[0]:-${(%):-%x}}")" && pwd)"
export ELANLIB="$_elan_ref/install"     # the interpreter looks in $ELANLIB/share/elanlib
export JAVA_HOME="${JAVA_HOME:-/opt/homebrew/opt/openjdk}"
export PATH="$_elan_ref/install/bin:$JAVA_HOME/bin:$PATH"
unset _elan_ref
