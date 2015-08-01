//
//  main.c
//  crackerNext
//
//  Created by Jacob Levy on 4/2/14.
//
//  Modified, updated Phil Shafer on 4/3/2014
//
//  Copyright (c) 2014 Jacob Levy. All rights reserved.
//
//
//The CRACKED KEY THAT WAS FOUND IS:  UKRAINE
// The implementation of DES used to crack the key was the implementation provided by Dr. Hnatyshin.
// It is the implementation that adheres to the FIPS PUB 46 spec.   
// ******************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "des.h"

void dec2bin (bool *testaRR, bool *to_return, int convert);//converts decimal to binary
void checkKeys(bool *in, bool *out, bool *check, int *start);//checks the keys


int main(int argc, const char * argv[])
{
    //  DES bits are numbered left to right, from least to most, the opposite of normal numbering
    //therefore we initially created the input and output in both directions (forward and backward)
    //and tested both sets, just in case.
    //After testing, we determined that input2 (forwards) and checkout2 (forwards) was the correct pair to pass into the EncryptDES function
    //but we use input and checkout for ease of formatting in printing to the screen
    
    //for example the binary vale of N is 01001110 but below is displayed as 01110010
    bool input[64] = {
        1, 0, 0, 0, 1, 1, 0, 0,//1
        0, 0, 0, 0, 1, 1, 0, 0,//0
        1, 1, 0, 0, 0, 0, 1, 0,//C
        1, 0, 1, 0, 0, 0, 1, 0,//E
        1, 1, 0, 0, 1, 0, 1, 0,//S
        0, 0, 1, 0, 1, 0, 1, 0,//T
        1, 0, 1, 0, 0, 0, 1, 0,//E
        0, 1, 1, 1, 0, 0, 1, 0 //N
    };//hard coded input, i.e. the plaintext
    
   
    //this is the input array in "reverse" which is how the DES implentation takes in the
    //plaintext message
    bool input2[64] ={
        0, 1, 0, 0, 1, 1, 1, 0,//N
        0, 1, 0, 0, 0, 1, 0, 1,//E
        0, 1, 0, 1, 0, 1, 0, 0,//T
        0, 1, 0, 1, 0, 0, 1, 1,//S
        0, 1, 0, 0, 0, 1, 0, 1,//E
        0, 1, 0, 0, 0, 0, 1, 1,//C
        0, 0, 1, 1, 0, 0, 0, 0,//0
        0, 0, 1, 1, 0, 0, 0, 1 //1
    };
    
    
    bool output[64];//empty block to store the output from the EncryptDES function
    bool checkout[64] ={
        1, 0, 1, 0, 1, 0, 0, 1,
        1, 1, 0, 1, 1, 1, 0, 1,
        1, 0, 1, 1, 0, 0, 1, 1,
        0, 1, 0, 1, 0, 1, 0, 0,
        1, 1, 1, 1, 0, 1, 1, 1,
        1, 1, 1, 1, 0, 1, 1, 0,
        0, 0, 1, 0, 0, 1, 1, 1,
        1, 1, 0, 0, 0, 0, 1, 0
    };
    //Hard coded output that we were given, converted into binary
    bool checkout2[64] ={
        0, 1, 0, 0, 0, 0, 1, 1,
        1, 1, 1, 0, 0, 1, 0, 0,
        0, 1, 1, 0, 1, 1, 1, 1,
        1, 1, 1, 0, 1, 1, 1, 1,
        0, 0, 1, 0, 1, 0, 1, 0,
        1, 1, 0, 0, 1, 1, 0, 1,
        1, 0, 1, 1, 1, 0, 1, 1,
        1, 0, 0, 1, 0, 1, 0, 1
    };
    
  
    
    // start checking key with AAAAAA    
    int startKey[7] = {65, 65, 65, 65, 65, 65, 65};
    
    //This converts the input array into Hex code
    int y = sizeof(input);
    printf("(Input MSG: ");
    for (int i = y-4; i >= 0; i-=4)
        printf("%X", input[i]+2*input[i+1]+4*input[i+2]+8*input[i+3]);//this is the conversion to hex code
    printf(")\n");
    
    //this converts the input array into Human Text
    int g = sizeof(input);
    printf("Translates to: ");
    for (int q = g-8; q >= 0; q-=8)
        printf("%c", input[q] +2*input[q+1] + 4*input[q+2] + 8*input[q+3] + 16*input[q+4] + 32*input[q+5] + 64*input[q+6] + 128*input[q+7]);
    printf(" in human text  \n");
    
    //converts output message into HEX code
    int x = sizeof(checkout);
    printf("\n ( MSG to check against: ");
    for (int q = x - 4; q >= 0; q-=4)
        printf("%X", checkout[q]+2*checkout[q+1]+4*checkout[q+2]+8*checkout[q+3]);//this is the conversion to hex code
    printf(")\n");
   

    
    
    checkKeys(input2, output, checkout2, startKey);//calls the checkKeys function which actually checks each key using the EncryptDES function from given DES implementation
    
    return 0;
}

