#ifndef _H_XMATH
#define _H_XMATH

#include "xbase.h"
#include "typedef.h"

//初始化xmath，计算10^6以内的素数和欧拉函数
//速度有点恶心
XAPI int xmath_init();

//a*b % n
XAPI uint64 mulmod(uint64 a, uint64 b, uint64 n);

//a^b %n
XAPI uint64 powermod(uint64 a,uint64 b, uint64 n);

//最大公约数
XAPI uint64	gcd(uint64 a, uint64 b);

//欧拉函数
XAPI int euler(int n);

//素数测试
XAPI int	rabin_miller(uint64	n);

//因数分解
XAPI uint64 rho(uint64 n);

//a * x + b * y = 1
XAPI void extendgcd(int64	a,int64	b,int64	*x,int64	*y, int64 *d);

//ax+bx=n
//d是a,b的最大公约数，表示解空间有d个基
//(tx,ty)返回第一个tx大于0的解
//一组最小基为(tx+i*b/d,ty-i*a/d)  (0<=i<d)
//ty*d/a+1为非负整数解的个数
XAPI int	equation(int64	a,int64	b,int64	n,int64	*tx,int64	*ty,int64	*d);

//最小公倍数
XAPI int64 lcm(int64	a,int64	b);

//x=Ai(mod Mi)
//假设C ≡ A1 (mod M1)，C ≡ A2 (mod M2)。
//令C = A1 + X1M1，那么X1M1 ≡ A2 - A1 (mod M2)。
//用扩展欧几里德算法求出X1，也就求出C。
//令M = lcm(M1, M2)，那么上面两条方程就可以被C’ ≡ C (mod M)代替。
//迭代直到只剩下一条方程。
//方程在[0,m1)内有唯一的解a1，（解空间基为1）
//a1+i*m1为所有解(m1为m[]的最小公倍数)
XAPI int	china(int64	*a,int64 *m,int	tail,int64	*a1,int64	*m1);

#endif//_H_XMATH
