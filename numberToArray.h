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
	static void numberToArray(float numberInput, uint8_t nAllowedCharacters, char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	static void numberToArray(double numberInput, uint8_t nAllowedCharacters, char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	static void numberToArray(int8_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	static void numberToArray(int16_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	static void numberToArray(int32_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	static void numberToArray(uint8_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	static void numberToArray(uint16_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	static void numberToArray(uint32_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
};

#endif // NUMBERTOARRAY_H
