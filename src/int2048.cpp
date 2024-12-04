#include "int2048.h"

namespace sjtu {

int int2048::FFT_NTT_rev[INT2048_MAX_DIGIT + 1];

bool int2048::is_zero() const {
  return storage.size() == 1 && storage.back() == 0;
}

int2048::storage_type int2048::upper_half(storage_type integer) {
  return integer / INT2048_BASE;
}

int2048::storage_type int2048::lower_half(storage_type integer) {
  return integer % INT2048_BASE;
}

long long int2048::upper_half(long long integer) {
  return integer / static_cast<long long>(INT2048_BASE);
}

int2048::storage_type int2048::lower_half(long long integer) {
  return static_cast<storage_type>(integer % static_cast<long long>(INT2048_BASE));
}

void int2048::trim() {
  while(!storage.empty() && storage.back() == 0) storage.pop_back();
  if(storage.empty()) {
    storage_sign = 1;
    storage.push_back(0);
  }
}




int2048::int2048() {
  storage_sign = 1;
  storage.push_back(0);
}

int2048::int2048(long long integer) {
  if(integer == 0) {
    storage_sign = 1;
    storage.push_back(0);
    return;
  }
  storage_sign = (integer > 0) ? 1 : -1;
  integer *= storage_sign;
  while(integer != 0) {
    storage.push_back(integer % INT2048_BASE);
    integer /= INT2048_BASE;
  }
}

int2048::int2048(const std::string &integer_str) {
  read(integer_str);
}

int2048::int2048(const int2048 &another) {
  storage = another.storage;
  storage_sign = another.storage_sign;
}


int2048::~int2048() {
  // storage.clear();
}

void int2048::read(const std::string &integer_str) {
  storage.clear();
  storage_sign = (integer_str[0] == '-') ? -1 : 1;
  int start_mark = (storage_sign == 1) ? 0 : 1;
  if(INT2048_BASE == 10000){
    int weight[4] = {1, 10, 100, 1000}, weight_cnt = -1;
    storage_type temp_storage = 0;
    for(int pos = integer_str.length() - 1; pos >= start_mark; pos--) {
      weight_cnt++;
      temp_storage += (integer_str[pos] - '0') * weight[weight_cnt];
      if(weight_cnt == 3) {
        storage.push_back(temp_storage);
        weight_cnt = -1;
        temp_storage = 0;
      }
    }
  storage.push_back(temp_storage);
  }
  if(INT2048_BASE == 1000) {
    int weight[3] = {1, 10, 100}, weight_cnt = -1;
    storage_type temp_storage = 0;
    for(int pos = integer_str.length() - 1; pos >= start_mark; pos--) {
      weight_cnt++;
      temp_storage += (integer_str[pos] - '0') * weight[weight_cnt];
      if(weight_cnt == 2) {
        storage.push_back(temp_storage);
        weight_cnt = -1;
        temp_storage = 0;
      }
    }
    storage.push_back(temp_storage);
  }
  if(INT2048_BASE == 100) {
    int weight[2] = {1, 10}, weight_cnt = -1;
    storage_type temp_storage = 0;
    for(int pos = integer_str.length() - 1; pos >= start_mark; pos--) {
      weight_cnt++;
      temp_storage += (integer_str[pos] - '0') * weight[weight_cnt];
      if(weight_cnt == 1) {
        storage.push_back(temp_storage);
        weight_cnt = -1;
        temp_storage = 0;
      }
    }
    storage.push_back(temp_storage);
  }
  if(INT2048_BASE == 10) {
    for(int pos = integer_str.length() - 1; pos >= start_mark; pos--) {
      storage.push_back(integer_str[pos] - '0');
    }
  }
  trim();
}

void int2048::print() {
  if(is_zero()) {
    putchar('0');
    return;
  }
  if(storage_sign == -1) putchar('-');
  std::cout << storage.back();
  for(auto pos_it = ++storage.rbegin(); pos_it != storage.rend(); ++pos_it) {
    storage_type integer = *pos_it;
    if(INT2048_BASE == 10000) {
      if(integer == 0) std::cout << "0000";
      else if(integer < 10) std::cout << "000" << integer;
      else if(integer < 100) std::cout << "00" << integer;
      else if(integer < 1000) std::cout << "0" << integer;
      else std::cout << integer;
    }
    if(INT2048_BASE == 1000) {
      if(integer == 0) std::cout << "000";
      else if(integer < 10) std::cout << "00" << integer;
      else if(integer < 100) std::cout << "0" << integer;
      else std::cout << integer;
    }
    if(INT2048_BASE == 100) {
      if(integer == 0) std::cout << "00";
      else if(integer < 10) std::cout << "0" << integer;
      else std::cout << integer;
    }
    if(INT2048_BASE == 10)
      std::cout << integer;
  }
}

int2048 int2048::operator+() const {
  int2048 temp = *this;
  return *this;
}


int2048 int2048::operator-() const {
  int2048 temp = *this;
  if(!temp.is_zero()) temp.storage_sign = -temp.storage_sign;
  return temp;
}

int2048 &int2048::operator=(const int2048 &another) {
  storage = another.storage;
  storage_sign = another.storage_sign;
  return *this;
}


std::istream &operator>>(std::istream &is_object, int2048 &integer) {
  std::string integer_str;
  is_object >> integer_str;
  integer.read(integer_str);
  return is_object;
}

std::ostream &operator<<(std::ostream &os_object, const int2048 &integer) {
  if(integer.is_zero()) {
    os_object << '0';
    return os_object;
  }
  if(integer.storage_sign == -1) os_object << '-';
  os_object << integer.storage.back();
  for(auto pos_it = ++integer.storage.rbegin(); pos_it != integer.storage.rend(); ++pos_it) {
    int2048::storage_type value = *pos_it;
    if(int2048::INT2048_BASE == 10000) {
      if(value == 0) os_object << "0000";
      else if(value < 10) os_object << "000" << value;
      else if(value < 100) os_object << "00" << value;
      else if(value < 1000) os_object << "0" << value;
      else os_object << value;
    }
    if(int2048::INT2048_BASE == 1000) {
      if(value == 0) os_object << "000";
      else if(value < 10) os_object << "00" << value;
      else if(value < 100) os_object << "0" << value;
      else os_object << value;
    }
    if(int2048::INT2048_BASE == 100) {
      if(value == 0) os_object << "00";
      else if(value < 10) os_object << "0" << value;
      else os_object << value;
    }
    if(int2048::INT2048_BASE == 10)
      os_object << value;
  }
  return os_object;
}


bool operator==(const int2048 &left, const int2048 &right) {
  bool left_zero = left.is_zero(), right_zero = right.is_zero();
  if(left_zero ^ right_zero) return false;
  if(left_zero && right_zero) return true;
  return left.storage_sign == right.storage_sign && left.storage == right.storage;
}

bool operator!=(const int2048 &left, const int2048 &right) {
  return !(left == right);
}

bool operator<(const int2048 &left, const int2048 &right) {
  bool left_zero = left.is_zero(), right_zero = right.is_zero();
  if(left_zero && right_zero) return false;
  if(left_zero) return 0 < right.storage_sign;
  if(right_zero) return left.storage_sign < 0;
  if(left.storage_sign != right.storage_sign) return left.storage_sign < right.storage_sign;
  bool is_negative = (left.storage_sign == -1);
  if(left.storage.size() != right.storage.size())
    return (left.storage.size() < right.storage.size()) ^ is_negative;
  for(int pos = left.storage.size() - 1; pos >= 0; pos--) {
    if(left.storage[pos] != right.storage[pos]) return (left.storage[pos] < right.storage[pos]) ^ is_negative;
  }
  return false;
}

bool operator>(const int2048 &left, const int2048 &right) {
  bool left_zero = left.is_zero(), right_zero = right.is_zero();
  if(left_zero && right_zero) return false;
  if(left_zero) return 0 > right.storage_sign;
  if(right_zero) return left.storage_sign > 0;
  if(left.storage_sign != right.storage_sign) return left.storage_sign > right.storage_sign;
  bool is_negative = (left.storage_sign == -1);
  if(left.storage.size() != right.storage.size())
    return (left.storage.size() > right.storage.size()) ^ is_negative;
  for(int pos = left.storage.size() - 1; pos >= 0; pos--) {
    if(left.storage[pos] != right.storage[pos]) return (left.storage[pos] > right.storage[pos]) ^ is_negative;
  }
  return false;
}

bool operator<=(const int2048 &left, const int2048 &right) {
  return !(left > right);
}

bool operator>=(const int2048 &left, const int2048 &right) {
  return !(left < right);
}

bool int2048::operator==(const int2048 &another) {
  bool left_zero = is_zero(), right_zero = another.is_zero();
  if(left_zero ^ right_zero) return false;
  if(left_zero && right_zero) return true;
  return storage_sign == another.storage_sign && storage == another.storage;
}
/*
bool int2048::operator<(const int2048 &another) {
  bool left_zero = is_zero(), right_zero = another.is_zero();
  if(left_zero && right_zero) return false;
  if(left_zero) return 0 < another.storage_sign;
  if(right_zero) return storage_sign < 0;
  if(storage_sign != another.storage_sign) return storage_sign < another.storage_sign;
  bool is_negative = (storage_sign == -1);
  if(storage.size() != another.storage.size())
    return (storage.size() < another.storage.size()) ^ is_negative;
  for(int pos = storage.size() - 1; pos >= 0; pos--) {
    if(storage[pos] != another.storage[pos]) return (storage[pos] < another.storage[pos]) ^ is_negative;
  }
  return false;
}

bool int2048::operator>(const int2048 &another) {
  bool left_zero = is_zero(), right_zero = another.is_zero();
  if(left_zero && right_zero) return false;
  if(left_zero) return 0 > another.storage_sign;
  if(right_zero) return storage_sign > 0;
  if(storage_sign != another.storage_sign) return storage_sign > another.storage_sign;
  bool is_negative = (storage_sign == -1);
  if(storage.size() != another.storage.size())
    return (storage.size() > another.storage.size()) ^ is_negative;
  for(int pos = storage.size() - 1; pos >= 0; pos--) {
    if(storage[pos] != another.storage[pos]) return (storage[pos] > another.storage[pos]) ^ is_negative;
  }
  return false;
}
*/
int2048 &int2048::operator+=(const int2048 &another) {
  if(is_zero()) {
    *this = another;
    return *this;
  }
  if(another.is_zero()) return *this;
  if(storage_sign != another.storage_sign) {
    *this -= -another;
    return *this;
  }
  if(storage.size() < another.storage.size()) storage.resize(another.storage.size());
  storage_type carry = 0;
  for(int pos = 0; pos < another.storage.size(); pos++) {
    carry += storage[pos] + another.storage[pos];
    storage[pos] = lower_half(carry);
    carry = upper_half(carry);
  }
  for(int pos = another.storage.size(); pos < storage.size() && carry != 0; pos++) {
    carry += storage[pos];
    storage[pos] = lower_half(carry);
    carry = upper_half(carry);
  }
  while(carry != 0) {
    storage.push_back(lower_half(carry));
    carry = upper_half(carry);
  }
  return *this;
}

int2048 operator+(int2048 left, const int2048 &right) {
  left += right;
  return left;
}

bool no_lesser_abs(const int2048 &left, const int2048 &right) {
  if(left.storage.size() != right.storage.size()) return left.storage.size() > right.storage.size();
  for(int pos = left.storage.size() - 1; pos >= 0; pos--) {
    if(left.storage[pos] != right.storage[pos]) return left.storage[pos] > right.storage[pos];
  }
  return true;
}

void int2048::minus_abs(const int2048 &another) {
  storage_type borrow = 0;
  for(int pos = 0; pos < another.storage.size(); pos++) {
    borrow += storage[pos] - another.storage[pos];
    storage[pos] = borrow;
    storage[pos] -= INT2048_BASE * (borrow >>= 31);
  }
  for(int pos = another.storage.size(); borrow != 0; pos++) {
    borrow += storage[pos];
    storage[pos] = borrow;
    storage[pos] -= INT2048_BASE * (borrow >>= 31);
  }
  trim();
}


int2048 &int2048::operator-=(const int2048 &another) {
  if(storage_sign != another.storage_sign) {
    *this += -another;
    return *this;
  }
  if(no_lesser_abs(*this, another)) {
    minus_abs(another);
    return *this;
  } else {
    int2048 temp = another;
    temp.minus_abs(*this);
    *this = -temp;
    return *this;
  }
}

int2048 operator-(int2048 left, const int2048 &right) {
  left -= right;
  return left;
}

int2048 &int2048::add(const int2048 &another) {
  return *this += another;
}

int2048 add(int2048 left, const int2048 &right) {
  int2048 sum = left + right;
  return sum;
}

int2048 &int2048::minus(const int2048 &another) {
  return *this -= another;
}

int2048 minus(int2048 left, const int2048 &right) {
  int2048 diff = left - right;
  return diff;
}

struct int2048::Complex {
  double real, imag;

