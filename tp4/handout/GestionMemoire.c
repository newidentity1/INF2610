#include "./libs/lib.h"

const int TAILLE_PAGE = 1024;

unsigned int calculerNumeroDePage(unsigned long adresse) {
	// TODO
	return adresse/TAILLE_PAGE;
}

unsigned long calculerDeplacementDansLaPage(unsigned long adresse) {
	// TODO
	return adresse%TAILLE_PAGE;
}

unsigned long calculerAdresseComplete(unsigned int numeroDePage, unsigned long deplacementDansLaPage) {
	// TODO
	return numeroDePage*TAILLE_PAGE + deplacementDansLaPage;
}

void rechercherTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	// TODO
	req->adressePhysique = 0;
	for (int i = 0; i < TAILLE_TLB; ++i) {
		if (mem->tlb->numeroPage[i] == calculerNumeroDePage(req->adresseVirtuelle) && mem->tlb->entreeValide[i]) {
			req->adressePhysique = calculerAdresseComplete(mem->tlb->numeroCadre[i], calculerDeplacementDansLaPage(req->adresseVirtuelle));
			mem->tlb->dernierAcces[i] = req->date;
			break;
		}
	}
}

void rechercherTableDesPages(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	// TODO
	const unsigned int numeroDePage = calculerNumeroDePage(req->adresseVirtuelle);
	req->adressePhysique = 0;
	if (mem->tp->entreeValide[numeroDePage]) {
		req->adressePhysique = calculerAdresseComplete(mem->tp->numeroCadre[numeroDePage], calculerDeplacementDansLaPage(req->adresseVirtuelle));
	}
}

void ajouterDansMemoire(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	//TODO
	const unsigned int numeroDePage = calculerNumeroDePage(req->adresseVirtuelle);
	for (int i = 0; i < TAILLE_MEMOIRE; ++i) {
		if (!mem->memoire->utilisee[i]) {
			req->adressePhysique = calculerAdresseComplete(i, calculerDeplacementDansLaPage(req->adresseVirtuelle));
			mem->memoire->utilisee[i] = 1;
			mem->memoire->numeroPage[i] = numeroDePage;
			mem->memoire->dateCreation[i] = req->date;
			mem->memoire->dernierAcces[i] = req->date;
			break;
		}
	}
}

void mettreAJourTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	// TODO
	int indexPlusAncienneEntree = 0;
	int indexEntreeVide = -1;
	for (int i = 0; i < TAILLE_TLB; ++i) {
		if (!mem->tlb->entreeValide[i]) {
			indexEntreeVide = i;
			break;
		}

		if (mem->tlb->dateCreation[i] < mem->tlb->dateCreation[indexPlusAncienneEntree]) 
			indexPlusAncienneEntree = i;
		
	}
	const int indexARemplacer = indexEntreeVide != -1 ? indexEntreeVide : indexPlusAncienneEntree;
	mem->tlb->dateCreation[indexARemplacer] = req->date;
	mem->tlb->dernierAcces[indexARemplacer] = req->date;
	mem->tlb->entreeValide[indexARemplacer] = 1;
	mem->tlb->numeroPage[indexARemplacer] = calculerNumeroDePage(req->adresseVirtuelle);
	mem->tlb->numeroCadre[indexARemplacer] = calculerNumeroDePage(req->adressePhysique);
}

// NE PAS MODIFIER
int main() {
    evaluate(
		&calculerNumeroDePage, 
		&calculerDeplacementDansLaPage, 
		&calculerAdresseComplete, 
        &rechercherTLB, 
		&rechercherTableDesPages,
		&mettreAJourTLB,
		&ajouterDansMemoire
    );
    return 0;
}
