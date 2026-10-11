package elanrt

import (
	"bufio"
	"errors"
	"fmt"
	"io"
	"os"
	"strings"

	"elan/elanrt/earley"
)

// Input/output builtins (S7e task 6): REM's fun_113, 114, 116, 118-122
// (src/compiler/runtime/builtin.h; termCommon.c open_file, close_file,
// flush_file, Getc, Putc; termOut.c term_write; termIn.c term_read), the
// operators of common/builtinStdio.eln and builtinIO.eln. REM has no
// fun_115, 117 (create a process) and 123 (Error): elanc-go refuses them
// too.
//
// A file is a number, an index into a table of MaxFile entries: 0, 1 and
// 2 are the standard input, output and error; Open takes the first free
// number. The standard output is [Out] (in order with the results); the
// standard error is unbuffered; the other files are buffered as C's
// FILEs, flushed by Flush, Close and the end of the program
// ([FlushOutput]). The effects are never undone (backtracking does not
// restore a file), as in REM.
//
// REM's semantics kept: getc gives C's char (signed: a byte >= 128 is
// negative) or -1 at the end of the file (or on a stream not open for
// reading); putc writes the byte c (C's (char)c) and gives c; open gives
// -1 and writes `Error in open_file(<file>,<mode>)` on the standard error
// when the file cannot be opened; close and flush give their argument;
// write prints the term as a result is printed (no newline; the printer's
// spacing state is the results', a global in C) and gives it; read parses
// a term of the sort asked for up to `end` (and one more character, as
// REM's lexer), then prints a newline on the standard output (REM's
// term_read), and stops the program (status 1) on a syntax error.
// Not REM's: a number that is not an open file stops the program with a
// run-time error (status 1; REM reads an invalid FILE pointer, undefined
// behaviour, typically a segmentation fault).

// MaxFile is the number of entries of the table of files (REM's MAXFILE).
const MaxFile = 300

// ioFile is an open file of the table.
type ioFile struct {
	f           *os.File // nil for the standard output ([Out])
	r           *bufio.Reader
	w           *bufio.Writer // nil: unbuffered (the standard error), or [Out]
	read, write bool
}

var (
	files [MaxFile]*ioFile
	// stdin is the standard input, shared by the query's parser and the
	// file 0 (a C FILE: what one has read, the other does not read again)
	stdin = bufio.NewReader(os.Stdin)
	// ioStderr receives the messages of Open (tests)
	ioStderr io.Writer = os.Stderr
	// printer prints the terms of WriteTerm: the [Session]'s printer (its
	// spacing state is the results')
	printer = &Writer{}
)

func init() {
	files[0] = &ioFile{f: os.Stdin, r: stdin, read: true}
	files[1] = &ioFile{write: true}
	files[2] = &ioFile{f: os.Stderr, write: true}
}

// file is the open file pid, or a run-time error naming the operation op.
func file(pid int32, op string) *ioFile {
	if pid < 0 || pid >= MaxFile || files[pid] == nil {
		Fatal(fmt.Sprintf("%s_builtinStdio: file %d is not open", op, pid))
	}
	return files[pid]
}

// flush writes what f holds.
func (f *ioFile) flush() {
	if f.f == nil && f.write {
		FlushOutput()
	} else if f.w != nil {
		f.w.Flush()
	}
}

// writeBytes writes b to f (nothing if f is not open for writing).
func (f *ioFile) writeBytes(b []byte) {
	if !f.write {
		return
	}
	switch {
	case f.f == nil:
		Out.Write(b)
	case f.w == nil:
		f.f.Write(b)
	default:
		if f.r != nil && f.r.Buffered() > 0 {
			// read ahead: the file's position is where reading stopped
			f.f.Seek(-int64(f.r.Buffered()), io.SeekCurrent)
			f.r.Reset(f.f)
		}
		f.w.Write(b)
	}
}

// reader is f's reader, nil if f is not open for reading.
func (f *ioFile) reader() *bufio.Reader {
	if !f.read {
		return nil
	}
	if f.w != nil {
		f.w.Flush()
	}
	return f.r
}

// flushFiles writes what the open files hold (the end of the program, as
// C's exit).
func flushFiles() {
	for i := 3; i < MaxFile; i++ {
		if f := files[i]; f != nil && f.w != nil {
			f.w.Flush()
		}
	}
}