  Complex(){real = imag = 0;}
  Complex(double x, double y): real(x), imag(y){}

  Complex operator+(const Complex &another) const {
    return {real + another.real, imag + another.imag};
  }
  Complex operator-(const Complex &another) const {
    return {real - another.real, imag - another.imag};
  }
  Complex operator*(const Complex &another) const {
    return {real * another.real - imag * another.imag, real * another.imag + imag * another.real};
  }
  Complex operator*(double scale) const {
    return {real * scale, imag * scale};
  }
};

void int2048::FFT(std::vector<Complex> &vec, int high_bit, int inv) {
  for(int i = 0; i < high_bit; i++)
    if(i < FFT_NTT_rev[i]) {
      Complex temp = vec[i];
      vec[i] = vec[FFT_NTT_rev[i]];
      vec[FFT_NTT_rev[i]] = temp;
  }
  for(int mid = 1; mid < high_bit; mid <<= 1) {
    Complex ident(cos(PI / mid), inv * sin(PI / mid));
    for(int j = 0; j < high_bit; j += (mid << 1)) {
      Complex w_j(1, 0);
      for(int k = 0; k < mid; k++, w_j = w_j * ident) {
        Complex l = vec[j + k], r = w_j * vec[j + k + mid];
        vec[j + k] = l + r;
        vec[j + k + mid] = l - r;
      }
    }
  }
  if(inv == -1) {
    for(int i = 0; i < high_bit; i++) {
      vec[i].real /= high_bit;
      vec[i].imag /= high_bit;
    }
  }
}

long long int2048::pow_mod(int base, int p, long long mod) {
  int ans = 1;
  for(; p > 0; p /= 2, base = 1ll * base * base % mod)
    if(p & 1) ans = 1ll * ans * base % mod;
  return ans;
}

long long int2048::mult_mod(long long a, long long b, long long mod) {
  a = (a + mod) % mod; b = (b + mod) % mod;
  return (a * b - (long long)((double)a * b / mod) * mod + mod) % mod;
}

void int2048::NTT(std::vector<long long> &vec, int high_bit, int inv, long long mod) {
  for(int i = 0; i < high_bit; i++)
    if(i < FFT_NTT_rev[i])
      std::swap(vec[i], vec[FFT_NTT_rev[i]]);
  for(int mid = 1; mid < high_bit; mid <<= 1) {
    long long ident = pow_mod(g_mod, (mod - 1) / (mid << 1), mod); // mod is int-acceptable
    for(int j = 0; j < high_bit; j += (mid << 1)) {
      long long w_k = 1;
      for(int k = 0; k < mid; k++, w_k = mult_mod(w_k, ident, mod)) {
        long long l = vec[j + k], r = mult_mod(w_k, vec[j + k + mid], mod);
        vec[j + k] = (l + r) % mod;
        vec[j + k + mid] = (l - r + mod) % mod;
      }
    }
  }
  if(inv == -1) {
    long long inversion = pow_mod(high_bit, mod - 2, mod);
    for(int i = 0; i < high_bit; i++)
      vec[i] = mult_mod(vec[i], inversion, mod);
    for(int i = 1; i < high_bit / 2; i++)
      std::swap(vec[i], vec[high_bit - i]);
  }
}

/*
// Plain version
int2048 &int2048::operator*=(const int2048 &another) {
  if(is_zero()) return *this;
  if(another.is_zero()) {
    *this = another;
    return *this;
  }
  storage_sign = (storage_sign == another.storage_sign) ? 1 : -1;
  std::vector<long long> temp(storage.size() + another.storage.size() + 1);
  for(int i = 0; i < storage.size(); i++)
    for(int j = 0; j < another.storage.size(); j++) {
      temp[i + j] += storage[i] * another.storage[j];
      temp[i + j + 1] += upper_half(temp[i + j]);
      temp[i + j] = lower_half(temp[i + j]);
    }
  storage.resize(temp.size());
  for(int pos = 0; pos < temp.size(); pos++) {
    temp[pos + 1] += upper_half(temp[pos]);
    temp[pos] = lower_half(temp[pos]);
    storage[pos] = temp[pos];
  }
  trim();
  return *this;
}
*/

// FFT version
// Not capable when log10(BASE) >= 4 and 200000 digits
int2048 &int2048::operator*=(const int2048 &another) {
  if(is_zero()) return *this;
  if(another.is_zero()) {
    *this = another;
    return *this;
  }
  storage_sign = (storage_sign == another.storage_sign) ? 1 : -1;
  int high_bit = 1, len = 0;
  int size_a = storage.size() - 1, size_b = another.storage.size() - 1;
  while(high_bit <= size_a + size_b) high_bit <<= 1, len++;
  FFT_NTT_rev[0] = 0;
  for(int i = 0; i < high_bit; i++)
    FFT_NTT_rev[i] = (FFT_NTT_rev[i >> 1] >> 1) | ((i & 1) << (len - 1));
  std::vector<Complex> fft_vec;
  fft_vec.resize(high_bit, Complex());
  for(int pos = 0; pos <= size_a; pos++) fft_vec[pos].real = storage[pos];
  for(int pos = 0; pos <= size_b; pos++) fft_vec[pos].imag = another.storage[pos];
  FFT(fft_vec, high_bit, 1);
  for(int pos = 0; pos < high_bit; pos++)
    fft_vec[pos] = fft_vec[pos] * fft_vec[pos];
  FFT(fft_vec, high_bit, -1);
  storage.resize(size_a + size_b + 5);
  long long carry = 0;
  for(int pos = 0; pos <= size_a + size_b; pos++) {
    carry += static_cast<long long>(fft_vec[pos].imag / 2 + 0.5);
    storage[pos] = lower_half(carry);
    carry = upper_half(carry);
  }
  for(int pos = size_a + size_b + 1; carry != 0; pos++) {
    storage[pos] = lower_half(carry);
    carry = upper_half(carry);
  }
  trim();
  return *this;
}

/*
// NTT version
int2048 &int2048::operator*=(const int2048 &another) {
  if(is_zero()) return *this;
  if(another.is_zero()) {
    *this = another;
    return *this;
  }
  storage_sign = (storage_sign == another.storage_sign) ? 1 : -1;
  int high_bit = 1, len = 0;
  int size_a = storage.size(), size_b = another.storage.size();
  int size_prediction = size_a + size_b - 1;
  while(high_bit < size_prediction) high_bit <<= 1, len++;
  if(FFT_NTT_rev[high_bit] != -1){
    FFT_NTT_rev[0] = 0;
    for(int i = 0; i < high_bit; i++)
      FFT_NTT_rev[i] = (FFT_NTT_rev[i >> 1] >> 1) | ((i & 1) << (len - 1));
    FFT_NTT_rev[high_bit] = -1;
  }

  std::vector<long long> ans_vec_1(high_bit), ans_vec_2(high_bit), empty_vec(high_bit);
  std::vector<long long> calc_vec_1(high_bit), calc_vec_2(high_bit);
  std::copy(storage.begin(), storage.end(), calc_vec_1.begin());
  std::copy(another.storage.begin(), another.storage.end(), calc_vec_2.begin());
  NTT(calc_vec_1, high_bit, 1, mod1); NTT(calc_vec_2, high_bit, 1, mod1);
  for(int i = 0; i < high_bit; i++) ans_vec_1[i] = mult_mod(calc_vec_1[i], calc_vec_2[i], mod1);
  NTT(ans_vec_1, high_bit, -1, mod1);
  calc_vec_1 = empty_vec; calc_vec_2 = empty_vec;
  std::copy(storage.begin(), storage.end(), calc_vec_1.begin());
  std::copy(another.storage.begin(), another.storage.end(), calc_vec_2.begin());
  NTT(calc_vec_1, high_bit, 1, mod2); NTT(calc_vec_2, high_bit, 1, mod2);
  for(int i = 0; i < high_bit; i++) ans_vec_2[i] = mult_mod(calc_vec_1[i], calc_vec_2[i], mod2);
  NTT(ans_vec_2, high_bit, -1, mod2);

  storage.resize(size_prediction + 2);
  long long carry = 0;
  for(int pos = 0; pos < size_prediction + 2; pos++) {
    long long temp = mult_mod(ans_vec_1[pos] - ans_vec_2[pos], inv2, mod1);
    temp = (temp * mod2 + ans_vec_2[pos]) % (mod1 * mod2);
    carry += temp;
    storage[pos] = lower_half(carry);
    carry = upper_half(carry);
  }
  trim();
  return *this;
}
*/

int2048 operator*(int2048 left, const int2048 &right) {
  left *= right;
  return left;
}

void int2048::shift(int shift) {
  if(shift == 0 || is_zero()) return;
  int old_size = storage.size();
  if(shift > 0) {
    storage.resize(old_size + shift);
    for(int pos = old_size - 1; pos >= 0; pos--) storage[pos + shift] = storage[pos];
    for(int pos = 0; pos < shift; pos++) storage[pos] = 0;
  } else if(shift < 0) {
    shift = -shift;
    if(old_size - shift <= 0) {
      *this = 0;
      return;
    }
    for(int pos = 0; pos < old_size - shift; pos++) storage[pos] = storage[pos + shift];
    storage.resize(old_size - shift);
  }
}

int2048 int2048::div_abs_brute(const int2048 &left, const int2048 &right) const {
  int2048 right_doubling = right, answer_doubling = 1;
  std::vector<int2048> right_list, answer_list;
  while(right_doubling <= left) {
    right_list.push_back(right_doubling);
    answer_list.push_back(answer_doubling);
    right_doubling += right_doubling;
    answer_doubling += answer_doubling;
  }
  int2048 res = left, answer = 0;
  while(!right_list.empty()) {
    if(right_list.back() <= res) {
      res -= right_list.back();
      answer += answer_list.back();
      if(res.is_zero()) return answer;
    }
    right_list.pop_back();
    answer_list.pop_back();
  }
  return answer;
}

int2048 int2048::inv() {
  if(storage.size() <= 5) {
    int2048 ident;
    ident.storage.resize(2 * storage.size() + 1);
    ident.storage[ident.storage.size() - 1] = 1;
    return div_abs_brute(ident, *this);
  }
  int this_size = storage.size(), half_size = this_size / 2 + 2;
  int2048 temp_b = *this;
  temp_b.shift(half_size - this_size);
  int2048 temp_c = temp_b.inv();
  temp_b = *this * temp_c;
  int2048 temp_d = temp_b * temp_c;
  temp_d.shift(-2 * half_size);
  temp_b = temp_c + temp_c;
  temp_b.shift(this_size - half_size);
  int2048 answer = temp_b - temp_d;
  answer -= 1;
  temp_b = answer * *this;
  int2048 ident;
  ident.storage.resize(2 * this_size + 1);
  ident.storage[ident.storage.size() - 1] = 1;
  temp_c = ident - temp_b;
  if(temp_c >= *this) answer += 1;
  return answer;
}

std::pair<int2048, int2048> int2048::div_abs(const int2048 &another) const {
  if(*this < another) return {0, *this};
  int2048 left = *this, right = another;
  int size_left = left.storage.size(), size_right = right.storage.size();
  if(size_left > 2 * size_right) {
    left.shift(size_left - 2 * size_right);
    right.shift(size_left - 2 * size_right);
    size_right = size_left - size_right;
    size_left = 2 * size_right;
  }

  int2048 temp = right.inv();
  right = left * temp;
  right.shift(-2 * size_right);
  temp = *this - another * right;
  while(temp >= another) {
    right += 1;
    temp -= another;
  }
  while(temp.storage_sign == -1 && !temp.is_zero()) {
    right -= 1;
    temp += another;
  }
  return {right, temp};
}


int2048 &int2048::operator/=(const int2048 &another) {
  if(another.is_zero()) throw std::runtime_error("Divided by zero");
  if(is_zero()) return *this;
  int2048 positive_this = (storage_sign == 1) ? *this : -*this;
  int2048 positive_another = (another.storage_sign == 1) ? another : -another;
  auto [quo, res] = positive_this.div_abs(positive_another);
  if(storage_sign == another.storage_sign) {
    *this = quo;
    return *this;
  } else {
    if(!res.is_zero()) quo += 1;
    quo.storage_sign = -1;
    *this = quo;
    return *this;
  }
}

int2048 operator/(int2048 left, const int2048 &right) {
  left /= right;
  return left;
}

int2048 &int2048::operator%=(const int2048 &another) {
  int2048 temp = *this;
  temp /= another;
  temp *= another;
  *this -= temp;
  return *this;
}

int2048 operator%(int2048 left, const int2048 &right) {
  left %= right;
  return left;
}

int2048::operator double() {
  if(is_zero()) return 0;
  double res = 0;
  for(int i = 0; i < storage.size(); i++)
    res = res * INT2048_BASE + storage[i];
  if(storage_sign == -1) res = -res;
  return res;
}

int2048::operator std::string() {
  if(is_zero()) return "0";
  std::string res = "";
  if(storage_sign == -1) res += "-";
  res += std::to_string(storage.back());
  for(auto pos_it = ++storage.rbegin(); pos_it != storage.rend(); ++pos_it) {
    storage_type integer = *pos_it;
    if(INT2048_BASE == 1000) {
      if(integer == 0) res += "000";
      else if(integer < 10) {
        res += "00";
        res += std::to_string(integer);
      }
      else if(integer < 100) {
        res += "0";
        res += std::to_string(integer);
      }
      else res += std::to_string(integer);
    } else throw std::runtime_error("Why you changed your base? Modify it.");
  }
  return res;
}


} // namespace sjtu
