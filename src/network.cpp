#include "network.h"
#include "random.h" 
#include <iostream> 
#include <algorithm> 

using namespace std; 

void Network::resize(const size_t& s)
{
	if(values.size() != 0) values.erase(values.begin(), values.end());
	values.resize(s); 
	RandomNumbers nb; 
	nb.normal(values, 0, 1); 
}
bool Network::add_link(const size_t& n1, const size_t& n2)
{
	for(std::multimap<size_t, size_t>::iterator it = links.begin(); it != links.end(); ++it )
	{
		/// test if the two nodes aren't the same node 
		if( n1 == n2 ) return false; 
		/// test if the node n1 and n2 exists = the values are not superior to the vector values or inferior to 0 
		if( ( ( n1 > values.size() ) or ( n2 > values.size() ) ) or ( (n1 < 0) or (n2 < 0) ) ) return false; 
		/// this if clause test if link already exist on both side
		if( ( (it->first == n1) and (it->second == n2) ) or ( ( it->first == n2 )  and  ( it->second == n1 ) ) ) return false;
	}
	links.insert(std::pair<size_t, size_t>(n1, n2));
	links.insert(std::pair<size_t, size_t>(n2, n1));
	return true;
}
size_t Network::random_connect(const double& d)
{
	/// on commence par effacer tous les liens 
	links.clear(); 
	///  le nombre de liens créer 
	size_t n_links(0); 
	/// un élément aléatoire pour utilisation plus tard 
	RandomNumbers nb; 
	/// pour éviter d'appeler values.size() trop de fois 
	size_t n(values.size());
	/// ce vector contient tous les degré de liaison pour chaque noeud, l'entrée du tableau correspond au numéro du noeud  
	vector<int> degres(n);
	nb.poisson(degres, d); 
	/// le i représente le noeud sur lequel on travaille, allant de 0 à n-1
	for(size_t i(0); i<n-1; ++i)
	{  
		/// si le degré est supérieur au nombre de voisins existant, on réduit le degré au nombre maximal de voisins 
		if( degres[i] >= n ) degres[i] = n-1;   
		while( degres[i] > 0 )
		{
			size_t node(nb.uniform_double(0, n-1)); 
			if( (degres[node] > 0) and add_link(i, node)) /// Si add_link fonctionne et que node peut encore faire des liens, un lien a été créé pour le noeud i et node, donc on décrémente de 1 le nombre de voisin/degré de liaison 
			{
				--degres[i]; 
				--degres[node];
				++n_links; 
			}
		}
	}
	return n_links; 
}
size_t Network::set_values(const std::vector<double>& new_values)
{
	values = new_values; 
	return new_values.size();  // A REVOIR, CAR JE DOIS RETOURNER LE NOMBRE DE VALEURS AFFECTÉ AVEC SUCCÈS, MAIS NORMALEMENT IL N'Y A PAS DE BUGS, NE COMPREND PAS L'UTILITÉ DU RETURN 
}
size_t Network::size() const
{
	return values.size(); 
}
size_t Network::degree(const size_t& n) const
{
	multimap<size_t, size_t> copy = links; 
	size_t nb(0);
	for(std::multimap<size_t, size_t>::iterator it = copy.begin(); it != copy.end(); ++it )
	{
		/// Si la clé est trouvée, on augmente le compteur 
		if(it->first == n) ++nb; 
	}
	return nb;
}
double Network::value(const size_t& n) const
{
	return values[n]; 
}
std::vector<double> Network::sorted_values() const
{
	vector<double> copy(values); 
	reverse(copy.begin(), copy.end());
	return copy;  
}
std::vector<size_t> Network::neighbors(const size_t& n) const
{
	multimap<size_t, size_t> copy = links; 
	vector<size_t> list; 
	for(std::multimap<size_t, size_t>::iterator it = copy.begin(); it != copy.end(); ++it )
	{
		if(it->first == n) list.push_back(it->second);
	}
	return list; 
}
