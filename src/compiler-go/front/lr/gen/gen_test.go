package main

import (
	"bytes"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

// The generator on a 3-state excerpt of ppexparser.cc.
func TestExcerpt(t *testing.T) {
	tab, err := parseParserFile("testdata/ppex3.cc", "ppexgram", "PpexGram")
	if err != nil {
		t.Fatal(err)
	}
	got, err := genTables([]*table{tab})
	if err != nil {
		t.Fatal(err)
	}
	want, err := os.ReadFile("testdata/ppex3.golden")
	if err != nil {
		t.Fatal(err)
	}
	if !bytes.Equal(got, want) {
		t.Fatalf("got:\n%s\nwant:\n%s", got, want)
	}
}

// Anything the generator does not recognise is an error naming the file and line.
func TestUnknownConstruct(t *testing.T) {
	src, _ := os.ReadFile("testdata/ppex3.cc")
	bad := strings.Replace(string(src), "{NUMBER,shift,{10}}", "{NUMBER,shift,{10}}, {WHAT,shift,{1}}", 1)
	_, err := parseParser("x.cc", []byte(bad), "ppexgram", "PpexGram")
	if err == nil || !strings.Contains(err.Error(), "x.cc:27:") || !strings.Contains(err.Error(), "WHAT") {
		t.Fatalf("err = %v", err)
	}
	bad = strings.Replace(string(src), "{-'(',shift,{3}}", "{-'(',jump,{3}}", 1)
	if _, err := parseParser("x.cc", []byte(bad), "ppexgram", "PpexGram"); err == nil || !strings.Contains(err.Error(), "x.cc:26:") {
		t.Fatalf("err = %v", err)
	}
	bad = strings.Replace(string(src), "{&appr[2],6,&apak[2],1}", "{&appr[2],7,&apak[2],1}", 1)
	if _, err := parseParser("x.cc", []byte(bad), "ppexgram", "PpexGram"); err == nil {
		t.Fatal("a state beyond the actions accepted")
	}
}

// With ELAN_GENERATED_DIR (the build's src/interpreter/generated), the
// generator's output equals the checked-in files byte for byte.
func TestUpToDate(t *testing.T) {
	dir := os.Getenv("ELAN_GENERATED_DIR")
	if dir == "" {
		t.Skip("ELAN_GENERATED_DIR not set (the build's src/interpreter/generated): up-to-date check skipped")
	}
	files, err := generate(dir)
	if err != nil {
		t.Fatal(err)
	}
	for rel, got := range files {
		want, err := os.ReadFile(filepath.Join("../../..", rel))
		if err != nil {
			t.Fatal(err)
		}
		if !bytes.Equal(got, want) {
			t.Errorf("%s is not up to date: run `make front-tables`", rel)
		}
	}
	if len(files) != 2 {
		t.Fatalf("%d files", len(files))
	}
}
