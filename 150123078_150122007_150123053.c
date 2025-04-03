#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/*
Yusuf Kurt 150123078
Beyza Parmak 150122007
Doðan Doðan 150123053
*/

#define MAX_TOKENS 50 /* Maximum number of tokens*/
#define MAX_BINARY_LEN 100
#define BUFFER_SIZE 1024

/*Function for converting hexaDecimal to binary */
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
/*Function for converting Binary to Integer*/
unsigned int binaryToInt(char *binary, int length) {
    unsigned int decimal = 0;
    int count = 0;
    while (*binary && count < length) {
        decimal = decimal * 2 + (*binary - '0');
        binary++;
        count++;
    }
    return decimal;
}

/*Function for calculating 2's Complement*/
int twosComplement(char *binary, int size){
	int sign;
    if(binary[0] == '1'){
        sign = -1 * pow(2, 8 * size - 1);
    } else {
        sign = 0;
    }
    int result = sign + binaryToInt(binary + 1, 8 * size - 1);
    return result;
}

/*Rounds even from 13th bit to the last bit*/
void round_even_binary(char *bin) {
    int len = strlen(bin);
    if (len <= 13) {
        printf("%s\n", bin);
        return;
    }
    
    int round_bit = bin[13] - '0'; /* 14th bit*/
    int last_bit = bin[12] - '0';  /* 13th bit*/
    
    /*This is condition checking if the bits after 14th bit includes 1 or not*/
    int condition = 0;
    int x;
    for(x = 14; bin[x] != '\0'; x++){
    	if(bin[x] == '1'){
    		condition = 1;
    		break;
		}
	}
	/*Adding 1 method for given conditions*/
	if((condition == 1 && round_bit == 1) || (condition == 0 && round_bit == 1 && last_bit == 1)){
		int i;
        for (i = 12; i >= 0; i--) {
            if (bin[i] == '0') {
                bin[i] = '1';
                break;
            } else {
                bin[i] = '0';
            }
        	}
		}
	/*Truncate to 13 bits*/
    bin[13] = '\0';
}

double floatingPoint(char* s, int bias, int expsize, int fraction){
    int i=1;
    int exp=-bias;
    /* calculating exponent */
    while(i<=expsize){
        exp+=(s[i]-'0')*pow(2,expsize-i);
        i++;
    }

    double res;
    if(exp==-bias){
        res=0;
        exp++;
    }else if(exp==bias+1){
        int last=expsize+fraction;
        for(;i<last;i++){
            if((s[i]-'0')==1){
                if((s[0]-'0')==0)return NAN;
                return -NAN;
            }
        }
        if((s[0]-'0')==0)return INFINITY;
        return -INFINITY;
    }
    else {
        res=pow(2,exp);
    }

    /*factoring in mantissa and completing floating point number*/
    int fraclast;
    
    if(fraction<=13)
        fraclast=expsize+fraction;
    else fraclast=expsize+13;
    round_even_binary(s+i);

    while(i<=fraclast){
        res+=(s[i]-'0')*pow(2,expsize-i+exp);
        i++;
    }
    /*sign configuration*/
    if((s[0]-'0')==1)return-res;
    return res;
}

int main(int argc, char *argv[]) {
	
	/*Exception handling part*/
    if(argc!=5){
        puts("Error: Argument count must be 4.");
        return 1;
    }
    
    /*Assigning given inputs to the variables*/
    char *endianType = argv[2];
	char *dataType = argv[3];
	int size = (argv[4][0])-'0';
	
	/*Exception handling part*/
    if(strcmp(endianType,"l")!=0 && strcmp(endianType,"b")!=0){
        printf("Error: Invalid endian type. Enter l or b.");
        return 2;
    }
    if(strcmp(dataType,"fp")!=0 && strcmp(dataType,"i")!=0 && strcmp(dataType,"u")!=0){
        printf("Error: Invalid data type. Enter fp, i or u.");
        return 3;
    }
    if(size<1 || size>4){
        printf("Error: Invalid size. Enter 1, 2, 3 or 4.");
        return 4;
    }

	/*Declaring exponent, fraction and bias if the dataType is floating point*/
    int bias, exponent, fraction;
    if(strcmp(dataType,"fp")==0){
        if(size == 1){/*8 bits*/
            exponent = 4;
            fraction = 3;
            bias = 7;
        } else if (size == 2){/*16 bits*/
            exponent = 6;
            fraction = 9;
            bias = 31;
        } else if (size == 3){/*24 bits*/
            exponent = 8;
            fraction = 15;
            bias = 127;
        } else {/*32 bits*/
            exponent = 10;
            fraction = 21;
            bias = 511;
        }
    }
    
    /*File reading*/
    FILE *inputfile=fopen(argv[1],"r");
    if(inputfile==NULL){
        printf("Error: Program could not open specified input file for read.");
        return 5;
    }
    FILE *outputfile=fopen("output.txt","w");
    char *input=calloc(BUFFER_SIZE,sizeof(char));
    char *tokens[MAX_TOKENS]; /* Array to store tokens*/
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
		
		/*Converting hexadecimal to binary and store it into binaries array*/
        char *token = strtok(input, " ");
        while (token != NULL && count < MAX_TOKENS) {
            tokens[count] = token;  /* Store hex token*/
            hexToBinary(token, binaries[count]); /* Convert to binary and store*/
            count++;
            token = strtok(NULL, " ");
        }
        
        /*LITTLE ENDIAN*/
        if(strcmp(endianType,"l")==0){
            for (i = 0; i < count; i += size) {
                char lineBuffer[BUFFER_SIZE] = "";
            
                int end = (i + size < count) ? i + size : count; /* Handle last incomplete subdivision*/
                for (j = end - 1; j >= i; j--) {  /* Read in reverse order*/
                    strcat(lineBuffer, binaries[j]);
                }
                
                /*Send the data to the functions according to dataType*/
                if(strcmp(dataType, "fp") == 0){/*Floating point*/
                    fprintf(outputfile,"%-12g ",floatingPoint(lineBuffer,bias,exponent,fraction));
                } else if (strcmp(dataType, "u") == 0){/*Unsigned*/
                    fprintf(outputfile,"%d ", binaryToInt(lineBuffer, 8 * size));
                } else {/*2's complement integer*/
                    fprintf(outputfile,"%d ", twosComplement(lineBuffer, size));
                }
                
            }
        } else {/*BIG ENDIAN*/
            for (i = 0; i < count; i += size) {
                char lineBuffer[BUFFER_SIZE] = "";
            
                int end = i + size; /* Handle last incomplete subdivision*/
                for (j = i; j < end; j++) {  /* Read in-order*/
                    strcat(lineBuffer, binaries[j]); 
                }
                
                /*Send the data to the functions according to dataType*/
                if(strcmp(dataType, "fp") == 0){/*Floating point*/
                    fprintf(outputfile,"%-12g\n",floatingPoint(lineBuffer,bias,exponent,fraction));
                } else if (strcmp(dataType, "u") == 0){/*Unsigned*/
                    fprintf(outputfile,"%d ", binaryToInt(lineBuffer, 8 * size));
                } else {/*2's complement integer*/
                    fprintf(outputfile,"%d ", twosComplement(lineBuffer, size));
                }
            }
        }
        fprintf(outputfile,"\n");
    }
    fclose(inputfile);
    fclose(outputfile);   
    return 0;
}
