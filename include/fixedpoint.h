/**
 * @file   fixedpoint.h
 * @author Michal Pochobradský
 * @author Tibor Zauko
 * @brief Single include implementation of fixedpoint arithmetic library.
 */
// TO DO list:
// Fix multiplication
// Implement long division for div_or_mod
// Implement general power calculations
// Implement a more memory friendly _number struct


//          Copyright Michal Pochobradský 2016.
//          Copyright Tibor Zauko 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef FIXEDPOINT_H
#define FIXEDPOINT_H
#include <iostream> // cerr, clog
#include <string>
#include <vector> // in convert_through_native
#include <type_traits> // SFINAE
#include <cmath> // floor, ceil
#include <stdexcept> // runtime_exception
#include <algorithm> // any, reverse, reverse_copy
#include <iterator> // back_inserter
#include <ostream> // operator << definition
#include <istream> // operator >> definition
#include <sstream> // in str() func

#if ! ( defined(FIXEDPOINT_CASE_SENSITIVE) || defined(FIXEDPOINT_CASE_INSENSITIVE) )
#define FIXEDPOINT_CASE_INSENSITIVE
#endif

namespace fixedpoint{
/**
 * @brief The _number struct stores the values of the number's whole and fractional part
 */
template<unsigned char max_digit_value>
struct _number{

    _number(){}

    _number(std::size_t newSize, int value){
        _data.resize(newSize,value);
    }

    _number(const _number &) = default;
    _number(_number &&) = default;
    _number & operator=(const _number &) = default;
    _number & operator=(_number &&) = default;

    /**
     * @brief get returns a digit of the stored number
     * @param index determines which digit is returned
     * @return value of the requested digit, 0 if nonexistent
     */
    int get(std::size_t index) const{
        return (index<_data.size()?_data[index]:0);
    }
    /**
     * @brief set sets a digit of number to value
     *
     * If specified digit does not exist, it creates a place for it
     *
     * @param index determines which digit to set
     * @param what  what value to set the choosen digit
     */
    void set(std::size_t index, int what){
        if(index >= _data.size()) {
            _data.resize(index,0);
            _data.push_back(what);
        }
        else{
            _data[index] = what;
        }
    }

    std::size_t size() const{
        return _data.size();
    }
    void resize(std::size_t newSize, int def_value){
        _data.resize(newSize,def_value);
    }
    void clear(){
        _data.clear();
    }

    bool empty() const{
        return _data.empty();
    }
    bool all_of(int value) const{
        return std::all_of(_data.cbegin(),
                           _data.cend(),
                           [value](const int x){
            return x==value;
        });
    }

    void push(int value){
        _data.push_back(value);
    }

    int pop(){
        int rval = _data.back();
        _data.pop_back();
        return rval;
    }

    int back() const{
        return _data.back();
    }
    //int front() const;

    void swap(_number & o){
        _data.swap(o._data);
    }

private:
    // _data;
    std::vector<uint8_t> _data;
};

#if defined( FIXEDPOINT_CASE_INSENSITIVE ) && ! defined( FIXEDPOINT_CASE_SENSITIVE )
#define MAX_RADIX (36)
// str -> value, value -> str translation tables:
/**
 * @brief Valuations of digits
 *
 * -1 means invalid digit
 * -2 means radix point
 */
static const signed char values[128] = {
//   0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x1
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -1, -2, -1, // 0x2
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1, // 0x3
    -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, // 0x4
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1, // 0x5
    -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, // 0x6
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1  // 0x7
};
// -2: desatinne oddelovace (. a ,), -1: neplatna cifra,
// ostatne: hodnota cislice, ak >= sustave - neplatna

/**
 * @brief Digits belonging to values
 */
static const char digits[36] = {
//   0    1    2    3    4    5    6    7    8    9
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', // 0
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', // 1
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', // 2
    'u', 'v', 'w', 'x', 'y', 'z'                      // 3
};
// mapovanie hodnota -> cislica
#elif defined( FIXEDPOINT_CASE_SENSITIVE ) && ! defined( FIXEDPOINT_CASE_INSENSITIVE )
#define MAX_RADIX (64)
/**
 * @brief Valuations of digits
 *
 * -1 means invalid digit
 * -2 means radix point
 */
static const signed char values[128] = {
//   0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x1
    -1, -1, -1, 62, 63, -1, -1, -1, -1, -1, -1, -1, -2, -1, -2, -1, // 0x2
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1, // 0x3
    -1, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, // 0x4
    51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, // 0x5
    -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, // 0x6
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1  // 0x7
};

/**
 * @brief Digits belonging to values
 */
static const char digits[64] = {
//   0    1    2    3    4    5    6    7    8    9
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', // 0
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', // 1
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', // 2
    'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', // 3
    'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', // 4
    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', // 5
    'Y', 'Z', '#', '$'                                // 6
};
#else
#error "FIXEDPOINT_H: Conflicting case sensitivity setting, please choose only one"
#endif

// Exceptions:

/**
 * @brief The invalid_number_format struct is an exception related to number struct
 *
 * If this exception is thrown, it means that a string supposed to encode
 * a number is formatted incorrectly. The reason is further specified in
 * the what(method).
 */
struct invalid_number_format: public std::runtime_error{
    invalid_number_format(const char * what):std::runtime_error(what){}
    invalid_number_format(const std::string & what):std::runtime_error(what){}
};

/**
 * @brief The invalid_expression_format struct is an exception related to number struct
 *
 * If this exception is thrown, it means that a string supposed to encode
 * a postfix or infix expression is formatted incorrectly.
 * The reason is further specified in the what(method).
 */
struct invalid_expression_format: public std::runtime_error{
    invalid_expression_format(const char * what):std::runtime_error(what){}
    invalid_expression_format(const std::string & what):std::runtime_error(what){}
};

/**
 * @brief The radix_too_high struct is an exception related to number struct
 *
 * If this exception is thrown, it means that a string supposed to encode
 * a number specifies a radix that is out of the supported range of radixes.
 */
struct radix_invalid: public std::runtime_error{
    radix_invalid():std::runtime_error("the specified radix is too high or lower than 2"){}
};

/**
 * @brief The radix_too_high struct is an exception related to number struct
 *
 * If this exception is thrown, it means that there was an attempt to divide
 * number by zero.
 */
struct division_by_zero: public std::runtime_error{
    division_by_zero():std::runtime_error("division by zero"){}
};

/**
 * @brief The unsupported_operation struct is an exception related to number struct
 *
 * If this exception is thrown, it means that the requested operation is not suported.
 * The reason is further specified in the what(method).
 */
struct unsupported_operation: public std::runtime_error{
    unsupported_operation(const char * what):std::runtime_error(what){}
    unsupported_operation(const std::string & what):std::runtime_error(what){}
};


/**
 * <b>The number struct represents the fixedpoint numbers</b>
 * <p>
 * Before using any functions provided by this library,
 * you must initialize its static member scale, you may use this code snippet:
 * @code
 * template<unsigned char radix>
 * std::size_t number<radix>::scale = 0;
 * @endcode
 * <p>
 * You can parametrise the numeral system used by template parameter.
 * The default numeral system is base-10, to set base-n numeral system
 * instantiate the template with n*.
 * <p>
 * You can select numeral interpretation strategy in strings
 * by defining a macro:
 * <ul>
 *  <li>FIXEDPOINT_CASE_INSENSITIVE for case insensitive interpretation</li>
 *  <li>FIXEDPOINT_CASE_SENSITIVE for case sensitive interpretation</li>
 * </ul>
 * The default is case insensitive interpretation.
 * The numerals take the following precedence, from lowest to highest value:
 * <ul>
 *  <li>numbers from 0 to 9</li>
 *  <li>letters 'a' to 'z' (lowercase) in alphabetical order</li>
 *  <li>letters 'A' to 'Z' (uppercase) in alphabetical order</li>
 *  <li>characters '#'(valued as 62 decimal) and '$'(valued as 63 decimal)</li>
 * </ul>
 * The lowercase letters are preferred for string representation in case
 * insensitive mode. Characters '#' and '$' are invalid for case insensitive
 * mode.
 * <p>
 * <p>
 * Notes:
 * <p>
 * <ol>
 * <li>implementation caveat - maximum supported system is base-36 (or base-64),
 * due to lack of suitable ASCII chaacters to express numerals</li>
 * <li><b>Known bug</b>: performing @code myVar1 *= myVar1; @endcode
 * results in an exception, use @code myVar1 = myVar1 * myVar1; @endcode
 * or @code myVar1.pow(2); //implicitly converts 2 to same type as myVar1 @endcode
 * instead.</li>
 * </ol>
 */
template<unsigned char radix>
struct number{

