#include <iostream>

int main() {
    const auto mul = [](const auto x){
        return [x](const auto y){
            return x * y;
        };
    };
    const auto half = [](const auto x){
        return x / 2;
    };
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
    
    std::cout << "x = 42" << std::endl;
    std::cout << "y = 41" << std::endl;
    
    const auto xmul = mul(x); // _42_mul();
    
    std::cout << "xmul(y) = " << xmul(y) << std::endl; // _42_mul(41) = 1722
    
    const auto Sxyz_1 = S(mul)(half)(x);
    std::cout << "S(mul)(half)(x) = " << Sxyz_1 << std::endl;    
 
    // S(mul)(half)(x) = 882

    const auto Sxyz_2 = S(mul)(I(half))(K(x)(3));
    std::cout << "S(mul)(I(half))(K(x)(3)) = " << Sxyz_2 << std::endl;

    const auto Ix = I(x);
    const auto Iy = I(y);
    const auto SKKx = S(K)(K)(x);
    
    if( Ix == SKKx ) std::cout << "Success! : Ix == SKKx" << std::endl;
    if( Iy not_eq SKKx ) std::cout << "Success! : Iy not_eq SKKx" << std::endl;
    

    return 0;
}
