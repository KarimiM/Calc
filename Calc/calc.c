//
//  calc.c
//  Calc
//
//  Created by Masood Karimi on 7/11/18.
//  Copyright © 2018 Masood Karimi. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

int getSize(char *array)
{
    int size = 0;
    for (int i = 0; array[i] != '\0'; i++) {
        size++;
    }
    return size;
}
int convertBinaryToDecimal(char *number)
{
    int i;
    int decimalValue = 0;
    int base = 1;
    int size = getSize(number);
    int negative = (size == 32) && (number[0] == '1' ? 1 : 0);
    for (i = size - 1; i >= 0; i--)
    {
        if (number[i] != '0' && number[i] != '1') {
            continue;
        }
        if (i == 0 && negative == 1) {
            break;
        }
        if ((negative == 0 && number[i] == '1') || (negative == 1 && number[i] == '0')) {
            decimalValue += base;
        }
        base *= 2;
    }
    if (negative == 1) {
        decimalValue += 1;
        decimalValue *= -1;
    }
    
    return decimalValue;
}

char *convertDecimalToBinary(int number)
{
    char *bits;
    bits = malloc(33 * sizeof(char));
    int i;
    for (i = 31; i >= 0; i--)
    {
        int value = number & 1;
        bits[i] = value == 1 ? '1' : '0';
        number = number >> 1;
    }
    bits[32] = '\0';
    return bits;
}

int convertHexToDecimal(char *hex)
{
    int value = 0;
    int base = 1;
    int size = getSize(hex);
    for (int i = size - 1; i >= 0; i--) {
        char c = hex[i];
        if (c >= '0' && c <= '9') {
            value += (c - 48) * base;
            base *= 16;
        } else if (c >= 'A' && c <= 'E') {
            value += (c - 55) * base;
            base *= 16;
        } else if (c >= 'a' && c <= 'e') {
            value += (c - 87) * base;
            base *= 16;
        }
    }
    return value;
}

char *convertDecimalToHex(int number)
{
    char *hex;
    hex = malloc(9 * sizeof(char));
    int i;
    
    for (i = 7; i >= 0; i--)
    {
        int value = number & 0xF;
        char c = 0;
        switch (value) {
            case 0: c = '0'; break;
            case 1: c = '1'; break;
            case 2: c = '2'; break;
            case 3: c = '3'; break;
            case 4: c = '4'; break;
            case 5: c = '5'; break;
            case 6: c = '6'; break;
            case 7: c = '7'; break;
            case 8: c = '8'; break;
            case 9: c = '9'; break;
            case 10: c = 'A'; break;
            case 11: c = 'B'; break;
            case 12: c = 'C'; break;
            case 13: c = 'D'; break;
            case 14: c = 'E'; break;
            case 15: c = 'F'; break;
        }
        hex[i] = c;
        number = number >> 4;
    }
    hex[8] = '\0';
    return hex;
}

char *convertDecimalToOctal(int number)
{
    char *octal;
    octal = malloc(12 * sizeof(char));
    int i;
    
    for (i = 10; i >= 0; i--)
    {
        int value = number & 7;
        char c = 0;
        switch (value) {
            case 0: c = '0'; break;
            case 1: c = '1'; break;
            case 2: c = '2'; break;
            case 3: c = '3'; break;
            case 4: c = '4'; break;
            case 5: c = '5'; break;
            case 6: c = '6'; break;
            case 7: c = '7'; break;
        }
        octal[i] = c;
        number = number >> 3;
    }
    octal[11] = '\0';
    return octal;
}

int convertOctalToDecimal(int number)
{
    int value = 0;
    int base = 1;
    int n = number;
    while (n > 0)
    {
        int last = n % 10;
        n /= 10;
        value += base * last;
        base *= 8;
    }
    return value;
}

//Gets the format type - 0 is decimal, 1 is negative decimal, 2 is binary, 3 is hex, 4 is octal
int getFormatType(char identifier)
{
    switch(identifier)
    {
        case 'd':
            return 0;
        case '-':
            return 1;
        case 'b':
            return 2;
        case 'x':
            return 3;
        case 'o':
            return 4;
    }
    return -1;
}

int getCharArrayAsInt(char data[])
{
    int i;
    sscanf(data, "%d", &i);
    return i;
}

int getDecimalValue(char *data, int formatType)
{
    int val = 0;
    switch(formatType)
    {
        case 0:
        case 1:
        case 4:
            val = getCharArrayAsInt(data);
            if (formatType == 4) {
                return convertOctalToDecimal(val);
            }
            return formatType == 1 ? val * -1 : val;
        case 2:
            val = convertBinaryToDecimal(data);
            break;
        case 3:
            val = convertHexToDecimal(data);
            break;
    }
    return val;
}

//gets the number from the argument.
int getNumberFromArg(char arg[])
{
    int type = getFormatType(arg[0]);
    if (type == -1)
    {
        printf("ERROR Invalid number type entered, must be x,d,b,o.\n");
        exit(1);
    }
    char *number = arg + (type == 1 ? 2 : 1);
    return getDecimalValue(number, type);
}

//conducts a calculation on two numbers based upon the operations: +,-,*
int calculate(int first, int second, char operation)
{
    switch (operation)
    {
        case '+':
            return first + second;
        case '-':
            return first - second;
        case '*':
            return first * second;
    }
    return 0;//only occurs if an operation not accounted for is provided.
}

int main(int argc, char *argv[])
{
    
    if (argc != 5)
    {
        printf("ERROR Four arguments required, %d arguments provided.\n", argc - 1);
        exit(1);
    }
    
    char operation = argv[1][0];
    
    if (operation != '+' && operation != '-' && operation != '*')
    {
        printf("ERROR Invalid Operation, '+', '-', or '*' required. Operation provided: %c.\n", operation);
        exit(1);
    }
    
    int firstNumber = getNumberFromArg(argv[2]);
    
    int secondNumber = getNumberFromArg(argv[3]);
    
    int output = getFormatType(argv[4][0]);
    
    if (output == -1)
    {
        printf("ERROR Invalid output operation, must be x,d,b,o.\n");
        exit(1);
    }
    
    //printf("First number: %d, Second Number %d, output: %d\n", firstNumber, secondNumber, output);
    
    int finalVal = calculate(firstNumber, secondNumber, operation);
    
    char *print;
    switch(output) {
        case 0:
            printf("d%d\n", finalVal);
            break;
        case 2:
            print = convertDecimalToBinary(finalVal);
            printf("b%s\n", print);
            free(print);
            break;
        case 3:
            print = convertDecimalToHex(finalVal);
            printf("x%s\n", print);
            free(print);
            break;
        case 4:
            print = convertDecimalToOctal(finalVal);
            printf("o%s\n", print);
            free(print);
            break;
    }
    return 0;
}



