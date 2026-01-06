// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_MACRO_UTILS_HPP
#define OKUTILS_MACRO_UTILS_HPP

/**
 * Avoids errors in certain situations when using macros that
 * expand to multiple lines of code or if statements.
 * Also forces the use of a semicolon at the end of the macro.
 * @param ... The code of the macro.
 */
#define OKL_MULTILINE_MACRO(...) \
	if constexpr (true) { \
		__VA_ARGS__ \
	} \
	else \
		OKL_DO_NOTHING()

/**
 * Turns a preprocessor token into a real string.
 * The preprocessor stringify operator (#) prevents arguments from expanding,
 * this macro allows its argument to expand before it is turned into a string.
 * @param x The preprocessor token to turn into a string.
 */
#define OKL_STRINGIFY(...) OKL_PRIVATE_STRINGIFY(__VA_ARGS__)
#define OKL_PRIVATE_STRINGIFY(...) #__VA_ARGS__

/**
 * Concatenates two preprocessor tokens.
 * The preprocessor concatenation operator (##) prevents arguments from expanding,
 * this macro allows its arguments to expand before they are concatenated.
 * @param x The first preprocessor token to concatenate.
 * @param y The second preprocessor token to concatenate.
 */
#define OKL_CONCAT(x, y) OKL_PRIVATE_CONCAT(x, y)
#define OKL_PRIVATE_CONCAT(x, y) x ## y

/**
 * Expands macro arguments before they are used.
 * @param ... The arguments to expand.
 */
#define OKL_EXPAND(...) __VA_ARGS__

/** Expands to nothing. */
#define OKL_EMPTY()

/** Expands to a statement that does nothing. */
#define OKL_DO_NOTHING() (void)0

/**
 * Expands to the variadic argument at the specified index.
 * @param index The index of the argument to retrieve, can be larger than the size of the variadic. [>= 0, <= 9]
 * @param ... The variadic arguments to retrieve from.
 * @warning 'VA_AT_X' versions require the va to be at least X in size.
 */
#define OKL_VA_AT(index, ...) OKL_EXPAND(OKL_CONCAT(OKL_VA_AT_, index)(__VA_ARGS__,,,,,,,,))
#define OKL_VA_AT_0(a0, ...) a0
#define OKL_VA_AT_1(a0, a1, ...) a1
#define OKL_VA_AT_2(a0, a1, a2, ...) a2
#define OKL_VA_AT_3(a0, a1, a2, a3, ...) a3
#define OKL_VA_AT_4(a0, a1, a2, a3, a4, ...) a4
#define OKL_VA_AT_5(a0, a1, a2, a3, a4, a5, ...) a5
#define OKL_VA_AT_6(a0, a1, a2, a3, a4, a5, a6, ...) a6
#define OKL_VA_AT_7(a0, a1, a2, a3, a4, a5, a6, a7, ...) a7
#define OKL_VA_AT_8(a0, a1, a2, a3, a4, a5, a6, a7, a8, ...) a8
#define OKL_VA_AT_9(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, ...) a9

/**
 * Consumes the specified number of variadic arguments.
 * @param numToConsume The number of arguments to remove. [>= 0, <= va_size]
 * @param ... The variadic arguments to consume from.
 * @warning 'VA_CONSUME_X' versions require the va to be at least X in size.
 */
#define OKL_VA_CONSUME(numToConsume, ...) OKL_EXPAND(OKL_CONCAT(OKL_VA_CONSUME_, numToConsume)(__VA_ARGS__))
#define OKL_VA_CONSUME_0(...) __VA_ARGS__
#define OKL_VA_CONSUME_1(a0, ...) __VA_ARGS__
#define OKL_VA_CONSUME_2(a0, a1, ...) __VA_ARGS__
#define OKL_VA_CONSUME_3(a0, a1, a2, ...) __VA_ARGS__
#define OKL_VA_CONSUME_4(a0, a1, a2, a3, ...) __VA_ARGS__
#define OKL_VA_CONSUME_5(a0, a1, a2, a3, a4, ...) __VA_ARGS__
#define OKL_VA_CONSUME_6(a0, a1, a2, a3, a4, a5, ...) __VA_ARGS__
#define OKL_VA_CONSUME_7(a0, a1, a2, a3, a4, a5, a6, ...) __VA_ARGS__
#define OKL_VA_CONSUME_8(a0, a1, a2, a3, a4, a5, a6, a7, ...) __VA_ARGS__
#define OKL_VA_CONSUME_9(a0, a1, a2, a3, a4, a5, a6, a7, a8, ...) __VA_ARGS__

/**
 * Expands to the number of variadic arguments.
 * @param ... The variadic arguments to count. [va_size >= 0, va_size <= 10]
 */
#define OKL_VA_SIZE(...) OKL_PRIVATE_VA_COUNT(__VA_OPT__(__VA_ARGS__,) 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define OKL_PRIVATE_VA_COUNT(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, num, ...) num

/**
 * Expands to the last index of the variadic arguments, or nothing if va args are empty.
 * @param ... The variadic arguments to get the last index of. [va_size >= 0, va_size <= 10]
 */
#define OKL_VA_LAST_INDEX(...) __VA_OPT__(OKL_PRIVATE_VA_COUNT(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

/**
 * Expands to the last variadic argument.
 * @param ... The variadic arguments to get the last of. [va_size >= 0, va_size <= 10]
 */
#define OKL_VA_LAST(...) __VA_OPT__(OKL_VA_AT(OKL_VA_LAST_INDEX(__VA_ARGS__), __VA_ARGS__))

#define OKL_VA_OPT(value, ...) __VA_OPT__(value)

#define OKL_VA_COMMA(...) __VA_OPT__(,) __VA_ARGS__

/**
 * Checks if the provided variadic is empty.
 * Expands to 1 if the variadic is empty, otherwise 0.
 */
#define OKL_IS_EMPTY(...) OKL_VA_AT_1(OKL_EMPTY() __VA_OPT__(,) 0, 1)

/**
 * Returns the first argument if the condition is true, otherwise the second argument.
 * @param condition The condition to check, must expand to 0 or 1.
 * @param trueValue The value to return if the condition is 1.
 * @param falseValue The value to return if the condition is 0.
 */
#define OKL_IF(condition, trueValue, falseValue) OKL_EXPAND(OKL_CONCAT(OKL_PRIVATE_IF_, condition)(trueValue, falseValue))
#define OKL_PRIVATE_IF_0(trueValue, falseValue) falseValue
#define OKL_PRIVATE_IF_1(trueValue, falseValue) trueValue

#endif
