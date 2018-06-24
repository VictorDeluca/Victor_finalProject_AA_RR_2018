#include <bits/stdc++.h>
#define INF 99999999
using namespace std;

int v,e,source; double graph[32][32];

pair<double,vector<int> > solve(int mask, int prev){
	if(mask==(1<<v)-1){ 
		vector<int> helper; helper.push_back(source);
		return make_pair(graph[prev][source],helper);
	}

	int i; double ans = INF; vector<int> best;
	for(i=0;i<v;i++){
		if(!(mask&(1<<i))){
			pair<double,vector<int> > helper = solve(mask|(1<<i),i);
			if(helper.first + graph[prev][i] < ans){
				ans = helper.first + graph[prev][i];
				best = helper.second; best.push_back(i);
			}
		}
	}
	return make_pair(ans,best);
}

int main(){
	int i,j;
	for(i=0;i<32;i++) for(j=0;j<32;j++) graph[i][j] = INF;

	scanf("%d%d",&v,&e);
	for(i=0;i<e;i++){
		int x,y; double z;
		scanf("%d%d%lf",&x,&y,&z);
		graph[x][y] = z;
	}
	scanf("%d",&source);
	pair<double,vector<int> > ans = solve((1<<source),source);
	printf("%.4lf\n",ans.first);

	reverse(ans.second.begin(),ans.second.end());
	printf("%d -> ",source);
	for(i=0;i<ans.second.size()-1;i++){
		printf("%d -> ",ans.second[i]);
	}
	printf("%d\n",source);

	return 0;
}