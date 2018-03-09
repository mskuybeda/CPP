#include "poly.h"

Term divTerm(const Term &lhs, const Term &rhs){
    const auto &coeff = lhs.coefficient_ / rhs.coefficient_;
    const auto &exponent = lhs.exponent_ - rhs.exponent_;

    return Term{coeff, exponent};
}

void divide(const Polynomial &lhs, const Polynomial &rhs, Polynomial &quotient, Polynomial &remaider)
{
    const auto &lhsTerm = *lhs.poly_.begin();
    const auto &rhsTerm = *rhs.poly_.begin();

    if (rhsTerm < lhsTerm) {
        remaider = lhs;
        return;
    }

    Term div = divTerm(lhsTerm, rhsTerm);
    Polynomial result;
    result.addTerm(div.coefficient_, div.exponent_);

    quotient.addTerm(div.coefficient_, div.exponent_);

    return divide(lhs - result * rhs, rhs, quotient, remaider);
}

/******you code these 3 functions******/
int GCD(int a, int b){
    a = abs(a);
    b = abs(b);

    if (a == 0 || b == 0) return a + b;
    if (a > b) return GCD(a%b, b);
    if (a < b) return GCD(a, b%a);

    //a and b are same, so any of them can be GCD
    return a;
}
Polynomial operator/(const Polynomial& p1, const Polynomial& p2){
    Polynomial quotient, remainder;
    divide(p1, p2, quotient, remainder);

    return quotient;
}

Polynomial operator%(const Polynomial& p1, const Polynomial& p2){
    Polynomial quotient, remainder;
    divide(p1, p2, quotient, remainder);

    return remainder;
}

int abs(int v){
	return (v>=0)?v:-v;
}

Fraction::Fraction(){
	numerator_=0;
	denominator_=1;
}
Fraction::Fraction(int num,int denom){
	set(num,denom);
}
void Fraction::set(int num,int denom){
	numerator_=num;
	denominator_=denom;
	reduce();
}
void Fraction::reduce(){
	int divisor=GCD(numerator_,denominator_);
	numerator_=numerator_/divisor;
	denominator_=denominator_/divisor;
	//always store negative in numerator
	if((numerator_ < 0 && denominator_ < 0) || 
		(numerator_ > 0 && denominator_ < 0)){
		numerator_=numerator_*-1;
		denominator_=denominator_*-1;
	}
}
bool isSame(const Term& a,const Term& b)
{
	bool rc=false;
	if(a.coefficient_==b.coefficient_ && a.exponent_==b.exponent_){
		rc=true;
	}
	return rc;
}
Fraction operator+(const Fraction& f1, const Fraction& f2){
	return Fraction(f1.numerator_*f2.denominator_+f2.numerator_*f1.denominator_, f1.denominator_*f2.denominator_);
	
}
Fraction operator-(const Fraction& f1, const Fraction& f2){
	return Fraction(f1.numerator_*f2.denominator_-f2.numerator_*f1.denominator_, f1.denominator_*f2.denominator_);

}
Fraction operator*(const Fraction& f1, const Fraction& f2){
	return Fraction(f1.numerator_*f2.numerator_, f1.denominator_*f2.denominator_);
}
Fraction operator/(const Fraction& f1, const Fraction& f2){
	return Fraction(f1.numerator_*f2.denominator_, f1.denominator_*f2.numerator_);
}
ostream& operator << (ostream& os, const Fraction& data){
	if(abs(data.numerator_) < abs(data.denominator_)){
		if(data.numerator_!=0){
			os << data.numerator_ << "/"  << data.denominator_;
		}
		else{
			os << "0";
		}
	}
	else{
		os << data.numerator_/data.denominator_;
		if(data.numerator_ % data.denominator_){
			os << " " << abs(data.numerator_%data.denominator_) << "/";
			os << data.denominator_;
		}
	}
	return os;
}

const Fraction& Fraction::operator+=(const Fraction& rhs){
	*this=*this+rhs;
	return *this;
}
const Fraction& Fraction::operator-=(const Fraction& rhs){
	*this=*this-rhs;
	return *this;
}
const Fraction& Fraction::operator*=(const Fraction& rhs){
	*this = *this * rhs;
	return *this;
}
const Fraction& Fraction::operator/=(const Fraction& rhs){
	*this = *this / rhs;
	return *this;
}
bool operator==(const Fraction& f1, const Fraction& f2){
	bool rc=true;
	if((f1.numerator_!=f2.numerator_) || (f1.denominator_!=f2.denominator_)){
		rc=false;
	}
	return rc;
}
bool operator!=(const Fraction& f1, const Fraction& f2){
	bool rc=true;
	if((f1.numerator_==f2.numerator_)  && (f1.denominator_==f2.denominator_)){
		rc=false;
	}
	return rc;
}
Polynomial::Polynomial(int data[][3],int numTerms){
	for(int i=0;i<numTerms;i++){
		poly_.insert(Term(data[i][0],data[i][1],data[i][2]));
	}
}
Polynomial::Polynomial(const Polynomial& poly){
	poly_=poly.poly_;
}
const Polynomial& Polynomial::operator=(const Polynomial& poly){
	poly_=poly.poly_;
	return *this;
}
void Polynomial::clear(){
	poly_.erase(poly_.begin(),poly_.end());
}
Polynomial::~Polynomial(){
}
void Polynomial::addTerm(const Fraction& coefficient, int degree){
	Term t(coefficient,degree);
	//search for a term with the same exponent
	auto it = poly_.search(t);
	if(it==poly_.end()){
		poly_.insert(t);
	}
	else{
		(*it).coefficient_+=coefficient;
	}

}

