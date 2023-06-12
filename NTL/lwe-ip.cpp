/*#include <NTL/matrix.h>
#include <NTL/ZZ.h>
#include <Windows.h>
#include <NTL/vec_ZZ.h>
#include <NTL/mat_ZZ.h>
#include <time.h>
#include <stdio.h>  
#include <stdlib.h> 
#include<fstream>

using namespace std;
using namespace NTL;

void main() {
	long m, n, l;
	long i, j, c, t;//t求长度
	ZZ p, q;
	ZZ sum, tmp;
	ZZ one, two;
	ZZ ee, nj, ll, ls;
	ofstream fout;
	clock_t start0, finish0;
	clock_t start1, finish1;
	clock_t start2, finish2;
	clock_t start3, finish3;
	clock_t start4, finish4;
	double  duration;
	one = 1; two = 2;
	n = 950;
	l = 50;
	ee = 20;
	t = 0; ll = 0; ls = 0;
	q = GenPrime_ZZ(30);
	p = GenPrime_ZZ(10);
	m = n * (log(q));
	cout << "p=" << p << "\n";
	cout << "q=" << q << "\n";
	cout << "m=n * log(q)=" << m << "\n";

	Vec<ZZ> X;    //噪声分布
	X.SetLength(32);

	ZZ a1, a2, a3, a4, a5, a6, a7, a8;
	ZZ zx, d;
	ZZ f[32];  //SHA256  种子12345
	//59 94 47 1A BB 01 11 2A
	//FC C1 81 59 F6 CC 74 B4
	//F5 11 B9 98 06 DA 59 B3
	//CA F5 A9 C1 73 CA CF C5
	f[0] = 0x59; f[1] = 0x94; f[2] = 0x47; f[3] = 0x1A; f[4] = 0xBB; f[5] = 0x01; f[6] = 0x11; f[7] = 0x2A;
	f[8] = 0xFC; f[9] = 0xC1; f[10] = 0x81; f[11] = 0x59; f[12] = 0xF6; f[13] = 0xCC; f[14] = 0x74; f[15] = 0xB4;
	f[16] = 0xF5; f[17] = 0x11; f[18] = 0xB9; f[19] = 0x98; f[20] = 0x06; f[21] = 0xDA; f[22] = 0x59; f[23] = 0xB3;
	f[24] = 0xCA; f[25] = 0xF5; f[26] = 0xA9; f[27] = 0xC1; f[28] = 0x73; f[29] = 0xCA; f[30] = 0xCF; f[31] = 0xC5;

	for (i = 0; i < 32; i++) {
		a1 = (f[i] >> 7) & 0x14;
		a2 = (f[i] >> 6) & 0x14;
		a3 = (f[i] >> 5) & 0x14;
		a4 = (f[i] >> 4) & 0x14;
		a5 = (f[i] >> 3) & 0x14;
		a6 = (f[i] >> 2) & 0x14;
		a7 = (f[i] >> 1) & 0x14;
		a8 = (f[i] >> 0) & 0x14;
		X[i] = (a1 - a2) * (a3 - a4) * (a5 - a6) * (a7 - a8);
		//if (X[i] < 0) {
		//	X[i] *= -1;
		//}
		MulMod(X[i], X[i], one, ee);
	}
	//cout << "X=" << X << "\n";

	start0 = clock();

	cout << "**********************Setup阶段*********************" << "\n";

	start1 = clock();

	//A采样 Zq:m*n
	Mat<ZZ> A;
	A.SetDims(m, n);
	for (i = 1; i <= m; i++) {
		for (j = 1; j <= n; j++) {
			RandomBnd(A(i, j), q);
			t = NumBits(A(i, j));
			ll = ll + t;
		}
	}
	cout << "矩阵A选取完成。" << "\n";
	cout << "A的长度为：" << ll << "\n";

	//采样y
	Vec<ZZ> y;
	y.SetLength(l);
	for (i = 0; i < l; i++) {
		RandomBnd(y[i], p);
	}
	//cout << "y=" << y << "\n";
	cout << "向量y已获得。" << 5 << "\n";

	//采样s
	Vec<ZZ> s;
	s.SetLength(n);
	for (i = 0; i < n; i++) {
		RandomBnd(s[i], q);
		t = NumBits(s[i]);
		ls = ls + t;
	}
	//cout << "s=" << s << "\n";
	cout << "s采样完成。" << "\n";
	cout << "msk的长度为：" << ls << "\n";

	//计算B=As
	Vec<ZZ> B;
	B.SetLength(m);
	for (i = 1; i <= m; i++) {
		sum = 0;
		for (j = 1; j <= n; j++) {
			MulMod(tmp, A(i, j), s[j - 1], q);
			AddMod(sum, sum, tmp, q);
		}
		B[i - 1] = sum;
	}

	//计算b=A*s*y+e;
	Mat<ZZ> b;
	b.SetDims(l, m);
	ZZ e;
	for (i = 1; i <= l; i++) {
		for (j = 1; j <= m; j++) {
			MulMod(b(i, j), y[i - 1], B[j - 1], q);
			c = RandomBnd(32);
			e = X[c];
			AddMod(b(i, j), b(i, j), e, q);
			t = NumBits(b(i, j));
			ll = ll + t;
		}
	}
	cout << "b计算完成。" << 5 << "\n";
	cout << "mpk的长度为：" << ll << "\n";

	finish1 = clock();
	duration = (double)(finish1 - start1) / CLOCKS_PER_SEC;
	printf("Setup运行时间: %f seconds\n", duration);


	cout << "********************加密阶段********************" << "\n";

	start2 = clock();

	// Enc阶段

	//采样x
	Vec<ZZ> x;
	x.SetLength(l);
	ll = 0;
	for (i = 0; i < l; i++) {
		RandomBnd(x[i], p);
		t = NumBits(x[i]);
		ll = ll + t;
	}
	cout << "向量x已获得。" << "\n";
	cout << "明文x的长度为：" << ll << "\n";

	//选择随机向量 r
	Vec<ZZ> r;
	r.SetLength(m);
	for (i = 0; i < m; i++) {
		RandomBnd(r[i], two);
	}
	cout << "随机向量r取样完成。" << "\n";

	//计算 ct0 = AT * r
	ll = 0;
	Vec<ZZ> ct0;
	ct0.SetLength(n);
	for (i = 1; i <= n; i++) {
		sum = 0;
		for (j = 1; j <= m; j++) {
			MulMod(tmp, A(j, i), r[j - 1], q);
			AddMod(sum, sum, tmp, q);
		}
		ct0[i - 1] = sum;
		t = NumBits(ct0[i - 1]);
		ll = ll + t;}
	Vec<ZZ> ctl;
	ctl.SetLength(l);
	for (i = 1; i <= l; i++) {
		sum = 0;
		for (j = 1; j <= m; j++) {
			MulMod(tmp, b(i, j), r[j - 1], q);
			AddMod(sum, sum, tmp, q);
		}
		ctl[i - 1] = sum;
	}

	Vec<ZZ> ctr;
	ctr.SetLength(l);
	ZZ tmpp;
	for (i = 0; i < l; i++) {
		tmpp = q / p;
		MulMod(tmp, x[i], y[i], p);
		MulMod(ctr[i], tmpp, tmp, q);
	}

	Vec<ZZ> ct;
	ct.SetLength(l);
	for (i = 0; i < l; i++) {
		AddMod(ct[i], ctl[i], ctr[i], q);
		t = NumBits(ct[i]);
		ll = ll + t;
	}
	cout << "ct=" << ct << "\n";
	cout << "密文ct的长度为：" << ll << "\n";
	finish2 = clock();
	duration = (double)(finish2 - start2) / CLOCKS_PER_SEC;
	printf("加密阶段运行时间: %f seconds\n", duration);


	cout << "******************密钥生成阶段******************" << "\n";

	start3 = clock();

	ll = 0;
	Mat<ZZ> sky;
	sky.SetDims(l, n);
	for (i = 1; i <= l; i++) {
		for (j = 1; j <= n; j++) {
			MulMod(sky(i, j), y[i - 1], s[j - 1], q);
			t = NumBits(sky(i, j));
			ll = ll + t;
		}
	}
	cout << "解密密钥sky计算完成。" << "\n";
	cout << "密钥sky的长度为：" << ll << "\n";


	finish3 = clock();
	duration = (double)(finish3 - start3) / CLOCKS_PER_SEC;
	printf("密钥生成阶段运行时间: %f seconds\n", duration);

	cout << "********************解密阶段********************" << "\n";

	start4 = clock();

	//计算求和右侧<ct0,sk> = <ATr,sk>
	Vec<ZZ> tr;
	tr.SetLength(l);
	for (i = 1; i <= l; i++) {
		sum = 0;
		for (j = 1; j <= n; j++) {
			MulMod(tmp, ct0[j - 1], sky(i, j), q);
			AddMod(sum, sum, tmp, q);
		}
		tr[i - 1] = sum;
	}
	Vec<ZZ> he;
	he.SetLength(l);
	for (i = 0; i < l; i++) {
		SubMod(he[i], ct[i], tr[i], q);//计算等式右侧部分=[q/p]*xi*yi
	}
	sum = 0;
	for (i = 0; i < l; i++) {
		AddMod(sum, sum, he[i], q);
	}
	ZZ y1, ty;
	y1 = p * sum;
	div(ty, q, two);
	add(ty, ty, y1);
	nj = ty / q;
	cout << "解密结果ans=" << nj << "\n";

	finish4 = clock();
	duration = (double)(finish4 - start4) / CLOCKS_PER_SEC;
	printf("解密阶段运行时间: %f seconds\n", duration);

	finish0 = clock();
	duration = (double)(finish0 - start0) / CLOCKS_PER_SEC;
	printf("算法总运行时间: %f seconds\n", duration);

	cout << "****************验证阶段*******************" << "\n";
	Vec<ZZ> xy;
	xy.SetLength(l);
	sum = 0;
	for (i = 0; i < l; i++) {
		MulMod(tmp, x[i], y[i], p);
		AddMod(sum, sum, tmp, p);
	}
	cout << "<x,y>=" << sum << "\n";

	p.kill();
	q.kill();
	A.kill();
	s.kill();
	y.kill();
	e.kill();
	b.kill();
	x.kill();
	r.kill();
	ct0.kill();
	ct.kill();
	sky.kill();

	system("pause");
}*/