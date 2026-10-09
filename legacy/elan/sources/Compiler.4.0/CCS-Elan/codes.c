#include <stdio.h>

int main() {
  char c[100];
  int i;
  printf("Eingabe: ");
  scanf("%s",&c);
  for (i=0; c[i]!=0; i++) 
    printf("%d ",c[i]);
  printf("\n");
  return 0;
}