    /**
     * @brief To what number of fractional places to perform division.
     *
     * Affects modulo as well - mudulo is performed to
     * scale+number of fractional digits of the divisor precision.
     */
    static std::size_t scale;

    /**
     * @brief Default constructor
     *
     * Constructs a number of zero value and zero scale.
     */
    number():
        isPositive(true)
    {
        static_assert(radix<=MAX_RADIX, "fixedpoint::number's radix too high");
        static_assert(radix>=2, "fixedpoint::number's radix is too low, use at least 2");
        whole.resize(1,0);
        fractional.resize(0,0);
    }
    /**
     * @brief String constructor
     *
     * Key for source string format:
     * <ul>
     *  <li>s       First position for - sign (optional)</li>
     *  <li>RR      Decimal digits of radix (optional, always couple with radix separator)</li>
     *  <li>::      Radix separator (optional, always must be coupled with radix)</li>
     *  <li>S       Second position for - sign (optional)</li>
     *  <li>D...D   Digits of the whole part</li>
     *  <li>p       Radix point - '.' or ',' character (optional, always couple with fractional part)</li>
     *  <li>d...d   Digits of the fractional part (optional, always precede by radix point)</li>
     * </li>
     *
     * If you don't specify fracnum, by default the size of the detected
     * fractional part is taken and multiplied by ceil of source_radix/target_radix
     * and the result of that operation is considered as fracnum (aka IMPLICIT STRATEGY).
     *
     * If you specify fracnum as a nonnegative integer that number is not modified
     * in any way and fractional part is calculated to that size.
     *
     * If you specify fracnum as a negative integer, the implicit strategy is performed.
     *
     * Regardless of what is the value of fracnum, the resulting number is always
     * stripped of leading and trailing zeroes.
     *
     * @param src   source string according to format sRR::SD...Dpd...d
     * @param fracnum how many fractional digits to store after conversion
     * @throw invalid_number_format upon misformatted number, further details in what() and cerr
     * @throw radix_invalid when radix is specified incorrectly or is not supported
     */
    explicit number(const std::string & src, long long int fracnum = -1):
        isPositive(src.find_first_of('-')==src.npos)
    {
        using namespace std::literals;
        static_assert(radix<=MAX_RADIX, "fixedpoint::number's radix too high");
        static_assert(radix>=2, "fixedpoint::number's radix is too low, use at least 2");
        // basic format verification:
        if (std::count_if(
                    src.cbegin(),
                    src.cend(),
                    [](const char c){return values[static_cast<int>(c)]==-2;}
                    ) > 1){
            std::cerr << src
                      << " contains too many decimal separators"
                      << std::flush;
            throw(invalid_number_format("contains more than one decimal separator"));
        }
        auto start = src.cbegin();
        std::size_t rdx = radix;
        if (*start=='-') ++start; // negative check already happened
        if ((src.find_first_of(':'))!=src.npos){ // has radix specified
            std::stringstream x("");
            while(*start != ':'){
                if(*start<'0' || '9'<*start) {
                    std::cerr << src
                              << " contains invalid characters in radix"
                              << std::flush;
                    throw(invalid_number_format("src's radix is ill formatted"));
                }
                x << *start;
                ++start;
            }
            x >> rdx; // radix is always specified as decimal number
            if ( rdx > MAX_RADIX || rdx < 2 ) {
                throw(radix_invalid());
            }
            ++start; // goes to second colon
            if (*start != ':'){
                std::cerr << src
                          << " is missing a colon in radix separator"
                          << std::flush;
                throw(invalid_number_format("missing 2nd colon in separator"));
            }
            ++start;
        }
        if ((!isPositive) && *start=='-'){
            // - sign was not read yet
            ++start;
        }
        // number validity check:
        // TO DO - remove static_cast;
        if (std::any_of(
                    start,
                    src.cend(),
                    [rdx = static_cast<int>(rdx)](const char c){
                    return (values[static_cast<int>(c)]==-1 ||
                            (values[static_cast<int>(c)]>=rdx));
                    }))
        {
            std::cerr << src
                      << " contains invalid characters for radix "
                      << rdx
                      << std::flush;
            throw(invalid_number_format("invalid characters found in input string"));
        }
        // get whole and decimal parts:
        std::string wholes,fractionals;
        std::vector<uint8_t> wp,fp;
        bool onDecimal = false;
        for (;start!=src.cend();++start){
            if (onDecimal){
                fractionals += *start;
            }
            else{
                if (values[static_cast<int>(*start)] == -2) onDecimal = true;
                else wholes += *start;
            }
        }
        // reverse whole part (so it meets storage requirement):
        //std::reverse(wholes.begin(), wholes.end());
        auto converter = [](const int x){return values[x];};
        wp.reserve(wholes.size()); fp.reserve(fractionals.size());
        std::transform(wholes.crbegin(),
                       wholes.crend(),
                       std::back_inserter(wp),
                       converter);
        std::transform(fractionals.begin(),
                       fractionals.end(),
                       std::back_inserter(fp),
                       converter);
        // convert here:
        if (rdx != radix){
            // convert from base rdx to base radix:
            // convert radix to base rdx:
            if (fracnum < 0) fracnum = std::ceil(static_cast<double>(rdx)/radix)*fp.size();
            auto res = convert_with_vector(wp, fp, rdx, fracnum);
            wp = std::move(res.first);
            fp = std::move(res.second);
        }
        else{
            if(fracnum<0) fracnum = fp.size();
        }
        for(std::size_t i = 0; i<wp.size();++i){
            whole.set(i,wp[i]);
        }
        for(std::size_t i = 0; i<fp.size();++i){
            fractional.set(i,fp[i]);
        }
        fractional.resize(fracnum,0);
        strip_zeroes();
    }

