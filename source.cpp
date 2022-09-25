#include<cstdio>
#include<iostream>
#include<string>
#include<algorithm>
#include<cstring>
const int maxLen = 10000;
using namespace std;
struct Number{
    int errorCode;//0: no error 1: length error 2: type error
    // intPart: higher digit has higher index
    // fracPart: higher digit has lower index
    // this is in order to process digital carrying easier
    // 12.45: intPart:{2,1} fracPart:{4,5} 
    int intPart[maxLen + 100], fracPart[maxLen + 100], numSign;
    int intLen, fracLen;
    friend Number operator *(Number num1, Number num2) {
        Number number;
        if (num1.errorCode >= 1 || num2.errorCode >= 1 ) {
            number.errorCode = max(num1.errorCode, num2.errorCode);
            return number;
        }
        if (num1.fracLen + num2.fracLen > maxLen || num1.intLen + num2.intLen > maxLen) {
            number.errorCode = 1;
            return number;
        }
        for (int i = 0;i < maxLen; i++)
        number.intPart[i] = number.fracPart[i] = 0;
        for (int i = 0;i < num1.intLen; i++)
        for (int j = 0;j < num2.intLen; j++) {
            number.intPart[i + j] += num1.intPart[i] * num2.intPart[j];
        }
        // i fracdigit * j fracdigit: i+j fracdigit
        for (int i = 0;i < num1.fracLen; i++)
        for (int j = 0;j < num2.fracLen; j++) {
            number.fracPart[i + j + 1] += num1.fracPart[i] * num2.fracPart[j];
        }
        for (int i = 0;i < num1.intLen; i++)
        for (int j = 0;j < num2.fracLen; j++) {
            if (i > j) number.intPart[i - j - 1] += num1.intPart[i] * num2.fracPart[j];
            else number.fracPart[j - i] += num1.intPart[i] * num2.fracPart[j];
        }
        for (int i = 0;i < num1.fracLen; i++)
        for (int j = 0;j < num2.intLen; j++) {
            if (j > i) number.intPart[j - i - 1] += num1.fracPart[i] * num2.intPart[j];
            else number.fracPart[i - j] += num1.fracPart[i] * num2.intPart[j];
        }
        for (int i = num1.fracLen + num2.fracLen + 1;i >= 0;i--) {
            if (i > 0) {
                number.fracPart[i - 1] += number.fracPart[i] / 10;
            }
            else {
                number.intPart[0] += number.fracPart[i] / 10;
            }
            number.fracPart[i] %= 10;
        }
        number.intLen = 0;
        for (int i = 0; i < num1.intLen + num2.intLen - 1 || number.intPart[i] > 0; i++) {
            number.intLen = i + 1;
            number.intPart[i + 1] += number.intPart[i] / 10;
            number.intPart[i] %= 10;
        }
        number.fracLen = num1.fracLen + num2.fracLen + 1;
        for (int i = num1.fracLen + num2.fracLen; i >= 0; i--){
            if (number.fracPart[i] > 0) break;
            number.fracLen = i;
        }
        for (int i = number.intLen - 1; i >= 0; i--) {
        if (number.intPart[i] == 0) number.intLen --;
        else break;
    }
        for (int i = number.fracLen - 1; i >= 0; i--) {
            if (number.fracPart[i] == 0) number.fracLen --;
            else break;
    }
        if (number.intLen > maxLen || number.fracLen > maxLen) number.errorCode = 1;
        number.numSign = num1.numSign * num2.numSign;
        return number;
    }
};
bool isDigit(char c)
{
    if (c > '9' || c < '0') return 0;
    return 1;
}
Number toNumber(string s)
{
    Number num;
    memset(num.fracPart, 0, sizeof(num.fracPart));
    memset(num.intPart, 0, sizeof(num.intPart));
    num.fracLen = 0;
    int type = 0, ePos = 0, numSign = 1, expSign = 1;
    int haveDot = 0, dotPoint = 0, havNumber = 0;
    for (int i = 0;i < s.length();i++)
    if (isDigit(s[i])) havNumber = 1;
    if (!havNumber) {
        num.errorCode = 2;
        return num;
    }
    while (s[0] == '-') {
        numSign *= -1;
        s = s.substr(1,s.length() - 1);
    }
    if (s[0] =='.') {
        s = '0' + s;
    }
    for (int i = 0;i < s.length(); i++){
        if (s[i] == '.' && type == 0) {
            dotPoint = i;
            type = 1;
        }
        else if (s[i] == 'e' && ePos == 0) {
            ePos = i;
            if (type == 1) haveDot = 1; 
            type = 2;
            if (i == 0) type = 3;
            else if (s[i - 1] == '.') type = 3;
            if (i == s.length() - 1) type = 3;
            else if (s[i+1] != '-' && !isDigit(s[i+1])) type = 3;
            else if (s[i+1] == '-') expSign = -1;
            for (int j = i + 2;j < s.length(); j++)
                if (!isDigit(s[j])) type = 3;
            break;
        }
        else if (!isDigit(s[i])) {
            type = 3;
            break;
        }
    }
    if (type == 3) {
        num.errorCode = 2;
        return num;
    }
    if (dotPoint > maxLen) {
        num.errorCode = 1;
        return num;
    }
    for (int i = 0;i < s.length() && s[i] != '.' && s[i] != 'e'; i++) {
        num.intPart[i] = s[i] - '0';
        num.intLen = i + 1;
    }
    reverse(num.intPart, num.intPart + num.intLen);
    if (dotPoint != 0) {
        int beginPos = min(int(s.length() - 1), ePos - 1);
        if (ePos == 0) beginPos = s.length() - 1;
        for (int i = beginPos;s[i] != '.' && i >= 0; i--) {
            num.fracPart[beginPos - i] = s[i] - '0';
            num.fracLen ++;
        }
        reverse(num.fracPart, num.fracPart + num.fracLen);
    }
    if (type == 2) {
        int mul = 0;// the number after "e"
        int pos1 = ePos + 1;
        if (expSign == -1) pos1 ++;
        for (int i = pos1;i < s.length();i++){
            mul = mul * 10 + s[i] - '0';
        } 
        if (expSign == 1 && mul + num.intLen > maxLen) num.errorCode = 1;
        if (expSign == -1 && mul + ePos - dotPoint - 1 > maxLen) num.errorCode = 1;
        if (num.errorCode == 1) {
            return num;
        }
        if (expSign == 1 && mul > 0) {
            for (int i = num.intLen - 1;i >= 0;i--) {
                num.intPart[i + mul] = num.intPart[i];
                num.intPart[i] = 0;
            }
            num.intLen += mul;
            for (int i = 0;i < num.fracLen;i++) {
                if (i < mul) num.intPart[mul - i - 1] = num.fracPart[i];
                else num.fracPart[i - mul] = num.fracPart[i];
                num.fracPart[i] = 0; 
            }
            num.fracLen = max(num.fracLen - mul, 0);
        }
        if (expSign == -1 && mul > 0) {
            for (int i = num.fracLen - 1;i >= 0;i--) {
                num.fracPart[i + mul] = num.fracPart[i];
                num.fracPart[i] = 0;
            }
            num.fracLen += mul;
            for (int i = 0;i < num.intLen;i++) {
                if (i < mul) num.fracPart[mul - i - 1] = num.intPart[i];
                else num.intPart[i - mul] = num.intPart[i];
                num.intPart[i] = 0;
            }
            num.intLen = max(num.intLen - mul, 0);
        }
    }
    for (int i = num.intLen - 1; i >= 0; i--) {
        if (num.intPart[i] == 0) num.intLen --;
        else break;
    }
    for (int i = num.fracLen - 1; i >= 0; i--) {
        if (num.fracPart[i] == 0) num.fracLen --;
        else break;
    }
    if (num.intLen == 0 && num.fracLen == 0)
    numSign = 1;
    num.numSign = numSign;
    return num;
}
void PrintNumber(Number number)
{
    if (number.numSign == -1) cout<< '-';
    if (number.intLen == 0) cout << 0;
    for (int i = number.intLen - 1; i >= 0; i--) {
        cout << number.intPart[i];
    }
    if (number.fracLen > 0) cout << '.';
    for (int i = 0;i < number.fracLen; i++) 
        cout << number.fracPart[i];
}
int main(int argc, char *argv[])
{
    string s1, s2;
    if (argc < 3) {
        printf("Too few input!");
        return 0;
    }
    if (argc > 3) {
        printf("Too many inputs!");
        return 0;
    }
    s1 = argv[1];
    s2 = argv[2];
    Number num1, num2;
    num1 = toNumber(s1);
    num2 = toNumber(s2);
    Number num = num1 * num2;
    if (num.errorCode == 2) cout<<"The input cannot be interpret as numbers!"<<'\n';
    else if (num.errorCode == 1) cout<<"The number is too large or too precise!\n";
    else {
        PrintNumber(num1);
        cout << " * ";
        PrintNumber(num2);
        cout << " = ";
        PrintNumber(num);
    }
}