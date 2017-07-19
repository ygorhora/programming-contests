#include<stdio.h>
#include<vector>
#include<iostream>

using namespace std;

typedef vector<int> vi;
typedef vector<bool> vb;
typedef vector<vi> vvi;

void computeTreeChildren(vvi &tree, vvi &treeChildren, int root, vb &visited){
	visited[root] = true;
	for(unsigned i = 0; i < tree[root].size(); i++){
		if(!visited[tree[root][i]]){
			treeChildren[root].push_back(tree[root][i]);
			computeTreeChildren(tree,treeChildren,tree[root][i],visited);
		}
	}
}

void computeTreeGrandchildren(vvi &treeChildren,vvi &treeGrandchildren, int root){
	for(unsigned i = 0; i < treeChildren[root].size(); i++){
		int child = treeChildren[root][i];
		for(unsigned j = 0; j < treeChildren[child].size(); j++){
			int grandChild = treeChildren[child][j];
			treeGrandchildren[root].push_back(grandChild);
		}
		computeTreeGrandchildren(treeChildren, treeGrandchildren, child);
	}
}


int mvc(vi &dp_mvc, vvi &treeChildren, vvi &treeGrandchildren, int root){
	
	if(dp_mvc[root] != -1){
		return dp_mvc[root];
	}
	
	int sum_mvc_children = 0;
	for(unsigned i = 0; i < treeChildren[root].size(); i++){
		int child = treeChildren[root][i];
		//printf("Child: Calculating MVC from node %d to %d\n", root, child);
		sum_mvc_children += mvc(dp_mvc, treeChildren, treeGrandchildren, child);
	}
	int root_incl = 1 + sum_mvc_children;
	
	int sum_mvc_grandchild = 0;
	for(unsigned i = 0; i < treeGrandchildren[root].size(); i++){
		int grandChild = treeGrandchildren[root][i];
		//printf("GrandChild: Calculating MVC from node %d to %d\n", root, grandChild);
		sum_mvc_grandchild += mvc(dp_mvc, treeChildren, treeGrandchildren, grandChild);
	}
	
	int root_excl = treeChildren[root].size() + sum_mvc_grandchild;
	
	dp_mvc[root] = min(root_incl, root_excl);
	
	//printf("MCV of %d = %d\n", root, dp_mvc[root]);
	return dp_mvc[root];
}

int main(){
	int n;
	scanf("%d", &n);
	
	vvi tree(n+1);
	int x, y;
	for(int i = 0; i < n - 1; ++i){
		scanf("%d %d", &x, &y);
		//printf("%d %d\n", x, y);
		tree[x].push_back(y);
		tree[y].push_back(x);
	}
	
	vb visited(n+1, false);
	vvi treeChildren(n+1);
	vvi treeGrandchildren(n+1);
	computeTreeChildren(tree,treeChildren,x,visited);
	computeTreeGrandchildren(treeChildren,treeGrandchildren,x);
	
	/*
	printf("root: %d\n", x);
	for(int i = 0; i <= n; ++i){
		printf("child i: %d => ", i);
		for(unsigned j = 0; j < treeChildren[i].size(); j++){
			printf("%d ,", treeChildren[i][j]);
		}
		puts("");
	}
	
	printf("root: %d\n", x);
	for(int i = 0; i <= n; ++i){
		printf("grandchild i: %d => ", i);
		for(unsigned j = 0; j < treeGrandchildren[i].size(); j++){
			printf("%d ,", treeGrandchildren[i][j]);
		}
		puts("");
	}
	*/
	vi dp_mvc(n+1, -1);
	cout << mvc(dp_mvc, treeChildren, treeGrandchildren, x) << '\n';
}