    /**
     * @brief Integral type constructor
     * @param x value to construct number with
     */
    template<typename T, typename = decltype(static_cast<std::true_type>(std::is_integral<T>()))>
    number(T x):
        fractional(),
        isPositive(x>=0)
    {
        static_assert(radix<=MAX_RADIX, "fixedpoint::number's radix too high");
        static_assert(radix>=2, "fixedpoint::number's radix is too low, use at least 2");
        std::size_t index=0; // obtain string representation
        //if (!isPositive) rep.pop_back(); // get rid of - sign
        int help;
        while(x != 0){
            help = x%radix;
            if(help<0) help *= -1;
            whole.set(index,help);
            x/=radix;
            ++index;
        }
        if(whole.size() == 0){
            whole.resize(1,0);
        }
        strip_zeroes();
    }

    /**
     * @brief Floating point type constructor
     *
     * Please note: Due to the limited resolution of the mantissa
     * of floating point types, too large or too small numbers may end up
     * converted with different values than expected. If this happens, you
     * probably hit the limit of the mantissa's resolution.
     * For correct conversion you might wish to consider using
     * the string constructor instead.
     * @param x value to construct number with
     * @param scale how many fractional places to calculate (in target radix)
     */
    /*template<typename T, typename = decltype(static_cast<std::true_type>(std::is_floating_point<T>()))>
    explicit number(T x, unsigned int fracnum = 5):
        isPositive(x>=0)
    {
        static_assert(radix<=MAX_RADIX, "fixedpoint::number's radix too high");
        static_assert(radix>=2, "fixedpoint::number's radix is too low, use at least 2");
        if (fracnum > 19){
            std::cerr << "Max scale for floating point constructor is 19!" << std::flush;
            fracnum = 19;
        }
        if (x < 0) x*=-1;
        std::string whole, decimal;
        whole = std::to_string(std::floor(x));
        if (whole.find_first_of('.') != std::string::npos) {
            whole.resize(whole.find_first_of('.'),'0');
        }
        std::reverse(whole.begin(), whole.end()); // reverse string - BIG ENDIAN storage
        x -= std::floor(x);
        for (int i = std::min(fracnum, static_cast<unsigned>(std::ceil(10.0/radix))); i>=0; --i) x*=10;
        decimal = std::to_string(std::floor(x));
        if (decimal.find_first_of('.') != std::string::npos) {
            decimal.resize(decimal.find_first_of('.'),'0');
        }
#if !(defined(FIXEDPOINT_CASE_INSENSITIVE) || defined(FIXEDPOINT_CASE_SENSITIVE))
        // only perform the following
        // if we can't suppose anything about digits[]
        for (auto x = whole.begin(); x!=whole.end(); ++x){
            // convert from digits 0-9 to whatever our encoding uses
            *x = digits[*x-'0'];
        }
        for (auto x = decimal.begin(); x!=decimal.end(); ++x){
            // convert from digits 0-9 to whatever our encoding uses
            *x = digits[*x-'0'];
        }
#endif
        if (radix == 10){
            whole = whole;
            fractional = decimal;
        }
        else{
            auto res = convert_with_vector(whole,decimal,10,fracnum);
            whole = std::move(res.first);
            fractional = std::move(res.second);
        }
        strip_zeroes();
    }*/

    number(const number &) = default;
    number(number &&) = default;
    number& operator =(const number &) = default;
    number& operator =(number &&) = default;

    // O(n) where n is number of digits in number
    number& operator +=(const number & other){
        if ( isPositive != other.isPositive ){
            isPositive = ! isPositive; // make the signs match
            operator-=(other); // do -this-other
            isPositive = ! isPositive;
            // flip the resulting sign so that the result is correct
        }
        else{
            size_t boundary = std::min(fractional.size(), other.fractional.size());

            // FRACTIONAL PART
            if (fractional.size() < other.fractional.size()){
                // possibly partial copy
                fractional.resize(other.fractional.size(),0);
                for(std::size_t i = boundary;i<other.fractional.size();++i){
                    fractional.set(i,other.fractional.get(i));
                }
            }
            uint64_t carry = 0;
            for (std::size_t i = boundary; i>0; --i ){
                carry += fractional.get(i-1) + other.fractional.get(i-1);
                fractional.set(i-1,carry%radix);
                carry /= radix;
            }
            // WHOLE PART
            if ( other.whole.size() > whole.size() ){
                whole.resize(other.whole.size(),0);
            }
            for (std::size_t i = 0; i < whole.size() ; ++i){
                carry += whole.get(i) + other.whole.get(i);
                whole.set(i,carry%radix);
                carry /= radix;
            }
            if ( carry > 0 ){
                whole.push(carry);
            }
        }
        // At worst 1 partial copy (extending the scale)
        // or whatever -= does
        strip_zeroes();
        return *this;
    }

    number & operator -=(const number &other){
        if ( isPositive != other.isPositive ){
            number o1(other);
            o1.isPositive = isPositive;
            return *this +=( o1 );
        }
        //1 copy
        number o1(other);
        if(cmp_ignore_sig(o1) == -1){
            // subtract bigger in abs value from smaller - swap if not the case
            swap(o1);
            isPositive = !isPositive;
        }
        // assuming subtraction of smaller from bigger (abs values)
        std::size_t boundary = o1.fractional.size();
        // FRACTIONAL PART
        if (fractional.size() < boundary){
            fractional.resize( o1.fractional.size(), 0 );
        }
        int tmp, carry = 0;
        for ( std::size_t i = boundary; i>0; --i ){ // easier than with iterators
            //tmp = values[static_cast<int>(fractional[i-1])] - values[static_cast<int>(o1.fractional[i-1])] + carry;
            tmp = fractional.get(i-1) - o1.fractional.get(i-1) + carry;
            if(tmp < 0) {
                carry = -1;
                tmp +=radix;
            }else {
                carry = 0;
            }
            fractional.set(i-1,tmp);
        }

        // WHOLE PART
        if ( o1.whole.size() > whole.size()){
            whole.resize( o1.whole.size(), 0 );
        }
        std::size_t index = 0;
        std::size_t first_digit = 0;
        std::size_t ourS = whole.size();
        std::size_t othS = o1.whole.size();
        for ( index = 0; index < ourS ;index++){
            //tmp = values[static_cast<int>( whole.at(index))] + carry;
            tmp = whole.get(index) + carry;
            if ( index < othS ){
                //tmp -= values[static_cast<int>( o1.whole.at(index)) ];
                tmp -= o1.whole.get(index);
            }
            if(tmp < 0) {
                carry = -1;
                tmp +=radix;
            }else {
                carry = 0;
            }
            if(tmp != 0) first_digit = index;
            //whole.at(index) =  digits[tmp];
            whole.set(index,tmp);
        }
        whole.resize(first_digit + 1, 0 );
        strip_zeroes();
        // At worst 1 copy
        // or 1 copy and whatever += does
        return *this;
    }

