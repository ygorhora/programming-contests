#include <stdio.h>
#include <utility>
#include <math.h>
#include <limits>
#include <vector>
#include <utility>
#include <algorithm>

#define PI 3.14159265

using namespace std;

typedef pair<int,int> pii;
typedef pair<double,double> pdd;
typedef unsigned long long ull;
typedef long double ld;

double angleBetween(pii p0, pii p1){
	double cosAlpha = (p0.first * p1.first + p0.second * p1.second)/ (hypot(p0.first, p0.second) * hypot(p1.first,p1.second));
	return acos(cosAlpha);
}

int main(){
	int tc;
	vector<pii> vect;
	int x, y;
	
	scanf("%d", &tc);
	while(tc--){
		int n;
		
		scanf("%d", &n);
		
		for(int i = 0; i < n; ++i){
			scanf("%d %d", &x, &y);
			vect.push_back(make_pair(x,y));
		}
		
		sort(vect.rbegin(), vect.rend());
		
		vector<pii> ans;
		for(int i = 1; (unsigned) i < vect.size(); i++){
			//printf("%d, %d\n", ans[ans.size()-1].second, vect[i].second);
			if(ans.size() == 0 || ans[ans.size()-1].second < vect[i].second){
				ans.push_back(vect[i-1]); // grouped idx's
				ans.push_back(vect[i]);
			}
		}
		/*
		for(int i = 0; (unsigned) i < ans.size(); ++i){
			printf("x: %d, y: %d\n", ans[i].first, ans[i].second);
		}
		puts("");*/
		
		double sum = hypot(ans[1].first-ans[0].first,ans[1].second-ans[0].second);
		for(int i = 3; (unsigned) i < ans.size(); i+=2){
			pii v1 = make_pair(ans[i].first-ans[i-1].first,ans[i].second-ans[i-1].second);
			pii v2 = make_pair(ans[i].first-ans[i-2].first,ans[i].second-ans[i-2].second);
			pii v3 = make_pair(-1,0);
			
			double angleOne = angleBetween(v1,v2);
			double angleTwo = angleBetween(v2,v3);
			//printf("angleOne: %f, angleTwo: %f\n", angleOne, angleTwo);
			double normV2 = hypot(v2.first, v2.second);
			
			sum += (normV2 * sin(angleTwo)) / (sin(PI-angleOne-angleTwo));
		}
		
		printf("%.2f\n",sum);
		
		vect.clear();
	}
}