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
typedef long double lld;

vector< pair<double, pair<ull, pair< pdd, int> > > > vect;
// {angle -> {distance -> { {x -> y} -> height} }

bool areSame(double a, double b) {
    return fabs(a - b) < numeric_limits<double>::epsilon();
}

int calculateLastIdx(vector< pair<double, pair<ull, pair< pdd, int> > > > &v, int firstIdx){
	int lastIdx = firstIdx;
	while(areSame(v[lastIdx].first, v[firstIdx].first)){
		lastIdx++;
		if((unsigned)lastIdx >= v.size()){
			break;
		}
	}
	
	return lastIdx;
}

int main(){
	int n;
	int dataset = 1;
	while(scanf("%d", &n), n != 0){
		for(int i = 0; i < n; ++i){
			double x, y;
			int height;
			scanf("%lf %lf %d", &x, &y, &height);
			pdd coord = make_pair(x, y);
			pair<pdd, int> elem = make_pair(coord, height);
			
			ull distance = x*x + y*y;
			
			pair<ull, pair<pii, int> > data1 = make_pair(distance, elem);
			
			//double angle = calculateAngle(x, y, sqrt(distance));
			double angle = atan2 (y, x) * 180 / PI;
			pair<double, pair<ull, pair<pii, int> > > data2 = make_pair(angle, data1);
			
			vect.push_back(data2);
		}
		
		sort(vect.begin(), vect.end());
		
		/*
		for(int i = 0; (unsigned) i < vect.size(); i++){
			printf("x = %lf, y = %lf\n", vect[i].second.second.first.first,vect[i].second.second.first.second);
			printf("Angle = %lf\n", vect[i].first);
			printf("Altura = %d\n", vect[i].second.second.second);
			printf("Distance = %llu\n\n", vect[i].second.first);
		}*/
		
		
		vector< pii > ans;
		for(int firstIdx = 0; (unsigned) firstIdx < vect.size();){
			int lastIdx = calculateLastIdx(vect, firstIdx);
			//printf("First Idx: %d,Last Idx: %d\n", firstIdx, lastIdx);
			
			// first to last indexes calculated have same angle now [)
			int hHeight = vect[firstIdx].second.second.second;
			for(int i = firstIdx + 1; i < lastIdx; i++){
				int height = vect[i].second.second.second;
				if(height > hHeight){
					hHeight = height;
				} else {
					//printf("%d %d\n", firstIdx, i);
					ans.push_back(vect[i].second.second.first);
				}
			}
			
			firstIdx = lastIdx;
		}
		
		// Answer
		sort(ans.begin(), ans.end());
		
		if(ans.empty()){
			printf("Data set %d:\nAll the lights are visible.\n", dataset);
		} else {
			printf("Data set %d:\nSome lights are not visible:\n", dataset);
			
			bool flag = false;
			for(int i = 0; (unsigned)i < ans.size(); ++i){
				if(flag)
					printf(";\n");
				printf("x = %d, y = %d", ans[i].first, ans[i].second);
				flag = true;
			}
			printf(".\n");
		}
		
		ans.clear();
		vect.clear();
		dataset++;
	}
}