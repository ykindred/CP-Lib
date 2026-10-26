#pragma once
#include <bits/stdc++.h>
using u64 = unsigned long long;
using namespace std;
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
struct chash {
    static u64 splitmix64(u64 x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    size_t operator()(u64 x) const {
        static const u64 F = 
            chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + F);
    }
};
template<typename K, typename V>
using HashMap = __gnu_pbds::gp_hash_table<K, V, chash>;