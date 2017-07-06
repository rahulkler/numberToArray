#include "NumToCharArray.h"

/*

There are a number of similar variables used in all of the functions below so I will state their uses here.
The following are the variables that are taken as parameters for all the functions.
Please note the the verb 'print' is used quite often to describe adding characters into the array because I assume that this array will be printed on an LCD screen later on.

1. numberInput -> the number that is input by the programmer to be stored in the array.

2. nAllowedCharacters -> the maximum number of characters allowed in the array, no matter the size of the numberInput or the number of decimal places.

3. toBePrinted[] -> the array passed into the function as an argument for the numberInput to be stored in.

4. displaySign -> a boolean variable for whether the programmer wants the sign (+/-) to be printed. 

5. shiftNumbersToRight -> a boolean variable for whether the programmer wants only the numbers to be shifted to the right of the array, while the sign stays at the left.

    For example, say numberInput = 12, nAllowedCharacters = 5, displaySign = true, shiftNumbersToRight = true and shiftSignToRight = false.

    The array is then produced as shown below.

    |+| | |1|2|

6.  shiftSignToRight -> a boolean variable for whether the programmer wants the sign to always be exactly one character to the left of the number.

    For example, say numberInput = 12, nAllowedCharacters = 5, displaySign = true, shiftNumbersToRight = true and shiftSignToRight = true.

    The array is then produced as shown below.

    | | |+|1|2|

    However, if the following conditions are the same except that shiftNumbersToRight = false, the array is produced as

    |+|1|2| | |


7. nDecimalPlaces -> The number of decimal places to be printed out. For doubles and floats, the decimal place is rounded off.


The following are the variables that are used in the functions.

1. nMeaningfulCharacters -> the number of characters that are either numerical digits, decimal places or signs. Spaces are not considered meaningful characters.

2. nSpaces -> the number of spaces to be printed before the meaningful characters are printed if either shiftNumbersToRight or shiftSignToRight is true.

3. positionOfCharacter -> the latest position in the array for the next character to be placed.

*/

