#define UNUSED   0
#define STATROPEN 1
#define STATWOPEN 2
#define STATAOPEN 3
#define STATROPENEND  4       // read file, end should be read

#ifndef FOPEN_MAX
#define FOPEN_MAX 30
#endif
struct {
    FILE *f;
    lstream *ls;
    ochstream *och;
    int  status; } FILES[FOPEN_MAX];

void init_files()
{
int i;
  for(i = 3; i < FOPEN_MAX; i++) FILES[i].status = UNUSED;
}

void close_pid(int pid)
{
  if (FILES[pid].status == UNUSED)
    //return 0;
    exit(0);
  if (FILES[pid].status == STATROPEN) {
    DELETE1(FILES[pid].och); }
   else {
    DELETE1(FILES[pid].ls); }
   FILES[pid].status = UNUSED;
}

void close_files()
{
int i;
  for(i = 3; i < FOPEN_MAX; i++) close_pid(i);
}

int find_fid()
{
int i;
  for(i = 3; i < FOPEN_MAX; i++)
    if (FILES[i].status == UNUSED) return i;
  return 0;
}

