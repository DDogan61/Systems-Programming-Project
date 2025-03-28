#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

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
// before rounding the partial is always one
    bool areAllOne = true;
//fraction rounding method for 32 bits
char* roundToEven (char* str){
    //after rounding the partial is always zero
    bool areAllZeros = true;
    if(str[24]==1){
        for(int i = 24; i<32;i++ ){
            if(str[i]==1){
                areAllZeros=false;}
        for(int i = 10; i<24;i++ ){
            if(str[i]==0){
                areAllOne=false;}}      

        //If the first bit to be cut = 1 and there is a 1 somewhere next
        if(!areAllZeros){
            if(){

            }
            for(int j=23; j>0;j--){

            }
        
        }else{
            
        }   

    }

    }
    
}

double binaryToDecimal (char* str, char *type,int exponent_bit, int fraction_bit){
    if(strcmp(type, "fp") == 0){
        int sign = (str[0] == '1')? -1 : 1;
        int bias = pow(2, exponent_bit-1) -1;
        int exponent = binaryToInt(str + 1, exponent_bit);
        int e = exponent -bias;
        double result; 
        
        //calculating fraction value
        double fraction_value = 0.0;
        size_t total_size = sizeof(str);
        size_t elemenet_size = sizeof(str);
        size_t number_of_element = total_size/elemenet_size;
        if(number_of_element == 32){
            str = roundToEven(str);
            for(int i = exponent_bit+1; i<24; i++){
            if(str[i] ==1){
                fraction_value = fraction_value + (2,-(i-exponent_bit));
            } 

        }}else{
            for(int i = exponent_bit+1; str[i] != '\0'; i++){
            if(str[i] ==1){
                fraction_value = fraction_value + (2,-(i-exponent_bit));
            } 
        }
        //if all exponent digits are zero, adding 0 to fraction_value otherwise 1 is added.
        if(exponent !=1){
            fraction_value += 1; 
        }
        }
        areAllOne = true;
        
    } else if (strcmp(type, "u") == 0){
        
    } else if (strcmp(type, "i") == 0){
        
    }
    return;
}

int main() {
    char input[] = "f0 90 01 40 03 00 ff ff 00 00 e0 7f";
    char endianType = 'l';
    char dataType[] = "fp";
    int size = 4;
    
    int bias, exponent, fraction;
    if(strcmp(dataType, "fp") == 0){
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
            //binaryToDecimal(lineBuffer, dataType, bias, exponent, fraction);
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