/**
 * This function will convert a number in decimal format (base 10) to a number in binary format (base 2).
 * It takes as input, two pointers to arrays and fills the arrays with the binary number.  testaRR stores the 
 * binary number in one direction  and to_return stores the binary number in the opposite direction  
 *
 *  bool testaRR - Least Significant bit stored at index 0, most significant bit stored at index 7  E.G. in an array with indices numbered 0-7, left to right,  65 = 10000010 (stored backwards as we would read it)
 *
 *  bool to_return - Least Significant bit stored at index 7, most significant bit stored at index 0.  E.G. in an array with indices numbered 0-7, left to right,  65 = 01000001 (stored forwards as we would read it)
 *
 *  int convert - this is the decimal number that is to be converted
 */
void dec2bin (bool *testaRR, bool *to_return, int convert)
{
    
    
    for(int st = 0; st<8; st++){
        testaRR[st] = convert%2;//using the modulus calculation by 2 gives us the remainder which represents the placement
                                //of a 1 bit in the binary string
        to_return[7-st] = testaRR[st];
      
        convert = convert/2;  //we use integer divsion here in order to get the next number to mod by 2 (returns the floor of the division)
       
    }
    
    
    
};

/**
 * This function will attempt to encrypt the known plaintext (in) with the generated key.  If the returned encrypted cipher(out) matches the provided cipher(check) then the generated key has been found.
 *
 * bool in - array representing the binary input
 * bool out - array to hold data that will be passed out of this function, gets the output of the encryption function
 * bool check - the binary representation of the cipher text used to test if encryption with discovered key matches
 * int check - integer array representation of the string to start the key with (makes it easier to test if you know the key)
 */
void checkKeys(bool *in, bool *out, bool *check, int *startKey){
    char keyphrase[8]; //holds the key being currently checked
    bool forward[8]; //holds the returned binary value of converted ASCII char "forwards"
    bool reverse[8];//holds the returned binary value of converted ASCII char in "reverse"
    int conversion;// this represents the ASCII character being converted to binary
    bool flipkey[56];//we use this empty array to store the current key being checked
    long attempts = 0;//we keep track of the number of attempts that it takes to crack the Key
    
    
    for(int start = startKey[0]; start<=90; start++)
    {
        keyphrase[0] = start;  // set first character value
        
        for(int two = startKey[1] ; two<=90; two++){
            
            keyphrase[1] = two; // set second character value
            
            for(int three = startKey[2] ; three<=90; three++){
                
                keyphrase[2] = three; // set third character value
                
                for(int four = startKey[3] ; four<=90; four++){
                    
                    keyphrase[3] = four; // set fourth character value
                    
                    for(int five = startKey[4] ;five<=90;five++){
                        
                        keyphrase[4] = five; // set fifth character value
                        
                        for(int six = startKey[5] ; six <=90; six++){
                            
                            keyphrase[5] = six; // set sixth character value
                            
                            for(int seven = startKey[6] ; seven <=90; seven++){
                                
                                attempts++; // increment the attempts counter
                                keyphrase[6] = seven; // set seventh character value
                                keyphrase[7] = '\0'; // terminate string with null character
                                
                                
                                //this takes each ASCII number in the keyphrase array and converts it to binary and stores it
                               //flipkey array to pass as a key into EncryptDES
                                for(int i = 0 ; i< 7; i++){
                                    conversion = keyphrase[i];
                                    dec2bin(forward, reverse, conversion);
                                    
                                    for(int g = 0;g<8; g++){
                                        flipkey[g + (8*i)] = reverse[g];
                                    }
                                }
                                
                        
                                
                                EncryptDES(flipkey, out, in, 0); //call the EncryptDES function to check the output block that is stored in out
                                
                                bool match = 0;
                                int bitCount = 0;
                                
                                //check each bit until a match isn't found
                                do{
                                    // do the bits match?
                                    match = (out[bitCount] == check[bitCount]);
                                    bitCount++;
                                }while(match && bitCount < 64);
                                
                                if(match) {
                                    printf("\n\n KEY CORRECT!  %s \n", keyphrase);//Key found! Print to screen and exit
                                    printf("\n");
                                    printf("found on attempt # %lu,", attempts);
                                    exit(0);
                                }
                                else
                                {
                                     if (attempts%5000000 == 0 ){//only print the incorrect phrase every 5 million tries to keep from hogging too many resources
                                         //it seems to make about 5 million tries approximately every 2 minutes
                                         //should take about 30ish hours to find the key and about 6 billion attempts
                                    printf("%s is not the phrase", keyphrase); // not found yet...
                                         
                                    printf("... Attempt number: %lu \n", attempts); // print attempt #
                                     }
                                }
                                
                            } // end seven loop
                            
                        }// end six loop
                        
                    } // end five loop
                    
                }// end four loop
                
            } // end three loop
            
        } // end two loop
        
    } // end start loop
};

