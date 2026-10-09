main() {
  int i;
  int *E;

  for(i=0 ; i<100000 ; i++) {
    printf("i = %d\n",i);
    E = (int*) GC_malloc(1*sizeof(int));
  }
}
