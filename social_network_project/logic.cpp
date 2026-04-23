#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <set>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

class SocialNetwork {
private:
    unordered_map<string, unordered_set<string>> adj; // adjacency list, undirected

public:
    // 1. Add User
    void addUser(const string& name) {
        adj.emplace(name, unordered_set<string>{});
    }

    // 2. Add Friendship (undirected)
    bool addFriendship(const string& user1, const string& user2) {
        if (user1 == user2) return false; // no self friendship
        if (!exists(user1) || !exists(user2)) return false;
        adj[user1].insert(user2);
        adj[user2].insert(user1);
        return true;
    }

    // 3. Show all users and their friend lists
    void showNetwork() const {
        if (adj.empty()) {
            cout << "No users in the network.\n";
            return;
        }
        // Sort users by name for stable output
        vector<string> users;
        users.reserve(adj.size());
        for (const auto& p : adj) users.push_back(p.first);
        sort(users.begin(), users.end());

        for (const auto& user : users) {
            cout << user << ": ";
            vector<string> friends(adj.at(user).begin(), adj.at(user).end());
            sort(friends.begin(), friends.end());
            for (size_t i = 0; i < friends.size(); ++i) {
                cout << friends[i];
                if (i + 1 < friends.size()) cout << ", ";
            }
            cout << '\n';
        }
    }

    // 4. Mutual friends
    vector<string> getMutualFriends(const string& user1, const string& user2) const {
        vector<string> res;
        if (!exists(user1) || !exists(user2)) return res;
        const auto& s1 = adj.at(user1);
        const auto& s2 = adj.at(user2);
        // iterate smaller set
        const auto *a = &s1, *b = &s2;
        if (s1.size() > s2.size()) swap(a, b);
        for (const auto& f : *a) if (b->count(f)) res.push_back(f);
        sort(res.begin(), res.end());
        return res;
    }

    // 5. Friend recommendations (friends of friends, ranked by mutual count desc, then name asc)
    vector<string> recommendFriends(const string& user) const {
        vector<string> res;
        if (!exists(user)) return res;
        const auto& friends = adj.at(user);
        unordered_map<string, int> score;
        for (const auto& f : friends) {
            const auto& foafs = adj.at(f);
            for (const auto& candidate : foafs) {
                if (candidate == user) continue;
                if (friends.count(candidate)) continue; // already a friend
                score[candidate]++;
            }
        }
        vector<pair<string,int>> items(score.begin(), score.end());
        sort(items.begin(), items.end(), [](const auto& lhs, const auto& rhs){
            if (lhs.second != rhs.second) return lhs.second > rhs.second; // higher mutuals first
            return lhs.first < rhs.first; // name asc
        });
        res.reserve(items.size());
        for (auto& p : items) res.push_back(p.first);
        return res;
    }

    // 6. Shortest path between two users via BFS
    vector<string> shortestPath(const string& start, const string& end) const {
        vector<string> path;
        if (!exists(start) || !exists(end)) return path;
        if (start == end) return vector<string>{start};

        unordered_map<string, string> parent; // child -> parent
        unordered_set<string> visited;
        queue<string> q;
        visited.insert(start);
        q.push(start);

        bool found = false;
        while (!q.empty() && !found) {
            auto u = q.front(); q.pop();
            for (const auto& v : adj.at(u)) {
                if (!visited.count(v)) {
                    visited.insert(v);
                    parent[v] = u;
                    if (v == end) { found = true; break; }
                    q.push(v);
                }
            }
        }
        if (!found) return path;
        // reconstruct
        string cur = end;
        while (true) {
            path.push_back(cur);
            if (cur == start) break;
            cur = parent[cur];
        }
        reverse(path.begin(), path.end());
        return path;
    }

    // 7. Most popular user (max degree)
    string mostPopularUser() const {
        string best;
        size_t best_deg = 0;
        for (const auto& [user, friends] : adj) {
            size_t deg = friends.size();
            if (deg > best_deg || (deg == best_deg && !best.empty() && user < best)) {
                best = user;
                best_deg = deg;
            }
            if (best.empty()) { // initialize on first iteration
                best = user;
                best_deg = deg;
            }
        }
        return best;
    }

