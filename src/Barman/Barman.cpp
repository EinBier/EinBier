
#include <Common/Message.h>

#include <Barman/Barman.h>
#include <Barman/Bier.h>

#include <string>
#include <map>
#include <list>
#include <algorithm> //std::find

int Barman::m_max_id = -1;
std::list<Barman::Element> Barman::m_list_of_element;
std::map<int, Barman::Element*> Barman::m_id_to_element;


//Function quite useless, now...
void Barman::Init()
{
    Message::Info("Welcome to the tavern, Bro'!");
}

void Barman::Clear()
{
    m_id_to_element.clear();
    m_list_of_element.clear();
}


bool Barman::doesBierExists(Bier *op)
{
    int id = getIdOfBier(op);
    return (id > 0);
}

int Barman::getIdOfBier(Bier *op)
{
    Barman::Element el(op);
    //Find the Element in the list
    std::list<Barman::Element>::iterator it_find;
    it_find = std::find(m_list_of_element.begin(), m_list_of_element.end(), el);
    if(it_find != m_list_of_element.end())
        return it_find->m_id;
    else
        return -1;
}

Bier* Barman::get_Bier_ptr(int id)
{
    if ( m_id_to_element.find(id) == m_id_to_element.end() )
	return nullptr;
    else
    {
	Barman::Element *el = nullptr;
	el = m_id_to_element[id];
	return el->m_ptr;
    }
}

int Barman::addBier(Bier* b, bool in_charge) {
    m_max_id++;
    int id = m_max_id;
    Barman::Element new_elem(b, id, in_charge);
    m_list_of_element.insert(m_list_of_element.end(), new_elem);
    m_id_to_element[id] = &(m_list_of_element.back());
    Message::Info("Barman: I successfully created this guy: %d [%p]", id, b);
    new_elem.m_delete_ptr = false; //otherwise, trouble

    return id;
}

/*
int Barman::removeBier(int op_id)
{
    // check if an Bier with this id exists
    std::map<int, Barman::Element*>::iterator map_find;
    map_find = m_id_to_element.find(op_id);
    // if it does, remove it from the list and from the maps
    if (map_find != m_id_to_element.end()) {
        // The value *(map_find->second) is an Barman::Element
        std::list<Element>::iterator it_find;
        it_find = find(m_list_of_element.begin(),
                       m_list_of_element.end(),
                       *(map_find->second));
        if (it_find == m_list_of_element.end()) {
        Message::Info("Barman: I do not find this guy: %d", op_id);
            return -2;
        }
        m_list_of_element.erase(it_find);
        m_id_to_element.erase(map_find);
        Message::Info("Barman: I kicked this guy: %d", op_id);
        return 0;
    }
    Message::Info("Barman: I do not find this guy: %d", op_id);
    return -1;
}

int Barman::removeBier(Bier *op_ptr)
{
    Barman::Element el_aux(op_ptr);
    // check if an object with this name exists
    std::list<Barman::Element>::iterator map_find;
    map_find = std::find(m_list_of_element.begin(), m_list_of_element.end(), el_aux);
    // if it does, remove it from the list and from the maps
    if (map_find != m_list_of_element.end()) {
        int id = map_find->m_id;
        m_list_of_element.erase(map_find);
        m_id_to_element.erase(id);
        Message::Info("Barman: I kicked this guy: %d", id);
        return 0;
    }
    return -1;
}
*/

int Barman::DecreaseNumberOfPointing(int op_id)
{
    // check if an Bier with this id exists
    std::map<int, Barman::Element*>::iterator map_find;
    map_find = m_id_to_element.find(op_id);
    // if it does, decreases the number of pointing Operator to this Bier
    if (map_find != m_id_to_element.end()) {
        // The value *(map_find->second) is an Barman::Element
        std::list<Barman::Element>::iterator it_find;
        it_find = find(m_list_of_element.begin(),
                       m_list_of_element.end(),
                       *(map_find->second));
        if (it_find == m_list_of_element.end()) {
        Message::Info("Barman: I do not find this guy: %d", op_id);
            return -2;
        }
	int nPointer = it_find->DecreaseNumberOfPointing();
	if( nPointer == 0)
	{
	    m_list_of_element.erase(it_find);
	    m_id_to_element.erase(map_find);
	    Message::Info("Barman: I kicked this guy: %d", op_id);
	}
        return nPointer;
    }
    Message::Info("Barman: I do not find this guy: %d", op_id);
    return -1;
}


void Barman::Print()
{
    int n_operator = m_list_of_element.size();
    if(n_operator == 0)
	Message::Info("The tavern is empty!");
    else
    {
	Message::Info("Clients in the tavern:");
	for(std::map < int, Barman::Element* >::const_iterator it = m_id_to_element.begin();
	    it != m_id_to_element.end(); ++it)
	{
	    Message::Info("Map(id,ptr,bool):%d %p %d",it->first, it->second->m_ptr, it->second->m_delete_ptr);
	}
    }
}
