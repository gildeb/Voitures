#include "telecommande.h"

Telecommande::Telecommande(IRrecv* recv){
    this->recv = recv;
    this->recv->enableIRIn();
}
  
String Telecommande::read(){
    String cmd = "";
    if (this->recv->decode(&results)) {
        cmd = int64ToString(results.value, HEX);
        this->recv->resume();
    }
    return cmd;
}