//Complexity is O(n^2), because in the worst case t.exponent_ will be same as poly_.size()
Fraction Polynomial::solve(const Fraction& x) const{
	Fraction rc;
	auto it=poly_.begin();
	while(it!=poly_.end()){
		Term t=*it;
		//find x^exp
		Fraction curr(1,1);
		for(int i=0;i<t.exponent_;i++){
			curr=curr*x;
		}
		rc+=t.coefficient_*curr;
		it++;
	}
	return rc;
}
const Polynomial& Polynomial::operator+=(const Polynomial& rhs){
	auto it1=poly_.begin();
	auto it2=rhs.poly_.begin();
	while(it1!=poly_.end() && it2!=rhs.poly_.end()){
		if(*it1 == *it2){
			(*it1).coefficient_ += (*it2).coefficient_;
			if((*it1).coefficient_==Fraction(0,1)){
				it1=poly_.erase(it1);
			}
			else{
				it1++;
			}
			it2++;
		}
		else if(*it1 < *it2){
			it1++;			
		}
		else{
			addTerm((*it2).coefficient_ ,(*it2).exponent_);
			it2++;
		}
	}
	while(it2!=rhs.poly_.end()){
		addTerm((*it2).coefficient_ ,(*it2).exponent_);
		it2++;
	}

	return *this;
}
const Polynomial& Polynomial::operator-=(const Polynomial& rhs){
	auto it1=poly_.begin();
	auto it2=rhs.poly_.begin();
	while(it1!=poly_.end() && it2!=rhs.poly_.end()){
		if(*it1 == *it2){
			(*it1).coefficient_ -= (*it2).coefficient_;
			if((*it1).coefficient_==Fraction(0,1)){
				it1=poly_.erase(it1);
			}
			else{
				it1++;
			}
			it2++;
		}
		else if(*it1 < *it2){
			it1++;			
		}
		else{
			addTerm(Fraction(-1,1)* (*it2).coefficient_ ,(*it2).exponent_);
			it2++;
		}
	}
	while(it2!=rhs.poly_.end()){
		addTerm(Fraction(-1,1)*(*it2).coefficient_ ,(*it2).exponent_);
		it2++;
	}
	return *this;

}
const Polynomial& Polynomial::operator*=(const Polynomial& rhs){
	auto it2=rhs.poly_.begin();
	Polynomial temp;
	while(it2!=rhs.poly_.end()){
		auto it1=poly_.begin();
		while(it1!=poly_.end()){
			temp.addTerm((*it1).coefficient_*(*it2).coefficient_,(*it1).exponent_+(*it2).exponent_);
			it1++;
		}
		it2++;
	}
	*this = temp;
	return *this;
}



Polynomial operator+(const Polynomial& p1, const Polynomial& p2){
	Polynomial rc=p1;
	rc+=p2;
	return rc;
}
Polynomial operator-(const Polynomial& p1, const Polynomial& p2){
	Polynomial rc=p1;
	rc-=p2;
	return rc;
}
Polynomial operator*(const Polynomial& p1, const Polynomial& p2){
	Polynomial rc=p1;
	rc*=p2;
	return rc;
}

bool operator==(const Polynomial& p1, const Polynomial& p2){
	auto it1=p1.poly_.begin();
	auto it2=p2.poly_.begin();
	bool rc=true;
	while(rc && it1 != p1.poly_.end() && it2!=p2.poly_.end()){
		if(!isSame(*it1,*it2)){
			rc=false;
		}
		it1++;
		it2++;
	}
	if(rc){
		return (it1==p1.poly_.end() && it2==p2.poly_.end());
	}
	else{
		return rc;
	}
}
ostream& operator<<(ostream& os, const Polynomial& dat){
	auto it=dat.poly_.begin();
	if(it!=dat.poly_.end()){
		os << (*it).coefficient_ << " x^" << (*it).exponent_;
		it++;
		while(it != dat.poly_.end()){
			os << " + ";
			os << (*it).coefficient_;
			if((*it).exponent_ > 1) {
				os << " x^" << (*it).exponent_;
			}
			else if ((*it).exponent_==1){
				os << " x";
			}
			it++;
		}
	}
	else{
		os << "0" << endl;
	}
	return os;
}
