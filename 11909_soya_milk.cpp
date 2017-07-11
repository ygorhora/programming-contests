#include<stdio.h>
#include<math.h>

const double PI = 3.141592653589793;

int main(){
	int l, w, h, d;
	
	while(scanf("%d %d %d %d", &l, &w, &h, &d) == 4){
		int total = l*h*w;
		double angleRad = d*PI/180;
		double ll = l*tan(angleRad);
		if(ll > h){
			angleRad = (90 - d)*PI/180;
			double hh = h*tan(angleRad);
			double res = (hh * h * w)/2;
			printf("%.3f mL\n", res);
		} else {
			double out = (w*l*ll)/2;
			printf("%.3f mL\n", total-out);
		}
	}
}