#ifndef FIXEDPOINT_H
#define FIXEDPOINT_H
#include <string>
#include <vector> // in convert_through_native
#include <climits> // UINT_MAX
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
static const signed char values[128] = {
//   0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x1
    -1, -1, -1, 62, 63, -1, -1, -1, -1, -1, -1, -1, -2, -1, -2, -1, // 0x2
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1, // 0x3
    -1, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, // 0x4
    51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1  // 0x5
    -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, // 0x6
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1, // 0x7
};
// -2: desatinne oddelovace (. a ,), -1: neplatna cifra,
// ostatne: hodnota cislice, ak >= sustave - neplatna

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
// mapovanie hodnota -> cislica
#else
#error "FIXEDPOINT_H: Conflicting case sensitivity setting, please choose only one"
#endif

// Exceptions:

/**
 * @brief The invalid_number_format struct is an exception related to number struct
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
 * If this exception is thrown, it means that a string supposed to encode
 * a number specifies a radix that is out of the supported range of radixes.
 */
struct radix_too_high: public std::runtime_error{
    radix_too_high():std::runtime_error("the specified radix is too high"){}
};

template<unsigned char radix>
/**
 * <b>The number struct represents the fixedpoint numbers</b>
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
 * </ol>
 */
struct number{

    number():isPositive(true){
        static_assert(radix<=MAX_RADIX && radix!=0, "fixedpoint::number's radix too high");
    }
    /**
     * @brief number
     * @param src
     */
    number(const std::string & src): // number<16>("18::Ged");
        isPositive(src.find_first_of('-')==src.npos)
    {
        static_assert(radix<=MAX_RADIX && radix!=0, "fixedpoint::number's radix too high");
        // basic format verification:
        if (std::count_if(
                    src.cbegin(),
                    src.cend(),
                    [](const char c){return values[static_cast<int>(c)]==-2;}
                    ) > 1){
            throw(invalid_number_format("contains more than one decimal separator"));
        }
        auto start = src.cbegin();
        std::size_t rdx = radix,rdxend;
        if (*start=='-') ++start; // negative check already happened
        if ((rdxend=src.find_first_of(':'))!=src.npos){ // has radix specified
            std::stringstream x("");
            while(*start != ':'){
                if(*start<'0' || '9'<*start) {
                    throw(invalid_number_format("src's radix is ill formatted"));
                }
                x << *start;
                ++start;
            }
            // start is now in sync with rdxend
            x >> rdx; // radix is always specified as decimal number
            if ( rdx > MAX_RADIX ) {
                throw(radix_too_high());
            }
            ++start;  ++rdxend;// goes to second colon
            if (*start != ':'){
                throw(invalid_number_format("missing 2nd colon in separator"));
            }
            ++start; ++rdxend;
        }
        else rdxend = 0;
        // number validity check:
        if (! isPositive && src[rdxend]=='-'){
            // - sign was not read yet
            ++start;
        }
        if (std::any_of(
                    start,
                    src.cend(),
                    [rdx = static_cast<int>(rdx)](const char c){
                    return (values[static_cast<int>(c)]==-1 &&
                            (values[static_cast<int>(c)]<rdx));
                    }))
        {
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
            // only need to assign to members
            cela_cast = std::move(whole);
            des_cast = std::move(decimal);
        }
        else{
            // convert from base rdx to base radix:
            // convert radix to base rdx:
            cela_cast = convert_with_vector(whole, rdx);
        }
        strip_zeroes_and_fix_scale();
    }
    /**
     * @brief operator =
     * @param src
     * @return
     */
    /*number& operator =(const std::string & src){
        number x(src);
        swap(x);
        return *this;
    }*/

    /**
     * @brief number
     * @param x
     */
    number(long long int x):
        number(static_cast<unsigned long long int>((x>=0)?x:-x))
    {
        isPositive = (x>=0);
    }

