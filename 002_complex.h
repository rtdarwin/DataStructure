#ifndef COMPLEX_H_
#define COMPLEX_H_

class Complex{
public:
	Complex():real_part(0.0), imag_part(0.0) {}
	Complex( double r ):real_part(r), imag_part(0.0) {}
	Complex( double r, double i ):real_part(r), imag_part(i) {}
	//Use default copy constructor
	//Use default destructor
	double real_part;
	double imag_part;

public:
	//Use the syned assgin operation function
	Complex& operator+= ( const Complex &r_operand );
	Complex& operator-= ( const Complex &r_operand );
	Complex& operator*= ( const Complex &r_operand );
	Complex& operator/= ( const Complex &r_operand );
	friend bool operator== ( const Complex &l_operand, const Complex &r_operand );
	friend bool operator!= ( const Complex &l_operand, const Complex &r_operand );
	friend Complex operator+ ( const Complex &l_operand, const Complex &r_operand );
	friend Complex operator- ( const Complex &l_operand, const Complex &r_operand );
	friend Complex operator* ( const Complex &l_operand, const Complex &r_operand );
	friend Complex operator/ ( const Complex &l_operand, const Complex &r_operand );
	friend std::ostream& operator<< ( std::ostream& os, const Complex &r_operand );
	friend std::istream& operator>> ( std::istream& is, Complex &r_operand );

};

#endif
