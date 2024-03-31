#include "tests.h"

using namespace std;

void Tests::testAll() {
	testDomain();
	testRepo();
	testValidator();
	testService();
}

void Tests::testDomain() {
	testConstructor_Getters();
	testCopyConstructor();
	testSetters();
	testToString();
	testOperatori();
	testComparatori();
}

void Tests::testRepo() {
	testAdaugaRepo();
	testModificaRepo();
	testCautaRepo();
	testStergeRepo();
	testGet_disciplina();
}

void Tests::testService() {
	testAdaugaSrv();
	testModificaSrv();
	testStergeSrv();
	testCautaSrv();
	testFiltrare();
	testSortare();
}

void Tests::testValidator() {
	Validator val;
	Disciplina test{ "mate", 3, "real", "Alexe" };
	Disciplina invalid{};

	val.valideazaDisciplina(test);
	try {
		val.valideazaDisciplina(invalid);
	}
	catch (ValidatorException& mesaj) {
		assert(mesaj.get_mesaj() == "Denumire invalida!\nNumar de ore invalid!\nTip invalid!\nNumele profesorului este invalid!\n");
	}
}

/// 
/// DOMAIN
/// 

void Tests::testConstructor_Getters() {
	Disciplina test_disciplina{ "mate",3,"real","Alexe" };
	assert(test_disciplina.get_denumire() == "mate");
	assert(test_disciplina.get_ore() == 3);
	assert(test_disciplina.get_tip() == "real");
	assert(test_disciplina.get_profesor() == "Alexe");

	Disciplina test_implicit{};
	assert(test_implicit.get_denumire().empty());
	assert(test_implicit.get_ore() == -1);
	assert(test_implicit.get_tip().empty());
	assert(test_implicit.get_profesor().empty());
}

void Tests::testCopyConstructor() {
	Disciplina test_disciplina{ "mate",3,"real","Alexe" };
	Disciplina test_copy(test_disciplina);
	assert(test_copy.get_denumire() == "mate");
	assert(test_copy.get_ore() == 3);
	assert(test_copy.get_tip() == "real");
	assert(test_copy.get_profesor() == "Alexe");
}

void Tests::testSetters() {
	Disciplina test_disciplina{ "mate",3,"real","Alexe" };

	test_disciplina.set_tip("uman");
	assert(test_disciplina.get_tip() == "uman");

	test_disciplina.set_ore(4);
	assert(test_disciplina.get_ore() == 4);
}

void Tests::testToString() {
	Disciplina test_disciplina{ "mate",3,"real","Alexe" };
	assert(test_disciplina.to_string_print() == "\tDisciplina: mate;   Ore: 3;   Tip: real;   Profesor: Alexe.");
}

void Tests::testOperatori() {
	Disciplina test1{ "mate",3,"real","Alexe" };
	assert(test1.operator==(test1));

	Disciplina test2;
	test2.operator=(test1);
	assert(test2.get_denumire() == "mate");
	assert(test2.get_ore() == 3);
	assert(test2.get_tip() == "real");
	assert(test2.get_profesor() == "Alexe");
}

void Tests::testComparatori() {
	Disciplina test1{ "info",3,"real","Alexe" };
	Disciplina test2{ "mate",6,"real","Ionescu" };

	assert(cmpDenumire(test1, test2) == true);
	assert(cmpDenumire(test2, test1) == false);

	assert(cmpOre(test1, test2) == true);
	assert(cmpOre(test2, test1) == false);

	assert(cmpProfesor(test1, test2) == true);
	assert(cmpProfesor(test2, test1) == false);

	assert(cmpProfTip(test1, test2) == true); 
	assert(cmpProfTip(test2, test1) == false);

	Disciplina test3{ "mateM2",4,"uman","Alexe" };

	assert(cmpProfTip(test1, test3) == true);
	assert(cmpProfTip(test3, test1) == false);
}

///
/// REPO
/// 

void Tests::testAdaugaRepo() {
	Repo repo;
	Disciplina test{ "info",3,"real","Alexe" };

	repo.adaugaRepo(test);
	try {
		repo.adaugaRepo(test);
	}
	catch (RepoException& mesaj) {
		assert(mesaj.get_mesaj() == "Disciplina deja exista!\n");
	}

	assert(test.operator == (repo.get_disciplina("info", "Alexe")));
}

void Tests::testCautaRepo() {
	Repo repo;
	Disciplina test{ "info",3,"real","Alexe" };
	repo.adaugaRepo(test);
	assert(repo.cautaRepo("info", "Alexe") == 0);
	assert(repo.cautaRepo("mate", "Ionescu") == -1);
}

void Tests::testModificaRepo() {
	Repo repo;
	Disciplina test{ "info",3,"real","Alexe" };
	Disciplina test1{ "info",4,"uman","Alexe" };
	Disciplina test2{ "mate", 2, "real", "Ionescu" };

	repo.adaugaRepo(test);
	repo.modificaRepo(test1);

	assert(repo.cautaRepo("info", "Alexe") == 0);

	try {
		repo.modificaRepo(test2);
	}
	catch (RepoException& mesaj) {
		assert(mesaj.get_mesaj() == "Disciplina nu exista!\n");
	}

	const vector<Disciplina> discipline = repo.getAll();
	assert(discipline.size() == 1);
}

