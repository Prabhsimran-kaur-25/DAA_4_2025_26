class Solution {
public:
    vector<int> maxOfSubarrays(vector<int>& arr, int k) {
        int n = arr.size();
        vector<int> maxi;
        for(int i = 0; i <= n-k; i++) {
          int maxa = arr[i];
            for(int j = i+1; j < i+k; j++) {
                maxa = max(maxa, arr[j]);
            }
        maxi.push_back(maxa);
        }
        return maxi;
    }
};