#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TOKENS 50 // Maximum number of tokens
#define MAX_BINARY_LEN 100
#define BUFFER_SIZE 1024

void hexToBinary(const char *hex, char *binary) {
    const char *lookup[] = {
        "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
        "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"
    };

    char temp[5];
    binary[0] = '\0';

    while (*hex) {
        if (*hex >= '0' && *hex <= '9') {
            strcpy(temp, lookup[*hex - '0']);
        } else if (*hex >= 'A' && *hex <= 'F') {
            strcpy(temp, lookup[*hex - 'A' + 10]);
        } else if (*hex >= 'a' && *hex <= 'f') {
            strcpy(temp, lookup[*hex - 'a' + 10]);
        } else {
            printf("Invalid hexadecimal character: %c\n", *hex);
            return;
        }
        strcat(binary, temp);
        hex++;
    }
}

int binaryToInt(char *binary, int length) {
    int decimal = 0;
    int count = 0;
    while (*binary && count < length) {
        decimal = decimal * 2 + (*binary - '0');
        binary++;
        count++;
    }
    return decimal;
}

char* roundToEven (char* str, int len){
	
}

double binaryToDecimal (char* str, char *type, int bias, int exponent, int fraction){
	if(type == "fp"){
		int sign = (str[0] == '1')? -1 : 1;
		int e = bias - binaryToInt(str + 1, exponent);
		if(e != 0){
			int f = ;
		} else {
			
		}
		
	} else if (type == "u"){
		
	} else if (type == "i"){
		
	}
	return;
}

int main() {
    char input[] = "f0 90 01 40 03 00 ff ff 00 00 e0 7f";
    char endianType = 'l';
	char dataType[] = "fp";
	int size = 4;
	
	int bias, exponent, fraction;
	if(dataType == "fp"){
		if(size == 1){
			exponent = 4;
			fraction = 3;
			bias = 7;
		} else if (size == 2){
			exponent = 5;
			fraction = 10;
			bias = 15;
		} else if (size == 3){
			exponent = 7;
			fraction = 16;
			bias = 63;
		} else {
			exponent = 8;
			fraction = 23;
			bias = 127;
		}
	}
	
    char *tokens[MAX_TOKENS]; // Array to store tokens
    char binaries[MAX_TOKENS][MAX_BINARY_LEN];
    int count = 0;

    char *token = strtok(input, " ");
    while (token != NULL && count < MAX_TOKENS) {
        tokens[count] = token;  // Store hex token
        hexToBinary(token, binaries[count]); // Convert to binary and store
        count++;
        token = strtok(NULL, " ");
    }
    
    int i ,j;
    if(endianType == 'l'){
    	for (i = 0; i < count; i += size) {
    		char lineBuffer[BUFFER_SIZE] = "";
    	
        	int end = (i + size < count) ? i + size : count; // Handle last incomplete subdivision
        	for (j = end - 1; j >= i; j--) {  // Read in reverse order
            	strcat(lineBuffer, binaries[j]);
        	}
        	printf("%s\n", lineBuffer); // Newline after each subdivision
        	binaryToDecimal(lineBuffer, dataType, bias, exponent, fraction);
    	}
	} else {
		for (i = 0; i < count; i += size) {
    		char lineBuffer[BUFFER_SIZE] = "";
    	
        	int end = i + size; // Handle last incomplete subdivision
        	for (j = i; j < end; j++) {  // Read in reverse order
            	strcat(lineBuffer, binaries[j]); 
        	}
        	printf("%s\n", lineBuffer); // Newline after each subdivision
    	}
	}
    
    return 0;
}

