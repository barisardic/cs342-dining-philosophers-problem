  #include <pthread.h>
  #include <stdio.h>
  #include <unistd.h>
  #include <stdlib.h>
  #include <errno.h>
  #include <assert.h>
  #include <string.h>
  #include <math.h>
  #include <time.h>
  #include <sys/timeb.h>
// header baris ardic 21401578
	void *runner (void *id);

	int PHILOS ;
	int minThink;
	int maxThink;
	int minEat;
	int maxEat;


	char* distribution;
    int count ;
    pthread_mutex_t *catal ; //wanted to name it fork, doesnt let me since fork() reserved fork
    pthread_mutex_t riceLock;
	int main (int argn,char **argv)
	{   // input parameters
	    PHILOS = atoi(argv[1]);
	    minThink = atoi(argv[2]);
	    maxThink = atoi(argv[3]);
	    minEat = atoi(argv[4]);
	    maxEat = atoi(argv[5]);
	    distribution = argv[6];
	    count = atoi(argv[7]);

	    pthread_mutex_t chopstick1[PHILOS];
        pthread_t philo[PHILOS];
        catal=chopstick1;

        int seed=time(NULL);
        srand(seed);

// initilise and create and join threads
	    int i;
	    pthread_mutex_init (&riceLock, NULL);
	    for (i = 0; i < PHILOS; i++)
	        pthread_mutex_init (&chopstick1[i], NULL);
	    for (i = 0; i < PHILOS; i++)
	        pthread_create (&philo[i], NULL, runner, (void *)i);
	    for (i = 0; i < PHILOS; i++)
	        pthread_join (philo[i], NULL);
	    return 0;
	}
// threads start here
	void *runner (void *num)
	{
	    int id;
	    int leftFork, rightFork;
	    id = (int)num;

	    rightFork = id%PHILOS;
	    leftFork = (id + 1)%PHILOS;
	    if(PHILOS==1){
            rightFork=leftFork;
	    }

        int totalHungry=0;
        struct timeb start, end;
        int noOfMeals=count;
	    while (noOfMeals-->0) {

	        if (id == 1)// prevents the deadlock where all philosophers take the right chopstick
	            usleep (1000);
            printf ("Philosopher %d hungry\n", id+1);//---------------prints the hungry state.
            ftime(&start);
	        pthread_mutex_lock (catal+rightFork);
	        pthread_mutex_lock (catal+leftFork);


            ftime(&end);
            int diff = (int) (1000.0 * (end.time - start.time) + (end.millitm - start.millitm));// time second, millitm ms
            //printf("---------------------%d\n",diff);
            //int hungryTimes[count];// for calculating stddev
            //hungryTimes[noOfMeals-1]= diff;
            totalHungry+=diff;

	       int durEat;
           if(strcmp(distribution,"exponential")==0){
                durEat= 0 ;
                srand(time(NULL)*(id+1));
                durEat = (-((minEat+maxEat)/2)*log((float)rand()/(RAND_MAX)));
                while(durEat<minEat || durEat>maxEat){
                    durEat = (-((minEat+maxEat)/2)*log((float)rand()/(RAND_MAX)));
                }
            }
            else{// for uniform distribution
                durEat= 0 ;
                srand(time(NULL)*(id+1));
                durEat = maxEat + rand()/(RAND_MAX/(minEat-maxEat+1)+1);
            }

	        printf ("Philosopher %d eating\n", id+1);
	        usleep (durEat*1000);// *1000 to convert to ms

            pthread_mutex_unlock ((catal+leftFork));
            pthread_mutex_unlock ((catal+rightFork));
	        int durThink;
            if(strcmp(distribution,"exponential")==0){
                durThink= 0 ;
                srand(time(NULL)*(id+1));
                durThink = (-((minThink+maxThink)/2)*log((float)rand()/(RAND_MAX)));
                while(durThink<minThink || durThink>maxThink){
                    durThink = (-((minThink+maxThink)/2)*log((float)rand()/(RAND_MAX)));
                }
            }
            else{// for uniform distribution
                durThink= 0 ;
                srand(time(NULL)*(id+1));
                durThink = maxThink + rand()/(RAND_MAX/(minThink-maxThink+1)+1);
            }
            //printf ("Philosopher %d thinking %d\n", id,durThink);
            printf ("Philosopher %d thinking \n", id+1);
            usleep (durThink*1000);
            // commented out , calculates std dev
            //if(noOfMeals==1){
            //int avg = totalHungry/count;
            //float stddev =0.0;
            //for(int i = 0 ; i<count;i++){
            //stddev+= ((hungryTimes[i]-avg)*(hungryTimes[i]-avg));
            //}
            //stddev= (int)sqrt(stddev/count);
            //printf("Std dev of philosopher %d is %f--average hungry duration %d\n",id+1,stddev,avg);
            //}

	    }// end of while

	    printf ("Philosopher %d duration of the hungry state =%d\n", id+1,totalHungry);
	    return (NULL);
	}

