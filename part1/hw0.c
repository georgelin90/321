#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
double pi_estimate;
long long number_of_tosses;
int number_of_cores,total_number_in_circle;
long int number_in_circle[64];
pthread_t t[64];
void* cal_pi(void *input){
	int tid = (int) input;
	pthread_mutex_t mutex;
	double x,y,distance_squared;
	time_t t;
	double sum=0;
	unsigned int seed = time(NULL);
	for (long long toss = 0; toss < number_of_tosses/ number_of_cores; toss ++) {
	    x = (double) rand_r(&seed)/(RAND_MAX+1.0);
	    y = (double) rand_r(&seed)/(RAND_MAX+1.0);
	    distance_squared = x * x + y * y;
	    if ( distance_squared <= 1)
	       // number_in_circle[tid]++;
	       sum++;
	}
	pthread_mutex_lock(&mutex);
	total_number_in_circle+=sum;
	pthread_mutex_unlock(&mutex);
}
int main(int argc, char* argv[]){
	if(argc<3) fprintf(stderr, "Too few arguments.\n");
	struct timeval start, end;
	gettimeofday(&start,NULL);
	number_of_cores = atoi(argv[1]);
	number_of_tosses = atoll(argv[2]);
	for(int i=0; i<64; i++){
		number_in_circle[i]=0;
	}
	for(int i=0; i<number_of_cores; i++)
		pthread_create(&t[i], NULL, cal_pi, (void*) i);
	for(int i=0; i<number_of_cores; i++)
		pthread_join(t[i], NULL);
//	for(int i=0; i<number_of_cores; i++)
//		total_number_in_circle+= number_in_circle[i];
	pi_estimate = (double)4 * total_number_in_circle /( number_of_tosses);
	printf("%.10f\n",pi_estimate);
	gettimeofday(&end, NULL);
	unsigned long diff=1000000*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
        double ddiff=(double)diff/1000000;
        printf("Time elasped: %f sec\n",ddiff);
}
