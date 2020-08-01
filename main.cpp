#include <cstdio>
#include <cstring>
#include <ctime>
#include <iostream>
#include <random>
#include <string>

#include "model.h"

using namespace std;

void train(string model, string article) {
    Model m(model);
    m.read(article);
    m.write(model);
}

void predict(string model, int mc) {
    Model m(model);
    m.ginit();
    string w, res;
    int cnt = 0;
    clock_t prev = clock();
    while (cnt < mc) {
        w = m.next();
        if (w == "") m.ginit();
        w = m.next();
        res += w + " ";
        cnt++;
        if (clock() - prev > CLOCKS_PER_SEC) {
            cerr << "Determined " << cnt << " words.\r" << flush;
            prev = clock();
        }
    }
    cerr << "Determined " << cnt << " words. Sample text is fully determined." << endl;
    cout << res << endl;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("Usage:\t%s T <MODEL> <ARTICLE>\n", argv[0]);
        printf("Usage:\t%s P <MODEL> <WC>\n", argv[0]);
        exit(-1);
    }
    if (!strcmp(argv[1], "T")) {
        train(argv[2], argv[3]);
    } else if (!strcmp(argv[1], "P")) {
        predict(argv[2], atoi(argv[3]));
    } else {
        printf("Usage:\t%s T <MODEL> <ARTICLE>\n", argv[0]);
        printf("Usage:\t%s P <MODEL>", argv[0]);
        exit(-1);
    }
}