    number & operator *=(const number & other){
        // handle sign:
        isPositive = (isPositive == other.isPositive);
        // trivial case - one of the numbers is (-)1 or 0:
        number one(1), zero(0);
        if ( (cmp_ignore_sig(one) == 0) ||
             (other.cmp_ignore_sig(zero) == 0) ){
            whole = other.whole;
            fractional = other.fractional;
        }
        else if ( (other.cmp_ignore_sig(one) == 0) ||
                  (cmp_ignore_sig(zero) == 0) ){
            return *this;
        }
        else{
            // representing numbers as fraxtions x/y, where y has format of
            // 1000...0 (number of fractional places zeroes)
            size_t decimals = std::max(fractional.size(), other.fractional.size());
            // expected maximum size of product
            size_t size = (decimals + whole.size()) +
                    (decimals + other.whole.size());
            // size of fractional part (precision)
            size_t endfrac = std::max(fractional.size(), other.fractional.size());

            const number a(*this);
            const number& b = other;

            const size_t dec_point = 2 * (decimals);
            fractional.clear();
            //fractional.resize(dec_point, digits[0] );
            whole.clear();
            whole.resize(size, 0 );

            auto product = [&dec_point, this](std::size_t i, int what) -> void{
                if(i < dec_point){
                    i = dec_point - i -1;
                    fractional.set(i,what);
                }else{
                    i = i - dec_point;
                    whole.set(i,what);
                }
            };
            auto get = [&decimals](const number &from,size_t i) -> int{
                if(i <decimals){
                    i = decimals - i -1;
                    if(i < from.fractional.size()) return from.fractional.get(i);
                    // implicit zeroes
                    else return 0;
                }else{
                    i = i - decimals;
                    if(i < from.whole.size()) return from.whole.get(i);
                    // implicit zeroes
                    else return 0;

                }
            };

            const size_t p = b.whole.size() + decimals;
            const size_t q = a.whole.size() + decimals;
            for(size_t b_i = 0; b_i < p; b_i++){
                unsigned char carry= 0;
                for(size_t a_i = 0; a_i < q; a_i++){
                    unsigned long tmp = get(*this,a_i + b_i);
                    tmp += carry + get(a, a_i ) * get(b, b_i);
                    carry = tmp / radix;
                    product(a_i + b_i, tmp % radix);
                }
                unsigned long tmp = get(*this,b_i + q);
                tmp += carry;
                product(b_i + q, tmp);
            }
            //size_t pos = whole.find_last_not_of(digits[0]);
            //if(pos != whole.npos) pos += 1;
            //else pos = 1;
            //whole.resize(pos, 0);
            fractional.resize(endfrac, 0);
        }
        strip_zeroes();
        return *this;
    }

    /**
     * @brief Divides number
     * @param other divisor
     * @return Reference to *this
     * @throw division_by_zero if divisor is zero
     */
    number & operator /=(const number &other){
        // handle signs here:
        isPositive = (isPositive == other.isPositive);
        div_or_mod(other,true);
        strip_zeroes();
        return *this;
    }

    /**
     * @brief Computes modulo of number
     * @param other divisor
     * @return Reference to *this
     * @throw division_by_zero if divisor is zero
     */
    number & operator %=(const number &other){
        // modulo has the sign of first operand
        div_or_mod(other,false);
        strip_zeroes();
        return *this;

    }

    number& operator ++(){
        operator+=(1); // 1 will be implicitly converted to number
        return *this;
    }
    number& operator --(){
        operator-=(1); // 1 will be implicitly converted to number
        return *this;
    }
    number operator ++(int){
        number copy(*this);
        operator++();
        return copy;
    }
    number operator --(int){
        number copy(*this);
        operator--();
        return copy;
    }

    bool operator <(const number & rhs) const{
        if (rhs.isPositive && !isPositive) return true;
        if (isPositive && ! rhs.isPositive) return false;
        if (isPositive) return cmp_ignore_sig(rhs)<0;
        return cmp_ignore_sig(rhs)>0;
    }

    bool operator ==(const number & rhs) const{
        bool tmpResult = ( isPositive == rhs.isPositive);
        if ( tmpResult ){
            return cmp_ignore_sig(rhs) == 0;
        }
        return tmpResult;
    }

    /**
     * @brief Calculates exponent-th power of number
     * @param exponent  which power to calculate
     * @return Reference to *this
     * @throw unsupported_operation when attemted to power with fractional number
     */
    number& pow(const number & exponent){
        // fractional part check - TO DO - implement
        if(! exponent.fractional.all_of(0)){
            throw unsupported_operation("Only integer exponent is suported for power function!");
        }
        // fast path for exponent 0
        if(exponent.cmp_ignore_sig(number()) == 0){
            operator=(number(1));
        }
        // fast path for powerbases 0 and (-)1
        else if(cmp_ignore_sig(number())==0 || cmp_ignore_sig(number(1))==0){
            if (! isPositive) { // 0 is treated as positive, therefore must be -1
                isPositive = ((exponent%number(2)).cmp_ignore_sig(number())==0);
            }
        }
        else{
            // negative exponent handling
            number expCopy(exponent);
            number others(1), help;
            number one(1),two(2);
            if(!expCopy.isPositive){
                // convert the problem to positive power of inverse number
                expCopy.isPositive = true;
                (*this) = one/(*this);
            }
            // divide and conquer - halve the exponent in each pass
            std::size_t scalebak = scale;
            scale = 0; // we need wholepart division for next part
            while(expCopy > one){

                if(expCopy%two == one) others *= (*this);
                help = (*this);
                operator*=(help);
                expCopy/=two;
            }
            scale = scalebak; // revert to original scale
            operator*=(others);
        }
        strip_zeroes();
        return *this;
    }

    /**
     * @brief Floors the number
     * @return Reference to *this
     */
    number& floor(){
        if( !fractional.all_of(0) ){
            if (! isPositive){
                operator--();
            }
        }
        fractional.clear();
        return *this;
    }

    /**
     * @brief Ceils the number
     * @return Reference to *this
     */
    number& ceil(){
        if( !fractional.all_of(0) ){
            if (isPositive){
                operator++();
            }
        }
        fractional.clear();
        return *this;
    }

    /**
     * @brief Truncates the number
     * @return Reference to *this
     */
    number& trunc(){
        fractional.clear();
        return *this;
    }

