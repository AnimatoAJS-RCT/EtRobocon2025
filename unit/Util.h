#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <string>
#include <sstream>


/*
    文字列を分割する

    注意
        s の文字列内に、区切り文字が連続する箇所があるとき、その隙間は無視される。
        たとえば、s が "a//b"、separator が "/" のとき、分割結果は、
        "a" と "b" であり、戻り値は 2 を返す。

    引数
        s: 対象の文字列。書き換えが起こる。NULL不可
        separator: 区切り文字の並び。NULL不可
        result: 結果を格納する文字列配列。十分な要素数を確保すること
        result_size: 引数result の要素数

    戻り値
        分割後の文字列の個数
*/
//size_t split(char* s, const char* separator, char** result, size_t result_size);
std::vector<std::string> split(const std::string& s, const std::string& separator);

enum eColor {
    RED,
    BLUE,
    YELLOW,
    GREEN,
    WHITE,
    BLACK,
    OTHER
};

eColor getColor(int hue, int saturation, int value);

const char* colorToString(eColor color);

#endif // UTILITY_H
