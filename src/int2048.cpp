#include "int2048.h"
namespace sjtu {

int2048::int2048() {
  op = 0;
  a.push_back(0);
}

int2048::int2048(long long val) {
  a.clear();
  op = 0;
  if (val < 0) {
    op = 1;
    val = -val;
  }
  while (val != 0) {
    a.push_back(val % bit);
    val /= bit;
  }
  if (a.size() == 0) {
    a.push_back(0);
  }
}
int2048::int2048(const std::string &s) { read(s); }
int2048::int2048(const int2048 &oth) {
  op = oth.op;
  a.clear();
  for (int i = 0; i < oth.a.size(); i++) {
    a.push_back(oth.a[i]);
  }
}
void int2048::read(const std::string &s) {
  a.clear();
  if (s[0] == '-') {
    op = 1;
    int val = 0, b = 1, m = s.length() - 1;
    for (int i = 1; i <= m; i++) {
      if (b == bit) {
        a.push_back(val);
        b = 1;
        val = 0;
      }
      val += b * (s[m + 1 - i] - '0');
      b *= 10;
    }
    if (val) {
      a.push_back(val);
      b = 1;
      val = 0;
    }
  } else {
    op = 0;
    int val = 0, b = 1, m = s.length();
    for (int i = 1; i <= m; i++) {
      if (b == bit) {
        a.push_back(val);
        b = 1;
        val = 0;
      }
      val += b * (s[m - i] - '0');
      b *= 10;
    }
    if (val) {
      a.push_back(val);
      b = 1;
      val = 0;
    }
  }
  if (a.size() == 0) {
    a.push_back(0);
  }
  return;
}
void int2048::print() {
  //  printf("n:%d\n",n);
  if (is_zero(*this)) {
    printf("0");
    return;
  }
  // if(a.size()==0||a[a.size()-1]==0) printf("NOOOOOO%d\n",a.size());
  if (op) {
    printf("-");
  }
  for (int i = a.size() - 1; i >= 0; i--) {
    if (i != a.size() - 1) {
      for (int j = 10; j < bit; j *= 10) {
        if (a[i] < j)
          printf("0");
      }
    }
    printf("%lld", a[i]);
  }
  return;
}
int2048 add_abs(int2048 x, const int2048 &y) {
  //  printf("after:%d %d\n",x.a[1],x.a[2]);x.print();puts("");
  for (int i = x.a.size(); i < y.a.size(); i++) {
    x.a.push_back(0);
  }
  for (int i = 0; i < y.a.size(); i++) {
    x.a[i] += y.a[i];
  }
  for (int i = 0; i + 1 < x.a.size(); i++) {
    x.a[i + 1] += x.a[i] / bit;
    x.a[i] %= bit;
  }
  while (x.a[x.a.size() - 1] >= bit) {
    //  printf("%d %d\n",x.a[m],x.a[m+1]);
    x.a.push_back(x.a[x.a.size() - 1] / bit);
    x.a[x.a.size() - 2] %= bit;
  }
  return x;
}
int2048 minus_abs(int2048 x, const int2048 &y) {
  //  printf("x:");x.print();puts("");
  for (int i = 0; i < y.a.size(); i++) {
    x.a[i] -= y.a[i];
    if (x.a[i] < 0) {
      x.a[i] += bit;
      x.a[i + 1] -= 1;
    }
  }
  for (int i = y.a.size(); i + 1 < x.a.size(); i++) {
    if (x.a[i] >= 0)
      break;
    x.a[i] += bit;
    x.a[i + 1] -= 1;
  }
  while (x.a.size() > 1 && x.a[x.a.size() - 1] == 0) {
    x.a.pop_back();
  }
  //  printf("n:%d\n",x.n);
  return x;
}
int2048 &int2048::add(const int2048 &x) {
  if (op != x.op) {
    if (bigger_abs((*this), x))
      *this = minus_abs(*this, x);
    else
      *this = minus_abs(x, *this);
  } else {
    //    printf("add:%d %d\n",a[1],a[2]);
    (*this) = add_abs(*this, x);
    //  printf("after add:");print();puts("");
  }
  return *this;
}
int2048 add(int2048 x, const int2048 &y) {
  if (x.op != y.op) {
    //    printf("a");
    if (bigger_abs(x, y))
      return minus_abs(x, y);
    else
      return minus_abs(y, x);
  } else {
    //    printf("b");
    x = add_abs(x, y);
    //  x.print();puts("");
  }
  return x;
}
int2048 &int2048::minus(const int2048 &x) {
  if (op != x.op) {
    *this = add_abs(*this, x);
  } else {
    if (bigger_abs((*this), x))
      *this = minus_abs(*this, x);
    else {
      *this = minus_abs(x, *this);
      this->op ^= 1;
    }
  }
  return *this;
}
int2048 minus(int2048 x, const int2048 &y) {
  if (x.op != y.op) {
    return add_abs(x, y);
  } else {
    //  printf("%d %d\n",x.n,y.n);
    //  x.print();puts("");
    //  int2048(y).print();puts("");
    if (bigger_abs(x, y))
      x = minus_abs(x, y);
    else {
      x = minus_abs(y, x);
      x.op ^= 1;
    }
    //  printf("+/-:%d\n",x.op);
  }
  return x;
}
int2048 int2048::operator+() const { return *this; }
int2048 int2048::operator-() const {
  int2048 x(*this);
  x.op ^= 1;
  return x;
}

int2048 &int2048::operator=(const int2048 &oth) {
  a.clear();
  op = oth.op;
  for (int i = 0; i < oth.a.size(); i++) {
    a.push_back(oth.a[i]);
  }
  return *this;
}
int2048 &int2048::operator+=(const int2048 &x) {
  //  printf("before:");print();puts("");
  //  printf("%d %d\n",a[1],a[2]);
  add(x);
  //  printf("show time:");print();puts("");
  return *this;
}
int2048 operator+(int2048 x, const int2048 &y) {
  x = add(x, y);
  return x;
}

int2048 &int2048::operator-=(const int2048 &x) {
  minus(x);
  return *this;
}
int2048 operator-(int2048 x, const int2048 &y) {
  x = minus(x, y);
  return x;
}

int2048 &int2048::operator*=(const int2048 &x) {
  *this = (*this) * x;
  return *this;
}
int rev[N], n;
const long long inf = 1231453023109121ll, V = 3, inV = 410484341036374ll;

long long chen(long long x, long long y) {
  long long ans = 1;
  while (y) {
    if (y & 1)
      ans = (__int128)ans * x % inf;
    //  printf("x:%lld\n",x);
    x = (__int128)x * x % inf;
    y >>= 1;
  }
  //  puts("");
  return ans;
}

void ntt(int2048 &f, int id) {
  for (int i = 0; i < n; i++)
    if (i < rev[i])
      std::swap(f.a[i], f.a[rev[i]]);
  for (int len = 2; len <= n; len <<= 1) {
    int h = len / 2;
    long long w = chen((id == 1) ? V : inV, (inf - 1) / len);
    for (int i = 0; i < n; i += len) {
      long long b = 1;
      for (int j = i; j < i + h; j++) {
        long long x = f.a[j], y = (__int128)b * f.a[j + h] % inf;
        f.a[j] = (x + y) % inf;
        f.a[j + h] = (x - y + inf) % inf;
        b = (__int128)b * w % inf;
      }
    }
  }
  if (id == -1) {
    long long inv = chen(n, inf - 2);
    for (int i = 0; i < n; i++)
      f.a[i] = (__int128)f.a[i] * inv % inf;
  }
}
int2048 operator*(int2048 x, int2048 y) {
  if (x.op == y.op)
    x.op = 0;
  else
    x.op = 1;
  int m = x.a.size() + y.a.size() - 2;
  n = 1;
  while (n <= m)
    n <<= 1;
  for (int i = x.a.size(); i < n; i++)
    x.a.push_back(0);
  for (int i = y.a.size(); i < n; i++)
    y.a.push_back(0);
  for (int i = 0; i < n; i++)
    rev[i] = (rev[i >> 1] >> 1) | ((i & 1) ? (n >> 1) : 0);
  ntt(x, 1);
  ntt(y, 1);
  for (int i = 0; i < n; i++)
    x.a[i] = (__int128)x.a[i] * y.a[i] % inf;
  ntt(x, -1);
  while (x.a.size() > m + 1)
    x.a.pop_back();
  for (int i = 0; i + 1 < x.a.size(); i++) {
    x.a[i + 1] += x.a[i] / bit;
    x.a[i] %= bit;
  }
  while (x.a[x.a.size() - 1] >= bit) {
    x.a.push_back(x.a[x.a.size() - 1] / bit);
    x.a[x.a.size() - 2] %= bit;
  }
  while (x.a.size() > 1 && x.a[x.a.size() - 1] == 0) {
    x.a.pop_back();
  }
  return x;

  /*    for(int i=0;i<x.a.size()+y.a.size();i++) ans.a.push_back(0);
      for(int i=0;i<x.a.size();i++){
        for(int j=0;j<y.a.size();j++){
          ans.a[i+j]+=x.a[i]*y.a[j];
        }
      }
      for(int i=0;i+1<ans.a.size();i++){
        ans.a[i+1]+=ans.a[i]/bit;
        ans.a[i]%=bit;
      }
      while(ans.a[ans.a.size()-1]>=bit){
        ans.a.push_back(ans.a[ans.a.size()-1]/bit);
        ans.a[ans.a.size()-2]%=bit;
      }
      while(ans.a.size()>1&&ans.a[ans.a.size()-1]==0){
        ans.a.pop_back();
      }
      */
}
bool bigger(const int2048 &x, const int2048 &y, int b) {
  //  printf("Bigger:\n");int2048(y).print();puts("");
  if (x.a.size() < y.a.size() + b)
    return 0;
  if (x.a.size() > y.a.size() + b)
    return 1;
  for (int i = y.a.size() - 1; i >= 0; i--) {
    //  printf("%d %d\n",x.a[b+i],y.a[i]);
    if (x.a[b + i] > y.a[i]) {
      return 1;
    }
    if (x.a[b + i] < y.a[i]) {
      return 0;
    }
  }
  return 1;
}
int2048 del(int2048 x, const int2048 &y, int b) {
  for (int i = 0; i < y.a.size(); i++) {
    x.a[i + b] -= y.a[i];
    if (x.a[i + b] < 0) {
      x.a[i + b] += bit;
      x.a[i + b + 1] -= 1;
    }
  }
  while (x.a.size() > 1 && x.a[x.a.size() - 1] == 0) {
    x.a.pop_back();
  }
  return x;
}
int2048 &int2048::operator/=(const int2048 &x) {
  *this = *this / x;
  return *this;
}
int2048 move_left(const int2048 &x, int k) {
  int2048 ans;
  ans.a.clear();
  for (int i = 1; i <= k; i++)
    ans.a.push_back(0);
  for (int i = 0; i < x.a.size(); i++)
    ans.a.push_back(x.a[i]);
  return ans;
}
int2048 move_right(const int2048 &x, int k) {
  int2048 ans;
  ans.a.clear();
  for (int i = k; i < x.a.size(); i++)
    ans.a.push_back(x.a[i]);
  return ans;
}
int2048 baoli(const int2048 &x, const int2048 &y) {
  if (y == int2048(1))
    return x;
  std::vector<int2048> sta1;
  std::vector<int2048> sta2;
  int2048 b(y), ans(1);
  while (b <= x) {
    sta1.push_back(b);
    sta2.push_back(ans);
    b += b;
    ans += ans;
  }
  b = x;
  ans = int2048(0);
  for (int i = sta1.size() - 1; i >= 0; i--) {
    if (b >= sta1[i]) {
      b -= sta1[i];
      ans += sta2[i];
    }
  }
  return ans;
}
int2048 inverse(const int2048 &x) {
  int num = x.a.size();
  if (num <= 9) {
    return baoli(move_left(1, num * 2), x);
  }
  int k = (num + 5) / 2;
  int2048 c = inverse(move_right(x, num - k));
  int2048 c1 = c + c;
  c1 = move_left(c1, num - k);
  int2048 y = move_right(x * c * c, k * 2);
  int2048 ans = c1 - y - int2048(1);
  c = move_left(1, num * 2) - ans * x;
  if (c >= x)
    ans += int2048(1);
  return ans;
}
int2048 operator/(int2048 x, int2048 y) {
  //  printf("HERE:\n");x.print();puts("");
  //  int2048(y).print();puts("");
  int2048 ans;
  bool o;
  ans.a.clear();
  if (x.op == y.op)
    o = 0;
  else
    o = 1;
  x.op = y.op = 0;
  if (x.a.size() < y.a.size()) {
    if (o == 0)
      ans.a.push_back(0);
    else
      ans.a.push_back(1);
    ans.op = o;
    return ans;
  }
  int numx = x.a.size(), numy = y.a.size();
  int2048 xx(x), yy(y);
  if (numx > (numy * 2)) {
    int k = numx - numy * 2;
    xx = move_left(xx, k);
    yy = move_left(yy, k);
    numy = numx - numy;
  }
  ans = xx * inverse(yy);
  ans = move_right(ans, numy * 2);
  int2048 yu = x - y * ans;
  if (yu >= y)
    ans += int2048(1);
  if (o == 1 && y * ans != x) {
    ans += int2048(1);
  }
  ans.op = o;
  while (ans.a.size() > 1 && ans.a[ans.a.size() - 1] == 0) {
    ans.a.pop_back();
  }
  return ans;
}

int2048 &int2048::operator%=(const int2048 &x) {
  *this = *this - ((*this) / x) * x;
  return *this;
}
int2048 operator%(int2048 x, const int2048 &y) {
  x -= (x / y) * y;
  return x;
}

std::istream &operator>>(std::istream &is, int2048 &x) {
  std::string val;
  //  printf("before\n");std::cout.flush();
  is >> val; // std::cout.flush();std::cout<<val<<'\n';
  //  std::cout.flush();
  //  printf("till now\n");
  x.read(val);
  //  printf("down\n");
  return is;
}
std::ostream &operator<<(std::ostream &os, const int2048 &x) {
  //  printf("Now I cout:");
  //  val.print();
  //  os<<"H "<<x.a.size()<<' '<<x.a[x.a.size()-1]<<std::endl;
  if (is_zero(x)) {
    os << 0;
    return os;
  }
  // if(a.size()==0||a[a.size()-1]==0) printf("NOOOOOO%d\n",a.size());
  if (x.op) {
    os << '-';
  }
  for (int i = x.a.size() - 1; i >= 0; i--) {
    if (i != x.a.size() - 1) {
      for (int j = 10; j < bit; j *= 10) {
        if (x.a[i] < j)
          os << 0;
      }
    }
    os << x.a[i];
  }
  return os;
}

bool is_zero(const int2048 &x) {
  if (x.a.size() == 1 && x.a[0] == 0)
    return 1;
  return 0;
}
bool bigger_abs(const int2048 &x, const int2048 &y) {
  if (x.a.size() != y.a.size())
    return x.a.size() > y.a.size();
  for (int i = x.a.size() - 1; i >= 0; i--) {
    if (x.a[i] < y.a[i])
      return 0;
    if (x.a[i] > y.a[i])
      return 1;
  }
  return 0;
}
bool bigger_eq_abs(const int2048 &x, const int2048 &y) {
  if (x.a.size() != y.a.size())
    return x.a.size() > y.a.size();
  for (int i = x.a.size() - 1; i >= 0; i--) {
    if (x.a[i] < y.a[i])
      return 0;
    if (x.a[i] > y.a[i])
      return 1;
  }
  return 1;
}
bool operator==(const int2048 &x, const int2048 &y) {
  if (is_zero(x) && is_zero(y))
    return 1;
  if (x.op != y.op)
    return 0;
  if (x.a.size() != y.a.size())
    return 0;
  for (int i = 0; i < x.a.size(); i++) {
    if (x.a[i] != y.a[i])
      return 0;
  }
  return 1;
}
bool operator!=(const int2048 &x, const int2048 &y) {
  if (is_zero(x) && is_zero(y))
    return 0;
  if (x.op != y.op)
    return 1;
  if (x.a.size() != y.a.size())
    return 1;
  for (int i = 0; i < x.a.size(); i++) {
    if (x.a[i] != y.a[i])
      return 1;
  }
  return 0;
}

bool operator<(const int2048 &x, const int2048 &y) {
  if (is_zero(x) && is_zero(y))
    return 0;
  if (x.op == 1 && y.op == 0)
    return 1;
  if (x.op == 0 && y.op == 1)
    return 0;
  if (x.op == 1) {
    return bigger_abs(x, y);
  }
  return bigger_abs(y, x);
}
bool operator>(const int2048 &x, const int2048 &y) {
  if (is_zero(x) && is_zero(y))
    return 0;
  if (x.op == 1 && y.op == 0)
    return 0;
  if (x.op == 0 && y.op == 1)
    return 1;
  if (x.op == 1) {
    return bigger_abs(y, x);
  }
  return bigger_abs(x, y);
}
bool operator<=(const int2048 &x, const int2048 &y) {
  if (is_zero(x) && is_zero(y))
    return 1;
  if (x.op == 1 && y.op == 0)
    return 1;
  if (x.op == 0 && y.op == 1)
    return 0;
  if (x.op == 1) {
    return bigger_eq_abs(x, y);
  }
  return bigger_eq_abs(y, x);
}
bool operator>=(const int2048 &x, const int2048 &y) {
  if (is_zero(x) && is_zero(y))
    return 1;
  if (x.op == 1 && y.op == 0)
    return 0;
  if (x.op == 0 && y.op == 1)
    return 1;
  if (x.op == 1) {
    return bigger_eq_abs(y, x);
  }
  return bigger_eq_abs(x, y);
}

double turn_to_double(const int2048 &x) {
  double ans = 0;
  for (int i = x.a.size() - 1; i >= 0; i--) {
    ans = ans * bit + x.a[i];
  }
  return ans;
}
std::string int_to_str(long long x) {
  std::string ans = "";
  while (x) {
    char c = '0' + x % 10;
    ans = c + ans;
    x /= 10;
  }
  return ans;
}
std::string turn_to_str(const int2048 &x) {
  std::string ans = "";
  for (int i = 1; i < x.a.size(); i++) {
    ans = int_to_str(x.a[i]) + ans;
  }
  return ans;
}

} // namespace sjtu
  /*
  1 68 19 61 78 56 86 30 66 46 00 67 51 94 83 04 76 55 83 96 02 08 01
  663720250075908693888302580638   1 68 19 61 78 56 86 30 66 46 00 67 51 94 83 04 76
  55 83 96 10 81 27 230899809780398974552684228297
  */