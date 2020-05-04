/*
 * pow.cpp
 * 
 * Copyright 2019 bruno <brunoartc>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <ctime>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


using namespace std;



int mine (int zeros, string block_data, int total_tentativas, char* result, int init = 0, unsigned long offset_time=0,  unsigned long current_time = (unsigned long) time(NULL), int debug = 0){
    string final_result = "";

    string final_result_hash = "";



    for(int i = init; i < total_tentativas; i++ ) {
        string block_full = ""; 
        block_full.append(to_string(current_time + offset_time));
        block_full.append("|");
        block_full.append(to_string(i));
        block_full.append("|");
        block_full.append(block_data);
        
        char const *data = block_full.data();
        
        
        if (debug) printf("%s", data);
        
        
        
        unsigned char digest[SHA256_DIGEST_LENGTH];
        
    
        SHA256_CTX ctx;
        SHA256_Init(&ctx);
        SHA256_Update(&ctx, data, block_full.size());
        SHA256_Final(digest, &ctx);
        
        
    
        char mdString[SHA256_DIGEST_LENGTH*2+1];
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
            sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
            
            
        string sub9 = mdString;
        
        sub9 = sub9.substr(0,8);
    
        if (debug) printf("SHA256 digest: %s\n", mdString);
        if (debug) printf("SHA256 digest sub: %s\n", sub9.c_str());
        int jj = 0;
        int done = 0;
        //printf("teste %d\n", jj<zeros);

        

        while (jj<zeros){

            const char* sub9array = sub9.c_str();
            if (sub9array[jj] != '0') {
                done = 0;
                break;
            } else {
                done = 1;
            }
            jj++;
            
        }
        if (done) {
            final_result = block_full;
            final_result_hash = mdString;


            

            sprintf(result, "Result block: %s \nHash:         %s\n", final_result.c_str(), final_result_hash.c_str());
            return 1;
        }

    }


    sprintf(result, "No result, try again with diferent parameters\n");
    return 2;
}


 
int main(int argc, char *argv[]) {

    if (argc < 4) {
        printf("Faltando argumentos\npow data<string> max de tentativas<int> dificuldade<int>\n");
        exit(1);
    } // data | max de tentativas | dificuldade
	
	int debug = 0;

	
	string block_data; 

    char result[200];
	
	
	
	
	//getline(cin, block_data);
	
	int offset = 0;

    block_data = string(argv[1]);

	int zeros = (int) atoi(argv[3]);

    int total = (int) atoi(argv[2]);

    if (argc == 6) offset = atoi(argv[4])*3600000;


    //cin >> zeros;



    pid_t forks[15];

    int init_r = total/15;
    for (int a = 0; a < 7; a++)
    {
        forks[a] = fork();

        if (forks[a]==0) {
            int resultado;
            if (argc > 5) {
                resultado =  mine (zeros, block_data, init_r*(a+1), result, init_r*a, offset);
            } else {
                resultado =  mine (zeros, block_data, init_r*(a+1), result, init_r*a);
            }
            if (resultado == 2) return 2;
            printf("Form %d:\n%s\n",  getpid(), result);
            return 1;
        }



    }

    int forks_status;

    int a = 0;

    for (int a = 0; a < 15; a++)
    {

        pid_t terminou = wait(&forks_status);


        if (WEXITSTATUS(forks_status) == 1) return 0;
        if (debug) printf("Code %d in %d\n", WEXITSTATUS(forks_status), terminou);

        
        


    }

    printf("Done");
    


    





	



 
 
    return 0;
}
