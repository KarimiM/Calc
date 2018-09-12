//
//  format.c
//  Calc
//
//  Created by Masood Karimi on 7/12/18.
//  Copyright © 2018 Masood Karimi. All rights reserved.
//

#include	<stdio.h>
#include <stdlib.h>
#include	<string.h>

//a union to represent both an int and a float value from the same address.
typedef union int_and_float_conversion {
    int value;
    float floatValue;
} Value;

//gets the size of an array
int getSize(char *array)
{
    int size = 0;
    for (int i = 0; array[i] != '\0'; i++) {
        size++;
    }
    return size;
}

//turns an int value into a char[] string
void intToDecASCII(int number, char *string)
{
    char format[12];
    char sign = '+';
    if (number < 0) {
        sign = '-';
        number *= -1;
    }
    int index = 10;
    while (number > 0)
    {
        int remainder = number % 10;
        number /= 10;
        format[index] = '0' + remainder;
        index--;
    }
    format[index] = sign;
    format[11] = '\0';
    for (int i = index; i <= 10; i++)
    {
        if (format[i] == '+') continue;
        *string = format[i];
        string++;
    }
    *string = '\0';
    
}

void get_float_digit( float x, char * digit, int * pow10, float * remainder )
{
    int			p10;
    
    if ( x == 0.0 )
    {
        *digit = '0';			// digit is always '0'
        *pow10 = 0;
        *remainder = 0.0;
    }
    else
    {
        *pow10 = 0;			// start with 10^0
        while ( x >= 10.0 )		// reduce
        {
            x /= 10.0;		// large values get smaller
            *pow10 += 1;
        }
        while ( x < 1.0 )		// increase
        {
            x *= 10.0;		// small values get larger
            *pow10 -= 1;
        }
        *digit = '0';
        do {				// 10.0 > x >= 1.0
            x -= 1.0;
            *digit += 1;		// digit is always non-'0'zero
        } while ( x >= 1.0 );
        p10 = 0;
        while ( p10 > *pow10 )		// leading digit is now removed from x
        {
            x /= 10;
            p10 -= 1;
        }
        while ( p10 < *pow10 )
        {
            x *= 10;
            p10 += 1;
        }
        *remainder = x;
    }
}

static void
append( char * s, char c )
{
    char		buf[2];
    
    buf[0] = c;
    buf[1] = '\0';
    strcat( s, buf );
}

union Number {
    int	i;
    float	f;
};

void floatToASCII( float x, char * output )
{
    char		c;
    int		pow10, p10, plast;
    int		i;
    float		remainder;
    char		exponent[10];
    union Number	a;
    unsigned int	biasedExp;
    unsigned int	mantissa;
    int		sign;
    
    a.f = x;
    biasedExp = a.i >> 23 & 0x000000ff;
    mantissa = a.i & 0x007fffff;
    sign = a.i >> 31;
    if (biasedExp == 0xFF)//check for ninf, pinf, and NaN
    {
        if (mantissa == 0)
        {
            strcpy(output, sign == 0 ? "pinf" : "ninf");
        } else {
            strcpy(output, "NaN");
        }
        
    }
    output[0] ='\0';
    if ( x < 0.0 )
    {
        append( output, '-' );
        x = -x;					// make x positive
    }
    get_float_digit( x, &c, &pow10, &remainder );
    append( output, c );
    append( output, '.' );
    x = remainder;
    plast = p10 = pow10;			// pow10 set by get_float_digit()
    for ( i = 1 ; i < 7 ; i++ )		// 7 significant digits in 32-bit float
    {
        get_float_digit( x, &c, &p10, &remainder );
        if ( (plast - p10) > 1 )
        {
            append( output, '0' );	// fill in zero to next nonzero digit
            plast -= 1;
        }
        else
        {
            append( output, c );
            x = remainder;
            plast = p10;
        }
    }
    if ( pow10 < 0 )		// negative exponent
    {
        exponent[0] = 'e';
        intToDecASCII( pow10, exponent+1 );
    }
    else if ( pow10 < 10 )		// positive single-digit exponent
    {
        exponent[0] = 'e';
        exponent[1] = '+';
        exponent[2] = '0';
        intToDecASCII( pow10, exponent+3 );
    }
    else				// positive multi-digit exponent
    {
        exponent[0] = 'e';
        exponent[1] = '+';
        intToDecASCII( pow10, exponent+2 );
    }
    strcat( output, exponent );
}

//checks if there's invalid bit-integer as an argument.
void errorCheck(char * arg)
{
    for (int i = 0; i < 32; i++)
    {
        if (arg[i] != '1' && arg[i] != '0')
        {
            printf("ERROR there are invalid bits.\n");
            exit(1);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("ERROR Two arguments needed, %d arguments supplied\n", argc - 1);
        exit(1);
    }
    if (getSize(argv[1]) > 32)
    {
        printf("ERROR more than 32 bits.");
        exit(1);
    }
    errorCheck(argv[1]);
    
    int isInt = argv[2][0] == 'i' ? 1 : 0;
    
    Value number;
    
    int value = 0;
    
    for (int i = 0; argv[1][i] != '\0'; i++)
    {
        char bit = argv[1][i];
        value = value << 1;
        if (bit == '1')
        {
             value |= 1;
        }
        
    }
    
    number.value = value;
    
    char string[100];
    
    if (isInt == 1)
    {
        intToDecASCII(number.value, string);
    } else {
        floatToASCII(number.floatValue, string);
    }
    
    
    printf("%s%s\n", isInt == 0 && number.floatValue == -0.0 && argv[1][0] == '1' ? "-" : "",string);
}
