#include <stdio.h>
#include <string.h>

#define NUM_THREADS 2

//pthread_mutex_t str_mutex;
//pthread_mutex_t results_mutex;
char **line;

/*
char * longest_common_substring(char * S, char * T)
{
    //make a 2d array 
    int r = strlen(S); 
    printf("size r %d", r); 

    int n = strlen(T);
    printf("size n %d", n); 
    //printf("%s", S);
    int L[r][n];

    int z = 0; 

    int i; 
    int j; 
    // give citation for the code
    //char* set = (char*)malloc(sizeof(char) * ((r < n)?r:n));
//    char set[4000][4000];
    char result[4000];
   memset( result, '\0', sizeof(result) );
    memset(L, 0, sizeof(L[0][0]) * r * n);
 //   memset(L, 0, sizeof(set[0][0]) * sizeof(char) *4000 * 4000);
    for (i = 0; i < r; i++) {
        for (j = 0; j < n; j++) {
            printf("%d ", L[i][j]);
        }
                printf("\n");
                    
    }

    


    for (i = 0; i < r; i++)
    {
       for (j = 0; j < n; j++)
       {
           if (S[i] == T[j])
           {
               if ( i == 0  ||  j == 0 )
               {
                   L[i][j] = 1; 
               }
               else
               {
                   L[i][j] = L[i -1][j - 1] + 1;
               }

               if (L[i][j] > z)
               {
                   z = L[i][j];
                   //find the length of the substring from i -z + 1 to i 
                   int math = i - z + 1; 
                   int math_2 = i - math; 
                   
                   printf("%s", S);
                   printf("i= %d", i); 
                   printf("z = %d", z); 
                   printf("math = %d", math); 
                   printf("math_2 = %d", math_2); 
                   
                   char temp[4000];
                   memset( temp, '\0', sizeof(temp) );
                   //printf("\n and clearing");
                   //strcat(result, " "); 
                   strncpy(temp, &S[math], math_2);
                   //strcat(result, temp); 
                   
                   printf("word added" );
                   printf("%s", temp);
                   //strcpy(result, temp); 
                   memset( result, '\0', sizeof(result) );
                     strncpy(result,  &S[math], math_2);
                   printf("L[i][j] > zchanged ");
                   printf("result = %s", result);
                  
               }
               else
               {
                   if (L[i][j] == z)
                   {
                       char temp[4000]; 
                       memset( temp, '\0', sizeof(temp) );
                       
                       printf("\n"); 
                       int math = i - z + 1; 
                       int math_2 = i - math; 
                       
                       //printf("adding");
                       //printf("%s", S);
                       //printf("%s", &S[math]);
                       //printf("%s", &S[math_2]);
                       //printf("i= %d", i); 
                       //printf("z = %d", z); 
                       //printf("math = %d", math); 
                       //printf("math_2 = %d", math_2); 
                       
                       //printf("\n");
                       //printf("result before cat %s", result);
                       //strcpy(temp, result); 
                       //strcat(temp, " "); 
                       //strcpy(result, temp);
                    
                       memset( temp, '\0', sizeof(temp) );
                       strncpy(temp, &S[math], math_2);
                       //strcat(result, temp); 
                       printf("word added: ");
                       printf("%s", temp);

                       //printf("together:::::");
                       //printf("%s", result);
                   }
               }
            }
           else
           {
            L[i][j] = 0; 
           }

        }
    }
    printf("finished");
    printf("answer: %s", result); 
    printf("\n"); 
 //   printf("\n");
    printf("%s", S);
    printf("\n");
    for (i = 0; i < r; i++) {
        printf("%c", T[i]); 
        for (j = 0; j < n; j++) {
            printf("%d ", L[i][j]);
        }
                printf("\n");
                    
    }
    return result; 
}

*/

//global variables
char * str[10000];
char  ret[4000];
char * results[10000];

//this method is found https://gist.github.com/adrian-source/4111719 .... THIS CODE IS NOT MINE
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
//have one thread run 10 lines lines 1 - 10 
//either thread run 10 lines  11 - 20
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

	fd = fopen( "/homes/dan/625/wiki_dump.txt", "r" );
	nlines = -1;
	do {
	  err = fscanf( fd, "%[^\n]\n", line[++nlines] );
	  if( line[nlines] != NULL ) nchars += (double) strlen( line[nlines] );
	} while( err != EOF && nlines < maxlines);
	fclose( fd );

   printf( "Read in %d lines averaging %.0lf chars/line\n", nlines, nchars / nlines);


	//algorithm for file reading starts here
/*	if ((fp = fopen(c, "r")) != NULL)
	{
		while (fgets(line, 4000, fp) != NULL) 
		{
            len = strlen(line);
            while (len && isspace(line[len - 1]))
			{
				len--;
			}
			line[len] = '\0';
        //  printf("the current line is %s\n", line); 

          // *str[i] = (char* )malloc(sizeof(char) *  4000);
          strcpy( str[i], line); 	
           
            i++;
        }
        return 0; //success 
    }*/
    //return -1; //failure
}
// this method prints the results from the results 
void PrintResults()
{
    printf("\n");
    printf("\n");
    printf("\n");
    printf("printing results now");
    int i;
    for (i= 0; i<10000; i++)
    {
       printf("%d-%d: %s\n", i, i+1, results[i]); 
    }
}


void *runLCS(void *mySection)
{
    int j,k;
    //j will be from 1 - 9 
    //k will be from 0 - 8
    for (k = 0; k< 9; k++)
    {
        char *A = str[k];
        char *B = str[k+1];
        char * answer = lcs(A,B); 
        printf("\n"); 
        printf("here are the words:: %s , %s", A, B);
        strcpy( results[k], answer); 	
        printf("here is the answer %s", results[k]);
        printf("\n");

    }
}

int main()
{
    /*
    char * test1= "ABAB"; 
    char * test2 = "BABA";

    printf("string1 %s", test1); 
    printf("string2 %s", test2);
    
    printf("\n");
    //char * ret = longest_common_substring(test1, test2);
    char * ret = lcs(test1,test2);
    char * cat = "cat";
    printf("%s", ret);
   // printf("%s", cat);

    printf("\n");
    */
//    pthread_t threads[NUM_THREADS];
//    pthread_attr_t attr;

//    pthread_attr_init(&attr);
//    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

//    pthread_mutex_init(&str_mutex, NULL);
    //pthread_mutex_init(&results_mutex, NULL);
    char * c = "one_word_line.txt";
    // /homes/dan/625/wiki_dump.txt   

    printf("before print");
    int success = ReadFile(c);

    if (success >= 0 )
    {
    int i;
    //initializing the results
    for(i = 0; i<38; i++ )
    {
        results[i] = malloc(sizeof(char) * 4000);
    }
    int j,k;
    for (j = 0; j < 38; j++)
    {
            char *A = line[j];
            char *B = line[j+1];
            char *answer = lcs(A,B); 
            strcpy( results[j], answer); 	
    }     
    PrintResults();
        
        /*
        for (i = 0; i < NUM_THREADS; i++ ) 
        {
          rc = pthread_create(&threads[i], &attr, runLCS, (void *)i);
          if (rc) 
          {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
          }
                  
        }
        */
    }
    else
    {
        printf("File failed to load");
        return 1; //failed
    }

    return 0; // succes
}


