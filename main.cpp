#include <iostream>
#include <vector>
#include <map>
#include <algorithm>


using namespace std;



struct Tree {

    int DP[15][3][3];
    int DPknap[15][40];
    int colors[3] = {-1, 1, 2};
    map<int, int> ids = {{-1, 0}, {1, 1}, {2, 1}};
    int INF = 1e5;
    int SHIFT = 22;

    int n;
    vector<vector<int>> adj;
    vector<vector<int>> children;

    void init(int n) {
        this->n = n;
        this->adj.resize(n);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void make_rooted_tree(int u, int p = -1) {

        if(p == -1) {
            children.assign(n,vector<int>());
        }

        for(int v : adj[u]) {
            if(v != p) {
                children[u].push_back(v);
                make_rooted_tree(v,u);
            }
        }

    }

    bool check_domination(vector<int> assignment) {
        if(assignment.size() != n) {
            return false;
        }

        for(int u = 0 ; u < n; u++) {

            int sum = assignment[u];
            for(int v : adj[u]) {
                sum += assignment[v];
            }
            if(sum < 1) {
                return false;
            }
        }

        return true;

    }



    int solve_brute() {

        if(n > 15) {
            cout << "Please Try with Smaller( <= 15) size of tree" << endl;
            return INF;
        }

        int ans = INF;

        int NUM_MASKS = 1;

        for(int i = 0; i < n; i++) {
            NUM_MASKS *= 3;
        }

        for(int mask = 0; mask < NUM_MASKS; mask++) {
            vector<int> assignment(n);
            int mask_t = mask;
            for(int u = 0; u < n; u++) {
                assignment[u] = colors[mask_t % 3];
                mask_t /= 3;
            }

            if(check_domination(assignment)) {
                int sum = 0;
                for(int i = 0; i < n; i++) {
                    sum += assignment[i];
                }

                ans = min(ans, sum);
            }

        }

        return ans;
    }



    int calcDPknap_(int idx, int sum, vector<int> &v1, vector<int> &v2, vector<int> &v3) {

        if(DPknap[idx][sum] != -INF) {
            return DPknap[idx][sum];
        }
        if(idx == 0) {
            if((sum - SHIFT) <= -1) {
                return DPknap[idx][sum] = min({v1[0], v2[0], v3[0]});
            }
            else if((sum - SHIFT) <= 1) {
                //cout << " HIIIIIII" << endl;
                return DPknap[idx][sum] = min(v2[0], v3[0]);
            }
            else if((sum - SHIFT) == 2) {
                return DPknap[idx][sum] = v3[0];
            }
            else {
                return DPknap[idx][sum] = INF;
            }
        }

        return DPknap[idx][sum] = min({
            v1[idx] + calcDPknap_(idx-1, sum +1, v1, v2, v3),
            v2[idx] + calcDPknap_(idx-1, sum -1, v1, v2, v3),
            v3[idx] + calcDPknap_(idx-1, sum -2, v1, v2, v3),
            });
    }

    int calcDPknap(int idx, int sum, vector<int> &v1, vector<int> &v2, vector<int> &v3) {

        for(int i = 0 ; i < 15; i++) {
            for(int j = 0; j < 40; j++) {
                DPknap[i][j] = -INF;
            }
        }
        sum = sum + SHIFT; // arbitrary shift for making neg values positive.

        return calcDPknap_(idx, sum, v1, v2, v3);

    }



    int calcDP(int v, int x, int px) {

        int ix = ids[x];
        int ipx = ids[px];

        if(children[v].size() == 0) {
            if(x + px >= 1) {
                return DP[v][ix][ipx] = x;
            }
            else {
                return DP[v][ix][ipx] = INF;
            }
        }

        int k = children[v].size();

        vector<int> v1(k), v2(k), v3(k);

        for(int i = 0; i < k; i++) {
            v1[i] = calcDP(children[v][i], -1, x);
            v2[i] = calcDP(children[v][i], 1, x);
            v3[i] = calcDP(children[v][i], 2, x);
        }


        return DP[v][ix][ipx] = x + calcDPknap(k-1, 1 - x - px, v1, v2, v3);


    }


    int solve_dp() {

        if(n > 15) {
            cout << "Please Try with Smaller( <= 15) size of tree" << endl;
            return INF;
        }

        int root = 0;

        make_rooted_tree(root);
        for(int i = 0 ; i < 15; i++) {
            for(int j = 0; j < 3; j++) {
                for(int k = 0; k < 3; k++) {
                    DP[i][j][k] = -INF;
                }
            }
        }
        int ans = INF;

        for(int x : colors) {
            ans = min(ans, calcDP(root, x, 0));
        }

        return ans;

    }


};

int main() {

    Tree tree = *(new Tree());

    tree.init(11);

    tree.addEdge(0,1);
    tree.addEdge(1,2);
    tree.addEdge(1,3);
    tree.addEdge(2,4);
    tree.addEdge(2,5);
    tree.addEdge(3,6);
    tree.addEdge(6,7);
    tree.addEdge(1,8);
    tree.addEdge(2,9);
    tree.addEdge(9,10);

    cout << "Brute Solution: " << tree.solve_brute() << endl;
    cout << "DP Solution: " << tree.solve_dp() << endl;



    return 0;
}