    /**
     * @brief Returns string representation of the object
     * @return  A newly constructed string
     */
    std::string str() const{
        std::stringstream acc("");
        //std::string reversewhole;
        //reversewhole.append(whole.crbegin(), whole.crend());
        acc << static_cast<unsigned int>(radix) << "::";
        if( ! isPositive ) acc << "-";
        //acc << reversewhole;
        std::size_t i = whole.size()-1;
        while( i>0 && whole.get(i)==0 ) --i;
        for (; i>0;--i){
            acc << digits[whole.get(i)];
        }
        acc << digits[whole.get(0)];
        if( ! fractional.empty() ){
            acc << "." ;
            for(i=0;i<fractional.size();++i){
                acc << digits[fractional.get(i)];
            }
        }
        std::string result(acc.str());
        return result;
    }

    /**
     * @brief Evaluates an expression in postfix (reverse polish) notation
     *
     * Expression format rules:
     * <ul>
     *  <li>Separate tokens (functions, numbers, operators) by space</li>
     *  <li>Denote function names with a leading '@' char
     * (in certain radices, the function name itself is a valid number)</li>
     *  <li>Write numbers according to the format specified in string constructor's documentation</li>
     * </ul>
     * @return Number containing the result of the evaluated expression
     * @throw invalid_expression_format with details of error provided by what() and printed to cerr
     * @throw (whatever the string constructor might throw)
     * @throw (whatever the functions or operations performed might throw)
     */
    static number eval_postfix(const std::string & expr){
        using namespace std::literals;
        std::vector<number> stack;
        std::istringstream tokenizer(expr);
        std::string tmp{};
        number a;
        std::string::const_iterator y;
        while( tokenizer >> tmp ){
            if( tmp.back() == '(') tmp.pop_back();
            if(tmp == "+"){
                if(stack.size() < 2){
                    std::clog << "Not enough arguments for performing operation: " << tmp;
                    throw(invalid_expression_format("requested operation needs more parameters than available"));
                }
                a = std::move(stack.back()); stack.pop_back();
                stack.back()+=a;
            }
            else if(tmp == "-"){
                if(stack.size() < 2){
                    std::clog << "Not enough arguments for performing operation: " << tmp;
                    throw(invalid_expression_format("requested operation needs more parameters than available"));
                }
                a = std::move(stack.back()); stack.pop_back();
                stack.back()-=a;
            }
            else if(tmp == "*"){
                if(stack.size() < 2){
                    std::clog << "Not enough arguments for performing operation: " << tmp;
                    throw(invalid_expression_format("requested operation needs more parameters than available"));
                }
                a = std::move(stack.back()); stack.pop_back();
                stack.back()*=a;
            }
            else if(tmp == "/"){
                if(stack.size() < 2){
                    std::clog << "Not enough arguments for performing operation: " << tmp;
                    throw(invalid_expression_format("requested operation needs more parameters than available"));
                }
                a = std::move(stack.back()); stack.pop_back();
                stack.back()/=a;
            }
            else if(tmp == "%"){
                if(stack.size() < 2){
                    std::clog << "Not enough arguments for performing operation: " << tmp;
                    throw(invalid_expression_format("requested operation needs more parameters than available"));
                }
                a = std::move(stack.back()); stack.pop_back();
                stack.back()%=a;
            }
            else if(tmp == "@pow"){
                if(stack.size() < 2){
                    std::clog << "Not enough arguments for performing operation: " << tmp;
                    throw(invalid_expression_format("requested operation needs more parameters than available"));
                }
                a = std::move(stack.back()); stack.pop_back();
                stack.back().pow(a);
            }
            else if(tmp == "@floor"){
                if(stack.empty()){
                    std::clog << "Not enough arguments for performing operation: " << tmp;
                    throw(invalid_expression_format("requested operation needs more parameters than available"));
                }
                stack.back().floor();
            }
            else if(tmp == "@ceil"){
                if(stack.empty()){
                    std::clog << "Not enough arguments for performing operation: " << tmp;
                    throw(invalid_expression_format("requested operation needs more parameters than available"));
                }
                stack.back().ceil();
            }
            else if(tmp == "@trunc"){
                if(stack.empty()){
                    std::clog << "Not enough arguments for performing operation: " << tmp;
                    throw(invalid_expression_format("requested operation needs more parameters than available"));
                }
                stack.back().trunc();
            }
            else if(tmp.front() == '@'){
                throw(invalid_expression_format(("unsupported function token found: "s).append(tmp)));
            }
            else{ // is a number
                stack.push_back(number(tmp));
            }
        }
        if (stack.empty()) throw(invalid_expression_format("No result after evaluation, did you enter an empty string?"));
        a = std::move(stack.back());
        return a;
    }


