#include "hotpot/xmath.h"
#include "hotpot/typedef.h"
#include "hotpot/xbase.h"
#include "hotpot/xerror.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>

#define MAX_NUM 1000000
int g_prime[MAX_NUM];
int g_prime_num;
int g_is_prime[MAX_NUM];
int g_euler[MAX_NUM];

static void _geteuler()
{
	int	i,j,t;

	g_euler[1] = 0;
	for (i = 2;i < MAX_NUM;i++)
	{
		if(g_is_prime[i] == TRUE)
		{
			g_euler[i]= i - 1;
		}
		else
		{
			j = 0;
			//这个不可能出界哦……
			while(i % g_prime[j])
			{
				j++;
			}

			t = g_prime[j];
			
			//t = rho(i);
			//t为i的一个因子
			if ((i / t) % t)
			{
				g_euler[i] = g_euler[i / t] * g_euler[t];
			}
			else
			{
				g_euler[i] = t * g_euler[i / t];
			}
		}
	}
}

static void _getprime()
{
	int i,j;
	
	for(i = 0; i < MAX_NUM; ++i)
	{
		g_is_prime[i] = TRUE;
	}
	g_is_prime[0] = FALSE;//其实不是素数也不是合数
	g_is_prime[1] = FALSE;//其实不是素数也不是合数
	i = 2;
	while(i <= sqrt(MAX_NUM))
	{	
		while((i < MAX_NUM)&&(g_is_prime[i] == FALSE))
		{
			i++;
		}

		j = i * 2;
		while (j < MAX_NUM)
		{
			g_is_prime[j] = FALSE;
			j += i;
		}
		++i;
	}

	g_prime_num = 0;
	for(i = 2; i < MAX_NUM; ++i)
	{
		if(g_is_prime[i] == TRUE)
		{
			g_prime[g_prime_num] = i;
			++g_prime_num;
		}
	}
}

int xmath_init()
{
	g_prime_num = 0;
	memset(g_is_prime, 0, sizeof(g_is_prime));
	memset(g_euler, 0, sizeof(g_euler));
	_getprime();
	//最好初始化一个随机种子？
	srand((unsigned int)time(0));

	//这个计算比较慢， 考虑是否算好了编译进来……
	_geteuler();


	return XNOERROR;
}

int euler(int n)
{
	if((n >= 0) && (n < MAX_NUM))
	{
		return XERROR;
	}

	assert(g_euler[n] != XERROR);
	return g_euler[n];
}

uint64 mulmod(uint64 a, uint64 b, uint64 n)
{
	uint64	t;	
	
	t=0;
	while (b)
	{
		if (b & 1)
		{
			t = (t + a) % n;
		}
		a = (a + a) % n;;
		b >>= 1;
	}
	return t;
}

uint64 powermod(uint64 a,uint64 b, uint64 n)
{
	uint64 d;
	
	d = 1;
	while((b > 0)&&(a != 1))
	{
		if (b&1)
		{
			d = mulmod(d, a, n);
		}
		b = b>>1;
		a = mulmod(a, a, n);
	}
	return d;
}

int	rabin_miller(uint64	n)
{
	const unsigned int	rule[10]={2,3,5,7,11,13,17,19,23,29};
	uint64	x,b,last;
	int					i,j,count;	

	if(n < MAX_NUM)
	{
		return g_is_prime[n];
	}

	count = 0;
	b = n - 1;
	while(!(b & 1))
	{
		++count;
		b >>= 1;
	}
		
	for(i = 0; i < 10; ++i)
	{
		if(rule[i] == n)
		{
			return TRUE;
		}
		x = powermod(rule[i], b, n);

		if (x == 1)
		{
			continue;
		}
		 
		for(j = 1; j <= count; ++j)
		{
			last = x;
			x = mulmod(last, last, n);
			if(x == 1)
			{
				if(last != n - 1)
				{
					return FALSE;
				}
				else
				{
					break;
				}
			}
		}
		if(x != 1)
		{
			return FALSE;
		}
	}

	return TRUE;
}

uint64	gcd(uint64 a, uint64 b)
{
	uint64 t;
		
	while(b)
	{
		t = a;
		a = b;
		b = t % b;
	}
	
	return a;
}

uint64 rho(uint64 n)
{
	uint64	x,y,k,i,d,c;
	
	while(TRUE)
	{	
		x = rand()%(n-2)+2;;
		y = x;
		c = rand()%n;

		i = 0;
		k = 1;
		
		d = 1;
		while(d == 1)
		{
			++i;
			x = (mulmod(x, x, n) + n - c) % n;
			if (x > y)
			{
				d = gcd(x-y, n);
			}
			else
			{
				d = gcd(y - x, n);
			}

			if((d != 1)&&(d != n))
			{
				return d;
			}
		
			if( i== k)
			{
				y = x;
				k <<= 1;
			}
		}
	}

	return d;
}

void extendgcd(int64	a,int64	b,int64	*x,int64	*y, int64 *d)
{
	int64 tx, ty;
	
	if(b == 0)
	{
		*x = 1;
		*y = 0;
		*d = a;
		return;
	}
	
	extendgcd(b, a % b, &tx, &ty, d);	
	*x = ty;
	*y = tx - (a / b) * ty;
}

//ax=b(mod n)
//d是a,n的最大公约数，表示解空间有d个基
//tx返回最小的大于0的解。
//一组最小基为tx+i*n/d  (0<=i<d)
int	congruence(int64 a, int64 b, int64 n, int64 *tx, int64 *d)
{
	int64 ty, t;
	
	extendgcd(a, n, tx, &ty, d);
	if(b % *d)
	{
		return FALSE;
	}
		
	t = n / (*d);
	*tx *= b / (*d);
	*tx = (*tx % t + t) % t;

	return TRUE;
}

int equation(int64 a, int64 b, int64 n, int64 *tx, int64 *ty, int64 *d)
{
	int64 t;

	extendgcd(a, b, tx, ty, d);
	if(n % (*d))
	{
		return FALSE;
	}
	t = b / (*d);
	*tx *= n / (*d);
	*tx = ((*tx) % t + t) % t;
	*ty= (n - a * (*tx)) / b;

	return TRUE;
}

int64 lcm(int64	a,int64	b)
{
	return(a* b / gcd(a, b));
}

int	china(int64	*a,int64 *m,int	tail,int64	*a1,int64	*m1)
{
	int64	tx,ty,d,t;
	int		i;
	
	*a1 = a[1];
	*m1 = m[1];
	
	for(i = 2; i <= tail; ++i)
	{		
		extendgcd(*m1, m[i], &tx, &ty, &d);
		t = ((a[i] - (*a1)) %m[i] + m[i]) % m[i];	
		if(t % d)
		{
			return FALSE;
		}
		tx *= t / d;
		tx = (tx % (m[i] / d) + (m[i] / d)) % (m[i] / d);
		
		*a1 = (*a1) + (*m1) * tx;
		*m1 = (*m1) * m[i] / d;		
		*a1 %= (*m1);
	}

	return TRUE;
}
