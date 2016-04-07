#ifndef FIXEDPOINT_H
#define FIXEDPOINT_H
#include <string>
#include <ostream>
#include <istream>

namespace fixedpoint{

const signed char[128] values = {
//0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 // 0x0
 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 // 0x1
 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -1, -2, -1 // 0x2
  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1 // 0x3
 -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 // 0x4
 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1 // 0x5
 -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 // 0x6
 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1 // 0x7
};
// -2: desatinne oddelovace (. a ,), -1: neplatna cifra, 
// ostatne: hodnota cislice, ak >= sustave - neplatna

template<unsigned char radix, unsigned char scale = 0>
struct number{ // forma reprezentacie cisla

    number();
    number(const std::string &); // 16::
    number& operator=(const std::string &);
    /*
     * ostatne konstruktory a operatory priradenia (copy, move),
     * mozno konstruktory a operatory aj pre ciselne typy
     */

    number& operator+=(const number &);
    number& operator+=(const std::string &);
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