void Tests::testStergeRepo() {
	Repo repo;
	Disciplina test{ "info",3,"real","Alexe" };

	repo.adaugaRepo(test);
	repo.stergeRepo("info", "Alexe");
	try {
		repo.stergeRepo("info", "Alexe");
	}
	catch (RepoException& mesaj) {
		assert(mesaj.get_mesaj() == "Disciplina nu exista!\n");
	}
}

void Tests::testGet_disciplina() {
	Repo repo;
	Disciplina test{ "info",3,"real","Alexe" };

	repo.adaugaRepo(test);
	assert(test.operator == (repo.get_disciplina("info", "Alexe")));
	try {
		repo.get_disciplina("mate", "Ionescu");
	}
	catch (RepoException& mesaj) {
		assert(mesaj.get_mesaj() == "Disciplina nu exista!\n");
	}
}

///
/// SERVICE
///

void Tests::testAdaugaSrv() {
	Repo repo;
	Validator validator;
	Service service(repo, validator);

	service.adaugaSrv("info", 3, "real", "Alexe");

	const vector<Disciplina>& discipline = service.getAll();
	assert(discipline.size() == 1);

	try {
		service.adaugaSrv("info", 3, "real", "Alexe");
	}
	catch (RepoException& mesaj) {
		assert(mesaj.get_mesaj() == "Disciplina deja exista!\n");
	}
}

void Tests::testCautaSrv() {
	Repo repo;
	Validator validator;
	Service service(repo, validator);
	Disciplina test{ "info", 3, "real", "Alexe" };

	service.adaugaSrv("info", 3, "real", "Alexe");

	const vector<Disciplina>& discipline = service.getAll();
	assert(discipline.size() == 1);

	assert(test.operator==(service.cautaSrv("info", "Alexe")));
	try {
		service.cautaSrv("mate", "Ionescu");
	}
	catch (RepoException& mesaj) {
		assert(mesaj.get_mesaj() == "Disciplina nu exista!\n");
	}
}

void Tests::testModificaSrv() {
	Repo repo;
	Validator validator;
	Service service(repo, validator);
	Disciplina test{ "info", 3, "real", "Alexe" };
	Disciplina test1{ "info", 4, "uman", "Alexe" };

	service.adaugaSrv("info", 3, "real", "Alexe");

	service.modificaSrv("info", 4, "uman", "Alexe");
	assert(test1.operator==(service.cautaSrv("info", "Alexe")));

	try {
		service.modificaSrv("mate", 4, "uman", "Ionescu");
	}
	catch (RepoException& mesaj) {
		assert(mesaj.get_mesaj() == "Disciplina nu exista!\n");
	}
}

void Tests::testStergeSrv() {
	Repo repo;
	Validator validator;
	Service service(repo, validator);
	Disciplina test{ "info", 3, "real", "Alexe" };
	Disciplina test1{ "info", 4, "uman", "Alexe" };

	service.adaugaSrv("info", 3, "real", "Alexe");

	const vector<Disciplina>& discipline = service.getAll();
	assert(discipline.size() == 1);
	service.stergeSrv("info", "Alexe");
	assert(discipline.size() == 0);

	try {
		service.stergeSrv("info", "Alexe");
	}
	catch (RepoException& mesaj) {
		assert(mesaj.get_mesaj() == "Disciplina nu exista!\n");
	}

}

void Tests::testSortare() {
	Repo repo;
	Validator validator;
	Service service(repo, validator);
	Disciplina test{ "info", 3, "real", "Alexe" };
	Disciplina test1{ "mate", 4, "uman", "Ionescu" };
	service.adaugaSrv("mate", 4, "uman", "Ionescu");
	service.adaugaSrv("info", 3, "real", "Alexe");

	vector<Disciplina> sortat_nume = service.sortDenumire();
	assert(test.operator == (sortat_nume[0]));
	assert(test1.operator == (sortat_nume[1]));

	vector<Disciplina> sortat_ore = service.sortOre();
	assert(test.operator == (sortat_ore[0]));
	assert(test1.operator == (sortat_ore[1]));

	vector<Disciplina> sortat_ProfTip = service.sortProfTip();
	assert(test.operator == (sortat_ProfTip[0]));
	assert(test1.operator == (sortat_ProfTip[1]));

	service.stergeSrv("info", "Alexe");
	vector<Disciplina> sortat= service.sortDenumire();
	assert(test1.operator==(sortat[0]));
	service.stergeSrv("mate", "Ionescu");

}

void Tests::testFiltrare() {
	Repo repo;
	Validator validator;
	Service service(repo, validator);
	Disciplina test{ "info", 3, "real", "Alexe" };
	Disciplina test1{ "mate", 4, "uman", "Ionescu" };

	service.adaugaSrv("info", 3, "real", "Alexe");
	service.adaugaSrv("mate", 4, "uman", "Ionescu");

	vector<Disciplina> filtrat_ore = service.filterOre(3);
	assert(test.operator==(filtrat_ore[0]));

	vector<Disciplina> filtrat_profesor = service.filterProfesor("Ionescu");
	assert(test1.operator==(filtrat_profesor[0]));

}