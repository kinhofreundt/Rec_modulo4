#include <iostream>

float converteSensor(float medida, float valmin, float valmax){
    float valor = 100*(medida-valmin)/(valmax-valmin);
    return valor;
}

int main(int argc, char** argv){
    std::cout<<"oi;";
    int medida = converteSensor(415,0,830);
    std::cout<<medida;
    return 0;
}


