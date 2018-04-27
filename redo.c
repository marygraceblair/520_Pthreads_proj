#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREADS 2
#define ARRAY_SIZE 8000000
char **line;
char * str[10000];
char  ret[4000];
char * results[10000];
pthread_mutex_t str_mutex;
pthread_mutex_t results_mutex;
int ReadFile (char *c)
{
//	printf("PRINT STATEMENT FOR DEBUGGING IN READ FILE");
	//variables for file reading
	FILE *fd;
	int i = 0;
    int len;
    int j; 
	int err, nlines, nchars = 0;

	long maxlines = 38;
	line = (char **) malloc( maxlines * sizeof( char * ) );
	for( i = 0; i < maxlines; i++ ) {
		line[i] = malloc( 2001 );
	}
// Read in the lines from the data file

	fd = fopen( c, "r" );
	nlines = -1;
	do {
	  err = fscanf( fd, "%[^\n]\n", line[++nlines] );
	  if( line[nlines] != NULL ) nchars += (double) strlen( line[nlines] );
	} while( err != EOF && nlines < maxlines);
	fclose( fd );

   printf( "Read in %d lines averaging %.0lf chars/line\n", nlines, nchars / nlines);
}

void PrintResults()
{
    printf("\n");
    printf("\n");
    printf("\n");
    printf("printing results now");
    int i;
    for (i= 0; i<36; i++)
    {
       printf("%d-%d: %s\n", i, i+1, results[i]); 
    }
}

char* lcs(char* s1, char* s2)
{
	int s1_l = strlen(s1), s2_l = strlen(s2);
	int i, s1_i, s2_i, longest = 0;

	char* set = (char*)malloc(sizeof(char) * ((s1_l < s2_l)?s1_l:s2_l));
	memset(set, ' ', strlen(set));

	// initialize 2d array
	int** l = (int**)malloc((s1_l + 1) * sizeof(int*));
	for (i = 0; i < s1_l + 1; i++)
		l[i] = (int*)malloc((s2_l + 1) * sizeof(int));

	// initialize all array fields to 0
	for (s1_i = 0; s1_i < s1_l; s1_i++)
		for (s2_i = 0; s2_i < s2_l; s2_i++)
			l[s1_i][s2_i] = 0;

	// loop through all letters
	for (s1_i = 0; s1_i < s1_l; s1_i++)
		for (s2_i = 0; s2_i < s2_l; s2_i++)
		{
			// if characters match
			if (s1[s1_i] == s2[s2_i])
			{
				// calculate the incremented length for current sequence
				int v = l[s1_i][s2_i] + 1;

				// save the incremented length in south-east cell
				l[s1_i+1][s2_i+1] = v;

				// if new length is longest thus far
				if (v > longest)
				{
					// set that to be our new longest length
					longest = v;

					// and clear the set
					memset(set, ' ', strlen(set));
				}	

				// if new length equals to the longest length
				if (v == longest)
				{
					// copy the char set to the return set
					for (i = 0; i < longest; i++)
						set[i] = s1[s1_i-longest+i+1];
				}
			}
		}

	// free
	for (i = 0; i < s1_l + 1; i++)
		free(l[i]);
	free(l);

	// return
	return set;
}

void *runLCS(void *mySection)
{
    int i,j,k;
    int startPos = ((int) mySection) * (ARRAY_SIZE / NUM_THREADS);
    int endPos = startPos + (ARRAY_SIZE / NUM_THREADS);
    
    int numTimes = endPos - startPos; 
    //char stringA[numTimes][4000]; 
    //char stringB[numTimes][4000]; 

    char **stringA = malloc(numTimes * sizeof(char *)); // Allocate row pointers
    for(i = 0; i < numTimes; i++)
          stringA[i] = malloc(4000 * sizeof(char));  // Allocate each row separately

    char **stringB = malloc(numTimes * sizeof(char *)); // Allocate row pointers
    for(i = 0; i < numTimes; i++)
          stringB[i] = malloc(4000 * sizeof(char));  // Allocate each row separately

    
    pthread_mutex_lock(&str_mutex);
    for (j = startPos; j < endPos; j++)
    {
        stringA[j]= line[j];
        stringB[j]= line[j+1];
    }
    pthread_mutex_unlock(&str_mutex);

    pthread_mutex_lock(&results_mutex);
    for (j = startPos; j < endPos; j++)
    {
        char *answer = lcs(stringA[j],stringB[j]); 
        strcpy( results[j], answer); 	
    } 
    pthread_mutex_unlock(&results_mutex);
    pthread_exit(NULL);
}

int main()
{
    void *status;
    char * c = "one_word_line.txt";
    printf("before print");
    int success = ReadFile(c);
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_mutex_init(&str_mutex, NULL);
    pthread_mutex_init(&results_mutex, NULL);
    if (success >= 0 )
    {
        int i, rc;
        //initializing the results
        for(i = 0; i<38; i++ )
        {
            results[i] = malloc(sizeof(char) * 4000);
        }

        for (i = 0; i < NUM_THREADS; i++ ) 
        {
          rc = pthread_create(&threads[i], &attr, runLCS, (void *)i);
          if (rc) 
          {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
          }
        }
/*
        int j,k;
        for (j = 0; j < 38; j++)
        {
                char *A = line[j];
                char *B = line[j+1];
                char *answer = lcs(A,B); 
                strcpy( results[j], answer); 	
        }     
        */
        
        pthread_attr_destroy(&attr);
        for(i=0; i<NUM_THREADS; i++) {
             rc = pthread_join(threads[i], &status);
             if (rc) {
                   printf("ERROR; return code from pthread_join() is %d\n", rc);
                   exit(-1);
             }
        }
        pthread_mutex_destroy(&str_mutex);
        pthread_mutex_destroy(&results_mutex);
        PrintResults();
    }
    else
    {
        printf("File failed to load");
        return 1; //failed
    }
}

