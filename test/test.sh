#!/bin/bash

assert() {
    expected="$1"
    input="$2"

    ./9cc "$input" > tmp.s
    cc -S test/test.c
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

assert 0 'return 0;'
assert 42 'return 42;'
assert 100 'return 100;'
assert 21 'return 5+20-4;'
assert 41 'return  12 + 34 - 5 ;'

assert 47 'return 5+6*7;'
assert 15 'return 5*(9-6);'
assert 4 'return (3+5)/2;'

assert 10 'return - -10;'
assert 10 'return - - +10;'

assert 0 'return 0==1;'
assert 1 'return 42==42;'
assert 1 'return 0!=1;'
assert 0 'return 42!=42;'

assert 1 'return 0<1;'
assert 0 'return 1<1;'
assert 0 'return 2<1;'
assert 1 'return 0<=1;'
assert 1 'return 1<=1;'
assert 0 'return 2<=1;'

assert 1 'return 1>0;'
assert 0 'return 1>1;'
assert 0 'return 1>2;'
assert 1 'return 1>=0;'
assert 1 'return 1>=1;'
assert 0 'return 1>=2;'
assert 10 'x=10; return x;'
assert 3 'a=1; b=2; return a+b;'
assert 12 'a = 2; b = 5 * 6 - 8; return (a + b) / 2;'
assert 10 'val=10; return val;'
assert 7 'ans = 15; tmp = 12 / 3; return ans - tmp * 2;'
assert 15 'return 15; tmp = 12 / 3;'
assert 8 'return 8; return 10;'
assert 3 'if (5 == 5) return 3; return 4;'
assert 1 'if (3 < 1) return 3; return 1;'
assert 4 'if (5 != 5) return 3; return 4;'
assert 10 'if (5 < 5) return 3; else return 10; return 5;'
assert 1 'a = 0; if (5 == 5) a = 1; else a = 2; return a;'
assert 10 'a = 0; while (a < 10) a = a + 1; return a;'
assert 18 'a = 0; for (i = 0; i < 9; i = i + 1) a = a + 2; return a;'
assert 6 'a = 2; b = 3; if (9 > 8) {b = b + 1; a = a + b;} else a = a + 1; return a;'
assert 12 'a = 10; b = 1; if (3 == 4) a = a + 1; else {b = b + 1; a = a + b;} return a;'
assert 45 'i = 0; a = 0; while (i < 10) {a = a + i; i = i + 1;} return a;'
assert 15 'a = 0; b = 0; for (i = 0; i < 5; i = i + 1) {b = b + 1; a = a + b;} return a;'
assert 4 'return ret_4();'
assert 9 'return ret_9();'
echo OK
