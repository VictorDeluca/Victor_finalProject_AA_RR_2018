#include <bits/stdc++.h>
#define INF 99999999
using namespace std;

int v,e,source; double graph[100][100],route_c,best; 
vector<int> route,bestroute;

double cost(vector<int> vetor){
	int i; double ans = 0;
	for(i=1;i<v;i++) ans+=graph[vetor[i-1]][vetor[i]];
	ans+=graph[vetor[i-1]][source];

	return ans;
}

vector<int> generate(){
	vector<int> ans; ans.push_back(source);
	vector<int> possibilities; int i;
	for(i=0;i<v;i++) if(i!=source) possibilities.push_back(i);

	for(i=0;i<v-1;i++){
		int next = rand()%possibilities.size();

		ans.push_back(possibilities[next]);
		possibilities.erase(possibilities.begin()+next);
	}

	return ans;
}

int main(){
	int i,j; srand(time(NULL));
	for(i=0;i<100;i++) for(j=0;j<100;j++) graph[i][j] = INF;

	scanf("%d%d",&v,&e);
	for(i=0;i<e;i++){
		int x,y; double z;
		scanf("%d%d%lf",&x,&y,&z);
		graph[x][y] = z;
	}
	scanf("%d",&source);
	bestroute = generate(); best = INF;

	int attempts = 100; //We try 100 different executions with different starting vectors
	while(attempts--){
	//Initializing with an arbitrary source.
		route = generate();

		route_c = cost(route);

		double T = 100, it;
		while(T>0.1){ //Ending at a low temperature, here it's 0.
			it = 50;
			while(it--){ //Iterations for every temperature
				int v1 = 1, v2 = 1;
				while(v1==v2 || v1==0 || v2==0){
					v1 = rand()%v;
					v2 = rand()%v;
				}
				vector<int> helper = route;
				int lo = min(v1,v2), hi = max(v1,v2);
				reverse(helper.begin()+lo, helper.begin()+hi);

				if(cost(helper) < route_c){
					route_c = cost(helper);
					route = helper;

					if(cost(helper) < best){
						best = cost(helper);
						bestroute = helper;
					}
				} else {
					double delta = cost(helper) - route_c;
					double prob = exp(-delta/T);
					double r1 = (rand()%1000); r1/=1000;

					if(r1<=prob){
						route_c = cost(helper);
						route = helper;
					}
				}
			}

			T*=0.90;
		}
	}

	printf("%.4lf\n",best);
	for(i=0;i<v;i++){
		printf("%d -> ",bestroute[i]);
	}
	printf("%d\n",source);

	return 0;
}
