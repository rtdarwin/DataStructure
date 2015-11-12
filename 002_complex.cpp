#include<iostream>
#include"002_complex.h"

//Return type is Complex&
Complex& Complex::operator+= ( const Complex &r ){
	real_part += r.real_part;
	imag_part += r.imag_part;
	return *this;
}

Complex& Complex::operator-= ( const Complex &r ){
	real_part -= r.real_part;
	imag_part -= r.imag_part;
	return *this;
}

Complex& Complex::operator*= ( const Complex &r ){
	real_part = real_part * real_part - imag_part * imag_part;
	imag_part = real_part * r.imag_part + imag_part * r.real_part;
	return *this;
}

Complex& Complex::operator/= ( const Complex &r ){
	double delta = r.real_part*r.real_part - r.imag_part*r.imag_part;
	(*this) *= Complex ( r.real_part/ delta , r.imag_part/ delta );
	return *this;
}

bool operator== ( const Complex &l, const Complex &r ){
	return ( l.real_part == r.real_part && l.imag_part == r.imag_part );
}

bool operator!= ( const Complex &l, const Complex &r ){
	return !( l == r );
}

Complex operator+ ( const Complex &l, const Complex &r ){
	//GCC error: can not call constructor 'Complex::Complex' directly
	//c = Complex::Complex( *, * ); is incorrect
	//but the following is correct
	Complex result = Complex( l.real_part + r.real_part , l.imag_part + r.imag_part );
	return result;
}

Complex operator- ( const Complex &l, const Complex &r ){
	return ( Complex( l.real_part - r.real_part, l.imag_part + r.imag_part ) );
}

Complex operator* ( const Complex &l, const Complex &r ){
	double real_part = r.real_part * l.real_part - r.imag_part * l.imag_part;
	double imag_part = r.real_part * l.imag_part + l.real_part * r.imag_part; 
	return ( Complex( real_part, imag_part ) );
}

Complex operator/ ( const Complex &l, const Complex &r ){
	double delta = r.real_part*r.real_part - r.imag_part*r.imag_part;
	Complex result = l * Complex ( r.real_part/ delta , r.imag_part/ delta );
	return result;
}

std::ostream& operator<< ( std::ostream& os, const Complex &r ){
	os<< r.real_part << "+" << r.imag_part << "i" ;
	return os;
}

std::istream& operator>> ( std::istream& is, Complex &r ){
	if( !is ){
		is >> r.real_part >> r.imag_part;
	}else{
		//input failed: reset object to default state
		r = Complex();
	}

	return is;
}
