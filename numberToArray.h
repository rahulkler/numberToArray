#ifndef NUMBERTOARRAY_H
#define NUMBERTOARRAY_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class NumberToArray
{
public:
	static bool numberToArray(float numberInput, uint8_t nAllowedCharacters, char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	static bool numberToArray(double numberInput, uint8_t nAllowedCharacters, char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	static bool numberToArray(int8_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	static bool numberToArray(int16_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	static bool numberToArray(int32_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	static bool numberToArray(uint8_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	static bool numberToArray(uint16_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	static bool numberToArray(uint32_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
};

#endif // NUMBERTOARRAY_H
