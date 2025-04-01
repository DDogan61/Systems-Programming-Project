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
            strcat(binary, temp);
        } else if (*hex >= 'A' && *hex <= 'F') {
            strcpy(temp, lookup[*hex - 'A' + 10]);
            strcat(binary, temp);
        } else if (*hex >= 'a' && *hex <= 'f') {
            strcpy(temp, lookup[*hex - 'a' + 10]);
            strcat(binary, temp);
        } else if(*hex=='\n');
        else{
            printf("Invalid hexadecimal character: %c\n", *hex);
            return;
        }
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
}

double floatingPoint(char* s, int bias, int expsize, int fraction){
    int i,size=(expsize-4)/2+1;
    /*for(i=0;i<size;i++){
        if(i<)
    }*/
    
    i=1;
    int exp=-bias;
    //calculating exponent 
    while(i<=expsize){
        exp+=(s[i]-'0')*pow(2,expsize-i);
        i++;
    }

    double res;
    if(exp==-bias){
        res=0;
        exp++;
    }
    else {
        res=pow(2,exp);
    }

    //factoring in mantissa and completing floating point number
    int fraclast;
    
    if(fraction<=13)
        fraclast=expsize+fraction;
    else fraclast=expsize+13;
    round_even_binary(s+i);

    while(i<=fraclast){
        res+=(s[i]-'0')*pow(2,expsize-i+exp);
        i++;
    }
    //sign configuration
    if((s[0]-'0')==1){
        if(log10(res)>30)res=INFINITY;
        res=-res;
    }
    if(log10(res)>30)res=INFINITY;
    return res;
}

int main(int argc, char *argv[]) {
    /*if(argc!=5){
        printf("Error: Argument count must be 4.");
        return 0;
    }*/
    char endianType = 'l';
	char *dataType = "fp";
	int size = 4;
	
    int bias, exponent, fraction;
    if(strcmp(dataType,"fp")==0){
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
        } else if (size == 4){//32 bits
            exponent = 10;
            fraction = 21;
            bias = 511;
        } else {
            printf("Error: Invalid size");
            return 0;
        }
    }
    FILE *inputfile=fopen("input.txt","r");
    char *input=calloc(BUFFER_SIZE,sizeof(char));
    char *tokens[MAX_TOKENS]; // Array to store tokens
    char binaries[MAX_TOKENS][MAX_BINARY_LEN];
    while (fgets(input, BUFFER_SIZE, inputfile) != NULL){
        int count = 0;
        int i,j;
        for(i=0;i<MAX_TOKENS;i++){
            tokens[i]=0;
            for(j=0;j<MAX_BINARY_LEN;j++){
                binaries[i][j]=0;
            }
        }

        
        char *token = strtok(input, " ");
        while (token != NULL && count < MAX_TOKENS) {
            tokens[count] = token;  // Store hex token
            hexToBinary(token, binaries[count]); // Convert to binary and store
            count++;
            token = strtok(NULL, " ");
        }
        
        if(endianType == 'l'){
            for (i = 0; i < count; i += size) {
                char lineBuffer[BUFFER_SIZE] = "";
            
                int end = (i + size < count) ? i + size : count; // Handle last incomplete subdivision
                for (j = end - 1; j >= i; j--) {  // Read in reverse order
                    strcat(lineBuffer, binaries[j]);
                }
                if(strcmp(dataType, "fp") == 0){
                    //Add floating point code here
                    printf("%g\n",floatingPoint(lineBuffer,bias,exponent,fraction));
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
                    printf("%g\n",floatingPoint(lineBuffer,bias,exponent,fraction));
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
        printf("\n");
    }   
    return 0;
}