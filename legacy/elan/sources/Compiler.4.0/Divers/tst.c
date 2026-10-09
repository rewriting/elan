#include<stdio.h>
main()
{
	char c;
	int n;
	int test_evalexp17();
	int test_evaltree17();
	for(;;)
	{
		system("clear");
		printf(" \n------------< Menu >-----------");
		printf(" \n 1 ====> teste avec bench-evalexp17");
		printf(" \n 2 ====> teste avec bench-evaltree17");
		printf("\n\n");
		printf("\n------------------------------");
		printf("\nTapez votre choix :  ");
		scanf("%c",&c);
		printf("\n\n");
		printf("\n------------------------------\n\n");
		switch(c)
		{
			case '1' : test_evalexp17();break;
			case '2' : test_evaltree17();break;
			default  : exit();
		}
		printf("\ntapez une touche pour continuez =>");
		scanf("%c%c",&c,&c);	
	}
}



int test_evalexp17()
{
	char c;
	char commande[100];
	char file[50];
	int i,n;
	FILE *fp;
	for(;;)
	{
		system("clear");
		printf("\n------------------------------\n");
		printf("\ntest de evalexp17 ");
		printf("\n------------------------------\n");
		printf("\nTapez le nombre ou 0 pour sortir  :  ");
		scanf("%d",&n);
		if(!n) return(1);
		strcpy(commande,"a.out");
		sprintf(commande+5,"%c",' ');
	sprintf(commande+6,"%s%d%s%d%s","<req",n,"exp>res_exp",n,".exp");
		sprintf(file,"%s%d%s","req",n,"exp");  	
		printf("%s",file);  	
		fp=fopen(file,"w"); 	
		fprintf(fp,"%s","bench-evalexp17("); 
		for( i=0;i<n;i++)
		{
		  	fprintf(fp,"%s","exs("); 	
		}
		fprintf(fp,"%s","exz)");
	 	for( i=0;i<n;i++)
		{
			fprintf(fp,"%s",")");
		}
		printf("\n\n%s\n\n",commande);
		fclose(fp);
		system(commande);
		printf("\ntapez une touche pour continuez =>");
		scanf("%c%c",&c,&c);	
	}
}

int test_evaltree17()
{
	char c;
	char commande[100];
	char file[50];
	int i,n;
	FILE *fp;
	for(;;)
	{
		system("clear");
		printf("\n------------------------------\n");
		printf("\ntest de evaltree17 ");
		printf("\n------------------------------\n");
		printf("\nTapez le nombre  ou 0 pour sortir :  ");
		scanf("%d",&n);
		if(!n) return(1);
		sprintf(file,"%s%d%s","req",n,"exp");
		printf("%s",file);
		fp=fopen(file,"w");
		strcpy(commande,"a.out");
		sprintf(commande+5,"%c",' ');
	sprintf(commande+6,"%s%d%s%d%s","< req",n,"exp > res_tree",n,".exp");
		fprintf(fp,"%s","bench-evaltree17(");
		for( i=0;i<n;i++)
		{
			fprintf(fp,"%s","exs(");
		}
		fprintf(fp,"%s","exz)");
		for( i=0;i<n;i++)
		{
			fprintf(fp,"%s",")");
		}
		printf("\n\n%s\n\n",commande);
		fclose(fp);
		system(commande);
		printf("\ntapez une touche pour continuez =>");
		scanf("%c%c",&c,&c);	
	}
}

