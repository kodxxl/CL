#include <iostream>

template <class T>
constexpr std::string_view type_name()
{
#ifdef __clang__
    string_view p = __PRETTY_FUNCTION__;
    return std::string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
    std::string_view p = __PRETTY_FUNCTION__;
#  if __cplusplus < 201402
    return std::string_view(p.data() + 36, p.size() - 36 - 1);
#  else
    return std::string_view(p.data() + 49, p.find(';', 49) - 49);
#  endif
#elif defined(_MSC_VER)
    std::string_view p = __FUNCSIG__;
    return std::string_view(p.data() + 84, p.size() - 84 - 7);
#endif
}

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
    std::cout << "type_name(mul) = " << type_name<decltype(mul)>() << std::endl;
    std::cout << "type_name(xmul) = " << type_name<decltype(xmul)>() << std::endl;
    std::cout << "type_name(half) = " << type_name<decltype(half)>() << std::endl;

    std::cout << "xmul(y) = " << xmul(y) << std::endl; // _x_mul(y) = 1722
    
    const auto Sxyz_1 = S(mul)(half)(x);
    std::cout << "S(mul)(half)(x) = " << Sxyz_1 << std::endl;    
 
    // S(mul)(half)(x) = 882

    const auto Sxyz_2 = S(mul)(I(half))(K(x)(3));
    std::cout << "S(mul)(I(half))(K(x)(3)) = " << Sxyz_2 << std::endl;
    
    // S(mul)(I(half))(K(x)(3)) = 882

    const auto Ix = I(x);
    const auto Iy = I(y);
    const auto SKKx = S(K)(K)(x);
    
    if( Ix == SKKx ) std::cout << "Success! : Ix == SKKx" << std::endl;
    if( Iy not_eq SKKx ) std::cout << "Success! : Iy not_eq SKKx" << std::endl;
    

    return 0;
}
