#include <iostream>
using namespace std;

const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;

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

void initialize(Object* state, int w, int h, const char* stageData);
void draw(const Object* state, int w, int h);
void update(Object* state, char input, int w, int h);
bool checkClear(const Object* state, int w, int h);

int main() {
	Object* state = new Object[gStageWidth * gStageHeight]; 

	initialize(state, gStageWidth, gStageHeight, gStageData); 
	while (true) {
		draw(state, gStageWidth, gStageHeight);
		if (checkClear(state, gStageWidth, gStageHeight)) {
			break; //クリア?ェック
		}
		cout << "a:left s:right w:up z:down. command?" << endl; //?作説明
		char input;
		cin >> input;
		update(state, input, gStageWidth, gStageHeight);
	}
	cout << "Congratulation's! you won." << endl;
	delete[] state;
	state = 0;

	while (true) {
		;
	}
	return 0;
}



void initialize(Object* state, int width, int /* height */, const char* stageData) {
	const char* d = stageData; 
	int x = 0;
	int y = 0;
	while (*d != '\0') { 
		Object t;
		switch (*d) {
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case 'O': t = OBJ_BLOCK_ON_GOAL; break;
		case '.': t = OBJ_GOAL; break;
		case 'p': t = OBJ_MAN; break;
		case 'P': t = OBJ_MAN_ON_GOAL; break;
		case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; 
		default: t = OBJ_UNKNOWN; break;
		}
		++d;
		if (t != OBJ_UNKNOWN) { 
			state[y*width + x] = t;
			++x;
		}
	}
}

void draw(const Object* state, int width, int height) {
	const char font[] = { ' ', '#', '.', 'o', 'O', 'p', 'P' }; 
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Object o = state[y*width + x];
			cout << font[o];
		}
		cout << endl;
	}
}

void update(Object* s, char input, int w, int h) {
	int dx = 0;
	int dy = 0;
	switch (input) {
	case 'a': dx = -1; break;
	case 's': dx = 1; break; 
	case 'w': dy = -1; break;
	case 'z': dy = 1; break; 
	}
	int i = -1;
	for (i = 0; i < w * h; ++i) {
		if (s[i] == OBJ_MAN || s[i] == OBJ_MAN_ON_GOAL) {
			break;
		}
	}
	int x = i % w; 
	int y = i / w; 

	int tx = x + dx;
	int ty = y + dy;
	if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
		return;
	}
	int p = y * w + x; 
	int tp = ty * w + tx; 
	if (s[tp] == OBJ_SPACE || s[tp] == OBJ_GOAL) {
		s[tp] = (s[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN; 
		s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE; 
	}
	else if (s[tp] == OBJ_BLOCK || s[tp] == OBJ_BLOCK_ON_GOAL) {
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) { 
			return;
		}

		int tp2 = (ty + dy)*w + (tx + dx);
		if (s[tp2] == OBJ_SPACE || s[tp2] == OBJ_GOAL) {
			s[tp2] = (s[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			s[tp] = (s[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}

bool checkClear(const Object* s, int width, int height) {
	for (int i = 0; i < width*height; ++i) {
		if (s[i] == OBJ_BLOCK) {
			return false;
		}
	}
	return true;
}
