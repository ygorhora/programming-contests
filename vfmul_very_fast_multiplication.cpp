/* Modified basic fft from http://e-maxx.ru/algo/fft_multiply */
#include<vector>
#include<algorithm>
#include<complex>
#include<cmath>
#include<cstring>
#include<stdio.h>

using namespace std;

typedef vector<long long> vll;
typedef vector<int> vi;
typedef complex<long double> base;
const double PI = 2 * acos(0.0);

void fft(vector<base> &a, bool inv){
	int n = (int) a.size();
	
	if(n == 1) return;
	
	vector<base> a0(n/2), a1(n/2);
	for(int i = 0, j = 0; i < n; i+=2, j++){
		a0[j] = a[i];
		a1[j] = a[i+1];
	}
	
	fft(a0, inv);
	fft(a1, inv);
	
	long double ang = 2 * PI/n * (inv? -1 : 1);
	base w(1), wn(cos(ang), sin(ang));
	for(int i = 0; i < n/2; i++){
		a[i] = a0[i] + w * a1[i];
		a[i+n/2] = a0[i] - w * a1[i];
		if(inv) a[i] /= 2, a[i+n/2] /= 2;
		w *= wn;
	}
}

// a[0] + a[1]*x + a[2]*x^2 + a[3]*x^3 + ...
// res[0] + res[1]*x + res[2]*x^2 + res[3]*x^3 +...
// resize == 1 -> last not zero
// resize == 2 -> infer based on a and b degree
void multiply(const vll &a, const vll &b, vll &res, int resize){
	vector<base> fa(a.rbegin(), a.rend()), fb(b.rbegin(), b.rend());
	int n = 1;
	while(n < max(a.size(), b.size())) n <<= 1;
	n <<= 1;
	fa.resize(n), fb.resize(n);
	
	fft(fa,false), fft(fb,false);
	for(int i = 0; i < n; i++)
		fa[i] *= fb[i];
	fft(fa, true); // back to semi-real;
	
	res.resize(n);
	for(int i = 0; i < n; i++){
		res[i] = (long long)(fa[i].real() + 0.5);
	}
	
	if(resize == 1){
		int i;
		for(i = res.size()-1; i > 0; i--){
			if(res[i] != 0)
				break;
		}
		res.resize(i+1);
	} else if (resize == 2){
		res.resize(a.size()+b.size()-1);
	}
}

const int DIGITS_PER_COEF = 4;
const long long BASE = 10000;

vll createPolynomial(char s[]){
	int n = strlen(s);
	
	int nBlocks = n / DIGITS_PER_COEF;
	int rest = n % DIGITS_PER_COEF;
	if(rest != 0) nBlocks++;
	
	int lastIdxBlock = rest;
	if (lastIdxBlock == 0) lastIdxBlock += DIGITS_PER_COEF;
	
	vll polynomial(nBlocks);
	int firstIdxBlock = 0;
	for(int i = 0; i < nBlocks; ++i){
		int number = 0;
		// creating number of the block
		for(int j = firstIdxBlock; j < lastIdxBlock; j++){
			number *= 10;
			number += s[j] - '0';
		}
		firstIdxBlock = lastIdxBlock;
		lastIdxBlock += DIGITS_PER_COEF;
		polynomial[i] = number;
	}
	
	return polynomial;
}

char s1[300100], s2[300100];

int main(){
	int n;
	scanf("%d ", &n);
	
	while(n--){
		
		vll a, b;
		
		scanf("%s %s", s1, s2);
		
		a = createPolynomial(s1);
		b = createPolynomial(s2);
		
		/*
		for(unsigned i = 0; i < a.size(); i++){
			printf("%lld ", a[i]);
		}
		printf("\t");
		
		for(unsigned i = 0; i < b.size(); i++){
			printf("%lld ", b[i]);
		}
		puts("");*/
		
		vll ans;
		multiply(a,b,ans,2);
		
		/*
		for(unsigned i = 0; i < ans.size(); ++i){
			printf("%d: %lld ",i, ans[i]);
		}
		puts("\nresult:\n");*/
		
		vll res;
		long long carry = 0;
		long long positionValue;
		long long digit;
		bool allAreZero = true;
		
		for(int i = 0; i < ans.size() || carry != 0; i++){
			positionValue = (i < ans.size()? ans[i] : 0) + carry;
			digit = positionValue%BASE;
			res.push_back(digit);
			if(digit != 0){
				allAreZero = false;
			}
			carry = positionValue/BASE;
			//printf("digit: %lld, carry: %lld\n", digit, carry);
		}

		if(allAreZero){
			printf("0");
		} else {
			for(int i = res.size() - 1; i >= 0 ; i--){
				if(i != res.size() - 1)
					printf("%.*lld", DIGITS_PER_COEF, res[i]);
				else
					printf("%lld", res[i]);
			}
		}
		puts("");
	}
}

