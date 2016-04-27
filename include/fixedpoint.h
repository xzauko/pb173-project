#ifndef FIXEDPOINT_H
#define FIXEDPOINT_H
#include <string>
#include <ostream>
#include <istream>
#include <sstream>

#if ! ( defined(FIXEDPOINT_CASE_SENSITIVE) || defined(FIXEDPOINT_CASE_INSENSITIVE) )
#define FIXEDPOINT_CASE_INSENSITIVE
#endif

namespace fixedpoint{

#if defined( FIXEDPOINT_CASE_INSENSITIVE ) && ! defined( FIXEDPOINT_CASE_SENSITIVE )
const signed char values[128] = {
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

const char digits[36] = {
//   0    1    2    3    4    5    6    7    8    9
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', // 0
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', // 1
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', // 2
    'u', 'v', 'w', 'x', 'y', 'z'                      // 3
};
// mapovanie hodnota -> cislica
#elif defined( FIXEDPOINT_CASE_SENSITIVE ) && ! defined( FIXEDPOINT_CASE_INSENSITIVE )
const signed char values[128] = {
//   0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x1
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -1, -2, -1, // 0x2
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1, // 0x3
    -1, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, // 0x4
    51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1  // 0x5
    -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, // 0x6
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1, // 0x7
};
// -2: desatinne oddelovace (. a ,), -1: neplatna cifra,
// ostatne: hodnota cislice, ak >= sustave - neplatna

const char digits[62] = {
//   0    1    2    3    4    5    6    7    8    9
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', // 0
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', // 1
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', // 2
    'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', // 3
    'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', // 4
    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', // 5
    'Y', 'Z'                                          // 6
};
// mapovanie hodnota -> cislica
#else
#error "FIXEDPOINT_H: Conflicting case sensitivity setting, please choose only one"
#endif


template<unsigned char radix = 10>
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
 * </ul>
 * The lowercase letters are preferred for string representation in case
 * insensitive mode.
 *
 * <p><p>
 * Notes:
 * <p>
 * <ol>
 * <li>implementation caveat - maximum supported system is base-36 (or base-62),
 * due to lack of suitable ASCII chaacters to express numerals</li>
 * </ol>
 */
struct number{

    number():isPositive(true){}
    number(const std::string &); // 16::
    number& operator =(const std::string &);

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
            return *this;
        }
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
            theirss = static_cast<int>( *( their++ ));
            tmp = values[ ourss ] + carry;
            if ( their != other.cela_cast.end() ){
                tmp += values[ theirss ];
            }
            carry = tmp / radix;
            *our = digits[ tmp % radix ];

        }
        if ( carry > 0 ){
            cela_cast.push_back(digits[carry]);
        }
        // At worst 1 partial copy (extending the scale)
        // or whatever -= does
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
//            // possibly partial copy
//            des_cast.append(other.des_cast,
//                            boundary,
//                            other.des_cast.size() - boundary);
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
        if ( carry < 0 ){//k tomuhle by myslim dojít nemělo
            cela_cast.push_back(digits[radix + carry]);
        }
        cela_cast.resize(first_digit + 1, digits[0] );
        // At worst 1 copy
        // or 1 copy and whatever -= does
        return *this;
    }

    number & operator *=(const number &);
    number & operator /=(const number &);
    number & operator %=(const number &);
    number & operator ^=(const number &);

    number& operator ++();
    number& operator --();
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

    bool operator <(const number &) const;

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
        acc << radix << "::";
        if( ! isPositive ) acc << "-";
        acc << cela_cast;
        if( ! des_cast.empty() ){
            acc << "." << des_cast;
        }
        std::string result(acc.str());
        return result;
    }

    static number eval_postfix(const std::string &);
    static number eval_infix(const std::string &); // number<16>::eval_infix("10::23 + 2::100010")

    template<unsigned char oradix>
    /**
     * @brief convert
     * @return
     */
    static number convert(const number<oradix> &);

    void swap( number& other ){
    	cela_cast.swap(other.cela_cast);
    	des_cast.swap(other.des_cast);
    	using std::swap;
    	swap(isPositive, other.isPositive);
    }

private:
    std::string cela_cast; // najnizsi rad na indexe [0]
    std::string des_cast;  // najvyssi rad na indexe [0]
    bool isPositive;

    int cmp_ignore_sig(const number &other){
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

} // namespace fixedpoint

#endif // FIXEDPOINT_H

