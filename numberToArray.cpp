#include "numberToArray.h"

typedef union
{
  long L;
  float F;
} LF_t;

void NumberToArray::numberToArray(float numberInput, uint8_t nAllowedCharacters, char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace)
{
  //http://stackoverflow.com/questions/2305770/efficiency-arrays-vs-pointers - Ali
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
    if(noOfDecimalPlace > 0)
    {
      nMeaningfulCharacters = noOfDecimalPlace + 2;
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

//shift sign and numbers to the right
    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

//shifts number to right
    if(shiftNumbersToRight && !shiftSignToRight)
    {
      for(uint8_t i=0 ; i<nSpaces ; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    toBePrinted[positionOfCharacter++] = '0';

    if(noOfDecimalPlace > 0)
    {
      if(positionOfCharacter >= nAllowedCharacters)
      {
        toBePrinted[nAllowedCharacters-1] = 'E';
        return;
      }
      toBePrinted[positionOfCharacter++] = '.';

      for(int i=0; i<noOfDecimalPlace; i++)
      {
        if(positionOfCharacter > nAllowedCharacters)
        {
          toBePrinted[nAllowedCharacters-1] = 'E';
          return;
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

    //Round down the value of c to increase the product of division. This is done to reduce the effect of the inaccuracy of the float on the exponentInBaseTen.
    //Then, increment the value by 1.
    //Take 55 x 10^2.1 = 6924 (4 digits) ,55 x 10^2.7 = 27565 (5 digits) and 55 x 10^3 = 55000 (5 digits).
    //Since the exponent is casted to uint8_t, instead of 2.1 or 2.7, it will be stored as 2.
    //So we take 2+1 = 3 to ensure we always get either the exact number of digits, or an extra digit.

    //Assume 2^e = 10^f. We wish to obtain the value of f to find the number of digits we will allocate in dtostrf(). We know m from the exponent component of the float.
    //Let 10^f = (2^c)^f.
    //Hence, 2^e = 2^cf.
    //f = e/c
    //And, 2^c = 10. Hence, c = log(10) / log(2).
    nDigitsInCharacteristic = int8_t ( float(exponentInBaseTwo) / 3.32192809 ) + 2;

    if(nDigitsInCharacteristic <= 0)
    {
      nDigitsInCharacteristic = 1;
    }

  //The existence of the sign is ommited in the nMeaningfulCharacters because nMeaningfulCharacters will be used in dtostrf(), and the sign is dealt with later in the function, and not by dtostrf().
    if(noOfDecimalPlace > 0)
    {
      if( numberInput < 0 || (numberInput > 0 && displaySign == true) )
      {
        nMeaningfulCharacters = nDigitsInCharacteristic + noOfDecimalPlace + 2;
        existenceOfSign = 1;
      }
      else
      {
        nMeaningfulCharacters = nDigitsInCharacteristic + noOfDecimalPlace + 1;
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

    char storeNumber[nMeaningfulCharacters-existenceOfSign]  = {};

    dtostrf( abs(numberInput), nMeaningfulCharacters-existenceOfSign, noOfDecimalPlace, storeNumber );

  //An integer that will calculate the number of spaces in front

    if(storeNumber[0] == ' ')
    {
      nMeaningfulCharacters -= 1;
      index = 1;
    }

    //setting number of spaces
    nSpaces = (nAllowedCharacters - nMeaningfulCharacters);

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
      for( uint8_t i=0 ; i<nSpaces ; i++ )
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

//Transfer storeNumber into toBePrinted
    if(nMeaningfulCharacters <= nAllowedCharacters)
    {
      for(int i = 0 ; i < nMeaningfulCharacters ; i++)
      {
        toBePrinted[positionOfCharacter++] = storeNumber[index + i];
      }
    }

    else
    {
      for(int i = 0 ; i < nMeaningfulCharacters ; i++)
      {
         if(positionOfCharacter >= nAllowedCharacters)
         {
          toBePrinted[nAllowedCharacters -1] = 'E';
          return;
         }

         else
         {
          toBePrinted[positionOfCharacter++] = storeNumber[index + i];
         }
      }
    }

  }
}

void NumberToArray::numberToArray (double numberInput, uint8_t nAllowedCharacters, char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace)
{
  //http://stackoverflow.com/questions/2305770/efficiency-arrays-vs-pointers - Ali
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
    if(noOfDecimalPlace > 0)
    {
      nMeaningfulCharacters = noOfDecimalPlace + 2;
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

//shift sign and numbers to the right
    if(shiftSignToRight && shiftNumbersToRight)
    {
      for( uint8_t i = 0; i < nSpaces; i++ )
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

//shifts number to right
    if(shiftNumbersToRight && !shiftSignToRight)
    {
      for( uint8_t i=0 ; i<nSpaces ; i++ )
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    toBePrinted[positionOfCharacter++] = '0';

    if(noOfDecimalPlace > 0)
    {
      if(positionOfCharacter >= nAllowedCharacters)
      {
        toBePrinted[nAllowedCharacters - 1] = 'E';
        return;
      }
      toBePrinted[positionOfCharacter++] = '.';

      for(int i = 0 ; i < noOfDecimalPlace ; i++)
      {
        if(positionOfCharacter > nAllowedCharacters)
        {
          toBePrinted[nAllowedCharacters - 1] = 'E';
          return;
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

    //Round down the value of c to increase the product of division. This is done to reduce the effect of the inaccuracy of the float on the exponentInBaseTen.
    //Then, increment the value by 1.
    //Take 55 x 10^2.1 = 6924 (4 digits) ,55 x 10^2.7 = 27565 (5 digits) and 55 x 10^3 = 55000 (5 digits).
    //Since the exponent is casted to uint8_t, instead of 2.1 or 2.7, it will be stored as 2.
    //So we take 2+1 = 3 to ensure we always get either the exact number of digits, or an extra digit.

    //Assume 2^e = 10^f. We wish to obtain the value of f to find the number of digits we will allocate in dtostrf(). We know m from the exponent component of the float.
    //Let 10^f = (2^c)^f.
    //Hence, 2^e = 2^cf.
    //f = e/c
    //And, 2^c = 10. Hence, c = log(10) / log(2).
    nDigitsInCharacteristic = int8_t ( float(exponentInBaseTwo) / 3.32192809 ) + 2;

    if(nDigitsInCharacteristic <= 0)
    {
      nDigitsInCharacteristic = 1;
    }

  //The existence of the sign is ommited in the nMeaningfulCharacters because nMeaningfulCharacters will be used in dtostrf(), and the sign is dealt with later in the function, and not by dtostrf().
    if(noOfDecimalPlace > 0)
    {
      if( numberInput < 0 || (numberInput > 0 && displaySign == true))
      {
        nMeaningfulCharacters = nDigitsInCharacteristic + noOfDecimalPlace + 2;
        existenceOfSign = 1;
      }
      else
      {
        nMeaningfulCharacters = nDigitsInCharacteristic + noOfDecimalPlace + 1;
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

    char storeNumber[nMeaningfulCharacters - existenceOfSign]  = {};

    dtostrf( abs(numberInput), nMeaningfulCharacters - existenceOfSign, noOfDecimalPlace, storeNumber );

  //An integer that will calculate the number of spaces in front

    if(storeNumber[0] == ' ')
    {
      nMeaningfulCharacters -= 1;
      index = 1;
    }

    //setting number of spaces
    nSpaces = (nAllowedCharacters - nMeaningfulCharacters);

    if(nSpaces < 0)
     {
       nSpaces = 0;
     }

  //shift sign and numbers to the right
    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0 ; i < nSpaces ; i++)
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
      for(uint8_t i = 0 ; i < nSpaces ; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

//Transfer storeNumber into toBePrinted
    if(nMeaningfulCharacters <= nAllowedCharacters)
    {
      for(int i=0 ; i<nMeaningfulCharacters ; i++)
      {
        toBePrinted[positionOfCharacter++] = storeNumber[index + i];
      }
    }

    else
    {
      for(int i = 0 ; i < nMeaningfulCharacters ; i++)
      {
         if(positionOfCharacter >= nAllowedCharacters)
         {
          toBePrinted[nAllowedCharacters - 1] = 'E';
          return;
         }


         else
         {
          toBePrinted[positionOfCharacter++] = storeNumber[index + i];
         }
      }
    }

  }
}

void NumberToArray::numberToArray(int8_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace)
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
    if(noOfDecimalPlace > 0)
    {
      nMeaningfulCharacters = noOfDecimalPlace + 2;
    }
    else
    {
        nMeaningfulCharacters = 1;
    }

    nSpaces = nAllowedCharacters - nMeaningfulCharacters;

     if( nSpaces < 0 )
    {
       nSpaces = 0;
    }

//shift sign and numbers to the right
    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0 ; i < nSpaces ; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

//shifts number to right
    if(shiftNumbersToRight && !shiftSignToRight)
    {
      for(uint8_t i=0 ; i<nSpaces ; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    toBePrinted[positionOfCharacter++] = '0';

    if(noOfDecimalPlace > 0)
    {
      if(positionOfCharacter >= nAllowedCharacters)
      {
        toBePrinted[nAllowedCharacters-1] = 'E';
        return;
      }
      toBePrinted[positionOfCharacter++] = '.';

      for(int i=0 ; i<noOfDecimalPlace ; i++)
      {
        if(positionOfCharacter > nAllowedCharacters)
        {
          toBePrinted[nAllowedCharacters-1] = 'E';
          return;
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
    char storeNumber[4] = {};

    //Storing the numberInput into the array storeNumber.
    itoa( abs(numberInput), storeNumber, 10 );

    //Computing the number of digits (characteristic only).
    nDigits = strlen(storeNumber);


    //Computing the number of elements in the array that will store characters that have a true meaning. This is done considering the length of numberInput,
    //the number of decimal places, the existence of the sign and the existence of the decimal point.

      if(noOfDecimalPlace > 0)
      {
        if( (numberInput < 0) || (numberInput > 0 && displaySign == true) )
        {
          nMeaningfulCharacters = nDigits + noOfDecimalPlace + 2;
        }
        else
        {
          nMeaningfulCharacters = nDigits + noOfDecimalPlace + 1;
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

    //setting number of spaces
      nSpaces = nAllowedCharacters - nMeaningfulCharacters;

      if(nSpaces < 0)
       {
         nSpaces = 0;
       }

    //shift sign and numbers to the right
      if(shiftSignToRight && shiftNumbersToRight)
      {
        for(uint8_t i = 0 ; i < nSpaces ; i++)
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
        for(uint8_t i=0 ; i<nSpaces ; i++)
        {
          toBePrinted[positionOfCharacter++] = ' ';
        }
      }

//If the number of characters that are meaningful is lesser than or equal to the number of allowed characters, immediately output the characteristic, decimal point and
//mantissa without checking for error.

       if(nAllowedCharacters >= nMeaningfulCharacters)
      {
        for(uint8_t i = 0 ; i < nDigits ; i++)
        {
          toBePrinted[positionOfCharacter++] = storeNumber[i];
        }
        if(noOfDecimalPlace > 0)
        {
          toBePrinted[positionOfCharacter++] = '.';

          for(uint8_t i = 0 ; i < noOfDecimalPlace ; i++)
          {
              toBePrinted[positionOfCharacter++] = '0';
          }
        }
      }

//Otherwise, check for error before adding the characteristic, and again before outputting the decimal point, and again before adding the mantissa to the array.

      else
      {
        for(uint8_t i = 0 ; i < nDigits ; i++)
        {
          if(positionOfCharacter > nAllowedCharacters - 1)
          {
           toBePrinted[nAllowedCharacters - 1] = 'E';
           return;
          }
          else
          {
           toBePrinted[positionOfCharacter++] = storeNumber[i];
          }
        }

        if(noOfDecimalPlace > 0  && positionOfCharacter > (nAllowedCharacters - 1) )
        {
         toBePrinted[nAllowedCharacters - 1] = 'E';
         return;
        }

        else if(noOfDecimalPlace > 0)
        {
         toBePrinted[positionOfCharacter++] = '.';
        }

        for(uint8_t i = 0; i < noOfDecimalPlace; i++ )
        {
           if( positionOfCharacter > nAllowedCharacters-1 )
           {
            toBePrinted[nAllowedCharacters -1] = 'E';
            return;
           }
           else
           {
            toBePrinted[positionOfCharacter++] = '0';
           }
        }

      }
   }
}


void NumberToArray::numberToArray (int16_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace)
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
    if(noOfDecimalPlace > 0)
    {
      nMeaningfulCharacters = noOfDecimalPlace + 2;
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

//shift sign and numbers to the right
    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

//shifts number to right
    if(shiftNumbersToRight && !shiftSignToRight)
    {
      for(uint8_t i=0; i<nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    toBePrinted[positionOfCharacter++] = '0';

    if(noOfDecimalPlace > 0)
    {
      if(positionOfCharacter >= nAllowedCharacters)
      {
        toBePrinted[nAllowedCharacters-1] = 'E';
        return;
      }
      toBePrinted[positionOfCharacter++] = '.';

      for(int i=0 ; i<noOfDecimalPlace ; i++)
      {
        if(positionOfCharacter > nAllowedCharacters)
        {
          toBePrinted[nAllowedCharacters-1] = 'E';
          return;
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
    char storeNumber[6] = {};

    //Storing the numberInput into the array storeNumber.
    itoa( abs(numberInput), storeNumber, 10 );

    //Computing the number of digits (characteristic only).
    nDigits = strlen(storeNumber);


    //Computing the number of elements in the array that will store characters that have a true meaning. This is done considering the length of numberInput,
    //the number of decimal places, the existence of the sign and the existence of the decimal point.

      if(noOfDecimalPlace > 0)
      {
        if( (numberInput < 0) || (numberInput > 0 && displaySign == true) )
        {
          nMeaningfulCharacters = nDigits + noOfDecimalPlace + 2;
        }
        else
        {
          nMeaningfulCharacters = nDigits + noOfDecimalPlace + 1;
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


    //setting number of spaces
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
        for(uint8_t i=0; i<nSpaces; i++)
        {
          toBePrinted[positionOfCharacter++] = ' ';
        }
      }

//If the number of characters that are meaningful is lesser than or equal to the number of allowed characters, immediately output the characteristic, decimal point and
//mantissa without checking for error.

       if(nAllowedCharacters >= nMeaningfulCharacters)
      {
        for(uint8_t i = 0; i < nDigits ; i++)
        {
          toBePrinted[positionOfCharacter++] = storeNumber[i];
        }
        if(noOfDecimalPlace > 0)
        {
          toBePrinted[positionOfCharacter++] = '.';

          for(uint8_t i = 0; i < noOfDecimalPlace; i++)
          {
              toBePrinted[positionOfCharacter++] = '0';
          }
        }
      }

//Otherwise, check for error before adding the characteristic, and again before outputting the decimal point, and again before adding the mantissa to the array.

      else
      {
        for(uint8_t i = 0; i < nDigits; i++)
        {
          if(positionOfCharacter > nAllowedCharacters - 1)
          {
           toBePrinted[nAllowedCharacters -1] = 'E';
           return;
          }
          else
          {
           toBePrinted[positionOfCharacter++] = storeNumber[i];
          }
        }

        if( noOfDecimalPlace > 0  && positionOfCharacter > (nAllowedCharacters - 1) )
        {
         toBePrinted[nAllowedCharacters - 1] = 'E';
         return;
        }

        else if(noOfDecimalPlace > 0)
        {
         toBePrinted[positionOfCharacter++] = '.';
        }

        for(uint8_t i = 0; i < noOfDecimalPlace; i++)
        {
           if(positionOfCharacter > nAllowedCharacters - 1)
           {
            toBePrinted[nAllowedCharacters - 1] = 'E';
            return;
           }
           else
           {
            toBePrinted[positionOfCharacter++] = '0';
           }
        }

      }
  }
}


void NumberToArray::numberToArray (int32_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace)
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
    if(noOfDecimalPlace > 0)
    {
      nMeaningfulCharacters = noOfDecimalPlace + 2;
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

//shift sign and numbers to the right
    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

//shifts number to right
    if(shiftNumbersToRight && !shiftSignToRight)
    {
      for(uint8_t i=0 ; i<nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    toBePrinted[positionOfCharacter++] = '0';

    if(noOfDecimalPlace > 0)
    {
      if(positionOfCharacter >= nAllowedCharacters)
      {
        toBePrinted[nAllowedCharacters-1] = 'E';
        return;
      }
      toBePrinted[positionOfCharacter++] = '.';

      for(int i=0 ; i<noOfDecimalPlace ; i++)
      {
        if(positionOfCharacter > nAllowedCharacters)
        {
          toBePrinted[nAllowedCharacters-1] = 'E';
          return;
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
    char storeNumber[11] = {};

    //Storing the numberInput into the array storeNumber.
    ltoa( abs(numberInput), storeNumber, 10 );

    //Computing the number of digits (characteristic only).
    nDigits = strlen(storeNumber);


    //Computing the number of elements in the array that will store characters that have a true meaning. This is done considering the length of numberInput,
    //the number of decimal places, the existence of the sign and the existence of the decimal point.

      if(noOfDecimalPlace > 0)
      {
        if( (numberInput < 0) || (numberInput > 0 && displaySign == true) )
        {
          nMeaningfulCharacters = nDigits + noOfDecimalPlace + 2;
        }
        else
        {
          nMeaningfulCharacters = nDigits + noOfDecimalPlace + 1;
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

    //setting number of spaces
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
        for( int8_t i=0; i<nSpaces; i++)
        {
          toBePrinted[positionOfCharacter++] = ' ';
        }
      }

//If the number of characters that are meaningful is lesser than or equal to the number of allowed characters, immediately output the characteristic, decimal point and
//mantissa without checking for error.

       if(nAllowedCharacters >= nMeaningfulCharacters)
      {
        for(uint8_t i = 0; i < nDigits; i++)
        {
          toBePrinted[positionOfCharacter++] = storeNumber[i];
        }
        if(noOfDecimalPlace > 0)
        {
          toBePrinted[positionOfCharacter++] = '.';

          for(uint8_t i = 0; i < noOfDecimalPlace; i++)
          {
              toBePrinted[positionOfCharacter++] = '0';
          }
        }
      }

//Otherwise, check for error before adding the characteristic, and again before outputting the decimal point, and again before adding the mantissa to the array.

      else
      {
        for(uint8_t i = 0; i < nDigits; i++)
        {
          if(positionOfCharacter > nAllowedCharacters - 1)
          {
           toBePrinted[nAllowedCharacters - 1] = 'E';
           return;
          }
          else
          {
           toBePrinted[positionOfCharacter++] = storeNumber[i];
          }
        }

        if( noOfDecimalPlace > 0  && positionOfCharacter > (nAllowedCharacters - 1) )
        {
         toBePrinted[nAllowedCharacters - 1] = 'E';
         return;
        }

        else if(noOfDecimalPlace > 0)
        {
         toBePrinted[positionOfCharacter++] = '.';
        }

        for(uint8_t i = 0; i < noOfDecimalPlace; i++)
        {
           if( positionOfCharacter > nAllowedCharacters - 1 )
           {
            toBePrinted[nAllowedCharacters - 1] = 'E';
            return;
           }
           else
           {
            toBePrinted[positionOfCharacter++] = '0';
           }
        }
      }
  }
}

void NumberToArray::numberToArray (uint8_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace)
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
    if(noOfDecimalPlace > 0)
    {
      nMeaningfulCharacters = noOfDecimalPlace + 2;
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

//shift sign and numbers to the right
    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

//shifts number to right
    if(shiftNumbersToRight && !shiftSignToRight)
    {
      for( uint8_t i=0 ; i<nSpaces ; i++ )
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    toBePrinted[positionOfCharacter++] = '0';

    if(noOfDecimalPlace > 0)
    {
      if(positionOfCharacter >= nAllowedCharacters)
      {
        toBePrinted[nAllowedCharacters-1] = 'E';
        return;
      }
      toBePrinted[positionOfCharacter++] = '.';

      for(int i=0 ; i<noOfDecimalPlace ; i++)
      {
        if(positionOfCharacter > nAllowedCharacters)
        {
          toBePrinted[nAllowedCharacters-1] = 'E';
          return;
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
    char storeNumber[5] = {};

    //Storing the numberInput into the array storeNumber.
    utoa( abs(numberInput), storeNumber, 10 );

    //Computing the number of digits (characteristic only).
    nDigits = strlen(storeNumber);


    //Computing the number of elements in the array that will store characters that have a true meaning. This is done considering the length of numberInput,
    //the number of decimal places, the existence of the sign and the existence of the decimal point.

      if(noOfDecimalPlace > 0)
      {
        if( (numberInput < 0) || (numberInput > 0 && displaySign == true) )
        {
          nMeaningfulCharacters = nDigits + noOfDecimalPlace + 2;
        }
        else
        {
          nMeaningfulCharacters = nDigits + noOfDecimalPlace + 1;
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

    //setting number of spaces
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
        for( uint8_t i=0 ; i<nSpaces ; i++ )
        {
          toBePrinted[positionOfCharacter++] = ' ';
        }
      }

//If the number of characters that are meaningful is lesser than or equal to the number of allowed characters, immediately output the characteristic, decimal point and
//mantissa without checking for error.

       if(nAllowedCharacters >= nMeaningfulCharacters)
      {
        for(uint8_t i = 0; i < nDigits; i++)
        {
          toBePrinted[positionOfCharacter++] = storeNumber[i];
        }
        if(noOfDecimalPlace > 0)
        {
          toBePrinted[positionOfCharacter++] = '.';

          for(uint8_t i = 0; i < noOfDecimalPlace; i++)
          {
              toBePrinted[positionOfCharacter++] = '0';
          }
        }
      }

//Otherwise, check for error before adding the characteristic, and again before outputting the decimal point, and again before adding the mantissa to the array.

      else
      {
        for(uint8_t i = 0; i < nDigits; i++)
        {
          if(positionOfCharacter > nAllowedCharacters - 1)
          {
           toBePrinted[nAllowedCharacters -1] = 'E';
           return;
          }
          else
          {
           toBePrinted[positionOfCharacter++] = storeNumber[i];
          }
        }

        if(noOfDecimalPlace > 0  && positionOfCharacter > (nAllowedCharacters - 1) )
        {
         toBePrinted[nAllowedCharacters - 1] = 'E';
         return;
        }

        else if(noOfDecimalPlace > 0)
        {
         toBePrinted[positionOfCharacter++] = '.';
        }

        for(uint8_t i = 0; i < noOfDecimalPlace; i++)
        {
           if(positionOfCharacter > nAllowedCharacters - 1)
           {
            toBePrinted[nAllowedCharacters - 1] = 'E';
            return;
           }
           else
           {
            toBePrinted[positionOfCharacter++] = '0';
           }
        }

      }
  }
}


void NumberToArray::numberToArray(uint16_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace)
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
    if(noOfDecimalPlace > 0)
    {
      nMeaningfulCharacters = noOfDecimalPlace + 2;
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

//shift sign and numbers to the right
    if(shiftSignToRight && shiftNumbersToRight)
    {
      for( uint8_t i = 0; i < nSpaces; i++ )
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

//shifts number to right
    if(shiftNumbersToRight && !shiftSignToRight)
    {
      for(uint8_t i=0; i<nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    toBePrinted[positionOfCharacter++] = '0';

    if(noOfDecimalPlace > 0)
    {
      if(positionOfCharacter >= nAllowedCharacters)
      {
        toBePrinted[nAllowedCharacters-1] = 'E';
        return;
      }
      toBePrinted[positionOfCharacter++] = '.';

      for(int i=0; i<noOfDecimalPlace; i++)
      {
        if(positionOfCharacter > nAllowedCharacters)
        {
          toBePrinted[nAllowedCharacters - 1] = 'E';
          return;
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
    char storeNumber[6] = {};

    //Storing the numberInput into the array storeNumber.
    utoa( abs(numberInput), storeNumber, 10 );

    //Computing the number of digits (characteristic only).
    nDigits = strlen(storeNumber);


    //Computing the number of elements in the array that will store characters that have a true meaning. This is done considering the length of numberInput,
    //the number of decimal places, the existence of the sign and the existence of the decimal point.

      if(noOfDecimalPlace > 0)
      {
        if( (numberInput < 0) || (numberInput > 0 && displaySign == true) )
        {
          nMeaningfulCharacters = nDigits + noOfDecimalPlace + 2;
        }
        else
        {
          nMeaningfulCharacters = nDigits + noOfDecimalPlace + 1;
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

    //setting number of spaces
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
        for( uint8_t i=0; i<nSpaces; i++)
        {
          toBePrinted[positionOfCharacter++] = ' ';
        }
      }

//If the number of characters that are meaningful is lesser than or equal to the number of allowed characters, immediately output the characteristic, decimal point and
//mantissa without checking for error.

       if(nAllowedCharacters >= nMeaningfulCharacters)
      {
        for(uint8_t i = 0; i < nDigits; i++)
        {
          toBePrinted[positionOfCharacter++] = storeNumber[i];
        }
        if(noOfDecimalPlace > 0)
        {
          toBePrinted[positionOfCharacter++] = '.';

          for(uint8_t i = 0 ; i < noOfDecimalPlace ; i++)
          {
              toBePrinted[positionOfCharacter++] = '0';
          }
        }
      }

//Otherwise, check for error before adding the characteristic, and again before outputting the decimal point, and again before adding the mantissa to the array.

      else
      {
        for(uint8_t i = 0; i < nDigits; i++)
        {
          if(positionOfCharacter > nAllowedCharacters - 1)
          {
           toBePrinted[nAllowedCharacters - 1] = 'E';
           return;
          }
          else
          {
           toBePrinted[positionOfCharacter++] = storeNumber[i];
          }
        }

        if(noOfDecimalPlace > 0  && positionOfCharacter > (nAllowedCharacters - 1) )
        {
         toBePrinted[nAllowedCharacters - 1] = 'E';
         return;
        }

        else if(noOfDecimalPlace > 0)
        {
         toBePrinted[positionOfCharacter++] = '.';
        }

        for(uint8_t i = 0; i < noOfDecimalPlace; i++)
        {
           if(positionOfCharacter > nAllowedCharacters - 1)
           {
            toBePrinted[nAllowedCharacters - 1] = 'E';
            return;
           }
           else
           {
            toBePrinted[positionOfCharacter++] = '0';
           }
        }

      }
  }
}

void NumberToArray::numberToArray(uint32_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace)
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
    if(noOfDecimalPlace > 0)
    {
      nMeaningfulCharacters = noOfDecimalPlace + 2;
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

//shift sign and numbers to the right
    if(shiftSignToRight && shiftNumbersToRight)
    {
      for(uint8_t i = 0; i < nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

//shifts number to right
    if(shiftNumbersToRight && !shiftSignToRight)
    {
      for(uint8_t i=0 ; i<nSpaces; i++)
      {
        toBePrinted[positionOfCharacter++] = ' ';
      }
    }

    toBePrinted[positionOfCharacter++] = '0';

    if(noOfDecimalPlace > 0)
    {
      if(positionOfCharacter >= nAllowedCharacters)
      {
        toBePrinted[nAllowedCharacters-1] = 'E';
        return;
      }
      toBePrinted[positionOfCharacter++] = '.';

      for(int i=0 ; i<noOfDecimalPlace ; i++)
      {
        if(positionOfCharacter > nAllowedCharacters)
        {
          toBePrinted[nAllowedCharacters-1] = 'E';
          return;
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
    char storeNumber[11] = {};

    //Storing the numberInput into the array storeNumber.
    ultoa( abs(numberInput), storeNumber, 10 );

    //Computing the number of digits (characteristic only).
    nDigits = strlen(storeNumber);


    //Computing the number of elements in the array that will store characters that have a true meaning. This is done considering the length of numberInput,
    //the number of decimal places, the existence of the sign and the existence of the decimal point.

      if(noOfDecimalPlace > 0)
      {
        if( (numberInput < 0) || (numberInput > 0 && displaySign == true) )
        {
          nMeaningfulCharacters = nDigits + noOfDecimalPlace + 2;
        }
        else
        {
          nMeaningfulCharacters = nDigits + noOfDecimalPlace + 1;
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

    //setting number of spaces
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
        for( int8_t i=0; i<nSpaces; i++)
        {
          toBePrinted[positionOfCharacter++] = ' ';
        }
      }

//If the number of characters that are meaningful is lesser than or equal to the number of allowed characters, immediately output the characteristic, decimal point and
//mantissa without checking for error.

       if(nAllowedCharacters >= nMeaningfulCharacters)
      {
        for(uint8_t i = 0; i < nDigits; i++)
        {
          toBePrinted[positionOfCharacter++] = storeNumber[i];
        }
        if(noOfDecimalPlace > 0)
        {
          toBePrinted[positionOfCharacter++] = '.';

          for(uint8_t i = 0; i < noOfDecimalPlace; i++)
          {
              toBePrinted[positionOfCharacter++] = '0';
          }
        }
      }

//Otherwise, check for error before adding the characteristic, and again before outputting the decimal point, and again before adding the mantissa to the array.

      else
      {
        for(uint8_t i = 0; i < nDigits; i++)
        {
          if(positionOfCharacter > nAllowedCharacters - 1)
          {
           toBePrinted[nAllowedCharacters - 1] = 'E';
           return;
          }
          else
          {
           toBePrinted[positionOfCharacter++] = storeNumber[i];
          }
        }

        if( noOfDecimalPlace > 0  && positionOfCharacter > (nAllowedCharacters - 1) )
        {
         toBePrinted[nAllowedCharacters - 1] = 'E';
         return;
        }

        else if(noOfDecimalPlace > 0)
        {
         toBePrinted[positionOfCharacter++] = '.';
        }

        for(uint8_t i = 0; i < noOfDecimalPlace; i++)
        {
           if( positionOfCharacter > nAllowedCharacters - 1 )
           {
            toBePrinted[nAllowedCharacters - 1] = 'E';
            return;
           }
           else
           {
            toBePrinted[positionOfCharacter++] = '0';
           }
        }
      }
  }
}
