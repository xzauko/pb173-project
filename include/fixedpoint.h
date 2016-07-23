/**
 * @file   fixedpoint.h
 * @author Michal Pochobradský
 * @author Tibor Zauko
 * @brief Single include implementation of fixedpoint arithmetic library.
 */
// TO DO list:
// Implement general power calculations

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
 * <li>Use c++14 for compilation.</li>
 * </ol>
 */
template<unsigned char radix>
struct number{
private:

    /**
     * @brief The _number struct stores the values of the number's whole and fractional part
     */
    struct _number{
    private:
        static constexpr unsigned int minBits(){
            unsigned int x = 2,s = 1;
            while(x<radix){
                x<<=1;
                ++s;
            }
            return s;
        }

        /**
         * @brief The _item struct is an element of the vector used for data storage
         *
         * Wastes 1B when bitsize is 1 (radix 2),
         * but saves 2B when bitsize==5 (radices from 17 to 32)
         * compared to uint8_t.
         */
        template<unsigned bitsize>
        struct _item{
            uint16_t b0:bitsize,b1:bitsize,
                       b2:bitsize,b3:bitsize,
                       b4:bitsize,b5:bitsize,
                       b6:bitsize,b7:bitsize;

            _item(int d_val = 0):
                b0(d_val),b1(d_val),b2(d_val),b3(d_val),
                b4(d_val),b5(d_val),b6(d_val),b7(d_val){}

            bool operator==(const _item & o) const{
                return (b0==o.b0 &&  b1==o.b1 &&
                        b2==o.b2 &&  b3==o.b3 &&
                        b4==o.b4 &&  b5==o.b5 &&
                        b6==o.b6 &&  b7==o.b7);
            }

        };

        using vecitem = _item<minBits()>;
    public:
        _number():
            realSize(0)
        {}

        _number(std::size_t newSize, int value):
            realSize(0)
        {
            resize(newSize,value);
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
            if(index>=realSize) return 0;
            else{
                auto cell = _data.at(index/8);
                int rval;
                switch(index%8){
                case 0:
                    rval = cell.b0;
                    break;
                case 1:
                    rval = cell.b1;
                    break;
                case 2:
                    rval = cell.b2;
                    break;
                case 3:
                    rval = cell.b3;
                    break;
                case 4:
                    rval = cell.b4;
                    break;
                case 5:
                    rval = cell.b5;
                    break;
                case 6:
                    rval = cell.b6;
                    break;
                case 7:
                    rval = cell.b7;
                    break;
                };
                return rval;
            }
        }

        /**
         * @brief set sets a digit of number to value
         *
         * If specified digit does not exist, it creates a place for it
         *
         * @param index determines which digit to set
         * @param what  what value to set the choosen digit
         */
        void set(std::size_t index, int value){
            if(index>=realSize){
                resize(index+1,0);
            }
            auto & cell = _data.at(index/8);
            switch(index%8){
            case 0:
                cell.b0 = value;
                break;
            case 1:
                cell.b1 = value;
                break;
            case 2:
                cell.b2 = value;
                break;
            case 3:
                cell.b3 = value;
                break;
            case 4:
                cell.b4 = value;
                break;
            case 5:
                cell.b5 = value;
                break;
            case 6:
                cell.b6 = value;
                break;
            case 7:
                cell.b7 = value;
                break;
            };
        }

        /**
         * @brief size Returns the size of stored _number
         */
        std::size_t size() const{
            return realSize;
        }

        /**
         * @brief resize Adjusts the size of stored _number
         * @param newSize   Targeted size
         * @param def_value Value to initialize new members with
         */
        void resize(std::size_t newSize, int def_value){
            vecitem a(def_value);
            std::size_t rnsize = newSize/8, roEnd = (_data.size()>0)?_data.size()-1:0;
            if(newSize%8!=0) ++rnsize;
            _data.resize(rnsize,a);
            if(newSize>realSize){
                auto & cell = _data.at(roEnd);
                switch(realSize%8){
                case 1:
                    cell.b1 = def_value;
                case 2:
                    cell.b2 = def_value;
                case 3:
                    cell.b3 = def_value;
                case 4:
                    cell.b4 = def_value;
                case 5:
                    cell.b5 = def_value;
                case 6:
                    cell.b6 = def_value;
                case 7:
                    cell.b7 = def_value;
                    break;
                case 0:
                    break;
                }
            }
            realSize = newSize;
        }