    /**
     * @brief Evaluates an expression in infix notation
     *
     * Expression format rules:
     * <ul>
     *  <li>Tokens can not contain spaces, however, you can separate
     * tokens with as many (including none) spaces</li>
     *  <li>Parentheses, square brackets and braces are allowed to indicate precedence
     * and denote function parameter list</li>
     *  <li>If a function has more than one parameter, separate with ',' (comma)</li>
     *  <li>Denote function names with a leading '@' char
     * (in certain radices, the function name itself is a valid number)</li>
     *  <li>Write numbers according to the format specified in string constructor's documentation,
     * but only use the '.' (dot) character as radix point</li>
     * </ul>
     * @return Number containing the result of the evaluated expression
     * @throw invalid_expression_format with details of error provided by what() and printed to cerr
     * @throw whatever eval_postfix might throw
     */
    static number eval_infix(const std::string & expr){
        using std::string; using namespace std::literals;
        auto getToken = [
                beg = expr.cbegin(),
                end = expr.cend(),
                separators = string("([{+*%/)]},")]
                ( string & out,
                  string::const_iterator & x)
                -> bool
        {
            string token;
            string::const_iterator prev = x-1;
            while(x!=end && *x == ' ') ++x;
            prev = x-1;
            if( x != end ){
                if (separators.find(*x)!=string::npos){
                    // single character token
                    token.push_back(*(prev = x++));
                }
                else if( (*x)=='-' ){
                    if( (prev<beg) ||
                        *prev=='-' ||
                        *prev==' ' ||
                        separators.find(*prev)!=string::npos ){
                        token.push_back(*(prev = x++));
                        while( x!=end &&
                               (*x != '-' || *prev==':') &&
                               *x != ' ' &&
                               separators.find(*x)==string::npos){
                            token.push_back(*(prev = x++));
                        }
                    }
                    else{
                        token.push_back(*(prev = x++));
                    }
                }
                else{ // function token or number token
                    while( x!=end &&
                           (*x != '-' || *prev==':') &&
                           *x != ' ' &&
                           separators.find(*x)==string::npos){
                        token.push_back(*(prev = x++));
                    }
                }
            }
            else prev=x;
            out = std::move(token);
            return prev!=end;
        };
        auto isOperator = [operators = string("+-*%/")](const string & x){
            return operators.find(x)!=string::npos;
        };
        auto lesserEqualPrecedence = [](const string & o1, const string & o2) -> bool{
            switch(o2.front()){
            case '+': case '-':
                return (o1=="+" || o1=="-");
            case '*': case '/': case '%':
                return true;
            }
            return false;
        };
        std::vector<string> operationStack, postfixBuild;
        string postfix, lparen{'(','[','{'}, rparen{')',']','}'};
        //std::istringstream inp(expr);
        auto x = expr.cbegin();
        string now;
        while( getToken(now,x) ){
            if(isOperator(now)){
                while( (!operationStack.empty()) &&
                       isOperator(operationStack.back()) &&
                       lesserEqualPrecedence(now, operationStack.back())){
                    postfixBuild.push_back(std::move(operationStack.back()));
                    operationStack.pop_back();
                }
                operationStack.push_back(std::move(now));
            }
            else if(now.front() == '@'){
                operationStack.push_back(std::move(now));
            }
            else if(lparen.find(now)!=string::npos){
                operationStack.push_back("(");
            }
            else if(now == ","){
                while( (!operationStack.empty()) &&
                       operationStack.back()!="("){
                    postfixBuild.push_back(std::move(operationStack.back()));
                    operationStack.pop_back();
                }
                if (operationStack.empty()){
                    std::cerr << "Misplaced function separator found in " << expr;
                    throw(invalid_expression_format("misplaced function argument separator - ','"));
                }
            }
            else if(rparen.find(now)!=string::npos){
                while( (!operationStack.empty()) &&
                       operationStack.back()!="("){
                    postfixBuild.push_back(std::move(operationStack.back()));
                    operationStack.pop_back();
                }
                if (operationStack.empty()){
                    std::cerr << "Mismatched right parethesis " << expr;
                    throw(invalid_expression_format("mismatched right parethesis found"));
                }
                operationStack.pop_back(); // remove the left parenthesis
                if ( (!operationStack.empty()) &&
                     operationStack.back().front() == '@'){ // function call
                    postfixBuild.push_back(std::move(operationStack.back()));
                    operationStack.pop_back();
                }
            }
            else{ // it's a number or something in place of a number
                postfixBuild.push_back(std::move(now));
            }
        }
        while(! operationStack.empty() ){
            if (operationStack.back() == "(") throw(invalid_expression_format("mismatched left parethesis found"));
            postfixBuild.push_back(std::move(operationStack.back()));
            operationStack.pop_back();
        }
        for(auto it = postfixBuild.begin(); it!=postfixBuild.end(); ++it){
            postfix.append(*it);
            postfix.push_back(' ');
        }
        postfix.pop_back(); // remove last ' '
        std::clog << "Built postfix expression: \"" << postfix << '"' << std::endl;
        number<radix> retVal(eval_postfix(postfix));
        return retVal;
    }

    template<unsigned char oradix>
    /**
     * @brief Converts numbers between radices
     * @return Converted number
     */
    static number convert(const number<oradix> & other){
        number result(other.str());
        return result;
    }

    /**
     * @brief Swaps number with other
     * @param other number to swap with
     */
    void swap( number& other ){
        // TO DO - implement swap or change to std::swap
        whole.swap(other.whole);
        fractional.swap(other.fractional);
        std::swap(isPositive, other.isPositive);
    }

private:
    /**
     * @brief blockSize Number of bits required to store a digit of radix
     *
     * Terminology:
     * * a 'block' is is a sequence of blockSize bits representing a digit,
     * * a 'cell' is an element in a vector.
     */
    //static const std::size_t blockSize = std::floor(std::log(radix-1)/std::log(2))+1;
    /**
     * @brief blockMask Masks maximum possible number to fit 'block'
     */
    //static const uint64_t blockMask = (1<<blockSize) - 1;
    /**
     * @brief cellCapacity Indicates how many digits of radix fit in a cell
     */
    //static const std::size_t cellCapacity = 64 / blockSize;
    /**
     * @brief whole Stores the whole part of the number in big endian
     *
     * Use of vector's native cells is naively maximized
     * (leftover unused bits at top)
     */
    _number<radix-1> whole; // BIG_ENDIAN element ordering
    /**
     * @brief fractional Stores the fractional part of the number in little endian
     */
    _number<radix-1> fractional;  // LITTLE_ENDIAN element ordering
    /**
     * @brief isPositive indicates sign of the number
     */
    bool isPositive;

    /*static uint64_t get(const std::vector<uint64_t> & where,
                        std::size_t blockIndex){
        std::size_t cellIndex = blockIndex/cellCapacity;
        uint64_t mask = blockMask<<(blockIndex%cellCapacity);
        return (cellIndex<where.size())?mask*where[cellIndex]:0;
    }

    static void set(std::vector<uint64_t> & where,
                    std::size_t blockIndex,
                    uint64_t what){
        std::size_t cellIndex = blockIndex/cellCapacity;
        uint64_t mask = !(blockMask<<(blockIndex%cellCapacity));
        what <<= blockIndex%cellCapacity;
        if(cellIndex >= where.size()) where.resize(cellIndex+1,0);
        (where[cellIndex] &= mask) |= what;
    }*/

    /**
     * @brief Compares two numbers whithout taking sign into account
     * @param other number to compare with
     * @return 0 if equal, n<0 if *this<other, n>0 if *this>other
     */
    int cmp_ignore_sig(const number &other) const{
        // skip possible leading zeroes - TO DO test if needed and remove if not
        std::size_t pos = whole.size() - 1;
        std::size_t other_pos = other.whole.size() - 1;
        //if(whole.npos == pos) pos=0;
        //if(other.whole.npos == other_pos) other_pos=0;
        if(pos != other_pos){
            return (pos > other_pos) ? 1 : -1;
        }else{
            while(pos > 0){
                if(whole.get(pos) != other.whole.get(pos)){
                    return (whole.get(pos) > other.whole.get(pos)) ? 1 : -1;
                }
                pos--;
            }
            // comparison of digits on 0th power
            if(whole.get(pos) != other.whole.get(pos)){
                return (whole.get(pos) > other.whole.get(pos)) ? 1 : -1;
            }
            // fractional part must decide
            std::size_t limit = (fractional.size() < other.fractional.size()) ? fractional.size() : other.fractional.size();
            while(pos < limit){
                if(fractional.get(pos) != other.fractional.get(pos)){
                    return (fractional.get(pos) > other.fractional.get(pos)) ? 1 : -1;
                }
                pos++;
            }
            // comparing against implicit zeroes
            if(other.fractional.size() > limit){
                for (;pos < other.fractional.size();pos++) {
                    if (other.fractional.get(pos) > 0) return -1;
                }
            }
            if(fractional.size() > limit){
                for (;pos < fractional.size();pos++) {
                    if (fractional.get(pos) > 0) return 1;
                }
            }
            return 0;
        }
    }

    /**
     * @brief Rise this number to the power of positive integer exponent
     * @param exponent nubmer representing positive integer value
     */
    /*
    void int_pow(number && exponent){
        number nula(0);
        number orig(*this);
        //naivní algoritmus
        while(exponent > nula){
            operator*=(orig);
            --exponent;
        }
    }
    */

