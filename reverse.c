/* Cameron Atkins
 * CompSci 4760
 * Project 01
 * Due September 12, 2023, by 11:59 PM
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void help(){
        printf("The options for this program are:\n");
        printf("-h: prints the menu for this program\n");
        printf("-i: The name of the input file\n");
        printf("If no input or output files are specified, default files will be input.dat and output.dat\n");
        printf("-o: The name of the output file\n");
        printf("If no output file is specified, an output file will be created with [inputFile].out naming convention.\n");
}

//Printing function for the child processes
void testPrint(int pid, int *nums, int index, FILE *outputFile){
        //print the childPid
        fprintf(outputFile, "%d: ", pid);

        //print numbers belonging to childPid
        for (int i = index -1; i >=0; i--){
                fprintf(outputFile, "%d ", nums[i]);
        }
        fprintf(outputFile, "\n");
}

int main(int argc, char** argv){
        char *inputFile = "input.dat";
        char *outputFile = "output.dat";
        int option;


        while((option = getopt(argc, argv, "hi:o:")) != -1){
                switch(option){//beginning of switch
                        case 'h':
                        help();
                        break;
                        return 1;

                        case 'i':
                        inputFile = optarg;
                        break;

                        case 'o':
                        outputFile = optarg;
                        break;

                        default:
                        help();
                        break;
                        return 1;
                }//end of switch
        }//end of while

        //Pointers for input and output files
        FILE *input = fopen(inputFile, "r");
        FILE *output = fopen(outputFile, "w");

        /* Get the number of times that will be required to do a task with copies
        of the process using fork */
        char line[100];
        int sets;
        pid_t childPid;
        fscanf(input, "%d", &sets);


        //Go into a loop based on the number of sets

for (int i = 0; i < sets; i++){


                //Get the number of numbers numbers that will follow
                int len_nums;
                fscanf(input, "%d", &len_nums);

                int *values = malloc(len_nums * sizeof(int));

                for(int j = 0; j < len_nums; j++){
                        fscanf(input, "%d", &values[j]);

                }

                childPid = fork();

                if (childPid < 0){
                        perror("reverse");
                        fprintf(stderr, "Error: Failed to fork. ending Program\n");
                        fclose(input);
                        fclose(output);
                        free(values);
                        return 1;
                }

                //Child process
                if(childPid == 0){
                        testPrint(getpid(), values, len_nums, output);
                        free(values);
                        fclose(output);
                        return 0;
                }

        }//end of for loop

        //Parent process
        //Allocate memory for the list of childPids
        int *pids = malloc(sets * sizeof(int));

        for(int i = 0; i < sets; i++){
                pids[i] = wait(NULL);
        }

        //Print childPids
        fprintf(output, "All children were: ");
        for (int i = 0; i < sets; i++){
                fprintf(output, "%d ", pids[i]);
        }
//Print parent pid
        fprintf(output, "\nParent PID: %d\n", getpid());

        //free memory for pids and close pointers for file inputs and outputs
        free(pids);
        fclose(input);
        fclose(output);

        return 0;
}//end of main