    /**
     * @brief number
     * @param x
     */
    number(unsigned long long int x){
        static_assert(radix<=MAX_RADIX && radix!=0, "fixedpoint::number's radix too high");
        std::stringstream src("");
        src << "10::";
        src << x;
        number y(number(src.str()));
        cela_cast = y.cela_cast;
        des_cast = y.des_cast;
        isPositive = y.isPositive;
    }
    /**
     * @brief number
     * @param x
     * @param scale
     */
    number(double x, unsigned int scale = 0);/*:scale(scale);{
        static_assert(radix<=MAX_RADIX && radix!=0, "fixedpoint::number's radix too high");
        std::stringstream src("10::");
        src<<x;
        number(src.str());
        if (scale>0){
            des_cast.resize(scale,digits[0]);
        }
    }*/

    number(const number &) = default;
    number(number &&) = default;
    number& operator =(const number &) = default;
    number& operator =(number &&) = default;

    /*
     * ostatne konstruktory a operatory priradenia (copy, move),
     * mozno konstruktory a operatory aj pre ciselne typy
     * POZN.: pre konstruktor double asi vyhodne pouzit stringstream
     */

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
            if (des_cast.size() > other.des_cast.size()){
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
        strip_zeroes_and_fix_scale();
        return *this;
    }

