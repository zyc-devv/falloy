#include <iostream>
#include <fstream>
class in:public::ifstream {
public:
	explicit in(const std::string & filename)
		: std::ifstream(filename) 
    {
        if (!is_open()) {
            throw std::runtime_error("无法打开文件: " + filename);
        }
	}
}
class out {
public:
	explicit out(const std::string& filename) 
        : std::ofstream(filename) 
    {
        if (!is_open()) {
            throw std::runtime_error("无法创建文件: " + filename);
        }
    }
}