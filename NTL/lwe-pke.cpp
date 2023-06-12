/*#include <NTL/matrix.h>
#include <NTL/ZZ.h>
#include <Windows.h>
#include <NTL/vec_ZZ.h>
#include <NTL/mat_ZZ.h>
#include <time.h>

using namespace std;
using namespace NTL;

void main()
{
    //long z;

    long i, j, c, k;
    long m, n, a, l;
    long temp;
    long lt, ll;
    long lta, ltb, ls, lc1, lc2;//计算参数的长度

    ZZ q, p, r;
	ZZ flag;
    ZZ one, two;
    ZZ y, y1, ty;

	flag = 1;
    a = 2;
    r = 3;
    l = 3;
    one = 1;
    two = 2;
    clock_t start0, finish0;
    clock_t start1, finish1;
    clock_t start2, finish2;
    clock_t start3, finish3;
    double  duration;

    n = RandomLen_long(l);
	RandomBnd(temp, 2*n*n);
	l = NumBits(temp);
	q = GenPrime_ZZ(l);

	while (flag == 1) {
		if ((q > (n * n)) && (q < (2 * n * n)))
		{
			flag = 0;
		}
		else
		{
			q = GenPrime_ZZ((l));
		}
	}
	//m = (1 + a) * (n + 1) * log(q);
    m = 4;
	k = log(q) / log(r);
    
    

    cout << "n=" << n << "\n";
	cout << "q=" << q << "\n";
	cout << "a=" << a << "\n";
	cout << "m=" << m << "\n";
	cout << "log q=" << log(q) << "\n";
    cout << "log r=" << log(r) << "\n";
	cout << "k=" << k << "\n";

    start0 = clock();

    //Setup阶段
    cout << "**************************Setup阶段*************************" << "\n";
    
    start1 = clock();

    Vec<ZZ> X;    //噪声分布
    X.SetLength(32);

    ZZ a1, a2, a3, a4, a5, a6, a7, a8;
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
        a1 = (f[i] >> 7) & 0x1;
        a2 = (f[i] >> 6) & 0x1;
        a3 = (f[i] >> 5) & 0x1;
        a4 = (f[i] >> 4) & 0x1;
        a5 = (f[i] >> 3) & 0x1;
        a6 = (f[i] >> 2) & 0x1;
        a7 = (f[i] >> 1) & 0x1;
        a8 = (f[i] >> 0) & 0x1;
        X[i] = (a1 - a2) * (a3 - a4) * (a5 - a6) * (a7 - a8);
        if (X[i] < 0) {
            X[i] *= -1;
        }
    }
    cout << "X=" << X << "\n";

    //A采样 Zq:m*n
    Mat<ZZ> A;
    A.SetDims(m, n);
    for (i = 1; i <= m; i++) {
        for (j = 1; j <= n; j++) {
            RandomBnd(A(i,j), q);
        }
    }
    cout << "A=" << A << "\n";
    lt = 0;
    lta = 0;
    for (i = 1; i <= m; i++) {
        for (j = 1; j <= n; j++) {
            ll = NumBits(A(i, j));
            lta = lta + ll;
        }
    }
    cout << "A长度为：" << lta << "\n";

    //s采样 
    Mat<ZZ> s;
    s.SetDims(n, k+1);
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= k+1; j++) {
            RandomBnd(s(i, j), q);
        }
    }
    cout << "s=" << s << "\n";

    ls = 0;
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= k + 1; j++) {
            ll = NumBits(s(i, j));
            ls = ls + ll;
        }
    }
    cout << "私钥sk的长度为：" << ls << "\n";

    Mat<ZZ> B;
    B.SetDims(m, k+1);

    //计算B=A*s  m*(k+1)
    ZZ sum, tmp;
    for (i = 1; i <= m; i++) {
        for (j = 1; j <= k+1; j++) {
             sum = 0;
             for (c = 1; c <= n; c++) {
                  MulMod(tmp, A(i, c), s(c, j), q);
                  AddMod(sum, sum, tmp, q);
             }
             B(i, j) = sum;
        }
    }
    cout << "B=" << B << "\n";

    //e取样 
    Mat<ZZ> e;
    e.SetDims(m, k+1);
    for (i = 1; i <= m; i++) {
        for (j = 1; j <= k + 1; j++) {
            c = RandomBnd(32);
            e(i, j) = X[c];
        }
    }
    cout << "e=" << e << "\n";


    //计算 b = A * s + e
    Mat<ZZ> b;
    b.SetDims(m, k + 1);
    for (i = 1; i <= m; i++) {
        for (j = 1; j <= k + 1; j++) {
             AddMod(b(i, j), B(i, j), e(i, j), q);
        }

    }
    cout << "b=" << b << "\n";

    ltb = 0;
    for (i = 1; i <= m; i++) {
        for (j = 1; j <= (k+1); j++) {
            ll = NumBits(b(i, j));
            ltb = ltb + ll;
        }
    }
    cout << "b长度为：" << ltb << "\n";

    lt = lta + ltb;
    cout << "公钥pk的长度为：" << lt << "\n";

    finish1 = clock();
    duration = (double)(finish1 - start1) / CLOCKS_PER_SEC;
    printf("Setup运行时间: %f seconds\n", duration);
    

    cout << "**************************Encrypt阶段*************************" << "\n";

    start2 = clock();

    //选择 p：1 < p < q;
    RandomBnd(p, q);
    l = NumBits(p);
    p = GenPrime_ZZ(l);
    cout << "p=" << p << "\n";


    //选择 message x
    ZZ mx;
    RandomBnd(mx, p);
    cout << "message x=" << mx << "\n";

    Vec<ZZ> x;  //xv
    x.SetLength(k+1);
    for (i = 0; i < k; i++) {
        RandomBnd(x[i], p);
    }
   // cout << "x=" << x << "\n";

    Vec<ZZ> et;//x*r
    et.SetLength(k + 1);
    Vec<ZZ> erv;//rv
    erv.SetLength(k + 1);
    for (i = 0; i < k; i++) {
        power(erv[i], r, i);
        et[i] = x[i] * erv[i];
    }
    sum = 0;
    for (i = 0; i < k; i++) {
        add(sum, sum, et[i]);
    }
    MulMod(sum, sum, one, q);
    SubMod(et[k], mx, sum, q);
    power(erv[k], r, k);
    ZZ inv;
    InvMod(inv, erv[k], q);
    MulMod(x[k], et[k], inv, q);
    cout << "x=" << x << "\n";

    //选择随机向量 r
    Vec<ZZ> rr;
    rr.SetLength(m);
    for (i = 0; i < m; i++) {
        RandomBnd(rr[i], two);
    }
    cout << "rr=" << rr << "\n";

    //生成A的转置矩阵 ：AT
    Mat<ZZ> AT;
    AT.SetDims(n, m);
    for (i = 1; i <= m; i++){
        for (j = 1; j <= n; j++) {
            AT(j,i)=A(i,j);
        }
    }
    cout << "AT=" << AT << "\n";


    //计算 ct0 = AT * r
    Vec<ZZ> ct0;
    ct0.SetLength(n);
    for (i = 1; i <= n; i++) {
        sum = 0;
        for (j = 1; j <= m; j++) {
            MulMod(tmp, AT(i, j), rr[j-1], q);
            AddMod(sum, sum, tmp, q);
        }
        ct0[i - 1] = sum;
    }
    cout << "ct0=" << ct0 << "\n";

    lt = 0;
    lc1 = 0;
    for (i = 0; i < n; i++) {
        ll = NumBits(ct0[i]);
        lc1 = lc1 + ll;
    }

    //计算ctv的左部分 ctl
    Vec<ZZ> ctl;
    ctl.SetLength(k + 1);
    Vec<ZZ> temp1;
    temp1.SetLength(m);

    for (i = 0; i <= k; i++) {
        sum = 0;
        for (j = 0; j < m; j++) {
            MulMod(temp1[j], rr[j], b(j+1,i+1), q);
        }
        for (c = 0; c < m; c++) {
            add(sum, sum, temp1[c]);
        }
        MulMod(ctl[i], sum, one, q);
    }

    //计算ctv的右部分 ctr
    Vec<ZZ> ctr;
    ctr.SetLength(k + 1);
    ZZ tempc, rv, tempp;
    for (i = 0; i <= k; i++) {
        tempp = q / p;
        PowerMod(rv, r, i, q);
        MulMod(tempc, tempp, rv, q);
        MulMod(ctr[i], tempc, x[i], q);
    }


    //计算 ct = ctl + ctr
    Vec<ZZ> ct;
    ct.SetLength(k + 1);
    for (i = 0; i <= k; i++) {
        AddMod(ct[i], ctl[i], ctr[i], q);
    }
    cout << "ct=" << ct << "\n";

    lc2 = 0;
    for (i = 0; i <= k; i++) {
        ll = NumBits(ct[i]);
        lc2 = lc2 + ll;
    }

    lt = lc1 + lc2;
    cout << "密文ct的长度为：" << lt << "\n";



    finish2 = clock();
    duration = (double)(finish2 - start2) / CLOCKS_PER_SEC;
    printf("加密阶段运行时间: %f seconds\n", duration);

    cout << "**************************Decrypt阶段*************************" << "\n";
    
    start3 = clock();

    //计算y的右侧内积部分
    Vec<ZZ> temp2;
    temp2.SetLength(n);
    Vec<ZZ> yr;
    yr.SetLength(k + 1);
    for (i = 0; i <= k; i++) {
        sum = 0;
        for (j = 0; j < n; j++) {
            MulMod(temp2[j], ct0[j], s(j + 1, i + 1), q);
        }
        for (c = 0; c < n; c++) {
            add(sum, sum, temp2[c]);
        }
        MulMod(yr[i], sum, one, q);
    }

    //计算右侧求和部分
    Vec<ZZ> temp3;
    temp3.SetLength(k + 1);
    for (i = 0; i <= k; i++) {
        SubMod(temp3[i], ct[i], yr[i], q);
    }
    sum = 0;
    for (i = 0; i <= k; i++) {
        add(sum, sum, temp3[i]);
    }
    MulMod(sum, sum, one, q);

    cout << "sum=" << sum << "\n";

    //y的最后计算步骤：y = (p * sum) / q; y1 = p * sum
    y1 = p * sum;
    cout << "y1=" << y1 << "\n";
    div(ty, q, two);
    add(ty, ty, y1);
    y = y1 / q;
    cout << "y=" << y << "\n";

    finish3 = clock();
    duration = (double)(finish3 - start3) / CLOCKS_PER_SEC;
    printf("解密阶段运行时间: %f seconds\n", duration);

    finish0 = clock();
    duration = (double)(finish0 - start0) / CLOCKS_PER_SEC;
    printf("算法总运行时间: %f seconds\n", duration);

	system("pause");
}*/