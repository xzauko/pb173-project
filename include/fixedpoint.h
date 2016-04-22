#ifndef FIXEDPOINT_H
#define FIXEDPOINT_H
#include <string>
#include <ostream>
#include <istream>
#include <sstream>

using namespace std;


namespace fixedpoint{

 const signed char values[128] = {
//   0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x1
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -1, -2, -1, // 0x2
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1, // 0x3
    -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, // 0x4
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1, // 0x5
    -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, // 0x6
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1, // 0x7
};
// -2: desatinne oddelovace (. a ,), -1: neplatna cifra,
// ostatne: hodnota cislice, ak >= sustave - neplatna

 const char digits[36] = {
//   0    1    2    3    4    5    6    7    8    9
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', // 0
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', // 1
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', // 2
    'u', 'v', 'w', 'x', 'y', 'z',                     // 3
};
// mapovanie hodnota -> cislica

template<unsigned char radix = 10, unsigned char scale = 0>
struct number{ // forma reprezentacie cisla

    number():isPositive(true){}
    number(const std::string &); // 16::
    number& operator =(const std::string &);

    /*
     * ostatne konstruktory a operatory priradenia (copy, move),
     * mozno konstruktory a operatory aj pre ciselne typy
     * POZN.: pre konstruktor double asi vyhodne pouzit stringstream
     */

    // O(n) where n is number of digits in number
    number& operator +=(const number & other){
        if ( isPositive != other.isPositive ){
            number o1(other);
            o1.isPositive = isPositive;
            return this->operator -=( o1 );
        }
        size_t boundary = min(des_cast.size(), other.des_cast.size());

        // DESATINNA CAST
        if (des_cast.size() > other.des_cast.size()){
            // possibly partial copy
            des_cast.append(other.des_cast,
                            boundary,
                            other.des_cast.size() - boundary);
        }
        int tmp, carry = 0;
        for ( size_t i = boundary; i>0; --i ){ // easier than with iterators
            tmp = values[des_cast[i-1]] + values[other.des_cast[i-1]] + carry;
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
            tmp = values[ *our ] + carry;
            if ( their != other.cela_cast.end() ){
                tmp += values[ *( their++ ) ];
            }
            carry = tmp / radix;
            *our = digits[ tmp % radix ];

        }
        if ( carry > 0 ){
            cela_cast.push_back(digits[carry]);
        }
        // At worst 1 partial copy (extending the scale)
        // or 1 copy and whatever -= does
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
            tmp = values[( unsigned)des_cast[i-1]] - values[( unsigned)o1.des_cast[i-1]] + carry;
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
            tmp = values[( unsigned) cela_cast.at(index) ] + carry;
            if ( index < o1.cela_cast.size() ){
                tmp -= values[( unsigned) o1.cela_cast.at(index) ];
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
        // At worst 1 partial copy (extending the scale)
        // or 1 copy and whatever -= does
        return *this;
    }
    number & operator *=(const number &);
    number & operator /=(const number &);
    number & operator %=(const number &);
    number & operator ^=(const number &);

    number& operator ++();
    number& operator --();
    number operator ++(int);
    number operator --(int);

    bool operator <(const number &) const;

    bool operator <=(const number & other) const{
        return ( ! ( other < ( *this ) ) );
    }

    bool operator ==(const number &) const;

    bool operator !=(const number & other) const{
        return ( ! ( ( *this ) == other ) );
    }

    bool operator >(const number & other) const{
        return ( other <= ( *this ) );
    }

    bool operator >=(const number & other) const{
        return ( ! ( ( *this ) < other ) );
    }

    std::string str() const; // string representation

    static number eval_postfix(const std::string &);
    static number eval_infix(const std::string &); // number<16>::eval_infix("10::23 + 2::100010")

    template<unsigned char oradix, unsigned char oscale>
    static number convert(const number<oradix, oscale> &);

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
    			if(values[cela_cast[pos]] != values[other.cela_cast[pos]]){
    	    		return (values[cela_cast[pos]] > values[other.cela_cast[pos]]) ? 1 : -1;
    			}
    			pos--;
    		}
    		//cela cast je stejna, rozhodne desetina cast
    		size_t limit = (des_cast.size() > other.des_cast.size()) ? des_cast.size() : other.des_cast.size();
    		while(pos < limit){
    			if(values[des_cast[pos]] != values[other.des_cast[pos]]){
    	    		return (values[des_cast[pos]] > values[other.des_cast[pos]]) ? 1 : -1;
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

template<unsigned char radix, unsigned char scale>
number<radix, scale> operator +(const number<radix, scale> &, const number<radix, scale> &);

template<unsigned char radix, unsigned char scale>
number<radix, scale> operator -(const number<radix, scale> &, const number<radix, scale> &);

template<unsigned char radix, unsigned char scale>
number<radix, scale> operator *(const number<radix, scale> &, const number<radix, scale> &);

template<unsigned char radix, unsigned char scale>
number<radix, scale> operator /(const number<radix, scale> &, const number<radix, scale> &);

template<unsigned char radix, unsigned char scale>
number<radix, scale> operator %(const number<radix, scale> &, const number<radix, scale> &);

template<unsigned char radix, unsigned char scale>
number<radix, scale> operator ^(const number<radix, scale> &, const number<radix, scale> &);


//number operator+(const number &, const std::string &); ??
//number operator+(const std::string &, const number &); ??

//template<unsigned char r1, unsigned char r2=10>
//number<r2> convert(const number<r1> &); MADE A STATIC MEMBER

/*
 * Mozno aj operatory pre aritmeticky posun - bc ich nema:
 * number operator<<(const number &, int);
 * number operator>>(const number &, int);
 */

template<unsigned char radix, unsigned char scale>
std::ostream& operator<<(std::ostream&, const number<radix, scale> &);

template<unsigned char radix, unsigned char scale>
std::istream& operator>>(std::istream&, number<radix, scale> &);

}

#endif // FIXEDPOINT_H

