#pragma once
#include "../head.hpp"

// 线性筛出质数、最小质因子、欧拉函数和莫比乌斯函数，并保存后两者的前缀和。
// 构造时指定上限 n；结果依次存于 primes、minp、phi、mu、sphi、smu，下标从零到 n。
struct MobiusSieve {
    int n;
    vector<int> primes, minp, phi, mu;
    vector<ll> sphi, smu;
    // 传入正上限 n，建立筛表；一的欧拉函数和莫比乌斯函数均为一，零号项保留为零。
    // 预处理时间、空间均为 $O(n)$。
    MobiusSieve(int n) : n(n), minp(n + 1), phi(n + 1), mu(n + 1),
                         sphi(n + 1), smu(n + 1) {
        assert(n >= 1);
        phi[1] = mu[1] = 1;
        for (int i = 2; i <= n; i++) {
            if (!minp[i]) {
                primes.push_back(i);
                minp[i] = i;
                phi[i] = i - 1;
                mu[i] = -1;
            }
            for (int p : primes) {
                if (p > n / i) break;
                minp[i * p] = p;
                if (i % p == 0) {
                    phi[i * p] = phi[i] * p;
                    break;
                }
                phi[i * p] = phi[i] * (p - 1);
                mu[i * p] = -mu[i];
            }
        }
        for (int i = 1; i <= n; i++) {
            sphi[i] = sphi[i - 1] + phi[i];
            smu[i] = smu[i - 1] + mu[i];
        }
    }
};
