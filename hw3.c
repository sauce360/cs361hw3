#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include<ncurses.h>
#include <sys/stat.h>
#include <fcntl.h>
#define NAME_MAX 100
int and=0;
void split(char* input, char *argv[]){
    char* p;                   
    int argc;                   
    argc=0;                                 
    p=strtok(input, " ");        

    while(p!=NULL){         
        argv[argc]=p;           
        argc++;
        p=strtok(NULL, " ");    
    }   
    argv[argc]='\0';   
}

char* readLine(char* line, size_t len){ 
    getline(&line, &len, stdin);            
    
	if(*(line+strlen(line)-2)=='&')
	{and=1; *(line+strlen(line)-2)='\0';}

	else{*(line+strlen(line)-1)='\0';}


    return line;
}

void split_p(char* input, char *argv[]){
    char* p;                   
    int argc;                   
    argc=0;                                 
    p=strtok(input, "|");        

    while(p!=NULL){         
        argv[argc]=p;           
        argc++;
        p=strtok(NULL, "|");    
    }   
    argv[argc]='\0';   
}

int main()
{
	char *input;
	char *temp_input;
	char *temp_input1;
	char *argv[50];
	char *args[50];
	char *args1[50];
	int file_pipes[2];	
	char buf2[NAME_MAX];		
			char buf1[NAME_MAX];
			getcwd(buf1, NAME_MAX);
    			printf("@abeer:~%s$ ", buf1);

int check_pipe;
int x,xx;
x=0;
xx=0;
check_pipe=0;
while(1)
{
	char buf[NAME_MAX];
	x=0;	
	xx=0;
	input=readLine(input, 0);
	if(input[0]=='t'){break;}
	while(input[check_pipe]!='\0'){
		if(input[check_pipe]=='|')
		{x=1;break;}
		if(input[check_pipe]=='>')
		{xx=1;break;}
		check_pipe++;
		}

	if(x==1)
	{
	split_p(input,argv);
	temp_input=argv[0];
	temp_input1=argv[1];

	split(temp_input,args);
	split(temp_input1,args1);
	}
	else {split(input,argv);
	//ex=argv[0];
        //check_re=argv[1];
	}
  
        pid_t fork_result;
	
	
if (pipe(file_pipes) == 0) 
		{
			
		fork_result = fork();
			check_pipe=0;
			if(fork_result==0)
				{
					if (strcmp(argv[0], "cd") == 0) //change directory
						{
							close(file_pipes[0]);
							argv[0]="#";
							write(file_pipes[1],argv[0],strlen(argv[0]));
							write(file_pipes[1],argv[1],strlen(argv[1]));
							exit(0);
						}
					else{ //execute remaining comands
							if(x==1)//detects the pipe
							{
								write(file_pipes[1],"!",strlen(argv[0]));
								int y=dup(1);								
								close(file_pipes[0]);
								dup2(file_pipes[1], 1);
								execvp(args[0],args);
								close(file_pipes[1]);
								dup2(y,1);
							}
							
					else if(xx==1)
					{
						int f = open(argv[2],O_CREAT|O_RDWR,0666);
						int x=dup(1);
						close(file_pipes[0]);
						dup2(f, 1);
						execvp(argv[0],argv);
						printf("%d\n",close(f));
						close(1);
						dup2(x,1);
						xx=0;
						exit(0);
					}

							else{
							
								execvp(argv[0],argv);
								exit(0);
						             }	
					    }
				}
				

				else if(fork_result>0)
				{
					int returnStatus;
					memset(buf, '\0', sizeof(buf));  
					if(and!=1){              					
					wait(&returnStatus);}
					else{and=0;}	 					
					close(file_pipes[1]); 	 						
					read(file_pipes[0], buf,1);
					if(buf[0]=='#')//chdir
					{
					read(file_pipes[0], buf,100);
					int j;
					j=0;	
		while(((char)buf[j]>=46&&(char)buf[j]<=57)||((char)buf[j]>=65&&(char)buf[j]<=90)||((char)buf[j]>=97&&(char)buf[j]<=122))
                                         {j++;}
					buf[j]='\0';
					chdir(buf);
					}
		if(buf[0]=='!'){ //pipe
		           if(fork()==0){
				 int x=dup(STDIN_FILENO);
   				 close(file_pipes[1]);
				 dup2( file_pipes[0], STDIN_FILENO );
                                 //char* prog2[] = {"wc", 0};
				 execvp(args1[0], args1);
				 close(file_pipes[0]);
				 dup2(x,STDIN_FILENO);
				 check_pipe=1;exit(0);}	

				}}
			xx=0;
			getcwd(buf2, NAME_MAX);
    			printf("@user:~%s$ ", buf2);
					
}		
}

return 0;
}