        /**
         * @brief clear Removes all stored data
         */
        void clear(){
            _data.clear();
            realSize = 0;
        }

        /**
         * @brief empty Test emptyness of _number
         * @return true if empty, false otherwise
         */
        bool empty() const{
            return _data.empty();
        }

        /**
         * @brief all_of Checks whether all elements are equal to a value
         * @param value Comparison value for elements
         * @return true if all elements are equal to value, false otherwise
         */
        bool all_of(int value) const{
            bool retVal = true;
            if(_data.size()>=1){
                vecitem cmpval(value);
                retVal = std::all_of(_data.cbegin(),
                                     _data.cend()-1,
                                     [&cmpval](const vecitem & x){
                    return x==cmpval;
                });
                auto cell = _data.back();
                switch(realSize%8){
                case 0:
                    retVal = retVal && cell.b7==value;
                case 7:
                    retVal = retVal && cell.b6==value;
                case 6:
                    retVal = retVal && cell.b5==value;
                case 5:
                    retVal = retVal && cell.b4==value;
                case 4:
                    retVal = retVal && cell.b3==value;
                case 3:
                    retVal = retVal && cell.b2==value;
                case 2:
                    retVal = retVal && cell.b1==value;
                case 1:
                    retVal = retVal && cell.b0==value;
                }
            }
            return retVal;
        }

        /**
         * @brief push Store value to end of _number
         * @param value what value to store
         */
        void push(int value){
            set(realSize,value);

        }

        /**
         * @brief pop Destructively read last value from _number
         * @return Value of removed item
         */
        int pop(){
            int rval = back();
            if((--realSize)%8 == 0) _data.pop_back();
            return rval;
        }

        /**
         * @brief back Return value of last element
         */
        int back() const{
            return get(realSize-1);
        }

