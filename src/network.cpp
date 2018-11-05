#include "network.h"
#include "random.h" 
#include <iostream> 
#include <algorithm> 
#include <iostream> 

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
	/// on suppose que les noeuds ont le même numéros que les values, donc commencent à 0  
	/// test if the two nodes aren't the same node 
	if( n1 == n2 ) return false;		 
	/// test if the node n1 and n2 exist = the values are not superior to the vector values (= size() -1) or inferior to 0 
	if( ( ( n1 > values.size()-1 ) or ( n2 > values.size()-1 ) ) or ( (n1 < 0) or (n2 < 0) ) ) return false;  
	/// On utilise un itérateur qui ne travaille que sur la clé du noeud sur lequel on veut itérer   
	auto X = links.equal_range(n1); 
	for( auto it = X.first; it != X.second; ++it)
	{
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
	for(size_t i(0); i<n; ++i)
	{  
		/// si le degré est supérieur au nombre de voisins existant, on réduit le degré au nombre maximal de voisins 
		if( degres[i] >= n ) degres[i] = n-1;
		size_t compteur(0);
		/// on ajoute une deuxième condition au while pour éviter de terminer en boucle infinie     
		while( degres[i] > 0 and compteur < n )
		{
			size_t node(nb.uniform_double(0, n-1)); 
			/// Si add_link fonctionne, le lien est créé, donc on décrémente le nombre de lien qu'il reste à créer 
			if(add_link(i, node))
			{
				--degres[i]; 
				++n_links;
				++compteur;  
			}
		}
	}
	return n_links; 
}
size_t Network::set_values(const std::vector<double>& new_values)
{
	values.clear(); 
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
	sort(copy.begin(), copy.end(), greater<double>() );
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
