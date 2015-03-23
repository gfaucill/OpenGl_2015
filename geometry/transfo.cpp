#include "transfo.hpp"

#define OUT_W 6

using namespace std;

ostream & operator << (ostream & out, const Matrix & m) {
	out << '{' 
	<< setw(OUT_W) << setprecision(4) << m.mat[0]  << ' '
	<< setw(OUT_W) << setprecision(4) << m.mat[1]  << ' '
	<< setw(OUT_W) << setprecision(4) << m.mat[2]  << ' '
	<< setw(OUT_W) << setprecision(4) << m.mat[3]  << endl;
	out << ' ' 
	<< setw(OUT_W) << setprecision(4) << m.mat[4]  << ' '
	<< setw(OUT_W) << setprecision(4) << m.mat[5]  << ' '
	<< setw(OUT_W) << setprecision(4) << m.mat[6]  << ' '
	<< setw(OUT_W) << setprecision(4) << m.mat[7]  << endl;
	out << ' ' 
	<< setw(OUT_W) << setprecision(4) << m.mat[8]  << ' '
	<< setw(OUT_W) << setprecision(4) << m.mat[9]  << ' '
	<< setw(OUT_W) << setprecision(4) << m.mat[10] << ' '
	<< setw(OUT_W) << setprecision(4) << m.mat[11] << endl;
	out << ' ' 
	<< setw(OUT_W) << setprecision(4) << m.mat[12] << ' '
	<< setw(OUT_W) << setprecision(4) << m.mat[13] << ' '
	<< setw(OUT_W) << setprecision(4) << m.mat[14] << ' '
	<< setw(OUT_W) << setprecision(4) << m.mat[15] << '}';
	return out;
}

void transfo_test() {
	RotationMatrix mr(X, 1.5707963267948966); // π/2
	TranslationMatrix mt(2.2, 4.4, 8.8);
	ScaleMatrix ms(2.5, 2.5, 5.);
	
	Matrix mp = mr.makeInverse();
	cout << mr << endl << endl;
	cout << mp << endl << "############" << endl;

	mp = mt.makeInverse();
	cout << mt << endl << endl;
	cout << mp << endl << "############" << endl;
	
	mp = ms.makeInverse();
	cout << ms << endl << endl;
	cout << mp << endl << "############" << endl;
}
