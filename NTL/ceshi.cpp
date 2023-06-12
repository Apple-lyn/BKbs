/*#include <NTL/ZZ.h>
#include <Windows.h>
#include <NTL/vec_ZZ.h>
#include <NTL/mat_ZZ.h>
#include <time.h>  

using namespace std;
using namespace NTL;

void main() {
	ZZ p, o, q, g, z;
	long l, flag, lq, test;
	p = GenPrime_ZZ(54);
	o = 2 * GenPrime_ZZ(969);
	l = NumBits(o);
	q = p * o + 1;
	cout << "l=" << l << "\n";
	flag = ProbPrime(q);
	while (flag == 0) {
		o = o + 2;
		q = p * o + 1;
		flag = ProbPrime(q);
	}
	lq = NumBits(q);

	test = divide(q - 1, p);
	cout << "lq=" << lq << "\n";
	cout << "flag=" << flag << "\n";
	cout << "test=" << test << "\n";
	cout << "p=" << p << "\n";
	cout << "q=" << q << "\n";

	//Éú³Ég
	flag = 1;
	RandomBnd(g, q);
	while (flag == 1) {
		//cout << "g=" << g << "\n";
		//RandomBnd(g, p);
		PowerMod(z, g, p, q);
		if ((g == 0) || (g == 1) || (z > 1))
			RandomBnd(g, q);
		else flag = 0;
	}
	cout << "g=" << g << "\n";
	cout << "z=" << z << "\n";


	system("pause");
}*/