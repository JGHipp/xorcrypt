/*
	 Xor file encryptor/decryptor
	 written by John Hippisley (unkown date)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char header[5] = "JMNR\x14";
const char check[5] = "CHCK\x14";

// Check if file is encrypted
int checkEnc(FILE* fd)
{
	for(int i = 0; i<5; ++i)
	{
		char in = fgetc(fd);
		if(in != header[i])
		{
			// Not encrypted
			rewind(fd);
			return 0;
		}
	}
	// Is encrypted
	return 1;
}

int main(int argc, char* argv[])
{
	// Handle args
	if(argc < 3)
	{
		printf("Usage: ./xorcrypt [file] [password]\n\e[1mNote:\e[0m xorcrypt doesn't care whether input file is encrypted or\ndecrypted; if its encrypted it will decrypt and vice versa.\n");
		exit(-1);
	}
	char* password = argv[2];
	char* path = argv[1];
	// Open streams
	FILE* fdr = fopen(path, "r");
	FILE* fdw = fopen(".xor_tmp", "w");
	// Generate password hash
	int pwdaverage = 0;
	for(int i = 0; i < strlen(password); ++i) pwdaverage += password[i];
	pwdaverage /= strlen(password);
	pwdaverage += strlen(password); // Prevents 'aa' hashing the same as 'a', for example
	
	// Password check
	int encrypted = checkEnc(fdr);
	if(encrypted)
	{
		for(int i = 0; i < sizeof(check); ++i)
		{
			char in = fgetc(fdr);
			if((in ^ pwdaverage) != check[i])
			{
				fclose(fdr);
				fclose(fdw);
				printf("Incorrect Password\n");
				exit(0);			
			}	
		}
		printf("Password '%s' is correct!\n", password);
	}
	
	printf("%sing '%s'...\n", (encrypted ? "Decrypt" : "Encrypt"), path);

	// Write headers
	if(!encrypted)
	{
		fwrite(header, sizeof(char), sizeof(header), fdw); // Write header to specify that it is now encyrypted
		char encCheck[sizeof(check)];
		for(int i = 0; i < sizeof(check); ++i)	encCheck[i] = check[i] ^ pwdaverage;
		fwrite(encCheck, sizeof(char), sizeof(check), fdw); // Write encrypted check to file
	}
	
	// Write encrypted/decrypted data
	int in, size = 0;
	while((in = fgetc(fdr)) != EOF) // Read the rest of data
	{	 
		char byte=in ^ password[size % strlen(password)];
		fwrite(&byte, sizeof(char), 1, fdw);
		++size;
	}
	
	// Cleanup
	fclose(fdw);
	fclose(fdr);
	remove(path);
	rename(".xor_tmp", path);	
	
	printf("\n%sed '%s'!\n", (encrypted ? "Decrypt" : "Encrypt"), path);
	return 0;
}