    /*number& operator+=(const std::string & other){ // ??
        number onum(other);
        *this += onum;
        return *this;
    }*/


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
            cela_cast.resize( boundary, digits[0] );
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
        // At worst 1 copy
        // or 1 copy and whatever += does
        return *this;
    }

    number & operator *=(const number &); // xpocho
    number & operator /=(const number &);
    number & operator %=(const number &);
    number & operator ^=(const number &); // xpocho

    number& operator ++(); // xzauko
    number& operator --(); // xzauko
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
        bool tmpResult = ( isPositive == rhs.isPositive &&
                           cela_cast == rhs.cela_cast );
        if ( tmpResult ){
            if ( des_cast.size() == rhs.des_cast.size() ){
                tmpResult = ( des_cast == rhs.des_cast );
            }
            else {
                tmpResult = false;
            }
        }
        return tmpResult;
    }

    /**
     * @brief str   returns string representation of the object
     * @return  a newly constructed string
     */
    std::string str() const{
        std::stringstream acc("");
        std::string reversewhole;
        std::reverse_copy(cela_cast.cbegin(),
                          cela_cast.cend(),
                          std::back_inserter(reversewhole));
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
     * @brief eval_postfix
     * @return
     */
    static number eval_postfix(const std::string &); // xzauko

    /**
     * @brief eval_infix
     * @return
     */
    static number eval_infix(const std::string &); // xzauko number<16>::eval_infix("10::23 + 2::100010")

    template<unsigned char oradix>
    /**
     * @brief convert
     * @return
     */
    static number convert(const number<oradix> & other){
        number result(other.str());
        return result;
    }

    void swap( number& other ){
    	cela_cast.swap(other.cela_cast);
    	des_cast.swap(other.des_cast);
        //using std::swap;
        //swap(isPositive, other.isPositive);
        std::swap(isPositive, other.isPositive);
    }

private:
    std::string cela_cast; // BIG_ENDIAN element ordering
    std::string des_cast;  // LITTLE_ENDIAN element ordering
    bool isPositive;
    std::size_t scale = 0; // 0 - default scaling, >0 always strip des_cast to scale

    /**
     * @brief cmp_ignore_sig
     * @param other
     * @return
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
    		//cela cast je stejna, rozhodne desetina cast
    		size_t limit = (des_cast.size() > other.des_cast.size()) ? des_cast.size() : other.des_cast.size();
    		while(pos < limit){
    			if(values[static_cast<int>(des_cast[pos])] != values[static_cast<int>(other.des_cast[pos])]){
    	    		return (values[static_cast<int>(des_cast[pos])] > values[static_cast<int>(other.des_cast[pos])]) ? 1 : -1;
    			}
    			pos++;
    		}
    		if(des_cast.size() != other.des_cast.size()){
	    		return (des_cast.size() > other.des_cast.size()) ? 1 : -1;
    		}else{
    			return 0;
    		}
    	}
    }

    /**
     * @brief strip_zeroes_and_fix_scale Strip leading 0 and maintain scale
     * If value of whole part is equal to 0, then strips all but the last
     * leading 0 digit, otherwise strips all leading zeroes.
     * If scale is set as nonzero, it strips or extends the "decimal" part
     * to lenght equal to scale (extends with digit of value 0, stripping
     * truncates), otherwise always truncates any trailing zeroes.
     */
    void strip_zeroes_and_fix_scale(){
        std::size_t pos;
        if((pos=cela_cast.find_last_not_of(digits[0]))!=cela_cast.npos){
            if (pos>=1) cela_cast.resize(pos+1,digits[0]);
        }
        else{
            cela_cast.resize(1,digits[0]);
        }
        // leading zeroes stripped
        if (scale != 0){
            des_cast.resize(scale, digits[0]);
        }
        else {
            pos = des_cast.find_last_not_of(digits[0]);
            if (pos == des_cast.npos) des_cast.clear();
            else des_cast.resize(pos+1,digits[0]);
        }
        // trailing zeroes truncated
    }

    /**
     * @brief convert_through_native Converts string of whole part between bases
     * <p>
     * Conversion of string a to string b is achieved by utilizing
     * the muladd_vector_uint_uint method. This method merely handles
     * digit -> value and value -> digit conversion and related tasks.
     * <p>
     * @param src   Source string to convert
     * @param rdx   Number base (radix) of source string
     * @return  Newly constructed string holding converted number representation
     */
    static std::string convert_with_vector(
            const std::string & src,
            unsigned int rdx){
        std::vector<unsigned int> vec;
        for(auto x=src.crbegin(); x!=src.crend();++x){
            muladd_vector_uint_uint(vec,rdx,values[static_cast<int>(*x)]);
        }
        std::string retVal;
        for (auto x : vec){
            retVal.push_back(digits[x]);
        }
        return retVal;
    }

    /**
     * @brief divmod_vector_uint Divides a vector by an uint, returns remainder
     * @param vec   Dividend, after return holds the quotient
     * @param div   Divisor
     * @return  Remainder
     */
    static unsigned int divmod_vector_uint(
            std::vector<unsigned int> & vec,
            unsigned int div){
        unsigned long long int rem = 0,uint_size=1;
        uint_size += UINT_MAX;
        for(auto x = vec.rbegin();x!=vec.rend();++x){
            rem *= uint_size;
            rem += *x;
            *x = rem / div;
            rem %= div;
        }
        while(vec.back()==0) {
            vec.pop_back();
            if (vec.empty()) break;
        }
        return static_cast<unsigned int>(rem);
    }

    /**
     * @brief muladd_vector_uint_uint Multiplies vector by uint, then adds another uint
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
        if (carry!=0) vec.push_back(carry);
        // add:
        carry = add;
        for (auto x = vec.begin(); x != vec.end(); ++x){
            carry += *x;
            *x = carry % max_element;
            carry /= max_element;
        }
        if (carry > 0){
            vec.push_back(carry);
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

template<unsigned char radix>
number<radix> operator -(const number<radix> & lhs,
                         const number<radix> & rhs){
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

template<unsigned char radix>
number<radix> operator /(const number<radix> & lhs,
                         const number<radix> & rhs){
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

template<unsigned char radix>
number<radix> operator ^(const number<radix> & lhs,
                         const number<radix> & rhs){
    number<radix> newnum(lhs);
    newnum^=rhs;
    return newnum;
}


//number operator+(const number &, const std::string &); ??
//number operator+(const std::string &, const number &); ??

/*
 * Mozno aj operatory pre aritmeticky posun - bc ich nema:
 * number operator<<(const number &, int);
 * number operator>>(const number &, int);
 */

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

#endif // FIXEDPOINT_H

