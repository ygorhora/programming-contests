/* Modified basic fft from http://e-maxx.ru/algo/fft_multiply */
#include<vector>
#include<algorithm>
#include<complex>
#include<iostream>
#include<cmath>

using namespace std;

typedef vector<long long> vll;
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
	vector<base> fa(a.begin(), a.end()), fb(b.begin(), b.end());
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

int main(){
	int tCases;
	cin >> tCases;
	
	while(tCases--){
		int maxDegree;
		cin >> maxDegree;
		
		vll a(maxDegree+1,0);
		vll b(maxDegree+1,0);
		
		long long coef;
		for(int i = maxDegree; i >= 0; i--){
			cin >> coef;
			a[i] = coef;
		}
		
		for(int i = maxDegree; i >= 0; i--){
			cin >> coef;
			b[i] = coef;
		}
		
		vll ans;
		multiply(a,b,ans,0);
		
		bool first = true;
		for(int i = maxDegree*2; i >= 0; i--){
			if(first){
				cout << ans[i];
				first = false;
			} else {
				cout << " " << ans[i];
			}
		}
		cout << "\n";
	}
}

