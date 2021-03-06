#include "A4_sort_helpers.h"

sem_t* semaphores[27];
char* sem_names[27] = {"first_letter", "second_letter", "third_letter", "fourth_letter", "fifth_letter", "sixth_letter", "seventh_letter", 
			"eighth_letter", "ninth_letter", "tenth_letter", "eleventh_letter", "twelvth_letter", "thirteenth_letter", "fourteenth_letter", "fifteenth_letter", 
			"sixteenth_letter", "seventeenth_letter", "eighteenth_letter", "nineteenth_letter", "twentith_letter", "twentifirst_letter", "twentisecond_letter", 
			"twentithird_letter", "twentifourth_letter", "twentififth_letter", "twentisixth_letter",  "last_semaphore",};

// Function: read_all() 
// Provided to read an entire file, line by line.
// No need to change this one.
void read_all( char *filename ){
    
    FILE *fp = fopen( filename, "r" );
    int curr_line = 0;
	
    while( curr_line < MAX_NUMBER_LINES && 
           fgets( text_array[curr_line], MAX_LINE_LENGTH, fp ) )
    {
        curr_line++;
    }
	
    text_array[curr_line][0] = '\0';
    fclose(fp);
}

// Function: read_all() 
// Provided to read only the lines of a file staring with first_letter.
// No need to change this one.
void read_by_letter( char *filename, char first_letter ){

    FILE *fp = fopen( filename, "r" );
    int curr_line = 0;
    text_array[curr_line][0] = '\0';
	
    while( fgets( text_array[curr_line], MAX_LINE_LENGTH, fp ) ){
        if( text_array[curr_line][0] == first_letter ){
            curr_line++;
        }

        if( curr_line == MAX_NUMBER_LINES ){
            sprintf( buf, "ERROR: Attempted to read too many lines from file.\n" );
            write( 1, buf, strlen(buf) );
            break;
        }
    }
	
    text_array[curr_line][0] = '\0';
    fclose(fp);
}

// YOU COMPLETE THIS ENTIRE FUNCTION FOR Q1.
void sort_words( ){

	int i, j, min_index;
    char min_string[MAX_LINE_LENGTH];

    //Go through unsorted array
    for (i = 0; i < MAX_NUMBER_LINES - 1; i++) {

        // Find the minimum element
        int min_index = i;
        strcpy(min_string, text_array[i]);
        if (strcmp(min_string, "\0") == 0){
        	return;
        }

        for (j = i + 1 ; j < MAX_NUMBER_LINES; j++) {

        	if (strcmp(text_array[j], "\0") == 0){
        		break;
        	}

        	// If min is greater
            if (strcmp(min_string, text_array[j]) > 0) {
                // Make text_array[j] as minimum string and update min_index
                strcpy(min_string, text_array[j]);
                min_index = j;
            }
         
        }
  
        // Swap the found minimum element with the first element
        if (min_index != i) {
            char temp[MAX_LINE_LENGTH];
            strcpy(temp, text_array[i]);
            strcpy(text_array[i], text_array[min_index]);
            strcpy(text_array[min_index], temp);
        }

    }
}

// YOU COMPLETE THIS ENTIRE FUNCTION FOR Q2.
int initialize( ){
    
	for (int i = 0 ; i < 27 ; i++){
		sem_unlink(sem_names[i]);
		if (i == 0)
			semaphores[i] = sem_open(sem_names[i], O_CREAT, 0666, 1);
		else
			semaphores[i] = sem_open(sem_names[i], O_CREAT, 0666, 0);
	}

    // sprintf(buf, "Initializing.\n"  );
    // write(1, buf, strlen(buf));
    
    return 0;
}

// YOU MUST COMPLETE THIS FUNCTION FOR Q2 and Q3.   
int process_by_letter( char* input_filename, char first_letter ){
    // For Q2, keep the following 2 lines in your solution (maybe not at the start).
    // Add lines above or below to ensure the "This process will sort..." lines
    // are printed in the right order (alphabetical).

	sem_wait(semaphores[first_letter - 'a']);

	read_by_letter( input_filename, first_letter );
    sort_words( );

	// sprintf( buf, "This process will sort the letter %c.\n",  first_letter );
	// write(1,buf,strlen(buf));  

    FILE* file = fopen("temp.txt","a");
    for (int i = 0; text_array[i][0] != '\0'; i++){
        fprintf(file, "%s", text_array[i]);
    }
    fclose(file);
	
    sem_post(semaphores[first_letter - 'a' + 1]);
    // For Q3, uncomment the following 2 lines and integrate them with your overall solution.
    

    return 0;
}

// YOU COMPLETE THIS ENTIRE FUNCTION FOR Q2 and Q3.
int finalize( ){
    // For Q2, keep the following 2 lines in your solution (maybe not at the start).
    // Add lines above or below to ensure the "Sorting complete!" line
    // is printed at the very end, after all letter lines.
	sem_wait(semaphores[26]);

	FILE* file = fopen("temp.txt", "r");
    if (file == NULL){
		return -1;
	}

    char string[MAX_LINE_LENGTH];
    //read the file and print it out
    while(!feof(file)){
      strcpy(string, "\0");
      fgets(string, MAX_LINE_LENGTH, file);
      sprintf(buf, "%s",string);
      write(1,buf,strlen(buf));
    }

    fclose(file);
    fopen("temp.txt", "w");
    fclose(file);

    sprintf( buf, "Sorting complete!\n" );
    write( 1, buf, strlen(buf) );

    // For Q3, come up with a way to accumulate the sorted results from each
    // letter process and print the overal sorted values to standard out.
    // You are not allowed to read from the input file, or call sort_words
    // directly from this function.

    return 0;
}

