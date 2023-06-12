/*#include <NTL/ZZ.h>
#include <Windows.h>
#include <NTL/vec_ZZ.h>
#include <NTL/mat_ZZ.h>
#include <time.h>  

using namespace std;
using namespace NTL;

void main()
{
	ZZ p, g, q, r, z, test, ct0, q1;
	ZZ er, inver, ans, flag;
	ZZ temp0, temp1, temp2, temp3, temp4;
	ZZ one, sum, mul;
	ZZ pp;
	long i, l, m, lq, ll;
	long b, b1;
	long xl;

	l = 50;
	sum = 0;
	mul = 1;
	one = 1;
	temp4 = 0;
	clock_t start0, finish0;
	clock_t start1, finish1;
	clock_t start2, finish2;
	clock_t start3, finish3;
	clock_t start4, finish4;
	double  duration;

	start0 = clock();

	cout << "********************Setup�׶�********************" << "\n";

	start1 = clock();


	//����p
	p = GenGermainPrime_ZZ(50);
	cout << "p=" << p << "\n";
	//A Germain prime is a prime p such that p' = 2*p+1 is also a prime.

	//����q
	q = 2 * p + 1;
	ll = NumBits(q);
	cout << "q=" << q << "\n";
	cout << "q�ĳ���Ϊ��" << ll << "\n";


	//����g
	flag = 1;
	RandomBnd(g, p);
	while (flag == 1) {
		//cout << "g=" << g << "\n";
		//RandomBnd(g, p);
		PowerMod(z, g, p, q);
		if ((g == 0)||(g == 1)|| (z > 1))
			RandomBnd(g, p);
		else flag = 0;
	}
	cout << "g=" << g << "\n";

	PowerMod(test, g, p, q);
	cout << "test=" << test << "\n";

	//����msk
	Vec<ZZ> s;
	s.SetLength(l);
	for (i = 0; i < l; i++) {
		RandomBnd(s[i], p);
	}
	//cout << "msk=" << s << "\n";

	//����msk����
	for (i = 0; i < l; i++) {
		b = NumBits(s[i]);
		temp4 = temp4 + b;
	}
	cout << "msk����Ϊ��" << temp4 << "\n";

	//����mpk
	Vec<ZZ> h;
	h.SetLength(l);
	for (i = 0; i < l; i++) {
		PowerMod(h[i], g, s[i], q);
	}
	//cout << "mpk=" << h << "\n";

	//����mpk����
	temp4 = 0;
	for (i = 0; i < l; i++) {
		b = NumBits(h[i]);
		temp4 = temp4 + b;
	}
	cout << "mpk����Ϊ��" << temp4 << "\n";

	finish1 = clock();
	duration = (double)(finish1 - start1) / CLOCKS_PER_SEC;
	printf("Setup����ʱ��: %f seconds\n", duration);


	cout << "********************���ܽ׶�********************" << "\n";

	start2 = clock();

	//Encrypt

	pp = GenPrime_ZZ(20);
	//����message x
	Vec<ZZ> x;
	x.SetLength(l);
	temp4 = 0;
	for (i = 0; i < l; i++) {
		RandomBnd(x[i], pp);
		b = NumBits(x[i]);
		temp4 = temp4 + b;
	}
	cout << "����x�ĳ���Ϊ��" << temp4 << "\n"; 
	//cout << "x=" << x << "\n";    

	RandomBnd(r, p);  //���������r
	cout << "�����r=" << r << "\n";

	PowerMod(ct0, g, r, q);
	//cout << "ct0=" << ct0 << "\n";

	b1 = NumBits(ct0);
	//cout << "b1=" << b1 << "\n";
	Vec<ZZ> ct;
	ct.SetLength(l);
	for (i = 0; i < l; i++) {
		PowerMod(temp0, h[i], r, q);
		PowerMod(temp1, g, x[i], q);
		MulMod(ct[i], temp0, temp1, q);
	}
	//cout << "ct=" << ct << "\n";

	//��������ct����
	temp4 = 0;
	for (i = 0; i < l; i++) {
		b = NumBits(ct[i]);
		temp4 = temp4 + b;
	}
	//cout << "temp4=" << temp4 << "\n";
	//��������
	//cout << "Ct=(" << ct0 <<"," << ct << ")" << "\n";
	cout << "����ct����Ϊ��" << (temp4 + b1) << "\n";

	finish2 = clock();
	duration = (double)(finish2 - start2) / CLOCKS_PER_SEC;
	printf("���ܽ׶�����ʱ��: %f seconds\n", duration);


	cout << "******************��Կ���ɽ׶�******************" << "\n";

	start3 = clock();

	//KeyDer
	Vec<ZZ> y;
	Vec<ZZ> sky;
	y.SetLength(l);
	sky.SetLength(l);
	for (i = 0; i < l; i++) {
		RandomBnd(y[i], pp);
		MulMod(sky[i], y[i], s[i], p);
	}
	//cout << "y=" << y << "\n";
	//cout << "sky=" << sky << "\n";

	for (i = 0; i < l; i++) {
		add(sum, sum, sky[i]);
	}
	cout << "sum=" << sum << "\n";
	MulMod(temp2, sum, one, p);
	cout << "sky=" << temp2 << "\n";

	//����sky����
	b = NumBits(temp2);
	cout << "sky����Ϊ��" << b << "\n";

	finish3 = clock();
	duration = (double)(finish3 - start3) / CLOCKS_PER_SEC;
	printf("��Կ���ɽ׶�����ʱ��: %f seconds\n", duration);

	cout << "********************���ܽ׶�********************" << "\n";

	start4 = clock();

	//Decrcrypt
	temp3 = 1;
	Vec<ZZ> el;   //��ʽ���
	el.SetLength(l);
	for (i = 0; i < l; i++) {
		PowerMod(el[i], ct[i], y[i], q);
	}
	//cout << "el=" << el << "\n";
	for (i = 0; i < l; i++) {
		MulMod(mul, mul, el[i], q);
	}
	cout << "mul=" << mul << "\n";

	PowerMod(er, ct0, temp2, q);
	cout << "er=" << er << "\n";
	InvMod(inver, er, q);
	cout << "inver=" << inver << "\n";
	MulMod(ans, mul, inver, q);
	cout << "���ܺ���Ϊ" << ans << "\n";

	cout << "\n";
	cout << "\n";

	cout << "p=" << p << "\n";
	cout << "q=" << q << "\n";
	cout << "g=" << g << "\n";
	cout << "c=" << ans << "\n";


	finish4 = clock();
	duration = (double)(finish4 - start4) / CLOCKS_PER_SEC;
	printf("���ܽ׶�����ʱ��: %f seconds\n", duration);

	finish0 = clock();
	duration = (double)(finish0 - start0) / CLOCKS_PER_SEC;
	printf("�㷨������ʱ��: %f seconds\n", duration);


	cout << "********************��֤�׶�********************"<< "\n";
	Vec<ZZ> tmp;
	tmp.SetLength(l);
	Vec<ZZ> xy;
	xy.SetLength(l);
	sum = 0;
	for (i = 0; i < l; i++) {
		MulMod(xy[i], x[i], y[i], p);
	}
	for (i = 0; i < l; i++) {
		add(sum, sum, xy[i]);
	}
	MulMod(sum, sum, one, p);
	cout << "<x,y>=" << sum << "\n";
	xl = NumBits(sum);
	cout << "�ڻ�����Ϊ��" << xl << "\n";



	system("pause");
}*/