        /**
         * @brief swap Swaps contents of 2 _numbers
         * @param o Othern _number to swap with
         */
        void swap(_number & o){
            _data.swap(o._data);
            std::swap(realSize,o.realSize);
        }
    private:
        /**
         * @brief _data holds the actual data
         */
        std::vector<vecitem> _data;
        /**
         * @brief realSize number of stored values
         *
         * Allocated size always rounds to closest higher (or equal) multiply of 8.
         */
        std::size_t realSize;
    };
public:

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
                    [](const char c){return values[c]==-2;}
                    ) > 1){
            std::cerr << src
                      << " contains too many decimal separators"
                      << std::flush;
            throw(invalid_number_format("contains more than one decimal separator"));
        }
        auto start = src.cbegin();
        int rdx = radix;
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
        if (std::any_of(
                    start,
                    src.cend(),
                    [rdx](const char c){
                    return (values[c]==-1 ||
                            (values[c]>=rdx));
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
                if (values[*start] == -2) onDecimal = true;
                else wholes += *start;
            }
        }
        // reverse whole part (so it meets storage requirement):
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
    template<typename T, typename = decltype(static_cast<std::true_type>(std::is_floating_point<T>()))>
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
        std::string whole1, decimal;
        whole1 = std::to_string(std::floor(x));
        if (whole1.find_first_of('.') != std::string::npos) {
            whole1.resize(whole1.find_first_of('.'),'0');
        }
        std::reverse(whole1.begin(), whole1.end()); // reverse string - BIG ENDIAN storage
        x -= std::floor(x);
        for (int i = std::min(fracnum, static_cast<unsigned>(std::ceil(10.0/radix))); i>=0; --i) x*=10;
        decimal = std::to_string(std::floor(x));
        if (decimal.find_first_of('.') != std::string::npos) {
            decimal.resize(decimal.find_first_of('.'),'0');
        }
#if !(defined(FIXEDPOINT_CASE_INSENSITIVE) || defined(FIXEDPOINT_CASE_SENSITIVE))
        // only perform the following
        // if we can't suppose anything about digits[]
        for (auto x = whole1.begin(); x!=whole1.end(); ++x){
            // convert from digits 0-9 to whatever our encoding uses
            *x = digits[*x-'0'];
        }
        for (auto x = decimal.begin(); x!=decimal.end(); ++x){
            // convert from digits 0-9 to whatever our encoding uses
            *x = digits[*x-'0'];
        }
#endif
        if (radix == 10){
            for(std::size_t i = 0; i<whole1.size();++i){
                whole.set(i,values[whole1[i]]);
            }
            for(std::size_t i = 0; i<decimal.size();++i){
                fractional.set(i,values[decimal[i]]);
            }
        }
        else{
            std::vector<uint8_t> whole2,decimal1;
            std::transform(whole1.begin(),whole1.end(),std::back_inserter(whole2),[](int x){return values[x];});
            std::transform(decimal.begin(),decimal.end(),std::back_inserter(decimal1),[](int x){return values[x];});
            auto res = convert_with_vector(whole2,decimal1,10,fracnum);
            whole2 = std::move(res.first);
            decimal1 = std::move(res.second);
            for(std::size_t i = 0; i<whole2.size();++i){
                whole.set(i,whole2[i]);
            }
            for(std::size_t i = 0; i<decimal1.size();++i){
                fractional.set(i,decimal1[i]);
            }
        }
        strip_zeroes();
    }

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
        // 1 copy
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
        for ( std::size_t i = boundary; i>0; --i ){
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
            tmp = whole.get(index) + carry;
            if ( index < othS ){
                tmp -= o1.whole.get(index);
            }
            if(tmp < 0) {
                carry = -1;
                tmp +=radix;
            }else {
                carry = 0;
            }
            if(tmp != 0) first_digit = index;
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
            // representing numbers as fractions x/y, where y has format of
            // 1000...0 (number of fractional places zeroes)
            // size of fractional part (precision)
            std::size_t decimals = std::max(fractional.size(), other.fractional.size());

            const number& a = *this;
            const number& b = other;
            number r; // result
            r.isPositive = isPositive;

            const std::size_t dec_point = fractional.size()+other.fractional.size();

            auto product = [&r, dec_point](std::size_t i, int what) -> void{
                if(i < dec_point){
                    i = dec_point - i - 1;
                    r.fractional.set(i,what);
                }else{
                    i = i - dec_point;
                    r.whole.set(i,what);
                }
            };
            auto get = [](const number &from, std::size_t i) -> int{
                if(i < from.fractional.size()){
                    i = from.fractional.size() - i - 1;
                    return from.fractional.get(i);
                }else{
                    i = i - from.fractional.size();
                    return from.whole.get(i);
                }
            };

            const std::size_t p = b.whole.size() + b.fractional.size();
            const std::size_t q = a.whole.size() + a.fractional.size();
            for(std::size_t b_i = 0; b_i < p; b_i++){
                unsigned char carry = 0;
                for(std::size_t a_i = 0; a_i < q; a_i++){
                    unsigned long tmp = get(r, a_i + b_i);
                    tmp += carry + get(a, a_i) * get(b, b_i);
                    carry = tmp / radix;
                    product(a_i + b_i, tmp % radix);
                }
                unsigned long tmp = get(r,b_i + q);
                tmp += carry;
                product(b_i + q, tmp);
            }
            operator=(std::move(r));
            fractional.resize(decimals, 0);
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
     *
     * When using real non-integer exponents, negative base is prohibited
     * (raises exception).
     *
     * For real exponents adjusting scale changes precision
     * (division is used in calculation). Parameter precision specifies,
     * how many elements of the binomial series (starting from the first)
     * should be considered for calculation/approximation of the result.
     * Default behavior is to take the first 50 elements,
     * and to not alter the scale.
     *
     * Please note: With scale and precision both at 100,
     * the execution takes a long time (~seconds).
     *
     * @param exponent  which power to calculate
     * @param precision represents the number of iterations to take when approximating result
     * @return Reference to *this
     * @throw unsupported_operation when attemted to power negative number with fractional number
     */
    number& pow(const number & exponent, const number & precision = number(50)){
        // fast path for exponent 0
        if(exponent.cmp_ignore_sig(number()) == 0){
            operator=(number(1));
        }
        else{
            if(exponent.fractional.all_of(0)){
                int_pow(exponent);
            }
            else{
                real_pow(exponent, precision);
            }
        }
        strip_zeroes();
        return *this;
    }

    /**
     * @brief binomial Calculates binomial coefficient ( n over k )
     *
     * You can affect the precision of this calculation by changing scale
     *
     * @param n The first input for the binomial
     * @param k The second input for the binomial
     * @return Result of the calculation
     */
    static number binomial(const number& n, const number & k){
        if(!k.fractional.all_of(0)){
            throw(unsupported_operation("Second parameter of binomial must be an integer"));
        }
        number result(1);
        for(number i=1;i<=k;++i){
            result *= (n-i+1);
            result /= i;
        }
        return result;
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
     * @param fractionals Number of fractional places to leave
     * @return Reference to *this
     */
    number& trunc(std::size_t fractionals = 0){
        if(fractionals<fractional.size()){
            fractional.resize(fractionals,0);
        }
        return *this;
    }

    /**
     * @brief Returns string representation of the object
     * @return  A newly constructed string
     */
    std::string str() const{
        std::stringstream acc("");
        acc << static_cast<unsigned int>(radix) << "::";
        if( ! isPositive ) acc << "-";
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
            else if(tmp == "@binomial"){
                if(stack.size()<2){
                    std::clog << "Not enough arguments for performing operation: " << tmp;
                    throw(invalid_expression_format("requested operation needs more parameters than available"));
                }
                number b(std::move(stack.back())); stack.pop_back();
                a = std::move(stack.back()); stack.pop_back();
                stack.push_back(binomial(a,b));
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
    _number whole; // BIG_ENDIAN element ordering
    /**
     * @brief fractional Stores the fractional part of the number in little endian
     */
    _number fractional;  // LITTLE_ENDIAN element ordering
    /**
     * @brief isPositive indicates sign of the number
     */
    bool isPositive;

    /**
     * @brief Compares two numbers whithout taking sign into account
     * @param other number to compare with
     * @return 0 if equal, n<0 if *this<other, n>0 if *this>other
     */
    int cmp_ignore_sig(const number &other) const{
        std::size_t pos = whole.size() - 1;
        std::size_t other_pos = other.whole.size() - 1;
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
            std::size_t limit = std::max(fractional.size() , other.fractional.size());
            while(pos < limit){
                if(fractional.get(pos) != other.fractional.get(pos)){
                    return (fractional.get(pos) > other.fractional.get(pos)) ? 1 : -1;
                }
                ++pos;
            }
            return 0;
        }
    }

    /**
     * @brief Raise this number to the power of integer exponent
     * @param exponent nubmer representing integer value
     */
    void int_pow(number exponent){
        std::size_t scalebak = scale;
        scale = 0; // we need wholepart division for int_pow
        // fast path for powerbases 0 and (-)1
        if(cmp_ignore_sig(number())==0 || cmp_ignore_sig(number(1))==0){
            if (! isPositive) { // 0 is treated as positive, therefore must be -1
                isPositive = ((exponent%number(2)).cmp_ignore_sig(number())==0);
            }
        }
        else{
            // negative exponent handling
            number others(1), help;
            number one(1),two(2);
            if(!exponent.isPositive){
                // convert the problem to positive power of inverse number
                exponent.isPositive = true;
                (*this) = one/(*this);
            }
            // divide and conquer - halve the exponent in each pass
            while(exponent > one){
                if(exponent%two == one) others *= (*this);
                help = (*this);
                operator*=(help);
                exponent/=two;
            }
            operator*=(others);
        }
        scale = scalebak; // revert to original scale
    }

    /**
     * @brief real_pow Raise this number to the power of non-integer exponent
     *
     * Setting scale affects precision of this operation
     *
     * @param exponent nubmer representing value of exponent
     * @param precision represents the number of iterations to take when approximating result
     */
    void real_pow(number exponent, const number & precision){
        if(!isPositive){
            throw(unsupported_operation("non-integer power of a negative number"));
        }
        number x,resRecipr(1),xcp;
        const number one(1);
        if(operator<(one)){
            // case (*this) > 1 gives better approximation
            operator=(one/(*this));
            exponent.isPositive = ! exponent.isPositive;
        }
        x = (((*this)-one)/(*this));
        x.isPositive = !x.isPositive; // unary - operation not available
        // k==0 case covered when initializing resRecipr:
        //  * power would be 1
        //  * binomial coefficient of anything over 0 is 1
        for (number k=1; k<precision;++k){
            xcp = x;
            xcp.int_pow(k);
            resRecipr+=(xcp * binomial(exponent,k));
        }
        operator=(one/resRecipr);
    }

    /**
     * @brief Divides this by other and returns either the result of division or modulo based on the div parameter
     * @param other number to divide this by
     * @param div whether division, or modulo shall be returned
     * @return Result of division if div is true and result of modulo otherwise
     */
    number& div_or_mod(number other,bool div){
        if(other == number()){
            throw(division_by_zero());
        }

        // Multiplies _number by scalar (unsigned) (big endian)
        // unify with muladd_vector_uint_uint ?
        auto muls_be =
                [](const _number & m1, unsigned int m2) -> _number{
            unsigned carry = 0;
            _number result;
            result.resize(m1.size(),0);
            std::size_t i;
            for(i = 0; i<m1.size(); ++i){
                carry += m2*m1.get(i);
                result.set(i,carry%radix);
                carry /= radix;
            }
            while(carry>0){
                result.push(carry%radix);
                carry /= radix;
            }
            return result;
        };

        // Less-or-equal comparison of big endian _numbers
        auto lt_be =
                [](const _number & n1, const _number & n2) -> bool{
            std::size_t i = std::max(n1.size(),n2.size());
            int a,b;
            for(; i>0; --i){
                if((a=n1.get(i-1))!=(b=n2.get(i-1))){
                    return a<b;
                }
            }
            return false;
        };

        // Subtracts 2 _numbers, returns borrow (modifies 1st operand)
        auto subref_be =
                [](_number& a, const _number& b) -> int{
            int borrow = 0,tmp;
            std::size_t i= 0;
            for(;i<b.size();++i){
                tmp = a.get(i) - b.get(i) - borrow;
                borrow = 0;
                while(tmp < 0){
                    tmp += radix;
                    ++borrow;
                }
                a.set(i,tmp);
            }
            for(;i<a.size();++i){
                if(borrow == 0) return 0;
                tmp = a.get(i) - borrow;
                borrow = 0;
                while(tmp < 0){
                    tmp += radix;
                    ++borrow;
                }
                a.set(i,tmp);
            }
            return borrow;
        };

        // Adds 2 _numbers, ignores last carry, if any
        auto addref_ic_be =
                [](_number& a, const _number& b) -> void{
            int carry = 0;
            std::size_t i = 0;
            for(;i<b.size();++i){
                carry += a.get(i) + b.get(i);
                a.set(i,carry%radix);
                carry /= radix;
            }
            for(;i<a.size();++i){
                if(carry == 0) return;
                carry += a.get(i);
                a.set(i,carry%radix);
                carry /= radix;
            }
        };

        // shifts _number to the right (arithmetic shift in base radix)
        auto shift_append_be =
                [](const _number cp, unsigned int shft) -> _number{
            _number rv;
            rv.resize(cp.size()+shft,0);
            for(std::size_t i=0; i<cp.size(); ++i){
                rv.set(i+shft,cp.get(i));
            }
            return rv;
        };

        std::size_t wbs = whole.size(), fbs = fractional.size();
        // amount of dividend's whole part and franctional part significant digits
        std::size_t wds = other.whole.size(), fds = other.fractional.size();
        // same for divisor
        std::size_t fdiff;
        // difference in amount of significant digits in fractional part
        std::size_t fbshift,fdshift;
        // amount of digits shifted from fractional part: b-dividend, d-divisor

        // calc wbs, then wds:
        while(wbs>0 && whole.get(wbs-1)==0) --wbs;
        while(wds>0 && other.whole.get(wds-1)==0) --wds;
        // calc fbs, then fds:
        while(fbs>0 && fractional.get(fbs-1)==0) --fbs;
        while(fds>0 && other.fractional.get(fds-1)==0) --fds;
        // calc fdiff and shifts:
        fdiff = (fbs>fds?fbs-fds:0);
        // if divisor has more significant digits, shift must be at least as long
        fbshift = (fdiff>scale?fbs:fds+scale); // account for scale here
        fdshift = fbshift-scale;

        // Algo from Knuth vol. 2 - "long division"
        _number u, v; // all big endian
        std::size_t cp_i; // copying idex
        // u - dividend, v - divisor
        // copy and shift dividend - fractional part first:
        for(cp_i = fbshift; cp_i>0; --cp_i){
            u.push(fractional.get(cp_i-1));
        }
        if( wbs > 0 ){
            for(cp_i = 0; cp_i<wbs; ++cp_i){
                u.push(whole.get(cp_i));
            }
        }
        else{ // strip possible leading zeroes if no digits in whole part
            while(u.back()==0) u.pop();
        }
        // copy and shift divisor:
        for(cp_i = fdshift; cp_i>0; --cp_i){
            v.push(other.fractional.get(cp_i-1));
        }
        if( wds > 0 ){
            for(cp_i = 0; cp_i<wds; ++cp_i){
                v.push(other.whole.get(cp_i));
            }
        }
        else{ // strip possible leading zeroes if no digits in whole part
            // SIGFPE occurs if this action is omitted
            while(v.back()==0) v.pop();
        }

        // Dividend is less than divisor after alignment
        if(lt_be(u,v)){
            if (div){
                operator=(number(0));
                isPositive = true;
            }
        }
        else{
            std::size_t m,n;
            int cq,borrow;
            // cq - candidate quotient in given step
            _number q, shiftedv,tmp;
            // q - quotient
            n = v.size();
            m = u.size() - n;
            // 1. Normalize
            unsigned int d = radix/(v.back()+1);
            u = muls_be(u,d);
            v = muls_be(v,d);
            const int v0 = v.back(); // MSD of divisor
            // 2. Initialize j, 7. Loop on j
            for(std::size_t j = m+1; j>0; --j){

                // 3. Calculate ^q
                if(u.get(n+j-1)==v0){
                    cq = radix-1;
                }
                else{
                    cq = (radix*u.get(n+j-1)+u.get(n+j-2))/v0;
                }
                // 3.-4. Check if cq isn't too big(3) and multiply(4)
                shiftedv = shift_append_be(v,j-1);
                if(lt_be(u, (tmp=muls_be(shiftedv,cq)))){
                    --cq;
                    subref_be(tmp,shiftedv);
                }
                // 4. Sub
                borrow = subref_be(u,tmp);

                // 5. Test remainder
                if (borrow > 0){

                    // 6. Add back
                    --cq;
                    addref_ic_be(u,shiftedv);
                }
                q.set(j-1,cq);
                if(u.all_of(0)) break;
            }

            // 8. Unnormalize
            whole.clear();
            fractional.clear();
            std::size_t boundary;
            if (div){
                isPositive = isPositive==other.isPositive;
                boundary = (q.size()>scale?q.size()-scale:0);
                for(cp_i = boundary; cp_i>0; --cp_i){
                    whole.set(cp_i-1,q.get(cp_i-1+scale));
                }
                if(whole.size()==0) whole.push(0);
                for(cp_i = scale; cp_i>0; --cp_i){
                    fractional.push(q.get(cp_i-1));
                }
            }
            else{
                // unnormalize must divide remainder of u by d (one digit integer division):
                std::size_t r_i = u.size(); // remainder index
                int carry = 0;
                for(;r_i>0;--r_i){
                    carry += u.get(r_i-1);
                    u.set(r_i-1,carry/d);
                    carry %= d;
                    carry *= radix;
                }
                boundary = (u.size()>fbshift?u.size()-fbshift:0);
                for(cp_i = boundary; cp_i > 0; --cp_i){
                    whole.set(cp_i-1,u.get(cp_i-1+fbshift));
                }
                if(whole.size()==0) whole.push(0);
                for(cp_i = fbshift; cp_i > 0; --cp_i){
                    fractional.push(u.get(cp_i-1));
                }
            }
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

