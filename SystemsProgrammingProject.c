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

void round_even_binary(char *bin) {
    int len = strlen(bin);
    if (len <= 13) {
        printf("%s\n", bin);
        return;
    }
    
    int round_bit = bin[13] - '0'; // 14th bit
    int last_bit = bin[12] - '0';  // 13th bit
    
    // Truncate at 13 bits
    bin[13] = '\0';
    
    if (round_bit == 1) {
        if (last_bit == 0) {
            bin[12] = '1'; // Round up if last bit was 0
        } else {
            // If last bit is 1, round up only if it results in an even number
            int i;
            for (i = 12; i >= 0; i--) {
                if (bin[i] == '0') {
                    bin[i] = '1';
                    break;
                } else {
                    bin[i] = '0';
                }
            }
            // Ensure the last bit remains even
            if (bin[12] == '1') {
                bin[12] = '0';
            }
        }
    }
    
    printf("%s\n", bin);
}

/*double floatingPoint (char* str, int bias, int exponent, int fraction){
	
	int sign = (str[0] == '1')? -1 : 1;
	int condition = binaryToInt(str + 1, exponent);
	int e;
	if(condition == 0){
		e = 1 - bias;
	} else {
		e = condition - bias;
	}
		
	return 1;
}*/

int main() {
    char input[] = "f0 90 01 40 03 00 ff ff 00 00 e0 7f";
    char endianType = 'l';
	char dataType[] = "fp";
	int size = 4;
	
	int bias, exponent, fraction;
	if(dataType == "fp"){
		if(size == 1){//8 bits
			exponent = 4;
			fraction = 3;
			bias = 7;
		} else if (size == 2){//16 bits
			exponent = 6;
			fraction = 9;
			bias = 31;
		} else if (size == 3){//24 bits
			exponent = 8;
			fraction = 15;
			bias = 127;
		} else {//32 bits
			exponent = 10;
			fraction = 21;
			bias = 511;
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
        	
        	
        	if(strcmp(dataType, "fp") == 0){
        		//Add floating point code here
        		
			} else if (strcmp(dataType, "u") == 0){
				printf("%d ", binaryToInt(lineBuffer, 8 * size));
			} else {
				int sign;
				if(lineBuffer[0] == '1'){
					sign = -1;
				} else {
					sign = 1;
				}
				int result = sign * binaryToInt(lineBuffer + 1, 8 * size - 1);
				printf("%d ", result);
			}
        	
    	}
	} else {
		for (i = 0; i < count; i += size) {
    		char lineBuffer[BUFFER_SIZE] = "";
    	
        	int end = i + size; // Handle last incomplete subdivision
        	for (j = i; j < end; j++) {  // Read in reverse order
            	strcat(lineBuffer, binaries[j]); 
        	}
        	printf("%s\n", lineBuffer); // Newline after each subdivision
        	
        	if(strcmp(dataType, "fp") == 0){
        		//Add floating point code here
        		
			} else if (strcmp(dataType, "u") == 0){
				printf("%d ", binaryToInt(lineBuffer, 8 * size));
			} else {
				int sign;
				if(lineBuffer[0] == '1'){
					sign = -1;
				} else {
					sign = 1;
				}
				int result = sign * binaryToInt(lineBuffer + 1, 8 * size - 1);
				printf("%d ", result);
			}
    	}
	}
    
    return 0;
}

