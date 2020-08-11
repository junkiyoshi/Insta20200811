#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(39);
	ofSetLineWidth(2);

	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_MULTIPLY);

	ofColor color;
	vector<ofColor> color_base_list;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		color_base_list.push_back(color);
	}

	this->fbo.allocate(ofGetWidth(), ofGetHeight());
	this->fbo.begin();
	ofClear(0);

	ofTrueTypeFont font;
	font.loadFont("fonts/Kazesawa-bold.ttf", 320, true, true, true);
	string word = "AB";
	font.drawString(word, ofGetWidth() * 0.5 - font.stringWidth(word) * 0.5, ofGetHeight() * 0.5 - font.stringHeight(word) * 0.05);
	word = "CD";
	font.drawString(word, ofGetWidth() * 0.5 - font.stringWidth(word) * 0.5, ofGetHeight() * 0.5 + font.stringHeight(word) * 1.05);

	this->fbo.end();
	this->fbo.readToPixels(this->pixels);

	this->number_of_agent = 350;
	this->len_of_tail = 30;
	while (this->logs.size() < this->number_of_agent) {

		int x = ofRandom(ofGetWidth());
		int y = ofRandom(ofGetHeight());
		if (this->pixels.getColor(x, y) != ofColor(0, 0)) {

			deque<glm::vec2> log;
			log.push_front(glm::vec2(x, y));
			this->logs.push_back(log);
			this->directions.push_back(glm::normalize(glm::vec2(ofRandom(-1, 1), ofRandom(-1, 1))));
			this->colors.push_back(color_base_list[(int)ofRandom(color_base_list.size())]);
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	if (ofGetFrameNum() % 60 == 0) {

		while (true) {
		
			float tmp = ofRandom(1);
			if (abs(this->speed_param - tmp) > 0.25) {

				this->speed_param = tmp;
				break;
			}

		}
	}

	for (int i = 0; i < this->number_of_agent; i++) {

		glm::vec2 future = this->logs[i].front() + this->directions[i] * 5 * this->speed_param;
		int x = future.x;
		int y = future.y;

		while (this->pixels.getColor(x, y) == ofColor(0, 0)) {

			this->directions[i] = glm::normalize(glm::vec2(ofRandom(-1, 1), ofRandom(-1, 1)));

			future = this->logs[i].front() + this->directions[i] * 10 * this->speed_param;
			x = future.x;
			y = future.y;
		}

		this->logs[i].push_front(future);
		while (this->logs[i].size() > this->len_of_tail) {

			this->logs[i].pop_back();
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int i = 0; i < this->number_of_agent; i++) {

		ofSetColor(this->colors[i]);
		for (int l = 0; l < this->logs[i].size() - 1; l++) {

			ofDrawLine(this->logs[i][l], this->logs[i][l + 1]);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}