    bool exists(const string& user) const {
        return adj.find(user) != adj.end();
    }
};

static string read_line_trim() {
    string s; getline(cin, s);
    // trim leading/trailing spaces
    auto l = s.find_first_not_of(" \t\r\n");
    auto r = s.find_last_not_of(" \t\r\n");
    if (l == string::npos) return string();
    return s.substr(l, r - l + 1);
}

static void print_path(const vector<string>& path) {
    if (path.empty()) { cout << "No connection found.\n"; return; }
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i + 1 < path.size()) cout << " \u2192 ";
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    SocialNetwork sn;

    while (true) {
        cout << "----- Mini Social Network -----\n";
        cout << "1. Add User\n";
        cout << "2. Add Friendship\n";
        cout << "3. Show All Users\n";
        cout << "4. Mutual Friends\n";
        cout << "5. Friend Recommendations\n";
        cout << "6. Shortest Path Between Two Users\n";
        cout << "7. Most Popular User\n";
        cout << "8. Exit\n";
        cout << "--------------------------------\n";
        cout << "Enter choice: ";
        string line; if (!getline(cin, line)) break; 
        if (line.empty()) continue;
        int choice = 0;
        try { choice = stoi(line); } catch (...) { cout << "Invalid choice.\n"; continue; }

        if (choice == 1) {
            cout << "Enter username: ";
            string name = read_line_trim();
            if (name.empty()) { cout << "Invalid username.\n"; continue; }
            bool already = sn.exists(name);
            sn.addUser(name);
            if (already) cout << "User already exists.\n";
            else cout << "User added successfully!\n";
        } else if (choice == 2) {
            cout << "Enter first user: ";
            string u1 = read_line_trim();
            cout << "Enter second user: ";
            string u2 = read_line_trim();
            if (!sn.exists(u1) || !sn.exists(u2)) {
                cout << "Both users must exist. Add them first.\n";
            } else if (u1 == u2) {
                cout << "Cannot friend with self.\n";
            } else {
                bool ok = sn.addFriendship(u1, u2);
                if (ok) cout << "Friendship added successfully!\n";
                else cout << "Friendship already exists or invalid.\n";
            }
        } else if (choice == 3) {
            sn.showNetwork();
        } else if (choice == 4) {
            cout << "Enter first user: ";
            string u1 = read_line_trim();
            cout << "Enter second user: ";
            string u2 = read_line_trim();
            if (!sn.exists(u1) || !sn.exists(u2)) {
                cout << "Both users must exist.\n";
            } else {
                auto mf = sn.getMutualFriends(u1, u2);
                if (mf.empty()) cout << "No mutual friends.\n";
                else {
                    cout << "Mutual friends for " << u1 << " and " << u2 << ": ";
                    for (size_t i = 0; i < mf.size(); ++i) {
                        cout << mf[i];
                        if (i + 1 < mf.size()) cout << ", ";
                    }
                    cout << '\n';
                }
            }
        } else if (choice == 5) {
            cout << "Enter user: ";
            string u = read_line_trim();
            if (!sn.exists(u)) cout << "User does not exist.\n";
            else {
                auto recs = sn.recommendFriends(u);
                if (recs.empty()) cout << "No recommendations available.\n";
                else {
                    cout << "Friend recommendations for " << u << ": ";
                    for (size_t i = 0; i < recs.size(); ++i) {
                        cout << recs[i];
                        if (i + 1 < recs.size()) cout << ", ";
                    }
                    cout << '\n';
                }
            }
        } else if (choice == 6) {
            cout << "Enter start user: ";
            string s = read_line_trim();
            cout << "Enter end user: ";
            string t = read_line_trim();
            if (!sn.exists(s) || !sn.exists(t)) cout << "Both users must exist.\n";
            else {
                auto p = sn.shortestPath(s, t);
                if (p.empty()) cout << "No connection found.\n";
                else {
                    cout << "Shortest path between " << s << " and " << t << ": ";
                    print_path(p);
                }
            }
        } else if (choice == 7) {
            if (sn.mostPopularUser().empty()) cout << "No users in the network.\n";
            else cout << "Most popular user: " << sn.mostPopularUser() << '\n';
        } else if (choice == 8) {
            cout << "Exiting...\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}
