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

assert 0 'main() { return 0; }'
assert 42 'main() { return 42; }'
assert 100 'main() { return 100; }'
assert 21 'main() { return 5+20-4; }'
assert 41 'main() { return  12 + 34 - 5 ; }'

assert 47 'main() { return 5+6*7; }'
assert 15 'main() { return 5*(9-6); }'
assert 4 'main() { return (3+5)/2; }'

assert 10 'main() { return - -10; }'
assert 10 'main() { return - - +10; }'

assert 0 'main() { return 0==1; }'
assert 1 'main() { return 42==42; }'
assert 1 'main() { return 0!=1; }'
assert 0 'main() { return 42!=42; }'

assert 1 'main() { return 0<1; }'
assert 0 'main() { return 1<1; }'
assert 0 'main() { return 2<1; }'
assert 1 'main() { return 0<=1; }'
assert 1 'main() { return 1<=1; }'
assert 0 'main() { return 2<=1; }'

assert 1 'main() { return 1>0; }'
assert 0 'main() { return 1>1; }'
assert 0 'main() { return 1>2; }'
assert 1 'main() { return 1>=0; }'
assert 1 'main() { return 1>=1; }'
assert 0 'main() { return 1>=2; }'
assert 10 'main() { x=10; return x; }'
assert 3 'main() { a=1; b=2; return a+b; }'
assert 12 'main() { a = 2; b = 5 * 6 - 8; return (a + b) / 2; }'
assert 10 'main() { val=10; return val; }'
assert 7 'main() { ans = 15; tmp = 12 / 3; return ans - tmp * 2; }'
assert 15 'main() { return 15; tmp = 12 / 3; }'
assert 8 'main() { return 8; return 10; }'
assert 3 'main() { if (5 == 5) return 3; return 4; }'
assert 1 'main() { if (3 < 1) return 3; return 1; }'
assert 4 'main() { if (5 != 5) return 3; return 4; }'
assert 10 'main() { if (5 < 5) return 3; else return 10; return 5; }'
assert 1 'main() { a = 0; if (5 == 5) a = 1; else a = 2; return a; }'
assert 10 'main() { a = 0; while (a < 10) a = a + 1; return a; }'
assert 18 'main() { a = 0; for (i = 0; i < 9; i = i + 1) a = a + 2; return a; }'
assert 6 'main() { a = 2; b = 3; if (9 > 8) {b = b + 1; a = a + b;} else a = a + 1; return a; }'
assert 12 'main() { a = 10; b = 1; if (3 == 4) a = a + 1; else {b = b + 1; a = a + b;} return a; }'
assert 45 'main() { i = 0; a = 0; while (i < 10) {a = a + i; i = i + 1;} return a; }'
assert 15 'main() { a = 0; b = 0; for (i = 0; i < 5; i = i + 1) {b = b + 1; a = a + b;} return a; }'
assert 4 'main() { return ret_4(); }'
assert 9 'main() { a = ret_9(); return a; }'
assert 13 'main() { return ret_4() + ret_9(); }'
assert 12 'main() { return add_2(4, 8); }'
assert 21 'main() { a = 5; ret = add_6(1, 2, 3, ret_4(), a, add_2(1, 5)); return ret; }'
assert 11 'main() { return sub(19, 11) + 3; }'
assert 17 'mul(a, b) { return a * b; } main() { return mul(2, 6) + 5; }'
assert 15 'fib(n) { if (n <= 1) return n; else return fib(n-1) + n;} main() { return fib(5); }'
assert 3 'main() { x = sub(9, 6); y = &x; return *y; }'
assert 3 'main() { x = 3; y = add_2(3, 4); z = &y + 8; return *z; }'
echo OK
