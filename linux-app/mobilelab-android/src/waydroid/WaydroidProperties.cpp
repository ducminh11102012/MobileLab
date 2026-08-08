#include "WaydroidProperties.h"
#include <sstream>
namespace mobilelab::waydroid {
std::map<std::string,std::string> WaydroidProperties::parse(const std::string& text){
    std::map<std::string,std::string> r; std::istringstream in(text); std::string line;
    while(std::getline(in,line)){auto p=line.find('='); if(p!=std::string::npos) r[line.substr(0,p)]=line.substr(p+1);} return r;
}
}