namespace numtochararray
{

//http://binaryupdates.com/structures-typedef-union-in-c-programming/
typedef union
{
  long L;
  float F;
} LF_t;



bool numToChar(float numberInput, uint8_t nAllowedCharacters, char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t nDecimalPlaces)
{
  uint8_t nMeaningfulCharacters = 0;
  int8_t nSpaces = 0;
  uint8_t positionOfCharacter = 0;

  //If nAllowedCharacters == 1 and displaySign = true, I assume that the programmer only wants to see the sign of the number. Hence, only the sign is printed.
  //If numberInput == 0, '0' is printed.

  if( (nAllowedCharacters == 1) && displaySign )
  {
    if(numberInput > 0)
    {
      toBePrinted[0] = '+';
    }
    else if(numberInput == 0)
    {
      toBePrinted[0] = '0';
    }
    else
    {
      toBePrinted[0] = '-';
    }
  }

  //If the nAllowedCharacters >= 1 and numberInput == 0, print '0' together with the decimal places according to the shiftNumbersToRight, 
  // shiftSignToRight and nDecimalPlaces variables.

  else if(nAllowedCharacters >= 1 && numberInput == 0)
  {

  	//Here, the nMeaningfulCharacters are calculated based on the conditions set by the programmer.

    if(nDecimalPlaces > 0)
    {
      nMeaningfulCharacters = nDecimalPlaces + 2;
    }
    else
    {
      nMeaningfulCharacters = 1;
    }

    //nSpaces is then calculated.

    nSpaces = nAllowedCharacters - nMeaningfulCharacters;

     if(nSpaces < 0)
    {
       nSpaces = 0;
    }

	//If the numbers have to be shifted to the right, print the spaces beforehand.

    if(shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    //Then print '0'.

    toBePrinted[positionOfCharacter++] = '0';

    if(nDecimalPlaces > 0)
    {
      if(positionOfCharacter >= nAllowedCharacters)
      {

      	//Print 'E' at the end of the array to indicate error (insufficient space).

        toBePrinted[nAllowedCharacters - 1] = 'E';

        //Exit function and return false.

        return false;
      }

      //Print decimal point.

      toBePrinted[positionOfCharacter++] = '.';

      for(uint8_t i = 0; i < nDecimalPlaces; i++)
      {

      	//Print 'E' at the end of the array to indicate error (insufficient space). Return.

        if(positionOfCharacter > nAllowedCharacters)
        {
          toBePrinted[nAllowedCharacters - 1] = 'E';

          //Exit function and return false.

          return false;
        }
        else
        {

        //Print out '0's at the decimal places.

        toBePrinted[positionOfCharacter++] = '0';
        }
      }
    }
  }

  //In all other conditions, except if nAllowedCharacters <= 0, the next statement is called.

  else if(nAllowedCharacters >= 1)
  {
    int8_t  exponentInBaseTwo;
    int8_t nDigitsInCharacteristic;
    uint8_t existenceOfSign = 0;
    uint8_t index = 0;
    LF_t x;
    x.F = numberInput;

    exponentInBaseTwo = int8_t( int16_t( (x.L >> 23) & 0x00FF ) - 127 );

    //Round down the value of c to increase the product of division. This is done to reduce the effect of the inaccuracy of the float on the exponentInBaseTen.
    //Then, increment the value by 1.
    //Take 55 x 10^2.1 = 6924 (4 digits) ,55 x 10^2.7 = 27565 (5 digits) and 55 x 10^3 = 55000 (5 digits).
    //Since the exponent is casted to uint8_t, instead of 2.1 or 2.7, it will be stored as 2.
    //So we take 2+1 = 3 to ensure we always get either the exact number of digits, or an extra digit.

    //Assume 2^e = 10^f. We wish to obtain the value of f to find the number of digits we will allocate in dtostrf(). We know m from the exponent component of the float.
    //Let 10^f = (2^c)^f.
    //Hence, 2^e = 2^cf.
    //f = e/c
    //And, 2^c = 10. Hence, c = log(10) / log(2) = 3.32192809.

    nDigitsInCharacteristic = int8_t ( float(exponentInBaseTwo) / 3.32192809 ) + 2;

    if(nDigitsInCharacteristic <= 0)
    {
      nDigitsInCharacteristic = 1;
    }

  //The existence of the sign is omitted in the nMeaningfulCharacters because nMeaningfulCharacters will be used in dtostrf(), and the sign is dealt with later in the function, and not by dtostrf().
    if(nDecimalPlaces > 0)
    {
      if( numberInput < 0 || (numberInput > 0 && displaySign == true) )
      {
        nMeaningfulCharacters = nDigitsInCharacteristic + nDecimalPlaces + 2;
        existenceOfSign = 1;
      }
      else
      {
        nMeaningfulCharacters = nDigitsInCharacteristic + nDecimalPlaces + 1;
      }
    }

    else
    {
      if( numberInput < 0 || (numberInput > 0 && displaySign == true) )
      {
        nMeaningfulCharacters = nDigitsInCharacteristic + 1;
        existenceOfSign = 1;
      }
      else
      {
        nMeaningfulCharacters = nDigitsInCharacteristic;
      }
    }

    //storeNumberTemporarily[] is a char array in which the numbers will be stored temporarily before being shifted into the toBePrinted[] array.

    char storeNumberTemporarily[nMeaningfulCharacters - existenceOfSign]  = {};

    dtostrf( abs(numberInput), nMeaningfulCharacters-existenceOfSign, nDecimalPlaces, storeNumberTemporarily );

  	//It should be noted that there may be a space printed out in the first position of the array by the dtostrf() function because the calculation done is not 100%
  	// accurate, but is efficient. Hence, an integer 'index' is used to indicate if there is a space printed out.

    if(storeNumberTemporarily[0] == ' ')
    {
      nMeaningfulCharacters -= 1;
      index = 1;
    }

    //Calculating number of spaces

    nSpaces = nAllowedCharacters - nMeaningfulCharacters;

    if(nSpaces < 0)
     {
       nSpaces = 0;
     }

  	//Print spaces if both numbers and sign are to be shifted.

    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    //Print sign if displaySign == true.

    if(displaySign && numberInput > 0)
    {
      toBePrinted[positionOfCharacter++] = '+';
    }

    else if(numberInput < 0)
    {
        toBePrinted[positionOfCharacter++] = '-';
    }

    //Print spaces if only the numbers are to be shifted to the right.

    if(shiftNumbersToRight && !shiftSignToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

	//Check if there is sufficient space to print the number of desired characters. If there is, transfer the characters in storeNumberTemporarily[] to toBePrintedp[].

    if(nMeaningfulCharacters <= nAllowedCharacters)
    {
      for(uint8_t i = 0; i < nMeaningfulCharacters ; i++)
      {
        toBePrinted[positionOfCharacter++] = storeNumberTemporarily[index + i];
      }
    }

    //Otherwise, check if the positionOfCharacter is equals to or more than nAllowedCharacters. If it is, print 'E'. Or else, continue printing.

    else
    {
      for(uint8_t i = 0; i < nMeaningfulCharacters ; i++)
      {
         if(positionOfCharacter >= nAllowedCharacters)
         {
          toBePrinted[nAllowedCharacters - 1] = 'E';

          //Exit function and return false.

          return false;
         }

         else
         {
          toBePrinted[positionOfCharacter++] = storeNumberTemporarily[index + i];
         }
      }
    }
  }

  //Exit function and return true (Success).

  return true;
}

//The function used to handle double variable types is the same as the function to handle float variable types. Hence, I won't explain the next function.



bool numToChar(double numberInput, uint8_t nAllowedCharacters, char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t nDecimalPlaces)
{
  uint8_t nMeaningfulCharacters;
  int8_t nSpaces = 0;
  uint8_t positionOfCharacter = 0;

  if( (nAllowedCharacters == 1) && displaySign )
  {
    if(numberInput > 0)
    {
      toBePrinted[0] = '+';
    }
    else if(numberInput == 0)
    {
      toBePrinted[0] = '0';
    }
    else
    {
      toBePrinted[0] = '-';
    }
  }

  else if(nAllowedCharacters >= 1 && numberInput == 0)
  {
    if(nDecimalPlaces > 0)
    {
      nMeaningfulCharacters = nDecimalPlaces + 2;
    }
    else
    {
        nMeaningfulCharacters = 1;
    }

    nSpaces = nAllowedCharacters - nMeaningfulCharacters;

     if(nSpaces < 0)
    {
       nSpaces = 0;
    }

    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    if(shiftNumbersToRight && !shiftSignToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    toBePrinted[positionOfCharacter++] = '0';

    if(nDecimalPlaces > 0)
    {
      if(positionOfCharacter >= nAllowedCharacters)
      {
        toBePrinted[nAllowedCharacters - 1] = 'E';

        //Exit function and return false.

        return false;
      }
      toBePrinted[positionOfCharacter++] = '.';

      for(uint8_t i = 0; i < nDecimalPlaces ; i++)
      {
        if(positionOfCharacter > nAllowedCharacters)
        {
          toBePrinted[nAllowedCharacters - 1] = 'E';

          //Exit function and return false.

          return false;
        }
        else
        {
        toBePrinted[positionOfCharacter++] = '0';
        }
      }
    }
  }

  else if(nAllowedCharacters >= 1)
  {
    int8_t  exponentInBaseTwo;
    int8_t nDigitsInCharacteristic;
    uint8_t existenceOfSign = 0;
    uint8_t index = 0;
    LF_t x;
    x.F = numberInput;

    exponentInBaseTwo = int8_t( int16_t( (x.L >> 23) & 0x00FF ) - 127 );

    nDigitsInCharacteristic = int8_t ( float(exponentInBaseTwo) / 3.32192809 ) + 2;

    if(nDigitsInCharacteristic <= 0)
    {
      nDigitsInCharacteristic = 1;
    }

    if(nDecimalPlaces > 0)
    {
      if( numberInput < 0 || (numberInput > 0 && displaySign == true))
      {
        nMeaningfulCharacters = nDigitsInCharacteristic + nDecimalPlaces + 2;
        existenceOfSign = 1;
      }
      else
      {
        nMeaningfulCharacters = nDigitsInCharacteristic + nDecimalPlaces + 1;
      }
    }

    else
    {
      if( numberInput < 0 || (numberInput > 0 && displaySign == true) )
      {
        nMeaningfulCharacters = nDigitsInCharacteristic + 1;
        existenceOfSign = 1;
      }
      else
      {
        nMeaningfulCharacters = nDigitsInCharacteristic;
      }
    }

    char storeNumberTemporarily[nMeaningfulCharacters - existenceOfSign]  = {};

    dtostrf( abs(numberInput), nMeaningfulCharacters - existenceOfSign, nDecimalPlaces, storeNumberTemporarily );

    if(storeNumberTemporarily[0] == ' ')
    {
      nMeaningfulCharacters -= 1;
      index = 1;
    }

    nSpaces = (nAllowedCharacters - nMeaningfulCharacters);

    if(nSpaces < 0)
     {
       nSpaces = 0;
     }

    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    if(displaySign && numberInput > 0)
    {
      toBePrinted[positionOfCharacter++] = '+';
    }

    else if(numberInput < 0)
    {
        toBePrinted[positionOfCharacter++] = '-';
    }

    if(shiftNumbersToRight && !shiftSignToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    if(nMeaningfulCharacters <= nAllowedCharacters)
    {
      for(uint8_t i = 0; i < nMeaningfulCharacters ; i++)
      {
        toBePrinted[positionOfCharacter++] = storeNumberTemporarily[index + i];
      }
    }

    else
    {
      for(uint8_t i = 0; i < nMeaningfulCharacters ; i++)
      {
	     if(positionOfCharacter >= nAllowedCharacters)
	     {
	      toBePrinted[nAllowedCharacters - 1] = 'E';

	      //Exit function and return false.

	      return false;
	     }


	     else
	     {
	      toBePrinted[positionOfCharacter++] = storeNumberTemporarily[index + i];
	     }
      }
    }
  }

  //Exit function and return true (Success).

  return true;
}



bool numToChar(int8_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t nDecimalPlaces)
{
  uint8_t nMeaningfulCharacters;
  int8_t nSpaces = 0;
  uint8_t positionOfCharacter = 0;

  //If nAllowedCharacters == 1 and displaySign = true, I assume that the programmer only wants to see the sign of the number. Hence, only the sign is printed.
  //If numberInput == 0, '0' is printed.

  if( (nAllowedCharacters == 1) && displaySign )
  {
    if(numberInput > 0)
    {
      toBePrinted[0] = '+';
    }
    else if(numberInput == 0)
    {
      toBePrinted[0] = '0';
    }
    else
    {
      toBePrinted[0] = '-';
    }
  }

  //If the nAllowedCharacters >= 1 and numberInput == 0, print '0' together with the decimal places according to the shiftNumbersToRight, 
  // shiftSignToRight and nDecimalPlaces variables.

  else if(nAllowedCharacters >= 1 && numberInput == 0)
  {

  	//Here, the nMeaningfulCharacters are calculated based on the conditions set by the programmer.

    if(nDecimalPlaces > 0)
    {
      nMeaningfulCharacters = nDecimalPlaces + 2;
    }
    else
    {
        nMeaningfulCharacters = 1;
    }

    //nSpaces is then calculated.

    nSpaces = nAllowedCharacters - nMeaningfulCharacters;

    if(nSpaces < 0)
    {
       nSpaces = 0;
    }

    //If the numbers have to be shifted to the right, print the spaces beforehand.

    if(shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    //Then print '0'.

    toBePrinted[positionOfCharacter++] = '0';

    if(nDecimalPlaces > 0)
    {
      if(positionOfCharacter >= nAllowedCharacters)
      {

      	//Print 'E' at the end of the array to indicate error (insufficient space). Return.

        toBePrinted[nAllowedCharacters - 1] = 'E';

        //Exit function and return false.

        return false;
      }

      //Print decimal point.

      toBePrinted[positionOfCharacter++] = '.';

      //Print 'E' at the end of the array to indicate error (insufficient space). Return.

      for(uint8_t i = 0; i < nDecimalPlaces; i++)
      {
        if(positionOfCharacter > nAllowedCharacters)
        {
          toBePrinted[nAllowedCharacters - 1] = 'E';

          //Exit function and return false.

          return false;
        }
        else
        {

          //Print out '0's at the decimal places.

          toBePrinted[positionOfCharacter++] = '0';
        }
      }
    }
  }

  else if(nAllowedCharacters >= 1)
  {

    uint8_t nDigits;

    //storeNumberTemporarily[] is a char array in which the numbers will be stored temporarily before being shifted into the toBePrinted[] array. 
    //3 characters for the numberInput, 1 character for terminating null.

    char storeNumberTemporarily[4] = {};

    //Storing the numberInput into the array storeNumberTemporarily[].

    itoa( abs(numberInput), storeNumberTemporarily, 10 );

    //Computing the number of digits (characteristic only).

    nDigits = strlen(storeNumberTemporarily);


    //Computing the number of elements in the array that will store characters that have a true meaning. This is done considering the length of numberInput,
    //the number of decimal places, the existence of the sign and the existence of the decimal point.

	if(nDecimalPlaces > 0)
	{
   	  if( (numberInput < 0) || (numberInput > 0 && displaySign == true) )
	  {
	    nMeaningfulCharacters = nDigits + nDecimalPlaces + 2;
	  }

	  else
	  {
	    nMeaningfulCharacters = nDigits + nDecimalPlaces + 1;
	  }
    }

	else
	{
      if( (numberInput < 0) || (numberInput > 0 && displaySign == true) )
      {
        nMeaningfulCharacters = nDigits + 1;
      }
      else
      {
        nMeaningfulCharacters = nDigits;
      }
    }

    //Calculating number of spaces

    nSpaces = nAllowedCharacters - nMeaningfulCharacters;

    if(nSpaces < 0)
    {
      nSpaces = 0;
    }

    //shift sign and numbers to the right
    if(shiftSignToRight && shiftNumbersToRight)
    {
     for(uint8_t i = 0; i < nSpaces; i++)
     {
       toBePrinted[positionOfCharacter++] = ' ';
     }
    }

    //display sign
	if(displaySign && numberInput > 0)
	{
	  toBePrinted[positionOfCharacter++] = '+';
	}

	else if(numberInput < 0)
	{
	  toBePrinted[positionOfCharacter++] = '-';
	}

	//shifts number to right
	if(shiftNumbersToRight && !shiftSignToRight)
	{
	  for(uint8_t i = 0; i < nSpaces; i++)
	  {
	    toBePrinted[positionOfCharacter++] = ' ';
	  }
	}

	//Check if there is sufficient space to print the number of desired characters. If there is, transfer the characters in storeNumberTemporarily[] to toBePrintedp[].

	if(nAllowedCharacters >= nMeaningfulCharacters)
	{
	  for(uint8_t i = 0; i < nDigits ; i++)
	  {
	    toBePrinted[positionOfCharacter++] = storeNumberTemporarily[i];
	  }

	  if(nDecimalPlaces > 0)
	  {
	    toBePrinted[positionOfCharacter++] = '.';

	    for(uint8_t i = 0; i < nDecimalPlaces ; i++)
	    {
	        toBePrinted[positionOfCharacter++] = '0';
	    }
	  }
	}

    //Otherwise, check if the positionOfCharacter is equals to or more than nAllowedCharacters. If it is, print 'E'. Or else, continue printing.

    else
    {
      for(uint8_t i = 0; i < nDigits; i++)
      {

        if(positionOfCharacter >= nAllowedCharacters)
        {

          //If the positionOfCharacter is equals to or more than nAllowedCharacters, print 'E'.

          toBePrinted[nAllowedCharacters - 1] = 'E';

          //Exit function and return false.

          return false;
        }

        else
        {

          //Otherwise, continue tranferring the characters from storeNumberTemporarily[] to toBePrinted[].

          toBePrinted[positionOfCharacter++] = storeNumberTemporarily[i];
        }
      }

      if( nDecimalPlaces > 0  && positionOfCharacter >= nAllowedCharacters )
      {

      	//If nDecimalPlaces > 0, the decimal point must first be printed. Check if 

       	toBePrinted[nAllowedCharacters - 1] = 'E';

       	//Exit function and return false.

        return false;
      }

      else if(nDecimalPlaces > 0)
      {
        toBePrinted[positionOfCharacter++] = '.';
      }

      for(uint8_t i = 0; i < nDecimalPlaces; i++ )
      {
        if( positionOfCharacter >= nAllowedCharacters )
        {
          toBePrinted[nAllowedCharacters - 1] = 'E';

          //Exit function and return false.

          return false;
        }
        else
        {
          toBePrinted[positionOfCharacter++] = '0';
        }
      }
    }
  }

  //Exit function and return true (Success).

  return true;
}

/*

The function used to handle int16_t, int32_t, uint8_t, uint16_t and uint32_t variable types is mostly the same as the function
to handle float variable types. Hence, I may only explain very little regarding the next functions.

However, do note that the following functions are used to store numberInput into toBePrinted[] for the following variable types:

int8_t, int16_t and uint8_t -> itoa()

int32_t -> ltoa()

uint16_t -> utoa()

uint32_t -> ultoa()

*/



bool numToChar(int16_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t nDecimalPlaces)
{
  uint8_t nMeaningfulCharacters;
  int8_t nSpaces = 0;
  uint8_t positionOfCharacter = 0;

  if( (nAllowedCharacters == 1) && displaySign )
  {
    if(numberInput > 0)
    {
      toBePrinted[0] = '+';
    }
    else if(numberInput == 0)
    {
      toBePrinted[0] = '0';
    }
    else
    {
      toBePrinted[0] = '-';
    }
  }

  else if(nAllowedCharacters >= 1 && numberInput == 0)
  {
    if(nDecimalPlaces > 0)
    {
      nMeaningfulCharacters = nDecimalPlaces + 2;
    }
    else
    {
      nMeaningfulCharacters = 1;
    }

    nSpaces = nAllowedCharacters - nMeaningfulCharacters;

     if(nSpaces < 0)
    {
       nSpaces = 0;
    }

    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    if(shiftNumbersToRight && !shiftSignToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    toBePrinted[positionOfCharacter++] = '0';

    if(nDecimalPlaces > 0)
    {
      if(positionOfCharacter >= nAllowedCharacters)
      {
        toBePrinted[nAllowedCharacters - 1] = 'E';

        //Exit function and return false.

        return false;
      }
      toBePrinted[positionOfCharacter++] = '.';

      for(uint8_t i = 0; i < nDecimalPlaces ; i++)
      {
        if(positionOfCharacter > nAllowedCharacters)
        {
          toBePrinted[nAllowedCharacters - 1] = 'E';

          //Exit function and return false.

          return false;
        }
        else
        {
          toBePrinted[positionOfCharacter++] = '0';
        }
      }
    }
  }

  else if(nAllowedCharacters >= 1)
  {
    uint8_t nDigits;

    //5 characters for the numberInput, 1 character for terminating null.
    char storeNumberTemporarily[6] = {};

    itoa( abs(numberInput), storeNumberTemporarily, 10 );

    nDigits = strlen(storeNumberTemporarily);

	if(nDecimalPlaces > 0)
	{
   	  if( (numberInput < 0) || (numberInput > 0 && displaySign == true) )
	  {
	    nMeaningfulCharacters = nDigits + nDecimalPlaces + 2;
	  }

	  else
	  {
	    nMeaningfulCharacters = nDigits + nDecimalPlaces + 1;
	  }
    }

	else
	{
      if( (numberInput < 0) || (numberInput > 0 && displaySign == true) )
      {
        nMeaningfulCharacters = nDigits + 1;
      }
      else
      {
        nMeaningfulCharacters = nDigits;
      }
    }

    nSpaces = nAllowedCharacters - nMeaningfulCharacters;

    if(nSpaces < 0)
    {
      nSpaces = 0;
    }

    if(shiftSignToRight && shiftNumbersToRight)
    {
     for(uint8_t i = 0; i < nSpaces; i++)
     {
       toBePrinted[positionOfCharacter++] = ' ';
     }
    }

	if(displaySign && numberInput > 0)
	{
	  toBePrinted[positionOfCharacter++] = '+';
	}

	else if(numberInput < 0)
	{
	  toBePrinted[positionOfCharacter++] = '-';
	}

	if(shiftNumbersToRight && !shiftSignToRight)
	{
	  for(uint8_t i = 0; i < nSpaces; i++)
	  {
	    toBePrinted[positionOfCharacter++] = ' ';
	  }
	}

	if(nAllowedCharacters >= nMeaningfulCharacters)
	{
	  for(uint8_t i = 0; i < nDigits ; i++)
	  {
	    toBePrinted[positionOfCharacter++] = storeNumberTemporarily[i];
	  }

	  if(nDecimalPlaces > 0)
	  {
	    toBePrinted[positionOfCharacter++] = '.';

	    for(uint8_t i = 0; i < nDecimalPlaces ; i++)
	    {
	        toBePrinted[positionOfCharacter++] = '0';
	    }
	  }
	}

    else
    {
      for(uint8_t i = 0; i < nDigits; i++)
      {
        if(positionOfCharacter >= nAllowedCharacters)
        {
        toBePrinted[nAllowedCharacters - 1] = 'E';

        //Exit function and return false.

        return false;
        }

        else
        {
           toBePrinted[positionOfCharacter++] = storeNumberTemporarily[i];
        }
      }

      if( nDecimalPlaces > 0  && positionOfCharacter >= nAllowedCharacters )
      {
       	toBePrinted[nAllowedCharacters - 1] = 'E';

       	//Exit function and return false.

        return false;
      }

      else if(nDecimalPlaces > 0)
      {
        toBePrinted[positionOfCharacter++] = '.';
      }

      for(uint8_t i = 0; i < nDecimalPlaces; i++ )
      {
        if( positionOfCharacter >= nAllowedCharacters )
        {
          toBePrinted[nAllowedCharacters - 1] = 'E';

          //Exit function and return false.

          return false;
        }
        else
        {
          toBePrinted[positionOfCharacter++] = '0';
        }
      }
    }
  }

  //Exit function and return true (Success).

  return true;
}



bool numToChar(int32_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t nDecimalPlaces)
{
  uint8_t nMeaningfulCharacters;
  int8_t nSpaces = 0;
  uint8_t positionOfCharacter = 0;

  if( (nAllowedCharacters == 1) && displaySign )
  {
    if(numberInput > 0)
    {
      toBePrinted[0] = '+';
    }
    else if(numberInput == 0)
    {
      toBePrinted[0] = '0';
    }
    else
    {
      toBePrinted[0] = '-';
    }
  }

  else if(nAllowedCharacters >= 1 && numberInput == 0)
  {
    if(nDecimalPlaces > 0)
    {
      nMeaningfulCharacters = nDecimalPlaces + 2;
    }
    else
    {
        nMeaningfulCharacters = 1;
    }

    nSpaces = nAllowedCharacters - nMeaningfulCharacters;

     if(nSpaces < 0)
    {
       nSpaces = 0;
    }

    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    if(shiftNumbersToRight && !shiftSignToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    toBePrinted[positionOfCharacter++] = '0';

    if(nDecimalPlaces > 0)
    {
      if(positionOfCharacter >= nAllowedCharacters)
      {
        toBePrinted[nAllowedCharacters - 1] = 'E';

        //Exit function and return false.

        return false;
      }
      toBePrinted[positionOfCharacter++] = '.';

      for(uint8_t i = 0; i < nDecimalPlaces ; i++)
      {
        if(positionOfCharacter > nAllowedCharacters)
        {
          toBePrinted[nAllowedCharacters - 1] = 'E';

          //Exit function and return false.

          return false;
        }
        else
        {
        toBePrinted[positionOfCharacter++] = '0';
        }
      }
    }
  }

  else if(nAllowedCharacters >= 1)
  {
    uint8_t nDigits;

    //10 characters for digits, 1 character for terminating null.

    char storeNumberTemporarily[11] = {};

    ltoa( abs(numberInput), storeNumberTemporarily, 10 );

    nDigits = strlen(storeNumberTemporarily);

	if(nDecimalPlaces > 0)
	{
   	  if( (numberInput < 0) || (numberInput > 0 && displaySign == true) )
	  {
	    nMeaningfulCharacters = nDigits + nDecimalPlaces + 2;
	  }

	  else
	  {
	    nMeaningfulCharacters = nDigits + nDecimalPlaces + 1;
	  }
    }

	else
	{
      if( (numberInput < 0) || (numberInput > 0 && displaySign == true) )
      {
        nMeaningfulCharacters = nDigits + 1;
      }
      else
      {
        nMeaningfulCharacters = nDigits;
      }
    }

    nSpaces = nAllowedCharacters - nMeaningfulCharacters;

    if(nSpaces < 0)
    {
      nSpaces = 0;
    }

    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

	if(displaySign && numberInput > 0)
	{
	  toBePrinted[positionOfCharacter++] = '+';
	}

	else if(numberInput < 0)
	{
	  toBePrinted[positionOfCharacter++] = '-';
	}

	if(shiftNumbersToRight && !shiftSignToRight)
	{
	  for(uint8_t i = 0; i < nSpaces; i++)
	  {
	    toBePrinted[positionOfCharacter++] = ' ';
	  }
	}

	if(nAllowedCharacters >= nMeaningfulCharacters)
	{
	  for(uint8_t i = 0; i < nDigits ; i++)
	  {
	    toBePrinted[positionOfCharacter++] = storeNumberTemporarily[i];
	  }

	  if(nDecimalPlaces > 0)
	  {
	    toBePrinted[positionOfCharacter++] = '.';

	    for(uint8_t i = 0; i < nDecimalPlaces ; i++)
	    {
	        toBePrinted[positionOfCharacter++] = '0';
	    }
	  }
	}

    else
    {
      for(uint8_t i = 0; i < nDigits; i++)
      {

        if(positionOfCharacter >= nAllowedCharacters)
        {
        toBePrinted[nAllowedCharacters - 1] = 'E';

        //Exit function and return false.

        return false;
        }

        else
        {
           toBePrinted[positionOfCharacter++] = storeNumberTemporarily[i];
        }
      }

      if( nDecimalPlaces > 0  && positionOfCharacter >= nAllowedCharacters )
      {
       	toBePrinted[nAllowedCharacters - 1] = 'E';

       	//Exit function and return false.

        return false;
      }

      else if(nDecimalPlaces > 0)
      {
        toBePrinted[positionOfCharacter++] = '.';
      }

      for(uint8_t i = 0; i < nDecimalPlaces; i++ )
      {
        if( positionOfCharacter >= nAllowedCharacters )
        {
          toBePrinted[nAllowedCharacters - 1] = 'E';

          //Exit function and return false.

          return false;
        }
        else
        {
          toBePrinted[positionOfCharacter++] = '0';
        }
      }
    }
  }

  //Exit function and return true (Success).

  return true;
}



bool numToChar(uint8_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t nDecimalPlaces)
{
  uint8_t nMeaningfulCharacters;
  int8_t nSpaces = 0;
  uint8_t positionOfCharacter = 0;

  if( (nAllowedCharacters == 1) && displaySign )
  {
    if(numberInput > 0)
    {
      toBePrinted[0] = '+';
    }
    else if(numberInput == 0)
    {
      toBePrinted[0] = '0';
    }
    else
    {
      toBePrinted[0] = '-';
    }
  }

  else if(nAllowedCharacters >= 1 && numberInput == 0)
  {
    if(nDecimalPlaces > 0)
    {
      nMeaningfulCharacters = nDecimalPlaces + 2;
    }
    else
    {
        nMeaningfulCharacters = 1;
    }

    nSpaces = nAllowedCharacters - nMeaningfulCharacters;

     if(nSpaces < 0)
    {
       nSpaces = 0;
    }

    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    if(shiftNumbersToRight && !shiftSignToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    toBePrinted[positionOfCharacter++] = '0';

    if(nDecimalPlaces > 0)
    {
      if(positionOfCharacter >= nAllowedCharacters)
      {


      	//Exit function and return false.

        return false;
      }
      toBePrinted[positionOfCharacter++] = '.';

      for(uint8_t i = 0; i < nDecimalPlaces ; i++)
      {
        if(positionOfCharacter > nAllowedCharacters)
        {
          toBePrinted[nAllowedCharacters - 1] = 'E';

          //Exit function and return false.

          return false;
        }
        else
        {
        toBePrinted[positionOfCharacter++] = '0';
        }
      }
    }
  }

  else if(nAllowedCharacters >= 1)
  {
    uint8_t nDigits;

    //3 for the numberInput, 1 for terminating null.
    char storeNumberTemporarily[5] = {};

    itoa( abs(numberInput), storeNumberTemporarily, 10 );

    nDigits = strlen(storeNumberTemporarily);

	if(nDecimalPlaces > 0)
	{
   	  if( (numberInput < 0) || (numberInput > 0 && displaySign == true) )
	  {
	    nMeaningfulCharacters = nDigits + nDecimalPlaces + 2;
	  }

	  else
	  {
	    nMeaningfulCharacters = nDigits + nDecimalPlaces + 1;
	  }
    }

	else
	{
      if( (numberInput < 0) || (numberInput > 0 && displaySign == true) )
      {
        nMeaningfulCharacters = nDigits + 1;
      }
      else
      {
        nMeaningfulCharacters = nDigits;
      }
    }

    nSpaces = nAllowedCharacters - nMeaningfulCharacters;

    if(nSpaces < 0)
    {
      nSpaces = 0;
    }

    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

	if(displaySign && numberInput > 0)
	{
	  toBePrinted[positionOfCharacter++] = '+';
	}

	else if(numberInput < 0)
	{
	  toBePrinted[positionOfCharacter++] = '-';
	}

	if(shiftNumbersToRight && !shiftSignToRight)
	{
	  for(uint8_t i = 0; i < nSpaces; i++)
	  {
	    toBePrinted[positionOfCharacter++] = ' ';
	  }
	}

	if(nAllowedCharacters >= nMeaningfulCharacters)
	{
	  for(uint8_t i = 0; i < nDigits ; i++)
	  {
	    toBePrinted[positionOfCharacter++] = storeNumberTemporarily[i];
	  }

	  if(nDecimalPlaces > 0)
	  {
	    toBePrinted[positionOfCharacter++] = '.';

	    for(uint8_t i = 0; i < nDecimalPlaces ; i++)
	    {
	        toBePrinted[positionOfCharacter++] = '0';
	    }
	  }
	}

    else
    {
      for(uint8_t i = 0; i < nDigits; i++)
      {
        if(positionOfCharacter >= nAllowedCharacters)
        {
        toBePrinted[nAllowedCharacters - 1] = 'E';

        //Exit function and return false.

        return false;
        }

        else
        {
           toBePrinted[positionOfCharacter++] = storeNumberTemporarily[i];
        }
      }

      if( nDecimalPlaces > 0  && positionOfCharacter >= nAllowedCharacters )
      {
       	toBePrinted[nAllowedCharacters - 1] = 'E';

       	//Exit function and return false.

        return false;
      }

      else if(nDecimalPlaces > 0)
      {
        toBePrinted[positionOfCharacter++] = '.';
      }

      for(uint8_t i = 0; i < nDecimalPlaces; i++ )
      {
        if( positionOfCharacter >= nAllowedCharacters )
        {
          toBePrinted[nAllowedCharacters - 1] = 'E';

          //Exit function and return false.

          return false;
        }
        else
        {
          toBePrinted[positionOfCharacter++] = '0';
        }
      }
    }
  }

  //Exit function and return true (Success).

  return true;
}



bool numToChar(uint16_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t nDecimalPlaces)
{
  uint8_t nMeaningfulCharacters;
  int8_t nSpaces = 0;
  uint8_t positionOfCharacter = 0;

  if( (nAllowedCharacters == 1) && displaySign )
  {
    if(numberInput > 0)
    {
      toBePrinted[0] = '+';
    }
    else if(numberInput == 0)
    {
      toBePrinted[0] = '0';
    }
    else
    {
      toBePrinted[0] = '-';
    }
  }

  else if(nAllowedCharacters >= 1 && numberInput == 0)
  {
    if(nDecimalPlaces > 0)
    {
      nMeaningfulCharacters = nDecimalPlaces + 2;
    }
    else
    {
        nMeaningfulCharacters = 1;
    }

    nSpaces = nAllowedCharacters - nMeaningfulCharacters;

    if(nSpaces < 0)
    {
       nSpaces = 0;
    }

    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++ )
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    if(shiftNumbersToRight && !shiftSignToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    toBePrinted[positionOfCharacter++] = '0';

    if(nDecimalPlaces > 0)
    {
      if(positionOfCharacter >= nAllowedCharacters)
      {
        toBePrinted[nAllowedCharacters - 1] = 'E';

        //Exit function and return false.

        return false;
      }
      toBePrinted[positionOfCharacter++] = '.';

      for(uint8_t i = 0; i < nDecimalPlaces; i++)
      {
        if(positionOfCharacter > nAllowedCharacters)
        {
          toBePrinted[nAllowedCharacters - 1] = 'E';

          //Exit function and return false.

          return false;
        }
        else
        {
        toBePrinted[positionOfCharacter++] = '0';
        }
      }
    }
  }

  else if(nAllowedCharacters >= 1)
  {
    uint8_t nDigits;

    //5 for the numberInput, 1 for terminating null.
    char storeNumberTemporarily[6] = {};

    utoa( abs(numberInput), storeNumberTemporarily, 10 );

    nDigits = strlen(storeNumberTemporarily);

	if(nDecimalPlaces > 0)
	{
   	  if( (numberInput < 0) || (numberInput > 0 && displaySign == true) )
	  {
	    nMeaningfulCharacters = nDigits + nDecimalPlaces + 2;
	  }

	  else
	  {
	    nMeaningfulCharacters = nDigits + nDecimalPlaces + 1;
	  }
    }

	else
	{
      if( (numberInput < 0) || (numberInput > 0 && displaySign == true) )
      {
        nMeaningfulCharacters = nDigits + 1;
      }
      else
      {
        nMeaningfulCharacters = nDigits;
      }
    }

    nSpaces = nAllowedCharacters - nMeaningfulCharacters;

    if(nSpaces < 0)
    {
      nSpaces = 0;
    }

    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

	if(displaySign && numberInput > 0)
	{
	  toBePrinted[positionOfCharacter++] = '+';
	}

	else if(numberInput < 0)
	{
	  toBePrinted[positionOfCharacter++] = '-';
	}

	if(shiftNumbersToRight && !shiftSignToRight)
	{
	  for(uint8_t i = 0; i < nSpaces; i++)
	  {
	    toBePrinted[positionOfCharacter++] = ' ';
	  }
	}

	if(nAllowedCharacters >= nMeaningfulCharacters)
	{
	  for(uint8_t i = 0; i < nDigits ; i++)
	  {
	    toBePrinted[positionOfCharacter++] = storeNumberTemporarily[i];
	  }

	  if(nDecimalPlaces > 0)
	  {
	    toBePrinted[positionOfCharacter++] = '.';

	    for(uint8_t i = 0; i < nDecimalPlaces ; i++)
	    {
	        toBePrinted[positionOfCharacter++] = '0';
	    }
	  }
	}

    else
    {
      for(uint8_t i = 0; i < nDigits; i++)
      {
        if(positionOfCharacter >= nAllowedCharacters)
        {
        toBePrinted[nAllowedCharacters - 1] = 'E';

        //Exit function and return false.

        return false;
        }

        else
        {
           toBePrinted[positionOfCharacter++] = storeNumberTemporarily[i];
        }
      }

      if( nDecimalPlaces > 0  && positionOfCharacter >= nAllowedCharacters )
      {
       	toBePrinted[nAllowedCharacters - 1] = 'E';

       	//Exit function and return false.

        return false;
      }

      else if(nDecimalPlaces > 0)
      {
        toBePrinted[positionOfCharacter++] = '.';
      }

      for(uint8_t i = 0; i < nDecimalPlaces; i++ )
      {
        if( positionOfCharacter >= nAllowedCharacters )
        {
          toBePrinted[nAllowedCharacters - 1] = 'E';

		  //Exit function and return false.

          return false;
        }
        else
        {
          toBePrinted[positionOfCharacter++] = '0';
        }
      }

    }
  }

  //Exit function and return true (Success).

  return true;
}



bool numToChar(uint32_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t nDecimalPlaces)
{
  uint8_t nMeaningfulCharacters;
  int8_t nSpaces = 0;
  uint8_t positionOfCharacter = 0;

  if( (nAllowedCharacters == 1) && displaySign )
  {
    if(numberInput > 0)
    {
      toBePrinted[0] = '+';
    }
    else if(numberInput == 0)
    {
      toBePrinted[0] = '0';
    }
    else
    {
      toBePrinted[0] = '-';
    }
  }

  else if(nAllowedCharacters >= 1 && numberInput == 0)
  {
    if(nDecimalPlaces > 0)
    {
      nMeaningfulCharacters = nDecimalPlaces + 2;
    }
    else
    {
        nMeaningfulCharacters = 1;
    }

    nSpaces = nAllowedCharacters - nMeaningfulCharacters;

     if(nSpaces < 0)
    {
       nSpaces = 0;
    }

    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    if(shiftNumbersToRight && !shiftSignToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    toBePrinted[positionOfCharacter++] = '0';

    if(nDecimalPlaces > 0)
    {
      if(positionOfCharacter >= nAllowedCharacters)
      {
        toBePrinted[nAllowedCharacters - 1] = 'E';

        //Exit function and return false.

        return false;
      }
      toBePrinted[positionOfCharacter++] = '.';

      for(uint8_t i = 0; i < nDecimalPlaces ; i++)
      {
        if(positionOfCharacter > nAllowedCharacters)
        {
          toBePrinted[nAllowedCharacters - 1] = 'E';

		  //Exit function and return false.

          return false;
        }
        else
        {
        toBePrinted[positionOfCharacter++] = '0';
        }
      }
    }
  }

  else if(nAllowedCharacters >= 1)
  {
    uint8_t nDigits;

    //10 for the numberInput, 1 for terminating null.
    char storeNumberTemporarily[11] = {};

    ultoa( abs(numberInput), storeNumberTemporarily, 10 );

    nDigits = strlen(storeNumberTemporarily);

	if(nDecimalPlaces > 0)
	{
   	  if( (numberInput < 0) || (numberInput > 0 && displaySign == true) )
	  {
	    nMeaningfulCharacters = nDigits + nDecimalPlaces + 2;
	  }

	  else
	  {
	    nMeaningfulCharacters = nDigits + nDecimalPlaces + 1;
	  }
    }

	else
	{
      if( (numberInput < 0) || (numberInput > 0 && displaySign == true) )
      {
        nMeaningfulCharacters = nDigits + 1;
      }
      else
      {
        nMeaningfulCharacters = nDigits;
      }
    }

    nSpaces = nAllowedCharacters - nMeaningfulCharacters;

    if(nSpaces < 0)
    {
      nSpaces = 0;
    }

    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

	if(displaySign && numberInput > 0)
	{
	  toBePrinted[positionOfCharacter++] = '+';
	}

	else if(numberInput < 0)
	{
	  toBePrinted[positionOfCharacter++] = '-';
	}

	if(shiftNumbersToRight && !shiftSignToRight)
	{
	  for(uint8_t i = 0; i < nSpaces; i++)
	  {
	    toBePrinted[positionOfCharacter++] = ' ';
	  }
	}

	if(nAllowedCharacters >= nMeaningfulCharacters)
	{
	  for(uint8_t i = 0; i < nDigits ; i++)
	  {
	    toBePrinted[positionOfCharacter++] = storeNumberTemporarily[i];
	  }

	  if(nDecimalPlaces > 0)
	  {
	    toBePrinted[positionOfCharacter++] = '.';

	    for(uint8_t i = 0; i < nDecimalPlaces ; i++)
	    {
	        toBePrinted[positionOfCharacter++] = '0';
	    }
	  }
	}

    else
    {
      for(uint8_t i = 0; i < nDigits; i++)
      {
        if(positionOfCharacter >= nAllowedCharacters)
        {
        toBePrinted[nAllowedCharacters - 1] = 'E';

        //Exit function and return false.

        return false;
        }

        else
        {
           toBePrinted[positionOfCharacter++] = storeNumberTemporarily[i];
        }
      }

      if( nDecimalPlaces > 0  && positionOfCharacter >= nAllowedCharacters )
      {
       	toBePrinted[nAllowedCharacters - 1] = 'E';

       	//Exit function and return false.

        return false;
      }

      else if(nDecimalPlaces > 0)
      {
        toBePrinted[positionOfCharacter++] = '.';
      }

      for(uint8_t i = 0; i < nDecimalPlaces; i++ )
      {
        if( positionOfCharacter >= nAllowedCharacters )
        {
          toBePrinted[nAllowedCharacters - 1] = 'E';

          //Exit function and return false.

          return false;
        }
        else
        {
          toBePrinted[positionOfCharacter++] = '0';
        }
      }
    }
  }

  //Exit function and return true (Success).

  return true;
}

}
