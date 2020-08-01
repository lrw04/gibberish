#include <fstream>
#include <map>
#include <random>
#include <set>
#include <string>
#include <utility>
#include <vector>

#ifndef H_LIB
#define H_LIB

// 3-gram

class Model {
    std::map<std::tuple<std::string, std::string, std::string>,
             std::map<std::string, long long>>
        m;
    std::set<std::string> dict;
    std::tuple<std::string, std::string, std::string> state;
    std::mt19937_64 engine;

   public:
    Model();
    Model(const std::string& file);       // Read initial state from file
    void write(const std::string& file);  // Write state to file
    void read(const std::string& file);   // Progress fron file content
    void read_word(const std::string& w);
    void set(std::tuple<std::string, std::string, std::string>& st);
    std::string next();
    void ginit();  // Initialize state for generation
};

#endif
