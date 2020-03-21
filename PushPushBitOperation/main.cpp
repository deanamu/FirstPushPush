#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

void readFile(char** buffer, int* size, const char* filename);

template< class T > class Array2D {
public:
	Array2D() : mArray(0) {}
	~Array2D() {
		delete[] mArray;
		mArray = 0;  
	}
	void setSize(int size0, int size1) {
		mSize0 = size0;
		mSize1 = size1;
		mArray = new T[size0 * size1];
	}
	T& operator()(int index0, int index1) {
		return mArray[index1 * mSize0 + index0];
	}
	const T& operator()(int index0, int index1) const {
		return mArray[index1 * mSize0 + index0];
	}
private:
	T* mArray;
	int mSize0;
	int mSize1;
};


class State {
public:
	State(const char* stageData, int size);
	void update(char input);
	void draw() const;
	bool hasCleared() const;
private:
	enum Object {
		OBJ_SPACE,
		OBJ_WALL,
		OBJ_BLOCK,
		OBJ_MAN,
		OBJ_UNKNOWN,

		OBJ_GOAL_FLAG = (1 << 7), 
	};
	void setSize(const char* stageData, int size);

	int mWidth;
	int mHeight;
	Array2D< unsigned char > mObjects; 
};

int main(int argc, char** argv) {
	const char* filename = "stageData.txt";
	if (argc >= 2) {
		filename = argv[1];
	}
	char* stageData;
	int fileSize;
	readFile(&stageData, &fileSize, filename);
	if (!stageData) {
		cout << "stage file could not be read." << endl;
		return 1;
	}
	State* state = new State(stageData, fileSize);

	while (true) {
		state->draw();
		if (state->hasCleared()) {
			break; 
		}
		cout << "a:left s:right w:up z:down. command?" << endl; 
		char input;
		cin >> input;
		state->update(input);
	}
	cout << "Congratulation's! you won." << endl;
	delete[] stageData;
	stageData = 0;

	while (true) {
		;
	}
	return 0;
}


void readFile(char** buffer, int* size, const char* filename) {
	ifstream in(filename);
	if (!in) {
		*buffer = 0;
		*size = 0;
	}
	else {
		in.seekg(0, ifstream::end);
		*size = static_cast<int>(in.tellg());
		in.seekg(0, ifstream::beg);
		*buffer = new char[*size];
		in.read(*buffer, *size);
	}
}

State::State(const char* stageData, int size) {
	setSize(stageData, size);
	mObjects.setSize(mWidth, mHeight);
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			mObjects(x, y) = OBJ_WALL; 
		}
	}
	int x = 0;
	int y = 0;
	for (int i = 0; i < size; ++i) {
		unsigned char t;
		switch (stageData[i]) {
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case 'O': t = OBJ_BLOCK | OBJ_GOAL_FLAG; break;
		case '.': t = OBJ_SPACE | OBJ_GOAL_FLAG; break;
		case 'p': t = OBJ_MAN; break;
		case 'P': t = OBJ_MAN | OBJ_GOAL_FLAG; break;
		case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; 
		default: t = OBJ_UNKNOWN; break;
		}
		if (t != OBJ_UNKNOWN) { 
			mObjects(x, y) = t; 
			++x;
		}
	}
}

void State::setSize(const char* stageData, int size) {
	mWidth = mHeight = 0; 
	int x = 0;
	int y = 0;
	for (int i = 0; i < size; ++i) {
		switch (stageData[i]) {
		case '#': case ' ': case 'o': case 'O':
		case '.': case 'p': case 'P':
			++x;
			break;
		case '\n':
			++y;
			mWidth = max(mWidth, x);
			mHeight = max(mHeight, y);
			x = 0;
			break;
		}
	}
}

void State::draw() const {
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			switch (mObjects(x, y)) {
			case (OBJ_SPACE | OBJ_GOAL_FLAG): cout << '.'; break;
			case (OBJ_WALL | OBJ_GOAL_FLAG): cout << '#'; break;
			case (OBJ_BLOCK | OBJ_GOAL_FLAG): cout << 'O'; break;
			case (OBJ_MAN | OBJ_GOAL_FLAG): cout << 'P'; break;
			case OBJ_SPACE: cout << ' '; break;
			case OBJ_WALL: cout << '#'; break;
			case OBJ_BLOCK: cout << 'o'; break;
			case OBJ_MAN: cout << 'p'; break;
			}
		}
		cout << endl;
	}
}

void State::update(char input) {
	int dx = 0;
	int dy = 0;
	switch (input) {
	case 'a': dx = -1; break; 
	case 's': dx = 1; break; 
	case 'w': dy = -1; break;
	case 'z': dy = 1; break; 
	}
	const int& w = mWidth;
	const int& h = mHeight;
	Array2D< unsigned char >& o = mObjects;
	int x, y;
	x = y = -1; 
	bool found = false;
	for (y = 0; y < h; ++y) {
		for (x = 0; x < w; ++x) {
			if ((o(x, y) & ~OBJ_GOAL_FLAG) == OBJ_MAN) { 
				found = true;
				break;
			}
		}
		if (found) {
			break;
		}
	}
	int tx = x + dx;
	int ty = y + dy;
	if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
		return;
	}
	if ((o(tx, ty) & ~OBJ_GOAL_FLAG) == OBJ_SPACE) {
		o(tx, ty) = (o(tx, ty) & OBJ_GOAL_FLAG) | OBJ_MAN; 
		o(x, y) = (o(x, y) & OBJ_GOAL_FLAG) | OBJ_SPACE;
	}
	else if (o(tx, ty) == OBJ_BLOCK) {
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) { 
			return;
		}
		if ((o(tx2, ty2) & ~OBJ_GOAL_FLAG) == OBJ_SPACE) {
			o(tx2, ty2) = (o(tx2, ty2) & OBJ_GOAL_FLAG) | OBJ_BLOCK;
			o(tx, ty) = (o(tx, ty) & OBJ_GOAL_FLAG) | OBJ_MAN;
			o(x, y) = (o(x, y) & OBJ_GOAL_FLAG) | OBJ_SPACE;
		}
	}
}

bool State::hasCleared() const {
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			if (mObjects(x, y) == OBJ_BLOCK) { 
				return false;
			}
		}
	}
	return true;
}

