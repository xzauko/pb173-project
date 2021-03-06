/**
 * @file   fixedpoint.h
 * @author Michal Pochobradský
 * @author Tibor Zauko
 * @brief Single include implementation of fixedpoint arithmetic library.
 */

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


template<unsigned char radix>
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
        cela_cast{digits[0]},
        des_cast{},
        isPositive(true)
    {
        static_assert(radix<=MAX_RADIX, "fixedpoint::number's radix too high");
        static_assert(radix>=2, "fixedpoint::number's radix is too low, use at least 2");
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
        std::string whole,decimal;
        bool onDecimal = false;
        for (;start!=src.cend();++start){
            if (onDecimal){
                decimal += *start;
            }
            else{
                if (values[static_cast<int>(*start)] == -2) onDecimal = true;
                else whole += *start;
            }
        }
        // reverse whole part (so it meets storage requirement):
        std::reverse(whole.begin(), whole.end());
        // convert here:
        if (rdx == radix){
            // only need to assign to members and maybe resize des_cast
            cela_cast = std::move(whole);
            des_cast = std::move(decimal);
            if (fracnum >= 0) des_cast.resize(fracnum, digits[0]);
        }
        else{
            // convert from base rdx to base radix:
            // convert radix to base rdx:
            if (fracnum < 0) fracnum = std::ceil(static_cast<double>(rdx)/radix)*decimal.size();
            auto res = convert_with_vector(whole, decimal, rdx, fracnum);
            cela_cast = std::move(res.first);
            des_cast = std::move(res.second);
        }
        strip_zeroes();
    }

    /**
     * @brief Integral type constructor
     * @param x value to construct number with
     */
    template<typename T, typename = decltype(static_cast<std::true_type>(std::is_integral<T>()))>
    number(T x):
        des_cast{},
        isPositive(x>=0)
    {
        static_assert(radix<=MAX_RADIX, "fixedpoint::number's radix too high");
        static_assert(radix>=2, "fixedpoint::number's radix is too low, use at least 2");
        std::string rep = std::to_string(x); // obtain string representation
        std::reverse(rep.begin(), rep.end()); // reverse string - BIG ENDIAN storage
        if (!isPositive) rep.pop_back(); // get rid of - sign
#if !(defined(FIXEDPOINT_CASE_INSENSITIVE) || defined(FIXEDPOINT_CASE_SENSITIVE))
        // only perform the following
        // if we can't suppose anything about digits[]
        for (auto x = rep.begin(); x!=rep.end(); ++x){
            // convert from digits 0-9 to whatever our encoding uses
            *x = digits[*x-'0'];
        }
#endif
        if (radix == 10){
            cela_cast = rep;
        }
        else{
            cela_cast = convert_with_vector(rep,"",10,0).first;
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
            cela_cast = whole;
            des_cast = decimal;
        }
        else{
            auto res = convert_with_vector(whole,decimal,10,fracnum);
            cela_cast = std::move(res.first);
            des_cast = std::move(res.second);
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
            size_t boundary = std::min(des_cast.size(), other.des_cast.size());

            // DESATINNA CAST
            if (des_cast.size() < other.des_cast.size()){
                // possibly partial copy
                des_cast.append(other.des_cast,
                                boundary,
                                other.des_cast.size() - boundary);
            }
            int tmp, carry = 0;
            int ourss, theirss;
            for ( size_t i = boundary; i>0; --i ){ // easier than with iterators
                ourss = static_cast<int>(des_cast[i-1]);
                theirss = static_cast<int>(other.des_cast[i-1]);
                tmp = values[ourss] + values[theirss] + carry;
                carry = tmp / radix;
                des_cast[i-1] = digits[tmp % radix];
            }

            // CELA CAST
            if ( other.cela_cast.size() > cela_cast.size()){
                cela_cast.resize( other.cela_cast.size(), digits[0] );
            }
            auto our = cela_cast.begin();
            auto their = other.cela_cast.begin();
            for ( ; our != cela_cast.end() ; ++our){
                ourss = static_cast<int>(*our);
                tmp = values[ ourss ] + carry;
                if ( their != other.cela_cast.end() ){
                    theirss = static_cast<int>( *( their++ ));
                    tmp += values[ theirss ];
                }
                carry = tmp / radix;
                *our = digits[ tmp % radix ];

            }
            if ( carry > 0 ){
                cela_cast.push_back(digits[carry]);
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
        if(cmp_ignore_sig(o1) == -1){//odečítám větší (v abs hodnotě) od menšího, tak je prohodím
            swap(o1);
            isPositive = !isPositive;
        }
        //dále už předpokladam, že odečítám menší od většího
        size_t boundary = o1.des_cast.size();
        // DESATINNA CAST
        if (des_cast.size() < boundary){
            des_cast.resize( boundary, digits[0] );
        }
        int tmp, carry = 0;
        for ( size_t i = boundary; i>0; --i ){ // easier than with iterators
            tmp = values[static_cast<int>(des_cast[i-1])] - values[static_cast<int>(o1.des_cast[i-1])] + carry;
            if(tmp < 0) {
                carry = -1;
                tmp +=radix;
            }else {
                carry = 0;
            }
            des_cast[i-1] = digits[tmp];
        }

        // CELA CAST
        if ( o1.cela_cast.size() > cela_cast.size()){
            cela_cast.resize( o1.cela_cast.size(), digits[0] );
        }
        size_t index = 0;
        size_t first_digit = 0;
        for ( index = 0; index < cela_cast.size() ;index++){
            tmp = values[static_cast<int>( cela_cast.at(index))] + carry;
            if ( index < o1.cela_cast.size() ){
                tmp -= values[static_cast<int>( o1.cela_cast.at(index)) ];
            }
            if(tmp < 0) {
                carry = -1;
                tmp +=radix;
            }else {
                carry = 0;
            }
            if(0 != tmp) first_digit = index;
            cela_cast.at(index) =  digits[tmp];

        }
        cela_cast.resize(first_digit + 1, digits[0] );
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
            cela_cast = other.cela_cast;
            des_cast = other.des_cast;
        }
        else if ( (other.cmp_ignore_sig(one) == 0) ||
                  (cmp_ignore_sig(zero) == 0) ){
            return *this;
        }
        else{
            //čísla reprezentuji jako zlomky x/y, kde y má formát 100...0
            //pocet desetinych míst
            size_t decimals = std::max(des_cast.size(), other.des_cast.size());
            //ocekavana maximalni velikost výsledku
            size_t size = (decimals + cela_cast.size()) + (decimals + other.cela_cast.size());
            // konecna velkost desatinnej casti:
            size_t endfrac = std::max(des_cast.size(), other.des_cast.size());

            const number a(*this);
            const number& b = other;

            const size_t dec_point = 2 * (decimals);
            des_cast.clear();
            des_cast.resize(dec_point, digits[0] );
            cela_cast.clear();
            cela_cast.resize(size, digits[0] );

            auto product = [&dec_point, this](size_t i) -> char&{
                if(i <dec_point){
                    i = dec_point - i -1;
                    return des_cast.at(i);
                }else{
                    i = i - dec_point;
                    return cela_cast.at(i);
                }
            };
            auto get = [&decimals](const number &from,size_t i) -> const char&{
                if(i <decimals){
                    i = decimals - i -1;
                    if(i < from.des_cast.size()) return from.des_cast.at(i);
                    //implicitní nuly
                    else return digits[0];
                }else{
                    i = i - decimals;
                    if(i < from.cela_cast.size()) return from.cela_cast.at(i);
                    //implicitní nuly
                    else return digits[0];

                }
            };

            const size_t p = b.cela_cast.size() + decimals;
            const size_t q = a.cela_cast.size() + decimals;
            for(size_t b_i = 0; b_i < p; b_i++){
                unsigned char carry= 0;
                for(size_t a_i = 0; a_i < q; a_i++){
                    unsigned long tmp = values[static_cast<int>(product(a_i + b_i))];
                    tmp += carry + values[static_cast<int>(get(a, a_i ))] * values[static_cast<int>(get(b, b_i))];
                    carry = tmp / radix;
                    product(a_i + b_i) = digits[tmp % radix];
                }
                unsigned long tmp = values[static_cast<int>(product(b_i + q))];
                tmp += carry;
                product(b_i + q) = digits[tmp];
            }
            size_t pos = cela_cast.find_last_not_of(digits[0]);
            if(pos != cela_cast.npos) pos += 1;
            else pos = 1;
            cela_cast.resize(pos, digits[0]);
            des_cast.resize(endfrac, digits[0]);
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
        // kontrola desatinnej casti:
        if(! std::all_of(exponent.des_cast.cbegin(),
                         exponent.des_cast.cend(),
                         [](const char x){return x==digits[0];}
                         )){
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
            //vyřeším záporný exponent
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
        if( !std::all_of( des_cast.cbegin(),
                          des_cast.cend(),
                          [](char x){return x==digits[0];}) ){
            if (! isPositive){
                operator--();
            }
        }
        des_cast.clear();
        return *this;
    }

    /**
     * @brief Ceils the number
     * @return Reference to *this
     */
    number& ceil(){
        if( !std::all_of( des_cast.cbegin(),
                          des_cast.cend(),
                          [](char x){return x==digits[0];}) ){
            if (isPositive){
                operator++();
            }
        }
        des_cast.clear();
        return *this;
    }

    /**
     * @brief Truncates the number
     * @return Reference to *this
     */
    number& trunc(){
        des_cast.clear();
        return *this;
    }

    /**
     * @brief Returns string representation of the object
     * @return  A newly constructed string
     */
    std::string str() const{
        std::stringstream acc("");
        std::string reversewhole;
        reversewhole.append(cela_cast.crbegin(), cela_cast.crend());
        acc << static_cast<unsigned int>(radix) << "::";
        if( ! isPositive ) acc << "-";
        acc << reversewhole;
        if( ! des_cast.empty() ){
            acc << "." << des_cast;
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
        // xzauko number<16>::eval_infix("10::23 + 2::100010")
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
        std::istringstream inp(expr);
        //while(x!=expr.cend() && *x==' ') ++x; //skip spaces at front of string
        auto x = expr.cbegin();
        string now;
        //while (x!=expr.cend()){
        while( getToken(now,x) ){
            //while(x!=expr.cend() && *x==' ') ++x; //skip spaces after expression
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
            else{ //it's a number or something in place of a number
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
        cela_cast.swap(other.cela_cast);
        des_cast.swap(other.des_cast);
        std::swap(isPositive, other.isPositive);
    }

private:
    std::string cela_cast; // BIG_ENDIAN element ordering
    std::string des_cast;  // LITTLE_ENDIAN element ordering
    bool isPositive;

    /**
     * @brief Compares two numbers whithout taking sign into account
     * @param other number to compare with
     * @return 0 if equal, n<0 if *this<other, n>0 if *this>other
     */
    int cmp_ignore_sig(const number &other) const{
        //přeskočení případných nul na začátku
        size_t pos = cela_cast.find_last_not_of('0');
        size_t other_pos = other.cela_cast.find_last_not_of('0');
        if(cela_cast.npos == pos) pos=0;
        if(other.cela_cast.npos == other_pos) other_pos=0;
        if(pos != other_pos){
            return (pos > other_pos) ? 1 : -1;
        }else{
            while(pos > 0){
                if(values[static_cast<int>(cela_cast[pos])] != values[static_cast<int>(other.cela_cast[pos])]){
                    return (values[static_cast<int>(cela_cast[pos])] > values[static_cast<int>(other.cela_cast[pos])]) ? 1 : -1;
                }
                pos--;
            }
            //porovnání nultých prvků celých částí
            if(values[static_cast<int>(cela_cast[pos])] != values[static_cast<int>(other.cela_cast[pos])]){
                return (values[static_cast<int>(cela_cast[pos])] > values[static_cast<int>(other.cela_cast[pos])]) ? 1 : -1;
            }
            //cela cast je stejna, rozhodne desetina cast
            size_t limit = (des_cast.size() < other.des_cast.size()) ? des_cast.size() : other.des_cast.size();
            while(pos < limit){
                if(values[static_cast<int>(des_cast[pos])] != values[static_cast<int>(other.des_cast[pos])]){
                    return (values[static_cast<int>(des_cast[pos])] > values[static_cast<int>(other.des_cast[pos])]) ? 1 : -1;
                }
                pos++;
            }
            //porovnání proti implicitním nulám
            if(other.des_cast.size() > limit){
                for (;pos < other.des_cast.size();pos++) {
                    if (values[static_cast<int>(other.des_cast[pos])] > 0) return -1;
                }
            }
            if(des_cast.size() > limit){
                for (;pos < des_cast.size();pos++) {
                    if (values[static_cast<int>(des_cast[pos])] > 0) return 1;
                }
            }
            return 0;
        }
    }

    /**
     * @brief Rise this number to the power of positive integer exponent
     * @param exponent nubmer representing positive integer value
     */
    void int_pow(number && exponent){
        number nula(0);
        number orig(*this);
        //naivní algoritmus
        while(exponent > nula){
            operator*=(orig);
            --exponent;
        }
    }

    /**
     * @brief Divides this by other and returns either the result of division or modulo based on the div parameter
     * @param other number to divide this by
     * @param div whether division, or modulo shall be returned
     * @return Result of division if div is true and result of modulo otherwise
     */
    number& div_or_mod(const number other,bool div){
        std::string result;
        number divisor;
        number dividend;

        //dostatečný posun abych obsáhl všechna desetiná čísla, plus nová des. čísla zbytku v případě nenulového scale
        dividend.des_cast.assign(cela_cast.rbegin(), cela_cast.rend());
        dividend.des_cast.append(des_cast);

        divisor.des_cast.assign(other.cela_cast.rbegin(), other.cela_cast.rend());
        size_t shift = 0;
        if(divisor == 0){
            shift = other.des_cast.find_last_not_of(digits[0]);

            if (std::string::npos == shift) throw division_by_zero();

            divisor.des_cast.assign(other.des_cast.substr(shift));
            //upřímě netuším proč shift++, ale jinak to vrací výsledek o řád menší než by měl být
            shift++;
        }else{
            divisor.des_cast.append(other.des_cast);
        }

        size_t dividend_size = cela_cast.size();
        long long deviation = dividend_size - other.cela_cast.size();

        //počet kroků dělení
        int steps = deviation + scale + shift;

        if (steps < 0){//dělitel je řádově větší, takže celé tohle číso je zbytek
            if(div) *this = 0;
            return *this;
        }

        for(int i = 0; i <= steps;i++){
            size_t tmp=0;
            while(dividend.cmp_ignore_sig(divisor) >= 0){
                tmp++;
                dividend -= divisor;
            }
            result.push_back(digits[tmp]);
            divisor.des_cast.insert(0, 1, digits[0]);
        }
        if(div){
            int move = result.size() - scale;
            if(move>0){
                cela_cast.assign(result.rbegin()+scale,result.rend());
                des_cast.clear();
                des_cast.append(result,move, std::string::npos);
            }else{
                des_cast.append(-move,digits[0]);
                des_cast.append(result);
            }

            isPositive = (isPositive == other.isPositive);
        }else{
            std::string tmp= dividend.des_cast.substr(0,cela_cast.size());
            std::reverse(tmp.begin(),tmp.end());
            cela_cast = std::move(tmp);
            des_cast = dividend.des_cast.substr(cela_cast.size());
            scale = 0;
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
     * Always truncates any trailing zeroes.
     *
     * If only one digit stays, that is a singular 0 in cela_cast,
     * also sets isPositive to true, so we are consistent with sign of zero.
     */
    void strip_zeroes(){
        std::size_t pos;
        bool isZero;
        if((pos=cela_cast.find_last_not_of(digits[0])) != cela_cast.npos){
            cela_cast.resize(pos+1,digits[0]);
            isZero = false;
        }
        else{
            cela_cast.resize(1,digits[0]);
            isZero = true;
        }
        // leading zeroes stripped
        if ((pos=des_cast.find_last_not_of(digits[0])) == des_cast.npos){
            des_cast.clear();
            // isZero stays as is true && isZero always evals as true
        }
        else {
            des_cast.resize(pos+1,digits[0]);
            isZero = false; // false && isZero always evals as false
        }
        if (isZero) isPositive = true; // 0 is treated as positive
        // trailing zeroes truncated
    }

    /**
     * @brief Converts strings of whole and decimal parts
     * <p>
     * Conversion of string a to string b is achieved by utilizing
     * the muladd_vector_uint_uint method for the whole part.
     * For the whole part this method merely handles
     * digit -> value and value -> digit conversion and related tasks.
     * For the decimal part this function also performs minor tasks related to
     * conversion
     * <p>
     * @param srcWhole   Source string to convert - whole part as BIG ENDIAN
     * @param srcDec     Source string to convert - decimal part as LITTLE ENDIAN
     * @param rdx   Number base (radix) of source string
     * @param scale Number of decimal places to compute
     * @return  Pair of holding converted number representation
     */
    static std::pair<std::string, std::string> convert_with_vector(
            const std::string & srcWhole,
            const std::string & srcDec,
            unsigned int rdx,
            unsigned int scale){
        std::vector<unsigned int> vec;
        for(auto x=srcWhole.crbegin(); x!=srcWhole.crend();++x){
            muladd_vector_uint_uint(vec,rdx,values[static_cast<int>(*x)]);
        }
        std::string retValWhole;
        for (auto x : vec){
            retValWhole.push_back(digits[x]);
        }
        std::string retValDec;
        vec.clear();
        for(auto x=srcDec.crbegin(); x!=srcDec.crend();++x){
            vec.push_back(values[static_cast<int>(*x)]);
        }
        std::size_t orig_size = vec.size();
        unsigned int digitConverter = 0;
        for(std::size_t i=0;i<scale;++i){
            muladd_vector_uint_uint(vec,radix,0,rdx);
            digitConverter = 0;
            while(vec.size() > orig_size){
                // does what muladd_vector_uint_uint does, but on small scale
                digitConverter *=rdx;
                digitConverter += vec.back();
                vec.pop_back();
            }
            retValDec.push_back(digits[digitConverter]);
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
            std::vector<unsigned int> & vec,
            unsigned int mul,
            unsigned int add,
            unsigned int max_element = radix){
        unsigned int carry = 0;
        // multiply:
        for(auto x = vec.begin(); x != vec.end(); ++x){
            carry += (*x)*mul;
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
        while (carry!=0) {
            vec.push_back(carry % max_element);
            carry /= max_element;
        }
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

