
#include <Core/Barman.h>
#include <Common/Message.h>

#include <string>
#include <map>
#include <list>
#include <algorithm> //std::find

// define the variable "operatorHandlers" (not global)
// This will contain the different Barman, even if a priori, there is only one :-)
static std::map<std::string, Barman*> operatorBarmans;

Barman::Barman(std::string myname): m_n_operator(0), m_max_id(-1), m_name(myname)
{
}

Barman::~Barman()
{
    m_id_to_element.clear();
    m_element_to_id.clear();
    m_list_of_element.clear();
}

void Barman::Clear()
{
//Loop on every potential Barman and close them
    std::map<std::string, Barman*>::iterator it;
    for(it = operatorBarmans.begin(); it != operatorBarmans.end(); it++) {
        delete it->second;
        it->second = NULL;
    }
//clear map
    operatorBarmans.clear();
}


void Barman::Init(std::string opBarmanName)
{
    Message::Info("Creating Barman...");
//Find in the local (but global to every Barman) map:
    std::map<std::string, Barman*>::iterator it_find;
    it_find = operatorBarmans.find(opBarmanName);
    if (it_find == operatorBarmans.end()) {
        //There is no one ? create one !
        operatorBarmans[opBarmanName] = new Barman(opBarmanName);
    }
    else
        Message::Warning("Barman already exist!");
}


Barman* Barman::getBarman(std::string opBarmanName)
{
//Find in the local (but global to every Barman) map:
    std::map<std::string, Barman*>::iterator it_find;
    it_find = operatorBarmans.find(opBarmanName);
    if (it_find == operatorBarmans.end())
        return NULL;
    else
        return it_find->second;
}

bool Barman::doesOperatorExists(Operator *op)
{
    int id = getIdOfOperator(op);
    return (id > 0);
}

int Barman::getIdOfOperator(Operator *op)
{
    Barman::element el(op, false);
    //Find the element in the list
    std::list<Barman::element>::iterator it_find;
    it_find = std::find(m_list_of_element.begin(), m_list_of_element.end(), el);
    if(it_find != m_list_of_element.end())
        return it_find->m__id;
    else
        return -1;
}


int Barman::addOperator(Operator *op, bool destroyIt)
{
    m_max_id++;
    int id = m_max_id;
    Barman::element new_elem(op, destroyIt, id);
    // push_back(new_elem) is equivalent ?
    m_list_of_element.insert(m_list_of_element.end(), new_elem );
    // &(m_list_of_element.back()) := new_elem ?
    m_id_to_element[id] = &(m_list_of_element.back());
    Message::Debug("Barman  add Operator  (barman: %d) (id: %d)", this, id);

    // int Barman::removeOperator(Operator *op_ptr)
    std::list<Barman::element>::iterator finder;
    finder = std::find(m_list_of_element.begin(), m_list_of_element.end(), new_elem);
    if (finder != m_list_of_element.end()) {
        int idd = finder->m__id;
        Message::Debug("Barman  find Operator (barman: %d) (id: %d) [%p]",
                       this, idd, finder->m__ptr);
    } else {
        Message::Debug("Barman  not found Operator  (barman: %d) (id: %d)",
                       this, id);
    }

    // int Barman::removeOperator(int op_id)
    std::map<int, Barman::element*>::iterator map_find;
    map_find = m_id_to_element.find(id);
    if (map_find != m_id_to_element.end()) {
        std::list<element>::iterator it_find;
        it_find = find(m_list_of_element.begin(),
                       m_list_of_element.end(),
                       *(map_find->second));
        if (it_find == m_list_of_element.end()) {
            Message::Error("Barman  not found Operator  (barman: %d) (id: %d)",
                           this, id);
        } else {
            int iddd = it_find->m__id;
            Message::Debug("Barman  find Operator (barman: %d) (id: %d) [%p]",
                           this, iddd, it_find->m__ptr);
        }
    } else {
        Message::Error("Barman  not found Operator  (barman: %d) (id: %d)",
                       this, id);
    }

    return id;
}


int Barman::removeOperator(int op_id)
{
    // check if an Operator with this id exists
    std::map<int, Barman::element*>::iterator map_find;
    map_find = m_id_to_element.find(op_id);
    // if it does, remove it from the list and from the maps
    if (map_find != m_id_to_element.end()) {
        // The value *(map_find->second) is an Barman::element
        std::list<element>::iterator it_find;
        it_find = find(m_list_of_element.begin(),
                       m_list_of_element.end(),
                       *(map_find->second));
        if (it_find == m_list_of_element.end()) {
            Message::Error("Barman  rm Operator by {id}: Cannot find element in the registry", this);
            return -2;
        }
        m_list_of_element.erase(it_find);
        m_id_to_element.erase(map_find);
        return 0;
    }
    Message::Error("Barman  rm Operator by {id}: Cannot find element in the registry", this);
    return -1;
}

int Barman::removeOperator(Operator *op_ptr)
{
    Barman::element el_aux(op_ptr);
    // check if an object with this name exists
    std::list<Barman::element>::iterator map_find;
    map_find = std::find(m_list_of_element.begin(), m_list_of_element.end(), el_aux);
    // if it does, remove it from the list and from the maps
    if (map_find != m_list_of_element.end()) {
        int id = map_find->m__id;
        m_list_of_element.erase(map_find);
        m_id_to_element.erase(id);
        Message::Debug("Barman  rm Operator  (barman: %d) (id: %d) {*ptr}", this, id);
        return 0;
    }
    return -1;
}

void Barman::Print()
{
    Message::Info("Printing Barman...");
    for(std::map < int, Barman::element* >::const_iterator it = m_id_to_element.begin();
	it != m_id_to_element.end(); ++it) {
        Message::Info("Map(id,ptr):%d %p",it->first, it->second->m__ptr);
    }

}
