#include <iostream>
#include <string>
#include "header.h" // Fichier d'entete

using namespace std ;

void ID()
{
	cout << "============================================== " <<endl ;
	cout << "	  SIRIKI KONE	A00166549"		  << endl ;
	cout << "	PROJET DE SIMULATION D'UNE NAT"	<< endl ;
	cout << "============================================== " <<endl<<endl ;
}

void Exit()
{
	cout << "============================================== " <<endl ;
	cout << "	FIN DE LA SIMULATION !!! "		  << endl ;
	cout << "============================================== " <<endl ;
}
int main()
{	
	ID() ;	// Debut du programme
	Simulation uneSimulation ;	// Creation d'une simulation
	uneSimulation.Accueil() ;	// Debut de la simulation
	cout << endl ;
	Exit() ; // Fin de la simulation

	return 0 ;
}