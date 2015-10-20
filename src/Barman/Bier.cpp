
#include<Common/Message.h>
#include<Barman/Barman.h>

#include<string>


Bier::Bier(bool in_charge) {
    m_id = Barman::addBier(this, in_charge);
}

Bier::~Bier() {
    Message::Debug("Bier %d (destroyed)", m_id);
}
