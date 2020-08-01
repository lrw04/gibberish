#include "model.h"

#include <cstdio>
#include <ctime>
#include <iostream>
#include <random>
#include <sstream>

#include "word.h"

using namespace std;

Model::Model() {
    m.clear();
    dict.clear();
    random_device rdev;
    engine = mt19937_64(rdev());
}

Model::Model(const string &file) {
    m.clear();
    dict.clear();
    random_device rdev;
    engine = mt19937_64(rdev());
    ifstream is(file);
    if (!is) {
        cerr << "Warning: error reading " << file << ", starting new." << endl;
        return;
    }
    string v1, v2, v3, n;
    long long t, c = 0;
    clock_t prev = clock();
    while (is >> v1 >> v2 >> v3 >> n >> t) {
        m[make_tuple(v1, v2, v3)][n] = t;
        dict.insert(v1);
        dict.insert(v2);
        dict.insert(v3);
        dict.insert(n);
        c++;
        if (clock() - prev > CLOCKS_PER_SEC) {
            cerr << "Read " << c << " entries from model.\r" << flush;
            prev = clock();
        }
    }
    cerr << "Read " << c << " entries from model. Done loading model.\r" << endl;
    is.close();
}

void Model::write(const string &file) {
    ofstream os(file);
    if (!os) {
        cerr << "Error writing to " << file << ", exiting." << endl;
        exit(-1);
    }
    long long c = 0;
    clock_t prev = clock();
    for (const auto &[tup, sp] : m) {
        for (const auto &[ns, t] : sp) {
            if (get<0>(tup).length() && get<1>(tup).length() &&
                get<2>(tup).length() && ns.length())
                os << get<0>(tup) << " " << get<1>(tup) << " " << get<2>(tup)
                   << " " << ns << " " << t << endl;
            c++;
            if (clock() - prev > CLOCKS_PER_SEC) {
                cerr << "Wrote " << c << " entries to model.\r" << flush;
                prev = clock();
            }
        }
    }
    cerr << "Wrote " << c << " entries to model. Done writing model.\r" << endl;
    os.close();
}

void Model::set(tuple<string, string, string> &st) { state = st; }

void Model::read_word(const string &w) {
    m[state][w]++;
    state = make_tuple(get<1>(state), get<2>(state), w);
    dict.insert(w);
}

void Model::read(const string &file) {
    string s;
    FILE *fin = fopen(file.c_str(), "r");
    if (!fin) {
        cerr << "Error reading " << file << ", exiting." << endl;
        exit(-1);
    }
    int c;
    while ((c = fgetc(fin)) != EOF) {
        if (is_punct(c)) s += ' ';
        s += c;
        if (is_punct(c)) s += ' ';
    }
    fclose(fin);

    stringstream ss;
    ss.str(s);
    string w;

    long long cnt = 0;
    clock_t prev = clock();
    while (ss >> w) {
        read_word(regular(w));
        cnt++;
        if (clock() - prev > CLOCKS_PER_SEC) {
            cerr << "Read " << cnt << " words from article.\r" << flush;
            prev = clock();
        }
    }
    cerr << "Read " << cnt << " words from article. Done reading article.\r" << endl;
}

string Model::next() {
    if (!(m.count(state))) return "";
    const auto &dist = m[state];
    vector<string> tab;
    vector<long long> weight;
    for (const auto &[w, p] : dist) {
        tab.push_back(w);
        weight.push_back(p);
    }
    discrete_distribution<long long> pdist(weight.begin(), weight.end());
    int res = pdist(engine);
    state = make_tuple(get<1>(state), get<2>(state), tab[res]);
    return tab[res];
}

void Model::ginit() {
    uniform_int_distribution<int> dist(0, m.size() - 1);
    int t = dist(engine);
    auto it = m.begin();
    for (int i = 0; i < t; i++) it++;
    state = it->first;
}
