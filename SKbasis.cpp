#include <iostream>

int main() {
    //-I-----------------------------
    const auto I = [](const auto x){
        return x;
    };
    //-K-S----------------------------
    const auto K = [](const auto x){
      return [x](const auto y){ return x; };
    };
    const auto S = [](const auto x){
        return[x](const auto y){
            return [x,y](const auto z){
                auto xz = x(z);
                return xz(y(z));
            };
        };
    };
    //------------------------------
    const auto x = 42;
    const auto y = 41;

    const auto Ix = I(x);
    const auto Iy = I(y);
    const auto SKKx = S(K)(K)(x);

    if( Ix == SKKx ) std::cout << "Success! : Ix == SKKx" << std::endl;
    if( Iy not_eq SKKx ) std::cout << "Success! : Iy not_eq SKKx" << std::endl;
    

    return 0;
}
