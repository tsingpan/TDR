#include "hotpot/hp_math.h"
#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>

#define HP_MAX_NUM 1000000
static hpint32 g_prime[HP_MAX_NUM];
static hpint32 g_prime_num;
static hpint32 g_is_prime[HP_MAX_NUM];
static hpint32 g_euler[HP_MAX_NUM];

static void _geteuler()
{
	hpint32	i,j,t;

	g_euler[1] = 0;
	for (i = 2;i < HP_MAX_NUM;i++)
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
	hpint32 i,j;
	
	for(i = 0; i < HP_MAX_NUM; ++i)
	{
		g_is_prime[i] = TRUE;
	}
	g_is_prime[0] = FALSE;//其实不是素数也不是合数
	g_is_prime[1] = FALSE;//其实不是素数也不是合数
	i = 2;
	while(i <= sqrt(HP_MAX_NUM))
	{	
		while((i < HP_MAX_NUM)&&(g_is_prime[i] == FALSE))
		{
			i++;
		}

		j = i * 2;
		while (j < HP_MAX_NUM)
		{
			g_is_prime[j] = FALSE;
			j += i;
		}
		++i;
	}

	g_prime_num = 0;
	for(i = 2; i < HP_MAX_NUM; ++i)
	{
		if(g_is_prime[i] == TRUE)
		{
			g_prime[g_prime_num] = i;
			++g_prime_num;
		}
	}
}

hpint32 hp_math_init()
{
	g_prime_num = 0;
	memset(g_is_prime, 0, sizeof(g_is_prime));
	memset(g_euler, 0, sizeof(g_euler));
	_getprime();
	//最好初始化一个随机种子？
	srand((hpuint32)time(0));

	//这个计算比较慢， 考虑是否算好了编译进来……
	_geteuler();


	return E_HP_NOERROR;
}

hpint32 hp_euler(hpint32 n)
{
	if((n >= 0) && (n < HP_MAX_NUM))
	{
		return E_HP_ERROR;
	}

	assert(g_euler[n] != E_HP_ERROR);
	return g_euler[n];
}

hpuint64 hp_mulmod(hpuint64 a, hpuint64 b, hpuint64 n)
{
	hpuint64	t;	
	
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

hpuint64 hp_powermod(hpuint64 a,hpuint64 b, hpuint64 n)
{
	hpuint64 d;
	
	d = 1;
	while((b > 0)&&(a != 1))
	{
		if (b&1)
		{
			d = hp_mulmod(d, a, n);
		}
		b = b>>1;
		a = hp_mulmod(a, a, n);
	}
	return d;
}

hpint32	hp_rabin_miller(hpuint64	n)
{
	const hpuint32	rule[10]={2,3,5,7,11,13,17,19,23,29};
	hpuint64	x,b,last;
	hpint32					i,j,count;	

	if(n < HP_MAX_NUM)
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
		x = hp_powermod(rule[i], b, n);

		if (x == 1)
		{
			continue;
		}
		 
		for(j = 1; j <= count; ++j)
		{
			last = x;
			x = hp_mulmod(last, last, n);
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

hpuint64	hp_gcd(hpuint64 a, hpuint64 b)
{
	hpuint64 t;
		
	while(b)
	{
		t = a;
		a = b;
		b = t % b;
	}
	
	return a;
}

hpuint64 hp_rho(hpuint64 n)
{
	hpuint64	x,y,k,i,d,c;
	
	for(;;)
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
			x = (hp_mulmod(x, x, n) + n - c) % n;
			if (x > y)
			{
				d = hp_gcd(x-y, n);
			}
			else
			{
				d = hp_gcd(y - x, n);
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
}

void hp_extendgcd(hpint64	a,hpint64	b,hpint64	*x,hpint64	*y, hpint64 *d)
{
	hpint64 tx, ty;
	
	if(b == 0)
	{
		*x = 1;
		*y = 0;
		*d = a;
		return;
	}
	
	hp_extendgcd(b, a % b, &tx, &ty, d);	
	*x = ty;
	*y = tx - (a / b) * ty;
}

//ax=b(mod n)
//d是a,n的最大公约数，表示解空间有d个基
//tx返回最小的大于0的解。
//一组最小基为tx+i*n/d  (0<=i<d)
hpint32	congruence(hpint64 a, hpint64 b, hpint64 n, hpint64 *tx, hpint64 *d)
{
	hpint64 ty, t;
	
	hp_extendgcd(a, n, tx, &ty, d);
	if(b % *d)
	{
		return FALSE;
	}
		
	t = n / (*d);
	*tx *= b / (*d);
	*tx = (*tx % t + t) % t;

	return TRUE;
}

hpint32 hp_equation(hpint64 a, hpint64 b, hpint64 n, hpint64 *tx, hpint64 *ty, hpint64 *d)
{
	hpint64 t;

	hp_extendgcd(a, b, tx, ty, d);
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

hpint64 hp_lcm(hpint64	a,hpint64	b)
{
	return(a* b / hp_gcd(a, b));
}

hpint32	hp_china(hpint64	*a,hpint64 *m,hpint32	tail,hpint64	*a1,hpint64	*m1)
{
	hpint64	tx,ty,d,t;
	hpint32		i;
	
	*a1 = a[1];
	*m1 = m[1];
	
	for(i = 2; i <= tail; ++i)
	{		
		hp_extendgcd(*m1, m[i], &tx, &ty, &d);
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
