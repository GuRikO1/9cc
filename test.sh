#!/bin/bash

assert() {
    expected="$1"
    input="$2"

    ./9cc "$input" > tmp.s
    cc -o tmp tmp.s
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
echo OK
