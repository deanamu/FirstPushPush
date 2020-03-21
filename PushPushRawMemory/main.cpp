#include <iostream> 
using namespace std;


char m[100];

enum Object {
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_GOAL,
	OBJ_MAN,
	OBJ_MAN_ON_GOAL,

	OBJ_UNKNOWN,
};

void initializeGlobalVariables();
void initialize();
void draw();
void update();
void checkClear();

int main() {
	initializeGlobalVariables();

	initialize(); 
	while (true) {
		draw();
		checkClear();
		if (m[0] == 1) {
			break; 
		}
		cout << "a:left s:right w:up z:down. command?" << endl; 
		cin >> m[0];
		update();
	}
	cout << "Congratulation! you win." << endl;

	while (true) {
		;
	}
	return 0;
}

void initialize() {
	m[0] = 0; 
	m[1] = 0;
	m[2] = 0;

	while (m[60 + m[0]] != '\0') {
		switch (m[60 + m[0]]) {
		case '#': m[3] = OBJ_WALL; break;
		case ' ': m[3] = OBJ_SPACE; break;
		case 'o': m[3] = OBJ_BLOCK; break;
		case 'O': m[3] = OBJ_BLOCK_ON_GOAL; break;
		case '.': m[3] = OBJ_GOAL; break;
		case 'p': m[3] = OBJ_MAN; break;
		case 'P': m[3] = OBJ_MAN_ON_GOAL; break;
		case '\n': m[1] = 0; ++m[2]; m[3] = OBJ_UNKNOWN; break; 
		default: m[3] = OBJ_UNKNOWN; break;
		}
		++m[0];
		if (m[3] != OBJ_UNKNOWN) { 
			m[20 + m[2] * m[18] + m[1]] = m[3];
			++m[1]; //++x
		}
	}
}

void draw() {
	for (m[0] = 0; m[0] < m[19]; ++m[0]) {
		for (m[1] = 0; m[1] < m[18]; ++m[1]) {
			m[2] = m[20 + m[0] * m[18] + m[1]];
			switch (m[2]) {
			case OBJ_SPACE: cout << ' '; break;
			case OBJ_WALL: cout << '#'; break;
			case OBJ_GOAL: cout << '.'; break;
			case OBJ_BLOCK: cout << 'o'; break;
			case OBJ_BLOCK_ON_GOAL: cout << 'O'; break;
			case OBJ_MAN: cout << 'p'; break;
			case OBJ_MAN_ON_GOAL: cout << 'P'; break;
			}
		}
		cout << endl;
	}
}

void update() {
	m[1] = 0;
	m[2] = 0;
	switch (m[0]) { 
	case 'a': m[1] = -1; break; 
	case 's': m[1] = 1; break; 
	case 'w': m[2] = -1; break; 
	case 'z': m[2] = 1; break; 
	}
	for (m[0] = 0; m[0] < m[18] * m[19]; ++m[0]) {
		if (m[20 + m[0]] == OBJ_MAN || m[20 + m[0]] == OBJ_MAN_ON_GOAL) {
			break;
		}
	}
	m[3] = m[0] % m[18]; 
	m[4] = m[0] / m[18]; 

	m[5] = m[3] + m[1]; 
	m[6] = m[4] + m[2]; 
	if (m[5] < 0 || m[6] < 0 || m[5] >= m[18] || m[6] >= m[19]) {
		return;
	}
	m[7] = 20 + m[4] * m[18] + m[3]; 
	m[8] = 20 + m[6] * m[18] + m[5];
	if (m[m[8]] == OBJ_SPACE || m[m[8]] == OBJ_GOAL) { 
		m[m[8]] = (m[m[8]] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN; 
		m[m[7]] = (m[m[7]] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE; 
	}
	else if (m[m[8]] == OBJ_BLOCK || m[m[8]] == OBJ_BLOCK_ON_GOAL) {
		m[9] = m[5] + m[1];
		m[10] = m[6] + m[2];
		if (m[9] < 0 || m[10] < 0 || m[9] >= m[18] || m[10] >= m[19]) { 
			return;
		}

		m[11] = 20 + (m[6] + m[2])*m[18] + (m[5] + m[1]); 
		if (m[m[11]] == OBJ_SPACE || m[m[11]] == OBJ_GOAL) {
			m[m[11]] = (m[m[11]] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			m[m[8]] = (m[m[8]] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			m[m[7]] = (m[m[7]] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}

void checkClear() {
	for (m[1] = 20; m[1] < 20 + m[18] * m[19]; ++m[1]) {
		if (m[m[1]] == OBJ_BLOCK) {
			m[0] = 0;
			return;
		}
	}
	m[0] = 1; 
	return;
}


//########\n\
//# .. p #\n\
//# oo   #\n\
//#      #\n\
//########";
void initializeGlobalVariables() {
	m[18] = 8;
	m[19] = 5;
	m[68] = '#';
	m[69] = ' ';
	m[70] = '.';
	m[71] = '.';
	m[72] = ' ';
	m[73] = 'p';
	m[74] = ' ';
	m[75] = '#';
	m[76] = '#';
	m[77] = ' ';
	m[78] = 'o';
	m[79] = 'o';
	m[80] = ' ';
	m[81] = ' ';
	m[82] = ' ';
	m[83] = '#';
	m[84] = '#';
	m[85] = ' ';
	m[86] = ' ';
	m[87] = ' ';
	m[88] = ' ';
	m[89] = ' ';
	m[90] = ' ';
	m[91] = '#';
	for (m[0] = 0; m[0] < m[18]; ++m[0]) {
		m[60 + m[0]] = '#';
		m[92 + m[0]] = '#';
	}
}
