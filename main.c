//SO IS1 212B LAB10
//Jacek Jacewicz
//jj53675@zut.edu.pl
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
double WYNIK =3;

pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
struct ThreadData {
    int start;
    int ile;
};

double ciag(int n)
{
	double wynikk=0;
	//printf("dostałem %d\n",n%2);
	wynikk=4.0/ ((2.0*n) * ((2.0*n)+1) * ((2.0*n)+2));
	if(n%2==0)
		wynikk*=(-1);
	return wynikk;
	
}
void *thread(void *data) {
	struct ThreadData *tdata = (struct ThreadData*)data; 
	double suma=0;
	pthread_t self = pthread_self();
	srand(self);
	printf("Watek id: %lxd,\t liczba wyrazów: %d \tstart: %d\n", self, tdata->ile, tdata->start);
	for(int i=0;i<tdata->ile;i++)
	{
		//printf("przekazuje %d do funkcji\n",tdata->start+i);
		suma+=ciag(tdata->start +i);
	}
	pthread_mutex_lock(&mymutex);
	WYNIK= WYNIK+suma;
	pthread_mutex_unlock(&mymutex);
	printf("Watek id: %lxd,\t sum=%.20f,\n", self,suma);
	return (void *)1;
}
int main(int argc, char* argv[])
{
	if (argc == 3)
	{
	//sprawdzanie czy argumenty sa liczba i intem
		if(atoi(argv[1]) %1 !=0 || atoi(argv[2])%1 != 0)
		{
			fprintf(stderr, "Argument 1 lub 2 nie jest liczba calkowita(jej reszta z dzielenia przez 1 nie jest rowna 0"); 
		}
		for(int i=0;i<strlen(argv[1]);i++)
		{
			if(!isdigit(argv[1][i]))
			{
				fprintf(stderr,"Argument 1 nie jest liczba\n");
				return 1;
			}
			
		}
		for(int i=0;i<strlen(argv[2]);i++)
		{
			if(!isdigit(argv[2][i]))
			{
				fprintf(stderr,"Argument 2 nie jest liczba\n");
				return 1;
			}
			
		}
		if(atoi(argv[1]) <1 || atoi(argv[1])>1000000)
		{
			fprintf(stderr,"Argument 1 <1 lub >1000000");
			return 1;
		}
		unsigned int min;
		if(atoi(argv[1])<100)
			min=atoi(argv[1]);
		else
			min=100;
		if(atoi(argv[2]) <1 || atoi(argv[2])>1000000)
		{
			fprintf(stderr,"Argument 2 <1 lub >1000000");
			return 1;
		}
		//koniec warunkow argumentu
		int n=atoi(argv[1]), w=atoi(argv[2]);
		struct timespec start, stop;
		//printf("n=%d,w=%d\n",n,w);
		pthread_t threads[w];
		struct ThreadData data[w];
		
		if(n%w==0)
		{
			int ile=n/w;
			for(int i=0;i<w;i++)
			{
				data[i].start=i*ile+1;
				data[i].ile=ile;
			}
		}
		else
		{
			int reszta=n%w;
			int ile=n/w;
			for(int i=0;i<w-1;i++)
			{
				data[i].start=i*ile+1;
				data[i].ile=ile;
			}
			data[w-1].start=(w-1)*ile+1;
			data[w-1].ile=ile+reszta;
			
		}

		clock_gettime(CLOCK_MONOTONIC, &start);
		for(int i = 0; i < w; i++) {
		//printf("ile:%d start:%d\n",data[i].ile,data[i].start);
		pthread_create(threads + i, NULL, thread, &data[i]);
		}
		for(int i = 0; i < w; i++) {
		long retval;
		pthread_join(threads[i], (void *)&retval);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);
		printf("W/threads:\tPi=%.20f\tczas=%.5f\n",WYNIK, (stop.tv_sec-start.tv_sec)+(stop.tv_nsec - start.tv_nsec) /1000000000.0);
		double wynik2=3;
		clock_gettime(CLOCK_MONOTONIC, &start);
		for(int i=1;i<n;i++)
			wynik2= wynik2 + ciag(i);
		clock_gettime(CLOCK_MONOTONIC, &stop);
		printf("Wo/threads:\tPi=%.20f\tczas=%.5f\n",wynik2,(stop.tv_sec-start.tv_sec)+(stop.tv_nsec - start.tv_nsec) /1000000000.0);
		return 0;

	}
	else
	{
		fprintf(stderr,"blad argumentu\n");
		return 1;
	}
	
}