    /**
     * @brief Divides this by other and returns either the result of division or modulo based on the div parameter
     * @param other number to divide this by
     * @param div whether division, or modulo shall be returned
     * @return Result of division if div is true and result of modulo otherwise
     */
    number& div_or_mod(number other,bool div){
        _number<radix-1> result;
        number divisor;
        number dividend;

        // shift so that handling is possible as whole numbers,
        // and account for possible new numbers of the modulo (nonzero scale)
        std::size_t dividend_size = whole.size();
        long long deviation = dividend_size - other.whole.size();
        while(!whole.empty()){
            dividend.fractional.push(whole.pop());
        }
        //dividend.fractional.assign(whole.rbegin(), whole.rend());
        for(std::size_t i; i<fractional.size();++i){
            dividend.fractional.push(fractional.get(i));
        }
        //dividend.fractional.append(fractional);

        while(!other.whole.empty()){
            divisor.fractional.push(other.whole.pop());
        }
        //divisor.fractional.assign(other.whole.rbegin(), other.whole.rend());
        std::size_t shift = other.fractional.size();
        if(divisor == 0){
            for(std::size_t i = 0; i<other.fractional.size();++i){
                if (other.fractional.get(i)!=0) shift = i;
            }
            //shift = other.fractional.find_last_not_of(digits[0]);

            if(shift == other.fractional.size() ) throw division_by_zero();

            for(std::size_t i = 0; i<shift;++i){
                divisor.fractional.push(other.fractional.get(i));
            }
            //divisor.fractional.assign(other.fractional.substr(shift));
            // TO DO - investigate logical reason for ++shift;
            ++shift;
        }else{
            for(std::size_t i = 0; i < other.fractional.size(); ++i){
                divisor.fractional.push(other.fractional.get(i));
            }
            //divisor.fractional.append(other.fractional);
        }

        whole.clear();

        // number of division steps
        int steps = deviation + scale + shift;

        if (steps < 0){
            // divisor is bigger by order(s) of magnitude,
            // the whole dividend is the modulo
            if(div) *this = 0;
            return *this;
        }

        for(int i = 0; i <= steps;i++){
            size_t tmp=0;
            while(dividend.cmp_ignore_sig(divisor) >= 0){
                tmp++;
                dividend -= divisor;
            }
            result.push(tmp);
            // insert 0 to beginning
            _number<radix-1> hlp(0,1);
            for (std::size_t i=0;i<divisor.fractional.size();++i){
                hlp.push(divisor.fractional.get(i));
            }
            divisor.fractional = std::move(hlp);
            //divisor.fractional.insert(0, 1, digits[0]);
        }
        if(div){
            int move = result.size() - scale;
            if(move>0){
                fractional.clear();
                for(std::size_t i=move;i<result.size();++i){
                    fractional.push(result.get(i));
                }
                result.resize(move,0);
                while(!result.empty()){
                    whole.push(result.pop());
                }
                //whole.assign(result.rbegin()+scale,result.rend());
                //fractional.clear();
                //fractional.append(result,move, std::string::npos);
            }else{
                fractional.resize(fractional.size()-move,0);
                for(std::size_t i=0; i<result.size();++i){
                    fractional.push(result.get(i));
                }
                //fractional.append(result);
            }

            isPositive = (isPositive == other.isPositive);
        }else{
            fractional.clear();
            for(std::size_t i=dividend_size;i<dividend.fractional.size();++i){
                fractional.push(dividend.fractional.get(i));
            }
            dividend.fractional.resize(dividend_size,0);
            while(!dividend.fractional.empty()){
                whole.push(dividend.fractional.pop());
            }
            //std::string tmp= dividend.fractional.substr(0,whole.size());
            //std::reverse(tmp.begin(),tmp.end());
            //whole = std::move(tmp);
            //fractional = dividend.fractional.substr(whole.size());
            //scale = 0;
        }
        strip_zeroes();
        return *this;
    }

    /**
     * @brief Strip leading and trailing 0
     *
     * If value of whole part is equal to 0, then strips all but the last
     * leading 0 digit, otherwise strips all leading zeroes.
     *
     * If all digits are zero, sets isPositive to true,
     * so we are consistent with sign of zero.
     */
    void strip_zeroes(){
        bool isZero;
        //if((pos=whole.find_last_not_of(digits[0])) != whole.npos){
        while(whole.size() > 1 && whole.back() == 0){
            whole.pop();
        }
        if (whole.back() == 0){
            isZero = true;
        }
        else{
            isZero = false;
        }
        // leading zeroes stripped
        isZero = isZero && (fractional.all_of(0));
        if (isZero) isPositive = true; // 0 is treated as positive
    }

    /**
     * @brief Converts whole and decimal parts between radices
     * <p>
     * Conversion from radix A to radix B is achieved by utilizing
     * the muladd_vector_uint_uint method for the whole part.
     * For the whole part this method merely walks through the .
     * For the decimal part this function also performs minor tasks related to
     * conversion
     * <p>
     * @param srcWhole   Source number to convert - whole part as BIG ENDIAN
     * @param srcDec     Source number to convert - decimal part as LITTLE ENDIAN
     * @param rdx   Number base (radix) of source number
     * @param size  Number of decimal places to compute
     * @return  Pair of holding converted number representation
     */
    static std::pair<std::vector<uint8_t>, std::vector<uint8_t>> convert_with_vector(
            const std::vector<uint8_t> & srcWhole,
            const std::vector<uint8_t> & srcDec,
            unsigned int rdx,
            unsigned int size){
        std::vector<uint8_t> retValWhole;
        for(auto x=srcWhole.crbegin(); x!=srcWhole.crend();++x){
            muladd_vector_uint_uint(retValWhole,rdx,*x);
        }
        std::vector<uint8_t> retValDec,vec;
        std::copy(srcDec.crbegin(),srcDec.crend(),std::back_inserter(vec));
        std::size_t orig_size = vec.size();
        retValDec.reserve(size);
        unsigned int digitConverter = 0;
        for(std::size_t i=0;i<size;++i){
            muladd_vector_uint_uint(vec,radix,0,rdx);
            digitConverter = 0;
            while(vec.size() > orig_size){
                // does what muladd_vector_uint_uint does, but on small scale
                digitConverter *=rdx;
                digitConverter += vec.back();
                vec.pop_back();
            }
            retValDec.push_back(digitConverter);
        }
        return make_pair(retValWhole, retValDec);
    }

