/*#include <NTL/ZZ.h>
#include <Windows.h>
#include <NTL/vec_ZZ.h>
#include <NTL/mat_ZZ.h>

using namespace std;
using namespace NTL;

void main() {
	
	ZZ p,g,q,r,flag,sum,mul;
	ZZ p1, temp, temp1, temp2,temp3,temp4,temp5,temp6;
	ZZ ct0;
	ZZ er,inver,ans;
	ZZ one, zero;
	long i,l;
	p = GenPrime_ZZ(3);
	cout <<"p="<< p << "\n";
	p1 = p - 1;
	cout << "p-1=" << p1 << "\n";
	l = 4	;
	temp3 = 1;
	temp4 = 1;
	zero = 0;
	one = 1;
	sum = 0;
	mul = 1;
	//g生成
	flag = 1;
	while (flag==1) {
		g = GenPrime_ZZ(2);
		if ((p1 > g) && (g > 1))       //1<g<p-1
		{
			PowerMod(temp1, g, 2, p);           //g^2 mod p!=1 且 g ^ p mod p!=1
			PowerMod(temp2, g, p, p);
			if ((temp1 > 1) && (temp2 > 1))
				flag = 0;
			else
				flag = 1;
		}
		else flag = 1;
	}
	cout << "g=" << g << "\n";
	
	//生成msk
	Vec<ZZ> s;
	Vec<ZZ> h;
	s.SetLength(l);
	h.SetLength(l);
	for (i = 0; i < l; i++) {
		RandomBnd(s[i],p);
	}
	cout << "msk="<< s << "\n";

	//生成mpk
	for (i = 0; i < l; i++) {
		PowerMod(h[i], g, s[i], q);
	}
	cout << "mpk="<< h << "\n";
	
	//Encrypt
	Vec<ZZ> x;
	x.SetLength(l);
	for (i = 0; i < l; i++) {
		 RandomBnd(x[i],p);
	}
	cout << "x=" << x << "\n";    //生成message x

	RandomBnd(r,p);  //生成随机数r
	cout << "随机数r=" << r << "\n";

	PowerMod(ct0, g, r, q);
	cout << "ct0=" << ct0 << "\n";

	Vec<ZZ> ct;
	ct.SetLength(l);
	for (i = 0; i < l; i++) {
		PowerMod(temp3, h[i], r, q);
		PowerMod(temp4, g, x[i], q);
		MulMod(ct[i], temp3, temp4, q);
	}
	cout << "ct=" << ct << "\n";

	//返回密文
	//cout << "Ct=(" << ct0 <<"," << ct << ")" << "\n";

	
	//KeyDer
	Vec<ZZ> y;
	Vec<ZZ> sky;
	y.SetLength(l);
	sky.SetLength(l);
	for (i = 0; i < l; i++) {
		 RandomBnd(y[i],p);
		 MulMod(sky[i], y[i], s[i],q); 
	}
	cout << "y=" << y << "\n";
	cout << "sky=" << sky << "\n";

	for (i = 0; i < l; i++) {
		add(sum, sum, sky[i]);
	}
	cout << "sum=" << sum << "\n";
	//cout << "p=" << p << "\n";
	MulMod(temp, sum, one, q);
	cout << "sky=" << temp << "\n";

	
	//Decrcrypt
	temp5 = 1;
	Vec<ZZ> el;   //等式左侧
	el.SetLength(l);
	for (i = 0; i < l; i++) {
		PowerMod(el[i], ct[i], y[i], q);
		//MulMod(temp5, temp5, el[i], p); // x = (a*b)%n
	}
	cout << "el=" << el << "\n";
	for (i = 0; i < l; i++) {
		MulMod(mul, mul, el[i], q);
	}
	cout << "mul=" << mul << "\n";

	PowerMod(er, ct0, temp,q);
	cout << "er=" << er << "\n";
	InvMod(inver, er, q);
	cout << "inver=" << inver << "\n";
	MulMod(ans, mul, inver, q);
	cout << "解密后的内积结果为" << ans << "\n";
	system("pause");



	/*Vec<ZZ> summ;   //等式左侧
	summ.SetLength(l);
	ZZ ceshi, tempp,temppp;
	ceshi = 0;
	for (i = 0; i < l; i++) {
		MulMod(summ[i], y[i], x[i], p);
	}
	cout << "summ=" << summ << "\n";

	for (i = 0; i < l; i++) {
		add(ceshi, ceshi, summ[i]);
	}
	cout << "ceshi=" << ceshi << "\n";
	MulMod(tempp, ceshi, one, p);
	PowerMod(temppp, g, tempp, p);
	cout << "temppp=" << temppp << "\n";
	system("pause"); 
}*/
