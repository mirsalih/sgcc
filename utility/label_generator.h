#ifndef __LABEL_GENERATOR_H__
#define __LABEL_GENERATOR_H__
#include <string>

namespace sgcc
{
class LabelGen {
public:
    std::string operator()() const {
        using namespace std;
        
        return "_label_"s + to_string(counter++);
    }
private:
    inline static unsigned long long counter{0};
};
}
#endif // __LABEL_GENERATOR_H__