#ifndef NUMTOCHARARRAY_H
#define NUMTOCHARARRAY_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

namespace numtochararray
{
	bool numToChar(float numberInput, uint8_t nAllowedCharacters, char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	bool numToChar(double numberInput, uint8_t nAllowedCharacters, char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	bool numToChar(int8_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	bool numToChar(int16_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	bool numToChar(int32_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	bool numToChar(uint8_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	bool numToChar(uint16_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
	bool numToChar(uint32_t numberInput, uint8_t nAllowedCharacters , char toBePrinted[], bool displaySign, bool shiftNumbersToRight, bool shiftSignToRight, uint8_t noOfDecimalPlace);
};

#endif // NUMTOCHARARRAY_H
