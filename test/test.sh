#!/bin/bash

cc -S test/test.c

assert() {
    expected="$1"
    input="$2"

    ./9cc "$input" > tmp.s
    cc -o tmp tmp.s test.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

assert 0 'int main() { return 0; }'
assert 42 'int main() { return 42; }'
assert 100 'int main() { return 100; }'
assert 21 'int main() { return 5+20-4; }'
assert 41 'int main() { return  12 + 34 - 5 ; }'

assert 47 'int main() { return 5+6*7; }'
assert 15 'int main() { return 5*(9-6); }'
assert 4 'int main() { return (3+5)/2; }'

assert 10 'int main() { return - -10; }'
assert 10 'int main() { return - - +10; }'

assert 0 'int main() { return 0==1; }'
assert 1 'int main() { return 42==42; }'
assert 1 'int main() { return 0!=1; }'
assert 0 'int main() { return 42!=42; }'

assert 1 'int main() { return 0<1; }'
assert 0 'int main() { return 1<1; }'
assert 0 'int main() { return 2<1; }'
assert 1 'int main() { return 0<=1; }'
assert 1 'int main() { return 1<=1; }'
assert 0 'int main() { return 2<=1; }'

assert 1 'int main() { return 1>0; }'
assert 0 'int main() { return 1>1; }'
assert 0 'int main() { return 1>2; }'
assert 1 'int main() { return 1>=0; }'
assert 1 'int main() { return 1>=1; }'
assert 0 'int main() { return 1>=2; }'
assert 10 'int main() { int x; x=10; return x; }'
assert 3 'int main() { int a; int b; a=1; b=2; return a+b; }'
assert 12 'int main() { int a; a = 2; int b; b = 5 * 6 - 8; return (a + b) / 2; }'
assert 10 'int main() { int val; val=10; return val; }'
assert 7 'int main() { int ans; ans = 15; int tmp; tmp = 12 / 3; return ans - tmp * 2; }'
assert 15 'int main() { return 15; int tmp; tmp = 12 / 3; }'
assert 8 'int main() { return 8; return 10; }'
assert 3 'int main() { if (5 == 5) return 3; return 4; }'
assert 1 'int main() { if (3 < 1) return 3; return 1; }'
assert 4 'int main() { if (5 != 5) return 3; return 4; }'
assert 10 'int main() { if (5 < 5) return 3; else return 10; return 5; }'
assert 1 'int main() { int a; a = 0; if (5 == 5) a = 1; else a = 2; return a; }'
assert 10 'int main() { int a; a = 0; while (a < 10) a = a + 1; return a; }'
assert 18 'int main() { int a; a = 0; int i; for (i = 0; i < 9; i = i + 1) a = a + 2; return a; }'
assert 6 'int main() { int a; a = 2; int b; b = 3; if (9 > 8) {b = b + 1; a = a + b;} else a = a + 1; return a; }'
assert 12 'int main() { int a; a = 10; int b; b = 1; if (3 == 4) a = a + 1; else {b = b + 1; a = a + b;} return a; }'
assert 45 'int main() { int i; i = 0; int a; a = 0; while (i < 10) {a = a + i; i = i + 1;} return a; }'
assert 15 'int main() { int a; int b; a = 0; b = 0; int i; for (i = 0; i < 5; i = i + 1) {b = b + 1; a = a + b;} return a; }'
assert 4 'int main() { return ret_4(); }'
assert 9 'int main() { int a; a = ret_9(); return a; }'
assert 13 'int main() { return ret_4() + ret_9(); }'
assert 12 'int main() { return add_2(4, 8); }'
assert 21 'int main() { int a; a = 5; int ret; ret = add_6(1, 2, 3, ret_4(), a, add_2(1, 5)); return ret; }'
assert 11 'int main() { return sub(19, 11) + 3; }'
assert 17 'int mul(int a, int b) { return a * b; } int div(int a, int b) {return a / b; } int main() { return mul(2, 6) + div(10, 2); }'
assert 15 'int fib(int n) { if (n <= 1) return n; else return fib(n-1) + n;} int main() { return fib(5); }'
assert 3 'int main() { int x; x = sub(9, 6); int y; y = &x; return *y; }'
assert 3 'int main() { int x; int y; int z; x = 3; y = add_2(3, 4); z = &y + 8; return *z; }'
echo OK
