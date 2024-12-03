#pragma once
#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

// Integer 1:
// 实现一个有符号的大整数类，只需支持简单的加减

// Integer 2:
// 实现一个有符号的大整数类，支持加减乘除，并重载相关运算符

// 请不要使用除了以下头文件之外的其它头文件
#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

// 请不要使用 using namespace std;

namespace sjtu {
  // 采用 BASE = 10000 进制
class int2048 {

  static constexpr int INT2048_BASE = 1000;
  // static constexpr int INT2048_MAX_DIGIT = 1000000;
  static constexpr double PI = 3.14159265358979324;
  static constexpr int INT2048_MAX_DIGIT_POWER = 20;
  static constexpr int INT2048_MAX_DIGIT = 1 << (INT2048_MAX_DIGIT_POWER + 1);
  static int FFT_NTT_rev[INT2048_MAX_DIGIT + 1];
  static constexpr long long mod1 = 469762049, mod2 = 167772161, g_mod = 3,
  inv1 = 130489458, inv2 = 104391568;

  struct Complex;

public:
  using storage_type = int;
  std::vector<storage_type> storage;  // 存储数字的绝对值
  int storage_sign;  // 正负指示， 1 为正， -1 为负，存储的数为0时指示符为1

  bool is_zero() const;
  static storage_type lower_half(storage_type);  // 进位辅助函数：无需进位部分。
  static storage_type upper_half(storage_type);  // 进位辅助函数：待进位部分。
  static storage_type lower_half(long long);  // 进位辅助函数：无需进位部分。
  static long long upper_half(long long);  // 进位辅助函数：待进位部分。
  void trim();
  // 返回abs(left) >= abs(right)的真假性
  friend bool no_lesser_abs(const int2048 &, const int2048 &);
  // 要求 abs(left) >= abs(right). 返回 abs(left) - abs(right)，符号继承left
  void minus_abs(const int2048 &);
  // FFT
  void FFT(std::vector<Complex> &, int, int);
  long long pow_mod(int, int, long long);
  long long mult_mod(long long, long long, long long);
  void NTT(std::vector<long long> &, int, int, long long);

  int2048 div_abs_brute(const int2048 &, const int2048 &) const;
  // shift left/times BASE^shift. use negative param to shift right/divide by BASE^shift.
  void shift(int shift);
  // return BASE^size / this
  int2048 inv();
  // {quo, res}
  std::pair<int2048, int2048> div_abs(const int2048 &) const;

  // 构造函数
  // 无参数初始化为 0
  int2048();
  int2048(long long);
  // 允许前导零
  int2048(const std::string &);
  int2048(const int2048 &);
  ~int2048();

  // 以下给定函数的形式参数类型仅供参考，可自行选择使用常量引用或者不使用引用
  // 如果需要，可以自行增加其他所需的函数
  // ===================================
  // Integer1
  // ===================================

  // 读入一个大整数
  void read(const std::string &);
  // 输出储存的大整数，无需换行
  void print();

  // 加上一个大整数
  int2048 &add(const int2048 &);
  // 返回两个大整数之和
  friend int2048 add(int2048, const int2048 &);

  // 减去一个大整数
  int2048 &minus(const int2048 &);
  // 返回两个大整数之差
  friend int2048 minus(int2048, const int2048 &);

  // ===================================
  // Integer2
  // ===================================

  bool operator==(const int2048 &);
  // bool operator<(const int2048 &);
  // bool operator>(const int2048 &);

  int2048 operator+() const;
  int2048 operator-() const;

  int2048 &operator=(const int2048 &);

  int2048 &operator+=(const int2048 &);
  friend int2048 operator+(int2048, const int2048 &);

  int2048 &operator-=(const int2048 &);
  friend int2048 operator-(int2048, const int2048 &);

  int2048 &operator*=(const int2048 &);
  friend int2048 operator*(int2048, const int2048 &);

  int2048 &operator/=(const int2048 &);
  friend int2048 operator/(int2048, const int2048 &);

  int2048 &operator%=(const int2048 &);
  friend int2048 operator%(int2048, const int2048 &);

  friend std::istream &operator>>(std::istream &, int2048 &);
  friend std::ostream &operator<<(std::ostream &, const int2048 &);

  friend bool operator==(const int2048 &, const int2048 &);
  friend bool operator!=(const int2048 &, const int2048 &);
  friend bool operator<(const int2048 &, const int2048 &);
  friend bool operator>(const int2048 &, const int2048 &);
  friend bool operator<=(const int2048 &, const int2048 &);
  friend bool operator>=(const int2048 &, const int2048 &);


  operator double();
  operator std::string();

};
} // namespace sjtu

#endif