// Getc reads a char from the file pid (fun_113): C's signed char, -1 at
// the end.
func Getc(pid int32) int32 {
	r := file(pid, "getc").reader()
	if r == nil {
		return -1
	}
	b, err := r.ReadByte()
	if err != nil {
		return -1
	}
	return int32(int8(b))
}

// Putc writes the byte c to the file pid and returns c (fun_114).
func Putc(pid, c int32) int32 {
	file(pid, "putc").writeBytes([]byte{byte(c)})
	return c
}

// Open opens the file name with the fopen mode (fun_116): its number, or
// -1 (REM's message on the standard error).
func Open(name, mode string) int32 {
	flag, read, write, err := openMode(mode)
	var f *os.File
	if err == nil {
		f, err = os.OpenFile(name, flag, 0o666)
	}
	pid := int32(-1)
	for i := range files {
		if files[i] == nil {
			pid = int32(i)
			break
		}
	}
	if pid < 0 {
		fmt.Fprintf(ioStderr, "Too many files are opened (MAXFILE=%d).\n", MaxFile)
		Exit(1)
	}
	if err != nil {
		fmt.Fprintf(ioStderr, "Error in open_file(%s,%s)\n", name, mode)
		return -1
	}
	x := &ioFile{f: f, read: read, write: write}
	if read {
		x.r = bufio.NewReader(f)
	}
	if write {
		x.w = bufio.NewWriter(f)
	}
	files[pid] = x
	return pid
}

var errMode = errors.New("invalid mode")

// openMode is the meaning of a C fopen mode: r, w or a, then + (read and
// write), x (exclusive creation), b and e (no effect here).
func openMode(mode string) (flag int, read, write bool, err error) {
	if mode == "" {
		return 0, false, false, errMode
	}
	plus := strings.Contains(mode[1:], "+")
	switch mode[0] {
	case 'r':
		flag, read, write = os.O_RDONLY, true, plus
	case 'w':
		flag, read, write = os.O_WRONLY|os.O_CREATE|os.O_TRUNC, plus, true
	case 'a':
		flag, read, write = os.O_WRONLY|os.O_CREATE|os.O_APPEND, plus, true
	default:
		return 0, false, false, errMode
	}
	if plus {
		flag = flag&^os.O_WRONLY | os.O_RDWR
	}
	if mode[0] != 'r' && strings.Contains(mode[1:], "x") {
		flag |= os.O_EXCL
	}
	return flag, read, write, nil
}

// Close closes the file pid and returns pid (fun_118); its number is free
// again. Closing the standard output ends what the program prints, as
// REM's fclose(stdout).
func Close(pid int32) int32 {
	f := file(pid, "close")
	f.flush()
	if f.f == nil {
		os.Stdout.Close()
	} else {
		f.f.Close()
	}
	files[pid] = nil
	return pid
}

// Flush writes what the file pid holds and returns pid (fun_122).
func Flush(pid int32) int32 {
	file(pid, "flush").flush()
	return pid
}

// WriteTerm prints t on the file pid as a result is printed (no newline)
// and returns t (fun_119).
func WriteTerm(pid int32, t Term) Term {
	f := file(pid, "write")
	if on := printer.onArray; on != nil {
		// an array: what was printed goes to the file (REM's printer)
		printer.onArray = func(w *Writer) { arrayPrintedStop(w, writerFunc(f.writeBytes)) }
		defer func() { printer.onArray = on }()
	}
	printer.Term(t)
	f.writeBytes(printer.Take())
	return t
}

// ReadTerm reads a term of the grammar sort sort from the file pid up to
// `end` (fun_120, REM's term_read: the query's parser and builder), then
// prints a newline on the standard output.
func ReadTerm(pid int32, sort int32) Term {
	f := file(pid, "read")
	q := queryParser
	if q == nil {
		Unsupported("read_builtinIO: the program has no grammar")
	}
	r := f.reader()
	if r == nil {
		r = bufio.NewReader(strings.NewReader(""))
	}
	tree, err := earley.ParseFile(r, "file input", queryTables(q), sort, os.Stderr)
	if err != nil {
		var fe *earley.Fatal
		if !errors.As(err, &fe) {
			Out.WriteString("term_read error: earleyCall failed\n")
		}
		parseFatal(err)
	}
	t := buildQuery(tree, q)
	Out.WriteString("\n")
	return t
}

// writerFunc is an io.Writer writing with a function.
type writerFunc func([]byte)

func (f writerFunc) Write(b []byte) (int, error) {
	f(b)
	return len(b), nil
}