    /**
     * @brief Multiplies vector by uint, then adds another uint
     *
     * Multiplies a vectors elements by an unsigned integer and then adds
     * another unsigned integer. Elements stored in vector are kept at values
     * smaller than specified in max_element - which is equivalent to storing
     * the results in "base-max_element". Vector should store values as big endian.
     * @param vec   Vector to multiply
     * @param mul   Vector's multiplier
     * @param max_element First value larger than allowed in vector's cells
     * @param add   The number to add to the vector
     */
    static void muladd_vector_uint_uint(
            std::vector<uint8_t> & vec,
            unsigned int mul,
            unsigned int add,
            unsigned int max_element = radix){
        unsigned int carry = 0;
        // multiply:
        for(auto x = vec.begin(); x != vec.end(); ++x){
            carry += mul*(*x);
            *x = carry % max_element; // lower half
            carry /= max_element; // upper half
        }
        while (carry!=0) {
            vec.push_back(carry % max_element);
            carry /= max_element;
        }
        // add:
        carry = add;
        for (auto x = vec.begin(); x != vec.end(); ++x){
            carry += *x;
            *x = carry % max_element;
            carry /= max_element;
        }
        do {
            vec.push_back(carry % max_element);
            carry /= max_element;
        } while (carry!=0);
    }

};

template<unsigned char radix>
bool operator <=(const number<radix>& lhs,const number<radix> & rhs){
    return ( ! ( rhs < lhs ) );
}

template<unsigned char radix>
bool operator !=(const number<radix>& lhs,const number<radix> & rhs){
    return ( ! ( lhs == rhs ) );
}

template<unsigned char radix>
bool operator >(const number<radix>& lhs,const number<radix> & rhs){
    return ( rhs < lhs );
}

template<unsigned char radix>
bool operator >=(const number<radix>& lhs,const number<radix> & rhs){
    return ( ! ( lhs < rhs ) );
}

template<unsigned char radix>
number<radix> operator +(const number<radix> & lhs,
                         const number<radix> & rhs){
    number<radix> newnum(lhs);
    newnum+=rhs;
    return newnum;
}
template<unsigned char radix,
         typename T,
         typename = decltype(static_cast<std::true_type>(std::is_integral<T>()))>
number<radix> operator +(const number<radix> & lhs, T rhs){
    number<radix> newnum(lhs);
    newnum+=number<radix>(rhs);
    return newnum;
}
template<unsigned char radix,
         typename T,
         typename = decltype(static_cast<std::true_type>(std::is_integral<T>()))>
number<radix> operator +(T lhs, const number<radix> & rhs){
    number<radix> newnum(lhs);
    newnum+=rhs;
    return newnum;
}

template<unsigned char radix>
number<radix> operator -(const number<radix> & lhs,
                         const number<radix> & rhs){
    number<radix> newnum(lhs);
    newnum-=rhs;
    return newnum;
}
template<unsigned char radix,
         typename T,
         typename = decltype(static_cast<std::true_type>(std::is_integral<T>()))>
number<radix> operator -(const number<radix> & lhs, T rhs){
    number<radix> newnum(lhs);
    newnum-=number<radix>(rhs);
    return newnum;
}
template<unsigned char radix,
         typename T,
         typename = decltype(static_cast<std::true_type>(std::is_integral<T>()))>
number<radix> operator -(T lhs, const number<radix> & rhs){
    number<radix> newnum(lhs);
    newnum-=rhs;
    return newnum;
}

template<unsigned char radix>
number<radix> operator *(const number<radix> & lhs,
                         const number<radix> & rhs){
    number<radix> newnum(lhs);
    newnum*=rhs;
    return newnum;
}
template<unsigned char radix,
         typename T,
         typename = decltype(static_cast<std::true_type>(std::is_integral<T>()))>
number<radix> operator *(const number<radix> & lhs, T rhs){
    number<radix> newnum(lhs);
    newnum*=number<radix>(rhs);
    return newnum;
}
template<unsigned char radix,
         typename T,
         typename = decltype(static_cast<std::true_type>(std::is_integral<T>()))>
number<radix> operator *(T lhs, const number<radix> & rhs){
    number<radix> newnum(lhs);
    newnum*=rhs;
    return newnum;
}

template<unsigned char radix>
number<radix> operator /(const number<radix> & lhs,
                         const number<radix> & rhs){
    number<radix> newnum(lhs);
    newnum/=rhs;
    return newnum;
}
template<unsigned char radix,
         typename T,
         typename = decltype(static_cast<std::true_type>(std::is_integral<T>()))>
number<radix> operator /(const number<radix> & lhs, T rhs){
    number<radix> newnum(lhs);
    newnum/=number<radix>(rhs);
    return newnum;
}
template<unsigned char radix,
         typename T,
         typename = decltype(static_cast<std::true_type>(std::is_integral<T>()))>
number<radix> operator /(T lhs, const number<radix> & rhs){
    number<radix> newnum(lhs);
    newnum/=rhs;
    return newnum;
}

template<unsigned char radix>
number<radix> operator %(const number<radix> & lhs,
                         const number<radix> & rhs){
    number<radix> newnum(lhs);
    newnum%=rhs;
    return newnum;
}
template<unsigned char radix,
         typename T,
         typename = decltype(static_cast<std::true_type>(std::is_integral<T>()))>
number<radix> operator %(const number<radix> & lhs, T rhs){
    number<radix> newnum(lhs);
    newnum%=number<radix>(rhs);
    return newnum;
}
template<unsigned char radix,
         typename T,
         typename = decltype(static_cast<std::true_type>(std::is_integral<T>()))>
number<radix> operator %(T lhs, const number<radix> & rhs){
    number<radix> newnum(lhs);
    newnum%=rhs;
    return newnum;
}

template<unsigned char radix>
std::ostream& operator<<(std::ostream& out, const number<radix> & ref){
    out << ref.str();
    return out;
}

template<unsigned char radix>
std::istream& operator>>(std::istream& in, number<radix> & ref){
    std::string temp;
    in >> temp;
    ref = number<radix>(temp);
    return in;
}

// Common radix typedefs:
#if MAX_RADIX>=2
using binary = number<2>;
#endif
#if MAX_RADIX>=8
using octal = number<8>;
#endif
#if MAX_RADIX>=10
using decimal = number<10>;
#endif
#if MAX_RADIX>=12
using duodecimal = number<12>;
#endif
#if MAX_RADIX>=16
using hexadecimal = number<16>;
#endif
#if MAX_RADIX>=60
using sexagesimal = number<60>;
#endif
#if MAX_RADIX>=64
using base64 = number<64>;
#endif


} // namespace fixedpoint

namespace std{

template<unsigned char radix>
fixedpoint::number<radix> pow(const fixedpoint::number<radix> & base,
                              const fixedpoint::number<radix> & exponent){
    fixedpoint::number<radix> result(base);
    result.pow(exponent);
    return result;
}

template<unsigned char radix>
fixedpoint::number<radix> floor(const fixedpoint::number<radix> & num){
    fixedpoint::number<radix> result(num);
    result.floor();
    return result;
}

template<unsigned char radix>
fixedpoint::number<radix> ceil(const fixedpoint::number<radix> & num){
    fixedpoint::number<radix> result(num);
    result.ceil();
    return result;
}

template<unsigned char radix>
fixedpoint::number<radix> trunc(const fixedpoint::number<radix> & num){
    fixedpoint::number<radix> result(num);
    result.trunc();
    return result;
}

} // namespace std
#endif // FIXEDPOINT_H

