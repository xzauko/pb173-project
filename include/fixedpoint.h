#ifndef FIXEDPOINT_H
#define FIXEDPOINT_H
#include <string>
#include <ostream>
#include <istream>
#include <sstream>

namespace fixedpoint{

inline const signed char values[128] = {
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

inline const char digits[36] = {
//   0    1    2    3    4    5    6    7    8    9
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', // 0
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', // 1
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', // 2
    'u', 'v', 'w', 'x', 'y', 'z',                     // 3
};
// mapovanie hodnota -> cislica

template<unsigned char radix = 10, unsigned char scale = 0>
struct number{ // forma reprezentacie cisla

    number(){}
    number(const std::string &); // 16::
    number& operator=(const std::string &);
    /*
     * ostatne konstruktory a operatory priradenia (copy, move),
     * mozno konstruktory a operatory aj pre ciselne typy
     * POZN.: pre konstruktor double asi vyhodne pouzit stringstream
     */

    // O(n) where n is number of digits in number
    number& operator+=(const number & other){
        size_t min_scale = min(des_cast.size(), other.des_cast.size());

        // DESATINNA CAST
        if (des_cast.size() > other.des_cast.size()){
            // possibly partial copy
            des_cast.append(other.des_cast,
                            min_scale,
                            other.des_cast.size() - min_scale);
        }
        int tmp, carry = 0;
        for (size_t i = min_scale; i>0; --i){ // easier than with iterators
            tmp = values[des_cast[i-1]] + values[other.des_cast[i-1]] + carry;
            if (tmp > radix){ carry = tmp / radix; }
            des_cast[i-1] = digits[tmp % radix];
        }

        // CELA CAST
        std::string ncela, nother;
        min_scale = min(cela_cast.size(), other.cela_cast.size());
        if (cela_cast.size() < other.cela_cast.size()){
            ncela = std::move(cela_cast);
            nother = other.cela_cast; // 1st copy
        }
        else{
            nother = std::move(cela_cast);
            ncela = other.cela_cast; // alternative 1st copy
        }
        auto our = ncela.rbegin(), their = nother.rbegin();
        for (; their != nother.rend(); ++our, ++their){
            // easier than with size_t i
            tmp = values[*our] + values[*their] + carry;
            if (tmp > radix){ carry = tmp / radix; }
            *our = digits[tmp % radix];
        }
        for(; our != ncela.rend(); ++our){
            tmp = values[*our] + carry;
            if (tmp > radix){ carry = tmp / radix; }
            *our = digits[tmp % radix];
            if (carry == 0) break;
        }
        if ( carry > 0 ){
            std::string nova;
            nova.push_back(digits[carry]);
            nova.append( ncela ); // possible 2nd copy
            cela_cast = std::move(nova);
        }
        else{
            cela_cast = std::move(ncela);
        }
        // at worst 2 full and 1 partial copy
        return *this;
    }
    /*number& operator+=(const std::string & other){
        number onum(other);
        *this += onum;
        return *this;
    }*/

    /*
     * obdobne -=, *=, /=, %=, ^= - mocnina, nie XOR
     * varianty pre string aj pre ine cislo
     */

    number& operator++();
    number& operator--();
    number operator++(int);
    number operator--(int);

    bool operator<(const number &) const;
    /* ostatne operatory porovnania */

    static number eval_postfix(const std::string &);
    static number eval_infix(const std::string &); // number<16>::eval_infix("10::23 + 2::100010")
private:
    std::string cela_cast, des_cast;

};


template<unsigned char radix>
number<radix> operator+(const number<radix> &, const number<radix> &);
number operator+(const number &, const std::string &);
number operator+(const std::string &, const number &);

template<unsigned char r1, unsigned char r2=10>
number<r2> convert(const number<r1> &);
/*
 * Obdobne varianty pre -, *, /, %, ^
 */

/*
 * Mozno aj operatory pre aritmeticky posun - bc ich nema:
 * number operator<<(const number &, int);
 * number operator>>(const number &, int);
 */

std::ostream& operator<<(std::ostream&, const number &);
std::istream& operator>>(std::istream&, number &);

}

#endif // FIXEDPOINT_H

